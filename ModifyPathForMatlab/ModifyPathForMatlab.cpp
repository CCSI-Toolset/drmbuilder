// CustomActionDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Msiquery.h"
#include <string>
#include "ShellApi.h"

#define MAX_KEY_LENGTH 255

int main()
{
	//called in commit phase
	int i;
	HKEY hKey;						// registry key
	HKEY hKey_matlab;				// registry key for matlab
	UINT broadcast_timeout = 100;	// timeout in 100 millisecond
	DWORD ikey;						// key index
	DWORD retCode;					// return code
	DWORD cbName;					// size of name string
	DWORD path_size;				// path string size
	DWORD_PTR result;				
	TCHAR* path_value;				// path string value
	TCHAR achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	std::wstring path_old;			// old path string
	std::wstring path_new;			// new path string
	std::wstring path_matlab;		// matlab bin path
	std::wstring path_matlab_engine;// matlab engine path
	FILETIME ftLastWriteTime;		// last write time 
	
	//first find the Matlab root directory. No warning message to display since it has been found during installation
	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\MathWorks\\MATLAB", 0, KEY_READ, &hKey_matlab);
	if (retCode!=ERROR_SUCCESS)
		return ERROR_INSTALL_FAILURE;
    // Enumerate the subkeys, until RegEnumKeyEx fails.
	ikey = 0;
    while (true)
	{
        cbName = MAX_KEY_LENGTH;
        retCode = RegEnumKeyEx(hKey_matlab, ikey, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
		if (retCode!=ERROR_SUCCESS)
			break;
		ikey++;
	}
	if (ikey<1)		//no version subkey found
		return ERROR_INSTALL_FAILURE;
	//achKey is now the key of the last version
	retCode = RegOpenKeyEx(hKey_matlab, achKey, 0, KEY_READ, &hKey);
	CloseHandle(hKey_matlab);
	retCode = RegQueryValueEx(hKey, L"MATLABROOT", 0, NULL, NULL, &path_size);
	if (retCode!=ERROR_SUCCESS)
		return ERROR_INSTALL_FAILURE;
	path_value = new TCHAR[path_size/2];
	retCode = RegQueryValueEx(hKey, L"MATLABROOT", 0, NULL, (LPBYTE)path_value, &path_size);
	path_matlab = path_value;
	path_matlab += L"\\bin";
	delete [] path_value;
	CloseHandle(hKey);
	//find the matlab engine folder, win32 only since only 32-bit MATLAB works with 32-bit ACM
	path_matlab_engine = path_matlab + L"\\win32";
	if (GetFileAttributes(path_matlab_engine.c_str())!=FILE_ATTRIBUTE_DIRECTORY)
	{
		MessageBoxW(NULL, L"32-bit MATLAB engine folder is not found in default location!\rPlease add the folder containing libeng.dll to environment variable PATH manually.", L"D-RM Builder Installer", MB_OK);
		return ERROR_FILE_NOT_FOUND;
	}
	//at this point, path_matlab and path_matlab_engine are both valid
	//now find the envrionment variable PATH
	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey);
	if (retCode!=ERROR_SUCCESS)
	{
		MessageBoxW(NULL, L"Cannot find environment variable PATH", L"D-RM Builder Installer", MB_OK);
		return ERROR_INSTALL_FAILURE;
	}
	//get the size of value first
	retCode = RegQueryValueEx(hKey, L"PATH", 0, NULL, NULL, &path_size);
	if (retCode!=ERROR_SUCCESS)		//PATH does not exist
	{
		MessageBoxW(NULL, L"Cannot find environment variable PATH", L"D-RM Builder Installer", MB_OK);
		return ERROR_INSTALL_FAILURE;
	}
	path_value = new TCHAR[path_size/2];
	retCode = RegQueryValueEx(hKey, L"PATH", 0, NULL, (LPBYTE)path_value, &path_size);
	path_old = path_value;
	delete [] path_value;
	//find the matlab
	i = path_old.find(path_matlab);
	if (i<0)	//MATLAB bin not found, this is unlikely
	{
		//check if the last character is ';'
		i = path_old.length();
		if (path_old[i-1]==L';')
			path_new = path_old + path_matlab + L";" + path_matlab_engine;
		else
			path_new = path_old + L";" + path_matlab + L";"+ path_matlab_engine;
		path_size = path_new.length();
		MessageBoxW(NULL, L"MATLAB bin and engine folders will be added to local machine\'s environment varialbe PATH.", L"D-RM Builder Installer", MB_OK);
		retCode = RegSetValueEx(hKey,L"PATH",0,REG_SZ,(LPBYTE)path_new.c_str(),path_size*2);
		if (retCode!=ERROR_SUCCESS)
		{
			MessageBoxW(NULL, L"Failed to append MATLAB bin and engine folders to environment variable PATH.\rPlease add MATLAB bin and engine folders to PATH manually.", L"D-RM Builder Installer", MB_OK);
			return ERROR_INSTALL_FAILURE;
		}
		CloseHandle(hKey);
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, (LPARAM)L"Environment", SMTO_NORMAL, broadcast_timeout, &result);
	}
	else
	{
		i = path_old.find(path_matlab_engine);
		if (i<0)		//MATLAB engine not in PATH, append path_matlab_engine to PATH
		{
			//check if the last character is ';'
			i = path_old.length();
			if (path_old[i-1]==L';')
				path_new = path_old + path_matlab_engine;
			else
				path_new = path_old + L";" + path_matlab_engine;
			path_size = path_new.length();
			MessageBoxW(NULL, L"MATLAB engine directory will be added to local machine\'s environment varialbe PATH.", L"D-RM Builder Installer", MB_OK);
			retCode = RegSetValueEx(hKey,L"PATH",0,REG_SZ,(LPBYTE)path_new.c_str(),path_size*2);
			if (retCode!=ERROR_SUCCESS)
			{
				MessageBoxW(NULL, L"Failed to append MATLAB engine folder to environment variable PATH.\rPlease add MATLAB engine folder to PATH manually.", L"D-RM Builder Installer", MB_OK);
				return ERROR_INSTALL_FAILURE;
			}
			CloseHandle(hKey);
			SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, (LPARAM)L"Environment", SMTO_NORMAL, broadcast_timeout, &result);
		}
	}
	//now register matlab server
	//check if MATLAB engine server is registered by checking a class key MATLAB.AutoServer.Single
	retCode = RegOpenKeyEx(HKEY_CLASSES_ROOT, L"MATLAB.AutoServer.Single", 0, KEY_READ, &hKey_matlab);
	if (retCode!=ERROR_SUCCESS)		//server not registered
	{
		SHELLEXECUTEINFO shExecInfo = {0};
		shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		shExecInfo.hwnd = NULL;
		shExecInfo.lpVerb = NULL;
		shExecInfo.lpFile = L"matlab";
		shExecInfo.lpParameters = L"/regserver -r exit";
		shExecInfo.lpDirectory = NULL;
		shExecInfo.nShow = SW_HIDE;
		shExecInfo.hInstApp = NULL;
		ShellExecuteEx(&shExecInfo);
		WaitForSingleObject(shExecInfo.hProcess,INFINITE);
	}
	return ERROR_SUCCESS;
}
