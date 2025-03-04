#include "couche.hpp"

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
    out<<"X="<<X<<endl;
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
    //cout << "Connexion::propagation()" << endl;
    Couche* prev=(*this).prevC();
    //cout << "prev->X = " << prev->X << endl;
    //cout << "C = " << C << endl;
    (*this).X=C*(prev->X); //maj de l etat X
    //cout << "X = " << X << endl;
}

void Connexion::retroPropagation() //maj des differents gradients
{
    //maj du gradient vs X
    //cout << "Connexion::retroPropagation()" << endl;
    //cout << "nextC()->GradX = " << nextC()->GradX << endl;
    //cout << "C = " << C << endl;
    //cout << "transposee(C) = " << transposee(C) << endl;
    GradX=transposee(C)*(nextC()->GradX); // le jacob de la connexion par rapport a X est de C
    //cout << "GradX = " << GradX << endl;
    //maj du gradient vs parametre
    Matrice A(prevC()->X);
    GradP=(nextC()->GradX)*transposee(A);
    //pas de gradient moyen
}

void Connexion::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k)
{
    Reel rhok=0;
    switch (tp)
    {
    case _dec_lineaire:
        rhok=rho/(1+alpha*k);
        break;
    case _quadratique:
        rhok=rho/(1+alpha*k*k);
        break;
    case _exponentielle:
        rhok=rho*exp(-alpha*k);
        break;
    default:
        rhok=rho;
        break;
    }
    Matrice Jacob_retropropag=GradP;
    //cout << "Jacob_retropropag = " << Jacob_retropropag << endl;
    //cout << "rhok = " << rhok << endl;
    C=C-(rhok*Jacob_retropropag);


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
        (*this).X = Vecteur(1,fun_perte(prev->X, vref));
    } else {
        std::cout << "Erreur: fun_perte est NULL dans propagation()." << std::endl;
    }
}

void Perte::retroPropagation() //maj des differents gradients
{
    //maj du gradient vs X
    //cout << "Perte::retroPropagation()" << endl;
    GradX=dfun_perte(prevC()->X,vref); // le jacob de la connexion par rapport a X est la transposee de C
    //cout << "GradX = " << GradX;
    //pas de parametre donc pas de maj du gradient vs parametre
    //pas de gradient moyen
}

Reel moindre_carre(const Vecteur& A,const Vecteur& B)
{
    cout << "moindre_carre()" << endl;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    if (A.size() != B.size()) {
        std::cerr << "Erreur: tailles incompatibles dans moindre_carre !" << std::endl;
        return 0;
    }
    return (0.5)*(1.0/A.size())*norme(A-B);
}

Reel moindre_abs(const Vecteur& A,const Vecteur& B)
{
    if (A.size() != B.size()) {
        std::cerr << "Erreur: tailles incompatibles dans moindre_abs !" << std::endl;
        return 0;
    }

    Reel res = 0;
    for (size_t i = 0; i < A.size(); i++) {
        res += std::abs(A[i] - B[i]);
    }
    return (1.0 / A.size()) * res;
}

Vecteur dmoindre_caree(const Vecteur& A, const Vecteur& B)
{
    return (1.0/A.size())*(A-B);
}

Vecteur d_moindre_abs(const Vecteur& A,const Vecteur& B)
{
    if(A==B)
    {
        return Vecteur(A.size(),0); //pas derivable
    }
    else return (1.0/A.size())*sgn(A-B);
}

void Perte::setFunPtr() // initialise les pointeurs de fonction à partir du type de perte (typeP)
{
    switch (typeP) {
        case _moindre_carre:
            fun_perte = &moindre_carre;
            dfun_perte = &dmoindre_caree;
            break;
        case _moindre_abs:
            fun_perte = &moindre_abs;
            dfun_perte = &d_moindre_abs;
            break;
        default:
            std::cerr << "Erreur : typeP inconnu, fun_perte non initialisé !" << std::endl;
            fun_perte = nullptr;
            dfun_perte = nullptr;
            break;
    }
}

void Perte::init_vref(const Vecteur& S) //change le vecteur S
{
    vref=S;
}

void Perte::print(ostream&out) const
{
    out<<"type perte"<<endl;
    out<<"X="<<X<<endl;
}


////////////


Reel relu(Reel x)
{
    if(x>0) 
        return x;
    else
        return 0;
}

Reel drelu(Reel x)
{
    if(x>0)
        return 1;
    else
        return 0;
}
Reel tanhsat(Reel x)
{
    return abs(std::tanh(x));
}

Reel sigmoide(Reel x)
{
    return 1/(1+exp(-x));
}

