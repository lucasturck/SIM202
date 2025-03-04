#include "couches.hpp"
#include <random>

//déclarations des fonctions membres
/////////////////////////////////////////:
//
//       Couche classique
//
/////////////////////////////////////////

Couche* Couche::nextC()
{
    if(reseau->sizereseau() == (index+1))
    {
        return this;
    }
    else
    {
        return reseau->at(index+1);
    }
}

Couche* Couche::prevC()
{
    if(index==0)
    {
        return this;
    }
    else
    {
        return reseau->at(index-1);
    }
}

Couche* Couche::clone() const
{
    return new Couche(*this);
}
////////////////////////
//
//      Couche entree
//
///////////////////////////

void Entree::print(ostream& out) const 
{
    out<<"type entree"<<endl;
    // out<<"X="<<X<<endl;
}

///////////////////////////////////////::
//
//
//        Couche connexion
//
//
/////////////////////////////////////////
void Connexion::propagation()
{

    Couche* prev=(*this).prevC();
    //le mat permet de flatten si jamais il y a besoin
    (*this).X=C*(prev->X.mat); //maj de l etat X


}

void Connexion::retroPropagation() //maj des differents gradients
{
    //maj du gradient vs X
     
    GradX=transposee(C)*(nextC()->GradX); // le jacob de la connexion par rapport a X est de C

    //maj du gradient vs parametre
   
    GradP=(nextC()->GradX)*transposee(Matrice(prevC()->X.mat));



    //pas de gradient moyen

}

void Connexion::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k)
{
    Reel rhok=0;
    
    if(tp==_dec_lineaire)
    {
        rhok=rho/(1+alpha*k);
    }
    else if(tp==_quadratique)
    {
        rhok=rho/(1+alpha*k*k);
    }
    else if(tp==_exponentielle)
    {
        rhok=rho*exp(-alpha*k);
    }
    else 
    {
        rhok=rho;//cas ou soit le pas n est pas bien indique ou si c est constant
    }
    Matrice Jacob_retropropag=GradP;
  
    Reel a=0.0000000001;
    rhok=max(rhok,a);
    C-=(rhok*Jacob_retropropag);

}

void Connexion::print(ostream&out) const 
{
    out<<"type connexion"<<endl;
    out<<"C:"<<endl;out<<C<<endl;
    out<<"X="<<X<<endl;
} // affichage


/////////////////////////////////////////////
//
//
//            Couche perte
//
//
/////////////////////////////////////////////

void Perte::propagation()
{

    Couche* prev=(*this).prevC();
    if (fun_perte) {

        (*this).X = Matrice(1,1,fun_perte(prev->X, vref));

        
    } else {
        std::cout << "Erreur: fun_perte est NULL dans propagation()." << std::endl;
    }

}

void Perte::retroPropagation() //maj des differents gradients
{
    //maj du gradient vs X
     
    GradX=dfun_perte(prevC()->X,vref); // le jacob de la connexion par rapport a X est la transposee de C
    //pas de parametre donc pas de maj du gradient vs parametre

    //pas de gradient moyen

}

Reel moindre_carre(const Matrice& A,const Matrice& B)
{
    return (0.5)*(1.0/A.n)*norme(A.mat-B.mat);
}
Reel moindre_abs(const Matrice& A,const Matrice& B)
{
    if (A.size() != B.size()) {
        std::cerr << "Erreur: tailles incompatibles dans moindre_abs !" << std::endl;
        return 0;
    }

    Reel res = 0;
    for (size_t i = 0; i < A.n; i++) {
        res += std::abs(A.mat[i] - B.mat[i]);
    }
    return (1.0 / A.n) * res;
}
Reel softmax(const Matrice& A,const Matrice& B) //B est un entier
{

    Reel res=0.0;

    Entier b=B.mat[0];
    Matrice C(A);

    for(int i=1;i<=A.n;i++)
    {
        res+=exp(C(i,1));
        
    }
    res=log(res);

    res-=C(b+1,1);


    return res;

}
Matrice dsoftmax(const Matrice& A,const Matrice& B)
{
    Matrice R(A.n,1);
    Matrice C(A);
    Entier b=B.mat[0];

    double div=0.0;
    for(int i=1;i<=A.n;i++)
    {
        double h=C(i,1);
        R(i,1)=exp(h);
        div+=exp(C(i,1));

    }

    R/=div;
    Matrice V(A.n,1,0);
    V(b+1,1)=1;
    return R-Matrice(V);
}

