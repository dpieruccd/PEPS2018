#ifndef DEF_BASKETOPTION
#define DEF_BASKETOPTION
#include "BlackScholesModel.hpp"
#include "Option.hpp"

class BasketOption : public Option
{
  double K_; //strike

  public:

    /**
    *Constructeur de l'option basket
    */
    BasketOption(double strike, double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1);

    /**
    *Constructeur par défaut de l'option basket
    */
    BasketOption();

    /**
    *Constructeur par défaut de l'option basket
    */
    ~BasketOption();

    /**
    *Calcul du Payoff de l'option basket
    *@param[in] path trajectoire du sous-jacent
    */
    double payoff(const PnlMat *path);
    double exactPrice0(BlackScholesModel *b);
};

#endif
