//DABNetInput.cpp
#include "stdafx.h"
#include "DABNetInput.h"

CDabnetInput::CDabnetInput()
{
	ipredict_opt = 1;
	itrain_lag_opt = 0;
	itrain_red_opt = 0;
	iscale_lag_opt = 0;
	iscale_red_opt = 1;
	nmax_iter_ipopt_lag = 3000;
	nmax_iter_ipopt_red = 6000;
	nmax_iter_bp_lag = 5000;
	nmax_iter_bp_red = 10000;
	weight_init = 0.01;
	fq_state = 0.01;
}

CDabnetInput::~CDabnetInput()
{
}

CDabnetInput::CDabnetInput(const CDabnetInput &t)
{
	ipredict_opt = t.ipredict_opt;
	itrain_lag_opt = t.itrain_lag_opt;
	itrain_red_opt = t.itrain_red_opt;
	iscale_lag_opt = t.iscale_lag_opt;
	iscale_red_opt = t.iscale_red_opt;
	nmax_iter_ipopt_lag = t.nmax_iter_ipopt_lag;
	nmax_iter_ipopt_red = t.nmax_iter_ipopt_red;
	nmax_iter_bp_lag = t.nmax_iter_bp_lag;
	nmax_iter_bp_red = t.nmax_iter_bp_red;
	weight_init = t.weight_init;
	fq_state = t.fq_state;
}

CDabnetInput& CDabnetInput::operator=(const CDabnetInput& t)
{
	if (this==&t)
		return *this;
	ipredict_opt = t.ipredict_opt;
	itrain_lag_opt = t.itrain_lag_opt;
	itrain_red_opt = t.itrain_red_opt;
	iscale_lag_opt = t.iscale_lag_opt;
	iscale_red_opt = t.iscale_red_opt;
	nmax_iter_ipopt_lag = t.nmax_iter_ipopt_lag;
	nmax_iter_ipopt_red = t.nmax_iter_ipopt_red;
	nmax_iter_bp_lag = t.nmax_iter_bp_lag;
	nmax_iter_bp_red = t.nmax_iter_bp_red;
	weight_init = t.weight_init;
	fq_state = t.fq_state;
	return *this;
}

void CDabnetInput::Write(FILE* pf)
{
	int iversion = 0;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&ipredict_opt,sizeof(int),1,pf);
	fwrite(&itrain_lag_opt,sizeof(int),1,pf);
	fwrite(&itrain_red_opt,sizeof(int),1,pf);
	fwrite(&iscale_lag_opt,sizeof(int),1,pf);
	fwrite(&iscale_red_opt,sizeof(int),1,pf);
	fwrite(&nmax_iter_ipopt_lag,sizeof(int),1,pf);
	fwrite(&nmax_iter_ipopt_red,sizeof(int),1,pf);
	fwrite(&nmax_iter_bp_lag,sizeof(int),1,pf);
	fwrite(&nmax_iter_bp_red,sizeof(int),1,pf);
	fwrite(&weight_init,sizeof(double),1,pf);
	fwrite(&fq_state,sizeof(double),1,pf);
}

void CDabnetInput::Read(FILE* pf)
{
	int iversion;
	fread(&iversion,sizeof(int),1,pf);
	fread(&ipredict_opt,sizeof(int),1,pf);
	fread(&itrain_lag_opt,sizeof(int),1,pf);
	fread(&itrain_red_opt,sizeof(int),1,pf);
	fread(&iscale_lag_opt,sizeof(int),1,pf);
	fread(&iscale_red_opt,sizeof(int),1,pf);
	fread(&nmax_iter_ipopt_lag,sizeof(int),1,pf);
	fread(&nmax_iter_ipopt_red,sizeof(int),1,pf);
	fread(&nmax_iter_bp_lag,sizeof(int),1,pf);
	fread(&nmax_iter_bp_red,sizeof(int),1,pf);
	fread(&weight_init,sizeof(double),1,pf);
	fread(&fq_state,sizeof(double),1,pf);
}