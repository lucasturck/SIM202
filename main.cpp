
#include "couches/couche.hpp" //contient deja les autres headers

int main()
{
    Vecteur X{2,4};
    // def des couches
    Entree Centree(2,1,1);
    Matrice A(1,2,1.1);
    Connexion Cconnexion(A,1,2,1);

    Perte Cperte(_moindre_carre);
    Cperte.setFunPtr();
    // def du reseau
    Reseau R({&Centree,&Cconnexion,&Cperte},"reseau1");
    for(int i=0;i<(R.sizereseau());i++)
    {
        R.chgmtindex(i);
    } 
    R.init_reseau();//link entre couches et reseau
    
    // Préparer les ensembles de données d'entrée et de sortie
    Entier n = 200;// nbneur*100; // Nombre d'exemples
    vector<Vecteur> Es(n); // 100 exemples d'entrée
    vector<Vecteur> Ss(n); // 100 exemples de sortie attendue
    for (int i = 0; i < n; i++)
    {
        Es[i].resize(2);
        Ss[i].resize(1);
        for (int j = 0; j < 2; j++){
            Es[i][j] = rand() %10; // Exemple d'entrée (x = 1.0, y = 2.0)
            Ss[i][0] += X[j]*Es[i][j]; // Exemple de sortie attendue (z = x + y)
        }
    }


    //entrainenemnt
    R.entrainement(Es, Ss, _exponentielle, 0.01, 0.001);

    //test du reseau
    Vecteur TE({10,0});
    Vecteur TS({20});
    R.propagation(TE,TS);


    // print de la fin du reseau
    R.print(cout);
    

    return 0;
}