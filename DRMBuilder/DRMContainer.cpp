//DRMContainer.cpp
#include "stdafx.h"
#include <math.h>
#include "DRMContainer.h"
#include "DRMManager.h"

CDRMContainer::CDRMContainer(void)
{
	imodel_type = 0;
	ninput = 1;
	noutput = 0;
	ninput_all = 1;
	noutput_all = 1;
	pmsg = NULL;
}

CDRMContainer::~CDRMContainer(void)
{
}

void CDRMContainer::InitDabnet()
{
	//assume ninput, noutput has been set
	int i;
	drm_dabnet.clear();
	drm_dabnet.resize(noutput);
	//set default pole, order data
	//the actual data will be provided by GUI with total number of Laguerre states updated inside the GUI
	for (i=0; i<noutput; i++)
	{
		drm_dabnet[i].ninput = ninput;
		drm_dabnet[i].noutput = 1;
		drm_dabnet[i].ioutput = i;
		drm_dabnet[i].SetUniformLaguerreOrders(6);
		drm_dabnet[i].SetUniformLaguerrePoles(0.5);
		drm_dabnet[i].pid_data = &data_varied;
		drm_dabnet[i].pmsg = pmsg;
		drm_dabnet[i].pdabnet_input = &dabnet_input;
	}
	//default order, poles, and others are assigned by constructor
}

void CDRMContainer::InitNarma()
{
	//assume ninput, noutput has been set
	drm_narma.ninput = ninput;
	drm_narma.noutput = noutput;
	drm_narma.pid_data = &data_varied;
}

void CDRMContainer::PredictTrainingOutputByDRM()
{
	//used scaled input data in data_varied to predict output and send results to id_data_drm
	int i, j;
	int npoint = data_varied.npair;
	tr_data_drm.SetSize(npoint,0,noutput);
	double ybar;		//average of ACM model prediction
	double dy;			//difference of y
	double sst;			//SST: total sum of squares sigma(yi-ybar)^2
	double sse;			//SSE: sum of squres due to error sigma(yhat-yi)^2
	double** ppin = data_varied.ppdata;
	double** ppout = tr_data_drm.ppdata;
	if (imodel_type==0)	//DABNET model
	{
		for (i=0; i<noutput; i++)
			drm_dabnet[i].PredictByDRM(npoint,ppin,ppout);
	}
	else		//NARMA model
		drm_narma.PredictByDRM(npoint,ppin,ppout);
	//scale output data
	for (i=0; i<noutput; i++)
	{
		tr_data_drm.pmean[i] = mean_train[ninput+i];
		tr_data_drm.psigma[i] = sigma_train[ninput+i];
	}
	tr_data_drm.UnscaleOutputData();
	//calculate R-squred of the DRM prediction
	double** ppout_acm = tr_data.ppdata;
	int* pioutput = new int [noutput];
	j = 0;
	for (i=0; i<noutput_all; i++)
	{
		if (boutput_list[i])
		{
			pioutput[j] = i;
			j++;
		}
	}
	tr_r2_drm.resize(noutput);
	for (i=0; i<noutput; i++)
	{
		//calculate averate y
		ybar = 0;
		for (j=0; j<npoint; j++)
			ybar += ppout_acm[j][ninput_all+pioutput[i]];
		ybar /= (double)npoint;
		//calculate sst and sse
		sst = 0;
		sse = 0;
		for (j=0; j<npoint; j++)
		{
			dy = ppout_acm[j][ninput_all+pioutput[i]] - ybar;
			sst += dy*dy;
			dy = ppout_acm[j][ninput_all+pioutput[i]] - ppout[j][i];
			sse += dy*dy;
		}
		tr_r2_drm[i] = 1 - sse/sst;
	}
	delete pioutput;
}

