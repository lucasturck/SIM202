#include "couche.hpp"
#include "reseau.hpp"

Couche* Couche::nextC(){
    if (index < reseau->couches.size()-1) return reseau->couches[index+1];
    return nullptr;
}

Couche* Couche::prevC(){
    if (index > 0) return reseau->couches[index-1];
    return nullptr;
}

void Entree::print(ostream& out) const {
    out << "Entree(" << dims[0] << "," << dims[1] << "," << dims[2] << ")";
}

void Convolution::randomK(Entier p, Entier q){
    K = Matrice(p,q);
    K.random();
}

void Convolution::propagation(){
    // calcul de la convolution
    Entier d0 = dims[0], d1 = dims[1], d2 = dims[2];
    Entier p = K.dims[0], q = K.dims[1];
    Entier d0p = d0-p+1, d1q = d1-q+1;
    Vecteur Y(d0p*d1q*d2);
    for (Entier k=0; k<d2; k++){
        for (Entier i=0; i<d0p; i++){
            for (Entier j=0; j<d1q; j++){
                Reel s = 0;
                for (Entier u=0; u<p; u++){
                    for (Entier v=0; v<q; v++){
                        s += K(u,v)*X(k,d0*(i+u)+j+v);
                    }
                Y(k,d1q*i+j) = s;
                }
            }
        }
    }
    X = Y;
}