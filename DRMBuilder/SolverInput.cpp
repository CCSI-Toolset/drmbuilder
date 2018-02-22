//SolverInput.cpp
#include "stdafx.h"
#include "SolverInput.h"

CSolverInput::CSolverInput()
{
	ireverse = 1;
	ndim = 2;
	npoint = 3;
	nduration = 1;
	duration0 = 5;
	nstep = 0;
	dt_sampling = 0.1;
	dt_min_solver = 0.001;
	vduration.clear();
	vduration.push_back(10);
	vsequence.clear();
}

CSolverInput::CSolverInput(int ndi, int np, int ndu)
{
	ireverse = 0;
	ndim = ndi;
	npoint = np;
	nduration = ndu;
	duration0 = 5;
	dt_sampling = 1;
	vduration.resize(ndu);
}

CSolverInput::~CSolverInput()
{
}

void CSolverInput::SetDurationVector(std::vector<int>* pd)
{
	nduration = pd->size();
	vduration.resize(nduration);
	for (int i=0; i<nduration; i++)
		vduration[i] = pd->at(i);
}

void CSolverInput::GenerateTrainingSequence()
{
	//based on the size of the problem estimate the number of tries
	int ntry = 10*npoint*nduration;
	ts.SetSizeAll(ndim, npoint, nduration);
	ts.SetDefaultStartingPoint();
	ts.SelectBestSampling(ntry);
}

void CSolverInput::WriteAPMonitorInputFile(FILE* pf, int nostep)
{
	int i, j, k, m, n;
	int** ppx;
	double t = 0;
	double* px;		//parameter value
	double* pdx;	//interval between two neighboring points
	double* plower;	//lower value
	double* pupper;	//upper value
	std::vector<std::string> input_names;
	std::list<CInputVariable>::iterator ite;
	px = new double [ndim];
	pdx = new double [ndim];
	plower = new double [ndim];
	pupper = new double [ndim];
	input_names.resize(ndim);
	i = 0;
	for (ite=pinput_list_sel->begin(); ite!=pinput_list_sel->end(); ite++)
	{
		px[i] = (*ite).xdefault;
		plower[i] = (*ite).xlower;
		pupper[i] = (*ite).xupper;
		input_names[i] = (*ite).name;
		i++;
	}
	//evenly spaced distance
	for (i=0; i<ndim; i++)
		pdx[i] = (pupper[i]-plower[i])/(double)(npoint-1);
	CLHS* plhs = ts.GetSampleList();
	fprintf(pf,"time");
	for (i=0; i<ndim; i++)
		fprintf(pf,",%s",input_names[i].c_str());
	fprintf(pf,"\n");
	//first point
	for (n=0; n<duration0; n++)
	{
		fprintf(pf,"%lg", t);
		for (m=0; m<ndim; m++)
			fprintf(pf,",%lg",px[m]);
		fprintf(pf,"\n");
		t += dt_sampling;
	}
	if (!nostep)
	{
		//now start loop for each duration, each point, and each time step
		for (i=0; i<nduration; i++)
		{
			ppx = plhs[i].GetLHSPoints();
			for (j=1; j<npoint; j++)
			{
				for (k=0; k<ndim; k++)
				{
					px[k] = ppx[j][k]*pdx[k] + plower[k];
					for (n=0; n<vduration[i]; n++)
					{
						fprintf(pf,"%lg",t);
						for (m=0; m<ndim; m++)
							fprintf(pf,",%lg",px[m]);
						fprintf(pf,"\n");
						t += dt_sampling;
					}
				}
			}
		}
		//write reverse order
		if (ireverse)
		{
			for (i=nduration-1; i>=0; i--)
			{
				ppx = plhs[i].GetLHSPoints();
				for (j=npoint-2; j>=0; j--)
				{
					for (k=ndim-1; k>=0; k--)
					{
						px[k] = ppx[j][k]*pdx[k] + plower[k];
						for (n=0; n<vduration[i]; n++)
						{
							fprintf(pf,"%lg",t);
							for (m=0; m<ndim; m++)
								fprintf(pf,",%lg",px[m]);
							fprintf(pf,"\n");
							t += dt_sampling;
						}
					}
				}
			}
		}
	}
	delete [] px;
	delete [] pdx;
	delete [] plower;
	delete [] pupper;
}