Matrice dmoindre_caree(const Matrice& A, const Matrice& B)
{
    return (1.0/A.n)*(A.mat-B.mat);
}
Matrice d_moindre_abs(const Matrice& A,const Matrice& B)
{
    if(A==B)
    {
        return Matrice(A.size(),1,0); //pas derivable
    }
    else return (1.0/A.n)*sgn(A.mat-B.mat);
}

void Perte::setFunPtr() // initialise les pointeurs de fonction à partir du type de perte (typeP)
{
        
    if (typeP == _moindre_carre) {
        fun_perte = &moindre_carre;
        dfun_perte = &dmoindre_caree;
    } else if (typeP == _moindre_abs) {
        fun_perte = &moindre_abs;
        dfun_perte = &d_moindre_abs;
    }
    else if(typeP==_softMax)
    {
        fun_perte=&softmax;
        dfun_perte=&dsoftmax;
    } else {
        std::cerr << "Erreur : typeP inconnu, fun_perte non initialisé !" << std::endl;
        fun_perte = nullptr;
        dfun_perte = nullptr;
    }
}

void Perte::init_vref(const Matrice& S) //change le vecteur S
{
    vref=S;
}

void Perte::print(ostream&out) const
{
    out<<"type perte"<<endl;
    out<<"X="<<X<<endl;
}


///////////////////////
//
//
//  Couche activation
//
//
//////////////////////////

//fonctions
Reel relu(Reel Xi)
{
    Reel a=0;
    Reel x(Xi);
    return max(x,a);
}
Reel hyperbolique(Reel Xi)
{
    Reel x(Xi);
    return tanh(x);
}
Reel hyperbolique_sat(Reel Xi)
{
    Reel x(Xi);
    return abs((tanh)(x));
}
Reel sigmoide(Reel Xi)
{
    Reel x(Xi);
    return 1/(1+exp(-x));
}
//derivees des fonctions
Reel drelu(Reel Xi)
{
    Reel a=0;
    if(Xi==0)
    {
        return 0; //non differentiable mais on renvoie 0
    }
    else return 1;
}
Reel dhyperbolique(Reel Xi)
{
    return 1-tanh(Xi)*tanh(Xi);
}
Reel dhyperbolique_sat(Reel Xi)
{
    if(Xi==0)
    {
        return 0; //fonction non derivable en 0
    }
    return abs(dhyperbolique(Xi));
}
Reel dsigmoide(Reel Xi)
{
     
    return exp(-Xi)/((1+exp(-Xi))*(1+exp(-Xi)));
}

void Activation::propagation() // mise a jour de l ’etat X
{   


    Couche* prev=(*this).prevC();
    dims[0]=prev->dims[0];

    if (fun_activation) {
        (*this).X = Matrice(prev->X.n,prev->X.m,0,prev->X.l);
        for(int i=1;i<=prev->X.n;i++)
        {
            for(int j=1;j<=X.m;j++)
            {
                for(int k=1;k<=X.l;k++)
                {
                X(i,j,k)=fun_activation(prev->X(i,j,k));

                }
            }

            
        }
    } else {
        std::cout << "Erreur: fun_perte est NULL dans propagation()." << std::endl;
    }

}

void Activation::retroPropagation() 
{
    if(GradX.mat.size()==X.n*X.n*X.l)
    {
        GradX*=0;
    }
    else{
        GradX.n=X.n;
    GradX.m=X.n;
    GradX.l=X.l;
    GradX.mat.resize(X.n*X.l*X.n);
    GradX*=0;

    }
    

    for(int i=1;i<=X.n;i++)
    {
        for(int k=1;k<=X.l;k++)
        {
            GradX(i,i,k)=dfun_activation((prevC()->X)(i,1,k));

        }
    }

    GradX=GradX*nextC()->GradX;
    //pas de parametres pour l instant
}

