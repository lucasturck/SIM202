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
class Vecteur : public vector<T>
{
public:
  Vecteur(int d=0, const T& v0=T());      // dim et composantes constantes
  Vecteur(const initializer_list<T>& vs); // depuis une liste explicite
  template<typename U> 
  Vecteur(const Vecteur<U>& v);           // depuis un autre vecteur
  T  operator ()(int i) const { return this->at(i-1);} // valeur    1->dim (indice non testé)
  T& operator ()(int i) { return this->at(i-1);}       // référence 1->dim (indice non testé)
  Vecteur<T>& operator +=(const Vecteur<T>& v);   // u += v
  Vecteur<T>& operator -=(const Vecteur<T>& v);   // u -= v
  Vecteur<T>& operator +=(const T& x);            // u += x
  Vecteur<T>& operator -=(const T& x);            // u -= x
  Vecteur<T>& operator *=(const T& x);            // u *= x
  Vecteur<T>& operator /=(const T& x);            // u /= x
}; // fin de définition de la classe
//Foncrions membres
template<typename T>
Vecteur<T>::Vecteur(int d, const T& v0) : vector<T>(d,v0) {}

template<typename T>
Vecteur<T>::Vecteur(const initializer_list<T>& vs) : vector<T>(vs) {}

template<typename T>
template<typename U>
Vecteur<T>::Vecteur(const Vecteur<U>& v) : vector<T>(v.size()){
  for (int i=1; i<=v.size(); i++) (*this)(i) = T(v(i));
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator +=(const Vecteur<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator +=");
  for (int i=1; i<=this->size(); i++) (*this)(i) += v(i);
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator -=(const Vecteur<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator -=");
  for (int i=1; i<=this->size(); i++) (*this)(i) -= v(i);
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator +=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) += x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator -=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) -= x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator *=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) *= x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator /=(const T& x){
  for (int i=1; i<=this->size(); i++) (*this)(i) /= x;
  return *this;
}
//functions externes
template<typename T>
ostream & operator <<(ostream & out, const Vecteur<T>& u){
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
Vecteur<T> operator +(const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i);
  return w;
}

template<typename T>
Vecteur<T> operator -(const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = -u(i);
  return w;
}

template<typename T>
Vecteur<T> operator +(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i) + v(i);
  return w;
}

template<typename T>
Vecteur<T> operator -(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i) - v(i);
  return w;
}

template<typename T>
Vecteur<T> operator +(const Vecteur<T>& u, const T& x){
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i) + x;
  return w;
}

template<typename T>
Vecteur<T> operator -(const T& x, const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = x - u(i);
  return w;
}



template<typename T, typename U>
Vecteur<U> operator *(const T& x, const Vecteur<U>& u){
  Vecteur<U> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = x * u(i);
  return w;
}

template<typename T, typename U>
Vecteur<U> operator *(const Vecteur<U>& u, const T& x){
  Vecteur<U> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i) * x;
  return w;
}

template<typename T, typename U>
Vecteur<U> operator /(const Vecteur<U>& u, const T& x){
  Vecteur<U> w(u.size());
  for (int i=1; i<=u.size(); i++) w(i) = u(i) / x;
  return w;
}

template<typename T>
T operator |(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator |");
  T s = 0;
  for (int i=1; i<=u.size(); i++) s += u(i) * v(i);
  return s;
}

complex<double> operator |(const Vecteur<complexe>& u, const Vecteur<complexe>& v){
  test_dim(u.size(),v.size(),"Vecteur<complexe>::operator |");
  complex<double> s = 0;
  for (int i=1; i<=u.size(); i++) s += u(i) * conj(v(i));
  return s;
}


template<typename T>
double norme(const Vecteur<T>& u){
  return sqrt(real(u|u));
}

template<typename T>
bool operator ==(const Vecteur<T>& u, const Vecteur<T>& v){
  if (u.size() != v.size()) return false;
  for (int i=1; i<=u.size(); i++) if (u(i) != v(i)) return false;
  return true;
}

template<typename T>
bool operator !=(const Vecteur<T>& u, const Vecteur<T>& v){
  return !(u==v);
}
//cas particuliers
template<typename T>
class Matrix : public Vector<T>
{
  public:
    int colone;
    int ligne;
    Matrix(int d1=0,int d2=0,const T& v0=T());      // dim et composantes constantes
    Matrix(int d1,int d2,const initializer_list<T>& vs);
    T operator ()(int i, int j);
};

template<typename T>
Matrix<T>::Matrix(int d1,int d2,const T& v0)
{
  colone=d2;
  ligne=d1;
  vector<T>(d1*d2,v0);
}
template<typename T>
Matrix<T>::Matrix(int d1,int d2,const initializer_list<T>& vs)
{ 
  colone=d2;
  ligne=d1;
  vector<T>(vs);
}
template<typename T>
T Matrix<T>::operator ()(int i, int j)
{
    return *this((j-1)*(this->ligne)+i-1);
}
#endif
