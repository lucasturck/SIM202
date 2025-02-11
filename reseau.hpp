/*fichier headers pour la classe réseau*/
#ifndef RESEAU_HPP
#define RESEAU_HPP
#include <iostream>
#include <vector>
#include "vecteurs.hpp"
#include "couche.hpp"

typedef float Reel; // alias du type de reels (float ou double)
typedef size_t Entier; // alias du type d’entiers positifs (unsigned int ou size_t)
//enum TypePas{_constant,_dec_lineaire,_quadratique,_exonentielle};
typedef Vecteur<Reel> vecteur;

class Reseau{
protected :
    vector<Couche*> couches; // liste des pointeurs des couches du reseau
    string name= ""; // nomgénérique du fichier de sortie
    vecteur residus; // vecteur des residus
    public:
    Couche* nextC(int i){ if (i<couches.size()-1) return couches[i+1]; return nullptr;}
    Couche* prevC(int i){ if (i>0) return couches[i-1]; return nullptr;}
    vecteur& propagation(const vecteur& E, const vecteur& S=vecteur()); // mise à jour des états X
    void retroPropagation(); // mise a jour des gradients G, Gpar
    void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. de gradient stochastique
    void entrainement(const vector<vecteur>&Es, const vector<vecteur>&Ss, TypePas tp,
    Reel rho=1E-2, Reel alpha=1E-3); //entrainement
    void test(const vector<vecteur>&Es, const vector<vecteur>&Ss); //test
    void print(ostream&out) const;
};



#endif