void Activation::print(ostream&out) const
{
    string typea;
    switch (typeA)
    {
        case _activation_indefini:
            typea="aucun";
            break;
        case _relu:
            typea="Relu";
            break;
        case _tanh:
            typea="tanh";
            break;
        case _tanhsat:
            typea="tanh sature";
            break;
        case _sigmoide:
            typea="sigmoide";
            break;
        default : break;
    }

    out<<"type activation : "<<typea<<endl;
    // cout<<"X="<<X<<endl;
}


/////////////////////
//
//   Couche reduction
//
//////////////////////
Reel moyRed(const Matrice& M)
{
    Reel res=0.;
    Matrice C(M);
    for(int i=1;i<=M.n;i++)
    {
        for(int j=1;j<=M.m;j++)
        {
            res+=C(i,j);
        }
    }
    res/=(C.n*C.m);
    return res;
}

Reel maxRed(const Matrice& M)
{
    Reel res=0.;
    Matrice C(M);
    for(int i=1;i<=M.n;i++)
    {
        for(int j=1;j<=M.m;j++)
        {
            res=max(res,C(i,j));
        }
    }
    return res;
}


void Reduction::propagation() // mise a jour de l ’etat X
{

    Couche* prev=prevC();


    Entier m_tilda=prev->X.n/p;
    Entier n_tilda=prev->X.m/q;
    dims[0]=m_tilda;dims[1]=n_tilda;dims[2]=prev->X.l;

    if(m_tilda<=0||n_tilda<=0)
    {
        cout<<"probleme de dimension pour la reduction"<<endl;
        return;
    }
    
        X.mat.resize(n_tilda*m_tilda*dims[2]);
        X.n=n_tilda;
        X.m=m_tilda;
        X.l=dims[2];
        for(int k=0;k<prev->X.l;k++)
        {
            for(int i=0;i<m_tilda;i++)
            {
                for(int j=0;j<n_tilda;j++)
                {
                    // Initialisation pour max ou moyenne
                double result;
                if (typeR == _maxReduction)
                    result = -std::numeric_limits<double>::infinity();
                else
                    result = 0.0;
                
                // Balayage du patch p x q
                for (int s = 0; s < p; s++) {
                    for (int t = 0; t < q; t++) {
                        int xi = i + s;
                        int xj = j + t;

                        // Récupération de la valeur du patch
                        double val = prev->X.mat[k * prev->X.n * prev->X.m + xi * prev->X.m + xj];


                        // Max-pooling
                        if (typeR == _maxReduction) {
                            if (val > result) result = val;
                        }
                        // Moyenne-pooling
                        else {
                            result += val;
                        }
                    }
                }

                // Normalisation pour moyenne
                if (typeR == _moyenneReduction) {
                    result /= (p * q);
                }

                // Stockage du résultat
                X.mat[k * m_tilda * n_tilda + i * n_tilda + j] = result;

                }
            }
        }

}

