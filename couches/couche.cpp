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
    Couche* prev=(*this).prevC();
    (*this).X=C*(prev->X); //maj de l etat X
}

void Connexion::retroPropagation() //maj des differents gradients
{
    //maj du gradient vs X
     
    GradX=transposee(C)*(nextC()->GradX); // le jacob de la connexion par rapport a X est de C

    //maj du gradient vs parametre
    Matrice A(prevC()->X);
    GradP=(nextC()->GradX)*transposee(A);
    



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
     
    GradX=dfun_perte(prevC()->X,vref); // le jacob de la connexion par rapport a X est la transposee de C

    //pas de parametre donc pas de maj du gradient vs parametre

    //pas de gradient moyen

}

Reel moindre_carre(const Vecteur& A,const Vecteur& B)
{
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
        
    if (typeP == _moindre_carre) {
        fun_perte = &moindre_carre;
        dfun_perte = &dmoindre_caree;
    } else if (typeP == _moindre_abs) {
        fun_perte = &moindre_abs;
        dfun_perte = &d_moindre_abs;
    } else {
        std::cerr << "Erreur : typeP inconnu, fun_perte non initialisé !" << std::endl;
        fun_perte = nullptr;
        dfun_perte = nullptr;
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