void CDRMContainer::PredictValidationOutputByDRM()
{
	//used scaled input data in data_varied to predict output and send results to vd_data_drm
	int i, j;
	int npoint = data_varied.npair;
	vd_data_drm.SetSize(npoint,0,noutput);
	double ybar;		//average of ACM model prediction
	double dy;			//difference of y
	double sst;			//SST: total sum of squares sigma(yi-ybar)^2
	double sse;			//SSE: sum of squres due to error sigma(yhat-yi)^2
	double** ppin = data_varied.ppdata;
	double** ppout = vd_data_drm.ppdata;
	if (imodel_type==0)	//DABNET model
	{
		for (i=0; i<noutput; i++)
			drm_dabnet[i].PredictByDRM(npoint,ppin,ppout);
	}
	else		//NARMA model
		drm_narma.PredictByDRM(npoint,ppin,ppout);
	//scale output data
	for (i=0; i<noutput; i++)
	{
		vd_data_drm.pmean[i] = mean_train[ninput+i];
		vd_data_drm.psigma[i] = sigma_train[ninput+i];
	}
	vd_data_drm.UnscaleOutputData();
	//calculate R-squred of the DRM prediction
	double** ppout_acm = vd_data.ppdata;
	int* pioutput = new int [noutput];
	j = 0;
	for (i=0; i<noutput_all; i++)
	{
		if (boutput_list[i])
		{
			pioutput[j] = i;
			j++;
		}
	}
	vd_r2_drm.resize(noutput);
	for (i=0; i<noutput; i++)
	{
		//calculate averate y
		ybar = 0;
		for (j=0; j<npoint; j++)
			ybar += ppout_acm[j][ninput_all+pioutput[i]];
		ybar /= (double)npoint;
		//calculate sst and sse
		sst = 0;
		sse = 0;
		for (j=0; j<npoint; j++)
		{
			dy = ppout_acm[j][ninput_all+pioutput[i]] - ybar;
			sst += dy*dy;
			dy = ppout_acm[j][ninput_all+pioutput[i]] - ppout[j][i];
			sse += dy*dy;
		}
		vd_r2_drm[i] = 1 - sse/sst;
	}
	delete pioutput;
}

void CDRMContainer::FilterTrainingData()
{
	//filter constant data in the id_data and assign them to data_varied
	int i, j;
	int ivar;
	int npoint;						//number of data point
	double* prow_old;				//pointer to a row of old data from id_data
	double* prow_new;				//pointer to a row of new data from data_varied
	double** ppdata_old;			//pointer to id_data.ppdata
	double** ppdata_new;			//pointer to data_varied.ppdata
	npoint = tr_data.npair;
	//allocate memeory for id_data
	data_varied.SetSize(npoint,ninput,noutput);
	ppdata_old = tr_data.ppdata;
	ppdata_new = data_varied.ppdata;
	for (i=0; i<npoint; i++)
	{
		prow_old = ppdata_old[i];
		prow_new = ppdata_new[i];
		ivar = 0;
		for (j=0; j<ninput_all; j++)
		{
			if (binput_list[j])
				prow_new[ivar++] = prow_old[j];
		}
		for (j=0; j<noutput_all; j++)
		{
			if (boutput_list[j])
				prow_new[ivar++] = prow_old[ninput_all+j];
		}
	}
}

void CDRMContainer::FilterValidationData()
{
	//filter constant data in the vd_data and assign them to data_varied
	int i, j;
	int ivar;
	int npoint;						//number of data point
	double* prow_old;				//pointer to a row of old data from vd_data
	double* prow_new;				//pointer to a row of new data from data_varied
	double** ppdata_old;			//pointer to vd_data.ppdata
	double** ppdata_new;			//pointer to data_varied.ppdata
	npoint = vd_data.npair;
	//allocate memeory for id_data
	data_varied.SetSize(npoint,ninput,noutput);
	ppdata_old = vd_data.ppdata;
	ppdata_new = data_varied.ppdata;
	for (i=0; i<npoint; i++)
	{
		prow_old = ppdata_old[i];
		prow_new = ppdata_new[i];
		ivar = 0;
		for (j=0; j<ninput_all; j++)
		{
			if (binput_list[j])
				prow_new[ivar++] = prow_old[j];
		}
		for (j=0; j<noutput_all; j++)
		{
			if (boutput_list[j])
				prow_new[ivar++] = prow_old[ninput_all+j];
		}
	}
}

void CDRMContainer::SaveMeanAndSigmaOfTrainingData()
{
	int i;
	int ntotal = ninput + noutput;
	mean_train.resize(ntotal);
	sigma_train.resize(ntotal);
	for (i=0; i<ntotal; i++)
	{
		mean_train[i] = data_varied.pmean[i];
		sigma_train[i] = data_varied.psigma[i];
	}
}

void CDRMContainer::ScaleVariedInputData()
{
	//scale the varied input data
	int i;
	for (i=0; i<ninput; i++)
	{
		data_varied.pmean[i] = mean_train[i];
		data_varied.psigma[i] = sigma_train[i];
	}
	data_varied.ScaleInputData();
}

void CDRMContainer::ScaleVariedOutputData()
{
	//scale the varied input data
	int i;
	for (i=0; i<noutput; i++)
	{
		data_varied.pmean[ninput+i] = mean_train[ninput+i];
		data_varied.psigma[ninput+i] = sigma_train[ninput+i];
	}
	data_varied.ScaleOutputData();
}

void CDRMContainer::GetOutputDataAsMatlabArray(CDataCollection* pdc, double* pdp)
{
	//assign output data in pdc to 1-D array (pointer) pdp to be mapped to a 2-D MATLAB array
	int i, j, k;
	int npair = pdc->npair;
	int nin = pdc->nin;
	int nout = pdc->nout;
	double** ppdata = pdc->ppdata;
	k = 0;
	for (i=0; i<npair; i++)
	{
		for (j=0; j<nout; j++)
			pdp[k++] = ppdata[i][nin+j];
	}
}

