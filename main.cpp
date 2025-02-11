/*
 * main.cpp
*/
#include "constantes.hpp"
#include "couches/couche.hpp"
#include "classe_reseau/reseau.hpp"
#include "cifar_10_image/image_processing.hpp"
int main()
{
    // vector<vecteur> image;
    // vecteur labels;
    // readCifar10("cifar_10_image/cifar-10-batches-bin/batches_batch_1.bin",image,labels,10000);
    Entree entree(_entree,1,2,1);
    Reseau reseau({&entree,&entree,&entree},"Reseau 1");
    reseau.print(cout);
    return 0;
}

