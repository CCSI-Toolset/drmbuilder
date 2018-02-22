//ROMManager.cpp

#include "stdafx.h"
#include <cmath>
#include "DRMManager.h"
#include "DRMBuilder.h"
#include "Simplex.h"
#include "Engine.h"

CDRMManager::CDRMManager()
{
	bhfm_file = false;
	bmdl_file = false;
	basnp_file = false;
	binputready = false;
	boutputready = false;
	bsequence_train = false;
	bsequence_valid = false;
	brun_train = false;
	brun_valid = false;
	bbuilt_dabnet = false;
	bbuilt_narma = false;
	bpredict_train = false;
	bpredict_valid = false;
	bnoise_set = false;
	brun_uq = false;
	bthread = false;
	bplot_error = TRUE;
	bplot_step_change = TRUE;
	bplot_correlation = TRUE;
	pthread = NULL;
	time_unit_acm = "hour";
}

CDRMManager::~CDRMManager()
{
}

bool CDRMManager::FindSnapShotValue(CString var_name, double& val)
{
	//return true if found the value corresponding the variable name
	//var_name:  variable name including scope dot operator
	//pval: value to be returned as reference
	//split dot separated var_name to substrings
	bool bfound = false;
	bool bwithin[10] = {true};
	CString cstr1, cstr2;
	CString str_names[10];	//assume maximum sub model layer of 10
	int i, j;
	int ilevel;				//variable level, 0 is the outer most level (no dot operator)
	int inewline0;			//previous newline character location
	int inewline1;			//current newline character location
	i = 0;
	do
	{
		j = var_name.Find('.');
		if (j<0)	//not found
		{
			str_names[i] = var_name;
			ilevel = i;
			break;
		}
		str_names[i++] = var_name.Left(j);
		var_name = var_name.Mid(j+1);
	}while (true);
	i = 0;
	inewline0 = 0;
	do
	{
		inewline1 = cstring_asnp.Find('\n',inewline0);
		if (inewline1==-1)
			break;
		cstr1 = cstring_asnp.Mid(inewline0,inewline1-inewline0-1);
		inewline0 = inewline1 + 1;
		cstr2 = cstr1.Left(6);
		if (cstr2=="WITHIN")
		{
			i++;
			if (bwithin[i-1])
			{
				cstr2 = cstr1.Mid(7);
				cstr2.TrimRight();
				bwithin[i] = cstr2==str_names[i-1];
			}
			else
				bwithin[i] = false;
		}
		else
		{
			cstr2 = cstr1.Left(9);
			if (cstr2=="ENDWITHIN")
			   i--;
			else
			{
				if (bwithin[i] && i==ilevel)		//in the same level as the variable
				{
					//find :
					j = cstr1.Find(':');
					if (j>0)
					{
						cstr2 = cstr1.Left(j);
						if (cstr2==str_names[ilevel])
						{
							cstr1 = cstr1.Mid(j+1);
							sscanf(cstr1,"%lg",&val);
							bfound = true;
							break;
						}
					}
				}
			}
		}
	}while (true);
	return bfound;
}

void CDRMManager::SetAllAvailableInputOutputList()
{
	//parse the dml file to get the input and output variable names
	//also find the sampling time interval and assign it to si_train
	bool bfound;
	int i, j, k;
	int nlength;
	int ninput_all;
	int noutput_all;
	double xdefault;
	double xlower;
	double xupper;
	CString pathname_acm_old;
	CString cstr1;
	CString cstr2;
	CString cstr3;
	CInputVariable var_in;
	COutputVariable var_out;
	input_list_all.clear();
	output_list_all.clear();
	//find time unit in acm file
	i = cstring_acm.Find("TimeSettings.CommunicationUnits:");
	if (i==-1)		//not found, default time is hour
		time_unit_acm = "hour";
	else
	{
		j = cstring_acm.Find('\"',i);
		i = cstring_acm.Find('\"',j+1);
		cstr1 = cstring_acm.Mid(j+1,i-j-2);		//remove 's'
		time_unit_acm = cstr1.MakeLower();
	}
	//find miminum time step size in acm file
	i = cstring_acm.Find("Integration.MinStepSize:");
	if (i==-1)		//not foundde, fault time step is 0.001
		xdefault = 0.001;
	else
	{
		j = cstring_acm.Find(':',i);
		i = cstring_acm.Find(';',j);
		cstr1 = cstring_acm.Mid(j+1,i-j-1);
		sscanf(cstr1,"%lg",&xdefault);
	}
	si_train.SetSolverMinimumTimeStep(xdefault);
	si_valid.SetSolverMinimumTimeStep(xdefault);
	//find sampling time interval in Simulink mdl file
	i = cstring_mdl.Find(" FixedStep");
	if (i==-1)		//not foundde, fault time step is 0.1
		xdefault = 0.1;
	else
	{
		j = cstring_mdl.Find('\"',i);
		i = cstring_mdl.Find('\"',j+1);
		cstr1 = cstring_mdl.Mid(j+1,i-j-1);
		sscanf(cstr1,"%lg",&xdefault);
	}
	si_train.SetSamplingTimeInterval(xdefault);
	si_valid.SetSamplingTimeInterval(xdefault);
	if (si_train.GetSamplingTimeInterval() < 5*si_train.GetSolverMinimumTimeStep())
		AfxMessageBox("Sampling time interval is less than 5 times of solver minimum time step!\rPlease consider adjusting at least one of them.");
	//find input/output variables in simulink mdl file
	i = cstring_mdl.Find("\"AMSimulation\"");
	if (i==-1)
		AfxMessageBox("The mdl file does not contain ACM custom block!");
	else
	{
		j = cstring_mdl.Find("Parameters",i);
		i = cstring_mdl.Find('\"',j);
		j = cstring_mdl.Find('\n',i+1);
		cstr1 = cstring_mdl.Mid(i+1,j-i-1);	//cstr1 is the first parameter line
		k = cstr1.ReverseFind('\"');
		cstr1 = cstr1.Left(k);
		i = cstring_mdl.Find('}',j+1);
		cstr2 = cstring_mdl.Mid(j+1,i-j-1);	//cstr2 contains other lines
		j = -1;
		do
		{
			i = cstr2.Find('\"',j+1);
			if (i==-1)		//no more parameter line enclosed by "..."
				break;
			j = cstr2.Find('\n',i+1);
			cstr3 = cstr2.Mid(i+1,j-i-1);
			k = cstr3.ReverseFind('\"');
			cstr3 = cstr3.Left(k);
			cstr1 += cstr3;
		}while (true);
		//process cstr1 to find parameter values
		//get acmf file name
		nlength = cstr1.GetLength();
		i = cstr1.Find(',');
		pathname_acm_old = cstr1.Mid(1,i-2);
		j = pathname_acm_old.ReverseFind('\\');
		cstr2 = pathname_acm_old.Mid(j+1);
		if (cstr2.MakeLower()!=filename_acm.MakeLower())
		{
			AfxMessageBox("ACM file name in Simulink mdl file does not match user specified ACM file name!");
			bhfm_file = false;
			basnp_file = false;
			bmdl_file = false;
			return;
		}
		cstr1 = cstr1.Right(nlength-i-1);
		//skip ACM Application
		nlength = cstr1.GetLength();
		i = cstr1.Find(',');
		cstr1 = cstr1.Right(nlength-i-1);
		//now read the number of inputs
		sscanf(cstr1,"%d",&ninput_all);
		//remove number of inputs
		nlength = cstr1.GetLength();
		i = cstr1.Find(',');
		cstr1 = cstr1.Right(nlength-i-2);
		//process input variables
		for (j=0; j<ninput_all; j++)
		{
			//need to consider non-dimensional quantities
			nlength = cstr1.GetLength();
			if (j<ninput_all-1)
				i = cstr1.Find("\\\\");
			else
				i = cstr1.Find("\',");
			cstr2 = cstr1.Left(i);
			cstr1 = cstr1.Right(nlength-i-2);
			//cstr2 contains name and possiblly unit
			nlength = cstr2.GetLength();
			i = cstr2.Find('|');
			if (i!=-1)	//found
			{
				cstr3 = cstr2.Right(nlength-i-1);
				cstr2 = cstr2.Left(i);
			}
			else
				cstr3 = " ";		//use space if without unit
			//replace \" by " if any
			cstr2.Replace("\\\"","\"");
			var_in.name = cstr2;
			var_in.unit = cstr3;
			//use substring after lasts dot of variable name
			i = cstr2.ReverseFind('.');
			if (i>0)
				var_in.desc = cstr2.Mid(i+1);
			else
				var_in.desc = cstr2;
			var_in.bvaried = true;
			//now cstr2 is the name of the variable
			//parse snap shot file to find the steady state value
			bfound = FindSnapShotValue(cstr2, xdefault);
			if (bfound)
			{
				if (xdefault>0)
				{
					xlower = 0.9*xdefault;
					xupper = 1.1*xdefault;
				}
				else
				{
					xlower = 1.1*xdefault;
					xupper = 0.9*xdefault;
				}
				var_in.xdefault = xdefault;
				var_in.xlower = xlower;
				var_in.xupper = xupper;
				//set default ramp rate to the value corresponding to 4 solver integration steps from minimum to maximum
				var_in.ramp_rate = (xupper-xlower)/si_train.GetSolverMinimumTimeStep()/4;
			}
			else
			{
				var_in.xdefault = 0;
				var_in.xlower = 0;
				var_in.xupper = 0;
				var_in.ramp_rate = 0;
				AfxMessageBox("Steady state value of input variable \"" + cstr2 + "\" is not found in ACM snap shot file.\n\rPlease make sure the snap shot file matches the ACM model.");
			}
			//finally add variable to input list
			input_list_all.push_back(var_in);
		}
		//now read the number of outputs
		sscanf(cstr1,"%d",&noutput_all);
		//remove number of outputs
		nlength = cstr1.GetLength();
		i = cstr1.Find(',');
		cstr1 = cstr1.Right(nlength-i-2);
		//process output variable
		for (j=0; j<noutput_all; j++)
		{
			//need to consider non-dimensional quantities
			nlength = cstr1.GetLength();
			if (j<noutput_all-1)
				i = cstr1.Find("\\\\");
			else
				i = cstr1.Find("\',");
			cstr2 = cstr1.Left(i);
			cstr1 = cstr1.Right(nlength-i-2);
			//cstr2 contains name and possiblly unit
			nlength = cstr2.GetLength();
			i = cstr2.Find('|');
			if (i!=-1)	//found
			{
				cstr3 = cstr2.Right(nlength-i-1);
				cstr2 = cstr2.Left(i);
			}
			else
				cstr3 = " ";		//use space if without unit
			//replace \" by " if any
			cstr2.Replace("\\\"","\"");
			var_out.name = cstr2;
			var_out.unit = cstr3;
			//use substring after lasts dot of variable name
			i = cstr2.ReverseFind('.');
			if (i>0)
				var_out.desc = cstr2.Mid(i+1);
			else
				var_out.desc = cstr2;
			var_out.bvaried = true;
			var_out.xdefault = 0;
			output_list_all.push_back(var_out);
		}	//end of for
	}	//end of else
	//replace the old acm path name with the current path name
	cstring_mdl.Replace(pathname_acm_old,"<<ACM_FILE_PATH>>");
}

