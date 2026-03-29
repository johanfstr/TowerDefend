#ifndef LISTEDOUBLE_H_INCLUDED
#define LISTEDOUBLE_H_INCLUDED

#include <stdbool.h>

typedef struct T_cell{
    struct T_cell *suiv;
    struct T_cell *prec;
    int *pdata;              //attention � faire un malloc sur ce champ avant de s'en servir
} T_cellule;
typedef T_cellule *T_liste;

typedef struct {
    int id;
    char nom[20];
    int taille;   //en cm
    float masse;  //en Kg
    int annee,mois,jour;
} T_personne;

typedef struct T_cell2{
    struct T_cell2 *suiv;
    struct T_cell2 *prec;
    T_personne *pdata;              //MAJ T_personne
} T_cellule2;

typedef T_cellule2 *T_liste2;

void initListe(T_liste *l);
bool listeVide( T_liste l);


// fonctions de comparaisons
bool comp_taille_AsupB ( T_personne a, T_personne b);
bool comp_taille_AinfB ( T_personne a, T_personne b);
bool comp_taille_AegaleB ( T_personne a, T_personne b);

bool comp_masse_AsupB  ( T_personne a, T_personne b);
bool comp_masse_AinfB  ( T_personne a, T_personne b);
bool comp_masse_AegaleB  ( T_personne a, T_personne b);


//fonctions d'entr�es/sorties
void print( T_personne p);
void printTabPers (T_personne *tab, int taille);
void printTabPersRec (T_personne *tab, int taille);

T_personne getPers(int id);
void get(T_personne* bob, int id);


//fonctions de g�n�rations aleatoires
T_personne getPersAlea(int id);
T_personne* getTabPersAlea(int taille);

int getRandInt(int debutplage, int finplage);
float getRandFloat(float debutplage, float finplage);


void afficheListeV1( T_liste l);
//Pour "AfficheListeV1" Vous avez le droit de lire directement dans la structure de donn�es
//Utile pour afficher et debuguer les fonctions ci-dessous

T_liste ajoutEnTete(T_liste l, int mydata); //Bien mettre � NULL les champs suiv et prec non utilis�s s'il y en a
T_liste ajoutEnFin(T_liste l, int mydata);
T_liste ajoutEnN(T_liste l, int pos, int mydata);

T_liste suppEnTete(T_liste l);
T_liste suppEnFin(T_liste l);
T_liste suppEnN(T_liste l, int pos);

T_liste getptrFirstCell(T_liste l);
T_liste getptrLastCell(T_liste l);
T_liste getptrNextCell(T_liste l);
T_liste getptrPrevCell(T_liste l);

int* getPtrData(T_liste l, int mydata);
void swapPtrData( T_liste source, T_liste destination );

int getNbreCell(T_liste l);
int getSizeBytes(T_liste l); //utilisation de sizeof

T_liste creatNewListeFromFusion(T_liste l1, T_liste l2); //on souhaite CREER une nouvelle liste sans modifier l1 et l2
T_liste addBehind(T_liste debut, T_liste suite);

T_liste findCell(T_liste l, int data);
int getOccurences(T_liste l, int data);  //nbre de fois que data est pr�sent dans toute la liste l1

void afficheListeV2( T_liste l);
//Pour "AfficheListeV2", vous devez utiliser uniquement les fonctions �crites ci-dessus (interdiction de passer directement par les champs ptr de la SD)


#endif // LISTEDOUBLE_H_INCLUDED;

