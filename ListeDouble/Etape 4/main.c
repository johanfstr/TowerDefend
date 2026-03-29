#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "listeDouble.h"

int main()
{
    T_liste l;
    printf("Zone de test de vos fonctions!\n");
    printf("1 fonction = 1 test immediat!\n");

    //tests
    initListe(&l);
    l=creerListeNElem(10);  //Nous créons 10 T_personnes
    afficheListeV1(l);

    int nb= getOccurences(l, *(getPtrData(getptrFirstCell(l))), comp_taille_AegaleB);  //comp_taille_AegaleB vient de types.h
    printf("Il y a %d personne(s) qui a(ont) la meme taille que la premiere personne\n\n", nb-1);


    tri_selection_liste(l,comp_masse_AinfB); //tri par décroisssant sur le champ "masse"
    printf("tri par decroisssant sur le champ \"masse\"\n");
    afficheListeV1(l);

    tri_selection_liste(l,comp_taille_AsupB); //tri par croisssant sur le champ "taille"
    printf("tri par croisssant sur le champ \"taille\"\n");
    afficheListeV1(l);
    //à vous la suite

    return 0;
}