void CDRMManager::UpdateSelectedInputList()
{
	bool bvaried;
	std::list<CInputVariable>::iterator ite;
	input_list_sel.clear();
	drm_container.binput_list.clear();
	for (ite=input_list_all.begin(); ite!=input_list_all.end(); ite++)
	{
		bvaried = (*ite).bvaried;
		if (bvaried)
			input_list_sel.push_back(*ite);
		drm_container.binput_list.push_back(bvaried);
	}
	si_train.SetAllInputVariableList(&input_list_all);
	si_valid.SetAllInputVariableList(&input_list_all);
	si_train.SetSelectedInputVariableList(&input_list_sel);
	si_valid.SetSelectedInputVariableList(&input_list_sel);
	drm_container.SetNumberOfAllInputs(input_list_all.size());
	drm_container.SetNumberOfInputs(input_list_sel.size());
	if (boutputready)
	{
		drm_container.InitDabnet();
		drm_container.InitNarma();
	}
}

void CDRMManager::UpdateSelectedOutputList()
{
	bool bvaried;
	std::list<COutputVariable>::iterator ite;
	output_list_sel.clear();
	drm_container.boutput_list.clear();
	for (ite=output_list_all.begin(); ite!=output_list_all.end(); ite++)
	{
		bvaried = (*ite).bvaried;
		if (bvaried)
			output_list_sel.push_back(*ite);
		drm_container.boutput_list.push_back(bvaried);
	}
	drm_container.SetNumberOfAllOutputs(output_list_all.size());
	drm_container.SetNumberOfOutputs(output_list_sel.size());
	//initialize D-RM models inside the container since numbers of input and output variables are fixed now
	drm_container.InitDabnet();
	drm_container.InitNarma();
}

void CDRMManager::PrepareTrainingSequence()
{
	srand(0);	//prevent randomness for training sequence
	si_train.GenerateTrainingSequence();
	si_train.PrepareSequenceVectorForACMInputArray();
}

void CDRMManager::PrepareValidationSequence()
{
	srand(1);	//prevent randomness for validation sequence
	si_valid.GenerateTrainingSequence();
	si_valid.PrepareSequenceVectorForACMInputArray();
}

void CDRMManager::SetMDLSamplingTimeInterval(double dt)
{
	//check the time interval in cstring_mdl and overwrite it if sampling time in the current file is different from dt
	//dt: new sampling time interval
	int i, j;
	double dt_old;			//old dt in current cstring_mdl
	CString str;
	i = cstring_mdl.Find(" FixedStep");
	if (i!=-1)		//found
	{
		j = cstring_mdl.Find('\"',i+1);
		i = cstring_mdl.Find('\"',j+1);
		str = cstring_mdl.Mid(j+1,i-j-1);
		sscanf(str,"%lg",&dt_old);
		if ((float)dt!=(float)dt_old)
		{
			str.Format("%lg",dt);
			str = cstring_mdl.Left(j+1) + str;
			cstring_mdl = str + cstring_mdl.Mid(i);
		}
	}
}