Reel dsigmoide(Reel x)
{
    return -exp(-x)/((1+exp(-x))*(1+exp(-x)));
}

Reel dtanh(Reel x)
{
    return 1-std::tanh(x)*std::tanh(x);
}

Reel dtanhsat(Reel x)
{
    if(std::tanh(x)>0)
        return dtanh(x);
    else if(x=0)
        return 0;
    else
        return -dtanh(x);
}

Activation::Activation(TypeActivation type, FR_p userActivation, FR_p duserActivation)
{
    typeA=type;
    //Couche(_activation);
    this->parametres=false;
    switch (type)
    {
    case _relu:
        fun_activation=relu;
        dfun_activation=drelu;
        break;
    case _tanh:
        fun_activation=std::tanh;
        dfun_activation=dtanh;
        break;
    case _tanhsat:
        fun_activation=tanhsat;
        dfun_activation=dtanhsat;
        break;
    case _sigmoide:
        fun_activation=sigmoide;
        dfun_activation=dsigmoide;
        break;
    case _userActivation:
        fun_activation=userActivation;
        dfun_activation=duserActivation;
        break;
    default:
        std::cerr << "Erreur : type d'activation inconnu !" << std::endl;
        break;
    }
}

Activation* Activation::clone() const
{
    return new Activation(*this);
}

void Activation::propagation()
{
    //cout << "Activation::propagation()" << endl;
    this->dims[2]=this->prevC()->dims[2];
    this->dims[1]=this->prevC()->dims[1];
    this->dims[0]=this->prevC()->dims[0];
    //cout << "this->dims[0] = " << this->dims[0] << endl;
    //cout << "this->dims[1] = " << this->dims[1] << endl;
    //cout << "this->dims[2] = " << this->dims[2] << endl;
    this->X.resize(this->dims[1]*this->dims[2]);
    for(int i=0;i<std::size(this->prevC()->X);i++)
    {
        this->X[i]=fun_activation(this->prevC()->X[i]);
        //cout << "this->X[" << i << "] = " << this->X[i] << endl;
    }
}

void Activation::retroPropagation()
{
    //cout << "Activation::retroPropagation()" << endl;
    /*
    Vecteur ngradX(this->dims[1]*this->dims[2]);
    this->GradX.resize(this->dims[1]*this->dims[2]);
    //cout << "this->dims[1] = " << this->dims[1] << endl;
    //cout << "this->dims[2] = " << this->dims[2] << endl;
    //cout << "this->prevC()->X.size() = " << this->prevC()->X.size() << endl;
    for(int i=0;i<std::size(this->prevC()->X);i++){
        cout << "this->prevC()->X[" << i << "] = " << this->prevC()->X[i] << endl;
        ngradX[i] = dfun_activation(this->prevC()->X[i]);
        //cout << "ngradX[" << i << "] = " << ngradX[i] << endl;
        //cout << "ngardX = " << ngradX << endl;
    }
    //cout << "ngradX = " << ngradX << endl;
    this->GradX=ngradX;
    //cout << "this->GradX = " << this->GradX << endl;
    //cout << "this->nextC()->GradX = " << this->nextC()->GradX << endl;
    this->GradX=(this->GradX)*(this->nextC()->GradX);
    //cout << "this->GradX = " << this->GradX << endl;*/
    
    Vecteur ngradX(this->dims[1]*this->dims[2]);
    //cout << "this->nextC()->GradX = " << this->nextC()->GradX << endl;
    //cout << "X = " << this->X << endl;
    this->GradX.resize(this->dims[1]*this->dims[2]);
    for(int i=0;i<std::size(this->X);i++){
        //cout << "this->X[" << i << "] = " << this->X[i] << endl;
        //cout << "this->nextC()->GradX(" << i << " + 1) = " << this->nextC()->GradX(i+1,1) << endl;
        ngradX[i] = dfun_activation(this->X[i]) * this->nextC()->GradX(i+1,1);
    }
    this->GradX=ngradX;
    //cout << "this->GradX = " << this->GradX << endl;
    //this->GradX=(this->GradX)*(this->nextC()->GradX);
}

void Activation::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k)
{
    Reel rhok=0;
    switch (tp)
    {
    case _dec_lineaire:
        rhok=rho/(1+alpha*k);
        break;
    case _quadratique:
        rhok=rho/(1+alpha*k*k);
        break;
    case _exponentielle:
        rhok=rho*exp(-alpha*k);
        break;
    default:
        rhok=rho;
        break;
    }
    this->X=this->X-rhok*this->GradX;
}

void Activation::print(ostream& out) const
{
    out<<"type activation : " << endl;
    out<<"X="<<X<<endl;
}