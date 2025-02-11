#include "../constantes.hpp"
#include <cstdint>
#include <fstream>

using namespace std;
typedef size_t Entier;
typedef float Reel;
typedef Vecteur<Reel> vecteur;

/*Read a CIFAR 10 file (https://www.cs.toronto .edu/~kriz/cifar .html) inside the given containers
 path
 : the path to the Cifar-10 file
 images : the container to fill with the images
 labels : the container to fill with the labels
 limit : the maximum number of elements to read (0: no limit , say 10000)
 image encoding : 3072 = 3x1024 = 3x32x32 (row by row), 8 bits encoding (0->255) */
 void readCifar10(const string& path , vector<vecteur>& images , vecteur& labels , Entier limit )
 { if ( limit==0 || limit >10000) limit=10000;
 ifstream file;
 file.open(path,std::ios::in|std::ios::binary|std::ios::ate) ;
 if(!file) {cout<<"Error␣opening␣ file :"<<path<<endl;exit(-1);}
 Entier file_size = limit *3073;
 char* buffer = new char[ file_size ]; //buffer to read file
 file.seekg(0,std::ios::beg);
 //read the entire file at once
 file.read(buffer,file_size);
 file.close();
 // fill images and labels
 images.resize(limit,vecteur(3072));
 labels.resize(limit);
 for(Entier i = 0; i < limit ; ++i)
 {
 labels[i]=buffer[i*3073]; // first byte is the label
 for( Entier j = 1; j < 3073; ++j)
 images [i][j-1] =uint8_t(buffer[i*3073+j])/Reel(255) ; //normalize byte to to [0. ,1.]
 }
 file.close();
 delete [] buffer ;}
 // deallocate buffer

