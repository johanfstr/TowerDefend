#ifndef TOWERDEFEND_H_INCLUDED
#define TOWERDEFEND_H_INCLUDED
#include <stdbool.h>
#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1

typedef enum{tourSol, tourAir, tourRoi, archer, chevalier, dragon, gargouille} TuniteDuJeu;
typedef enum{sol, solEtAir, air } Tcible;


typedef struct {
    TuniteDuJeu nom;
    Tcible cibleAttaquable;	//indique le type des unités que l’on peut attaquer
    Tcible maposition;		//indique soit « air » soit « sol », utile pour savoir
                            //qui peut nous attaquer
    int pointsDeVie;
    float vitesseAttaque;	//en seconde, plus c’est petit plus c’est rapide
    int degats;
    int portee ;			//en mètre, distance sur laquelle on peut atteindre une
                            //cible

    float vitessedeplacement;	//en m/s
    int posX, posY;			//position sur le plateau de jeu
    int indiceParcours;     //indice de la case du chemin sur laquelle se trouve l'unité, à mettre à jour à chaque déplacement
    int peutAttaquer;		//permet de gérer le fait que chaque unité attaque une
                            //seule fois par tour ;
                            //0 = a déjà attaqué, 1 = peut attaquer ce tour-ci
                            // à remettre à 1 au début de chaque tour

    //struct Tunite *cible;   //NULL si pas de cible. mettre à jour ce pointeur permet l'aninamtion (simpliste certe) du tir
    //non utilisé au final -> utiliser directement dessineAttaque

    int score_emplacement;  
} Tunite;

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unité
} *TListePlayer;


typedef Tunite* ** TplateauJeu;  ////tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)

TplateauJeu AlloueTab2D(int largeur, int hauteur);
void afficheCoordonneesParcours(int **t, int nbcoord);
int **initChemin(int *nbcase, int *x, int *y);         //retourne le chemin emprunter par la horde, sous forme d'un tableau[NBCOORDPARCOURS][2]
void freeChemin(int **tab);

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur, int** tabParcours);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

Tunite *creeTourSol(int posx, int posy);
Tunite *creeTourAir(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);

Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);


// Nos fonctions de jeu

TListePlayer creerhorde(TplateauJeu jeu, int x, int y, int nb_horde); //fonction de départ pour créer un nombre donné d'hordes
TListePlayer creer_tour_roi(TplateauJeu jeu, int **tabParcours, int x, int y);
bool tourRoiDetruite(TListePlayer player); 

void deplacer_horde(TplateauJeu jeu, int** tabParcours, TListePlayer horde, int nbcase); //déplace la horde d'une case sur le parcours, en vérifiant que la case d'arrivée est libre

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite); //supprime une unité de la liste avec son Tunite
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);   //ajoute une unité à la liste avec son Tunite

void tri_selection(TListePlayer *UniteAttaquante, int taille);    //trie la liste des unités par leurs points de vie, du plus petit au plus grand, en échangeant leurs pointeurs
void affiche_liste (TListePlayer player); //affiche la liste
int tailleListe(TListePlayer player); //retourne le nombre d'unités dans la liste, en comptant seulement celles qui ont des points de vie > 0

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);
TListePlayer creer_rand_unite (TplateauJeu jeu,int **tabParcours, int x , int y, TListePlayer horde); // crée une unité aléatoire de la horde (archer, chevalier, dragon ou gargouille) et l'ajoute à la liste de la horde
TListePlayer creer_rand_tour (TplateauJeu jeu,int **tabParcours, TListePlayer tour, int nbcase); // crée une tour aléatoirement (tourSol, tourAir) en utilisant le meilleur emplacement
int score_emplacement(int** tabParcours, int nbcase, int x, int y); // calcule le score d'un emplacement pour une tour en fonction du nombre de cases du parcours qui sont à portée de la tour

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante) ; //retourne la liste des cibles possibles
void combat(SDL_Surface *surface, int i, Tunite * UniteAttaquante, Tunite * UniteCible);  // gère le combat entre une unité attaquante et une unité cible selon la vitesse d'attaque
void peut_attaquer(SDL_Surface *surface, int i, TListePlayer *UniteAttaquante, TListePlayer Unitecible, TplateauJeu jeu); //gère les attaques de toutes les unités des deux listes

#endif // TOWERDEFEND_H_INCLUDED
