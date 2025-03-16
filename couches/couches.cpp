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

    ///clipping grad

    Reel a=0.00000001;
    rhok=max(rhok,a);//pour eviter nan
    Reel norm = 0.0;
    for(int i = 1; i <= Jacob_retropropag.n; i++)
    {
        for(int j = 1; j <= Jacob_retropropag.m; j++)
        {
            if (std::isnan(Jacob_retropropag(i, j)) || std::isinf(Jacob_retropropag(i, j))) {
                std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour majparam connexion" << std::endl;
                Jacob_retropropag(i,j)=0; //pour éviter les problemes avec nan
            }
            norm += Jacob_retropropag(i, j) * Jacob_retropropag(i, j);
        }
    }
    norm = sqrt(norm);  // Norme L2 des gradients

    Reel clip_value =0.1 / rhok;
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
  
    C-=(rhok*Jacob_retropropag); //maj

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
Reel softmax(const Matrice& A, const Matrice& B) 
{
    Reel res = 0.0;
    Reel sum_exp = 0.0;
    Matrice C(A);

    // Recherche du maximum pour éviter les débordements numériques
    Reel maximum = -std::numeric_limits<double>::infinity();
    for (int i = 1; i <= A.n; i++) {
        maximum = std::max(maximum, C(i, 1));  // Calcul du max
    }

    // B est l'étiquette de la classe réelle
    Entier b = B.mat[0];

    // Calcul de la somme des exponentielles des éléments, après normalisation par le maximum
    for (int i = 1; i <= A.n; i++) {
        sum_exp += exp(C(i, 1) - maximum);  // Soustraction du maximum pour eviter les nan
    }

    // Calcul de la fonction softmax
    res = log(sum_exp + 1e-10) - C(b + 1, 1);  // Soustraction de la sortie de la classe réelle

    // Vérification de NaN ou Inf dans le résultat
    if (std::isnan(res) || std::isinf(res)) {
        std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour softmax" << std::endl;
        res = 0.0;  // Remettre à zéro si problème
    }

    return res;
}

Matrice dsoftmax(const Matrice& A, const Matrice& B)
{
    Matrice R(A.n, 1);
    Matrice C(A);
    Entier b = B.mat[0];  // Indice de la classe réelle

    Reel maximum = -std::numeric_limits<double>::infinity();
    for (int i = 1; i <= A.n; i++) {
        maximum = max(maximum, C(i, 1));  // Calcul du max
    }
    // Calcul de la somme des exponentielles des éléments de A
    Reel sum_exp = 0.0;
    for (int i = 1; i <= A.n; i++) {
        sum_exp += exp(C(i, 1)-maximum);  // Soustraction du maximum pour eviter les nan
    }

    // Calcul de la sortie softmax
    for (int i = 1; i <= A.n; i++) {
        R(i, 1) = exp(C(i, 1)-maximum) / (sum_exp + 1e-10);  // Normalisation par la somme des exponentielles
    }

    // Si l'indice est égal à l'étiquette réelle, on soustrait 1 (pour la classe correcte)
    for (int i = 1; i <= A.n; i++) {
        if (i == b + 1) {
            R(i, 1) -= 1.0;  // Dérivée par rapport à la classe correcte
        }
    }

    return R;
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
    Reel a=0.01;
    Reel x(Xi);
    return max(x,a*x);
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
    // Limiter l'argument de exp() pour éviter un inf ou nan
    if (Xi > 20) {
        return 1.0;  // Si Xi est trop grand, la dérivée devient quasiment 1
    } else if (Xi < -20) {
        return 0.0;  // Si Xi est trop petit, la dérivée devient quasiment 0
    }
    return 1/(1+exp(-x));
}
//derivees des fonctions
Reel drelu(Reel Xi)
{
    Reel a=0;
    if(Xi<=0) //changement de relu avec max(0.01x,x) pour eviter la mort des neurones
    {
        return 0.01; //non differentiable en 0 mais on renvoie 0.01
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
    // Limiter l'argument de exp() pour éviter un inf
    if (Xi > 20) {
        return 0.0;  // Si Xi est trop grand, la dérivée devient quasiment 0
    } else if (Xi < -20) {
        return 0.0;  // Si Xi est trop petit, la dérivée devient quasiment 0
    }
    if (std::isnan((exp(-Xi)+1e-5)/((1+exp(-Xi))*(1+exp(-Xi)) +1e-5)) || std::isinf((exp(-Xi)+1e-5)/((1+exp(-Xi))*(1+exp(-Xi)) +1e-5))) {
        std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour dsigmoide" << std::endl;
        cout<<Xi<<endl;
        cout<<(exp(-Xi)+1e-5)/((1+exp(-Xi))*(1+exp(-Xi)) +1e-5)<<endl;
        return 0; //pour eviter de trainer les NAN si il y en a
    }
    return (exp(-Xi)+1e-5)/((1+exp(-Xi))*(1+exp(-Xi)) +1e-5);
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
                if (std::isnan(X(i,j,k)) || std::isinf(X(i,j,k))) {
                    std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour propag acti" << std::endl;
                    X(i,j,k)=0; //enlever les nan
                }

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
            if (std::isnan(GradX(i,i,k)) || std::isinf(GradX(i,i,k))) {
                std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour retroprop acti" << std::endl;
                GradX(i,i,k)=0;
            }

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


    Entier n_tilda=prev->X.n/p;
    Entier m_tilda=prev->X.m/q;
    dims[0]=n_tilda;dims[1]=m_tilda;dims[2]=prev->X.l;

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
            for(int i=0;i<n_tilda;i++)
            {
                for(int j=0;j<m_tilda;j++)
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
                        int xi = i * p + s;
                        int xj = j * q + t;
                        

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
                if (std::isnan(result) || std::isinf(result)) {
                    std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur propag reduction" << std::endl;
                    result=0;
                }

                X.mat[k * m_tilda * n_tilda + i * m_tilda + j] = result;

                }
            }
        }

}

