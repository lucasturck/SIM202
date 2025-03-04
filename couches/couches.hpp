#ifndef COUCHES_HPP
#define COUCHES_HPP

#include "../constantes.hpp"
#include "../reseau/reseau.hpp"

 class Reseau;//forward declaration

 class Couche
 {
 public :
    Reseau* reseau=nullptr ; // réseau auquel appartient la couche
    TypeCouche type ;// type de la couche Vecteur
    Entier dims [3];// dimensions de l ’etat
    Entier index=0;// index numerique de la couche
    Matrice X; // état de la couche (neurones)
    Matrice GradX; // gradient vs X
    Matrice GradP; // gradient vs paramètres (si paramètres)
    Matrice GradPm; // moyenne de GradP si besoin
    bool parametres; // true si la couche est paramétrée
    bool flagP=false; // premiere couche parametrée (pour stopper la rétropropagation)
    bool initGradPm=true; // indicateur pour initialiser GradPm=GradP
    // fonctions membres

    Couche(Reseau* reseau1=nullptr,TypeCouche tc=_nondefini,
            Entier dim1=0,Entier dim2=0,Entier dim3=0, Entier index1=0,Vecteur X1=Vecteur(),
            Vecteur GradX1=Vecteur(), Vecteur GradP1=Vecteur(),Vecteur GradPm1=Vecteur())
            : reseau(reseau1),type(tc),index(index1),X(X1),GradX(GradX1),
            GradP(GradP1), GradPm(GradPm1),parametres(false) {{dims[0]=dim1;dims[1]=dim2;dims[2]=dim3;} 
                                            }                                       
                                             //Constructeur
    Couche* nextC(); // couche suivante (0 si dernière couche)
    Couche* prevC(); // couche précedente (0 si première couche)
    virtual Couche* clone() const; // clonage
    virtual void propagation(){}; // mise a jour de l ’etat X
    virtual void retroPropagation(){}; // mise a jour des gradients
    virtual void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k){}; // iter. gradient
    virtual void print(ostream&out) const {}; // affichage
    };


class Entree : public Couche
{
 public:
 Entree(Reseau* reseau1=nullptr,
            Entier dim1=0,Entier dim2=0,Entier dim3=0, Entier index1=0,Vecteur X1=Vecteur(),
            Vecteur GradX1=Vecteur(), Vecteur GradP1=Vecteur(),Vecteur GradPm1=Vecteur())
            {
                reseau=reseau1;
                type=_entree;
                index=index1;
                X=X1;
                GradX=GradX1;
                GradP=GradP1;
                GradPm=GradPm1;
                parametres=false;// C est le parametre
                dims[0]=dim1;dims[1]=dim2;dims[2]=dim3;
                //on fait l hyp que si on a les dimensions alors le rest est paramétré
            } //Constructeur
    Entree(Entier dim1=0,Entier dim2=0,Entier dim3=0){parametres=true;
        type=_entree;
        parametres=false;
        dims[0]=dim1;dims[1]=dim2;dims[2]=dim3;}
    virtual Entree* clone() const {return new Entree(*this);}
    virtual void print(ostream& out) const;
};


class Connexion : public Couche 
{
 protected :
 Matrice C; // matrice de connexion
 public:
Connexion(Matrice A=Matrice()): C(A){parametres=true;
                type=_connexion;
                parametres=true;
                dims[0]=A.n;dims[1]=1;dims[2]=1;}
 virtual Connexion* clone() const {return new Connexion(*this);}; // clonage
 virtual void propagation(); // mise a jour de l ’etat X
 virtual void retroPropagation(); // mise a jour des gradients
 virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
 virtual void print(ostream&out) const; // affichage
 };


typedef Reel (*FV2_p)(const Matrice&, const Matrice&); // pointeur de fonction Rn x Rp−>R
typedef Matrice (*FV2V_p)(const Matrice&, const Matrice&); // pointeur de fonction Rn x Rp−>Rn
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};
Reel moindre_carre(const Matrice& A,const Matrice& B); //fct de moindre carre
Reel moindre_abs(const Matrice& A,const Matrice& B); //fonction moindre abs
// Reel huber(const Vecteur& A,const Vecteur& B);
Reel softmax(const Vecteur& A,const Vecteur& B);
// Reel userparte(const Vecteur& A,const Vecteur& B);
// Reel entropie_croisee(const Vecteur& A,const Vecteur& B); //a faire plus tard
Matrice dmoindre_caree(const Matrice&, const Matrice&);
Matrice d_moindre_abs(const Matrice& A,const Matrice& B); //fonction moindre abs
Matrice dsoftmax(const Matrice& A,const Entier& B);



