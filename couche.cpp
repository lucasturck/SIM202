#include "couche.hpp"
#include <iostream>
#include "vecteurs.hpp"
#include <random>

using namespace std;
typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;


void Entree::print(ostream& out) const {
    out << "Entree(" << dims[0] << "," << dims[1] << "," << dims[2] << ")";
}

void Convolution::randomK(Entier p, Entier q){
    K = matrix(p,q);
    for (Entier i=0; i<p; i++){
        for (Entier j=0; j<q; j++){
            K(i, j) = rand()/10;
        }
    }
}

void Convolution::propagation(){
    // calcul de la convolution
    Entier d0 = dims[0], d1 = dims[1], d2 = dims[2];
    Entier p = K.dims[0], q = K.dims[1];
    Entier d0p = d0-p+1, d1q = d1-q+1;
    vecteur Y(d0p*d1q*d2);
    for (Entier k=0; k<d2; k++){
        for (Entier i=0; i<d0p; i++){
            for (Entier j=0; j<d1q; j++){
                Reel s = 0;
                for (Entier u=0; u<p; u++){
                    for (Entier v=0; v<q; v++){
                        s += K(u,v)*X[k*d0*d1+(i+u)*d1+j+v];
                    }
                Y[k*d0p*d1q+i*d1q+j] = s;
                }
            }
        }
    }
    X = Y;
}

void Convolution::retroPropagation(){
    // calcul du gradient
    Entier d0 = dims[0], d1 = dims[1], d2 = dims[2];
    Entier p = K.dims[0], q = K.dims[1];
    Entier d0p = d0-p+1, d1q = d1-q+1;
    vecteur G(d0*d1*d2);
    for (Entier k=0; k<d2; k++){
        for (Entier i=0; i<d0; i++){
            for (Entier j=0; j<d1; j++){
                Reel s = 0;
                for (Entier u=0; u<p; u++){
                    for (Entier v=0; v<q; v++){
                        s += K(u,v)*GradX[k*d0*d1+(i+u)*d1+j+v];
                    }
                G[k*d0*d1+i*d1+j] = s;
                }
            }
        }
    }
    GradX = G;
}

void Convolution::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k){
    // mise à jour du noyau K
    Entier p = K.dims[0], q = K.dims[1];
    Entier d0 = dims[0], d1 = dims[1], d2 = dims[2];
    Entier d0p = d0-p+1, d1q = d1-q+1;
    matrix Gp(p,q);
    for (Entier i=0; i<p; i++){
        for (Entier j=0; j<q; j++){
            Reel s = 0;
            for (Entier k=0; k<d2; k++){
                for (Entier u=0; u<d0p; u++){
                    for (Entier v=0; v<d1q; v++){
                        s += GradX[k*d0*d1+(u+i)*d1+v+j]*X[k*d0*d1+u*d1+v];
                    }
                }
            }
            Gp(i,j) = s;
        }
    }
    K -= rho*Gp;
}

void Convolution::print(ostream& out) const {
    out << "Convolution(" << dims[0] << "," << dims[1] << "," << dims[2] << ")" <<endl;
}

