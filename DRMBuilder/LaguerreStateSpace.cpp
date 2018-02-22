//LaguerreStateSpace.cpp
#include "stdafx.h"
#include <math.h>
#include "LaguerreStateSpace.h"

#include "DRMManager.h"

CLaguerreStateSpace::CLaguerreStateSpace(void)
{
	irealization = 1;	//Wang's realization makes all state values the same at a steady state 
	nstate = 7;
	a = 0.5;
	pmsg = NULL;
}

CLaguerreStateSpace::~CLaguerreStateSpace(void)
{
}

void CLaguerreStateSpace::InitStateVectorToSteadyState(double u)
{
	//using Wang's realization, all state space terms converges to sqrt(1-a^2)/(1-a)*u
	int i;
	double x_ss1 = u/(1-a);
	double x_ssm = (1+a)/(1-a)*u;
	px[0] = x_ss1;
	for (i=1; i<nstate; i++)
		px[i] = x_ssm;
}

void CLaguerreStateSpace::CreateUnbalancedStateSpace()
{
	//realization based on Page 66 of Zhou et al., 1996 given low-pass and all-pass matrices
	if (nstate<2) return;
	int i, j;
	double beta = 1-a*a;
	double sqrt_beta = sqrt(beta);
	double sqrt_gamma = sqrt(sqrt_beta*a);
	if (irealization)	//based on Liuping Wang's book
		sqrt_gamma = sqrt_beta;
	//assign A matrix
	if (nstate==2)
	{
		ppa[0][0] = a;
		ppa[0][1] = 0;
		ppa[1][0] = sqrt_gamma*sqrt_beta;
		ppa[1][1] = a;
	}
	else
	{
		//upper triangle elements are 0
		//diagonal elements are a
		for (i=0; i<nstate; i++)
		{
			ppa[i][i] = a;
			for (j=i+1; j<nstate; j++)
				ppa[i][j] = 0;
		}
		//one below diagonal
		for (i=1; i<nstate-1; i++)
			ppa[i][i-1] = beta;
		//loop through the element below except the last row
		for (i=2; i<nstate-1; i++)
		{
			for (j=0; j<nstate-i-1; j++)
				ppa[i+j][j] = -a*ppa[i+j-1][j];
		}
		//last row
		ppa[nstate-1][nstate-2] = sqrt_beta*sqrt_gamma;
		for (i=nstate-3; i>=0; i--)
			ppa[nstate-1][i] = -a*ppa[nstate-1][i+1];
	}
	//assign B matrix
	pb[0] = 1;
	//all other element of B is zero
	for (i=1; i<nstate; i++)
		pb[i] = 0;
	//assign C matrix
	pc[nstate-1] = sqrt_gamma;
	for (i=nstate-2; i>=0; i--)
		pc[i] = -a*pc[i+1];
}

void CLaguerreStateSpace::PrepareWeightMatrix(int nneuron, double** ppweight, double* psigma, int iscale)
{
	//consider the normalization by standard deviation
	int i, j;
	mweight.SetDimensions(nneuron, nstate);
	mweight.AllocateMemory();
	if (iscale)
	{
		for (i=0; i<nneuron; i++)
		{
			for (j=0; j<nstate; j++)
				mweight.SetElementTo(i,j,ppweight[i][j]/psigma[j]);
		}
	}
	else
	{
		for (i=0; i<nneuron; i++)
		{
			for (j=0; j<nstate; j++)
				mweight.SetElementTo(i,j,ppweight[i][j]);
		}
	}
}

