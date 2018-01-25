#include "BlackScholesModel.hpp"
#include <cmath>
#include <iostream>


BlackScholesModel::BlackScholesModel(int size,double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend):
  BlackScholesModel(size,r,rho,sigma,spot)
{
  trends_ = trend;
}

BlackScholesModel::BlackScholesModel(int size,double r, double rho, PnlVect *sigma, PnlVect *spot)
{
    size_=size;
    r_ = r;
    rho_=rho;
    sigma_ = sigma;
    spot_=spot;
    corr = pnl_mat_create_from_scalar(size_, size_, rho_);
    for(int k = 0; k < size_; k++){
      pnl_mat_set(corr, k, k, 1);
    }
    pnl_mat_chol(corr);
    g = pnl_vect_new();
}


BlackScholesModel::BlackScholesModel(){
  size_=0;
  r_ = 0.0;
  rho_ = 0.0;
  sigma_ = NULL;
  spot_ = NULL;
  corr = NULL;
  g = NULL;
}

BlackScholesModel::~BlackScholesModel()
{
  pnl_mat_free(&corr);
  pnl_vect_free(&g);
}

 void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng){
   pnl_mat_set_row(path, spot_, 0);

   PnlVect pastPrices = *spot_;

   for (int i = 0; i < nbTimeSteps; i++){
       pnl_vect_rng_normal(g, size_, rng);
       for (int j = 0; j < size_; j++){
           PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, j);
           double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
           double sigmaShare = GET(sigma_, j);
           double bs = GET(&pastPrices,j) * exp( (pnl_vect_get(trends_,j) - (pow(sigmaShare,2)/2))*(T/nbTimeSteps) +  sigmaShare*sqrt(T/nbTimeSteps)*prodScal) ;
           pnl_mat_set(path, i+1, j, bs);
       }
       pastPrices = pnl_vect_wrap_mat_row(path, i+1);
   }
 }

 void BlackScholesModel::assetEurostral(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng){
   pnl_mat_set_row(path, spot_, 0);

   PnlVect pastPrices = *spot_;

   for (int i = 0; i < nbTimeSteps; i++){
       pnl_vect_rng_normal(g, size_, rng);
       for (int j = 0; j < size_; j++){
           PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, j);
           double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
           double sigmaShare = GET(sigma_, j);
           double bs = GET(&pastPrices,j) * exp( (pnl_vect_get(trends_,j) - (pow(sigmaShare,2)/2))*(T/nbTimeSteps) +  sigmaShare*sqrt(T/nbTimeSteps)*prodScal) ;

           pnl_mat_set(path, i+1, j, bs);
       }
       pastPrices = pnl_vect_wrap_mat_row(path, i+1);
   }
 }

void BlackScholesModel::asset_static(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng){
   pnl_mat_set_row(path, spot_, 0);

   PnlVect *pastPrices = pnl_vect_copy(spot_);

   for (int i = 0; i < nbTimeSteps; i++){
       PnlMat *correl = pnl_mat_copy(corr);
       pnl_mat_chol(correl);
       pnl_vect_rng_uni(g, size_,0,0,rng);
       //pnl_vect_print(g);
       for (int j = 0; j < size_; j++){
           PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, j);
           double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
           double bs = GET(pastPrices,j) * exp( (pnl_vect_get(trends_,j) - (pow(GET(sigma_, j),2)/2))*(T/nbTimeSteps) +  GET(sigma_, j)*sqrt(T/nbTimeSteps)*prodScal   ) ;
           pnl_vect_set(pastPrices, j, bs);
       }
       pnl_mat_set_row(path, pastPrices, i+1);
       pnl_mat_free(&correl);
   }
   pnl_vect_free(&pastPrices);
 }

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past){
  double discrStep = T/nbTimeSteps;

  //First we compute the value of the next discretisation point: t(i+1)
  int nextDiscrPointIndex = -floor(-(t/discrStep));
  double firstStep = nextDiscrPointIndex*discrStep - t;

  //Then we copy s(t0), s(t1), ... s(ti) in the path matrix
  if (firstStep == 0.0) {//if t is a point of discretisation: t = ti
    pnl_mat_set_subblock(path, past, 0, 0);
    firstStep = discrStep;
    nextDiscrPointIndex += 1;
  } else {
    for (int i=0; i < past->m -1; i++){
      PnlVect vectView = pnl_vect_wrap_mat_row(past, i);
      pnl_mat_set_row(path, &vectView, i);
    }
  }

  //Then we compute the next S
  PnlVect pastPrices = pnl_vect_wrap_mat_row(past, past->m -1);

  for (int i= nextDiscrPointIndex; i < nbTimeSteps+1; i++){
    pnl_vect_rng_normal(g, size_, rng);
    for (int share = 0; share < size_; share++){
        PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, share);
        double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
        double step;
        if (i == nextDiscrPointIndex){
          step = firstStep;
        } else {
          step = discrStep;
        }
        double sigmaShare = GET(sigma_, share);
        double bs = GET(&pastPrices,share) * exp( ( pnl_vect_get(trends_,share)- (pow(sigmaShare,2)/2))*step +  sigmaShare*sqrt(step)*prodScal) ;
        pnl_mat_set(path, i, share, bs);
    }
    pastPrices = pnl_vect_wrap_mat_row(path, i);
  }
}

