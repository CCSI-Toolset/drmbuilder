//DRMContainer.h
#ifndef __DRMCONTAINER_H__
#define __DRMCONTAINER_H__

#include <vector>
#include "DataCollection.h"
#include "Narma.h"
#include "Dabnet.h"

class CDRMContainer
{
public:
	int imodel_type;								//model type, 0=DABNet, 1=NARMA
	int ninput;										//number of input variables (size of vector)
	int noutput;									//number of output variables
	int ninput_all;									//number of all inputs from Simulink custom block
	int noutput_all;								//number of all outputs from Simulink custom block
	std::vector<bool> binput_list;					//flag list, true for included input variables
	std::vector<bool> boutput_list;					//flag list, true for included output variables
	std::vector<double> mean_train;					//mean of training data, input and output
	std::vector<double> sigma_train;				//standard deviation of training data, input and output
	std::vector<double> fr_output;					//fraction of standard deviation for DRM output variables as measurement noise, used to calculate R
	std::vector<double> tr_r2_drm;					//R-squared of DRM prediction for training data, vector size = noutput
	std::vector<double> vd_r2_drm;					//R-squared of DRM prediction for validation data, vector size = noutput
	std::vector<double> vd_r2_ukf;					//R-squared of UKF prediction for validation data, vector size = noutput
	std::vector<double> cov_process;				//covariance matrix of process noise Q
	std::vector<double> cov_measurement;			//covariance matrix of measurement noise R
	std::vector<double> cov_state;					//covariance matrix of state-space variables P
	std::vector<double> cov_output;					//covariance matrix of output variables S
	CDataCollection tr_data;						//plant identification data, vector size: ninput_all + noutput_all
	CDataCollection vd_data;						//plant validation data, vector size: ninput_all + noutput_all
	CDataCollection tr_data_drm;					//predicted output data by DRM for plant identification, vector size: noutput
	CDataCollection vd_data_drm;					//predicted output data by DRM for plant validation, vector size: noutput
	CDataCollection data_varied;					//data containing the varied input and output variables (excluding constant input variables), vector size: ninput + noutput
	CNarma drm_narma;								//D-RM based on NARMA model
	CDabnetInput dabnet_input;						//DABNet model parameters and options
	std::vector<CDabnet> drm_dabnet;				//D-RM based on DABNet model, one for each output variable
	CDRMMessage* pmsg;								//a pointer to message object for message display

	CDRMContainer(void);
	virtual ~CDRMContainer(void);
	void SetNumberOfInputs(int n) {ninput = n;}
	void SetNumberOfOutputs(int n) {noutput = n;}
	void SetNumberOfAllInputs(int n) {ninput_all = n;}
	void SetNumberOfAllOutputs(int n) {noutput_all = n;}
	void InitDabnet();
	void InitNarma();
	void PredictTrainingOutputByDRM();
	void PredictValidationOutputByDRM();
	void FilterTrainingData();
	void FilterValidationData();
	void SaveMeanAndSigmaOfTrainingData();
	void ScaleVariedInputData();
	void ScaleVariedOutputData();
	void GetOutputDataAsMatlabArray(CDataCollection* pdc, double* pdp);
	void WriteWeightMatrixFile(FILE* pf);
	void WriteDRMTextFile(FILE* pf);
	void WriteDRMMatlabFile(FILE* pf);
	void WriteTrainingDataToExcel(FILE* pf, bool bpredict, double dt);
	void WriteValidationDataToExcel(FILE* pf, bool bpredict, double dt);
	void Write(FILE* pf);
	void Read(FILE* pf);
};

#endif