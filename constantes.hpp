#ifndef constantes_hpp
#define constantes_hpp
#include <cmath>
#include "vecteurs.hpp"
#include "matrices.hpp"

//def des constantes pour eviter les doubles declarations
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};
enum TypePas{_constant,_dec_lineaire,_quadratique,_exponentielle};
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};


typedef float Reel; // alias du type de reels (float ou double)
typedef size_t Entier; // alias du type d’entiers positifs (unsigned int ou size_t)


typedef Vector<Reel> vecteur;
typedef Matrix<Reel> matrix;

#endif