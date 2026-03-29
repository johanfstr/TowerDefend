#ifndef LISTEDOUBLE_H_INCLUDED
#define LISTEDOUBLE_H_INCLUDED

#include <stdbool.h>

#include "types.h"  //Pour disposer du type T_personne et des fonctions qui le manupulent
#define DEBUG 1     //� mettre � 0 pour supprimer tous les messages d'erreurs (si listeVide, pas de suivant etc).

typedef struct T_cell{
    struct T_cell *suiv;
    struct T_cell *prec;
    T_personne *pdata;              //MAJ T_personne
} T_cellule;

typedef T_cellule *T_liste;

void initListe(T_liste *l);
bool listeVide( T_liste l);                                 //MAJ pour afficher un T_personne

void afficheListeV1( T_liste l);

T_liste ajoutEnTete(T_liste l, T_personne mydata);          //MAJ T_personne
void ajoutEnTetePtr2Ptr(T_liste *l, T_personne mydata);     //MAJ T_personne
/* fin de tag ligne 50
T_liste ajoutEnFin(T_liste l, T_personne mydata);           //MAJ T_personne
T_liste ajoutEnN(T_liste l, int pos, T_personne mydata);    //MAJ T_personne

T_liste suppEnTete(T_liste l);
T_liste suppEnFin(T_liste l);
T_liste suppEnN(T_liste l, int pos);

*/
T_liste getptrFirstCell(T_liste l);
T_liste getptrLastCell(T_liste l);
T_liste getptrNextCell(T_liste l);
T_liste getptrPrevCell(T_liste l);

T_personne* getPtrData(T_liste l);                          //MAJ T_personne
void swapPtrData( T_liste source, T_liste destination );

/*
int getNbreCell(T_liste l);
int getSizeBytes(T_liste l);

T_liste creatNewListeFromFusion(T_liste l1, T_liste l2);
T_liste addBehind(T_liste debut, T_liste suite);

T_liste findCell(T_liste l, T_personne data);
*/                                               //MAJ T_personne
int getOccurences(T_liste l, T_personne data, bool(*fcomp)(T_personne A, T_personne B));   //MAJ T_personne + fct de comparaison
/*
void afficheListeV2( T_liste l);

d�but de tag ligne 16 */

T_liste creerListeNElem( int taille );      //MAJ: ajout de cette fonction dans la librairie  + utilise types.h
void tri_selection_liste(T_liste l, bool(*fcomp)(T_personne A, T_personne B));        //MAJ: ajout de cette fonction dans la librairie
#endif // LISTEDOUBLE_H_INCLUDED;

