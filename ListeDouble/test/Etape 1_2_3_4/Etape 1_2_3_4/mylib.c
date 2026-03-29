#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mylib.h"
#include "listeDouble.h"

void tri_selection(int *tableau, int taille){
    int en_cours, plus_petit, j, temp;

    for (en_cours = 0; en_cours < taille - 1; en_cours++) {
        plus_petit = en_cours;

        for (j = en_cours; j < taille; j++) {
            if (tableau[j] < tableau[plus_petit]) {
                plus_petit = j;
            }
        }

        temp = tableau[en_cours];
        tableau[en_cours] = tableau[plus_petit];
        tableau[plus_petit] = temp;
    }
}

// Question 1
T_liste randomlist(T_liste l, int taille){
    for (int i = 0; i < taille; i++){
        l = ajoutEnTete(l, rand()%100);
    }
    return l;
}

// Question 2
int* listtotab(T_liste l, int taille){
    int* tab = (int*)malloc(taille * sizeof(int));
    for (int i = 0; i < taille && l != NULL; i++){
        tab[i] = *(l->pdata);
        l = l->suiv;
    }
    return tab;
}

// Question 3
int* ltotabfree(T_liste l, int taille){
    int* tab = malloc(taille * sizeof(int));

    T_liste tmp;
    int i = 0;

    while(l != NULL){
        tab[i++] = *(l->pdata);

        tmp = l;
        l = l->suiv;

        free(tmp->pdata);
        free(tmp);
    }

    return tab;
}

// Question 4
T_liste tabtolist(int* tab, int taille){
    T_liste l;
    initListe(&l);
    for (int i = 0; i < taille; i++){
        l = ajoutEnTete(l, tab[i]);
    }
    return l;
}

// Question 5
T_liste trilistbytab(T_liste l, int taille){
    int *tab = listtotab(l, taille);
    tri_selection(tab, taille);

    T_liste lnew;
    initListe(&lnew);

    lnew = tabtolist(tab, taille);
    free(tab);
    return lnew;

}

void trichrono(int taille){
    time_t now = time(NULL);

    T_liste l;
    initListe(&l);
    l = randomlist(l, taille);

    int *tab = listtotab(l, taille);
    tri_selection(tab, taille);

    T_liste lnew;
    initListe(&lnew);

    lnew = tabtolist(tab, taille);

    T_liste tmp = lnew;
    T_liste last = NULL;
    while(tmp != NULL){
        last = tmp;
        tmp = tmp->suiv;
    }
    printf(" Premier : %d", *lnew->pdata);
    printf("     Dernier : %d\n", *last->pdata);
    
    free(tab);
    time_t apres = time(NULL);
    printf("\n");
    printf("Traitement effectue en %f seconde(s) !\n", difftime(apres,now));
}

void freel(T_liste l){
    while(l != NULL){
        T_liste tmp = l;
        l = l->suiv;
        free(tmp->pdata);
        free(tmp);
    }
}

void tri_selection_liste(T_liste l){
    T_liste actuel, min, ind;
    time_t now = time(NULL);
    if (l == NULL || l->suiv == NULL){
        return;
    }

    actuel = l->suiv->prec;
    while(actuel != NULL){
        min = actuel;
        ind = actuel->suiv;
        while(ind != NULL){
            if (*(ind->pdata) < *(min->pdata)){
                    min = ind;
            }
            ind=ind->suiv;
        }

        int temp = *(actuel->pdata);
        *(actuel->pdata) = *(min->pdata);
        *(min->pdata) = temp;

        actuel=actuel->suiv;
    }

    T_liste tmp = l;
    T_liste last = NULL;
    while(tmp != NULL){
        last = tmp;
        tmp = tmp->suiv;
    }
    printf(" Premier : %d", *l->pdata);
    printf("     Dernier : %d\n", *last->pdata);
    time_t apres = time(NULL);
    printf("Traitement effectue en %f seconde(s) !\n", difftime(apres,now));
    
}







// ************************************************************************************************
// Étape 4
// ************************************************************************************************

T_liste2 ajoutPersonne(T_liste2 l, T_personne mydata){

    T_liste2 nouv = malloc(sizeof(T_cellule2));
    if(nouv == NULL)
        return l;

    nouv->pdata = malloc(sizeof(T_personne));
    if(nouv->pdata == NULL){
        free(nouv);
        return l;
    }

    *(nouv->pdata) = mydata;

    nouv->suiv = l;
    nouv->prec = NULL;

    if(l != NULL)
        l->prec = nouv;

    return nouv;
}

void initListeP(T_liste2 *l){
    *l=NULL;
}

void afficheListe( T_liste2 l){
    while(l != NULL){
        printf("%d ", l->pdata->id);
        l = l->suiv;
    }
    printf("\n");
}

T_liste2 creerListeNElem(int taille){
    T_liste2 l = NULL;
    for(int i = 0; i < taille; i++)
        l = ajoutPersonne(l, getPersAlea(i));

    return l;
}

void tri_selection_liste2(T_liste2 l, bool (*comp)(T_personne, T_personne)){
    T_liste2 actuel, min, ind;
    if(l == NULL){
        return;
    }

    actuel = l;
    while(actuel != NULL){
        min = actuel;
        ind = actuel->suiv;
        while(ind != NULL){
            if(comp(*(ind->pdata), *(min->pdata))){
                min = ind;
            }
            ind = ind->suiv;
        }

        T_personne tmp = *(actuel->pdata);
        *(actuel->pdata) = *(min->pdata);
        *(min->pdata) = tmp;

        actuel = actuel->suiv;
    }
}