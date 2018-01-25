#include "Utilities.hpp"


Utilities::Utilities(){}

PnlMat* Utilities::getConstatationDates(const PnlMat* past, Option* opt, double t){

  //We copy all the data corresponding to the constatation dates occuring before t
  PnlVect vectInit = pnl_vect_wrap_mat_row(past, 0);
  PnlMat* res = pnl_mat_create_from_zero(1, past->n);
  pnl_mat_set_row(res, &vectInit, 0);

  int i = 1;
  while (t > (opt->T_ / opt->nbTimeSteps_) * i){
    PnlVect vect = pnl_vect_wrap_mat_row(past, floor(i * (past->m -1) / opt->nbTimeSteps_ ));
    pnl_mat_add_row(res, res->m, &vect);
    i++;
  }
  if (t!=0){
    PnlVect vectFin = pnl_vect_wrap_mat_row(past, floor(t*(opt->nbTimeSteps_ / opt->T_)*((past->m -1)/ opt->nbTimeSteps_)));
    pnl_mat_add_row(res, res->m, &vectFin);
  }

  return res;
}

void Utilities::getConstatationDatesFromZero(PnlMat* constDates, const PnlMat* past, Option* opt, double t){
  //the date corresponding to the last line of constDates must be inferior or equal to t

  PnlVect vectInit = pnl_vect_wrap_mat_row(past, 0);
  pnl_mat_set_row(constDates, &vectInit, 0);
  int initSizeMat = constDates->m;

  int i = 1;
  while (t > (opt->T_ / opt->nbTimeSteps_) * i){
    if (i == initSizeMat-1){
      PnlVect vect = pnl_vect_wrap_mat_row(past, floor(i * (past->m -1) / opt->nbTimeSteps_ ));
      pnl_mat_set_row(constDates, &vect, i);
    }
    if (i > initSizeMat) {
      PnlVect vect = pnl_vect_wrap_mat_row(past, floor(i * (past->m -1) / opt->nbTimeSteps_ ));
      pnl_mat_add_row(constDates, constDates->m, &vect);
    }
    i++;
  }
  if (t!=0){
    PnlVect vectFin = pnl_vect_wrap_mat_row(past, floor(t*(opt->nbTimeSteps_ / opt->T_)*((past->m -1)/ opt->nbTimeSteps_)));
    if (i < initSizeMat){
      pnl_mat_set_row(constDates, &vectFin, i);
    } else {
      pnl_mat_add_row(constDates, constDates->m, &vectFin);
    }
  }
}



void Utilities::get_param(char *infile, double &T, double &r, double &strike, double &rho,
              PnlVect **spot, PnlVect **sigma, PnlVect **divid, PnlVect **lambdas,
              string &optionType, int &size, int &timestep, size_t &n_samples,
              double &fdStep, PnlVect **trend)
{
  Param *P = new Parser(infile);
  P->extract("option type", optionType);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", *spot, size);
  P->extract("volatility", *sigma, size);
  P->extract("payoff coefficients", *lambdas, size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);
  P->extract("trend", *trend,size);
  if (P->extract("fd step", fdStep) == false){
    fdStep = 0.1;
  }
  if (P->extract("dividend rate", *divid, size) == false)
  {
      *divid = pnl_vect_create_from_zero(size);
  }
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("timestep number", timestep);
  //Check that the format of the input values was correct
  assert(size > 0);
  assert(T > 0);
  assert(r > 0);
  assert(strike > 0);
  if (size > 1){
    assert(rho > -(1. / (size - 1.)));
    assert(rho < 1);
  }
  assert((*spot)->size == size);
  assert((*sigma)->size == size);
  assert((*divid)->size == size);
  assert((*lambdas)->size == size);
  assert(n_samples > 0);
  assert(timestep > 0);
  assert(fdStep > 0);

  //Free the memory
  delete P;
}



char* Utilities::getOptDescrAndCheckParam(int argc, char **argv){
  if ((argc != 2) && (argc != 4) && (argc != 6)){
    throw string("Wrong number of parameters: must be \n./pricer [[-d date] -c market_file] data_input \nDo ./pricer --help for more information");
  }
  if (argc == 2){
    if (strcmp(argv[1], "--help") == 0){
      std::cout << "./pricer data_input : computes the price (with the associated confidence interval) and the deltas at t=0. Also gives the timing of the function." << std::endl;
      std::cout << "./pricer -c market_file data_input : computes the P&L." << std::endl;
      std::cerr << "./pricer -d date -c market_ftrend               <vector>  1 1 1 1 1ile data_input : computes the price (with the associated confidence interval) and the detas at t=date." << std::endl;
      exit(0);
    } else {
      return argv[1];
    }
  }
  if (argc == 4){
    if (strcmp(argv[1], "-c") == 0){
      return argv[3];
    } else {
      throw string("Wrong way of calling the function: must be \n./pricer [[-d date] -c market_file] data_input \nDo ./pricer --help for more information");
    }
  }
  if (argc == 6){
    if ((strcmp(argv[1], "-d") == 0) && (strcmp(argv[3], "-c") == 0)){
      return argv[5];
    } else {
      throw string("Wrong way of calling the function: must be \n./pricer [[-d date] -c market_file] data_input \nDo ./pricer --help for more information");
    }
  }
}