class Perte : public Couche
{
protected :
    TypePerte typeP; // type de perte
    FV2_p fun_perte; // pointeur sur la fonction perte Rn−>R
    FV2V_p dfun_perte; // pointeur sur la derivee de la fonction perte Rn−>Rn
    Matrice vref ; // pour stocker le vecteur attendu
    public:
    Perte(TypePerte tp) : typeP(tp){type=_perte;setFunPtr();dims[0]=1;dims[1]=1;dims[2]=1;}
    void init_vref(const Matrice& S); //change le vecteur S
    void setFunPtr(); // initialise les pointeurs de fonction à partir du type de perte (typeP)
    virtual Perte* clone() const {return new Perte(*this);};
    virtual void propagation (); // mise a jour de l ’état X
    virtual void retroPropagation(); // mise a jour des gradients
    virtual void print(ostream&out) const;
};



enum TypeActivation{_activation_indefini ,_relu, _tanh, _tanhsat, _sigmoide, _userActivation};

typedef Reel (*FR_p)(Reel); // pointeur de fonction R−>R
Reel relu(Reel Xi);
Reel hyperbolique( Reel Xi);
Reel hyperbolique_sat(Reel Xi);
Reel sigmoide(Reel Xi);
Reel drelu(Reel Xi);
Reel dhyperbolique( Reel Xi);
Reel dhyperbolique_sat(Reel Xi);
Reel dsigmoide(Reel Xi);

class Activation : public Couche {
protected :
TypeActivation typeA;
FR_p fun_activation; // pointeur sur la fonction d’activation
FR_p dfun_activation; // pointeur sur la dérivée de la fonction d’activation
public:
Activation(TypeActivation tp=_activation_indefini,bool p=false) : typeA(tp)
{
    type=_activation;
    if(tp==_relu)
    {
        fun_activation=&relu;
        dfun_activation=&drelu;

    }
    if(tp==_tanh)
    {
        fun_activation=&hyperbolique;
        dfun_activation=&dhyperbolique;

    }
    if(tp==_tanhsat)
    {
        fun_activation=&hyperbolique_sat;
        dfun_activation=&dhyperbolique_sat;

    }
    if(tp==_sigmoide)
    {
        fun_activation=&sigmoide;
        dfun_activation=&dsigmoide;

    }
    parametres=p; //dans le cas ou on ajoute des parametres un jour
    dims[1]=1;dims[2]=1;

}
virtual Activation* clone() const {return new Activation(*this);}; // clonage
virtual void propagation(); // mise a jour de l ’etat X
virtual void retroPropagation(); // mise a jour des gradients
// virtual void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. gradient
virtual void print(ostream&out) const ; // affichage
};


Reel maxRed(const Matrice& M);
Reel moyRed(const Matrice& M);
enum TypeReduction{_maxReduction, _moyenneReduction};
class Reduction : public Couche {
protected :
TypeReduction typeR;
Entier p,q; // taille de la fenetre de reduction
public:
Reduction(TypeReduction tr=_maxReduction,Entier p0=0,Entier q0=0) : typeR(tr),p(p0),q(q0)
{
    type=_reduction;
}
virtual Reduction* clone() const {return new Reduction(*this);}; // clonage
virtual void propagation(); // mise a jour de l ’etat X
virtual void retroPropagation(); // mise a jour du gradient
virtual void print(ostream&out) const; // affichage
};


class Convolution : public Couche {
    protected :
    Matrice K; // noyau de convolution
    Entier mu=1,nu=1; // decalages (stride)
    Entier i0=1,j0=1; // indices de départ
    bool memeTaille=false; // true si la taille d’origine doit être conservée (padding)
    public:
    Convolution(Entier p0,Entier q0,Entier mu0=1,Entier nu0=1,Entier i00=1,Entier j00=1,bool mmtaille=false) : mu(mu0),nu(nu0),i0(i00),j0(j00),memeTaille(mmtaille)
    {
        parametres=true;
        type=_convolution;
        randomK(p0,q0);
    }
    void randomK(Entier p, Entier q=0); //initialisation aléatoire du noyauK
    virtual Convolution* clone() const {return new Convolution(*this);}
    virtual void propagation(); // mise a jour de l ’état X
    virtual void retroPropagation(); // mise a jour des gradients
    virtual void majParametres(TypePas tp,Reel rho,Reel alpha,Entier k); // iter. gradient
    virtual void print(ostream&out) const; // affichage de la couche
    };
#endif