void CDRMManager::RunMatlabHighFidelityModel(int iseq)
{
	//Call Matlab Engine to plot high-fidelity and DRM model results
	//iseq:  step change sequence type, 0=training, 1=validation
	//note the MATLAB 2-D array is row major, the same as Fortran
	CString str_matlab;				//MATLAB command string;
	Engine *ep;						//MATLAB enginer handle
	mxArray *mx_input = NULL;		//MATLAB training/validation sequence input data
	mxArray *mx_output = NULL;		//MATLAB training/validation response output data
	int ninput_all;					//number of all input variables in mdl file
	int noutput_all;				//number of all output variables in mdl file
	int nstep;						//number of training steps or number of step or ramp changes
	double* pinput;					//array pointer for input
	double* poutput;				//array pointer for output
	//use engOpen() rather than engOpenSingleUse().  It does not cause the closing of D-RM Builder's main window
	ep = engOpen(NULL);
	if (ep==NULL)
	{
		AfxMessageBox("Initializing MATLAB engine failed!");
		return;
	}
	//copy temporary files to current working directory
	CopyMatlabScriptFilesToWorkingDirectory();
	WriteSimulinkAndAcmFilesToWorkingDirectory();
	ninput_all = input_list_all.size();
	noutput_all = output_list_all.size();
	if (iseq==0)		//training
	{
		nstep = si_train.GetNumberOfTrainingSteps();
		pinput = si_train.GetSequenceVectorAsMemoryPointer();
	}
	else				//validation
	{
		nstep = si_valid.GetNumberOfTrainingSteps();
		pinput = si_valid.GetSequenceVectorAsMemoryPointer();
	}
	mx_input = mxCreateDoubleMatrix(ninput_all, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_input), (char*)pinput, nstep*ninput_all*sizeof(double));
	engPutVariable(ep, "u", mx_input);
	engEvalString(ep, command_param);
	str_matlab = "run('" + foldername + "\\ProcessCommand.m')";
	engEvalString(ep, str_matlab);
	mx_output = engGetVariable(ep, "y");
	if (mx_output == NULL)
	{
		AfxMessageBox("Failed to get MATLAB output array");
		engClose(ep);
		mxDestroyArray(mx_input);
	}
	else
	{
		poutput = mxGetPr(mx_output);
		//assign input and output data to tr_data or vd_data
		if (iseq==0)		//training
		{
			drm_container.tr_data.SetSize(nstep, ninput_all, noutput_all);
			drm_container.tr_data.AssignInputOutputData(pinput,poutput);
		}
		else				//validation
		{
			drm_container.vd_data.SetSize(nstep, ninput_all, noutput_all);
			drm_container.vd_data.AssignInputOutputData(pinput,poutput);
		}
		AfxMessageBox("Simulation completed successfully.\rPlease click OK button to close MATLAB window.",MB_ICONINFORMATION);
		engEvalString(ep, "exit");		//required to issue exit command to close ACM window properly
		engClose(ep);
		mxDestroyArray(mx_input);
		mxDestroyArray(mx_output);
	}
	DeleteSimulinkAndAcmFilesFromWorkingDirectory();
	DeleteMatlabScriptFilesFromWorkingDirectory(false);
	DeleteACMTemporaryyFolder();
}

void CDRMManager::RunMatlabPlotScript(int iseq)
{
	//Call Matlab Engine to launch high-fidelity simulation
	//iseq:  step change sequence type, 0=training, 1=validation
	//note the MATLAB 2-D array is row major, the same as Fortran
	CString str_matlab;				//MATLAB command string;
	Engine *ep;						//MATLAB enginer handle
	mxArray *mx_input = NULL;		//MATLAB training sequence input data
	mxArray *mx_output = NULL;		//MATLAB training response output data
	mxArray *mx_output_drm = NULL;	//MATLAB training/validation response output data predicted by DRM
	mxArray *mx_r2 = NULL;			//MATLAB R^2 for fitting
	int ninput_all;					//number of all input variables in mdl file
	int noutput_all;				//number of all output variables in mdl file
	int noutput;					//number of varied output variables
	int nstep;						//number of training steps or number of step or ramp changes
	double* pinput;					//array pointer for input
	double* poutput;				//array pointer for output
	double* poutput_drm;			//array pointer for output predicted by DRM
	/* It was found that engOpenSingleUse() could cause main window closed after the application is inactive for a long time (in MFC wincore.cpp)
	if (!(ep = engOpenSingleUse(NULL,NULL,&status)))
	{
		AfxMessageBox("Initializing MATLAB engine failed!");
		return;
	}
	if (status!=0)
	{
		AfxMessageBox("Failed to open MATLAB engine!");
		return;
	}
	*/
	//engOpen() does not cause the closing of D-RM Builder's main window
	ep = engOpen(NULL);
	if (ep==NULL)
	{
		AfxMessageBox("Initializing MATLAB engine failed!");
		return;
	}

	//copy temporary Matlab script files
	CopyMatlabScriptFilesToWorkingDirectory();
	ninput_all = input_list_all.size();
	noutput_all = output_list_all.size();
	noutput = output_list_sel.size();
	if (iseq==0)		//training
	{
		nstep = si_train.GetNumberOfTrainingSteps();
		pinput = si_train.GetSequenceVectorAsMemoryPointer();
		poutput = new double[nstep*noutput_all];
		poutput_drm = new double[nstep*noutput];
		drm_container.GetOutputDataAsMatlabArray(&drm_container.tr_data,poutput);
		drm_container.GetOutputDataAsMatlabArray(&drm_container.tr_data_drm,poutput_drm);
		//R^2 vector
		mx_r2 = mxCreateDoubleMatrix(noutput, 1, mxREAL);
		memcpy((char*)mxGetPr(mx_r2), (char*)&drm_container.tr_r2_drm[0], noutput*sizeof(double));
		engPutVariable(ep, "R_squared", mx_r2);
	}
	else				//validation
	{
		nstep = si_valid.GetNumberOfTrainingSteps();
		pinput = si_valid.GetSequenceVectorAsMemoryPointer();
		poutput = new double[nstep*noutput_all];
		poutput_drm = new double[nstep*noutput];
		drm_container.GetOutputDataAsMatlabArray(&drm_container.vd_data,poutput);
		drm_container.GetOutputDataAsMatlabArray(&drm_container.vd_data_drm,poutput_drm);
		//R^2 vector
		mx_r2 = mxCreateDoubleMatrix(noutput, 1, mxREAL);
		memcpy((char*)mxGetPr(mx_r2), (char*)&drm_container.vd_r2_drm[0], noutput*sizeof(double));
		engPutVariable(ep, "R_squared", mx_r2);
	}
	mx_input = mxCreateDoubleMatrix(ninput_all, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_input), (char*)pinput, nstep*ninput_all*sizeof(double));
	engPutVariable(ep, "u", mx_input);
	mx_output = mxCreateDoubleMatrix(noutput_all, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_output), (char*)poutput, nstep*noutput_all*sizeof(double));
	engPutVariable(ep, "y", mx_output);
	mx_output_drm = mxCreateDoubleMatrix(noutput, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_output_drm), (char*)poutput_drm, nstep*noutput*sizeof(double));
	engPutVariable(ep, "ym_", mx_output_drm);
	engEvalString(ep, command_param);
	str_matlab.Format("bplot_error = %d;",bplot_error);
	engEvalString(ep, str_matlab);
	str_matlab.Format("bplot_step_change = %d;",bplot_step_change);
	engEvalString(ep, str_matlab);
	str_matlab.Format("bplot_correlation = %d;",bplot_correlation);
	engEvalString(ep, str_matlab);
	str_matlab = "run(\'" + foldername + "\\ProcessCommand.m\')";
	engEvalString(ep, str_matlab);
	AfxMessageBox("MATLAB plotting commands completed succesfully.\rPlease click OK button to close MATLAB window.",MB_ICONINFORMATION);
	engClose(ep);
	mxDestroyArray(mx_input);
	mxDestroyArray(mx_output);
	mxDestroyArray(mx_output_drm);
	mxDestroyArray(mx_r2);
	delete [] poutput;
	delete [] poutput_drm;
	//delete Matlab script files
	DeleteMatlabScriptFilesFromWorkingDirectory(false);
}