void CLaguerreStateSpace::RealizeBalancedStateSpace(CStateSpace* pss)
{
	//given the weight matrix of neural network, reduce the state space through balanced realization based on Lyapunov equation
	//pw is W1j hat (nw by mj in Larry's paper). However, we decide to map state variables directly to output. Therefore weights does not to multiply by C matrix
	//pss is the returned reduced state space model
	int i, j;
	int nstate_tc;							//number of states after truncation
	double** ppa_matrix;					//ppa pointer to a matrix
	double** ppa_ss;						//ppa pointer to a state space A
	double* psv = new double [nstate];		//singular value of Hankel matrix
	double* psv_sqrt = new double [nstate];	//sqrt(psv)
	CMatrix am(nstate,nstate,ppa);			//A matrix, need to avoid ppa being deleted
	CMatrix amt(nstate,nstate);				//transpose of A matrix
	CMatrix ami(nstate,nstate);				//inverse of A matrix
	CMatrix amti(nstate,nstate);			//inverse of the transpose of A matrix
	CMatrix pm(nstate,nstate);				//observability Grammian
	CMatrix qm(nstate,nstate);				//controllability Grammian
	CMatrix wwm(nstate,nstate);				//[W1j]'[W1j]
	CMatrix bbm(nstate,nstate);				//[bj][bj]'
	CMatrix rhsm(nstate,nstate);			//right hand side matrix of Lyapunov equations, also as temporary matrix
	CMatrix pmc(nstate,nstate);				//pm = pmc(pmc)'  pmc is Cholesky decomposition of pm
	CMatrix qmc(nstate,nstate);				//qm = qmc(qmc)'  qmc is Cholesky decomposition of qm
	CMatrix hm(nstate,nstate);				//Hankel matrix
	CMatrix um(nstate,nstate);				//U in singular value decomposition of USV'
	CMatrix vm(nstate,nstate);				//V in singular value decomposition
	CMatrix tm(nstate,nstate);				//T matrix, which is sv^(-1/2)U'pmc', used to calculate A(balanced)=TAT^(-1), C(balanced)=CR
	CMatrix tmi(nstate,nstate);				//inverse of T, which is qmcVsv^(-1/2), used to calculate B(balanced)=TB and C(balanced)=CT^(-1)
	CMatrix abm(nstate,nstate);				//balanced A matrix
	//CMatrix cm(nstate,nstate);				//C matrix from pc
	//CMatrix whatm(mweight.nrow,nstate);		//W hat matrix
	//calculate amt
	am.Transpose(&amt);
	//calculate ami
	am.LMatrixInverse(&ami);
	//calculate amti
	ami.Transpose(&amti);
	/*
	//calculate C from pc
	ppa_matrix = cm.ppa;
	for (i=0; i<nstate; i++)
	{
		for (j=0; j<=i; j++)
			ppa_matrix[i][j] = pc[nstate-i-1+j];
		for (j=i+1; j<nstate; j++)
			ppa_matrix[i][j] = 0;
	}
	//mweight.MultiplyByMatrixOnRight(&cm,&whatm);
	//whatm.MultiplyBySelfTransposeOnLeft(&wwm);
	*/
	mweight.MultiplyBySelfTransposeOnLeft(&wwm);
	//calculate bbm based on pb
	ppa_matrix = bbm.ppa;
	for (i=0; i<nstate; i++)
	{
		for (j=0; j<nstate; j++)
			ppa_matrix[i][j] = pb[i]*pb[j];
	}
	//solve observability Grammian P:  P - A'PA = W'W converted to (A')^(-1)P - PA = (A')^(-1)W'W
	amti.MultiplyByMatrixOnRight(&wwm,&rhsm);
	pm.SolveGrammianMatrix(&amti,&am,&rhsm);
	//solve controllability Grammian Q: Q-AQA' = BB' converted to A^(-1)Q - QA' = A^(-1)BB'
	ami.MultiplyByMatrixOnRight(&bbm,&rhsm);
	qm.SolveGrammianMatrix(&ami,&amt,&rhsm);

	//Cholesky decomposition
	pm.CholeskyDecompose(&pmc);
	qm.CholeskyDecompose(&qmc);
	//use rhsm to save pmc'
	pmc.Transpose(&rhsm);
	rhsm.MultiplyByMatrixOnRight(&qmc,&hm);
	hm.SingularValueDecomposition(true,true,&um,&vm,psv);

	//write Hankel singular values to message line
	CString str = "Hankel singular values are [";
	CString str1;
	for (i=0; i<nstate; i++)
	{
		if (i<nstate-1)
			str1.Format("%lg ",psv[i]);
		else
			str1.Format("%lg].\n",psv[i]);
		str += str1;
	}
	pmsg->AddMessageLine(str);

	for (i=0; i<nstate; i++)
		psv_sqrt[i] = sqrt(psv[i]);
	//T inverse matrix
	qmc.MultiplyByMatrixOnRight(&vm,&tmi);
	ppa_matrix = tmi.ppa;
	//multiply diagonal matrix sv^(-1/2) on right
	for (i=0; i<nstate; i++)
	{
		for (j=0; j<nstate; j++)
			ppa_matrix[i][j] /= psv_sqrt[j];
	}
	//T matrix
	pmc.MultiplyByMatrixOnRight(&um,&rhsm);
	rhsm.Transpose(&tm);
	//multiply diagonal matrix sv^(-1/2) on left
	ppa_matrix = tm.ppa;
	for (i=0; i<nstate; i++)
	{
		for (j=0; j<nstate; j++)
			ppa_matrix[i][j] /= psv_sqrt[i];
	}

	/* debug: calculate balanced Grammians to see if they are diagonal
	CMatrix qmbar(nstate,nstate);
	CMatrix pmbar(nstate,nstate);
	tmi.Transpose(&rhsm);
	pm.MultiplyByMatrixOnLeft(&rhsm,&qmbar);
	qmbar.MultiplyByMatrixOnRight(&tmi,&pmbar);
	tm.Transpose(&qmbar);
	qm.MultiplyByMatrixOnRight(&qmbar,&rhsm);
	rhsm.MultiplyByMatrixOnLeft(&tm,&qmbar);
	*/

	//calculate balanced A
	am.MultiplyByMatrixOnLeft(&tm,&rhsm);
	rhsm.MultiplyByMatrixOnRight(&tmi,&abm);
	//truncate the singular values
	for (i=1; i<nstate; i++)
	{
		if (psv[i]*10<psv[i-1])
			break;
	}
	nstate_tc = i;
	pss->nstate = nstate_tc;
	pss->AllocateMemory();
	//assign balanced and truncated A in reduced state space
	ppa_matrix = abm.ppa;
	ppa_ss = pss->ppa;
	for (i=0; i<nstate_tc; i++)
	{
		for (j=0; j<nstate_tc; j++)
			ppa_ss[i][j] = ppa_matrix[i][j];
	}
	//assign balanced and truncated B in reduced state space
	tm.MultiplyByVectorOnRight(pb,psv);
	for (i=0; i<nstate_tc; i++)
		pss->pb[i] = psv[i];
	//assign balanced and truncated C in reduced state space
	tmi.MultiplyByVectorOnLeft(pc,psv);
	for (i=0; i<nstate_tc; i++)
		pss->pc[i] = psv[i];
	//set am.ppa to NULL to avoid being deleted
	am.ppa = NULL;
	delete [] psv;
	delete [] psv_sqrt;
}

