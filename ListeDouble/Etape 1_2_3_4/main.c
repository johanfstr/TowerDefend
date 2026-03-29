#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mylib.h"
#include "listeDouble.h"

int main()
{
    srand(time(NULL));
    int taille = 100;
    T_liste l;

    // Question 1
    printf("Question 1 : \n");
    initListe(&l);
    if (listeVide(l)) printf("la liste est initialis�e\n");

    l = randomlist(l, taille);

    // Question 2
    printf("Question 2 : \n");
    int *tab = listtotab(l, taille);

    for(int i = 0; i < taille; i++) printf(" %d", tab[i]);
    printf("\n");

    // Question 3
    printf("Question 3 : \n");
    int *tab2 = ltotabfree(l, taille);

    for(int i = 0; i < taille; i++) printf(" %d", tab2[i]);
    printf("\n");

    // Question 4
    printf("Question 4 : \n");
    int *tab3 = (int*)malloc(sizeof(int)*taille);
    for (int i = 0; i < taille; i++){
        tab3[i] = rand() % 100 + 1;
    }

    T_liste lnew;
    initListe(&lnew);

    lnew = tabtolist(tab3, taille);
    T_liste ltmp = lnew;
    while(lnew != NULL){
        printf("%d ", *lnew->pdata);
        lnew = lnew->suiv;
    }
    printf("\n");

    // Question 5
    printf("Question 5 : \n");

    ltmp = trilistbytab(ltmp, taille);
    while(ltmp != NULL){
        printf("%d ", *ltmp->pdata);
        ltmp = ltmp->suiv;
    }
    printf("\n");
    

    // Question 6
    printf("Question 6 : \n");
    trichrono(20);

    // Étape 3
    printf("Étape 3 : \n");
    T_liste liste;
    initListe(&liste);
    liste = randomlist(liste, 10);
    tri_selection_liste(liste);

    // Étape 4
    printf("Étape 4 : \n");
    T_liste2 liste2;
    initListeP(&liste2);
    liste2 = creerListeNElem(10);
    afficheListe(liste2);
    tri_selection_liste2(liste2, comp_taille_AinfB);
    afficheListe(liste2);
    liste2 = creerListeNElem(10);
    tri_selection_liste2(liste2, comp_taille_AsupB);
    afficheListe(liste2);
    liste2 = creerListeNElem(10);
    tri_selection_liste2(liste2, comp_masse_AinfB);
    afficheListe(liste2);
    liste2 = creerListeNElem(10);
    tri_selection_liste2(liste2, comp_masse_AsupB);
    afficheListe(liste2);

    

/*
    free(tab);
    free(tab2);
    free(tab3);
    freel(l);
    freel(lnew);
    freel(ltmp);
*/
    return 0;
}