void Reduction::retroPropagation() {

    Couche* next = nextC();
    if (typeR == _moyenneReduction) { // Gradient moyen
        GradX = Matrice(prevC()->X.n, prevC()->X.m, 0, prevC()->X.l);
        int n = prevC()->X.n;
        int m = prevC()->X.m;
        int l = prevC()->X.l;
        int n_tilda = n / p;
        int m_tilda = m / q;
        for (int alpha = 0; alpha < prevC()->X.n; alpha++) {
            for (int beta = 0; beta < prevC()->X.m; beta++) {
                for (int gamma = 0; gamma < prevC()->X.l; gamma++) {
                    // Balayage des blocs
                    for (int i = 0; i < n_tilda; i++) {
                        for (int j = 0; j < m_tilda; j++) {

                            
                            // Vérifier si (alpha, beta) appartient au bloc
                            if (alpha >= i && alpha < i + p && beta >= j && beta < j + q) {
                                GradX.mat[gamma * prevC()->X.n * prevC()->X.m + alpha * prevC()->X.m + beta] += 
                                    next->GradX.mat[gamma * next->GradX.n * next->GradX.m + i * next->GradX.m + j];
                            }
                        }
                    }
                    // Vérification pour éviter la division par zéro
                    if (p * q != 0) {
                        GradX.mat[gamma * prevC()->X.n * prevC()->X.m + alpha * prevC()->X.m + beta] /= (p * q);
                    }
                }
            }
        }
    } 
    else if (typeR == _maxReduction) { // Max-pooling backward
        int n = prevC()->X.n;
        int m = prevC()->X.m;
        int l = prevC()->X.l;
        int n_tilda = n / p;
        int m_tilda = m / q;

        GradX = Matrice(n, m, 0, l); 

        // Propagation du gradient
        for (int k = 0; k < l; k++) {
            for (int i = 0; i < n_tilda; i++) {
                for (int j = 0; j < m_tilda; j++) {
                    // Trouver l'élément max dans le bloc
                    int max_i = i * p;
                    int max_j = j * q;
                    double max_val = -std::numeric_limits<double>::infinity();

                    for (int s = 0; s < p; s++) {
                        for (int t = 0; t < q; t++) {
                            int xi = i * p + s;
                            int xj = j * q + t;

                            if (xi < n && xj < m) {
                                double val = prevC()->X.mat[k * n * m + xi * m + xj];
                                if (val > max_val && !(std::isnan(val) || std::isinf(val))) {
                                    
                                    max_val = val;
                                    max_i = xi;
                                    max_j = xj;
                                }
                            }
                        }
                    }

                    // Propager le gradient uniquement à l'élément max trouvé
                    GradX.mat[k * n * m + max_i * m + max_j] += 
                        next->GradX.mat[k * next->GradX.n * next->GradX.m + i * next->GradX.m + j];
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
    std::uniform_real_distribution<double> distrib(0, sqrt(2.0 / (p + q))); //pour eviter que le double soit trop grand
    K=Matrice(p,q,0);
    for(int i=1;i<=K.n;i++)
    {
        for(int j=1;j<=K.m;j++)
        {
            K(i,j)=distrib(gen); //double aleatoire
        }
    }

}

void Convolution::propagation() 
{
    // Vérification des dimensions pour éviter les erreurs
    if (mu == 0 || nu == 0) {
        std::cerr << "Erreur: mu ou nu est nul dans propagation(), division par zéro évitée." << std::endl;
        return;
    }

    Entier n_tilda =(prevC()->X.n - K.n) / mu + i0;
    Entier m_tilda =(prevC()->X.m - K.m) / nu + j0;

    X.n = n_tilda;
    X.m = m_tilda;
    X.l = prevC()->X.l;
    X.mat.resize(n_tilda * m_tilda * X.l, 0.0); // Initialisation à 0

    for (int i = 0; i < n_tilda; i++) {
        for (int j = 0; j < m_tilda; j++) {
            for (int k = 0; k < X.l; k++) {
                double result = 0.0;

                // Balayage du filtre K sur la matrice précédente
                for (int s = 0; s < K.n; s++) {
                    for (int t = 0; t < K.m; t++) {
                        int xi = i * mu + s;
                        int xj = j * nu + t;

                        // Vérification des bornes
                        if (xi < prevC()->X.n && xj < prevC()->X.m) {
                            result += prevC()->X.mat[k * prevC()->X.n * prevC()->X.m + xi * prevC()->X.m + xj] * 
                                      K.mat[s * K.m + t];
                        }
                    }
                }
                if (std::isnan(result) || std::isinf(result)) {
                    std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur propag convolution" << std::endl;
                    result=0;
                }
                X.mat[k * n_tilda * m_tilda + i * m_tilda + j] = result;
            }
        }
    }

    // Gestion du cas où on veut garder la même taille
    if (memeTaille) {
        Matrice Y(prevC()->X.n, prevC()->X.m, 0, prevC()->X.l);
        for (int i = 0; i < n_tilda; i++) {
            for (int j = 0; j < m_tilda; j++) {
                for (int k = 0; k < X.l; k++) {
                    Y.mat[k * Y.n * Y.m + (i + 1) * Y.m + (j + 1)] = X.mat[k * n_tilda * m_tilda + i * m_tilda + j];
                }
            }
        }
        X = Y;
    }
}


void Convolution::retroPropagation()
{

    Couche* next = nextC();

    if (!next) {
        std::cerr << "Erreur: nextC() est null dans retroPropagation(), propagation interrompue." << std::endl;
        return;
    }

    Entier n_tilda = (prevC()->X.n - K.n) / mu + i0;
    Entier m_tilda = (prevC()->X.m - K.m) / nu + j0;

    // Initialisation du gradient vs X
    GradX.n = prevC()->X.n;
    GradX.m = prevC()->X.m;
    GradX.l = prevC()->X.l;
    GradX.mat.resize(GradX.n * GradX.m * GradX.l, 0.0);

    for (int alpha = 0; alpha < prevC()->X.n; alpha++) {
        for (int beta = 0; beta < prevC()->X.m; beta++) {
            for (int gamma = 0; gamma < prevC()->X.l; gamma++) {
                double sum_grad = 0.0;

                for (int i = 0; i < n_tilda; i++) {
                    for (int j = 0; j < m_tilda; j++) {
                        if (alpha >= i && alpha < i + K.n && beta >= j && beta < j + K.m) {
                            if (i < K.n && j < K.m) {
                                sum_grad += next->GradX.mat[gamma * n_tilda * m_tilda + i * m_tilda + j] * 
                                            K.mat[i * K.m + j];
                            }
                        }
                    }
                }
                if (std::isnan(sum_grad) || std::isinf(sum_grad)) {
                    std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur retroprop convol vs X" << std::endl;
                    sum_grad=0;
                }
                GradX.mat[gamma * GradX.n * GradX.m + alpha * GradX.m + beta] = sum_grad;
            }
        }
    }

    // Mise à jour du gradient vs K
    GradP.n = K.n;
    GradP.m = K.m;
    GradP.mat.resize(GradP.n * GradP.m, 0.0);

    for (int u = 0; u < K.n; u++) {
        for (int v = 0; v < K.m; v++) {
            double sum_gradP = 0.0;

            for (int i = 0; i < n_tilda; i++) {
                for (int j = 0; j < m_tilda; j++) {
                    for (int k = 0; k < prevC()->X.l; k++) {
                        int xi = i * mu + u;
                        int xj = j * nu + v;

                        if (xi >= 0 && xi < prevC()->X.n && xj >= 0 && xj < prevC()->X.m) {
                            sum_gradP += next->GradX.mat[k * n_tilda * m_tilda + i * m_tilda + j] * 
                                         prevC()->X.mat[k * prevC()->X.n * prevC()->X.m + xi * prevC()->X.m + xj];
                        }
                    }
                }
            }
            if (std::isnan(sum_gradP) || std::isinf(sum_gradP)) {
                std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur retroprop convol vs K" << std::endl;
                sum_gradP=0;
            }
            GradP.mat[u * GradP.m + v] = sum_gradP;
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

    Reel a=0.00000001;
    rhok=max(rhok,a);//pour eviter nan
    Reel norm = 0.0;
    for(int i = 1; i <= Jacob_retropropag.n; i++)
    {
        for(int j = 1; j <= Jacob_retropropag.m; j++)
        {
            if (std::isnan(Jacob_retropropag(i, j)) || std::isinf(Jacob_retropropag(i, j))) {
                std::cerr << "Erreur : NaN ou Inf trouvé dans la valeur pour softmax" << std::endl;
                Jacob_retropropag(i, j)=0;
            }
            norm += Jacob_retropropag(i, j) * Jacob_retropropag(i, j);
        }
    }
    norm = sqrt(norm);  // Norme L2 des gradients

    Reel clip_value =0.1 / rhok;
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


