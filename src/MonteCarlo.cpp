//
#include <iostream>
#include <string>
#include <typeinfo>
#include "MonteCarlo.hpp"

using namespace std ;

MonteCarlo::MonteCarlo()
{
    rng_ = NULL;
    opt_ = NULL;
    mod_ = NULL;
    fdStep_ = 0.1;
    nbSamples_ = 0;
}

MonteCarlo::MonteCarlo(double fdStep, int nbSamples):
MonteCarlo()
{
    rng_ = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng_, time(NULL));
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

MonteCarlo::MonteCarlo(double fdStep, int nbSamples, Option *opt, BlackScholesModel *mod):
MonteCarlo(fdStep, nbSamples)
{
    opt_ = opt;
    mod_ = mod;
}

MonteCarlo::~MonteCarlo()
{
    pnl_rng_free(&rng_);
}

void MonteCarlo::price(double &prix, double &ic)
{
    PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    double sum = 0, sq_sum = 0, payoff_, cf_act_ = actualisation(0.0);
//
// reduction on two variables
// firstprivate to make a copy of each object
//
// #pragma omp parallel for reduction(+:sum, sq_sum) firstprivate(path_, mod_, opt_, rng_)

    for(int M = 1; M <= nbSamples_; M++)
    {
      mod_->asset(path_, opt_->T_, opt_->nbTimeSteps_, rng_);
      payoff_ = opt_->payoff(path_);
      sum += payoff_;
      sq_sum += payoff_ * payoff_;
    }
    prix = cf_act_ * sum;
    ic = 1.96 * cf_act_ * sqrt(sq_sum - pow(sum, 2) / nbSamples_);
    pnl_mat_free(&path_);
}

