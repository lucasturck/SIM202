#ifndef constantes_hpp
#define constantes_hpp
#include <cmath>
#include "vecteurs.hpp"
//def des constantes pour eviter les doubles declarations
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};
enum TypePas{_constant,_dec_lineaire,_quadratique,_exonentielle};
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};




#endif