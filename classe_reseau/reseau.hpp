/*fichier headers pour la classe réseau*/
#ifndef RESEAU_HPP
#define RESEAU_HPP
#include <iostream>
#include <vector>
#include "../constantes.hpp"
#include "../couches/couche.hpp" //à modifier si le chemin change
using namespace std;
typedef size_t Entier;
typedef float Reel;
typedef Vecteur<Reel> vecteur;
class Couche; //forward declaration
 class Reseau{
 protected :
    vector<Couche*> couches; // liste des pointeurs des couches du reseau
    string name=""; // nomgénérique du fichier de sortie
    vecteur residus; // vecteur des residus
    public:
    Reseau(const initializer_list<Couche*>& vs, const string& nom="non_def") : couches(vector<Couche*>{vs}), name(nom), residus(vecteur(vs.size(),0)){}; //Creation d un reseau à l'aide d une liste de couches et d'un nom
    vecteur&propagation(const vecteur& E, const vecteur& S=vecteur()); // mise à jour des états X
    void retroPropagation(); // mise a jour des gradients G, Gpar
    void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. de gradient stochastique
    void entrainement(const vector<vecteur>&Es, const vector<vecteur>&Ss, TypePas tp,
    Reel rho=1E-2, Reel alpha=1E-3); //entrainement
    void test(const vector<vecteur>&Es, const vector<vecteur>&Ss); //test
    void print(ostream&out) const; //affichage du reseau
 };

#endif