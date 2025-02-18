/*fichier headers pour la classe réseau*/
#ifndef RESEAU_HPP
#define RESEAU_HPP
#include "../couches/couche.hpp"
class Couche;//forward declaration
#include "../constantes.hpp"

 class Reseau{
 protected : //je mets en public le temps de trouver une solution
    vector<Couche*> couches; // liste des pointeurs des couches du reseau
    string name= ""; // nomgénérique du fichier de sortie
    Vecteur residus; // vecteur des residus ie S-S_calculé
    public:
    Reseau(const initializer_list<Couche*>& vs, const string& nom) : couches(vector<Couche*>{vs}), name(nom), residus(Vecteur(1,0)){}; //Creation d un reseau à l'aide d une liste de couches et d'un nom
    Couche* at(int i) const; //recuperation d un couche
    int sizereseau() const; //recup taille reseau
    void chgmtindex(int i);//changement de l index de la couche
    void stockS(const Vecteur& S);//pour stocker le vecteur attendu dans la couche perte
    void init_reseau(); //lier les couches au reseau
    Vecteur acces_residus();//acces au residus
    Vecteur&propagation(const Vecteur& E, const Vecteur& S=Vecteur()); // mise à jour des états X
    void retroPropagation(); // mise a jour des gradients G, Gpar
    void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. de gradient stochastique
    void entrainement(const vector<Vecteur>&Es, const vector<Vecteur>&Ss, TypePas tp,
    Reel rho=1E-2, Reel alpha=1E-3); //entrainement
    void test(const vector<Vecteur>&Es, const vector<Vecteur>&Ss); //test
    void print(ostream&out) const; //affichage du reseau
 };

 
#endif