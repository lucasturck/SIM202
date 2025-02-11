#include "couches/couche.hpp"
#include "constantes.hpp"
#include "classe_reseau/reseau.hpp"
#include <iostream>

using namespace std;


int main()
{
    Couche* c1 = new Entree(2, 2, 1);
    c1->print(cout);
    Couche* c2 = new Connexion(2, 2, 1);
    c2->print(cout);
    Couche* c3 = new Perte(_moindre_carre, 2, 2, 1);
    c3->print(cout);
    Reseau reseau({c1, c3});
    return 0;
}