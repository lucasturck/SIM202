#include "couche.hpp"


using namespace std;
typedef Vecteur<Reel> vecteur;
typedef Matrix<Reel> matrix;


void Entree::print(ostream& out) const {
    out << "Entree(" << dims[0] << "," << dims[1] << "," << dims[2] << ")";
}


