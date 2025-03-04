#ifndef MATRICES_HPP
#define MATRICES_HPP
#include "vecteurs.hpp"

template <typename T>
class Matrix : public Vector<T>
{
public:
    Vector<T> mat; //matrice comme vecteur
    int n,m,l; //dim de la matrice
    Matrix(int d=0,int p=0, T v0=T(),int l0=1) : mat(Vector(d*p*l0,v0)), n(d),m(p),l(l0){}
    Matrix (const Vector<T>& d) : n(d.size()),m(1),l(1),mat(Vector(d)){} // constructeur d'une matrice a partir d un vecteur
    Matrix<T> (const initializer_list<Vector<T>>& vs) : n(vs.size()),m((vs.begin())->size()),l(1)
    {
        mat.resize(n*m);
        int k=0;
        for(auto& li : vs)
        {
            if(li.size()!=size_t(n)) {cout<<"taille d'un vecteur incompatible"<<endl; exit(-1);}
            for(int i=0;i<n;i++,k++) mat[k]=li[i];
        }
    } // constructeur d'une matrice à partir de vecteurs
    T& operator ()(int i, int j,int l=1); //acces au coef (i,j,l) ou (i,j)
    Vector<T> operator()(int i) const; // retourne la ligne i en tant que Vecteur (de la premiere matrice si l!=1)
    Matrix<T>& operator +=(const Matrix& A);
    Matrix<T>& operator -=(const Matrix& A);
    Matrix<T>& operator +=(const T& x);
    Matrix<T>& operator -=(const T& x);
    Matrix<T>& operator *=(const T& a);
    Matrix<T>& operator /=(const T& a);
};

template<typename T>
T& Matrix<T>::operator ()(int i, int j,int p) //acces au coef (i,j)
{
    if(i<=0 || j<=0 || p<=0) {cout<<"coef(i,j) : i,j en dehors des bornes"<<endl; exit(-1);}
     return mat[(p-1)*(n*m)+(i-1)*m+j-1];
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
    if(A.n!=n || A.m!=m || (A.l!=l && A.l>1) )
    {
        cout<<"pb de dim pour l ajout de deux matrices"<<endl;
        return *this;
    }
    if(l==A.l)
    {
    mat+=A.mat; //cas matrices de meme taille
    }
    else if(A.l==1)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)
            {
                for(int p=1;p<=l;p++)
                {
                    mat[(p-1)*(n*m)+(i-1)*m+j-1]+=A.mat[(i-1)*m+j-1]; //ajout d une matrice à l matrices
                }
            }
        }

        
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator -=(const Matrix& A)
{
    if(A.n!=n || A.m!=m || (A.l!=l && A.l>1) )
    {
        cout<<"pb de dim pour la difference de deux matrices"<<endl;
        return *this;
    }
    if(l==A.l)
    {
    mat-=A.mat; //cas matrices de meme taille
    }
    else if(A.l==1)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)
            {
                for(int p=1;p<=l;p++)
                {
                    mat[(p-1)*(n*m)+(i-1)*m+j-1]-=A.mat[(i-1)*m+j-1]; //ajout d une matrice à l matrices
                }
            }
        }

        
    }
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
template <typename T, typename U>
Matrix<T> operator *(const U& x,const Matrix<T>& B)
{
    Matrix<T> C(B);
    return (C*=x);
}
template <typename T, typename U>
Matrix<T> operator *(const Matrix<T>& B,const U& x)
{
    Matrix<T> C(B);
    return (C*=x);
}
template <typename T>
Matrix<T> operator *(const Matrix<T>& A,const Matrix<T>& B)
{
    if(A.m!=B.n){cout<<"pb dim pour mult matrice/matricee"<<endl; return A;}
    if((A.l>1&&B.l>1) && A.l!=B.l){cout<<"pb dim pour mult matrice/matrice a cause nb matrice"<<endl; return A;}
    Matrix<T> C(A);
    Matrix<T> D(B);
    Matrix<T> Res(A.n,B.m,0,A.l);
    if(B.l==1)
    {
        for(int p=1;p<=A.l;p++)
        {
        for(int i=1;i<=Res.n;i++)
        {
            for(int j=1;j<=Res.m;j++)
            {
                for(int s=1;s<=A.m;s++)
                {
                    Res(i,j,p)+=C(i,s,p)*D(s,j); //on fait le pdt de chaque matrice par B
                }
            }
        }
        }

    }
    else{
        for(int p=1;p<=A.l;p++)
        {
        for(int i=1;i<=Res.n;i++)
        {
            for(int j=1;j<=Res.m;j++)
            {
                for(int s=1;s<=A.m;s++)
                {
                    Res(i,j,p)+=C(i,s,p)*D(s,j,p); //on fait le pdt de chaque matrice par B
                }
            }
        }
        }
        
    }
    
    return Res;
}
template <typename T>
Matrix<T> operator *(const Matrix<T>& A,const Vector<T>& V)
{
    if(A.m!=V.size()){cout<<"pb dim matrice/vecteur"<<endl; return V;}
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
    return Matrix<T>(Res);
}
template <typename T>
Matrix<T> operator *(const Vector<T>& V,const Matrix<T>& A)
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
    return Matrix<T>(Res);
}
template <typename T>
Matrix<T> transposee(const Matrix<T>& M)
{
    Matrix<T> Res(M.m, M.n,0,M.l); // Création d'une matrice transposée de dimensions inversées

    for(int p=0;p<M.l;p++)
    {

        for (int i = 0; i < M.m; i++)
        {
            for (int j = 0; j < M.n; j++)
            {
                Res.mat[p*M.m*M.n+j * M.m + i] = M.mat[p*M.m*M.n + i * M.n + j];
            }
        }
    }

    return Res; // Retour par valeur (évite les références invalides)
}
template<typename T>
ostream& operator <<(ostream& out,const Matrix<T>& A)
{
    Matrix<T> R(A);
    if(R.l==1)
    {
    for(int i=1;i<=R.n;i++)
        {out<<(R(i));cout<<endl;}
    }
    else
    {
        Matrix<T> B(R.n,R.m);
        for(int p=1;p<=R.l;p++)
        {
            for(int i=1;i<=R.n;i++)
            {
                for(int j=1;j<=R.m;j++)
                {
                    B(i,j)=R(i,j,p);
                }
            }
            out<<"matrice "<<p<<endl;
            for(int i=1;i<=R.n;i++)
            {out<<(B(i));cout<<endl;}

        }

    }
    return out;

}


#endif