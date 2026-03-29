#include <stdio.h>
#include <stdlib.h>

#include "listeDouble.h"


//initListe ne fait pas de malloc, juste une initialisation � NULL du pointeur de liste
void initListe(T_liste *l){
    *l=NULL;
}


bool listeVide( T_liste l){
    return (l==NULL);
}

//A vous la suite
void afficheListeV1( T_liste l){
    while(l != NULL){
        printf("%d ", *l->pdata);
        l = l->suiv;
    }
    printf("\n");
}

T_liste ajoutEnTete(T_liste l, int mydata){
    T_liste nouv = (T_liste)malloc(sizeof(T_cellule));
    nouv->pdata = malloc(sizeof(int));

    if (nouv == NULL) {
        return l; // �chec allocation
    }

    *nouv -> pdata = mydata;
    nouv -> suiv = l;
    nouv -> prec = NULL;

    if (l != NULL) {
        l->prec = nouv;
    }

    return nouv;
}

T_liste ajoutEnFin(T_liste l, int mydata){
    T_liste nouv = (T_liste)malloc(sizeof(T_cellule));
    nouv->pdata = malloc(sizeof(int));

    if (nouv == NULL) {
        return l; // �chec allocation
    }

    *nouv -> pdata = mydata;
    nouv -> suiv = NULL;

    if (l == NULL) {
        nouv->prec = NULL;
        return nouv;
    }
    T_liste tmp = l;
    while(tmp->suiv != NULL){
        tmp = tmp->suiv;
    }

    tmp->suiv = nouv;
    nouv->prec = tmp;
    return l;
}

T_liste ajoutEnN(T_liste l, int pos, int mydata){
    T_liste nouv = (T_liste)malloc(sizeof(T_cellule));
    nouv->pdata = malloc(sizeof(int));

    if (pos <= 0) {
        return l;
    }

    if (nouv == NULL) {
        return l; // �chec allocation
    }
    if (pos == 1){
        return ajoutEnTete(l, mydata);
    }

    *nouv -> pdata = mydata;
    if (l == NULL) {
        nouv->prec = NULL;
        return nouv;
    }

    T_liste tpmp = l;
    for(int i = 1; i < pos; i++){
        if(tpmp == NULL) printf("liste d�pass�");
        tpmp = tpmp->suiv;
    }

    if (tpmp == NULL) {
        return l;
    }

    nouv->suiv = tpmp->suiv;
    nouv->prec = tpmp;

    if (tpmp->suiv != NULL) {
        tpmp->suiv->prec = nouv;
    }

    tpmp->suiv = nouv;

    return l;
}

T_liste suppEnTete(T_liste l){

    if (l == NULL) {
        return NULL;
    }

    T_liste tpmp = l->suiv;

    free(l->pdata);
    free(l);

    if (tpmp != NULL){
        tpmp->prec = NULL;
    }

    return tpmp;
}

T_liste suppEnFin(T_liste l){
    if (l == NULL) {
        return NULL;
    }
    if (l->suiv == NULL) {
        free(l->pdata);
        free(l);
        return NULL;
    }
    T_liste tmp = l;
    while(tmp->suiv != NULL){
        tmp = tmp->suiv;
    }

    T_liste prec = tmp->prec;
    prec->suiv = NULL;

    free(tmp->pdata);
    free(tmp);

    return l;
}

T_liste suppEnN(T_liste l, int pos){
    if (l == NULL) {
        return NULL;
    }
     if (pos <= 0) {
        return l;
    }
    if (pos == 1){
        return suppEnTete(l);
    }
    T_liste tpmp = l;
    int i = 1;
    while (tpmp != NULL && i < pos) {
        tpmp = tpmp->suiv;
        i++;
    }
    if (tpmp == NULL) {
        return l;
    }
    T_liste prec = tpmp->prec;
    T_liste suiv = tpmp->suiv;
    if (prec != NULL){
        prec->suiv = suiv;
    }
    if (suiv != NULL){
        suiv->prec = prec;
    }

    free(tpmp->pdata);
    free(tpmp);

    return l;
}

T_liste getptrFirstCell(T_liste l){
    return l;
}

T_liste getptrLastCell(T_liste l){
    if (l == NULL) return NULL;

    T_liste tmp = l;
    while (tmp->suiv != NULL){
        tmp = tmp->suiv;
    }
    return tmp;
}

T_liste getptrNextCell(T_liste l){
    T_liste lsuiv = l->suiv;
    if (lsuiv == NULL){
        return NULL;
    }
    return lsuiv;
}

T_liste getptrPrevCell(T_liste l){
    if (l->prec == NULL){
        return NULL;
    }
    return l->prec;
}

int* getPtrData(T_liste l, int mydata){
    T_liste tmp = l;

    while (tmp != NULL){
        if (*(int*)tmp->pdata == mydata)
            return (int*)tmp->pdata;

        tmp = tmp->suiv;
    }

    return NULL;
}

void swapPtrData(T_liste source, T_liste destination){
    T_liste tmp = source;
    destination = tmp;
}

int getNbreCell(T_liste l){
    if (l == NULL){
        return 0;
    }
    T_liste tmp = l;
    int i = 0;
    while (tmp != NULL) {
        tmp = tmp->suiv;
        i++;
    }
    return i;
}

int getSizeBytes(T_liste l){
    return sizeof(struct T_cell)*getNbreCell(l);
}

T_liste creatNewListeFromFusion(T_liste l1, T_liste l2){
    return l1;
}

T_liste addBehind(T_liste debut, T_liste suite){
    return debut;
}

T_liste findCell(T_liste l, int data){
    return l;
}

int getOccurences(T_liste l, int data){
    return 0;
}

void afficheListeV2( T_liste l){
    printf("zebi la moce");
}