void CDRMContainer::WriteWeightMatrixFile(FILE* pf)
{
	int i;
	for (i=0; i<noutput; i++)
	{
		fprintf(pf,"Weight matrices for Output %d\n",i+1);
		drm_dabnet[i].WriteWeightMatrixFile(pf);
	}
}

void CDRMContainer::WriteDRMTextFile(FILE* pf)
{
	int i;
	fprintf(pf,"%d\t//number of input variables\n",ninput);
	fprintf(pf,"%d\t//number of output variables\n",noutput);
	fprintf(pf,"//mean of training input data\n");
	for (i=0; i<ninput; i++)
		fprintf(pf,"%lg\t", mean_train[i]);
	fprintf(pf,"\n");
	fprintf(pf,"//standard deviation of training input data\n");
	for (i=0; i<ninput; i++)
		fprintf(pf,"%lg\t", sigma_train[i]);
	fprintf(pf,"\n");
	fprintf(pf,"//mean of training output data\n");
	for (i=0; i<noutput; i++)
		fprintf(pf,"%lg\t", mean_train[i+ninput]);
	fprintf(pf,"\n");
	fprintf(pf,"//standard deviation of training output data\n");
	for (i=0; i<noutput; i++)
		fprintf(pf,"%lg\t", sigma_train[i+ninput]);
	fprintf(pf,"\n");
	//write model data
	if (imodel_type==0)	//DABNet model
	{
		//always write the reduced form
		for (i=0; i<noutput; i++)
			drm_dabnet[i].WriteDRMTextFile(pf);
	}
	else	//NARMA model
		drm_narma.WriteDRMTextFile(pf);
	
}

void CDRMContainer::WriteDRMMatlabFile(FILE* pf)
{
	int i;
	//numbers of inputs and outputs
	fprintf(pf,"nu = %d;\n",ninput);
	fprintf(pf,"ny = %d;\n",noutput);
	//write mean and sigma of input and output data
	for (i=0; i<ninput; i++)
		fprintf(pf,"u_mean(%d) = %lg;\n",i+1,mean_train[i]);
	for (i=0; i<noutput; i++)
		fprintf(pf,"y_mean(%d) = %lg;\n",i+1,mean_train[i+ninput]);
	for (i=0; i<ninput; i++)
		fprintf(pf,"u_sigma(%d) = %lg;\n",i+1,sigma_train[i]);
	for (i=0; i<noutput; i++)
		fprintf(pf,"y_sigma(%d) = %lg;\n",i+1,sigma_train[i+ninput]);
	if (imodel_type==0)	//DABNet model
	{
		for (i=0; i<noutput; i++)
			drm_dabnet[i].WriteDRMMatlabFile(pf);
	}
	else	//NARMA model
		drm_narma.WriteDRMMatlabFile(pf);
}

void CDRMContainer::WriteTrainingDataToExcel(FILE* pf, bool bpredict, double dt)
{
	//write training input output data, including DRM prediction data is available
	//bpredict: true of DRM model prediction results is available
	//dt: sampling time interval
	int i, j;
	int npair = tr_data.npair;
	int ntotal = ninput_all + noutput_all;
	double t = 0;
	double** ppdata = tr_data.ppdata;
	double** ppdata_drm;
	if (bpredict)
		ppdata_drm = tr_data_drm.ppdata;
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",t);
		t += dt;
		for (j=0; j<ntotal; j++)
			fprintf(pf,",%lg",ppdata[i][j]);
		if (bpredict)
		{
			for (j=0; j<noutput; j++)
				fprintf(pf,",%lg",ppdata_drm[i][j]);
		}
		fprintf(pf,"\n");
	}
}

void CDRMContainer::WriteValidationDataToExcel(FILE* pf, bool bpredict, double dt)
{
	//write validation input output data, including DRM prediction data is available
	//bpredict: true of DRM model prediction results is available
	//dt: sampling time interval
	int i, j;
	int npair = vd_data.npair;
	int ntotal = ninput_all + noutput_all;
	double t = 0;
	double** ppdata = vd_data.ppdata;
	double** ppdata_drm;
	if (bpredict)
		ppdata_drm = vd_data_drm.ppdata;
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",t);
		t += dt;
		for (j=0; j<ntotal; j++)
			fprintf(pf,",%lg",ppdata[i][j]);
		if (bpredict)
		{
			for (j=0; j<noutput; j++)
				fprintf(pf,",%lg",ppdata_drm[i][j]);
		}
		fprintf(pf,"\n");
	}
}