void CDRMManager::RunMatlabUQScript()
{
	//Call Matlab Engine to calculate uncertainty quantification based on UKF
	//iseq:  step change sequence type, 0=training, 1=validation
	//note the MATLAB 2-D array is row major, the same as Fortran
	//first check if the model is DABNet
	if (drm_container.imodel_type!=0)
	{
		AfxMessageBox("Only DABNet model can be performed for UQ!");
		return;
	}
	//find "models" folder under installed D-RMBuilder application root directory
	char szAppPath[MAX_PATH];
	CString strModelPath;
	CString strUkfPath;
	::GetModuleFileName(0, szAppPath, MAX_PATH);
	strModelPath = szAppPath;
	strModelPath = strModelPath.Left(strModelPath.ReverseFind('\\'));
	strModelPath = strModelPath.Left(strModelPath.ReverseFind('\\'));
	strUkfPath = strModelPath + "\\matlab_files\\ukf_toolbox";
	strModelPath += "\\matlab_files\\drm_models";
	//write DRMParameters.m for DABNet only
	FILE* pf = fopen("DRMParameters.m","w");
	if (pf==NULL)
	{
		AfxMessageBox("Cannot write \"DRMParameters.m\" to the working directory!");
		return;
	}
	WriteDRMMatlabFile(pf);
	//add path to the script
	fprintf(pf,"addpath(\'%s\');\n",strModelPath);
	fprintf(pf,"addpath(\'%s\');\n",strUkfPath);
	fclose(pf);
	CString str_matlab;					//MATLAB command string;
	Engine *ep;							//MATLAB enginer handle
	mxArray *mx_input = NULL;			//MATLAB training sequence input data
	mxArray *mx_output = NULL;			//MATLAB training response output data
	mxArray *mx_output_drm = NULL;		//MATLAB training/validation response output data predicted by DRM
	mxArray *mx_fr_output = NULL;		//MATLAB variable for diagonal element of R matrix
	mxArray *mx_cov_process = NULL;		//MATLAB process noise covariance matrix
	mxArray *mx_cov_measurement = NULL;	//MATLAB measurement noise covariance matrix
	mxArray *mx_cov_state = NULL;		//MATLAB state covariance matrix
	mxArray *mx_cov_output = NULL;		//MATLAB output covariance matrix
	mxArray *mx_r2_drm = NULL;			//MATLAB R^2 for drm data, provided to MATLAB script
	mxArray *mx_r2_ukf = NULL;			//MATLAB R^2 for ukf data, returned from MATLAB script
	int i, j;
	int ninput_all;						//number of all input variables in mdl file
	int noutput_all;					//number of all output variables in mdl file
	int noutput;						//number of varied output variables
	int nstep;							//number of training steps or number of step or ramp changes
	int nstate;							//number of state-space variables
	double* pinput;						//array pointer for input
	double* poutput;					//array pointer for output
	double* poutput_drm;				//array pointer for output predicted by DRM
	double* presult;					//pointer to results returned by MATLAB
	//use engOpen() rather than engOpenSingleUse().  It does not cause the closing of D-RM Builder's main window
	ep = engOpen(NULL);
	if (ep==NULL)
	{
		AfxMessageBox("Initializing MATLAB engine failed!");
		return;
	}
	//copy temporary Matlab script files
	CopyMatlabScriptFilesToWorkingDirectory();
	ninput_all = input_list_all.size();
	noutput_all = output_list_all.size();
	noutput = output_list_sel.size();
	//using validation sequence
	nstep = si_valid.GetNumberOfTrainingSteps();
	pinput = si_valid.GetSequenceVectorAsMemoryPointer();
	poutput = new double[nstep*noutput_all];
	poutput_drm = new double[nstep*noutput];
	drm_container.GetOutputDataAsMatlabArray(&drm_container.vd_data,poutput);
	drm_container.GetOutputDataAsMatlabArray(&drm_container.vd_data_drm,poutput_drm);
	//memory copy the data to mxArrays
	mx_input = mxCreateDoubleMatrix(ninput_all, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_input), (char*)pinput, nstep*ninput_all*sizeof(double));
	engPutVariable(ep, "u", mx_input);
	mx_output = mxCreateDoubleMatrix(noutput_all, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_output), (char*)poutput, nstep*noutput_all*sizeof(double));
	engPutVariable(ep, "y_acm", mx_output);
	mx_output_drm = mxCreateDoubleMatrix(noutput, nstep, mxREAL);
	memcpy((char*)mxGetPr(mx_output_drm), (char*)poutput_drm, nstep*noutput*sizeof(double));
	engPutVariable(ep, "y_drm", mx_output_drm);
	mx_r2_drm = mxCreateDoubleMatrix(noutput, 1, mxREAL);
	memcpy((char*)mxGetPr(mx_r2_drm), (char*)&drm_container.vd_r2_drm[0], noutput*sizeof(double));
	engPutVariable(ep, "r2_drm", mx_r2_drm);
	mx_fr_output = mxCreateDoubleMatrix(noutput,1,mxREAL);
	memcpy((char*)mxGetPr(mx_fr_output), (char*)&drm_container.fr_output[0], noutput*sizeof(double));
	engPutVariable(ep, "fr_output", mx_fr_output);
	engEvalString(ep, command_param);
	str_matlab.Format("fq_state = %lg;", drm_container.dabnet_input.fq_state);
	engEvalString(ep, str_matlab);
	str_matlab.Format("bplot_error = %d;",bplot_error);
	engEvalString(ep, str_matlab);
	str_matlab.Format("bplot_step_change = %d;",bplot_step_change);
	engEvalString(ep, str_matlab);
	str_matlab.Format("bplot_correlation = %d;",bplot_correlation);
	engEvalString(ep, str_matlab);
	str_matlab = "run(\'" + foldername + "\\DRMParameters.m\')";
	engEvalString(ep, str_matlab);
	str_matlab = "run(\'" + foldername + "\\ProcessCommand.m\')";
	engEvalString(ep, str_matlab);
	mx_cov_process = engGetVariable(ep, "Q");
	mx_cov_measurement = engGetVariable(ep, "R");
	mx_cov_state = engGetVariable(ep, "Pk1");
	mx_cov_output = engGetVariable(ep, "Sk1");
	mx_r2_ukf = engGetVariable(ep, "r2_ukf");
	brun_uq = true;
	if (mx_cov_process == NULL)
	{
		AfxMessageBox("Failed to get MATLAB process noise covariance matrix");
		brun_uq = false;
	}
	else
	{
		presult = mxGetPr(mx_cov_process);
		nstate = mxGetN(mx_cov_process);
		drm_container.cov_process.resize(nstate*nstate);
		//save process noise covariance matrix, switch to column major from row major
		for (i=0; i<nstate; i++)
		{
			for (j=0; j<nstate; j++)
				drm_container.cov_process[i*nstate+j] = presult[j*nstate+i];
		}
		mxDestroyArray(mx_cov_process);
	}
	if (mx_cov_measurement == NULL)
	{
		AfxMessageBox("Failed to get MATLAB measurement noise covariance matrix");
		brun_uq = false;
	}
	else
	{
		presult = mxGetPr(mx_cov_measurement);
		drm_container.cov_measurement.resize(noutput*noutput);
		//save measurement noise covariance matrix, switch to column major from row major
		for (i=0; i<noutput; i++)
		{
			for (j=0; j<noutput; j++)
				drm_container.cov_measurement[i*noutput+j] = presult[j*noutput+i];
		}
		mxDestroyArray(mx_cov_measurement);
	}
	if (mx_cov_state == NULL)
	{
		AfxMessageBox("Failed to get MATLAB state covariance matrix");
		brun_uq = false;
	}
	else
	{
		presult = mxGetPr(mx_cov_state);
		nstate = mxGetN(mx_cov_state);
		drm_container.cov_state.resize(nstate*nstate);
		//save state covariance matrix, switch to column major from row major
		for (i=0; i<nstate; i++)
		{
			for (j=0; j<nstate; j++)
				drm_container.cov_state[i*nstate+j] = presult[j*nstate+i];
		}
		mxDestroyArray(mx_cov_state);
	}
	if (mx_cov_output == NULL)
	{
		AfxMessageBox("Failed to get MATLAB output covariance matrix");
		brun_uq = false;
	}
	else
	{
		presult = mxGetPr(mx_cov_output);
		//used for debug
		if (noutput != mxGetN(mx_cov_output))
		{
			AfxMessageBox("Covariance matrix size does not match the number of outputs!");
		}
		drm_container.cov_output.resize(noutput*noutput);
		//save state covariance matrix, switch to column major from row major
		for (i=0; i<noutput; i++)
		{
			for (j=0; j<noutput; j++)
				drm_container.cov_output[i*noutput+j] = presult[j*noutput+i];
		}
		mxDestroyArray(mx_cov_output);
	}
	if (mx_r2_ukf == NULL)
	{
		AfxMessageBox("Failed to get MATLAB R-Squared for filtered prediction!");
		brun_uq = false;
	}
	else
	{
		presult = mxGetPr(mx_r2_ukf);
		//used for debug
		if (noutput != mxGetM(mx_r2_ukf))
		{
			AfxMessageBox("Size of R-Squared for filtered prediction does not match the number of outputs!");
		}
		drm_container.vd_r2_ukf.resize(noutput);
		//save state covariance matrix, switch to column major from row major
		for (i=0; i<noutput; i++)
			drm_container.vd_r2_ukf[i] = presult[i];
		mxDestroyArray(mx_r2_ukf);
	}
	AfxMessageBox("MATLAB UQ calculation commands completed succesfully.\rPlease click OK button to close MATLAB window.",MB_ICONINFORMATION);
	engClose(ep);
	mxDestroyArray(mx_input);
	mxDestroyArray(mx_output);
	mxDestroyArray(mx_output_drm);
	mxDestroyArray(mx_r2_drm);
	mxDestroyArray(mx_fr_output);
	delete [] poutput;
	delete [] poutput_drm;
	//delete Matlab script files
	DeleteMatlabScriptFilesFromWorkingDirectory(true);
}

