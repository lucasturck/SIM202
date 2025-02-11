#ifndef vecteurT_hpp
#define vecteurT_hpp
#include <iostream>
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> complexe;  
typedef size_t Entier ; // entier positif
typedef float Reel ; // nombre réel
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
                classe Vecteur<T> héritée de vector<T>
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

inline complex<double> operator |(const Vecteur<complexe>& u, const Vecteur<complexe>& v){
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
//classe Matrix
template <typename T>
class Matrix : public Vecteur<T>
{
public:
    Vecteur<T> mat; //matrice comme vecteur
    int n,m; //dim de la matrice
    Matrix(int d=0,int p=0, T v0=T()) : mat(Vecteur(d*p,v0)), n(d),m(p){}
    Matrix (const Vecteur<T>& d) : n(d.size()),m(d.size())
        {
            mat.resize(n*n,T());
            for(int i=0;i<n;i++)
            mat[i*n+i]=d[i];
        }          // constructeur d'une matrice diagonale
    Matrix<T> (const initializer_list<Vecteur<T>>& vs) : n(vs.size()),m((vs.begin())->size())
    {
        mat.resize(n*m);
        int k=0;
        for(auto& li : vs)
        {
            if(li.size()!=size_t(n)) {cout<<"taille d'un vecteur incompatible"<<endl; exit(-1);}
            for(int i=0;i<n;i++,k++) mat[k]=li[i];
        }
    } // constructeur d'une matrice à partir de vecteurs
    T& operator ()(int i, int j); //acces au coef (i,j)
    Vecteur<T> operator()(int i) const; // retourne la ligne i en tant que Vecteur
    Matrix<T>& operator +=(const Matrix& A);
    Matrix<T>& operator -=(const Matrix& A);
    Matrix<T>& operator +=(const T& x);
    Matrix<T>& operator -=(const T& x);
    Matrix<T>& operator *=(const T& a);
    Matrix<T>& operator /=(const T& a);
};
template<typename T>
T& Matrix<T>::operator ()(int i, int j) //acces au coef (i,j)
{
    if(i<=0 || j<=0) {cout<<"coef(i,j) : i,j en dehors des bornes"<<endl; exit(-1);}
     return mat[(i-1)*m+j-1];
}
template<typename T>
Vecteur<T> Matrix<T>::operator ()(int i) const
{
    
    Vecteur<T> V;
    V.resize(m);
    for(int j=0;j<m;j++)
    {
        V[j]=mat[(i-1)*m+j];
    }
    return V;
}
template <typename T>
Matrix<T>& Matrix<T>::operator +=(const Matrix& A)
{
    if(A.n!=n || A.m!=m)
    {
        cout<<"pb de dim"<<endl;
        return *this;
    }
    mat+=A.mat;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator -=(const Matrix& A)
{
    if(A.n!=n || A.m!=m)
    {
        cout<<"pb de dim"<<endl;
        return *this;
    }
    mat-=A.mat;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator +=(const T& x)
{
    mat+=x;
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator -=(const T& x)
{
    mat-=x;
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator *=(const T& x)
{
    mat*=x;
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator /=(const T& x)
{
    if(x==T())
    {
        cout<<"div par 0"<<endl;
        return *this;
    }
    mat/=x;
    return *this;
}
template <typename T>
Matrix<T> operator +(const Matrix<T>& A,const Matrix<T>& B)
{
    Matrix<T> C(A);
    return (C+=B);
}
template <typename T>
Matrix<T> operator -(const Matrix<T>& A,const Matrix<T>& B)
{
    Matrix<T> C(A);
    return (C-=B);
}
template <typename T>
Matrix<T> operator -(const Matrix<T>& A)
{
    Matrix<T> C(A);
    return C*=(-1);
}
template <typename T>
Matrix<T> operator *(const Matrix<T>& A,const Matrix<T>& B)
{
    if(A.m!=B.n){cout<<"pb dim"<<endl; return A;}
    Matrix<T> C(A);
    Matrix<T> D(B);
    Matrix<T> Res=C-C;
    for(int i=1;i<=C.n;i++)
    {
        for(int j=1;j<=C.m;j++)
        {
            for(int s=1;s<=C.m;s++)
            {
                Res(i,j)+=C(i,s)*D(s,j);
            }
        }
    }
    return Res;
}
template <typename T>
Vecteur<T> operator *(const Matrix<T>& A,const Vecteur<T>& V)
{
    if(A.m!=V.size()){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vecteur<T> D(V);
    Vecteur<T> Res(A.n,T());
    for(int i=1;i<=C.n;i++)
    {
        for(int s=1;s<=C.m;s++)
        {
            Res[i-1]+=C(i,s)*D[s-1];
        }
    }
    return Res;
}
template <typename T>
Vecteur<T> operator *(const Vecteur<T>& V,const Matrix<T>& A)
{
    if(A.n!=V.size()){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vecteur<T> D(V);
    Vecteur<T> Res(A.m,T());
    for(int i=1;i<=C.m;i++)
    {
        for(int s=1;s<=C.n;s++)
        {
            Res[i-1]+=C(s,i)*D[s-1];
        }
    }
    return Res;
}
template<typename T>
ostream& operator <<(ostream& out,const Matrix<T>& A)
{
    Matrix<T> R(A);
    for(int i=1;i<=R.n;i++)
        {out<<(R(i));cout<<endl;}
    return out;
}
#endif