void Reduction::retroPropagation() // mise a jour du gradient
{
    Couche* next=nextC();
    if(typeR==_moyenneReduction) //gradient moyen
    {
        GradX=Matrice(prevC()->X.n,prevC()->X.m,0,prevC()->X.l);
        for(int alpha=1;alpha<=prevC()->X.n;alpha++)
        {
            for(int beta=1;beta<=prevC()->X.m;beta++)
            {
                for(int gamma=1;gamma<=prevC()->X.l;gamma++)
                {
                    // Balayage des blocs
                    for (int i = 1; i <= (prevC()->X.n - p + 1); i++) {
                        for (int j = 1; j <= (prevC()->X.m - q + 1); j++) {
                            
                            // Vérifier si (alpha, beta) appartient au bloc
                            if (alpha >= i && alpha < i + p && beta >= j && beta < j + q) {
                                GradX(alpha, beta, gamma) += next->GradX(i, j, gamma);
                            }
                        }
                    }
                    GradX(alpha,beta,gamma)/=(p*q);
                    
                    
                }

            }
        }

    }
    else if(typeR==_maxReduction)
    {

        GradX.n=prevC()->X.n;
        GradX.m=prevC()->X.m;
        GradX.l=prevC()->X.l;
        GradX.mat.resize(GradX.n*GradX.m*GradX.l);
        int n=prevC()->X.n;
        int m=prevC()->X.m;
        int l=prevC()->X.l;
        int n_tilda = prevC()->X.n/p;
        int m_tilda = prevC()->X.m/q;

        // Propagation du gradient
        for (int k = 0; k < l; k++) {
            for (int i = 0; i < n_tilda; i++) {
                for (int j = 0; j < m_tilda; j++) {
                    GradX(i+1,j+1,k+1)=0;
                    // Trouver l'élément max dans le bloc
                    int max_i = i ;
                    int max_j = j ;
                    double max_val = -numeric_limits<double>::infinity();

                    for (int s = 0; s < p; s++) {
                        for (int t = 0; t < q; t++) {
                            int xi = i  + s;
                            int xj = j  + t;                                     
                            if (xi < n && xj < m && prevC()->X.mat[k * n * m + xi * m + xj] > max_val) {
                                max_val = prevC()->X.mat[k * n * m + xi * m + xj];
                                max_i = xi;
                                max_j = xj;

                            }
                        }
                    }

                    // Propager le gradient uniquement à l'élément max
                    GradX(max_i+1,max_j+1,k+1) += next->GradX(i+1,j+1,k+1);
                }
            }
        }


    }


}



void Reduction::print(ostream&out) const // affichage
{
    string st;
    switch(typeR)
    {
        case _maxReduction:
        st="max";
        break;
        case _moyenneReduction:
        st="moyenne";
        break;
        default : break;
    }
    out<<"type reduction "<<st<<" avec p="<<p<<" et q="<<q<<endl;
    out<<"X="<<X<<endl;
}
/////////////////////////////////
//
//
//     Couche convolution
//
//
/////////////////////////////////


void Convolution::randomK(Entier p, Entier q) //initialisation aléatoire du noyauK
{
    std::random_device rd;  // Générateur basé sur le matériel
    std::mt19937 gen(rd()); // Générateur Mersenne Twister
    std::uniform_real_distribution<double> distrib(-sqrt(2.0 / (p + q)), sqrt(2.0 / (p + q))); //pour eviter que le double soit trop grand
    K=Matrice(p,q,0);
    for(int i=1;i<=K.n;i++)
    {
        for(int j=1;j<=K.m;j++)
        {
            K(i,j)=distrib(gen); //double aleatoire
        }
    }

}

void Convolution::propagation() // mise a jour de l ’état X
{

    Entier n_tilda=(prevC()->X.n-K.n)/mu+i0;
    Entier m_tilda=(prevC()->X.m-K.m)/nu+j0;
    X.n=n_tilda;
    X.m=m_tilda;
    X.l=prevC()->X.l;
    X.mat.resize(n_tilda*m_tilda*X.l);
    for(int i=0;i<n_tilda;i++)
    {
        for(int j=0;j<m_tilda;j++)
        {
            for(int k=0;k<X.l;k++)
            {
                // Initialisation pour max ou moyenne
                double result=0.0;
                
                // Balayage du patch p x q
                for (int s = 0; s < K.n; s++) {
                    for (int t = 0; t < K.m; t++) {
                        int xi = i + s;
                        int xj = j + t;


                        // Récupération de la valeur du patch
                        double val = prevC()->X(xi+1,xj+1,k+1)*K(s+1,t+1);

                        result+=val;
                    }
                }

                X(i+1,j+1,k+1)= result;

            }
        }
    }
    if(memeTaille==true)
    {
        Matrice Y(prevC()->X.n,prevC()->X.m,0,prevC()->X.l);
        for(int i=1;i<=n_tilda;i++)
        {
            for(int j=1;j<=m_tilda;j++)
            {
                for(int k=1;k<=X.l;k++)
                {
                    Y(i+1,j+1,k)=X(i,j,k);


                }
            }
        }
    X=Y;

    }


}