void CDRMManager::GenerateDRM()
{
	//generate either DABNet or NARMA model
	int i, j;
	int ninput = drm_container.ninput;
	int noutput = drm_container.noutput;
	CString str, str1;
	msg.AddMessageLine("Starting to generate D-RM. It takes a while to train neural network. Please wait...\n");
	drm_container.pmsg = &msg;
	//no need to read data but need to filter out the constant variables
	drm_container.FilterTrainingData();
	drm_container.data_varied.CalcMeanAndSigma();
	drm_container.data_varied.ScaleInputData();
	drm_container.data_varied.ScaleOutputData();
	drm_container.SaveMeanAndSigmaOfTrainingData();
	//start to build DRM
	if (drm_container.imodel_type==0)	//DABNet model
	{
		for (j=0; j<noutput; j++)
		{
			str.Format("Building DABNet model for output %d...\n",j+1);
			msg.AddMessageLine(str);
			drm_container.drm_dabnet[j].pmsg = &msg;
			drm_container.drm_dabnet[j].lss_list.clear();
			drm_container.drm_dabnet[j].rss_list.clear();
			drm_container.drm_dabnet[j].CreateLaguerreStateSpaceList();
			drm_container.drm_dabnet[j].CreateReducedModelStateSpaceList();
			if (drm_container.drm_dabnet[j].ipole_opt)	//optimize the pole values using user specified as initial guess
			{
				msg.AddMessageLine("Optimize Laguerre poles...\n");
				CSimplex sp;
				CObjectiveFunction objfunc;
				objfunc.SetModel(&drm_container.drm_dabnet[j]);
				objfunc.ResetCounter();
				objfunc.pmsg = &msg;
				double* ppole = new double [ninput];
				double* plen = new double [ninput];
				double* pmax = new double [ninput];
				double* pmin = new double [ninput];
				sp.SetNvar(ninput);
				sp.SetObjectiveFunctionPointer(&objfunc);
				sp.AllocateMemory();
				for (i=0; i<ninput; i++)
				{
					ppole[i] = drm_container.drm_dabnet[j].pole_list[i];
					plen[i] = 0.1;
					pmax[i] = 0.999;
					pmin[i] = 0.001;
				}
				sp.InitSimplex(ppole,plen,pmax,pmin);
				sp.Optimize();
				delete [] ppole;
				delete [] plen;
				delete [] pmax;
				delete [] pmin;
				//pole_list now has the optimized values
				str = "Optimized Laguerre pole values: [";
				for (i=0; i<ninput; i++)
				{
					if (i<ninput-1)
						str1.Format("%lg ",drm_container.drm_dabnet[j].pole_list[i]);
					else
						str1.Format("%lg]\n",drm_container.drm_dabnet[j].pole_list[i]);
					str += str1;
				}
				msg.AddMessageLine(str);
			}
			else	//use user specified pole values
			{
				//train Laguerre model
				msg.AddMessageLine("Training Laguerre model neural network...\n");
				drm_container.drm_dabnet[j].ProcessIdentificationDataForLaguerreTraining();
				drm_container.drm_dabnet[j].TrainLaguerreNeuralNetwork();
			}
			//train reduced model
			msg.AddMessageLine("Reducing order of state space through balanced realization...\n");
			drm_container.drm_dabnet[j].PrepareLaguerreNeuralNetworkWeightMatrices();
			drm_container.drm_dabnet[j].ReduceLaguerreStateSpace();
			//prepare message
			str = "Numbers of state-space variables are [";
			for (i=0; i<ninput; i++)
			{
				str1.Format("%d",drm_container.drm_dabnet[j].rss_list[i].nstate);
				if (i<ninput-1)
					str += str1 + " ";
				else
					str += str1 + "].\n";
			}
			msg.AddMessageLine(str);
			drm_container.drm_dabnet[j].ProcessIdentificationDataForReducedModelTraining();
			msg.AddMessageLine("Training balanced model neural network...\n");
			drm_container.drm_dabnet[j].TrainReducedModelNeuralNetwork();
		}
		msg.AddMessageLine("D-RM has been generated.\n");
	}
	else		//NARMA model
	{
		msg.AddMessageLine("Training neural network...\n");
		drm_container.drm_narma.ann.pmsg = &msg;
		drm_container.drm_narma.ProcessIdentificationDataForTraining();
		drm_container.drm_narma.TrainNeuralNetwork();
		msg.AddMessageLine("D-RM has been generated.\n");
	}
}

void CDRMManager::PredictUsingTrainingData()
{
	drm_container.FilterTrainingData();
	drm_container.ScaleVariedInputData();
	if (drm_container.imodel_type==1)		//NARMA model requires history output data
		drm_container.ScaleVariedOutputData();
	drm_container.PredictTrainingOutputByDRM();
}

void CDRMManager::PredictUsingValidationData()
{
	drm_container.FilterValidationData();
	drm_container.ScaleVariedInputData();
	if (drm_container.imodel_type==1)		//NARMA model requires history output data
		drm_container.ScaleVariedOutputData();
	drm_container.PredictValidationOutputByDRM();
}

