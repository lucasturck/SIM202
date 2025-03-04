#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

#include <cmath>
#include "vecteurs.hpp"
#include "matrices.hpp"

using namespace std;

typedef float Reel;// alias du type de reels (float ou double)
typedef size_t Entier; // alias du type d’entiers positifs (unsigned int ou size_t)

typedef Vector<Reel> Vecteur ;
typedef Matrix<Reel> Matrice ;

enum TypeCouche{_nondefini ,_entree , _convolution , _reduction , _activation ,_connexion , _perte};
enum TypePas{_constant,_dec_lineaire,_quadratique,_exponentielle};
enum TypeActivation { _activation_indefini , _relu , _tanh , _tanhsat , _sigmoide , _userActivation } ;
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};


#endif