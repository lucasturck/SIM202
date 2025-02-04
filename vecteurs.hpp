#ifndef vecteurT_hpp
#define vecteurT_hpp
#include <iostream>
#include <vector>
#include <complex>
using namespace std;
typedef complex<double> complexe; // alias
inline void stop(const string& msg) // message d’arrêt
{ cout << "ERREUR classe Vecteur, " << msg; exit(-1); }
inline void test_dim(int d1, int d2, const string& org) // test dimension
{
    if (d1 == d2) return;
    cout << org << " (" << d1 << " ," << d2 << ") dimensions incompatibles";
    exit(-1);
}

/*--------------------------------------------------------------
CLASSE GENERIQUE vecteur<T>
--------------------------------------------------------------*/
template<typename T>
class Vecteur : public vector<T>
{
public:
    Vecteur(int d = 0, const T& v0 = T()); // dim et composantes constantes
    Vecteur(const initializer_list<T>& vs); // depuis une liste explicite
    T operator()(int i) const { return this->at(i - 1); } // valeur 1->dim (indice non testé)
    T& operator()(int i) { return this->at(i - 1); } // référence 1->dim (indice non testé)
    Vecteur<T>& operator+=(const Vecteur<T>& v); // u += v
    Vecteur<T>& operator-=(const Vecteur<T>& v); // u -= v
    Vecteur<T>& operator+=(const T& x); // u += x
    Vecteur<T>& operator-=(const T& x); // u -= x
    Vecteur<T>& operator*=(const T& x); // u *= x
    Vecteur<T>& operator/=(const T& x); // u /= x
    template<typename S> Vecteur(const Vecteur<S>& vs); // Vecteur<S>->Vecteur<T>
};

//--------------------------------------------------------------
// IMPLEMENTATION DES FONCTIONS MEMBRES
//--------------------------------------------------------------
// constructeurs-destructeur
template<typename T>
Vecteur<T>::Vecteur(int d, const T& v0) : vector<T>(d, v0) {}

template<typename T>
Vecteur<T>::Vecteur(const initializer_list<T>& vs) : vector<T>(vs) {}

template<typename T> template<typename S>
Vecteur<T>::Vecteur(const Vecteur<S>& vs)
{
    this->assign(vs.begin(), vs.end());
}

template<typename T>
ostream& operator<<(ostream& os, const Vecteur<T>& u)
{
    int n = u.size();
    if (n == 0) { os << "()"; return os; }
    os << "(";
    for (int i = 1; i < n; i++) os << u(i) << ",";
    os << u(n) << ")";
    return os;
}

// opérateurs algébriques
template<typename T>
Vecteur<T>& Vecteur<T>::operator+=(const Vecteur<T>& v)
{
    int n = this->size();
    test_dim(n, v.size(), "op +=");
    for (int i = 0; i < n; i++) this->at(i) += v.at(i);
    return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator-=(const Vecteur<T>& v)
{
    int n = this->size();
    test_dim(n, v.size(), "op -=");
    for (int i = 0; i < n; i++) this->at(i) -= v.at(i);
    return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator+=(const T& x)
{
    for (auto& e : (*this)) e += x;
    return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator-=(const T& x)
{
    for (auto& e : (*this)) e -= x;
    return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator*=(const T& x)
{
    for (auto& e : (*this)) e *= x;
    return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator/=(const T& x)
{
    for (auto& e : (*this)) e /= x;
    return *this;
}

// opérateurs externes
template<typename T> Vecteur<T> operator+(const Vecteur<T>& u) // +u
{ return u; }

template<typename T> Vecteur<T> operator-(const Vecteur<T>& u) // -u
{ Vecteur<T> w = u; return w *= -1.; }

template<typename T> Vecteur<T> operator+(const Vecteur<T>& u, const Vecteur<T>& v) // u+v
{ Vecteur<T> w = u; return w += v; }

template<typename T> Vecteur<T> operator-(const Vecteur<T>& u, const Vecteur<T>& v) // u-v
{ Vecteur<T> w = u; return w -= v; }

template<typename T> Vecteur<T> operator+(const Vecteur<T>& u, const T& x) // u+x
{ Vecteur<T> w = u; return w += x; }

template<typename T> Vecteur<T> operator-(const Vecteur<T>& u, const T& x) // u-x
{ Vecteur<T> w = u; return w -= x; }

template<typename T> Vecteur<T> operator*(const Vecteur<T>& u, const T& x) // u*x
{ Vecteur<T> w = u; return w *= x; }

template<typename T> Vecteur<T> operator/(const Vecteur<T>& u, const T& x) // u/x
{ Vecteur<T> w = u; return w /= x; }

template<typename T> Vecteur<T> operator+(const T& x, const Vecteur<T>& u) // x+u
{ Vecteur<T> w = u; return w += x; }

template<typename T> Vecteur<T> operator-(const T& x, const Vecteur<T>& u) // x-u
{ Vecteur<T> w = -u; return w -= x; }

template<typename T> Vecteur<T> operator*(const T& x, const Vecteur<T>& u) // x*u
{ Vecteur<T> w = u; return w *= x; }

template<typename T> T operator|(const Vecteur<T>& u, const Vecteur<T>& v) // u|v
{
    int n = u.size();
    test_dim(n, v.size(), "op |");
    T s = 0;
    for (int i = 1; i <= n; i++) s += u(i) * v(i);
    return s;
}

template<typename T>
T norme(const Vecteur<T>& u) // sqrt(u|u)
{ return sqrt(u | u); }

template<typename T>
bool operator==(const Vecteur<T>& u, const Vecteur<T>& v)
{
    if (u.size() != v.size()) return false;
    for (int i = 1; i <= int(u.size()); i++)
        if (u(i) != v(i)) return false;
    return true;
}

template<typename T>
bool operator!=(const Vecteur<T>& u, const Vecteur<T>& v)
{ return !(u == v); }

// produit hermitien pour des Vecteurs complexes
template <class T>
complex<T> operator|(const Vecteur<complex<T>>& Z1, const Vecteur<complex<T>>& Z2)
{
    int n = Z1.size();
    test_dim(n, Z2.size(), "op Z1|Z2");
    complex<T> r = 0;
    for (int i = 1; i <= n; i++) r += Z1(i) * conj(Z2(i));
    return r;
}

// norme retournant un réel pour des complexes
template <class T>
T norme(const Vecteur<complex<T>>& v) // norme euclidienne d’un Vecteur complexe
{ return sqrt((v | v).real()); }

//classe Matrix
template <class T>
class Matrix : public Vecteur<T>
{
  public:
    int dim = 0;
    int dims[0];
    Vecteur<T> M;
    Matrix(int d1=0,int d2=0,const T& v0=T());      // dim et composantes constantes
    Matrix(int d1,int d2,const initializer_list<T>& vs);
    T& operator ()(int i, int j);
};

template <class T>
Matrix<T>::Matrix(int d1,int d2,const T& v0)
{
  dim = 2;
  dims[0] = d1;
  dims[1] = d2;
  M=Vecteur<T>(d1*d2,v0);
}

template <class T>
Matrix<T>::Matrix(int d1,int d2,const initializer_list<T>& vs)
{ 
  dim = 2;
  dims[0] = d1;
  dims[1] = d2;
  M=Vecteur<T>(d1*d2);
}

template <class T>
T& Matrix<T>::operator ()(int i, int j)
{
    return (*this).M((j-1)*(this->dims[0])+i-1);
}

#endif
