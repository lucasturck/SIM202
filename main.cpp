
#include "couches/couches.hpp" //contient deja les autres headers
#include "cifar10/image_process.hpp"
#include <random>
int main()
{
    // //////////////////////
    // 
    // 
    //   TEST CAS z=ax+by
    // 
    // 
    // ///////////////////////
    // cout<<"////////////test z=ax+by///////////////"<<endl;
    // Vecteur X{2,4};
    // // def des couches
    // Entree Centree(2,1,1);
    // Matrice A(1,2,1.1);
    // Connexion Cconnexion(A);

    // Perte Cperte(_moindre_carre);
    // // Cperte.setFunPtr();
    // // def du reseau
    // Reseau R({&Centree,&Cconnexion,&Cperte},"reseau z=ax+b");
    
    // // Préparer les ensembles de données d'entrée et de sortie
    // Entier n = 100;// nbneur*100; // Nombre d'exemples
    // vector<Vecteur> Es(n); // 100 exemples d'entrée
    // vector<Vecteur> Ss(n); // 100 exemples de sortie attendue
    // for (int i = 0; i < n; i++)
    // {
    //     Es[i].resize(2);
    //     Ss[i].resize(1);
    //     for (int j = 0; j < 2; j++){
    //         Es[i][j] = rand() %10; // Exemple d'entrée (x = 1.0, y = 2.0)
    //         Ss[i][0] += X[j]*Es[i][j]; // Exemple de sortie attendue (z = x + y)
    //     }
    // }
    // vector<Matrice> Esm(n);
    // vector<Matrice> Ssm(n);
    // for (int i = 0; i < n; i++)
    // {
    //     Esm[i]=Matrice(Es[i]);
    //     Ssm[i]=Matrice(Ss[i]);
    // }

    // //entrainenemnt
    // R.entrainement(Esm, Ssm, _constant, 0.01, 0.001);

    // //test du reseau
    // Vecteur TE({10,0});
    // Vecteur TS({20});
    // R.propagation(Matrice(TE),Matrice(TS));


    // // print de la fin du reseau
    // R.print(cout);

    /////////////////////////////
    //
    //    f(x,y)=sin pi*x cos pi*y
    //
    ///////////////////////////////
    cout<<"//////////////// test f(x,y)=sin pi*x cos pi*y ////////////"<<endl;
    std::random_device rd;  // Générateur basé sur le matériel
    std::mt19937 gen(rd()); // Générateur Mersenne Twister
    std::uniform_real_distribution<double> distrib(-1, 1); // Bornes : -1. à 1.

    // //def couches 
    // Entree Centree2(2,1,1);

    // Matrice C1(20,2,0);
    // for(int i=1;i<=20;i++)
    // {
    //     for(int j=1;j<=2;j++)
    //     {
    //         C1(i,j)=distrib(gen); //matrice avec coeffs aleatoires
    //     }
    // }
    // Connexion connexion1(C1);
    // Matrice C2(20,20,0);
    // for(int i=1;i<=20;i++)
    // {
    //     for(int j=1;j<=20;j++)
    //     {
    //         C2(i,j)=distrib(gen);
    //     }
    // }
    // Connexion connexion2(C2);
    // Matrice C3(1,20,0);
    // for(int i=1;i<=1;i++)
    // {
    //     for(int j=1;j<=20;j++)
    //     {
    //         C3(i,j)=distrib(gen);
    //     }
    // }
    // Connexion connexion3(C3);

    // Activation act1(_tanh);
    // Activation act2(_tanh);
    // Activation act3(_tanh);

    // Perte perte1(_moindre_carre);

    // //def reseau
    // Reseau R2({&Centree2,&connexion1,&act1,&connexion2,&act2,
    //         &connexion3,&act3,&perte1},"f(x,y)=sin pi*x cos pi*y");


    
    // // Préparer les ensembles de données d'entrée et de sortie
    // std::uniform_real_distribution<double> distrib2(0, 1.0); // Bornes : 0 à 1

    // n = 10000;// Nombre d'exemples
    // vector<Vecteur> Es2(n); // n exemples d'entrée
    // vector<Vecteur> Ss2(n); // n exemples de sortie attendue

    // for (int i = 0; i < n; i++)
    // {
    //     Es2[i].resize(2);
    //     Ss2[i].resize(1);
    //     for (int j = 0; j < 2; j++){
    //         Es2[i][j] = distrib2(gen); 
    //         Ss2[i][0] = sin(M_PI*Es2[i][0])*cos(M_PI*Es2[i][1]); // Exemple de sortie attendue  
    //     }
    // }

    // vector<Matrice> Esm2(n);
    // vector<Matrice> Ssm2(n);
    // for (int i = 0; i < n; i++)
    // {
    //     Esm2[i]=Matrice(Es2[i]);
    //     Ssm2[i]=Matrice(Ss2[i]);
        
    // }

    // //entrainement
    // R2.entrainement(Esm2, Ssm2, _dec_lineaire, 0.01, 0.001);

    // //test du reseau
    // Entier p = 100; // Nombre de tests
    // vector<Vecteur> T(p); // n exemples d'entrée
    // vector<Vecteur> S(p); // n exemples de sortie attendue

    // for (int i = 0; i < p; i++)
    // {
    //     T[i].resize(2);
    //     S[i].resize(1);
    //     for (int j = 0; j < 2; j++){
    //         T[i][j] = distrib2(gen); 
    //         S[i][0] = sin(M_PI*T[i][0])*cos(M_PI*T[i][1]); 
    //     }
    // }
    // vector<Matrice> Tm(p);
    // vector<Matrice> Sm(p);
    // for (int i = 0; i < p; i++)
    // {
    //     Tm[i]=Matrice(T[i]);
    //     Sm[i]=Matrice(S[i]);
        
    // }

    // //affichage du reseau
    // R2.print(cout);
    // //affichage de l erreur moyenne
    // R2.test(Tm,Sm);

    ////////////////////////////////////////////////////////////////////
    //
    //  Partie classification d images
    //
    /////////////////////////////////////////////////////////////////
    
    // def des couches
    Entree entree_image(32,32,3);
    Convolution convol1_image(5,5);
    Activation act1_image(_sigmoide);
    Convolution convol7_image(3,3);
    Activation act7_image(_sigmoide);
    Convolution convol8_image(3,3);
    Activation act8_image(_sigmoide);
    Reduction red1_image(_maxReduction,2,2);
    Convolution convol2_image(3,3);
    Activation act2_image(_sigmoide);
    Reduction red2_image(_maxReduction,2,2);
    Matrice Cim(2,147);
    for(int i=1;i<=2;i++)
    {
        for(int j=1;j<=147;j++)
        {
            Cim(i,j)=distrib(gen);
        }
    }
    Connexion con1_image(Cim);
    Activation act3_image(_sigmoide);
    Matrice Cim2(2,2);
    for(int i=1;i<=2;i++)
    {
        for(int j=1;j<=2;j++)
        {
            Cim2(i,j)=distrib(gen);
        }
    }
    Connexion con2_image(Cim2);
    Activation act4_image(_sigmoide);
    Perte per_image(_softMax);

    // init reseau

    Reseau rimage({&entree_image,&convol1_image,&act1_image,&red1_image,
            &red2_image,&con1_image,&act3_image,&con2_image,&act4_image,&per_image},
                "analyse immages");
    // base d entrainement
    vector<Matrice> image;
    Vecteur labels;
    int nbimage=4000;
    readCifar10("cifar10/cifar-10-batches-bin/data_batch_1.bin",image,labels,nbimage);
    vector<Matrice> lab;
    vector<Matrice> ima;
    int nbentr=400;
    ima.resize(nbentr);
    lab.resize(nbentr);
    for(int nb=0;nb<nbimage/nbentr;nb++)
    {
            for(int i=0;i<nbentr;i++)
        {
            if(labels[i +nb*nbentr]==0)//on va essayer de detecter des avions juste
            {
            lab[i]=Matrice(1,1,0);
            }
            else
            {
            lab[i]=Matrice(1,1,1);

            }
            ima[i]=image[i+nb*nbentr];
        }

        //entrainement
        
        rimage.entrainement(ima, lab, _dec_lineaire, 0.01, 0.001);

    }

    // readCifar10("cifar10/cifar-10-batches-bin/test_batch.bin",image,labels,100);
    // lab.resize(labels.size());
    // for(int i=0;i<labels.size();i++)
    // {
    //     lab[i]=Matrice(1,1,labels[i]);
    // }
    // rimage.print(cout);

    // rimage.test(image,lab);
    rimage.print(cout);

    return 0;
}