void CLaguerreStateSpace::Write(FILE* pf)
{
	int i;
	int iversion = 0;
	bool bppa = ppa!=NULL;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&nstate,sizeof(int),1,pf);
	fwrite(&irealization,sizeof(int),1,pf);
	fwrite(&a,sizeof(double),1,pf);
	fwrite(&bppa,sizeof(bool),1,pf);
	if (bppa)
	{
		for (i=0; i<nstate; i++)
			fwrite(ppa[i],sizeof(double),nstate,pf);
		fwrite(pb,sizeof(double),nstate,pf);
		fwrite(pc,sizeof(double),nstate,pf);
		fwrite(px,sizeof(double),nstate,pf);
	}
	mweight.Write(pf);
}

void CLaguerreStateSpace::Read(FILE* pf)
{
	int i;
	int iversion;
	bool bppa;
	DeleteMemory();
	fread(&iversion,sizeof(int),1,pf);
	fread(&nstate,sizeof(int),1,pf);
	fread(&irealization,sizeof(int),1,pf);
	fread(&a,sizeof(double),1,pf);
	fread(&bppa,sizeof(bool),1,pf);
	if (bppa)
	{
		AllocateMemory();
		for (i=0; i<nstate; i++)
			fread(ppa[i],sizeof(double),nstate,pf);
		fread(pb,sizeof(double),nstate,pf);
		fread(pc,sizeof(double),nstate,pf);
		fread(px,sizeof(double),nstate,pf);
	}
	mweight.Read(pf);
}