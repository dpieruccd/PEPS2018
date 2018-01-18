#include "Option.hpp"

Option::Option(double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1)
  :T_(T1),nbTimeSteps_(nbTimeSteps1),size_(size1),lambdas_(lambdas1)
{}

Option::Option():T_(0),nbTimeSteps_(0),size_(0){}


Option::~Option()
{
}
