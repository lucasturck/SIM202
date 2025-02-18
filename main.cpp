
#include "couches/couche.hpp" //contient deja les autres headers

int main()
{
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
    
    // def des vecteurs d entree et de sortie
    Vecteur E0({1,1});
    Vecteur E1({2,1});
    Vecteur E2({4,7});
    Vecteur E3({3,10});
    Vecteur E4({0,1});
    Vecteur E5({3,0});
    Vecteur E6({-3,1});
    Vecteur E7({-8,-9});
    Vecteur E8({0,-9});
    Vecteur E9({-2,-4});
    Vecteur E10({16,2});
    Vecteur E11({-8,0});
    Vecteur E12({3,3});
    Vecteur E13({1,19});
    Vecteur E14({0,-1});
    Vecteur E15({2,4});

    Vecteur S0({2});
    Vecteur S1({3});
    Vecteur S2({11});
    Vecteur S3({13});
    Vecteur S4({1});
    Vecteur S5({3});
    Vecteur S6({-2});
    Vecteur S7({-17});
    Vecteur S8({-9});
    Vecteur S9({-6});
    Vecteur S10({18});
    Vecteur S11({-8});
    Vecteur S12({6});
    Vecteur S13({20});
    Vecteur S14({-1});
    Vecteur S15({6});


    //entrainenemnt
    R.entrainement({E0,E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15},
                    {S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15},_exponentielle);
    

    //test du reseau
    Vecteur TE({10,0});
    Vecteur TS({10});
    R.propagation(TE,TS);


    // print de la fin du reseau
    R.print(cout);
    

    return 0;
}