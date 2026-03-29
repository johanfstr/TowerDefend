#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "utils.h"

bool fcompare( T_personne p1, T_personne p2, bool (*fcomp)(T_personne a, T_personne b) )
{
    return ( fcomp( p1, p2) );
}


int main()
{
    srand(time(NULL)); //initialisation des nombres aléatoires
    T_personne p1, p2, *tabPers;


    //Partie A: manips basiques
/*
    //p1=getPers(1);  //p1 est écrasé par la valeur de retour de getPers
    //get(&p2,2);     //p2 est modifié en place, on doit donc passer ici l'adresse de p2
*/
    p1 = getPersAlea(1);
    p2 = getPersAlea(2);

    printf("Bots aleatoires :\n");
    print(p1);
    print(p2);

    printf("\nDemo utilisation de fonctions de comparaison sur un champ de votre struct\n");

    // utilisation d'une fonction de comparaison sur un champ en particulier, tout simple
    printf("Le plus GRAND:\n");
    if (comp_taille_AsupB(p1,p2))
        print(p1);
    else
        print(p2);

    //la même, sur un autre champ
    printf("Le plus LOURD:\n");
    if (comp_masse_AsupB(p1,p2))
        print(p1);
    else
        print(p2);

    printf("\n\nDemo utilisation d'une fonction ayant pour parametre une fct de comparaison (sur un champ de votre struct)\n");
    //passage par adresse de la fonction de comparaison
    printf("\nLe plus ??? (regarder les parametres de fcompare):\n");
    if (fcompare(p1,p2,comp_masse_AsupB))
        print(p1);
    else
        print(p2);


    return 0;
}
