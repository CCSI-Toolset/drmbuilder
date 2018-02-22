//Dabnet.cpp
#include "stdafx.h"
#include <math.h>
#include "Dabnet.h"
#include "IpIpoptApplication.hpp"
#include "AnnIpopt.h"

using namespace Ipopt;

CDabnet::CDabnet(void)
{
	ninput = 1;
	noutput = 1;
	ioutput = 0;
	ipole_opt = 0;
	nneuron_hid = 10;
	nstate_lag = 1;
	nstate_red = 1;
	pid_data = NULL;
}

CDabnet::~CDabnet(void)
{
}

void CDabnet::SetUniformLaguerreOrders(int n)
{
	nstate_lag = 0;
	norder_list.resize(ninput);
	for (int i=0; i<ninput; i++)
	{
		norder_list[i] = n;
		nstate_lag += n;
	}
}

void CDabnet::UpdateTotalLaguerreOrders()
{
	int i;
	nstate_lag = 0;
	for (i=0; i<ninput; i++)
		nstate_lag += norder_list[i];
}

void CDabnet::SetUniformLaguerrePoles(double p)
{
	pole_list.resize(ninput);
	for (int i=0; i<ninput; i++)
		pole_list[i] = p;
}

void CDabnet::CreateLaguerreStateSpaceList()
{
	int i;
	lss_list.resize(ninput);
	for (i=0; i<ninput; i++)
	{
		lss_list[i].a = pole_list[i];
		lss_list[i].nstate = norder_list[i];
		lss_list[i].AllocateMemory();
		lss_list[i].CreateUnbalancedStateSpace();
		lss_list[i].InitStateVectorAsB();
		lss_list[i].pmsg = pmsg;
	}
}

void CDabnet::UpdateLaguerreStateSpaceList()
{
	//assume memory already allocated
	CString str = "Trying Laguerre pole values at [";
	CString str1;
	int i;
	for (i=0; i<ninput; i++)
	{
		if (i<ninput-1)
			str1.Format("%lg ",pole_list[i]);
		else
			str1.Format("%lg]...\n",pole_list[i]);
		str += str1;
		lss_list[i].a = pole_list[i];
		lss_list[i].nstate = norder_list[i];
		lss_list[i].CreateUnbalancedStateSpace();
		lss_list[i].InitStateVectorAsB();
	}
	pmsg->AddMessageLine(str);
}

void CDabnet::ProcessIdentificationDataForLaguerreTraining()
{
	//id_data has already been scaled (both input and output)
	int i, j, k, n;
	int nstate_j;
	int npoint = pid_data->npair;
	ann_data.SetSize(npoint, nstate_lag, noutput);
	double** ppdata_id = pid_data->ppdata;
	double** ppdata_ann = ann_data.ppdata;
	double* prow_id;
	double* prow_ann;
	//calculate the state variables at steady state assuming input for the steady state is the same as the input of the first point
	prow_id = ppdata_id[0];
	for (j=0; j<ninput; j++)
		lss_list[j].InitStateVectorToSteadyState(prow_id[j]);
	//define u(k) as the input value between k and k+1 time interval [k,k+1)
	//note that each row in id_data is a pair of u(k) and y(k+1), the response after u(k) is applied for a fixed time interval
	//the ANN mapping is x(k+1) to y(k+1)
	for (i=0; i<npoint; i++)
	{
		prow_id = ppdata_id[i];
		prow_ann = ppdata_ann[i];
		n = 0;
		//inputs
		for (j=0; j<ninput; j++)
		{
			//lss_list[j].CalcNextStateVector(prow_id[j]);
			nstate_j = norder_list[j];
			for (k=0; k<nstate_j; k++)
			{
				/* does not apply C matrix
				sum = 0;
				for (m=0; m<=k; m++)
					sum += lss_list[j].px[m]*lss_list[j].pc[nstate_j-1-k+m];
				prow_ann[n++] = sum;
				*/
				prow_ann[n++] = lss_list[j].px[k];
			}
			//apply Ax+Bu
			lss_list[j].CalcNextStateVector(prow_id[j]);
		}
		//outputs, note that noutput=1 and ioutput is the index of modeled output variables assigned by CDRMContainer
		prow_ann[n++] = prow_id[ninput+ioutput];
	}
}