void MonteCarlo::priceEurostral(double &prix, double &ic)
{
    PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    double sum = 0, sq_sum = 0, payoff_, cf_act_ = actualisation(0.0);
//
// reduction on two variables
// firstprivate to make a copy of each object
//
// #pragma omp parallel for reduction(+:sum, sq_sum) firstprivate(path_, mod_, opt_, rng_)

    for(int M = 1; M <= nbSamples_; M++)
    {
      mod_->assetEurostral(path_, opt_->T_, opt_->nbTimeSteps_, rng_);
      payoff_ = opt_->payoff(path_);
      sum += payoff_;
      sq_sum += payoff_ * payoff_;
    }
    prix = cf_act_ * sum;
    ic = 1.96 * cf_act_ * sqrt(sq_sum - pow(sum, 2) / nbSamples_);
    pnl_mat_free(&path_);
}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
{
  if (t> opt_->T_){
    throw std::string("t is greater than the maturity");
  }
  PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
  double sum = 0, sq_sum = 0, payoff_, cf_act_ = actualisation(t);
  PnlVect *vect=pnl_vect_create_from_zero(2);
  PnlVect *pathb=pnl_vect_create_from_zero(2);
  for(int M = 1; M <= nbSamples_; M++)
  {
    mod_->asset(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
    //pnl_mat_print(path_);
    payoff_ = opt_->payoff(path_);
    pnl_mat_get_row(pathb,path_,1);
    pnl_vect_plus_vect(vect,pathb);
	  sum += payoff_;
    sq_sum += payoff_ * payoff_;
  }

  //pnl_vect_print(vect);
  prix = cf_act_ * sum;
  ic = 1.96 * cf_act_ * sqrt(sq_sum - pow(sum, 2) / nbSamples_);
  pnl_mat_free(&path_);
}

void MonteCarlo::priceEurostral(const PnlMat *past, double t, double &prix, double &ic)
{
  if (t> opt_->T_){
    throw std::string("t is greater than the maturity");
  }
  PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
  double sum = 0, sq_sum = 0, payoff_, cf_act_ = actualisation(t);
  PnlVect *vect=pnl_vect_create_from_zero(2);
  PnlVect *pathb=pnl_vect_create_from_zero(2);
  for(int M = 1; M <= nbSamples_; M++)
  {
    mod_->assetEurostral(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
    //pnl_mat_print(path_);
    payoff_ = opt_->payoff(path_);
    pnl_mat_get_row(pathb,path_,1);
    pnl_vect_plus_vect(vect,pathb);
	  sum += payoff_;
    sq_sum += payoff_ * payoff_;
  }

  //pnl_vect_print(vect);
  prix = cf_act_ * sum;
  ic = 1.96 * cf_act_ * sqrt(sq_sum - pow(sum, 2) / nbSamples_);
  pnl_mat_free(&path_);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta){
    if (t> opt_->T_)
      throw std::string("t is greater than the maturity");

    PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *path_Plus = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *path_Minus = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);

    double sum = 0, payoff_ = 0.0;

    for (int d = 0; d < mod_->size_; d++)
    {
      for(int M = 1; M <= nbSamples_; M++)
      {
        mod_->asset(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        mod_->shiftAsset(path_Plus, path_, d, fdStep_ ,t, opt_->T_ / opt_->nbTimeSteps_ );
        mod_->shiftAsset(path_Minus, path_, d, -fdStep_ ,t, opt_->T_ / opt_->nbTimeSteps_ );
        payoff_ = opt_->payoff(path_Plus) - opt_->payoff(path_Minus);
        sum += payoff_;
      }
      sum = sum * actualisation(t) * (1/ (2* fdStep_ *MGET(past,past->m -1,d)));
      pnl_vect_set(delta,d,sum);
    }


    double tmp;
    //cout << typeid(opt_).name() << endl;
    pnl_mat_free(&path_);
    pnl_mat_free(&path_Plus);
    pnl_mat_free(&path_Minus);
}

void MonteCarlo::deltaEurostral(const PnlMat *past, double t, PnlVect *delta){
    if (t> opt_->T_)
      throw std::string("t is greater than the maturity");

    PnlMat *path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *path_Plus = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);
    PnlMat *path_Minus = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, mod_->size_);

    double sum = 0, payoff_ = 0.0;

    for (int d = 0; d < mod_->size_; d++)
    {
      for(int M = 1; M <= nbSamples_; M++)
      {
        mod_->assetEurostral(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        mod_->shiftAsset(path_Plus, path_, d, fdStep_ ,t, opt_->T_ / opt_->nbTimeSteps_ );
        mod_->shiftAsset(path_Minus, path_, d, -fdStep_ ,t, opt_->T_ / opt_->nbTimeSteps_ );
        payoff_ = opt_->payoff(path_Plus) - opt_->payoff(path_Minus);
        sum += payoff_;
      }
      sum = sum * actualisation(t) * (1/ (2* fdStep_ *MGET(past,past->m -1,d)));
      pnl_vect_set(delta,d,sum);
    }


    double tmp;
    //cout << typeid(opt_).name() << endl;
    tmp = pnl_vect_get(delta,1) / pnl_mat_get(path_,0,3);
    pnl_vect_set(delta,1,tmp);
    tmp = pnl_vect_get(delta,2) / pnl_mat_get(path_,0,4);
    pnl_vect_set(delta,2,tmp);
    tmp = exp(pnl_vect_get(mod_->trends_,1)*opt_->T_)*(pnl_mat_get(path_,0,1)*pnl_vect_get(delta,1)-pnl_vect_get(delta,3));
    pnl_vect_set(delta,3,tmp);
    tmp = exp(pnl_vect_get(mod_->trends_,2)*opt_->T_)*(pnl_mat_get(path_,0,2)*pnl_vect_get(delta,2)-pnl_vect_get(delta,4));
    pnl_vect_set(delta,4,tmp);

    pnl_mat_free(&path_);
    pnl_mat_free(&path_Plus);
    pnl_mat_free(&path_Minus);
}

double MonteCarlo::PL_init(const PnlMat *past, PnlVect *delta)
{
    PnlVect S = pnl_vect_wrap_mat_row(past, 0);
    PnlMat *currPast = pnl_mat_create_from_zero(1, past->n);
    double p, ic;

    pnl_mat_extract_subblock(currPast, past, 0, 1, 0, past->n);

    this->delta(currPast, 0.0, delta);
    this->price(p, ic);
    p -= pnl_vect_scalar_prod(delta, &S);

    pnl_mat_free(&currPast);
    return(p);
}

double MonteCarlo::PL_init_Eurostral(const PnlMat *past, PnlVect *delta){
    PnlVect *S =pnl_vect_create_from_zero(past->n);
    pnl_mat_get_row(S,past, 0);
    PnlMat *currPast = pnl_mat_create_from_zero(1, past->n);
    double p, ic;
    double r_dollar= -1*pnl_vect_get(mod_->trends_,3) + mod_->r_;
    double r_aus= -1*pnl_vect_get(mod_->trends_,4) + mod_->r_;
    pnl_mat_extract_subblock(currPast, past, 0, 1, 0, past->n);

    this->delta(currPast, 0.0, delta);
    this->price(p, ic);
    //p -= pnl_vect_scalar_prod(delta, &S);
    p-= pnl_vect_get(delta,0)*pnl_vect_get(S,0);
    p-=pnl_vect_get(delta,1)*pnl_vect_get(S,1)*pnl_vect_get(S,3);
    p-= pnl_vect_get(delta,2)*pnl_vect_get(S,2)*pnl_vect_get(S,4);
    p-=pnl_vect_get(delta,3)*exp(-(opt_->T_* r_dollar))*pnl_vect_get(S,3);
    p-=pnl_vect_get(delta,4)*exp(-(opt_->T_* r_aus))*pnl_vect_get(S,4);


    pnl_mat_free(&currPast);
    return(p);
}


void MonteCarlo::PL_build_V(const PnlMat *past, double H,
                            PnlVect *delta, PnlVect *V)
{
    pnl_vect_set(V, 0, PL_init(past, delta));

    double expCoef = exp(mod_->r_ * opt_->T_/ H), tho;
    PnlVect *prevDelta = pnl_vect_create_from_zero(past->n);
    PnlMat *cPast = pnl_mat_create_from_zero(1, past->n);

    pnl_vect_clone(prevDelta, delta);

    for(int i = 1; i < H; i++)
    {
        tho = i * opt_->T_ /H;
        u.getConstatationDatesFromZero(cPast, past, opt_, tho);
        this->delta(cPast, tho, delta);

        PnlVect S = pnl_vect_wrap_mat_row(cPast, cPast->m-1);

        pnl_vect_minus_vect(prevDelta, delta);

        pnl_vect_set(V, i,
                     pnl_vect_get(V, i-1) * expCoef +
                     pnl_vect_scalar_prod(prevDelta, &S)
                     );

        pnl_vect_clone(prevDelta, delta);
    }
    pnl_vect_free(&prevDelta);
    pnl_mat_free(&cPast);
}
void MonteCarlo::PL_build_V_Eurostral(const PnlMat *past, double H,PnlVect *delta, PnlVect *V){
    pnl_vect_set(V, 0, PL_init_Eurostral(past, delta));

    double expCoef = exp(mod_->r_ * opt_->T_/ H), tho;
    PnlVect *prevDelta = pnl_vect_create_from_zero(past->n);
    PnlMat *cPast = pnl_mat_create_from_zero(1, past->n);
    double r_dollar= -1*pnl_vect_get(mod_->trends_,3) + mod_->r_;
    double r_aus= -1*pnl_vect_get(mod_->trends_,4) + mod_->r_;
    pnl_vect_clone(prevDelta, delta);
    double trackingErrror;
    double prix,ic;
    for(int i = 1; i < H; i++)
    {
        tho = i * opt_->T_ /H;
        u.getConstatationDatesFromZero(cPast, past, opt_, tho);
        this->delta(cPast, tho, delta);

        PnlVect S = pnl_vect_wrap_mat_row(cPast, cPast->m-1);
        pnl_vect_set(&S,1,pnl_vect_get(&S,1)*pnl_vect_get(&S,3));
        pnl_vect_set(&S,2,pnl_vect_get(&S,2)*pnl_vect_get(&S,4));
        pnl_vect_set(&S,3,exp(-((opt_->T_-tho)* r_dollar))* pnl_vect_get(&S,3)) ;
        pnl_vect_set(&S,4,exp(-((opt_->T_-tho)* r_aus))* pnl_vect_get(&S,4)) ;
        this->priceEurostral(cPast,tho,prix,ic);
        trackingErrror=pnl_vect_scalar_prod(prevDelta, &S)+pnl_vect_get(V,i-1)- prix;
        cout<<trackingErrror<<endl;
        pnl_vect_minus_vect(prevDelta, delta);

        pnl_vect_set(V, i,
                     pnl_vect_get(V, i-1) * expCoef +
                     pnl_vect_scalar_prod(prevDelta, &S)
                     );

        pnl_vect_clone(prevDelta, delta);
    }
    pnl_vect_free(&prevDelta);
    pnl_mat_free(&cPast);
}
void MonteCarlo::PL_finalSet(const PnlMat *past, double v_h,
                             PnlVect *delta, double& pl)
{
    double p, ic;
    PnlMat* constDates = u.getConstatationDates(past, opt_, opt_->T_);
    PnlVect S = pnl_vect_wrap_mat_row(past, past->m-1);

    this->price(constDates, opt_->T_, p, ic);
    pl = v_h + pnl_vect_scalar_prod(delta, &S) - p;

    pnl_mat_free(&constDates);
}
void MonteCarlo::PL_finalSet_Eurostral(const PnlMat *past, double v_h, PnlVect *delta, double& pl)
{
    double p, ic;
    PnlMat* constDates = u.getConstatationDates(past, opt_, opt_->T_);
    PnlVect S = pnl_vect_wrap_mat_row(past, past->m-1);

    this->price(constDates, opt_->T_, p, ic);

    pnl_vect_set(&S,1,pnl_vect_get(&S,1)*pnl_vect_get(&S,3));
    pnl_vect_set(&S,2,pnl_vect_get(&S,2)*pnl_vect_get(&S,4));

    pl = v_h + pnl_vect_scalar_prod(delta, &S) - p;

    pnl_mat_free(&constDates);
}


void MonteCarlo::profitLoss(const PnlMat *past, double H, double &pl)
{
    assert(past->m == H+1);
    PnlVect *V = pnl_vect_create_from_zero(H+1);
    PnlVect *delta = pnl_vect_create_from_zero(past->n);

    PL_build_V(past, H, delta, V);
    pnl_vect_set(V, H, pnl_vect_get(V, H-1) * exp(mod_->r_ * opt_->T_/ H));
    PL_finalSet(past, pnl_vect_get(V, H), delta, pl);

    pnl_vect_free(&V);
    pnl_vect_free(&delta);
}
void MonteCarlo::profitLoss_Eurostral(const PnlMat *past, double H, double &pl)
{
    assert(past->m == H+1);
    PnlVect *V = pnl_vect_create_from_zero(H+1);
    PnlVect *delta = pnl_vect_create_from_zero(past->n);

    PL_build_V_Eurostral(past, H, delta, V);
    pnl_vect_set(V, H, pnl_vect_get(V, H-1) * exp(mod_->r_ * opt_->T_/ H));
    PL_finalSet_Eurostral(past, pnl_vect_get(V, H), delta, pl);

    pnl_vect_free(&V);
    pnl_vect_free(&delta);
}


double MonteCarlo::actualisation(double t)
{
    return(exp(- mod_->r_ * (opt_->T_ - t)) / nbSamples_);
}

MonteCarlo& MonteCarlo::operator= (const MonteCarlo& mc)
{
    fdStep_ = mc.fdStep_;
    nbSamples_ = mc.nbSamples_;
    rng_ = mc.rng_;
    mod_ = mc.mod_;
    opt_ = mc.opt_;
    return(*this);
}
