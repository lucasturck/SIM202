typedef Reel float; // alias du type de reels (float ou double)
typedef Entier size_t; // alias du type d’entiers positifs (unsigned int ou size_t)
enum TypeCouche{_nondefini, _entree, _convolution, _reduction, _activation, _connexion, _perte};

class Couche{
    public:
    Reseau* reseau = nullptr; // réseau auquel appartient la couche
    TypeCouche type; // type de la couche
    Entier dims[3]; // dimensions de l’état
    Entier index=0; // index numerique de la couche
    Vecteur X; // état de la couche (neurones)
    Vecteur GradX; // gradient vs X
    Vecteur GradP; // gradient vs paramètres (si paramètres)
    Vecteur GradPm; // moyenne de GradP si besoin
    bool parametres; // true si la couche est paramétrée
    bool flagP=false; // premiere couche paramétrée (pour stopper la rétropropagation)
    bool initGradPm=true; // indicateur pour initialiser GradPm=GradP
    // fonctions membres
    Couche* nextC(); // couche suivante (0 si dernière couche)
    Couche* prevC(); // couche précédente (0 si première couche)
    virtual Couche* clone() const; // clonage
    virtual void propagation() {}; // mise à jour de l’état X
    virtual void retroPropagation() {}; // mise à jour des gradients
    virtual void majParametres(TypePas tp, Reel rho, Reel alpha, Entier k); // iter. gradient
    virtual void print(ostream& out) const {}; // affichage
};