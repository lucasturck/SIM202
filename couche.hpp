#ifndef COUCHE_HPP
#define COUCHE_HPP
#include <iostream>
#include "vecteurs.hpp"
#include "reseau.hpp"

typedef Reel float; // alias du type de reels (float ou double)
typedef Entier size_t; // alias du type d’entiers positifs (unsigned int ou size_t)
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};

class Couche{
    public:
    Reseau* reseau = nullptr; // réseau auquel appartient la couche
    TypeCouche type; // type de la couche
    Entier dims[3]; // dimensions de l’état
    Entier index=0; // index numerique de la couche
    Vecteur X; // état de la couche (neurones)
    Vecteur GradX; // gradient vs X
    Vecteur GradP; // gradient vs paramètres (si paramètres)
    Vecteur GradPm; // moyenne de GradP si besoin
    bool parametres; // true si la couche est paramétrée
    bool flagP=false; // premiere couche paramétrée (pour stopper la rétropropagation)
    bool initGradPm=true; // indicateur pour initialiser GradPm=GradP
    // fonctions membres
    Couche(TypeCouche t=_nondefini, Entier d0=0, Entier d1=0, Entier d2=0): type(t){
        dims[0] = d0; dims[1] = d1; dims[2] = d2;
        X = Vecteur(d0*d1*d2);
        GradX = Vecteur(d0*d1*d2);
        GradP = Vecteur(0);
        GradPm = Vecteur(0);
        parametres = false;
    }
    Couche* nextC(); // couche suivante (0 si dernière couche)
    Couche* prevC(); // couche précédente (0 si première couche)
    virtual Couche* clone( return new Entree(*this); ) const; // clonage
    virtual void propagation() {}; // mise à jour de l’état X
    virtual void retroPropagation() {}; // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
    virtual void print(ostream& out) const {}; // affichage
};

class Entree : public Couche {
public:
    virtual Entree* clone() const { return new Entree(*this); }
    virtual void print(ostream& out) const {};
};

class Convolution : public Couche {
protected:
    Matrice K; // noyau de convolution
    Entier mu=1, nu=1; // décalages (stride)
    Entier i0=1, j0=1; // indices de départ
    bool memeTaille=false; // true si la taille d’origine doit être conservée (padding)
public:
    void randomK(Entier p, Entier q=0); // initialisation aléatoire du noyau K
    virtual Convolution* clone() const { return new Convolution(*this); }
    virtual void propagation(); // mise à jour de l’état X
    virtual void retroPropagation(); // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // itération gradient
    virtual void print(ostream& out) const; // affichage de la couche
};

#endif // COUCHE_HPP