void CDabnet::ProcessIdentificationDataForReducedModelTraining()
{
	int i, j, k, n;
	int nstate_j;
	int npoint = pid_data->npair;
	//resize the ann_data, note that nstate_red has been updated by ReduceLaguerreStateSpace()function call
	ann_data.SetSize(npoint, nstate_red, noutput);
	double** ppdata_id = pid_data->ppdata;
	double** ppdata_ann = ann_data.ppdata;
	double* prow_id;
	double* prow_ann;
	//for the first point, calculate the state variables at steady state
	prow_id = ppdata_id[0];
	for (j=0; j<ninput; j++)
		rss_list[j].InitStateVectorToSteadyState(prow_id[j]);
	//define u(k) as the input value between k and k+1 time interval [k,k+1)
	//note that each row in id_data is a pair of u(k) and y(k+1), the response after u(k) is applied for a fixed time interval
	//the ANN mapping is x(k+1) to y(k+1)
	for (i=0; i<npoint; i++)
	{
		prow_id = ppdata_id[i];
		prow_ann = ppdata_ann[i];
		n = 0;
		//inputs
		for (j=0; j<ninput; j++)
		{
			nstate_j = rss_list[j].nstate;
			//rss_list[j].CalcNextStateVector(prow_id[j]);
			for (k=0; k<nstate_j; k++)
				prow_ann[n++] = rss_list[j].px[k];
			rss_list[j].CalcNextStateVector(prow_id[j]);
		}
		//outputs, note that noutput=1 and ioutput is the index of modeled output variables assigned by CDRMContainer
		prow_ann[n++] = prow_id[ninput+ioutput];
	}
}

void CDabnet::TrainLaguerreNeuralNetworkByBackPropagation()
{
	//nn is the number of neurons in hidden layer
	//use default activation function (sigmoid for hidden layer and linear for output layer)
	int nhidden_layer = 1;		//use just one hidden layer
	int pnneuron_hid[1];
	pnneuron_hid[0] = nneuron_hid;
	ann_lag.SetNetworkStructure(nstate_lag,noutput,nhidden_layer,pnneuron_hid);
	ann_lag.InitUniformWeights(pdabnet_input->weight_init);		//use small weights help balanced realization
	ann_lag.SetRandomSeed(0);
	ann_lag.SetTrainMethod(1);
	ann_lag.SetStopError(0.0001);
	ann_lag.SetMaximumNumverOfEpoches(pdabnet_input->nmax_iter_bp_lag);
	if (pdabnet_input->iscale_lag_opt)
	{
		ann_lag.CalcMeanSigmaForInputOutputData(ann_data.npair,ann_data.ppdata);
		ann_lag.ScaleInputOutputData(ann_data.npair,ann_data.ppdata);
	}
	else
		ann_lag.SetDefaultMeanSigma();
	ann_lag.TrainDataSet(ann_data.npair,ann_data.ppdata);
}

void CDabnet::TrainLaguerreNeuralNetworkByIpopt()
{
	int nhidden_layer = 1;		//use just one hidden layer
	int pnneuron_hid[1];
	pnneuron_hid[0] = nneuron_hid;
	ann_lag.SetNetworkStructure(nstate_lag,noutput,nhidden_layer,pnneuron_hid);
	ann_lag.SetWeightForInitialization(pdabnet_input->weight_init);	//to be used by IPOPT initialization function
	if (pdabnet_input->iscale_lag_opt)
	{
		ann_lag.CalcMeanSigmaForInputOutputData(ann_data.npair,ann_data.ppdata);
		ann_lag.ScaleInputOutputData(ann_data.npair,ann_data.ppdata);
	}
	else
		ann_lag.SetDefaultMeanSigma();
	CAnnIpopt* pmynlp = new CAnnIpopt();
	pmynlp->pmsg = pmsg;
	SmartPtr<TNLP> mynlp = pmynlp;
	pmynlp->pann = &ann_lag;
	pmynlp->pdc = &ann_data;
	SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
	app->Options()->SetNumericValue("tol", 1e-5);		//revised from 1e-7
	app->Options()->SetIntegerValue("max_iter", pdabnet_input->nmax_iter_ipopt_lag);
	app->Options()->SetStringValue("mu_strategy", "adaptive");
	app->Options()->SetStringValue("output_file", "ipopt.out");
	ApplicationReturnStatus status;
	//initialization
	status = app->Initialize();
	if (status != Solve_Succeeded)
	{
		AfxMessageBox("Error during initialization!");
		return;
	}
	//solve the problem
	status = app->OptimizeTNLP(mynlp);
	if (status != Solve_Succeeded)
		pmsg->AddMessageLine("Maximum number of iterations reached!\n");
}

