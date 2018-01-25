#ifndef UTILITIES_H
#define UTILITIES_H

#include "pnl/pnl_matrix.h"
#include "parser.hpp"
#include "Option.hpp"
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Utilities
{
public:

  /**
  * Constructeur par défaut
  */
  Utilities();

  /**
  * Génère la matrice contenant les spots aux dates de constatation antérieures à une date t
  *
  *@param[out] Matrice contenant les spots aux dates de constatation antérieures à t
  *@param[in] past Matrice contenant tous les spots historiques associés à l'option opt
  *@param[in] opt Option
  *@param[in] t Dernière date qui doit être présente dans la matrice retournée
  */
  PnlMat* getConstatationDates(const PnlMat *past, Option *opt, double t);

  /**
  * Génère la matrice contenant les spots aux dates de constatation antérieures à une date t
  *
  *@param[in] constDates Matrice contenant les spots aux dates de constatation antérieures à t. Peut déjà contenir les premières dates de constatation.
  *@param[in] past Matrice contenant tous les spots historiques associés à l'option opt
  *@param[in] opt Option
  *@param[in] t Dernière date qui doit être présente dans la matrice retournée
  */
  void getConstatationDatesFromZero(PnlMat* constDates, const PnlMat* past, Option* opt, double t);

  /**
  *Parse un fichier et renseigne la valeur des champs utiles au lancement du pricer
  *
  *@param[in] infile Fichier à parser
  *@param[in] T maturité
  *@param[in] r taux sans risque
  *@param[in] strike Strike de l'option
  *@param[in] rho correlation
  *@param[in] spot Spots initiaux de l'option
  *@param[in] sigma Volatilité
  *@param[in] divid Dividendes
  *@param[in] lambdas Poids des sous-jacents
  *@param[in] optionType Type de l'option
  *@param[in] size Nombre de sous-jacents
  *@param[in] timestep Pas de discrétisation
  *@param[in] n_samples Nombre de tirages de MonteCarlo
  *@param[in] fdStep pas de discretisation dans MonteCarlo
  */
  void get_param(char *infile, double &T, double &r, double &strike, double &rho,
                PnlVect **spot, PnlVect **sigma, PnlVect **divid, PnlVect **lambdas,
                string &optionType, int &size, int &timestep, size_t &n_samples,
                double &fdStep,PnlVect **trend);

  /**
  * Renvoit le nom du fichier où se trouve la description des données du problème
  *
  *@param[in] argc Nombre d'arguments
  *@param[in] argv Tableau contenant les arguments
  */
  char* getOptDescrAndCheckParam(int argc, char **argv);
};

#endif
