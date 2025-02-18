#include "couches/couche.hpp"
#include "constantes.hpp"
#include "classe_reseau/reseau.hpp"
#include <iostream>
#include <random>

using namespace std;

int main()
{
    // Créer les couches
    srand(time(NULL));
    int nbneur = 2; // Nombre de neurones dans la couche cachée
    vecteur X(nbneur); // Entrée
    for (int i = 0; i < nbneur; i++){
        X[i] = rand() %10; // Exemple d'entrée (x = 1.0, y = 2.0)
    }
    Couche* c1 = new Entree(nbneur, 1, 1, 0); // 2 entrées (x et y)
    Couche* c2 = new Connexion(1, nbneur, 1, 1); // 1 sortie (z), 2 entrées (x et y)
    Perte* c3 = new Perte(_moindre_carre, 1, 1, 1, 2); // Perte quadratique
    

    // Initialiser les poids de la couche de connexion
    // Supposons que les poids sont initialisés pour correspondre à z = ax + by
    // Par exemple, on initialise : a = 5.0 et b = 5.0, coefficients que l'on veut apprendre et qui doivent tendre vers a = 1.0 et b = 2.0
    //dynamic_cast<Connexion*>(c2)->mat_C(0, 0) = 5.0; // a
    //dynamic_cast<Connexion*>(c2)->mat_C(0, 1) = 5.0; // b
    for (int i = 0; i < nbneur; i++){
        dynamic_cast<Connexion*>(c2)->mat_C(0, i) = 1.0;
    }
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
    Entier n = 3;// nbneur*100; // Nombre d'exemples
    vector<vecteur> Es(n); // 100 exemples d'entrée
    vector<vecteur> Ss(n); // 100 exemples de sortie attendue
    for (int i = 0; i < n; i++)
    {
        Es[i].resize(nbneur);
        Ss[i].resize(1);
        for (int j = 0; j < nbneur; j++){
            Es[i][j] = rand() %10; // Exemple d'entrée (x = 1.0, y = 2.0)
            Ss[i][0] += X[j]*Es[i][j]; // Exemple de sortie attendue (z = x + y) 
        }
    }

    // Entraîner le réseau
    //reseau.entrainement(Es, Ss, _constant, 0.01, 0.001);
    //reseau.entrainement(Es, Ss, _dec_lineaire, 0.01, 0.001);
    //reseau.entrainement(Es, Ss, _quadratique, 0.01, 0.001);
    reseau.entrainement(Es, Ss, _exponentielle, 0.01, 0.001);

    // Afficher la sortie après l'entraînement
    //vecteur E = {1.0, 2.0}; // Exemple d'entrée (x = 1.0, y = 2.0)
    vecteur S = reseau.propagation(X); 
    cout << "entrée du réseau : " << X << endl;
    cout << "Sortie du réseau après entraînement : " << S[0] << endl;
    cout << "Sortie attendue : " << (X|X) << endl;


    // Nettoyer la mémoire
    delete c1;
    delete c2;
    delete c3;

    return 0;
}