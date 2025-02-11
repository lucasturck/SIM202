#include "couche.hpp"
#include <iostream>
#include "vecteurs.hpp"
#include <random>

using namespace std;
typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;

Couche* Couche::nextC(){
    return reseau->nextC(index);
}

Couche* Couche::prevC(){
    if (index > 0) return reseau->prevC(index);
    return nullptr;
}

void Entree::print(ostream& out) const{
    out << "Entree : " << endl;
    out << "dims = " << dims[0] << " " << dims[1] << " " << dims[2] << endl;
    out << "X = " << X << endl;
    out << "GradX = " << GradX << endl;
    out << "GradP = " << GradP << endl;
    out << "GradPm = " << GradPm << endl;    
}


Reel moindre_carre(const vecteur& X, const vecteur& Y){
    vecteur E = X - Y;
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
    Perte* p = new Perte;
    p->typeP = typeP;
    p->fun_perte = fun_perte;
    p->dfun_perte = dfun_perte;
    p->v_ref = v_ref;
    return p;
}

void Perte::propagation(){
    reseau->perte = fun_perte(reseau->couches.back()->X, v_ref);
}

void Perte::retroPropagation(){
    reseau->couches.back()->GradX = dfun_perte(reseau->couches.back()->X, v_ref);
}

void Perte::print(ostream& out) const{
    out << "Perte : ";
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
    out << endl;
}