void Convolution::retroPropagation()
{

    Couche* next=nextC();
    Entier n_tilda=(prevC()->X.n-K.n)/mu+i0;
    Entier m_tilda=(prevC()->X.m-K.m)/nu+j0;
    //maj du grad vs X
        GradX.n=prevC()->X.n;
        GradX.m=prevC()->X.m;
        GradX.l=prevC()->X.l;
        GradX.mat.resize(GradX.n*GradX.m*GradX.l);

        for(int alpha=1;alpha<=prevC()->X.n;alpha++)
        {
            for(int beta=1;beta<=prevC()->X.m;beta++)
            {
                for(int gamma=1;gamma<=prevC()->X.l;gamma++)
                {
                    GradX(alpha, beta, gamma)=0;
                    // Balayage des blocs
                    for (int i = 1; i <= (prevC()->X.n - K.n + 1); i++) {
                        for (int j = 1; j <= (prevC()->X.m - K.m + 1); j++) {
                            
                            // Vérifier si (alpha, beta) appartient au bloc
                            if (alpha >= i && alpha < i + K.n && beta >= j && beta < j + K.m) {
                                if (i <= K.n && j <= K.m) {
                                    GradX(alpha, beta, gamma) += next->GradX(i, j, gamma) * K(i, j);
                                }
                                
                            }
                        }
                    }
                    
                    
                    
                }

            }
        }

        //maj du grad vs K
        GradP.n=K.n;
        GradP.m=K.m;
        GradP.mat.resize(GradP.n*GradP.m);
        for(int u=1;u<=K.n;u++)
        {
            for(int v=1;v<=K.m;v++)
            {
                GradP(u,v)=0;


                for(int i=1;i<=n_tilda;i++)
                {
                    for(int j=1;j<=m_tilda;j++)
                    {
                        for(int k=1;k<=prevC()->X.l;k++)
                        {
                            GradP(u,v)+=next->GradX(i,j,k)*prevC()->X((i-i0)*mu+u,(j-j0)*nu+v,k);

                        }
                    }
                }
                
            }
        }
        


}


void Convolution::majParametres(TypePas tp,Reel rho,Reel alpha,Entier k) // iter. gradient
{
    Reel rhok=0;

    
    if(tp==_dec_lineaire)
    {
        rhok=rho/(1+alpha*k);
    }
    else if(tp==_quadratique)
    {
        rhok=rho/(1+alpha*k*k);
    }
    else if(tp==_exponentielle)
    {
        rhok=rho*exp(-alpha*k);
    }
    else 
    {
        rhok=rho;//cas ou soit le pas n est pas bien indique ou si c est constant
    }
    
    Matrice Jacob_retropropag=GradP;  
    ///clipping grad

    Reel a=0.00000000001;
    rhok=max(rhok,a);//pour eviter nan
    Reel norm = 0.0;
    for(int i = 1; i <= Jacob_retropropag.n; i++)
    {
        for(int j = 1; j <= Jacob_retropropag.m; j++)
        {
            norm += Jacob_retropropag(i, j) * Jacob_retropropag(i, j);
        }
    }
    norm = sqrt(norm);  // Norme L2 des gradients

    Reel clip_value = 1.0 / rhok;
    if (norm > clip_value)
    {
        Reel scale = clip_value / norm;  // Facteur de réduction
        for(int i = 1; i <= Jacob_retropropag.n; i++)
        {
            for(int j = 1; j <= Jacob_retropropag.m; j++)
            {
                Jacob_retropropag(i, j) *= scale;  // Normalisation des gradients
            }
        }
    }
    K-=(rhok*Jacob_retropropag);
    


}




void Convolution::print(ostream&out) const // affichage de la couche
{
    out<<"type convolution"<<endl;
    out<<"matrice de convolution : "<<endl;
    out<<K<<endl;
    // out<<"X="<<X<<endl;
}


