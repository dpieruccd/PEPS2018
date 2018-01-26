#ifndef DEF_ASIANOPTION
#define DEF_ASIANOPTION

#include "Option.hpp"
#include <algorithm>

class AsianOption : public Option
{
  double K_;

  public:

    /**
    *Constructeur de l'option asiatique
    */
    AsianOption(double strike, double T1, int nbTimeSteps1, int size1, PnlVect *lambdas1);


    /**
    *Constructeur par défaut de l'option asiatique
    */
    AsianOption();

    /**
    *Destructeur de l'option asiatique
    */
    ~AsianOption();

    /**
    *Calcul du Payoff de l'option Asiatique
    *@param[in] path trajectoire du sous-jacent
    */
    double payoff(const PnlMat *path);
    double exactPrice0(BlackScholesModel *b);
};

#endif
