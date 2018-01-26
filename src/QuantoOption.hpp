#ifndef DEF_QUANTOOPTION
#define DEF_QUANTOOPTION

#include "Option.hpp"

class QuantoOption : public Option
{
  double K_; //strike

  public:

    /**
    *Constructeur de l'option basket
    */
    QuantoOption(double strike, double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1);

    /**
    *Constructeur par défaut de l'option basket
    */
    QuantoOption();

    /**
    *Constructeur par défaut de l'option basket
    */
    ~QuantoOption();

    /**
    *Calcul du Payoff de l'option basket
    *@param[in] path trajectoire du sous-jacent
    */
    double payoff(const PnlMat *path);
    double exactPrice0(BlackScholesModel *b);
};

#endif
