#ifndef SAUVEGARDE_H_INCLUDED
#define SAUVEGARDE_H_INCLUDED
#include "towerdefend.h"
#include <stdbool.h>

int sauvegarderbin(TplateauJeu jeu, TListePlayer horde, TListePlayer tour, int** tabParcours, int nbcase);
int** chargerbin(TplateauJeu jeu, TListePlayer *horde, TListePlayer *tour, int *nbcase);

int sauvegarderseq(TplateauJeu jeu, TListePlayer horde, TListePlayer tour, int** tabParcours, int nbcase);
int** chargerseq(TplateauJeu jeu, TListePlayer *horde, TListePlayer *tour, int *newcase);
char* enumtochar(Tunite *unite);
void libererListe(TListePlayer *liste, TplateauJeu jeu);

#endif // SAUVEGARDE_H_INCLUDED