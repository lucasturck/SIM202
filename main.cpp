/*
 * main.cpp
*/
#include <iostream>
#include "reseau.hpp"
//#include "couche.hpp"

using namespace std;

int main(){
    cout << "Hello World" << endl;
    Convolution couche = Convolution(_convolution, 3, 3, 3);
    cout << "couche = "<<endl;
    couche.randomK(2, 2);
    cout <<endl;
    cout << "couche = ";
    couche.print(cout);
    couche.propagation();
    cout << "couche = ";
    couche.print(cout);
    return 0;
}