#ifndef COUCHE_HPP
#define COUCHE_HPP
#include <iostream>
#include "vecteurs.hpp"

typedef float Reel; // alias du type de reels (float ou double)
typedef size_t Entier; // alias du type d’entiers positifs (unsigned int ou size_t)
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};
enum TypePas{_constant,_dec_lineaire,_quadratique,_exonentielle};
typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;

class Reseau; // forward declaration

class Couche{
    public:
    Reseau* reseau = nullptr; // réseau auquel appartient la couche
    TypeCouche type; // type de la couche
    Entier dims[3]; // dimensions de l’état
    Entier index=0; // index numerique de la couche
    vecteur X; // état de la couche (neurones)
    vecteur GradX; // gradient vs X
    vecteur GradP; // gradient vs paramètres (si paramètres)
    vecteur GradPm; // moyenne de GradP si besoin
    bool parametres; // true si la couche est paramétrée
    bool flagP=false; // premiere couche paramétrée (pour stopper la rétropropagation)
    bool initGradPm=true; // indicateur pour initialiser GradPm=GradP
    // fonctions membres
    Couche* nextC(); // couche suivante (0 si dernière couche)
    Couche* prevC(); // couche précédente (0 si première couche)
    Couche(TypeCouche t=_nondefini, Entier d0=0, Entier d1=0, Entier d2=0): type(t){
        dims[0] = d0; dims[1] = d1; dims[2] = d2;
        X = vecteur(d0*d1*d2);
        GradX = vecteur(d0*d1*d2);
        GradP = vecteur(0);
        GradPm = vecteur(0);
        parametres = false;
    }
    virtual Couche* clone() const; // clonage
    virtual void propagation() {}; // mise à jour de l’état X
    virtual void retroPropagation() {}; // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
    virtual void print(ostream& out) const {}; // affichage
};

class Entree : public Couche {
public:
    virtual Entree* clone() const override{ return new Entree(*this); }
    virtual void propagation() override {} // Implémentation vide
    virtual void retroPropagation() override {} // Implémentation vide
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k) override {} // Implémentation vide
    virtual void print(ostream& out) const override;
};

typedef Reel (*FV2_p)(const vecteur&, const vecteur&); // pointeur de fonction Rn x Rp -> R
typedef vecteur (*FV2V_p)(const vecteur&, const vecteur&); // pointeur de fonction Rn x Rp -> Rn
enum TypePerte {_moindre_carre, _moindre_abs, _huber, _entropie_croisee, _softMax, _userPerte};

class Perte {
protected:
    TypePerte typeP; // type de perte
    FV2_p fun_perte; // pointeur sur la fonction perte Rn -> R
    FV2V_p dfun_perte; // pointeur sur la dérivée de la fonction perte Rn -> Rn
    vecteur v_ref; // pour stocker le vecteur attendu
public:
    void setFunPtr(); // initialise les pointeurs de fonction à partir du type de perte (typeP)
    virtual Perte* clone() const;
    virtual void propagation(); // mise à jour de l’état X
    virtual void retroPropagation(); // mise à jour des gradients
    virtual void print(ostream& out) const;
};

#endif