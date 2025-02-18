#ifndef vecteurT_hpp
#define vecteurT_hpp
#include <iostream>
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> complexe;  // alias

// utilitaires
inline void stop(const string& msg) // message d'arrêt
{ cout << "ERREUR classe Vecteur, " << msg;  exit(-1); }
inline void test_dim(int d1, int d2, const string& org) // test dimension
{
  if (d1==d2) return;
  cout << org << " (" << d1 << "," << d2 << ") dimensions incompatibles";
  exit(-1);
}
/*--------------------------------------------------------------------------
                classe vecteur<T> héritée de vector<T>
--------------------------------------------------------------------------*/
template<typename T>
class Vector : public vector<T>
{
public:
  Vector(int d=0, const T& v0=T());      // dim et composantes constantes
  Vector(const initializer_list<T>& vs); // depuis une liste explicite
  template<typename U> 
  Vector(const Vector<U>& v);           // depuis un autre vecteur
  T  operator ()(int i) const { return this->at(i-1);} // valeur    1->dim (indice non testé)
  T& operator ()(int i) { return this->at(i-1);}       // référence 1->dim (indice non testé)
  Vector<T>& operator +=(const Vector<T>& v);   // u += v
  Vector<T>& operator -=(const Vector<T>& v);   // u -= v
  Vector<T>& operator +=(const T& x);            // u += x
  Vector<T>& operator -=(const T& x);            // u -= x
  Vector<T>& operator *=(const T& x);            // u *= x
  Vector<T>& operator /=(const T& x);            // u /= x
}; // fin de définition de la classe
//Foncrions membres
template<typename T>
Vector<T>::Vector(int d, const T& v0) : vector<T>(d,v0) {}

template<typename T>
Vector<T>::Vector(const initializer_list<T>& vs) : vector<T>(vs) {}

template<typename T>
template<typename U>
Vector<T>::Vector(const Vector<U>& v) : vector<T>(v.size()){
  for (int i=1; i<=v.size(); i++) (*this)(i) = T(v(i));
}

template<typename T>
Vector<T>& Vector<T>::operator +=(const Vector<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator +=");
  for (int i=1; i<=this->size(); i++) (*this)(i) += v(i);
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator -=(const Vector<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator -=");
  for (int i=1; i<=this->size(); i++) (*this)(i) -= v(i);
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator +=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) += x;
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator -=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) -= x;
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator *=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) *= x;
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator /=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) /= x;
  return *this;
}
//functions externes
template<typename T>
ostream & operator <<(ostream & out, const Vector<T>& u){
  out << "(";
  for (int i=1; i<=u.size(); i++) {
    out << u(i);
    if (i<u.size()) out << ",";
  }
  out << ")";
  return out;
}
//test commit
template<typename T>
Vector<T> operator +(const Vector<T>& u){
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i);
  return w;
}

template<typename T>
Vector<T> operator -(const Vector<T>& u){
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = -u(i);
  return w;
}

template<typename T>
Vector<T> operator +(const Vector<T>& u, const Vector<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i) + v(i);
  return w;
}

template<typename T>
Vector<T> operator -(const Vector<T>& u, const Vector<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i) - v(i);
  return w;
}

template<typename T>
Vector<T> operator +(const Vector<T>& u, const T& x){
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i) + x;
  return w;
}

template<typename T>
Vector<T> operator -(const T& x, const Vector<T>& u){
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = x - u(i);
  return w;
}



template<typename T, typename U>
Vector<U> operator *(const T& x, const Vector<U>& u){
  Vector<U> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = x * u(i);
  return w;
}

template<typename T, typename U>
Vector<U> operator *(const Vector<U>& u, const T& x){
  Vector<U> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i) * x;
  return w;
}

template<typename T, typename U>
Vector<U> operator /(const Vector<U>& u, const T& x){
  Vector<U> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++) w(i) = u(i) / x;
  return w;
}
template<typename T>
Vector<T> sgn(const Vector<T>& u){
  Vector<T> w(u.size());
  for (long unsigned int i=1; i<=u.size(); i++){if (u(i)>0){w(i)=1;}
                                                else if(u(i)<0){w(i)=-1;}
                                                else{ w(i)=0;}}
  return w;
}

template<typename T>
T operator |(const Vector<T>& u, const Vector<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator |");
  T s = 0;
  for (long unsigned int i=1; i<=u.size(); i++) s += u(i) * v(i);
  return s;
}

inline complex<double> operator |(const Vector<complexe>& u, const Vector<complexe>& v){
  test_dim(u.size(),v.size(),"Vecteur<complexe>::operator |");
  complex<double> s = 0;
  for (long unsigned int i=1; i<=u.size(); i++) s += u(i) * conj(v(i));
  return s;
}


template<typename T>
inline double norme(const Vector<T>& u){
  return sqrt(real(u|u));
}

template<typename T>
inline bool operator ==(const Vector<T>& u, const Vector<T>& v){
  if (u.size() != v.size()) return false;
  for (int i=1; i<=u.size(); i++) if (u(i) != v(i)) return false;
  return true;
}

template<typename T>
inline bool operator !=(const Vector<T>& u, const Vector<T>& v){
  return !(u==v);
}
//cas particuliers
#endif