void CDabnet::TrainLaguerreNeuralNetwork()
{
	ann_lag.pmsg = pmsg;
	if (pdabnet_input->itrain_lag_opt)
		TrainLaguerreNeuralNetworkByIpopt();
	else
		TrainLaguerreNeuralNetworkByBackPropagation();
}

void CDabnet::TrainReducedModelNeuralNetworkByBackPropagation()
{
	int nhidden_layer = 1;
	int pnneuron_hid[1];
	pnneuron_hid[0] = nneuron_hid;
	ann_red.SetNetworkStructure(nstate_red,noutput,nhidden_layer,pnneuron_hid);
	ann_red.SetRandomSeed(0);
	ann_red.SetTrainMethod(1);
	ann_red.SetStopError(0.00001);
	ann_red.SetMaximumNumverOfEpoches(pdabnet_input->nmax_iter_bp_red);
	if (pdabnet_input->iscale_red_opt)
	{
		ann_red.CalcMeanSigmaForInputOutputData(ann_data.npair,ann_data.ppdata);
		ann_red.ScaleInputOutputData(ann_data.npair,ann_data.ppdata);
	}
	else
		ann_red.SetDefaultMeanSigma();
	ann_red.TrainDataSet(ann_data.npair,ann_data.ppdata);	
}

void CDabnet::TrainReducedModelNeuralNetworkByIpopt()
{
	int nhidden_layer = 1;		//use just one hidden layer
	int pnneuron_hid[1];
	pnneuron_hid[0] = nneuron_hid;
	ann_red.SetNetworkStructure(nstate_red,noutput,nhidden_layer,pnneuron_hid);
	ann_red.SetWeightForInitialization(0.2);	//to be used by IPOPT initialization function
	if (pdabnet_input->iscale_red_opt)
	{
		ann_red.CalcMeanSigmaForInputOutputData(ann_data.npair,ann_data.ppdata);
		ann_red.ScaleInputOutputData(ann_data.npair,ann_data.ppdata);
	}
	else
		ann_red.SetDefaultMeanSigma();
	CAnnIpopt* pmynlp = new CAnnIpopt();
	pmynlp->pmsg = pmsg;
	SmartPtr<TNLP> mynlp = pmynlp;
	pmynlp->pann = &ann_red;
	pmynlp->pdc = &ann_data;
	SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
	app->Options()->SetNumericValue("tol", 1e-5);		//revised from 1e-7
	app->Options()->SetIntegerValue("max_iter", pdabnet_input->nmax_iter_ipopt_red);
	app->Options()->SetStringValue("mu_strategy", "adaptive");
	app->Options()->SetStringValue("output_file", "ipopt.out");
	ApplicationReturnStatus status;
	//initialization
	status = app->Initialize();
	if (status != Solve_Succeeded)
	{
		AfxMessageBox("Error during initialization!");
		return;
	}
	//solve the problem
	status = app->OptimizeTNLP(mynlp);
	if (status != Solve_Succeeded)
		pmsg->AddMessageLine("Maximum number of iterations reached!\n");
}

void CDabnet::TrainReducedModelNeuralNetwork()
{
	ann_red.pmsg = pmsg;
	if (pdabnet_input->itrain_red_opt)
		TrainReducedModelNeuralNetworkByIpopt();
	else
		TrainReducedModelNeuralNetworkByBackPropagation();
}

void CDabnet::PrepareLaguerreNeuralNetworkWeightMatrices()
{
	int i, j;
	int istate = 0;
	int nneuron = ann_lag.GetNeuronCountOfFirstHiddenLayerExcludingBias();
	double* pweight = ann_lag.GetWeightPointer();	//1-D array from ANN
	double* psigma = ann_lag.GetSigmaPointer();
	double** ppweight = new double* [nneuron];
	for (i=0; i<ninput; i++)
	{
		for (j=0; j<nneuron; j++)
			ppweight[j] = pweight + j*(nstate_lag+1) + istate;
		lss_list[i].PrepareWeightMatrix(nneuron, ppweight, psigma+istate, pdabnet_input->iscale_lag_opt);
		istate += norder_list[i];
	}
	delete [] ppweight;
}

void CDabnet::ReduceLaguerreStateSpace()
{
	int i;
	//reduce Laguerre model and update nstate_red
	nstate_red = 0;
	for (i=0; i<ninput; i++)
	{
		rss_list[i].DeleteMemory();		//delete in case already allocated, e.g. read from case file
		lss_list[i].RealizeBalancedStateSpace(&rss_list[i]);
		nstate_red += rss_list[i].nstate;
	}
}

