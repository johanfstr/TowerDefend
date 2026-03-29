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
    if (listeVide(l)) printf("la liste est vide\n");

    //� vous la suite

    l = ajoutEnTete(l, 5);
    l = ajoutEnTete(l, 50);
    l = ajoutEnTete(l, 500);

    printf("ajoutEnTete : ");
    afficheListeV1(l);

    l = ajoutEnFin(l, 2);
    l = ajoutEnFin(l, 20);
    l = ajoutEnFin(l, 200);

    printf("ajoutEnFin : ");
    afficheListeV1(l);

    printf("ajoutEnN : ");
    l = ajoutEnN(l, 3, 67);
    afficheListeV1(l);

    printf("suppEnTete : ");
    l = suppEnTete(l);
    afficheListeV1(l);

    printf("suppEnFin : ");
    l = suppEnFin(l);
    afficheListeV1(l);

    printf("Liste + 2 20 200 : ");
    l = ajoutEnFin(l, 2);
    l = ajoutEnFin(l, 20);
    l = ajoutEnFin(l, 200);
    afficheListeV1(l);

    printf("suppEnN : ");
    l = suppEnN(l, 4);
    afficheListeV1(l);

    int *p1 = getptrFirstCell(l)->pdata;
    if (p1 != NULL) printf("getptrFirstCell : %d \n", *p1);
    
    int *p2 = getptrLastCell(l)->pdata;
    if (p2 != NULL) printf("getptrLastCell : %d \n", *p2);

    int *p3 = getptrNextCell(l)->pdata;
    if (p3 != NULL) printf("getptrNextCell : %d \n", *p3);

    int *p4 = getptrPrevCell(l)->pdata;
    if (p4 != NULL) printf("getptrPrevCell : %d \n", *p4);

    int *p = getPtrData(l, 5);
    if (p != NULL) printf("getPtrData : %d\n", *p);
    swapPtrData(l, l);
    printf("getNbreCell : %d \n", getNbreCell(l));
    printf("getSizeBytes : %d \n", getSizeBytes(l));

    return 0;
}
