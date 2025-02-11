#ifndef couche_hpp
#define couche_hpp
#include "vecteurs.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <complex>
using namespace std;
typedef size_t Entier ; // e n t i e r p o s i t i f
typedef float Reel ; // nombre réel
typedef Vecteur<Reel> vecteur ; // vecteurréel
typedef Matrix<Reel> Matrice ;
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};
enum TypePas{_constant,_dec_lineaire,_quadratique,_exonentielle};

class  Reseau;


class Couche{
    public:
    Reseau* reseau = nullptr; // réseau auquel appartient la couche
    TypeCouche type; // type de la couche
    Entier dims[3]; // dimensions de l’état
    Entier index=0; // index numerique de la couche
    vecteur X; // état de la couche (neurones)
    vecteur GradX; // gradient vs X
    vecteur GradP; // gradient vs paramètres (si paramètres)
    vecteur GradPm; // moyenne de GradP si besoin
    bool parametres; // true si la couche est paramétrée
    bool flagP=false; // premiere couche paramétrée (pour stopper la rétropropagation)
    bool initGradPm=true; // indicateur pour initialiser GradPm=GradP
    // fonctions membres
     Couche(TypeCouche t=_nondefini, Entier d0=0, Entier d1=0, Entier d2=0): type(t){
        dims[0] = d0; dims[1] = d1; dims[2] = d2;
        X = vecteur(d0*d1*d2);
        GradX = vecteur(d0*d1*d2);
        GradP = vecteur(0);
        GradPm = vecteur(0);
        parametres = false;
    }
    Couche* nextC(); // couche suivante (0 si dernière couche)
    Couche* prevC(); // couche précédente (0 si première couche)
    virtual Couche* clone() const; // clonage
    virtual void propagation() {}; // mise à jour de l’état X
    virtual void retroPropagation() {}; // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
    virtual void print(ostream& out) const {}; // affichage
};
enum TypeReduction{_maxReduction , _moyenneReduction } ;
class Reduction : public Couche {
protected :
TypeReduction typeR ;
Entier  p,q ; 
public :
 Reduction(int ligne,int colone, TypeReduction t, Entier d0=0, Entier d1=0, Entier d2=0);
 virtual Reduction* clone () const ; // clonage
 virtual void propagation ( ) ; // mise a jour de l’etat X
 virtual void retroPropagation ( ) ; // mise a jour du gradient
 virtual void print (ostream& out) const { }; // affichage
};
Reduction::Reduction(int ligne,int colone,TypeReduction t, Entier d0=0, Entier d1=0, Entier d2=0)
{
    p=ligne;
    q=colone;
    typeR=t;
    Couche(_reduction,d0, d1, d2);
}
Reduction* Reduction::clone () const 
{
    return new Reduction(*this); 
}
void Reduction::propagation ( )
{
  this->dims[2]=this->prevC()->dims[3];
  this->dims[1]=floor(this->prevC()->dims[1]/q);
  this->dims[0]=floor(this->prevC()->dims[0]/p);
  int times=this->prevC()->dims[2];
    vecteur& cop=this->prevC()->X;
    int x=this->prevC()->dims[0];
    int y=this->prevC()->dims[1];
   if(this->typeR==_maxReduction)
   {
    for(int i=0;i<times;i++)
{
     for(int i2=1;i2<=floor(y/q);i2++)
    {
        if(i2<floor(y/q))
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            int max1=0;
            for(int j=0;j<q;j++)
            {
              for(int j2=0;j2<p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]>max1)max1=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)]=max1;
        }
        if(i1=floor(x/p))
        {
           
           int max2=0;
            for(int j=0;j<q;j++)
            {
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]>max2)max2=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)]=max2;
        }

      }
    }
    else
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            int max1=0;
            for(int j=0;j<q+y-floor(y/q)*q;j++)
            {
              for(int j2=0;j2<p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]>max1)max1=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)]=max1;
        }
        if(i1=floor(x/p))
        {
           
           int max2=0;
            for(int j=0;j<q+y-floor(y/q)*q;j++)
            {
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]>max2)max2=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)]=max2;
        }

      }
    }


    }




    }
   }
   if(this->typeR==_moyenneReduction)
   {
    for(int i=0;i<times;i++)
{
     for(int i2=1;i2<=floor(y/q);i2++)
    {
        if(i2<floor(y/q))
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            int moyen1=0;
            for(int j=0;j<q;j++)
            {
              for(int j2=0;j2<p;j2++){moyen1+=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)]=moyen1/(p*q);
        }
        if(i1=floor(x/p))
        {
           
           int moyen2=0;
            for(int j=0;j<q;j++)
            {
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){moyen2+=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)]=moyen2/((p+x-floor(x/p)*p)*q);
        }

      }
    }
    else
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            int moyen1=0;
            for(int j=0;j<q+y-floor(y/q)*q;j++)
            {
              for(int j2=0;j2<p;j2++){moyen1+=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)]=moyen1/(p*(q+y-floor(y/q)*q));
        }
        if(i1=floor(x/p))
        {
           
           int moyen2=0;
            for(int j=0;j<q+y-floor(y/q)*q;j++)
            {
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){moyen2+=cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2];}// par petite colone
            
            }
            this->X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)]=moyen2/((p+x-floor(x/p)*p))*(p*(q+y-floor(y/q)*q));
        }

      }
    }


    }




    }





   }
}
void Reduction::retroPropagation()
{ this->dims[2]=this->prevC()->dims[3];
  this->dims[1]=floor(this->prevC()->dims[1]/q);
  this->dims[0]=floor(this->prevC()->dims[0]/p);
  int times=this->prevC()->dims[2];
  int x=this->prevC()->dims[0];
  int y=this->prevC()->dims[1];
  this->GradX=vecteur(x*y*times);
   if(this->typeR==_maxReduction)
   {
    
    vecteur& cop=this->prevC()->X;
    for(int i=0;i<times;i++)
{
     for(int i2=1;i2<=floor(y/q);i2++)
    {
        if(i2<floor(y/q))
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            for(int j=0;j<q;j++)
            
              for(int j2=0;j2<p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]==this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)])GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]=this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)];}// par petite colone
            
        }
        if(i1=floor(x/p))
        {
          
            for(int j=0;j<q;j++)
        
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]==this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)])GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]=this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)];}// par petite colone
            
        }

      }
    }
    else
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            for(int j=0;j<q+y-floor(y/q)*q;j++)
            
              for(int j2=0;j2<p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]==this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)])GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]=this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)];}// par petite colone
        }
        if(i1=floor(x/p))
        {
           
            for(int j=0;j<q+y-floor(y/q)*q;j++)
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){if(cop[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]==this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)])GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]=this->X[i1-1+i*floor(x/p)*floor(y/q)+(i2-1)*floor(x/p)];}// par petite colone
            
        }

      }
    }


    }




    }
   }
   if(this->typeR==_moyenneReduction)
   {
    for(int i=0;i<times;i++)
{
     for(int i2=1;i2<=floor(y/q);i2++)
    {
        if(i2<floor(y/q))
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
        
            for(int j=0;j<q;j++)
            
              for(int j2=0;j2<p;j2++){GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]+=1/(p*q)*X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)];}// par petite colone
            
            
        }
        if(i1=floor(x/p))
        {
           
          
            for(int j=0;j<q;j++)
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]+=1/((p+x-floor(x/p)*p)*q)*X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)];}// par petite colone
            
          
        }

      }
    }
    else
    {
      for(int i1=1;i1<=floor(x/p);i1++)
      {
        if(i1<floor(x/p))
        {
            for(int j=0;j<q+y-floor(y/q)*q;j++)
              for(int j2=0;j2<p;j2++){GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]+=1/(p*(q+y-floor(y/q)*q))*X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)];}// par petite colone
            
            
        }
        if(i1=floor(x/p))
        {
           
           int moyen2=0;
            for(int j=0;j<q+y-floor(y/q)*q;j++)
              for(int j2=0;j2<p+x-floor(x/p)*p;j2++){GradX[i*x*y+x*j+(i1-1)*p+(i2-1)*x+j2]+=1/((p+x-floor(x/p)*p)*(q+y-floor(y/q)*q))*X[i1-1+i*floor(x/p)*floor(y/q)+i2*floor(x/p)];}// par petite colone
            
        }

      }
    }


    }




    }





   }











}
void Reduction::print(ostream& out) const 
{ 
   out<<this->X<<endl<<this->GradX;
}

#endif

