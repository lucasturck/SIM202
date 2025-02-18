#ifndef COUCHES_HPP
#define COUCHES_HPP

#include "../constantes.hpp"
#include "../classe_reseau/reseau.hpp"

 class Reseau;//forward declaration

 class Couche
 {
 public :
    Reseau* reseau=nullptr ; // réseau auquel appartient la couche
    TypeCouche type ;// type de la couche Vecteur
    Entier dims [3];// dimensions de l ’etat
    Entier index=0;// index numerique de la couche
    Vecteur X; // état de la couche (neurones)
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
 Connexion(Matrice A=Matrice(),Reseau* reseau1=nullptr,TypeCouche tc=_connexion,
            Entier dim[3]=nullptr, Entier index1=0,Vecteur X1=Vecteur(),
            Vecteur GradX1=Vecteur(), Vecteur GradP1=Vecteur(),Vecteur GradPm1=Vecteur()) : C(A)
            {
                reseau=reseau1;
                type=tc;
                index=index1;
                X=X1;
                GradX=GradX1;
                GradP=GradP1;
                GradPm=GradPm1;
                if(dim==nullptr){dims[0]=0;dims[1]=0;dims[2]=0; parametres=false;} 
                else{dims[0]=dim[0];dims[1]=dim[1];dims[2]=dim[2]; parametres=true;}//on fait l hyp que si on a les dimensions alors le rest est paramétré
            } //Constructeur
Connexion(Matrice A,Entier dim1=0,Entier dim2=0,Entier dim3=0): C(A){parametres=true;
                type=_connexion;
                parametres=true;
                dims[0]=dim1;dims[1]=dim2;dims[2]=dim3;}
 virtual Connexion* clone() const {return new Connexion(*this);}; // clonage
 virtual void propagation(); // mise a jour de l ’etat X
 virtual void retroPropagation(); // mise a jour des gradients
 virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
 virtual void print(ostream&out) const; // affichage
 };


typedef Reel (*FV2_p)(const Vecteur&, const Vecteur&); // pointeur de fonction Rn x Rp−>R
typedef Vecteur (*FV2V_p)(const Vecteur&, const Vecteur&); // pointeur de fonction Rn x Rp−>Rn
enum TypePerte{_moindre_carre, _moindre_abs, _huber, _entropie_croisee , _softMax, _userPerte};
Reel moindre_carre(const Vecteur& A,const Vecteur& B); //fct de moindre carre
Reel moindre_abs(const Vecteur& A,const Vecteur& B); //fonction moindre abs
// Reel huber(const Vecteur& A,const Vecteur& B);
// Reel softmax(const Vecteur& A,const Vecteur& B);
// Reel userparte(const Vecteur& A,const Vecteur& B);
// Reel entropie_croisee(const Vecteur& A,const Vecteur& B); //a faire plus tard
Vecteur dmoindre_caree(const Vecteur&, const Vecteur&);
Vecteur d_moindre_abs(const Vecteur& A,const Vecteur& B); //fonction moindre abs

class Perte : public Couche
{
protected :
    TypePerte typeP; // type de perte
    FV2_p fun_perte; // pointeur sur la fonction perte Rn−>R
    FV2V_p dfun_perte; // pointeur sur la derivee de la fonction perte Rn−>Rn
    Vecteur vref ; // pour stocker le vecteur attendu
    public:
    Perte(TypePerte tp) : typeP(tp){type=_perte;}
    void init_vref(const Vecteur& S); //change le vecteur S
    void setFunPtr(); // initialise les pointeurs de fonction à partir du type de perte (typeP)
    virtual Perte* clone() const {return new Perte(*this);};
    virtual void propagation (); // mise a jour de l ’état X
    virtual void retroPropagation(); // mise a jour des gradients
    virtual void print(ostream&out) const;
};

#endif