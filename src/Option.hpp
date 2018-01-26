#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BlackScholesModel.hpp"
/// \brief Classe Option abstraite
class Option
{
public:
  double T_; /// maturité
  int nbTimeSteps_; /// nombre de pas de temps de discrétisation
  int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
  PnlVect *lambdas_;
  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille (N+1) x d
   * contenant une trajectoire du modèle telle que créée
   * par la fonction asset.
   * @return phi(trajectoire)
   */
  virtual double payoff(const PnlMat *path) = 0;
  virtual double exactPrice0(BlackScholesModel *b) =0;
  /**
   * Constructeur de l'Option
   *
   */
  Option(double T1,int nbTimeSteps1,int size1,PnlVect *lambdas1);

  /**
   * Constructeur par défaut de l'Option
   *
   */
  Option();

  /**
   * Destructeur de l'Option
   *
   */
  virtual ~Option();
};
