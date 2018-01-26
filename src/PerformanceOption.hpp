#ifndef DEF_PERFORMANCEOPTION
#define DEF_PERFORMANCEOPTION

#include "Option.hpp"

class PerformanceOption : public Option
{
public:

  /**
  *Constructeur de l'option performance
  */
  PerformanceOption(double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1);

  /**
  *Constructeur par defaut de l'option performance
  */
  PerformanceOption();
  double exactPrice0(BlackScholesModel *b);
  /**
  *Destructeur de l'option performance
  */
  ~PerformanceOption();

  /**
  *Calcul du Payoff de l'option Performance
  *@param[in] path trajectoire du sous-jacent
  */
  double payoff(const PnlMat *path);
};

#endif