void CSolverInput::WriteACMInputFile(FILE* pf)
{
	int i, j, k, m, n;
	int jstart;
	int ndim_all = pinput_list_all->size();
	int* piall2sel;	//index mapping from all to selected
	int** ppx;
	double t = 0;
	double* px;		//parameter value
	double* px_all;	//initial or default values for all parameters
	double* pdx;	//interval between two neighboring points
	double* plower;	//lower value
	double* pupper;	//upper value
	std::vector<std::string> input_names;
	std::list<CInputVariable>::iterator ite;
	piall2sel = new int [ndim_all];
	px = new double [ndim];
	px_all = new double [ndim_all];
	pdx = new double [ndim];
	plower = new double [ndim];
	pupper = new double [ndim];
	input_names.resize(ndim_all);
	//assign piall2sel
	i = 0;
	j = 0;
	for (ite=pinput_list_all->begin(); ite!=pinput_list_all->end(); ite++)
	{
		piall2sel[i] = -1;
		px_all[i] = (*ite).xdefault;
		if ((*ite).bvaried)
			piall2sel[i] = j++;
		input_names[i] = (*ite).name;
		i++;
	}
	i = 0;
	for (ite=pinput_list_sel->begin(); ite!=pinput_list_sel->end(); ite++)
	{
		px[i] = (*ite).xdefault;
		plower[i] = (*ite).xlower;
		pupper[i] = (*ite).xupper;
		i++;
	}
	//evenly spaced distance
	for (i=0; i<ndim; i++)
		pdx[i] = (pupper[i]-plower[i])/(double)(npoint-1);
	CLHS* plhs = ts.GetSampleList();
	//first point
	for (n=0; n<duration0; n++)
	{
		m = 0;
		if (piall2sel[m]==-1)
			fprintf(pf,"%lg",px_all[m]);
		else
			fprintf(pf,"%lg",px[piall2sel[m]]);
		for (m=1; m<ndim_all; m++)
		{
			if (piall2sel[m]==-1)
				fprintf(pf,",%lg",px_all[m]);
			else
				fprintf(pf,",%lg",px[piall2sel[m]]);
		}
		fprintf(pf,"\n");
		t += dt_sampling;
	}
	
	//now start loop for each duration, each point, and each time step
	for (i=0; i<nduration; i++)
	{
		ppx = plhs[i].GetLHSPoints();
		for (j=1; j<npoint; j++)
		{
			for (k=0; k<ndim; k++)
			{
				px[k] = ppx[j][k]*pdx[k] + plower[k];
				for (n=0; n<vduration[i]; n++)
				{
					m = 0;
					if (piall2sel[m]==-1)
						fprintf(pf,"%lg",px_all[m]);
					else
						fprintf(pf,"%lg",px[piall2sel[m]]);
					for (m=1; m<ndim_all; m++)
					{
						if (piall2sel[m]==-1)
							fprintf(pf,",%lg",px_all[m]);
						else
							fprintf(pf,",%lg",px[piall2sel[m]]);
					}
					fprintf(pf,"\n");
					t += dt_sampling;
				}
			}
		}
	}
	//write reverse order
	if (ireverse)
	{
		for (i=nduration-1; i>=0; i--)
		{
			ppx = plhs[i].GetLHSPoints();
			if (i==nduration-1)
				jstart = npoint-2;
			else
				jstart = npoint-1;
			for (j=jstart; j>0; j--)
			{
				for (k=0; k<ndim; k++)
				{
					px[k] = ppx[j][k]*pdx[k] + plower[k];
					for (n=0; n<vduration[i]; n++)
					{
						m = 0;
						if (piall2sel[m]==-1)
							fprintf(pf,"%lg",px_all[m]);
						else
							fprintf(pf,"%lg",px[piall2sel[m]]);
						for (m=1; m<ndim_all; m++)
						{
							if (piall2sel[m]==-1)
								fprintf(pf,",%lg",px_all[m]);
							else
								fprintf(pf,",%lg",px[piall2sel[m]]);
						}
						fprintf(pf,"\n");
						t += dt_sampling;
					}
				}
			}
		}
	}
	delete [] piall2sel;
	delete [] px;
	delete [] px_all;
	delete [] pdx;
	delete [] plower;
	delete [] pupper;
}

