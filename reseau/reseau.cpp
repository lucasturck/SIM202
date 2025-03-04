#include "reseau.hpp"
/*implémentaion des fonctions pour classe reseau*/

Couche* Reseau::at(int i) const //recuperation d un couche
{
    if(i>=couches.size())
    {
        cout<<"attention indice qui depasse le nombre de couches"<<endl;
        return couches[0];
    }
    return couches[i];
}

int Reseau::sizereseau() const
{
    return couches.size();
}
void Reseau::chgmtindex(int i)//changement de l index de la couche
{
    couches[i]->index=i;
}

void Reseau::init_reseau() //lier les couches au reseau
{
    for(int i=0;i<couches.size();i++)
    {
        couches[i]->reseau=this;
    }
}
Vecteur Reseau::acces_residus()
{
    return residus;
}
void Reseau::stockS(const Matrice& S)//pour stocker le vecteur attendu dans la couche perte
{
    

        Perte* per = reinterpret_cast<Perte*>(couches.back()) ; //derniere couche (perte)
        per->init_vref(S);
    
    
}


 Matrice& Reseau::propagation(const Matrice& E, const Matrice& S)
 {
   if (couches.back()->type==_perte) stockS(S);
 auto itc=couches.begin();
 (*itc)->X=E; itc++;
 for (;itc!=couches.end();++itc)
 {
 (*itc)->propagation();
 }
 return couches[couches.size ()-2]->X; //etat avant dernière couche
 }


 void Reseau::retroPropagation()
 {
    for(auto itc=couches.rbegin(); itc!=couches.rend();++itc )
    {
       (*itc)->retroPropagation();
       if((*itc)->flagP) break; // fin de la rétropropagation
    }
 }

 void Reseau::majParametres(TypePas tp , Reel rho , Reel alpha , Entier k)
 {
   for(auto itc=couches.rbegin(); itc!=couches.rend();++itc)
   {
   if((*itc)->parametres)(*itc)->majParametres(tp ,rho , alpha ,k) ;
   if((*itc)->flagP) break;
   }
 }

 void Reseau::entrainement(const vector<Matrice>& Es, const vector<Matrice>& Ss ,
 TypePas tp , Reel rho0 , Reel alpha)
 {

    residus.resize(Es.size());//on met la bonne taille du vecteur residu
    Perte* per = reinterpret_cast<Perte*>(couches.back()) ; //derniere couche (perte)
    auto its=Ss.begin();
    Entier i=0;
    Reel rho=rho0;
    for(auto ite=Es.begin(); ite!=Es.end(); i++,its++,ite++)
    {
        propagation(*ite,*its);


        retroPropagation();

    majParametres(tp,rho,alpha,i);

        // residus[i]=per->X.mat[0];


    }

 }

void Reseau::test(const vector<Matrice>&Es, const vector<Matrice>&Ss)
{
    if(Es.size()!=Ss.size())
    {
        cout<<"pas autant d entrees que de sorties"<<endl;
        return;
    }
    double resultat=0;

    auto its=Ss.begin();
    Entier i=0;
    for(auto ite=Es.begin(); ite!=Es.end(); i++,its++,ite++)
    {
        propagation(*ite,*its);
        resultat+=couches.back()->X.mat[0];
    }
    resultat/=Es.size();
    cout<<"resultat du test : "<<endl;
    cout<<"erreur moyenne : "<<resultat<<endl;
}

void Reseau::print(ostream&out) const
{
    out<<"affichage du reseau "<<name<<endl;
    Entier i=0;
    for(auto ite=couches.begin(); ite!=couches.end(); i++,ite++)
    {
        out<<"neurone "<<i<<endl;
        (couches.at(i))->print(out);
        out<<endl;

    }
}