void CDRMManager::PrepareCommandParameterCellArray(int i, int j)
{
	//i=0: training, i=1: validation
	//j=0: simulation, j=1: plot, j=2: UQ calculation
	CTime t = CTime::GetCurrentTime();
	filename_temporary = t.Format(_T("DRMBuilder%d%b%Hh%Mm%Ss"));
	CString str;
	command_param = "ID_Par = {";
	bool bfirst;		//first element in an array
	int ii;
	int ncount;			//number of input or output variables to be plotted
	double dtmp;		//temporary double variable
	CString str_unit;
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	str.Format("\'%s.mdl\', ",filename_temporary);
	command_param += str;
	str.Format("[%lg], ",si_train.GetSamplingTimeInterval());
	command_param += str;
	str.Format("[%lg], ",si_train.GetSolverMinimumTimeStep());
	command_param += str;
	str.Format("\'%s\', ",time_unit_acm);
	command_param += str;
	//input variable name
	bfirst = true;
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
	{
		if (bfirst)
		{
			str.Format("\'%s",(*ite_in).desc.c_str());
			bfirst = false;
		}
		else
			str.Format(" %s",(*ite_in).desc.c_str());
		command_param += str;
	}
	str.Format("\', ");
	command_param += str;
	//input units
	bfirst = true;
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
	{
		str_unit = (*ite_in).unit.c_str();
		if (bfirst)
		{
			str.Format("\'%s",str_unit);
			bfirst = false;
		}
		else
			str.Format(" %s",str_unit);
		command_param += str;
	}
	str.Format("\', ");
	command_param += str;
	//input ramp rate
	bfirst = true;
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
	{
		if ((*ite_in).bramp)
			dtmp = (*ite_in).ramp_rate;
		else
			dtmp = -1;
		if (bfirst)
		{
			str.Format("[%lg",dtmp);
			bfirst = false;
		}
		else
			str.Format(" %lg",dtmp);
		command_param += str;
	}
	str.Format("], ");
	command_param += str;
	//input variable indices
	ii = 0;
	bfirst = true;
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ite_in++)
	{
		ii++;
		if ((*ite_in).bvaried)
		{
			if (bfirst)
			{
				str.Format("[%d",ii);
				bfirst = false;
			}
			else
				str.Format(" %d",ii);
			command_param += str;
		}
	}
	str.Format("], ");
	command_param += str;
	//output names
	bfirst = true;
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
	{
		if (bfirst)
		{
			str.Format("\'%s",(*ite_out).desc.c_str());
			bfirst = false;
		}
		else
			str.Format(" %s",(*ite_out).desc.c_str());
		command_param += str;
	}
	str.Format("\', ");
	command_param += str;
	//output units
	bfirst = true;
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
	{
		str_unit = (*ite_out).unit.c_str();
		if (bfirst)
		{
			str.Format("\'%s",str_unit);
			bfirst = false;
		}
		else
			str.Format(" %s",str_unit);
		command_param += str;
	}
	str.Format("\', ");
	command_param += str;
	//output variable indices
	ii = 0;
	bfirst = true;
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ite_out++)
	{
		ii++;
		if ((*ite_out).bvaried)
		{
			if (bfirst)
			{
				str.Format("[%d",ii);
				bfirst = false;
			}
			else
				str.Format(" %d",ii);
			command_param += str;
		}
	}
	str.Format("], ");
	command_param += str;
	//command name
	switch (j)
	{
	case 0:		//simulation
		command_param += "\'Simulation\', ";
		if (i)	//for validation
			command_param += "\'Validation\'}";
		else	//for training
			command_param += "\'Training\'}";
		break;
	case 1:		//plot
		command_param += "\'Plot\', ";
		if (i)	//for validation
			command_param += "\'Validation\', ";
		else	//for training
			command_param += "\'Training\', ";
		//selected input variables to plot
		ncount = input_to_plot.size();
		for (ii=0; ii<ncount; ii++)
		{
			if (ii==0)
				str.Format("[%d", input_to_plot[ii]+1);
			else
				str.Format(" %d", input_to_plot[ii]+1);
			command_param += str;
		}
		str.Format("], ");
		command_param += str;
		//selected output variables to plot
		ncount = output_to_plot.size();
		for (ii=0; ii<ncount; ii++)
		{
			if (ii==0)
				str.Format("[%d", output_to_plot[ii]+1);
			else
				str.Format(" %d", output_to_plot[ii]+1);
			command_param += str;
		}
		str.Format("]}");
		command_param += str;
		break;
	case 2:		//UQ
		command_param += "\'UQ\', ";
		if (i)	//for validation
			command_param += "\'Validation\', ";
		else	//for training
			command_param += "\'Training\', ";
		//selected input variables to plot
		ncount = input_to_plot.size();
		for (ii=0; ii<ncount; ii++)
		{
			if (ii==0)
				str.Format("[%d", input_to_plot[ii]+1);
			else
				str.Format(" %d", input_to_plot[ii]+1);
			command_param += str;
		}
		str.Format("], ");
		command_param += str;
		//selected output variables to plot
		ncount = output_to_plot.size();
		for (ii=0; ii<ncount; ii++)
		{
			if (ii==0)
				str.Format("[%d", output_to_plot[ii]+1);
			else
				str.Format(" %d", output_to_plot[ii]+1);
			command_param += str;
		}
		str.Format("]}");
		command_param += str;
		break;
	}
}

void CDRMManager::WriteTrainingDataToExcel(FILE* pf)
{
	//write the first row as table header
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	fprintf(pf,"Time(%s)",time_unit_acm);
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
		fprintf(pf,",%s(%s)",(*ite_in).name.c_str(),(*ite_in).unit.c_str());
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
		fprintf(pf,",%s(%s)",(*ite_out).name.c_str(),(*ite_out).unit.c_str());
	if (bpredict_train)
	{
		for (ite_out=output_list_sel.begin(); ite_out!=output_list_sel.end(); ++ite_out)
			fprintf(pf,",%s_DRM(%s)",(*ite_out).name.c_str(),(*ite_out).unit.c_str());
	}
	fprintf(pf,"\n");
	//delegate to drm_container to write the file
	drm_container.WriteTrainingDataToExcel(pf, bpredict_train, si_train.GetSamplingTimeInterval());
}

void CDRMManager::WriteValidationDataToExcel(FILE* pf)
{
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	fprintf(pf,"Time(%s)",time_unit_acm);
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
		fprintf(pf,",%s(%s)",(*ite_in).name.c_str(),(*ite_in).unit.c_str());
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
		fprintf(pf,",%s(%s)",(*ite_out).name.c_str(),(*ite_out).unit.c_str());
	if (bpredict_valid)
	{
		for (ite_out=output_list_sel.begin(); ite_out!=output_list_sel.end(); ++ite_out)
			fprintf(pf,",%s_DRM(%s)",(*ite_out).name.c_str(),(*ite_out).unit.c_str());
	}
	fprintf(pf,"\n");
	//delegate to drm_container to write the file
	drm_container.WriteValidationDataToExcel(pf, bpredict_valid, si_valid.GetSamplingTimeInterval());
}

void CDRMManager::Write(FILE* pf)
{
	int iversion = 0;
	int nlength;	//string length
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&bhfm_file,sizeof(bool),1,pf);
	fwrite(&basnp_file,sizeof(bool),1,pf);
	fwrite(&bmdl_file,sizeof(bool),1,pf);
	fwrite(&binputready,sizeof(bool),1,pf);
	fwrite(&boutputready,sizeof(bool),1,pf);
	fwrite(&bsequence_train,sizeof(bool),1,pf);
	fwrite(&bsequence_valid,sizeof(bool),1,pf);
	fwrite(&brun_train,sizeof(bool),1,pf);
	fwrite(&brun_valid,sizeof(bool),1,pf);
	fwrite(&bbuilt_dabnet,sizeof(bool),1,pf);
	fwrite(&bbuilt_narma,sizeof(bool),1,pf);
	fwrite(&bpredict_train,sizeof(bool),1,pf);
	fwrite(&bpredict_valid,sizeof(bool),1,pf);
	fwrite(&bnoise_set,sizeof(bool),1,pf);
	fwrite(&brun_uq,sizeof(bool),1,pf);
	//time unit
	nlength = time_unit_acm.GetLength();
	fwrite(&nlength,sizeof(int),1,pf);
	fwrite(time_unit_acm,sizeof(char),nlength,pf);
	//acm file names
	nlength = filename_acm.GetLength();
	fwrite(&nlength,sizeof(int),1,pf);
	fwrite(filename_acm,sizeof(char),nlength,pf);
	//lists
	nlength = input_list_all.size();
	fwrite(&nlength,sizeof(int),1,pf);
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
		(*ite_in).Write(pf);
	nlength = input_list_sel.size();
	fwrite(&nlength,sizeof(int),1,pf);
	for (ite_in=input_list_sel.begin(); ite_in!=input_list_sel.end(); ++ite_in)
		(*ite_in).Write(pf);
	nlength = output_list_all.size();
	fwrite(&nlength,sizeof(int),1,pf);
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
		(*ite_out).Write(pf);
	nlength = output_list_sel.size();
	fwrite(&nlength,sizeof(int),1,pf);
	for (ite_out=output_list_sel.begin(); ite_out!=output_list_sel.end(); ++ite_out)
		(*ite_out).Write(pf);
	//solver inputs
	si_train.Write(pf);
	si_valid.Write(pf);
	//model
	drm_container.Write(pf);
	//ACM file content
	if (bhfm_file)
	{
		nlength = cstring_acm.GetLength();
		fwrite(&nlength,sizeof(int),1,pf);
		fwrite(cstring_acm,sizeof(char),nlength,pf);
	}
	//ASNP file content
	if (basnp_file)
	{
		nlength = cstring_asnp.GetLength();
		fwrite(&nlength,sizeof(int),1,pf);
		fwrite(cstring_asnp,sizeof(char),nlength,pf);
	}
	//MDL file content
	if (bmdl_file)	//mdl file has been specified and read
	{
		nlength = cstring_mdl.GetLength();
		fwrite(&nlength,sizeof(int),1,pf);
		fwrite(cstring_mdl,sizeof(char),nlength,pf);
	}
}

