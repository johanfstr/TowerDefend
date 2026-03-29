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

    //int score_emplacement;  //un clin d'oeil pour suscister une idée de tri
} Tunite;

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unité
} *TListePlayer;


typedef Tunite* ** TplateauJeu;  ////tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)




TplateauJeu AlloueTab2D(int largeur, int hauteur);
void afficheCoordonneesParcours(int **t, int nbcoord);
int **initChemin();         //retourne le chemin emprunter par la horde, sous forme d'un tableau[NBCOORDPARCOURS][2]
void freeChemin(int **tab);

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur, int** tabParcours);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

Tunite *creeTourSol(int posx, int posy);
Tunite *creeTourAir(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);



bool tourRoiDetruite(TListePlayer player);
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante) ; //retourne la liste des cibles possibles
void combat(int i, Tunite * UniteAttaquante, Tunite * UniteCible);  //qui utilise dessineAttaque (de maSDL.h)

Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);

TListePlayer creerhorde(TplateauJeu jeu, int x, int y, int nb_horde, TuniteDuJeu nouvelleUnite);
TListePlayer creerTour(TplateauJeu jeu,int nb_tour, TuniteDuJeu nouvelleUnite);
void deplacer_horde(TplateauJeu jeu, int** tabParcours, TListePlayer horde);
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);
void placer_Tour(TplateauJeu jeu , int **tabParcours,TListePlayer unite_tour);
void peut_attaquer(int i, TListePlayer *UniteAttaquante, TListePlayer Unitecible, TplateauJeu jeu);



#endif // TOWERDEFEND_H_INCLUDED
