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
inline void test_dim(Entier d1, Entier d2, const string& org) // test dimension
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
  Vecteur(Entier d=0, const T& v0=T());      // dim et composantes constantes
  Vecteur(const initializer_list<T>& vs); // depuis une liste explicite
  template<typename U> 
  Vecteur(const Vecteur<U>& v);           // depuis un autre vecteur
  T  operator ()(Entier i) const { return this->at(i);} // valeur    1->dim (indice non testé)
  T& operator ()(Entier i) { return this->at(i);}       // référence 1->dim (indice non testé)
  Vecteur<T>& operator +=(const Vecteur<T>& v);   // u += v
  Vecteur<T>& operator -=(const Vecteur<T>& v);   // u -= v
  Vecteur<T>& operator +=(const T& x);            // u += x
  Vecteur<T>& operator -=(const T& x);            // u -= x
  Vecteur<T>& operator *=(const T& x);            // u *= x
  Vecteur<T>& operator /=(const T& x);            // u /= x
}; // fin de définition de la classe
//Foncrions membres
template<typename T>
Vecteur<T>::Vecteur(Entier d, const T& v0) : vector<T>(d,v0) {}

template<typename T>
Vecteur<T>::Vecteur(const initializer_list<T>& vs) : vector<T>(vs) {}

template<typename T>
template<typename U>
Vecteur<T>::Vecteur(const Vecteur<U>& v) : vector<T>(v.size()){
  for (Entier i=0; i<v.size(); i++) (*this)(i) = T(v(i));
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator +=(const Vecteur<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator +=");
  for (Entier i=0; i<this->size(); i++) (*this)(i) += v(i);
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator -=(const Vecteur<T>& v){
  test_dim(this->size(),v.size(),"Vecteur<T>::operator -=");
  for (Entier i=0; i<this->size(); i++) (*this)(i) -= v(i);
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator +=(const T& x){
  for (Entier i=0; i<this->size(); i++) (*this)(i) += x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator -=(const T& x){
  for (Entier i=0; i<this->size(); i++) (*this)(i) -= x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator *=(const T& x){
  for (Entier i=0; i<this->size(); i++) (*this)(i) *= x;
  return *this;
}

template<typename T>
Vecteur<T>& Vecteur<T>::operator /=(const T& x){
  for (Entier i=0; i<this->size(); i++) (*this)(i) /= x;
  return *this;
}
//functions externes
template<typename T>
ostream & operator <<(ostream & out, const Vecteur<T>& u){
  out << "(";
  for (Entier i=0; i<u.size(); i++) {
    out << u(i);
    if (i<u.size()-1) out << ",";
  }
  out << ")";
  return out;
}

template<typename T>
Vecteur<T> operator +(const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i);
  return w;
}

template<typename T>
Vecteur<T> operator -(const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = -u(i);
  return w;
}

template<typename T>
Vecteur<T> operator +(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i) + v(i);
  return w;
}

template<typename T>
Vecteur<T> operator -(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator -=");
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i) - v(i);
  return w;
}

template<typename T>
Vecteur<T> operator +(const Vecteur<T>& u, const T& x){
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i) + x;
  return w;
}

template<typename T>
Vecteur<T> operator -(const T& x, const Vecteur<T>& u){
  Vecteur<T> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = x - u(i);
  return w;
}



template<typename T, typename U>
Vecteur<U> operator *(const T& x, const Vecteur<U>& u){
  Vecteur<U> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = x * u(i);
  return w;
}

template<typename T, typename U>
Vecteur<U> operator *(const Vecteur<U>& u, const T& x){
  Vecteur<U> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i) * x;
  return w;
}

template<typename T, typename U>
Vecteur<U> operator /(const Vecteur<U>& u, const T& x){
  Vecteur<U> w(u.size());
  for (Entier i=0; i<u.size(); i++) w(i) = u(i) / x;
  return w;
}


template<typename T>
T operator |(const Vecteur<T>& u, const Vecteur<T>& v){
  test_dim(u.size(),v.size(),"Vecteur<T>::operator |");
  T s = 0;
  for (Entier i=0; i<u.size(); i++) s += u(i) * v(i);
  return s;
}

inline complex<double> operator |(const Vecteur<complexe>& u, const Vecteur<complexe>& v){
  test_dim(u.size(),v.size(),"Vecteur<complexe>::operator |");
  complex<double> s = 0;
  for (Entier i=0; i<u.size(); i++) s += u(i) * conj(v(i));
  return s;
}


template<typename T>
double norme(const Vecteur<T>& u){
  return sqrt(real(u|u));
}

