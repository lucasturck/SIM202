#include "couche.hpp"
#include <iostream>
#include "../vecteurs.hpp"
#include "../constantes.hpp"
#include <random>

using namespace std;
typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;

/************************************************************************
 * Fonctions de la classe Couche
 ************************************************************************/
Couche* Couche::nextC(){
    return reseau->nextC(index);
}

Couche* Couche::prevC(){
    if (index > 0) return reseau->prevC(index);
    return nullptr;
}

/************************************************************************
 * Fonctions de la classe Entree
 ************************************************************************/
void Entree::print(ostream& out) const{
    out << endl << "Entree : " << endl << endl;
    out << "dims = " << dims[0] << " " << dims[1] << " " << dims[2] << endl;
    out << "X = " << X << endl;
    out << "GradX = " << GradX << endl;
    out << "GradP = " << GradP << endl;
    out << "GradPm = " << GradPm << endl;    
}

void Entree::propagation(){
    //cout << "adresse de (*this) : " << this << endl;
    Couche* c = nextC();
    if (c != nullptr){
        c->X = X;
    }
    else{
        cout << "c est nullptr" << endl;
    }
}

/************************************************************************
 * Fonctions de la classe Connexion
 ************************************************************************/

Connexion* Connexion::clone() const{
    Connexion* c;
    c->reseau = reseau;
    c->type = type;
    c->dims[0] = dims[0];
    c->dims[1] = dims[1];
    c->dims[2] = dims[2];
    c->index = index;
    c->X = X;
    c->GradX = GradX;
    c->GradP = GradP;
    c->GradPm = GradPm;
    c->parametres = parametres;
    c->flagP = flagP;
    c->initGradPm = initGradPm;
    c->C = C;
    return c;
}

void Connexion::propagation(){
    //cout << "adresse de (*this) : " << this << endl;
    Couche* c = nextC();
    if (c != nullptr){
        c->X = (*this).C*(*this).X;
    }
    else{
        cout << "c est nullptr" << endl;
    }
}

void Connexion::retroPropagation(){
    //cout << "début retroPropagation" << endl;
    Couche* c = nextC();
    if (c != nullptr){
        //cout << "c n'est pas nullptr" << endl;
        //GradX = transpose(C)*c->GradX;
        GradX = c->GradX*C;
        //cout << "GradX = " << GradX << endl;
    }
    //cout << "fin retroPropagation" << endl;
}

void Connexion::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k){
    //cout << "début majParametres" << endl;
    Couche* c = nextC();
    //c->print(cout);
    if (c != nullptr){
        //cout << "X = " << X << endl;
        matrix Xmat = matrix(dims[0], dims[1], dims[2], X);
        //cout << "Xmat = " << Xmat << endl;
        matrix G = matrix(dims[0], dims[1], dims[2], c->GradX*Xmat);
        //cout << "G = " << G << endl;
        switch (tp){
            case _constant:
                C = C - rho*G;
                //cout << "C = " << C << endl;
                break;
            case _dec_lineaire:
                C = C - rho/(1 + alpha*k)*G;
                break;
            case _quadratique:
                C = C - rho/(1 + alpha*k*k)*G;
                break;
            default:
                cout << "type de pas non défini" << endl;
                exit(-1);
        }
    }
}

void Connexion::print(ostream& out) const{
    out << endl << "Connexion : " << endl << endl;
    out << "dims = " << dims[0] << " " << dims[1] << " " << dims[2] << endl;
    out << "X = " << X << endl;
    out << "GradX = " << GradX << endl;
    out << "GradP = " << GradP << endl;
    out << "GradPm = " << GradPm << endl;
    out << "C = " << endl << C << endl;
}

/************************************************************************
 * Fonctions de la classe Perte
 ************************************************************************/

Reel moindre_carre(const vecteur& X, const vecteur& Y){
    //cout<<"moindre_carre"<<endl;
    //cout << "X = " << X << endl;
    //cout << "Y = " << Y << endl;
    
    vecteur E = X - Y;
    //cout<<"iciici"<<endl;
    return 0.5 * norme(E)/X.size();
}

vecteur dmoindre_carre(const vecteur& X, const vecteur& Y){
    return (X - Y)/X.size();
}

void Perte::setFunPtr(){
    switch (typeP){
        case _moindre_carre:
            fun_perte = moindre_carre;
            dfun_perte = dmoindre_carre;
            break;
        /*
        case _moindre_abs:
            fun_perte = moindre_abs;
            dfun_perte = dmoindre_abs;
            break;
        case _huber:
            fun_perte = huber;
            dfun_perte = dhuber;
            break;
        case _entropie_croisee:
            fun_perte = entropie_croisee;
            dfun_perte = dentropie_croisee;
            break;
        case _softMax:
            fun_perte = softMax;
            dfun_perte = dsoftMax;
            break;
        case _userPerte:
            fun_perte = userPerte;
            dfun_perte = duserPerte;
            break;
        */
        default:
            cout << "type de perte non défini" << endl;
            exit(-1);
    }
}

Perte* Perte::clone() const{
    Perte* p;
    p->reseau = reseau;
    p->type = type;
    p->dims[0] = dims[0];
    p->dims[1] = dims[1];
    p->dims[2] = dims[2];
    p->index = index;
    p->X = X;
    p->GradX = GradX;
    p->GradP = GradP;
    p->GradPm = GradPm;
    p->parametres = parametres;
    p->flagP = flagP;
    p->initGradPm = initGradPm;
    p->typeP = typeP;
    p->fun_perte = fun_perte;
    p->dfun_perte = dfun_perte;
    p->vref = vref;
    return p;
}

void Perte::propagation(){
    GradP = fun_perte(X, vref);
}

void Perte::retroPropagation(){
    GradX = dfun_perte(X, vref);
}

void Perte::print(ostream& out) const{
    out << endl << "Perte : ";
    switch (typeP){
        case _moindre_carre:
            out << "moindre_carre";
            break;
        case _moindre_abs:
            out << "moindre_abs";
            break;
        case _huber:
            out << "huber";
            break;
        case _entropie_croisee:
            out << "entropie_croisee";
            break;
        case _softMax:
            out << "softMax";
            break;
        case _userPerte:
            out << "userPerte";
            break;
        default:
            out << "type de perte non défini";
    }
    out << endl << endl;
    out << "dims = " << dims[0] << " " << dims[1] << " " << dims[2] << endl;
    out << "X = " << X << endl;
    out << "GradX = " << GradX << endl;
    out << "GradP = " << GradP << endl;
    out << "GradPm = " << GradPm << endl;
    out << "vref = " << vref << endl;
}