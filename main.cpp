#include "couches/couche.hpp"
#include "constantes.hpp"
#include "classe_reseau/reseau.hpp"
#include <iostream>
#include <random>

using namespace std;

int main()
{
    // Créer les couches
    Couche* c1 = new Entree(2, 1, 1, 0); // 2 entrées (x et y)
    Couche* c2 = new Connexion(1, 2, 1, 1); // 1 sortie (z), 2 entrées (x et y)
    Perte* c3 = new Perte(_moindre_carre, 1, 1, 1, 2); // Perte quadratique
    /*cout << "affichage des adresses des couches" << endl;
    cout << "adresse de c1 : " << c1 << endl;
    cout << "adresse de c1->X : " << &c1->X << endl;
    cout << "adresse de c2 : " << c2 << endl;
    cout << "adresse de c2->X : " << &c2->X << endl;
    cout << "adresse de c3 : " << c3 << endl;
    cout << "adresse de c3->X : " << &c3->X << endl;*/
    

    // Initialiser les poids de la couche de connexion
    // Supposons que les poids sont initialisés pour correspondre à z = ax + by
    // Par exemple, a = 1.0 et b = 2.0
    dynamic_cast<Connexion*>(c2)->mat_C(0, 0) = 5.0; // a
    dynamic_cast<Connexion*>(c2)->mat_C(0, 1) = 5.0; // b
    c2->flagP = true; // La couche de connexion est une couche de paramètres

    // Créer le réseau
    Reseau reseau({c1, c2, c3});
    c1->reseau = &reseau;
    c2->reseau = &reseau;
    c3->reseau = &reseau;

    c1->print(cout);
    c2->print(cout);
    c3->print(cout);

    // Préparer les ensembles de données d'entrée et de sortie
    Entier n = 100; // Nombre d'exemples
    vector<vecteur> Es(n); // 100 exemples d'entrée
    vector<vecteur> Ss(n); // 100 exemples de sortie attendue
    for (int i = 0; i < n; i++)
    {
        Es[i] = {static_cast<Reel>(rand()%10), static_cast<Reel>(rand()%10)}; // Exemple d'entrée (x = i, y = i + 1)
        Ss[i] = {Es[i][0]+2*Es[i][1]}; // Exemple de sortie attendue (z = 2i + 1)
    }
    //vector<vecteur> Es = {{1.0, 2.0}, {2.0, 3.0}, {3.0, 4.0}, {3.0, 5.0}}; // Exemples d'entrées
    //vector<vecteur> Ss = {{5.0}, {8.0}, {11.0}, {13.0}}; // Exemples de sorties attendues

    // Entraîner le réseau
    //reseau.entrainement(Es, Ss, _constant, 0.01, 0.001);
    reseau.entrainement(Es, Ss, _dec_lineaire, 0.01, 0.001);
    //reseau.entrainement(Es, Ss, _quadratique, 0.01, 0.001);

    // Afficher la sortie après l'entraînement
    vecteur E = {1.0, 2.0}; // Exemple d'entrée (x = 1.0, y = 2.0)
    vecteur S = reseau.propagation(E);
    cout << "Sortie du réseau après entraînement : " << S << endl;


    // Nettoyer la mémoire
    delete c1;
    delete c2;
    delete c3;

    return 0;
}