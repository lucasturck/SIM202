#ifndef MATRICES_HPP
#define MATRICES_HPP
#include "vecteurs.hpp"

template <typename T>
class Matrix : public Vector<T>
{
public:
    Vector<T> mat; //matrice comme vecteur
    int n,m; //dim de la matrice
    Matrix(int d=0,int p=0, T v0=T()) : mat(Vector(d*p,v0)), n(d),m(p){}
    Matrix (const Vector<T>& d) : n(d.size()),m(1),mat(Vector(d))
        {
        }          // constructeur d'une matrice a partir d un vecteur
    Matrix<T> (const initializer_list<Vector<T>>& vs) : n(vs.size()),m((vs.begin())->size())
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
    Vector<T> operator()(int i) const; // retourne la ligne i en tant que Vecteur
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
Vector<T> Matrix<T>::operator ()(int i) const
{
    
    Vector<T> V;
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
Matrix<T> operator *(const T& x,const Matrix<T>& B)
{
    Matrix<T> C(B);
    return (C*=x);
}
template <typename T>
Matrix<T> operator *(const Matrix<T>& B,const T& x)
{
    Matrix<T> C(B);
    return (C*=x);
}
template <typename T>
Matrix<T> operator *(const Matrix<T>& A,const Matrix<T>& B)
{
    if(A.m!=B.n){cout<<"pb dim"<<endl; return A;}
    Matrix<T> C(A);
    Matrix<T> D(B);
    Matrix<T> Res(A.n,B.m,0);
    for(int i=1;i<=Res.n;i++)
    {
        for(int j=1;j<=Res.m;j++)
        {
            for(int s=1;s<=A.m;s++)
            {
                Res(i,j)+=C(i,s)*D(s,j);
            }
        }
    }
    cout << "Res = " << Res << endl;
    return Res;
}
template <typename T>
Vector<T> operator *(const Matrix<T>& A,const Vector<T>& V)
{
    if(A.m!=V.size()){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vector<T> D(V);
    Vector<T> Res(A.n,T());
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
Vector<T> operator *(const Vector<T>& V,const Matrix<T>& A)
{
    if(A.n!=V.size()){cout<<"pb dim"<<endl; return V;}
    Matrix<T> C(A);
    Vector<T> D(V);
    Vector<T> Res(A.m,T());
    for(int i=1;i<=C.m;i++)
    {
        for(int s=1;s<=C.n;s++)
        {
            Res[i-1]+=C(s,i)*D[s-1];
        }
    }
    return Res;
}
template <typename T>
Matrix<T> transposee(const Matrix<T>& M)
{
    Matrix<T> Res(M.m, M.n); // Création d'une matrice transposée de dimensions inversées

    for (int i = 0; i < M.m; i++)
    {
        for (int j = 0; j < M.n; j++)
        {
            Res.mat[j * M.m + i] = M.mat[i * M.n + j];
        }
    }

    return Res; // Retour par valeur (évite les références invalides)
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