void CDRMManager::Read(FILE* pf)
{
	int i;
	int iversion;
	int nlength;	//string length
	char buffer[MAX_PATH];
	CInputVariable iv;
	COutputVariable ov;
	fread(&iversion,sizeof(int),1,pf);
	fread(&bhfm_file,sizeof(bool),1,pf);
	fread(&basnp_file,sizeof(bool),1,pf);
	fread(&bmdl_file,sizeof(bool),1,pf);
	fread(&binputready,sizeof(bool),1,pf);
	fread(&boutputready,sizeof(bool),1,pf);
	fread(&bsequence_train,sizeof(bool),1,pf);
	fread(&bsequence_valid,sizeof(bool),1,pf);
	fread(&brun_train,sizeof(bool),1,pf);
	fread(&brun_valid,sizeof(bool),1,pf);
	fread(&bbuilt_dabnet,sizeof(bool),1,pf);
	fread(&bbuilt_narma,sizeof(bool),1,pf);
	fread(&bpredict_train,sizeof(bool),1,pf);
	fread(&bpredict_valid,sizeof(bool),1,pf);
	fread(&bnoise_set,sizeof(bool),1,pf);
	fread(&brun_uq,sizeof(bool),1,pf);
	//time unit
	fread(&nlength,sizeof(int),1,pf);
	fread(buffer,sizeof(char),nlength,pf);
	buffer[nlength] = '\0';
	time_unit_acm = buffer;
	//acm file names
	fread(&nlength,sizeof(int),1,pf);
	fread(buffer,sizeof(char),nlength,pf);
	buffer[nlength] = '\0';
	filename_acm = buffer;
	//lists
	fread(&nlength,sizeof(int),1,pf);
	input_list_all.clear();
	for (i=0; i<nlength; i++)
	{
		iv.Read(pf);
		input_list_all.push_back(iv);
	}
	fread(&nlength,sizeof(int),1,pf);
	input_list_sel.clear();
	for (i=0; i<nlength; i++)
	{
		iv.Read(pf);
		input_list_sel.push_back(iv);
	}
	fread(&nlength,sizeof(int),1,pf);
	output_list_all.clear();
	for (i=0; i<nlength; i++)
	{
		ov.Read(pf);
		output_list_all.push_back(ov);
	}
	fread(&nlength,sizeof(int),1,pf);
	output_list_sel.clear();
	for (i=0; i<nlength; i++)
	{
		ov.Read(pf);
		output_list_sel.push_back(ov);
	}
	//solver inputs
	si_train.Read(pf);
	si_valid.Read(pf);
	si_train.SetAllInputVariableList(&input_list_all);
	si_valid.SetAllInputVariableList(&input_list_all);
	si_train.SetSelectedInputVariableList(&input_list_sel);
	si_valid.SetSelectedInputVariableList(&input_list_sel);
	//model
	drm_container.Read(pf);
	//ACM file content
	if (bhfm_file)
	{
		fread(&nlength,sizeof(int),1,pf);
		fread(cstring_acm.GetBuffer(nlength),sizeof(char),nlength,pf);
		cstring_acm.ReleaseBuffer();
	}
	//ASNP file content
	if (basnp_file)
	{
		fread(&nlength,sizeof(int),1,pf);
		fread(cstring_asnp.GetBuffer(nlength),sizeof(char),nlength,pf);
		cstring_asnp.ReleaseBuffer();
	}
	//MDL file content
	if (bmdl_file)
	{
		fread(&nlength,sizeof(int),1,pf);
		fread(cstring_mdl.GetBuffer(nlength),sizeof(char),nlength,pf);
		cstring_mdl.ReleaseBuffer();
	}
}

void CDRMManager::ReadTextFileToString(CString& fpath, CString& cstr)
{
	ULONGLONG len;
	CFile f;
	cstr = "";
	if (f.Open(fpath, CFile::modeRead))
	{
		len = f.GetLength();
		f.Read(cstr.GetBuffer((int)len),(UINT)len);
		cstr.ReleaseBuffer();
		f.Close();
	}
}

void CDRMManager::CopyMatlabScriptFilesToWorkingDirectory()
{
	//copy matlab script file
	char szAppPath[MAX_PATH];
	char szCurPath[MAX_PATH];
	CString strSrcFile, strSrcFile_processcommand, strSrcFile_strsplit;
	CString strDesFile, strDesFile_processcommand, strDesFile_strsplit;
	::GetModuleFileName(0, szAppPath, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, szCurPath);
	strSrcFile = szAppPath;
	strDesFile = szCurPath;
	strSrcFile = strSrcFile.Left(strSrcFile.ReverseFind('\\'));
	strSrcFile = strSrcFile.Left(strSrcFile.ReverseFind('\\'));
	strSrcFile_processcommand = strSrcFile + "\\matlab_files\\ProcessCommand.m";
	strDesFile_processcommand = strDesFile + "\\ProcessCommand.m";
	strSrcFile_strsplit = strSrcFile + "\\matlab_files\\strsplit.m";
	strDesFile_strsplit = strDesFile + "\\strsplit.m";
	if (!::CopyFile(strSrcFile_processcommand,strDesFile_processcommand,FALSE))
		AfxMessageBox("Unable to copy MATLAB script file ProcessCommand.m to the working directory!");
	if (!::CopyFile(strSrcFile_strsplit,strDesFile_strsplit,FALSE))
		AfxMessageBox("Unable to copy MATLAB script file strsplit.m to the working directory!");
}

void CDRMManager::WriteSimulinkAndAcmFilesToWorkingDirectory()
{
	//use temporary base file name generated in PrepareCommandParameterCellArray() for mdl and acm files
	FILE* pf;
	CString str = foldername;
	str.Replace("\\","\\\\");
	CString str_acm = str + "\\\\" + filename_temporary + ".acmf";
	CString str_mdl = filename_temporary + ".mdl";
	str = cstring_mdl;
	str.Replace("<<ACM_FILE_PATH>>",str_acm);
	pf = fopen(str_mdl,"w");
	if (pf!=NULL)
	{
		fprintf(pf,"%s",str);
		fclose(pf);
	}
	pf = fopen(str_acm,"w");
	if (pf!=NULL)
	{
		fprintf(pf,"%s",cstring_acm);
		fclose(pf);
	}
}

void CDRMManager::WriteACMFileInMemory(FILE* pf)
{
	//write cstring_acm to a file
	if (cstring_acm.GetLength()<100000000)		//limit file size to 100 MB
		fprintf(pf,"%s",cstring_acm);
	else
		AfxMessageBox("ACM file size is over 100 MB.\rZero byte is written.");
}

void CDRMManager::WriteASNPFileInMemory(FILE* pf)
{
	//write cstring_acm to a file
	if (cstring_asnp.GetLength()<100000000)		//limit file size to 100 MB
		fprintf(pf,"%s",cstring_asnp);
	else
		AfxMessageBox("ACM snapshot file size is over 100 MB.\rZero byte is written.");
}

