
#include "couches/couche.hpp" //contient deja les autres headers
#include <random>


int main(){
    srand(time(NULL));

    Entree Centree(2,1,1);
    Matrice A(20,2,1.0);
    Matrice B(20,20,1.0);
    Matrice C(1,20,1.0);
    Matrice D(1,1,1.0);
    /**/
    for (int i = 1; i <= 20; i++)
    {
        for (int j = 1; j <= 2; j++)
        {
            A(i,j) = static_cast<Reel>(rand()) / RAND_MAX;
        }
        for (int j = 1; j <= 20; j++)
        {
            B(i,j) = static_cast<Reel>(rand()) / RAND_MAX;
        }
    }
    for (int i = 1; i <= 20; i++)
    {
        C(1,i) = static_cast<Reel>(rand()) / RAND_MAX;
    }
    Connexion Cconnexion_1(A,1,20,1);
    Cconnexion_1.flagP = true;
    //Cconnexion_1.print(cout);
    Activation Cactivation_1(_tanh);
    //Cactivation_1.print(cout);
    Connexion Cconnexion_2(B,20,20,1);
    //Cconnexion_2.print(cout);
    Activation Cactivation_2(_tanh);
    //Cactivation_2.print(cout);
    Connexion Cconnexion_3(C,1,1,1);
    //Cconnexion_3.print(cout);
    Activation Cactivation_3(_tanh);
    //Cactivation_3.print(cout);
    //Connexion Cconnexion_4(D,1,1,1);
    //Cconnexion_4.print(cout);
    Perte Cperte(_moindre_carre);
    //Cperte.print(cout);

    Cperte.setFunPtr();
    // def du reseau
    Reseau R({&Centree,&Cconnexion_1,&Cactivation_1,&Cconnexion_2,&Cactivation_2,&Cconnexion_3,&Cactivation_3,&Cperte},"reseau1");
    for(int i=0;i<(R.sizereseau());i++)
    {
        R.chgmtindex(i);
    }
    R.init_reseau();//link entre couches et reseau

    // Préparer les ensembles de données d'entrée et de sortie
    /**/
    Entier n = 200;// nbneur*100; // Nombre d'exemples
    vector<Vecteur> Es(n); // 100 exemples d'entrée
    vector<Vecteur> Ss(n); // 100 exemples de sortie attendue
    for (int i = 0; i < n; i++)
    {
        Es[i].resize(2);
        Ss[i].resize(1);
        Es[i][0] = static_cast<Reel>(rand()) / RAND_MAX; // x
        Es[i][1] = static_cast<Reel>(rand()) / RAND_MAX; // y
        Ss[i][0] = cos(M_PI * Es[i][0]) * sin(M_PI * Es[i][1]); // cos(pi*x) * sin(pi*y)
    }
    //vector<Vecteur> Es = {{0.25,0.25}};
    //vector<Vecteur> Ss = {{cos(M_PI * Es[0][0]) * sin(M_PI * Es[0][1])}};
    //cout << "Es[0] = " << Es[0] << endl;
    //cout << "Ss[0] = " << Ss[0] << endl;

    //entrainenemnt
    R.entrainement(Es, Ss, _dec_lineaire, 0.01, 0.001);

    //test du reseau
    Vecteur TE({0.25,0.25});
    Vecteur TS({cos(M_PI * TE[0]) * sin(M_PI * TE[1])});
    //R.propagation(TE,TS);
    cout << "TE = " << TE << endl;
    cout << "TS = " << TS << endl;
    cout << "R.propagation(TE,TS) = " << R.propagation(TE,TS) << endl;

    // print de la fin du reseau
    //R.print(cout);
}

/*
int reconstructionAX()
{
    Vecteur X{2,4};
    // def des couches
    Entree Centree(2,1,1);
    Matrice A(1,2,1.1);
    A(1,1)=1.0;
    A(1,2)=2.0;
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
    Entier n = 300;// nbneur*100; // Nombre d'exemples
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
    Vecteur TS({X[0]*TE[0]+X[1]*TE[1]});
    R.propagation(TE,TS);


    // print de la fin du reseau
    R.print(cout);
    

    return 0;
}
*/