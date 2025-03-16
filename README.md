# SIM202-Reseau de neurone 
## Description
Le but de ce projet est de créer un réseau de neurone convolutionnel. Ce projet se sépare en trois parties :
    -  Création d'un réseau qui doit être capable de faire une régression linéaire de type z=ax+b.
    -  Création d'un réseau capable d'estimer une fonction de type z=cos(pi*x)*sin(pi*y). 
    -  Partie plus complexe : Création d'un réseau capable de réaliser une classification d'images à l'aide de Cifar10.

## Utilisation
Le main.cpp contient les trois parties. Les parties 1 et 2 ne demandent aucun changement. La partie 3 sur la classification d'image peut être très longue à lancer car la base de donnée est grande, donc il est possible soit de commenter cette partie, soit de changer le nombre d'images à analyser, la taille des batchs ou le nombre d'époques (changer dans le main.cpp : nbimage, nbentr, nbep resp. lignes 246,247,248).

 Pour lancer les résultats, un makefile est disponible pour les utilisateurs de linux. Pour ces utilisateurs, un make suivi de ./main lancera les résultats.
 Pour les utilisateurs dont le makefile ne marche pas, entrer cette ligne de code :
    - g++ -Wall -Wextra main.cpp reseau/reseau.cpp couches/couches.cpp cifar10/image_process.cpp
    - lancer avec ./main
