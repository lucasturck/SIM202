#ifndef COUCHE_HPP
#define COUCHE_HPP
#include <iostream>
#include "../constantes.hpp"
#include "../classe_reseau/reseau.hpp"


typedef float Reel; // alias du type de reels (float ou double)
typedef size_t Entier; // alias du type d’entiers positifs (unsigned int ou size_t)

typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;

class Reseau; // forward declaration

class Couche{
    public:
    Reseau* reseau = nullptr; // réseau auquel appartient la couche
    //Couche* nextC = nullptr; // couche suivante
    //Couche* prevC = nullptr; // couche précédente
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
    Couche(TypeCouche t=_nondefini, Entier d0=0, Entier d1=0, Entier d2=0): type(t){
        dims[0] = d0; dims[1] = d1; dims[2] = d2;
        X = vecteur(d0*d1*d2);
        GradX = vecteur(d0*d1*d2);
        GradP = vecteur(0);
        GradPm = vecteur(0);
        parametres = false;
    }
    virtual ~Couche() = default;

    virtual Couche* clone() const = 0; // clonage
    virtual void propagation() = 0; // mise à jour de l’état X
    virtual void retroPropagation() = 0; // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k) = 0; // iter. gradient
    virtual void print(ostream& out) const = 0; // affichage
};

class Entree : public Couche {
public:
Entree(TypeCouche t=_entree, Entier d0=0, Entier d1=0, Entier d2=0){
    type=t;
    dims[0] = d0; dims[1] = d1; dims[2] = d2;
    X = vecteur(d0*d1*d2);
    GradX = vecteur(d0*d1*d2);
    GradP = vecteur(0);
    GradPm = vecteur(0);
    parametres = true;
}
    virtual Entree* clone() const override{ return new Entree(*this); }
    virtual void propagation() override {} // Implémentation vide
    virtual void retroPropagation() override {} // Implémentation vide
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k) override {} // Implémentation vide
    virtual void print(ostream& out) const override;
};


typedef Reel (*FV2_p)(const vecteur&, const vecteur&); // pointeur de fonction Rn x Rp−>R
typedef vecteur (*FV2V_p)(const vecteur&, const vecteur&); // pointeur de fonction Rn x Rp−>Rn
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};

class Perte : public Couche
{
    protected :
        TypePerte typeP; // type de perte
        FV2_p fun_perte; // pointeur sur la fonction perte Rn−>R
        FV2V_p dfun_perte; // pointeur sur la derivee de la fonction perte Rn−>Rn
        vecteur vref ; // pour stocker le vecteur attendu
        public:
        void setFunPtr(); // initialise les pointeurs de fonction à partir du type de perte (typeP)
        virtual Perte* clone() const ;
        virtual void propagation (); // mise a jour de l ’état X
        virtual void retroPropagation(); // mise a jour des gradients
        virtual void print(ostream&out) const;
    };

// class Convolution : public Couche {
// protected:
//     matrix K; // noyau de convolution
//     Entier mu=1, nu=1; // décalages (stride)
//     Entier i0=1, j0=1; // indices de départ
//     bool memeTaille=false; // true si la taille d’origine doit être conservée (padding)
// public:
//     Convolution(TypeCouche type, Entier d0, Entier d1, Entier d2) : Couche(type, d0, d1, d2) {}
//     void randomK(Entier p, Entier q=0); // initialisation aléatoire du noyau K
//     virtual Convolution* clone() const override{ return new Convolution(*this); }
//     virtual void propagation() override; // mise à jour de l’état X
//     virtual void retroPropagation() override; // mise à jour des gradients
//     virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k) override; // itération gradient
//     virtual void print(ostream& out) const override; // affichage de la couche
// };

#endif // COUCHE_HPP