template<typename T>
bool operator ==(const Vecteur<T>& u, const Vecteur<T>& v){
  if (u.size() != v.size()) return false;
  for (Entier i=0; i<u.size(); i++) if (u(i) != v(i)) return false;
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
    Entier n,m,l; //dim de la matrice
    Matrix(Entier d=0,Entier p=0, Entier l=0, T v0=T()) : mat(Vecteur(d*p*l,v0)), n(d),m(p),l(l){}
    Matrix(Entier d,Entier p, Entier l, const Vecteur<T>& vs) : mat(vs), n(d),m(p),l(l){}
    Matrix<T> (const initializer_list<Vecteur<T>>& vs) : n(vs.size()),m((vs.begin())->size())
    {
        mat.resize(n*m);
        Entier k=0;
        for(auto& li : vs)
        {
            if(li.size()!=size_t(n)) {cout<<"taille d'un vecteur incompatible"<<endl; exit(-1);}
            for(Entier i=0;i<n;i++,k++) mat[k]=li[i];
        }
    } // constructeur d'une matrice à partir de vecteurs
    T& operator ()(Entier i, Entier j, Entier k = 0); //acces au coef (i,j,k)
    const T& operator ()(Entier i, Entier j, Entier k = 0) const; //acces au coef (i,j,k)
    Vecteur<T> operator()(Entier i, Entier k = 0) const; // retourne la ligne i de la matrice k en tant que Vecteur
    Matrix<T>& operator +=(const Matrix& A);
    Matrix<T>& operator -=(const Matrix& A);
    Matrix<T>& operator +=(const T& x);
    Matrix<T>& operator -=(const T& x);
    Matrix<T>& operator *=(const T& a);
    Matrix<T>& operator /=(const T& a);
};
template<typename T>
T& Matrix<T>::operator ()(Entier i, Entier j, Entier k) //acces au coef (i,j,k)
{
    if(i<0 || j<0 || k<0 || i>=n || j>=m || k>=l) {cout<<"coef(i,j,k) : i,j,k en dehors des bornes"<<endl; exit(-1);}
     return mat[k*n*m+i*m+j];
}

template<typename T>
const T& Matrix<T>::operator ()(Entier i, Entier j, Entier k) const //acces au coef (i,j,k)
{
    if(i<0 || j<0 || k<0 || i>=n || j>=m || k>=l) {cout<<"coef(i,j,k) : i,j,k en dehors des bornes"<<endl; exit(-1);}
     return mat[k*n*m+i*m+j];
}

template<typename T>
Vecteur<T> Matrix<T>::operator ()(Entier i, Entier k) const
{
    
    Vecteur<T> V;
    V.resize(m);
    for(Entier j=0;j<m;j++)
    {
        V[j]=mat[k*m*n+i*m+j];
    }
    return V;
}
template <typename T>
Matrix<T>& Matrix<T>::operator +=(const Matrix& A)
{
    if(A.n!=n || A.m!=m || A.l!=l)
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
    if(A.n!=n || A.m!=m || A.l!=l)
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
Matrix<T> operator *(const Matrix<T>& A,const T& x)
{
    Matrix<T> C(A);
    return (C*=x);
}

template <typename T>
Matrix<T> operator *(const T& x,const Matrix<T>& A)
{
    Matrix<T> C(A);
    return (C*=x);
}


template <typename T>
Matrix<T> operator *(const Matrix<T>& A,const Matrix<T>& B)
{
    if(A.m!=B.n && A.l==1 && B.l==1){cout<<"pb dim"<<endl; return A;}
    Matrix<T> C(A);
    Matrix<T> D(B);
    Matrix<T> Res=C-C;
    for(Entier i=1;i<=C.n;i++)
    {
        for(Entier j=1;j<=C.m;j++)
        {
            for(Entier s=1;s<=C.m;s++)
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
    if(A.m!=V.size() && A.l == 1){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vecteur<T> D(V);
    Vecteur<T> Res(A.n,T());
    for(Entier i=0;i<C.n;i++)
    {
        for(Entier s=0;s<C.m;s++)
        {
            Res[i]+=C(i,s)*D[s];
        }
    }
    return Res;
}
template <typename T>
Vecteur<T> operator *(const Vecteur<T>& V,const Matrix<T>& A)
{
    if(A.n!=V.size() && A.l == 1){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vecteur<T> D(V);
    Vecteur<T> Res(A.m,T());
    for(Entier i=0;i<C.m;i++)
    {
        for(Entier s=0;s<C.n;s++)
        {
            Res[i]+=C(s,i)*D[s];
        }
    }
    return Res;
}

template <typename T>
Matrix<T>& transpose(const Matrix<T>& A)
{
    Matrix<T> R(A.m,A.n,A.l);
    for(Entier i=0;i<A.n;i++)
    {
        for(Entier j=0;j<A.m;j++)
        {
            for(Entier k=0;k<A.l;k++)
            {
                R(j,i,k)=A(i,j,k);
            }
        }
    }
    return R;
}

template<typename T>
ostream& operator <<(ostream& out,const Matrix<T>& A)
{
    Matrix<T> R(A);
    for(Entier i=0;i<R.n;i++){
      out<<(R(i));
      if (i<R.n-1){
        cout<<endl;
      }
    }
    return out;
}
#endif
