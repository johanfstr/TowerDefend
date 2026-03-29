#ifndef MYLIB_H_INCLUDED
#define MYLIB_H_INCLUDED

#include "listeDouble.h"

void tri_selection(int *tableau, int taille);
T_liste randomlist(T_liste l, int taille);
int* listtotab(T_liste l, int taille);
int* ltotabfree(T_liste l, int taille);
T_liste tabtolist(int* tab, int taille);
T_liste trilistbytab(T_liste l, int taille);
void trichrono(int taille);
void freel(T_liste l);
void tri_selection_liste(T_liste l);
void initListeP(T_liste2 *l);
void afficheListe( T_liste2 l);
T_liste2 creerListeNElem(int taille);
T_liste2 ajoutPersonne(T_liste2 l, T_personne mydata);
void tri_selection_liste2(T_liste2 l, bool (*comp)(T_personne, T_personne));

#endif // MYLIB_H_INCLUDED;
