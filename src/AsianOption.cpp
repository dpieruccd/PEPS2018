#include "AsianOption.hpp"


AsianOption::AsianOption(double strike, double T1, int nbTimeSteps1, int size1, PnlVect *lambdas1)
  :Option(T1,nbTimeSteps1,size1,lambdas1)
{
  K_ = strike;
}

AsianOption::AsianOption()
  :Option()
{}


AsianOption::~AsianOption()
{}
double AsianOption::exactPrice0(BlackScholesModel *b){
    return 0.0;
    

}
double AsianOption::payoff(const PnlMat *path)
{
    double sum_asian1 = 0;// somme sur les pas de temps
    double sum_asian2 = 0;// somme sur les actifs
    int i, j;
    for(i = 0; i < size_; i++)
    {
        for(j = 0; j <= nbTimeSteps_; j++)
            sum_asian1 += MGET(path, j, i);
        sum_asian2 += pnl_vect_get(lambdas_, i) * sum_asian1 / (nbTimeSteps_+1);
        sum_asian1 = 0;
    }
    return( std::max(sum_asian2 - K_, 0.0) );

}