void CDabnet::PredictByLaguerreModel(int np, double** ppin, double** ppout)
{
	//np: the number of the data points
	//ppin: the 2-D array of input data, ppdata in vd_data can be used as ppin for prediction (ignore the output part)
	//ppout: the 2-D array of predicted output values without unscaling, only element at index of ioutput is modified and returned
	int i, j, k, n;
	int nstate_j;
	double* prow;
	double* pann_input = new double [nstate_lag];	//state variables
	double* pann_output = new double [noutput];		//noutput is always 1
	prow = ppin[0];
	for (j=0; j<ninput; j++)
		lss_list[j].InitStateVectorToSteadyState(prow[j]);
	for (i=0; i<np; i++)
	{
		prow = ppin[i];
		n = 0;
		for (j=0; j<ninput; j++)
		{
			//lss_list[j].CalcNextStateVector(prow[j]);
			nstate_j = norder_list[j];
			for (k=0; k<nstate_j; k++)
			{
				/*  does not apply C matrix
				sum = 0;
				for (m=0; m<=k; m++)
					sum += lss_list[j].px[m]*lss_list[j].pc[nstate_j-1-k+m];
				pann_input[n++] = sum;
				*/
				pann_input[n++] = lss_list[j].px[k];
			}
			lss_list[j].CalcNextStateVector(prow[j]);
		}
		if (pdabnet_input->iscale_lag_opt)
			ann_lag.ScaleInputData(pann_input);
		ann_lag.Predict(pann_input,pann_output);
		//since scaling factor for output inside ANN is 1 with 0 mean, no need to unscale output inside ANN
		//ann_lag.UnscaleOutputData(pann_output);
		ppout[i][ioutput] = pann_output[0];
	}
	delete [] pann_input;
	delete [] pann_output;
}

void CDabnet::PredictByReducedModel(int np, double** ppin, double** ppout)
{
	//np: the number of the data points
	//ppin: the 2-D array of input data, ppdata in vd_data can be used for prediction (ignore the output part)
	//pout: the 2-D array of predicted output values without unscaling, only element at index of ioutput is modified and returned
	int i, j, k, n;
	double* prow;
	double* pann_input = new double [nstate_red];
	double* pann_output = new double [noutput];
	prow = ppin[0];
	for (j=0; j<ninput; j++)
		rss_list[j].InitStateVectorToSteadyState(prow[j]);
	for (i=0; i<np; i++)
	{
		prow = ppin[i];
		n = 0;
		for (j=0; j<ninput; j++)
		{
			//rss_list[j].CalcNextStateVector(prow[j]);
			for (k=0; k<rss_list[j].nstate; k++)
				pann_input[n++] = rss_list[j].px[k];
			rss_list[j].CalcNextStateVector(prow[j]);
		}
		if (pdabnet_input->iscale_red_opt)
			ann_red.ScaleInputData(pann_input);
		ann_red.Predict(pann_input,pann_output);
		//since output scaling factor for the ANN is 1 with mean of 0, no need to unscale inside ANN
		//ann_red.UnscaleOutputData(pann_output);
		ppout[i][ioutput] = pann_output[0];
	}
	delete [] pann_input;
	delete [] pann_output;
}

void CDabnet::PredictByDRM(int np, double** ppin, double** ppout)
{
	if (pdabnet_input->ipredict_opt)	//balanced/reduced model
		PredictByReducedModel(np,ppin,ppout);
	else		//Laguerre model
		PredictByLaguerreModel(np,ppin,ppout);
}

void CDabnet::WriteReducedModelStateSpaceMatrices(FILE* pf)
{
	int i;
	for (i=0; i<ninput; i++)
		rss_list[i].WriteStateSpaceMatrices(pf);
}

void CDabnet::ReadReducedModelStateSpaceMatrices(FILE* pf)
{
	int i;
	nstate_red = 0;
	for (i=0; i<ninput; i++)
	{
		rss_list[i].ReadStateSpaceMatrices(pf);
		nstate_red += rss_list[i].nstate;
	}
}

void CDabnet::WriteLaguerreModelStateSpaceMatrices(FILE* pf)
{
	int i;
	for (i=0; i<ninput; i++)
		lss_list[i].WriteStateSpaceMatrices(pf);
}

void CDabnet::WriteDRMTextFile(FILE* pf)
{
	//write reduced model
	fprintf(pf,"%d\t//number of input variables\n",ninput);
	fprintf(pf,"%d\t//number of output variables\n",noutput);
	fprintf(pf,"//list of matrices\n");
	WriteReducedModelStateSpaceMatrices(pf);
	fprintf(pf,"//neural network data\n");
	ann_red.WriteANNData(pf);
}

