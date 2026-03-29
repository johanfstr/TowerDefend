#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    tmp->prec->suiv = NULL;
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

T_liste addBehind(T_liste debut, T_liste suite){
    if(debut == NULL) return suite;
    if(suite == NULL) return debut;

    T_liste last = debut;
    while(last->suiv != NULL){
        last = last->suiv;
    }

    last->suiv = suite;
    suite->prec = last;

    return debut;
}


T_liste creatNewListeFromFusion(T_liste l1, T_liste l2){
    T_liste newList = NULL;

    T_liste tmp = l1;
    while(tmp != NULL){
        newList = ajoutEnFin(newList, *(tmp->pdata));
        tmp = tmp->suiv;
    }

    tmp = l2;
    while(tmp != NULL){
        newList = ajoutEnFin(newList, *(tmp->pdata));
        tmp = tmp->suiv;
    }

    return newList;
}


T_liste findCell(T_liste l, int data){
    T_liste tmp = l;

    while(tmp != NULL){
        if(*(tmp->pdata) == data){
            return tmp;
        }
        tmp = tmp->suiv;
    }

    return NULL;
}


int getOccurences(T_liste l, int data){
    int count = 0;
    T_liste tmp = l;

    while(tmp != NULL){
        if(*(tmp->pdata) == data){
            count++;
        }
        tmp = tmp->suiv;
    }

    return count;
}


void afficheListeV2(T_liste l){
    T_liste tmp = getptrFirstCell(l);

    while(tmp != NULL){
        int *data = getPtrData(tmp, 0); 
        printf("%d ", *data);
        tmp = getptrNextCell(tmp);
    }

    printf("\n");
}












// Fonctions puantes données :

// Fonctions de comparaisons
//-------------------------------------------------------------------
bool comp_taille_AsupB ( T_personne a, T_personne b){
    return a.taille > b.taille;
};
//-------------------------------------------------------------------
bool comp_taille_AinfB ( T_personne a, T_personne b){
    return a.taille < b.taille;
};
//-------------------------------------------------------------------
bool comp_taille_AegaleB ( T_personne a, T_personne b){
    return a.taille == b.taille;
};
//-------------------------------------------------------------------
bool comp_masse_AsupB  ( T_personne a, T_personne b){
    return a.masse > b.masse;
};
//-------------------------------------------------------------------
bool comp_masse_AinfB ( T_personne a, T_personne b){
    return a.masse < b.masse;
};
//-------------------------------------------------------------------
bool comp_masse_AegaleB ( T_personne a, T_personne b){
    return a.masse == b.masse;
};
//-------------------------------------------------------------------






//Fonctions d'entr�es/sorties
//-------------------------------------------------------------------
void print (T_personne p){
    printf("id:%d nom:\"%s\", taille: %dcm, masse: %2.1fkg, ne(e) le %2d/%2d/%4d \n",
           p.id, p.nom, p.taille, p.masse, p.jour, p.mois, p.annee );
};
//-------------------------------------------------------------------
void printTabPers (T_personne *tab, int taille){
    for (int i=0; i<taille; i++)
    {
        print(tab[i]);
    }
    return ;
};
//-------------------------------------------------------------------
void printTabPersRec (T_personne *tab, int taille){
    if (taille >0)
    {
        print(tab[0]);
        printTabPers (tab+1, taille-1);
    }
    return ;
};
//-------------------------------------------------------------------
T_personne getPers(int id)
{
    T_personne bob;

    fflush(stdin);  //vide la m�moire tampon du flux issu du clavier
    printf("\nDonnez un nom:\n");
    scanf("%s", bob.nom);

    printf("\nDonnez taille (cm), masse (kg) et date de naissance (jj/mm/aaaa):\n");
    scanf("%d %f %d/%d/%d",&(bob.taille), &(bob.masse), &bob.jour, &bob.mois, &bob.annee );

    bob.id=id;
    return bob;
};
//-------------------------------------------------------------------
void get(T_personne* bob, int id)  // nous disposons ici de l'adresse de la variable dans laquelle on doit �crire
{
    fflush(stdin);  //vide la m�moire tampon du flux issu du clavier
    printf("\nDonnez Nom Taille Masse Date de naissance (jj/mm/aaaa):\n");
    scanf("%s %d %f %d/%d/%d"
         ,bob->nom, &bob->taille, &bob->masse, &bob->jour, &bob->mois, &bob->annee );
    bob->id=id;
    return;
};
//-------------------------------------------------------------------


//Fonctions de g�n�ration auto
//-------------------------------------------------------------------
T_personne getPersAlea(int id)
{
    T_personne p;
    p.id = id;
    strcpy(p.nom, "i am a bot");
    p.taille = getRandInt(150,199);
    p.masse = getRandFloat(50.0,130.0);

    p.mois = getRandInt(1,12);
    p.annee = getRandInt(1900,2021);

    int bisextile = ((p.annee%4 ==0  && p.annee !=100) || (p.annee%400==0)) ? 1 : 0;
    p.jour = (p.mois==2) ? getRandInt(1,28 + bisextile )
            : (p.mois==4 || p.mois==6 || p.mois==4 || p.mois==9 || p.mois==11) ? 30:31;

    return p;
};
//-------------------------------------------------------------------
T_personne* getTabPersAlea(int taille)
{
    T_personne *tab = (T_personne*)malloc(taille*sizeof(T_personne));

    for (int i=0; i<taille; i++){
        tab[i]=getPersAlea(i);
    }
    return tab;
};
//-------------------------------------------------------------------



int getRandInt(int debutplage, int finplage){
    return (rand()%(finplage-debutplage+1) + debutplage);
};

float getRandFloat(float debutplage, float finplage){
    return debutplage + (float) (rand()) /( (float) (RAND_MAX/(finplage-debutplage)));
};