void CDRMContainer::Write(FILE* pf)
{
	bool btmp;	//for read bool variable in vector.  somehow using vector element directly causes crash or incorrect true or false read
	int i;
	int iversion = 0;
	int nsize;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&imodel_type,sizeof(int),1,pf);
	fwrite(&ninput,sizeof(int),1,pf);
	fwrite(&noutput,sizeof(int),1,pf);
	fwrite(&ninput_all,sizeof(int),1,pf);
	fwrite(&noutput_all,sizeof(int),1,pf);
	nsize = binput_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
	{
		btmp = binput_list[i];
		fwrite(&btmp,sizeof(bool),1,pf);
	}
	nsize = boutput_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
	{
		btmp = boutput_list[i];
		fwrite(&btmp,sizeof(bool),1,pf);
	}
	nsize = mean_train.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&mean_train[i],sizeof(double),1,pf);
	nsize = sigma_train.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&sigma_train[i],sizeof(double),1,pf);
	nsize = fr_output.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&fr_output[i],sizeof(double),1,pf);
	nsize = tr_r2_drm.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&tr_r2_drm[i],sizeof(double),1,pf);
	nsize = vd_r2_drm.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&vd_r2_drm[i],sizeof(double),1,pf);
	nsize = vd_r2_ukf.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&vd_r2_ukf[i],sizeof(double),1,pf);
	nsize = cov_process.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&cov_process[i],sizeof(double),1,pf);
	nsize = cov_measurement.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&cov_measurement[i],sizeof(double),1,pf);
	nsize = cov_state.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&cov_state[i],sizeof(double),1,pf);
	nsize = cov_output.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&cov_output[i],sizeof(double),1,pf);
	tr_data.Write(pf);
	vd_data.Write(pf);
	tr_data_drm.Write(pf);
	vd_data_drm.Write(pf);
	drm_narma.Write(pf);
	dabnet_input.Write(pf);
	for (i=0; i<noutput; i++)
		drm_dabnet[i].Write(pf);
}

void CDRMContainer::Read(FILE* pf)
{
	bool btmp;		//for read bool variable in vector.  somehow using vector element directly causes crash
	int i;
	int iversion;
	int nsize;
	fread(&iversion,sizeof(int),1,pf);
	fread(&imodel_type,sizeof(int),1,pf);
	fread(&ninput,sizeof(int),1,pf);
	fread(&noutput,sizeof(int),1,pf);
	fread(&ninput_all,sizeof(int),1,pf);
	fread(&noutput_all,sizeof(int),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	binput_list.resize(nsize);
	for (i=0; i<nsize; i++)
	{
		fread(&btmp,sizeof(bool),1,pf);
		binput_list[i] = btmp;
	}
	fread(&nsize,sizeof(int),1,pf);
	boutput_list.resize(nsize);
	for (i=0; i<nsize; i++)
	{
		fread(&btmp,sizeof(bool),1,pf);
		boutput_list[i] = btmp;
	}
	fread(&nsize,sizeof(int),1,pf);
	mean_train.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&mean_train[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	sigma_train.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&sigma_train[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	fr_output.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&fr_output[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	tr_r2_drm.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&tr_r2_drm[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	vd_r2_drm.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&vd_r2_drm[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	vd_r2_ukf.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&vd_r2_ukf[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	if (nsize==0)
		cov_process.clear();
	else
	{
		cov_process.resize(nsize);
		for (i=0; i<nsize; i++)
			fread(&cov_process[i],sizeof(double),1,pf);
	}
	fread(&nsize,sizeof(int),1,pf);
	if (nsize==0)
		cov_measurement.clear();
	else
	{
		cov_measurement.resize(nsize);
		for (i=0; i<nsize; i++)
			fread(&cov_measurement[i],sizeof(double),1,pf);
	}
	fread(&nsize,sizeof(int),1,pf);
	if (nsize==0)
		cov_state.clear();
	else
	{
		cov_state.resize(nsize);
		for (i=0; i<nsize; i++)
			fread(&cov_state[i],sizeof(double),1,pf);
	}
	fread(&nsize,sizeof(int),1,pf);
	if (nsize==0)
		cov_output.clear();
	else
	{
		cov_output.resize(nsize);
		for (i=0; i<nsize; i++)
			fread(&cov_output[i],sizeof(double),1,pf);
	}
	tr_data.Read(pf);
	vd_data.Read(pf);
	tr_data_drm.Read(pf);
	vd_data_drm.Read(pf);
	drm_narma.Read(pf);
	drm_narma.pid_data = &data_varied;
	dabnet_input.Read(pf);
	drm_dabnet.clear();
	drm_dabnet.resize(noutput);
	for (i=0; i<noutput; i++)
	{
		drm_dabnet[i].Read(pf);
		drm_dabnet[i].pid_data = &data_varied;
		drm_dabnet[i].pdabnet_input = &dabnet_input;
	}
}