void CDRMManager::WriteMDLFileInMemory(FILE* pf, CString path_acm)
{
	//write cstring_mdl to a file useing path_acm as the ACM file location string
	CString str = cstring_mdl;
	path_acm.Replace("\\","\\\\");
	str.Replace("<<ACM_FILE_PATH>>",path_acm);
	fprintf(pf,"%s",str);
}

void CDRMManager::WriteDRMMatlabFile(FILE* pf)
{
	int icount;
	CString str;
	CString name_list;
	CString desc_list;
	CString unit_list;
	CString varied_list_input;
	CString varied_list_output;
	CString lower_list;
	CString upper_list;
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	//model type
	if (drm_container.imodel_type==0)
		fprintf(pf,"DRM_type = \'DABNet\';\n");
	else
		fprintf(pf,"DRM_type = \'NARMA\';\n");
	//sampling time
	fprintf(pf,"dt = %lg;\n",si_train.GetSamplingTimeInterval());
	//sampling time unit
	fprintf(pf,"dt_unit = \'%s\';\n",time_unit_acm);
	//number of total input variables
	fprintf(pf,"ninput = %d;\n",drm_container.ninput_all);
	//number of total output variables
	fprintf(pf,"noutput = %d;\n",drm_container.noutput_all);
	//all input variables
	name_list = "input_names = {";
	desc_list = "input_descs = {";
	unit_list = "input_units = {";
	varied_list_input = "input_indices = [";
	lower_list = "input_lowers = [";
	upper_list = "input_uppers = [";
	icount = 1;
	for (ite_in=input_list_all.begin(); ite_in!=input_list_all.end(); ++ite_in)
	{
		name_list += "\'" + (CString)(*ite_in).name.c_str() + "\' ";
		desc_list += "\'" + (CString)(*ite_in).desc.c_str() + "\' ";
		unit_list += "\'" + (CString)(*ite_in).unit.c_str() + "\' ";
		if ((*ite_in).bvaried)
		{
			str.Format("%d ",icount);
			varied_list_input += str;
			str.Format("%lg ",(*ite_in).xlower);
			lower_list += str;
			str.Format("%lg ",(*ite_in).xupper);
			upper_list += str;
		}
		else
		{
			str.Format("%lg ",(*ite_in).xdefault);
			lower_list += str;
			upper_list += str;
		}
		icount++;
	}
	name_list.TrimRight();
	desc_list.TrimRight();
	unit_list.TrimRight();
	varied_list_input.TrimRight();
	lower_list.TrimRight();
	upper_list.TrimRight();
	fprintf(pf,"%s};\n",name_list);
	fprintf(pf,"%s};\n",desc_list);
	fprintf(pf,"%s};\n",unit_list);
	fprintf(pf,"%s];\n",varied_list_input);
	fprintf(pf,"%s];\n",lower_list);
	fprintf(pf,"%s];\n",upper_list);
	//all output variables
	name_list = "output_names = {";
	desc_list = "output_descs = {";
	unit_list = "output_units = {";
	varied_list_output = "output_indices = [";
	icount = 1;
	for (ite_out=output_list_all.begin(); ite_out!=output_list_all.end(); ++ite_out)
	{
		name_list += "\'" + (CString)(*ite_out).name.c_str() + "\' ";
		desc_list += "\'" + (CString)(*ite_out).desc.c_str() + "\' ";
		unit_list += "\'" + (CString)(*ite_out).unit.c_str() + "\' ";
		if ((*ite_out).bvaried)
		{
			str.Format("%d ",icount);
			varied_list_output += str;
			icount++;
		}
	}
	name_list.TrimRight();
	desc_list.TrimRight();
	unit_list.TrimRight();
	varied_list_output.TrimRight();
	fprintf(pf,"%s};\n",name_list);
	fprintf(pf,"%s};\n",desc_list);
	fprintf(pf,"%s};\n",unit_list);
	fprintf(pf,"%s];\n",varied_list_output);
	//write additional model parameter in drm_container
	drm_container.WriteDRMMatlabFile(pf);
}

void CDRMManager::WriteCovarianceMatrices(FILE* pf)
{
	if (!brun_uq) return;
	int i, j, nrow;
	//process noise matrix Q
	nrow = drm_container.cov_process.size();
	nrow = (int)sqrt((double)nrow);		//Q is a square matrix
	fprintf(pf,"Process noise covariance matrix (%d by %d)\n", nrow, nrow);
	for (i=0; i<nrow; i++)
	{
		for (j=0; j<nrow-1; j++)
			fprintf(pf,"%lg\t", drm_container.cov_process[i*nrow+j]);
		fprintf(pf,"%lg\n", drm_container.cov_process[i*nrow+nrow-1]);
	}
	//measurement noise matrix R
	nrow = drm_container.cov_measurement.size();
	nrow = (int)sqrt((double)nrow);		//R is a square matrix
	fprintf(pf,"\nMeasurement noise covariance matrix (%d by %d)\n", nrow, nrow);
	for (i=0; i<nrow; i++)
	{
		for (j=0; j<nrow-1; j++)
			fprintf(pf,"%lg\t", drm_container.cov_measurement[i*nrow+j]);
		fprintf(pf,"%lg\n", drm_container.cov_measurement[i*nrow+nrow-1]);
	}
	//state covariance matrix P, P is a square matrix
	nrow = drm_container.cov_state.size();
	nrow = (int)sqrt((double)nrow);		
	fprintf(pf,"\nState-space covariance matrix (%d by %d)\n", nrow, nrow);
	for (i=0; i<nrow; i++)
	{
		for (j=0; j<nrow-1; j++)
			fprintf(pf,"%lg\t", drm_container.cov_state[i*nrow+j]);
		fprintf(pf,"%lg\n", drm_container.cov_state[i*nrow+nrow-1]);
	}
	//output covariance matrix S, S is a square matrix
	nrow = drm_container.cov_output.size();
	nrow = (int)sqrt((double)nrow);		//P is a square matrix
	fprintf(pf,"\nOutput covariance matrix (%d by %d)\n", nrow, nrow);
	for (i=0; i<nrow; i++)
	{
		for (j=0; j<nrow-1; j++)
			fprintf(pf,"%lg\t", drm_container.cov_output[i*nrow+j]);
		fprintf(pf,"%lg\n", drm_container.cov_output[i*nrow+nrow-1]);
	}
}

void CDRMManager::DeleteMatlabScriptFilesFromWorkingDirectory(bool buq)
{
	if (!::DeleteFile("ProcessCommand.m"))
		AfxMessageBox("Failed to delete ProcessCommand.m file!");
	if (!::DeleteFile("strsplit.m"))
		AfxMessageBox("Failed to delete strsplit.m file!");
	if (buq)
	{
		if (!::DeleteFile("DRMParameters.m"))
			AfxMessageBox("Failed to delete DRMParameters.m file!");
	}
}

void CDRMManager::DeleteSimulinkAndAcmFilesFromWorkingDirectory()
{
	CString str_mdl = filename_temporary + ".mdl";
	CString str_acm = filename_temporary + ".acmf";
	if (!::DeleteFile(str_mdl))
		AfxMessageBox("Failed to delete temporary MDL file!");
	if (!::DeleteFile(str_acm))
		AfxMessageBox("Failed to delete temporary ACM file!");
	if (!::DeleteFile("AMSimulation.m"))
		AfxMessageBox("Failed to delete AMSimulation file!");
}

void CDRMManager::DeleteACMTemporaryyFolder()
{
	CString foldername_acm = foldername + "\\AM_" + filename_temporary;
	CString str = foldername_acm + "\\*.*";
	CFileFind finder;
	BOOL bfound = finder.FindFile(str);
	while (bfound)
	{
		bfound = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		//assuming no subfolders
		str = finder.GetFilePath();
		if (!::DeleteFile(str))
			AfxMessageBox("Failed to delete " + str);
	}
	finder.Close();
	if (!::RemoveDirectory(foldername_acm))
		AfxMessageBox("Failed to delete ACM temporary folder!");
}