void CSolverInput::PrepareSequenceVectorForACMInputArray()
{
	//vsequence is the array to be passed to MATLAB engine
	int i, j, k, m, n;
	int jstart;
	int ndim_all = pinput_list_all->size();
	int* piall2sel;	//index mapping from all to selected
	int** ppx;
	double* px;		//parameter value
	double* px_all;	//initial or default values for all parameters
	double* pdx;	//interval between two neighboring points
	double* plower;	//lower value
	double* pupper;	//upper value
	std::list<CInputVariable>::iterator ite;
	piall2sel = new int [ndim_all];
	px = new double [ndim];
	px_all = new double [ndim_all];
	pdx = new double [ndim];
	plower = new double [ndim];
	pupper = new double [ndim];
	vsequence.clear();
	//assign piall2sel
	i = 0;
	j = 0;
	for (ite=pinput_list_all->begin(); ite!=pinput_list_all->end(); ite++)
	{
		piall2sel[i] = -1;
		px_all[i] = (*ite).xdefault;
		if ((*ite).bvaried)
			piall2sel[i] = j++;
		i++;
	}
	i = 0;
	for (ite=pinput_list_sel->begin(); ite!=pinput_list_sel->end(); ite++)
	{
		px[i] = (*ite).xdefault;
		plower[i] = (*ite).xlower;
		pupper[i] = (*ite).xupper;
		i++;
	}
	//evenly spaced distance
	for (i=0; i<ndim; i++)
		pdx[i] = (pupper[i]-plower[i])/(double)(npoint-1);
	CLHS* plhs = ts.GetSampleList();
	//start to prepare sequence array
	nstep = 0;
	//first point is the steady state point
	for (n=0; n<duration0; n++)
	{
		m = 0;
		if (piall2sel[m]==-1)
			vsequence.push_back(px_all[m]);
		else
			vsequence.push_back(px[piall2sel[m]]);
		for (m=1; m<ndim_all; m++)
		{
			if (piall2sel[m]==-1)
				vsequence.push_back(px_all[m]);
			else
				vsequence.push_back(px[piall2sel[m]]);
		}
		nstep++;
	}
	//now start loop for each duration, each point, and each time step
	for (i=0; i<nduration; i++)
	{
		ppx = plhs[i].GetLHSPoints();
		for (j=1; j<npoint; j++)
		{
			for (k=0; k<ndim; k++)
			{
				px[k] = ppx[j][k]*pdx[k] + plower[k];
				for (n=0; n<vduration[i]; n++)
				{
					m = 0;
					if (piall2sel[m]==-1)
						vsequence.push_back(px_all[m]);
					else
						vsequence.push_back(px[piall2sel[m]]);
					for (m=1; m<ndim_all; m++)
					{
						if (piall2sel[m]==-1)
							vsequence.push_back(px_all[m]);
						else
							vsequence.push_back(px[piall2sel[m]]);
					}
					nstep++;
				}
			}
		}
	}
	//write reverse order
	if (ireverse)
	{
		for (i=nduration-1; i>=0; i--)
		{
			ppx = plhs[i].GetLHSPoints();
			if (i==nduration-1)
				jstart = npoint-2;
			else
				jstart = npoint-1;
			for (j=jstart; j>0; j--)
			{
				for (k=0; k<ndim; k++)
				{
					px[k] = ppx[j][k]*pdx[k] + plower[k];
					for (n=0; n<vduration[i]; n++)
					{
						m = 0;
						if (piall2sel[m]==-1)
							vsequence.push_back(px_all[m]);
						else
							vsequence.push_back(px[piall2sel[m]]);
						for (m=1; m<ndim_all; m++)
						{
							if (piall2sel[m]==-1)
								vsequence.push_back(px_all[m]);
							else
								vsequence.push_back(px[piall2sel[m]]);
						}
						nstep++;
					}
				}
			}
		}
	}
	delete [] piall2sel;
	delete [] px;
	delete [] px_all;
	delete [] pdx;
	delete [] plower;
	delete [] pupper;
}

void CSolverInput::Write(FILE* pf)
{
	int i;
	int iversion = 0;
	int nsize = vduration.size();
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&ireverse,sizeof(int),1,pf);
	fwrite(&ndim,sizeof(int),1,pf);
	fwrite(&npoint,sizeof(int),1,pf);
	fwrite(&nduration,sizeof(int),1,pf);
	fwrite(&duration0,sizeof(int),1,pf);
	fwrite(&nstep,sizeof(int),1,pf);
	fwrite(&dt_sampling,sizeof(double),1,pf);
	fwrite(&dt_min_solver,sizeof(double),1,pf);
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&vduration[i],sizeof(int),1,pf);
	nsize = vsequence.size();
	fwrite(&nsize,sizeof(int),1,pf);
	for (i=0; i<nsize; i++)
		fwrite(&vsequence[i],sizeof(double),1,pf);
	ts.Write(pf);
}

void CSolverInput::Read(FILE* pf)
{
	int i;
	int iversion;
	int nsize;
	fread(&iversion,sizeof(int),1,pf);
	fread(&ireverse,sizeof(int),1,pf);
	fread(&ndim,sizeof(int),1,pf);
	fread(&npoint,sizeof(int),1,pf);
	fread(&nduration,sizeof(int),1,pf);
	fread(&duration0,sizeof(int),1,pf);
	fread(&nstep,sizeof(int),1,pf);
	fread(&dt_sampling,sizeof(double),1,pf);
	fread(&dt_min_solver,sizeof(double),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	vduration.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&vduration[i],sizeof(int),1,pf);
	fread(&nsize,sizeof(int),1,pf);
	vsequence.resize(nsize);
	for (i=0; i<nsize; i++)
		fread(&vsequence[i],sizeof(double),1,pf);
	ts.Read(pf);
}