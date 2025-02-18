#include "reseau.hpp"
/*implémentaion des fonctions pour classe reseau*/

vecteur& Reseau::propagation(const vecteur& E, const vecteur& S)
{
    //cout<<"propagation"<<endl;
    auto itc=couches.begin();
    (*itc)->X=E; itc++;    
    for (;itc!=couches.end();++itc){
        //cout << "couche " << (*itc)->index << endl;
        //(*itc)->print(cout);
        (*itc)->propagation();
        //(*itc)->print(cout);
    }
    return couches[couches.size ()-2]->X; //etat avant dernière couche
}

void Reseau::retroPropagation()
{
    for(auto itc=couches.rbegin(); itc!=couches.rend();++itc )
    {
        (*itc)->print(cout);
        (*itc)->retroPropagation();
        (*itc)->print(cout);
        if((*itc)->flagP) break; // fin de la rétropropagation
    }
}

void Reseau::majParametres(TypePas tp, Reel rho, Reel alpha, Entier k)
{
    for(auto itc=couches.rbegin(); itc!=couches.rend();++itc)
    {
        //(*itc)->print(cout);
        if((*itc)->parametres)(*itc)->majParametres(tp, rho, alpha, k);
        //(*itc)->print(cout);
        if((*itc)->flagP) break;
    }
}

void Reseau::entrainement(const vector<vecteur>& Es, const vector<vecteur>& Ss, TypePas tp , Reel rho0 , Reel alpha)
{
    Perte* per = reinterpret_cast<Perte*>(couches.back()); //derniere couche (perte)
    auto its = Ss.begin();
    Entier i = 0;
    Reel rho = rho0;
    for(auto ite=Es.begin(); ite!=Es.end(); ++i, ++ite, ++its)
    {
        residus.resize(i+1);
        cout << "itération " << i << endl;
        per->initRef(*its);
        cout << "propagation" << endl;
        (*this).propagation(*ite, *its);
        (*this).print(cout);
        cout << "rétropropagation" << endl;
        (*this).retroPropagation();
        (*this).print(cout);
        cout << "majParametres" << endl;
        (*this).majParametres(tp, rho, alpha, i);
        residus[i] = per->X[0];// - (*its)[0];
        reinterpret_cast<Connexion*>(couches[1])->printC(cout);
    }
}

void Reseau::test(const vector<vecteur>&Es, const vector<vecteur>&Ss)
{
    double epsilon=0.000001; //epsilon pour tester la fin de l'algo si une precision est acceptable
    if(Es.size()!=Ss.size())
    {
        cout<<"pas autant d entrees que de sorties"<<endl;
    }
    double resultat=0;

    auto its=Ss.begin();
    Entier i=0;
    for(auto ite=Es.begin(); ite!=Es.end(); i++)
    {
        propagation(*ite,*its);
        resultat+=0.5*norme((*ite)-(*its)); //fonction quadratique pour le cout
    }
    if(resultat<=epsilon)
    {
        cout<<"erreur faible"<<endl;//il faut arreter l algo alors c est bon
                                    //mais on a un void donc il faut le gérer autrement
    }

}


void Reseau::print(ostream&out) const
{
    out<<"affichage du reseau"<<endl;
    for(auto itc=couches.begin(); itc!=couches.end(); ++itc)
    {
        (*itc)->print(out);
    }
    cout << "residus = " << residus << endl;
}