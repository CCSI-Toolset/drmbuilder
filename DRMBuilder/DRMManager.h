//DRMManager.h

#ifndef __DRMMANAGER_H__
#define __DRMMANAGER_H__

#include <list>
#include "InputVariable.h"
#include "OutputVariable.h"
#include "SolverInput.h"
#include "DRMContainer.h"
#include "DRMMessage.h"

class CDRMManager
{
public:
	bool bhfm_file;					//true if high-fidelity model file has been specified
	bool bmdl_file;					//true if simulink mdl file has been specified
	bool basnp_file;				//true if snap shot file has been specified
	bool binputready;				//true if input variable list has been assembled
	bool boutputready;				//true if output variable list has been assembled
	bool bsequence_train;			//true if training sequence has been generated
	bool bsequence_valid;			//true if validation sequence has been generated
	bool brun_train;				//true if high-fidelity simulation for training has been completed
	bool brun_valid;				//true if high-fidelity simulation for validation has been completed
	bool bbuilt_dabnet;				//true if DABNet DRM model has been generated
	bool bbuilt_narma;				//true if NARMA DRM model has been generated
	bool bpredict_train;			//true if training response is predicted by DRM
	bool bpredict_valid;			//true if validation response is predicted by DRM
	bool bnoise_set;				//true if noise related to Q and R is set
	bool brun_uq;					//true if UQ calculation is performed
	bool bthread;					//true if thread is running, no save to case file
	BOOL bplot_error;				//true if plotting relative errors, no save to case file
	BOOL bplot_step_change;			//true if plotting input step changes, no save to case file
	BOOL bplot_correlation;			//true if plotting correlation points, no save to case file

	CWinThread* pthread;			//pointer to a thread
	CString time_unit_acm;							//time unit, lower case, e.g. second, hour, minute
	CString filename_temporary;						//temporary base file name for ACM and MDL files with time stamp, not saved to .drm file
	CString filename_acm;							//ACM file file name, saved to .drm file, used to compare the acm file name in mdl file
	CString foldername;								//folder name of current working directory, same as the Simulink .mdl folder, not saved to .drm file
	CString command_param;							//matlab command parameter cell array string, not saved to .drm file
	CString cstring_acm;							//content of acm file
	CString cstring_asnp;							//content of asnp file
	CString cstring_mdl;							//content of mdl file
	std::list<CInputVariable> input_list_all;		//all available input variable list
	std::list<CInputVariable> input_list_sel;		//selected input variable list
	std::list<COutputVariable> output_list_all;		//all available output variable list
	std::list<COutputVariable> output_list_sel;		//selected output variable list
	std::vector<int> input_to_plot;					//indices of input variables to plot, not saved in case file
	std::vector<int> output_to_plot;				//indices of output variables to plot, not saved in case file
	
	CSolverInput si_train;							//high-fidelity solver input for training
	CSolverInput si_valid;							//high-fidelity solver input for validation
	CDRMContainer drm_container;					//container of different kind of DRM models
	CDRMMessage msg;								//message object for the instance of this CDRMManager for displaying messages

	CDRMManager();
	virtual ~CDRMManager();
	bool FindSnapShotValue(CString var_name, double& val);
	void SetAllAvailableInputOutputList();
	void UpdateSelectedInputList();
	void UpdateSelectedOutputList();
	void PrepareTrainingSequence();
	void PrepareValidationSequence();
	void SetMDLSamplingTimeInterval(double dt);
	void RunMatlabHighFidelityModel(int iseq);
	void RunMatlabPlotScript(int iseq);
	void RunMatlabUQScript();
	void GenerateDRM();
	void PredictUsingTrainingData();
	void PredictUsingValidationData();
	void PrepareCommandParameterCellArray(int i, int j);
	void WriteTrainingDataToExcel(FILE* pf);
	void WriteValidationDataToExcel(FILE* pf);
	void Write(FILE* pf);
	void Read(FILE* pf);
	void ReadTextFileToString(CString& fpath, CString& cstr);
	void CopyMatlabScriptFilesToWorkingDirectory();
	void WriteSimulinkAndAcmFilesToWorkingDirectory();
	void WriteACMFileInMemory(FILE* pf);
	void WriteASNPFileInMemory(FILE* pf);
	void WriteMDLFileInMemory(FILE* pf, CString path_acm);
	void WriteDRMMatlabFile(FILE* pf);
	void WriteCovarianceMatrices(FILE* pf);
	void DeleteMatlabScriptFilesFromWorkingDirectory(bool buq);
	void DeleteSimulinkAndAcmFilesFromWorkingDirectory();
	void DeleteACMTemporaryyFolder();
};

#endif