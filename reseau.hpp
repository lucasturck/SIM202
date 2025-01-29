/*fichier headers pour la classe réseau*/
#ifndef RESEAU_HPP
#define RESEAU_HPP
#include <iostream>
#include <vector>
 class Reseau{
 protected :
    vector<Couche*> couches; // liste des pointeurs des couches du reseau
    string name= ""; // nomgénérique du fichier de sortie
    Vecteur residus; // vecteur des residus
    public:
    Vecteurr&propagation(const Vecteur&E, const Vecteur&S=Vecteur()); // mise à jour des é
        tats X
    void retroPropagation(); // mise a jour des gradients G, Gpar
    void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. de gradient stochastique
    void entrainement(const vector<Vecteur>&Es, const vector<Vecteur>&Ss, TypePas tp,
    Reel rho=1E-2, Reel alpha=1E-3); //entrainement
    void test(const vector<Vecteur>&Es, const vector<Vecteur>&Ss); //test
    void print(ostream&out) const;
 };



#endif