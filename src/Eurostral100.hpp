#ifndef DEF_EUROSTRAL100
#define DEF_EUROSTRAL100

#include "Option.hpp"

class Eurostral100 : public Option
{
  double K_; //strike

  public:

    /**
    *Constructeur de l'option basket
    */
    Eurostral100(double strike, double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1);

    /**
    *Constructeur par défaut de l'option basket
    */
    Eurostral100();

    /**
    *Constructeur par défaut de l'option basket
    */
    ~Eurostral100();

    /**
    *Calcul du Payoff de l'option basket
    *@param[in] path trajectoire du sous-jacent
    */
    double payoff(const PnlMat *path);
    double exactPrice0(BlackScholesModel *b);
};

#endif
