#pragma once

#include <cassert>
#include <math.h>
#include <stdexcept>
#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "Utilities.hpp"

class MonteCarlo
{
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    PnlRng *rng_; /*! pointeur sur le générateur */
    Utilities u;
    double fdStep_; /*! pas de différence finie */
    int nbSamples_; /*! nombre de tirages Monte Carlo */

    /**
     * Constructeur par default
     *
     * Attention : attribut initialises a NULL
     */
    MonteCarlo();

    /**
     * Constructeur par default
     *    - definie le générateur rng_
     *    - initialise le pas et le nombre de tirages
     *
     */
    MonteCarlo(double fdStep, int nbSamples);

    /**
     * Constructeur
     *
     * @param[in] opt reference vers une option
     * @param[in] mod reference vers un modele Black Scholes
     *
     * Ne serait-il pas mieux d'avoir des references constantes en param[in] ?
     */
    MonteCarlo(double fdStep, int nbSamples, Option *opt, BlackScholesModel *mod);

    /**
     * Constructeur par recopie
     *
     * Necessite un acces public aux attributs de cette classe
     */
    MonteCarlo(const MonteCarlo&);

    /**
     * Destructeur
     *
     */
    ~MonteCarlo();

    /**
     * Calcule le prix de l'option à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     */
    void price(double &prix, double &ic);

   /**
     * Calcule le prix de l'Eurostral à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     */
    void priceEurostral(double &prix, double &ic);

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);

  /**
     * Calcule le prix de l'Eurostral à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void priceEurostral(const PnlMat *past, double t, double &prix, double &ic);

  /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */
    void delta(const PnlMat *past, double t, PnlVect *delta);


   /**
     * Calcule le delta du produit Eurostral100  à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */
    void deltaEurostral(const PnlMat *past, double t, PnlVect *delta);

    /**
     * Calcule le Profit & Loss
     *
     *@param[in] past trajectoire du sous-jacent
     *@param[in] H nombre de rebalancements
     *@param[out] pl renvois le resultat du Profit & Loss
     */
    void profitLoss(const PnlMat *past, double H, double &pl);

    /**
    *Initialisation du Profit & Loss
    *
    *@param[in] past trajectoire du sous-jacent
    *@param[out] delta Contient les deltas en 0
    */
    double PL_init(const PnlMat *past, PnlVect *delta);

    /**
    *Construit le vecteur contenant le montant placé au taux sans risque
    *
    *@param[in] past trajectoire du sous-jacent
    *@param[in] H nombre de rebalancements
    *@param[out] delta contient les deltas au temps courant
    *@param[out] V le vecteur contenant le montant placé au taux sans risque
    */
    void PL_build_V(const PnlMat *past, double H, PnlVect *delta, PnlVect *V);

    /**
    *Calcul du PL au point T_H
    *
    *@param[in] past trajectoire du sous-jacent
    *@param[out] delta Contient les deltas en 0
    *@param[] v_h montant placé au taux sans risque en T_H
    *@param[] pl renvois le resultat du Profit & Loss en T_H
    */
    void PL_finalSet(const PnlMat *past, double v_h, PnlVect *delta, double &pl);
  	/**
   	* Retourne le coeffient d'actualisation divisé par le nombre de tirages
    *
    * @param[in] t date à laquelle le calcul est fait
   	*/
  	double actualisation(double t);
    void PL_build_V_Eurostral(const PnlMat *past, double H,PnlVect *delta, PnlVect *V);
    double PL_init_Eurostral(const PnlMat *past, PnlVect *delta);
    void PL_finalSet_Eurostral(const PnlMat *past, double v_h, PnlVect *delta, double& pl);
    void profitLoss_Eurostral(const PnlMat *past, double H, double &pl);
    /**
     * Operateur d'assignation
     *
     * @param[in] reference constant a un MonteCarlo
     *
     */
    MonteCarlo& operator= (const MonteCarlo&);

};