void CDabnet::WriteDRMMatlabFile(FILE* pf)
{
	//based on ipredict_opt model option, 0=Laguerre, 1=reduced/balanced
	//does not write the mean and sigma data
	int i, j, k;
	int nstate;
	int ioutput1 = ioutput + 1;		//convert from 0-based index to 1-based index for MATLAB
	double** ppa;
	double* pb;
	if (pdabnet_input->ipredict_opt)		//reduced form
	{
		//for each input, write A B matrices
		for (i=0; i<ninput; i++)
		{
			nstate = rss_list[i].nstate;
			ppa = rss_list[i].ppa;
			fprintf(pf,"A{%d,%d} = [",ioutput1,i+1);
			for (j=0; j<nstate; j++)
			{
				for (k=0; k<nstate; k++)
					fprintf(pf,"%lg ",ppa[j][k]);
				fprintf(pf,"\n");
			}
			fprintf(pf,"];\n");
			pb = rss_list[i].pb;
			fprintf(pf,"B{%d,%d} = [",ioutput1,i+1);
			for (j=0; j<nstate; j++)
				fprintf(pf,"%lg ",pb[j]);
			fprintf(pf,"]\';\n");
		}
		ann_red.WriteANNMatlabFile(pf,ioutput);
	}
	else		//Laguerre form
	{
		//for each input, write A B matrices
		for (i=0; i<ninput; i++)
		{
			nstate = lss_list[i].nstate;
			ppa = lss_list[i].ppa;
			fprintf(pf,"A{%d,%d} = [",ioutput1,i+1);
			for (j=0; j<nstate; j++)
			{
				for (k=0; k<nstate; k++)
					fprintf(pf,"%lg ",ppa[j][k]);
				fprintf(pf,"\n");
			}
			fprintf(pf,"];\n");
			pb = lss_list[i].pb;
			fprintf(pf,"B{%d,%d} = [",ioutput1,i+1);
			for (j=0; j<nstate; j++)
				fprintf(pf,"%lg ",pb[j]);
			fprintf(pf,"]\';\n");
		}
		ann_lag.WriteANNMatlabFile(pf,ioutput);
	}
}

void CDabnet::WriteWeightMatrixFile(FILE* pf)
{
	int i;
	for (i=0; i<ninput; i++)
	{
		fprintf(pf, "Weight matrices for Input %d\n",i+1);
		lss_list[i].mweight.WriteText(pf);
	}
}

void CDabnet::Write(FILE* pf)
{
	int i;
	int iversion = 0;
	int nsize;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&ninput,sizeof(int),1,pf);
	fwrite(&noutput,sizeof(int),1,pf);
	fwrite(&ioutput,sizeof(int),1,pf);
	fwrite(&ipole_opt,sizeof(int),1,pf);
	fwrite(&nneuron_hid,sizeof(int),1,pf);
	fwrite(&nstate_lag,sizeof(int),1,pf);
	fwrite(&nstate_red,sizeof(int),1,pf);
	nsize = norder_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&norder_list[i],sizeof(int),1,pf);
	nsize = pole_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&pole_list[i],sizeof(double),1,pf);
	nsize = lss_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		lss_list[i].Write(pf);
	nsize = rss_list.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		rss_list[i].Write(pf);
	ann_lag.Write(pf);
	ann_red.Write(pf);
	//no need to save ann_data
}

void CDabnet::Read(FILE* pf)
{
	CLaguerreStateSpace lss;
	CStateSpace ss;
	int i;
	int iversion;
	int nsize;
	fread(&iversion,sizeof(int),1,pf);
	fread(&ninput,sizeof(int),1,pf);
	fread(&noutput,sizeof(int),1,pf);
	fread(&ioutput,sizeof(int),1,pf);
	fread(&ipole_opt,sizeof(int),1,pf);
	fread(&nneuron_hid,sizeof(int),1,pf);
	fread(&nstate_lag,sizeof(int),1,pf);
	fread(&nstate_red,sizeof(int),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	norder_list.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&norder_list[i],sizeof(int),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	pole_list.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&pole_list[i],sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	lss_list.resize(nsize);
	for (i=0; i<nsize; i++)
		lss_list[i].Read(pf);
	fread(&nsize,sizeof(int),1,pf);
	rss_list.resize(nsize);
	for (i=0; i<nsize; i++)
		rss_list[i].Read(pf);
	ann_lag.Read(pf);
	ann_red.Read(pf);
}