void BlackScholesModel::assetEurostral(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past){
  double discrStep = T/nbTimeSteps;

  //First we compute the value of the next discretisation point: t(i+1)
  int nextDiscrPointIndex = -floor(-(t/discrStep));
  double firstStep = nextDiscrPointIndex*discrStep - t;

  //Then we copy s(t0), s(t1), ... s(ti) in the path matrix
  if (firstStep == 0.0) {//if t is a point of discretisation: t = ti
    pnl_mat_set_subblock(path, past, 0, 0);
    firstStep = discrStep;
    nextDiscrPointIndex += 1;
  } else {
    for (int i=0; i < past->m -1; i++){
      PnlVect vectView = pnl_vect_wrap_mat_row(past, i);
      pnl_mat_set_row(path, &vectView, i);
    }
  }

  //Then we compute the next S
  PnlVect pastPrices = pnl_vect_wrap_mat_row(past, past->m -1);

  for (int i= nextDiscrPointIndex; i < nbTimeSteps+1; i++){
    pnl_vect_rng_normal(g, size_, rng);
    for (int share = 0; share < size_; share++){
        PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, share);
        double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
        double step;
        if (i == nextDiscrPointIndex){
          step = firstStep;
        } else {
          step = discrStep;
        }
        double sigmaShare = GET(sigma_, share);
        double bs = GET(&pastPrices,share) * exp( (pnl_vect_get(trends_,share) - (pow(sigmaShare,2)/2))*step +  sigmaShare*sqrt(step)*prodScal) ;
        pnl_mat_set(path, i, share, bs);
    }
    pastPrices = pnl_vect_wrap_mat_row(path, i);
  }
}


void BlackScholesModel::simul_market(PnlMat *path, double T, int H, PnlRng *rng){

  pnl_mat_set_row(path, spot_, 0);

  PnlVect pastPrices = *spot_;
  for (int i = 0; i < H; i++){

      pnl_vect_rng_normal(g, size_, rng);
      for (int j = 0; j < size_; j++){

          PnlVect viewCorr = pnl_vect_wrap_mat_row(corr, j);
          double prodScal = pnl_vect_scalar_prod(&viewCorr,g);
          double sigmaShare = GET(sigma_, j);
          double bs = GET(&pastPrices,j) * exp( ( GET(trends_,j)- (pow(sigmaShare,2)/2))*(T/H) +  sigmaShare*sqrt(T/H)*prodScal) ;

          pnl_mat_set(path, i+1, j, bs);

      }
      pastPrices = pnl_vect_wrap_mat_row(path, i+1);
  }

}


void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep){
  pnl_mat_clone(shift_path,path);

  int dep = floor(t/timestep)+1;
  for(int i=dep;i<path->m;i++){
    pnl_mat_set(shift_path,i,d,(1+h)*pnl_mat_get(path,i,d));
  }
}
