#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"

class BlackScholesModel
{
public:
    int size_; /// nombre d'actifs du modèle
    double r_; /// taux d'intérêt
    double rho_; /// paramètre de corrélation
    PnlVect *sigma_; /// vecteur de volatilités
    PnlVect *spot_; /// valeurs initiales du sous-jacent
    PnlVect *trends_; /// tendances
    PnlMat *corr;
    PnlVect *g;

    /**
    *Constructeur
    */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend);

    /**
    *Constructeur
    */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot);

    /**
    *Destructeur
    */
    ~BlackScholesModel();

    /**
    *Constructeur par defaut
    */
    BlackScholesModel();

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

   /**
     * Génère une trajectoire du modèle et la stocke dans path
     * pour l'Eurostral
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void assetEurostral(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);
    /**
    *Calcule une trajectoire du modèle avec un générateur constant
    *utilité: tests
    *
    *@param[out] path contient une trajectoire du modèle
    *@param[in] T maturité
    *@param[in] nbTimeSteps nombre de dates de constatation
    *@param[in] rng Generateur signature a modifier enlever rng!!)
    */

    void asset_static(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);
    
  /**
     * Calcule une trajectoire du sous-jacent connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */
    void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

   /**
     * Calcule une trajectoire du sous-jacent connaissant le
     * passé jusqu' à la date t pour l'Eurostral
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */

    void assetEurostral(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);
    /**
     * Génère une simulation du marché, prend compte de la tendance dans ses calculs
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     *@param[in] rng generateur de nombres aléatoires
     */
    void simul_market(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a été shiftée par (1+h)
     * à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

};
