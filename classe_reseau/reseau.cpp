#include "reseau.hpp"
/*implémentaion des fonctions pour classe reseau*/

 vecteur& Reseau::propagation(const vecteur& E, const vecteur& S)
 {
 auto itc=couches.begin();
 (*itc)->X=E; itc++;
 for (;itc!=couches.end();++itc)
 (*itc)->propagation();
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

 void Reseau::entrainement(const vector<vecteur>& Es, const vector<vecteur>& Ss ,
 TypePas tp , Reel rho0 , Reel alpha)
 {
    Perte* per = reinterpret_cast<Perte*>(couches.back()) ; //derniere couche (perte)
    auto its=Ss.begin();
    Entier i=0;
    Reel rho=rho0;
    for(auto ite=Es.begin(); ite!=Es.end(); i++,its++,ite++)
    {
        propagation(*ite,*its);
        retroPropagation();
        majParametres(tp,rho,alpha,i);
        residus[i]=per->X[0];
    }
 }

void Reseau::test(const vector<vecteur>&Es, const vector<vecteur>&Ss)
{
    double epsilon=0.000001; //epsilon pour tester la fin de l'algo si une precision est acceptable
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
        resultat+=0.5*norme(Es[i]-Ss[i]); //fonction quadratique pour le cout
    }
    if(resultat<=epsilon)
    {
        cout<<"erreur faible"<<endl;//il faut arreter l algo alors c est bon
                                    //mais on a un void donc il faut le gérer autrement
        return;
    }

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
    out<<"résumé: "<<"X0=";(couches.at(0))->print(out);out<<"--->XS=";(couches.at(couches.size()-1))->print(out);out<<endl;
}