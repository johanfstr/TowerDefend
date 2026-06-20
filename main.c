//NOM pr nom  tudiant 1 : Becker Pierre-Antoine
//NOM pr nom  tudiant 2 : Forestier Johan

#include "SDL.h"
#include "maSDL.h"    //biblioth que avec des fonctions d'affichage utilisant la SDL
#include "towerdefend.h"
#include "sauvegarde.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>

/*--------- Main ---------------------*/
int main(int argc, char* argv[]){
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

int largeurFenetre = (LARGEURJEU + HAUTEURJEU) * (TILE_ISO_W/2) + TILE_ISO_W;
int hauteurFenetre = (LARGEURJEU + HAUTEURJEU) * (TILE_ISO_H/2) + TILE_PIXEL_SIZE + 40;

pWindow = SDL_CreateWindow(
    "Appuyez sur ECHAP pour quitter, S/C ET D/V pour gerer les sauvegardes",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    largeurFenetre,
    hauteurFenetre,
    SDL_WINDOW_SHOWN
);

    //SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0); //non utilis , pour m moire

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow); //le sprite qui couvre tout l' cran
    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data_iso/TourSol.bmp"); //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data_iso/TourAir.bmp"); //indice 1 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data_iso/TourRoi.bmp"); //indice 2
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data_iso/Archer.bmp"); //indice 3
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data_iso/Chevalier.bmp"); //indice 4
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data_iso/Dragon.bmp"); //indice 5
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data_iso/Gargouille.bmp"); //indice 6
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data_iso/Eau.bmp"); //indice 7 Ne figure pas dans l'enum TuniteDuJeu
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data_iso/Herbe.bmp"); //indice 8 idem
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data_iso/Pont.bmp"); //indice 9 idem
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data_iso/Terre.bmp"); //indice 10 idem

SDL_SetColorKey(pSpriteHerbe,      SDL_TRUE, SDL_MapRGB(pSpriteHerbe->format,      255, 0, 255));
SDL_SetColorKey(pSpriteTerre,      SDL_TRUE, SDL_MapRGB(pSpriteTerre->format,      255, 0, 255));
SDL_SetColorKey(pSpriteEau,        SDL_TRUE, SDL_MapRGB(pSpriteEau->format,        255, 0, 255));
SDL_SetColorKey(pSpritePont,       SDL_TRUE, SDL_MapRGB(pSpritePont->format,       255, 0, 255));
SDL_SetColorKey(pSpriteArcher,     SDL_TRUE, SDL_MapRGB(pSpriteArcher->format,     255, 0, 255));
SDL_SetColorKey(pSpriteChevalier,  SDL_TRUE, SDL_MapRGB(pSpriteChevalier->format,  255, 0, 255));
SDL_SetColorKey(pSpriteDragon,     SDL_TRUE, SDL_MapRGB(pSpriteDragon->format,     255, 0, 255));
SDL_SetColorKey(pSpriteGargouille, SDL_TRUE, SDL_MapRGB(pSpriteGargouille->format, 255, 0, 255));
SDL_SetColorKey(pSpriteTourSol,    SDL_TRUE, SDL_MapRGB(pSpriteTourSol->format,    255, 0, 255));
SDL_SetColorKey(pSpriteTourAir,    SDL_TRUE, SDL_MapRGB(pSpriteTourAir->format,    255, 0, 255));
SDL_SetColorKey(pSpriteTourRoi,    SDL_TRUE, SDL_MapRGB(pSpriteTourRoi->format,    255, 0, 255));

    // ASTUCE : on stocke le sprite d'une unit l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite
    // SAUF pour l'Eau, l''herbe et le pont qui apparaitront en l absence d'unit (NULL dans le plateau) et en foction de certains indices x,y d finissant le chemin central
    SDL_Surface* TabSprite[11]={pSpriteTourSol,pSpriteTourAir,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};
    int nbcase = 0;
    int x, y;
    int** tabParcours=initChemin(&nbcase, &x, &y); //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau 2 cases (indice 0 pour X, indice 1 pour Y)


	// on initialise ici pour le free en dehors du if
	int** newchemin = NULL;  //initialisation pour éviter un warning, sera réalloué dans chargerbin et charge seq

    if ( pSpriteTourSol ) //si le permier sprite a bien t charg , on suppose que les autres aussi
    {
    TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
    initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU, tabParcours);
    affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

    prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
    maj_fenetre(pWindow);


    /**********************************************************************/
    /*DEFINISSEZ/INITIALISER ICI VOS VARIABLES */

    srand(time(NULL)); // initialiser les nombres aléatoires
    bool ischarged = false; // différencie entre la partie non chargée (false) et la partie chargée avec la sauvegarde (true)
    TListePlayer newunite_horde = NULL; // intialise la nouvelle liste d'horde pour la sauvergarde chargée
    TListePlayer newunite_tour = NULL; // intialise la nouvelle liste de tour pour la sauvergarde chargée
    int newcases = 0; // initialisation du nombre de cases du chemin de la sauvegarde chargée
    int i = 0; // compteur de chaque itération de la boucle du jeu pour faire le combat tour par tour
    TListePlayer unite_horde = NULL; // intialise la liste des hordes
    TListePlayer unite_tour = creer_tour_roi(jeu,tabParcours, x, y); //Créer la tour roi et intialise la liste des tours

    /* // FIN de vos variables */
    /********************************************************************/
	
	// boucle principale du jeu
	int cont = 1;
	while ( cont != 0 ){   //VOUS DEVEZ GERER (DETECTER) LA FIN DU JEU -> tourRoiDetruite
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				cont = 0;
			}
		}
		//SDL_PumpEvents(); //do events
		efface_fenetre(pWinSurf);
		prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
		
		/***************************** début de nos fonctions ***********************************/

		if (ischarged == false){ // si la sauvergarde n'est pas encore chargée
			//APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE JEU
			int random = rand() % 101;
			if (random >= 15 && random <= 50) { // entre 15 et 50% de chance de créer une unité de la horde
				unite_horde = creer_rand_unite(jeu, tabParcours, x, y, unite_horde);
			}
			else if (random >= 5 && random <= 60 && tailleListe(unite_tour) < 11) { // entre 5 et 60% de chance de créer une tour
				unite_tour = creer_rand_tour(jeu, tabParcours, unite_tour, nbcase);
			}
			if (unite_horde != NULL) { // tant que la liste des hordes n'est pas vide
				deplacer_horde(jeu, tabParcours, unite_horde, nbcase); // les hordes avancent d'une case sur le chemin
				TListePlayer a_portee = quiEstAPortee(jeu, unite_horde->pdata); // on récup la liste des hordes à portée des tours
				if (a_portee != NULL) {// pour afficher leurs pv
					printf("///// Tour roi : %d\n", a_portee->pdata->pointsDeVie);
				}
				peut_attaquer(pWinSurf, i, &unite_horde, unite_tour, jeu); // les hordes attaquent la tourRoi et les tours attaquent les hordes à portée
				if (tourRoiDetruite(unite_tour)) { // si la tour roi est détruite, fin du jeu
					printf(" ------------------ Fin du jeu -------------------\n");
					printf("     Tour roi detruite, les hordes ont gagnés\n");
					printf(" -------------------------------------------------\n");
					jeu[x][y] = NULL;
					cont = 0; // on met cont à 0 pour sortir de la boucle du jeu
				}
			}
			else {
				printf ("Pas de horde\n"); // pour débug pour afficher que le jeu continue même sans horde
			}
			i++; // on incrémente le compteur d'un tour
		} else { // si la sauvergarde est chargée, on utilise les nouvelles variables chargées
			prepareAllSpriteDuJeu(jeu,newchemin,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, newcases);
			//APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE JEU
			int random = rand() % 101;
			if (random >= 15 && random <= 50) { // entre 15 et 50% de chance de créer une unité de la horde
				newunite_horde = creer_rand_unite(jeu, newchemin, x, y, newunite_horde);
			}
			else if (random >= 5 && random <= 60 && tailleListe(newunite_tour) < 11) { // entre 5 et 60% de chance de créer une tour
				newunite_tour = creer_rand_tour(jeu, newchemin, newunite_tour, newcases);
			}
			if (newunite_horde != NULL) { // tant que la liste des hordes n'est pas vide
				deplacer_horde(jeu, newchemin, newunite_horde, newcases); // les hordes avancent d'une case sur le chemin
				TListePlayer a_portee = quiEstAPortee(jeu, newunite_horde->pdata); // on récup la liste des hordes à portée des tours
				if (a_portee != NULL) {
					printf("///// Tour roi : %d\n", a_portee->pdata->pointsDeVie);
				}
				peut_attaquer(pWinSurf,i, &newunite_horde, newunite_tour, jeu); // les hordes attaquent la tourRoi et les tours attaquent les hordes à portée
				if (tourRoiDetruite(newunite_tour)) { // si la tour roi est détruite, fin du jeu
					printf(" ------------------ Fin du jeu -------------------\n");
					printf("     Tour roi detruite, les hordes ont gagnés\n");
					printf(" -------------------------------------------------\n");
					TListePlayer tempo = newunite_tour;
					int posroix = 0;
					int posroiy = 0;
					while (tempo != NULL){ // pour récupérer les coordonnées de la tour roi pour la supprimer du plateau
						if (tempo->pdata->nom == tourRoi){
							posroix = tempo->pdata->posX;
							posroiy = tempo->pdata->posY;
							break;
						}
						tempo = tempo->suiv;
					}
					jeu[posroix][posroiy] = NULL; // on supprime la tour roi du plateau
					cont = 0; // on met cont à 0 pour sortir de la boucle du jeu
				}
			} else {
				printf ("Pas de horde\n");
			}
			i++;
		}
		// FIN DE VOS APPELS
		/***********************************************************************/
		//affichage du jeu   chaque tour
		maj_fenetre(pWindow);
		SDL_Delay(100);  //valeur du d lai   modifier  ventuellement
		
		//LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
		const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
		if ( pKeyStates[SDL_SCANCODE_V] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			// on free les listes pour éviter les fuites mémoires avant de charger la sauvegarde
			libererListe(&unite_horde, jeu);
			libererListe(&unite_tour, jeu);
			libererListe(&newunite_horde, jeu);
			libererListe(&newunite_tour, jeu);
			// on récupère les variables de la sauvegarde chargée dans les nouvelles variables
			newchemin = chargerseq(jeu, &newunite_horde, &newunite_tour, &newcases); // on récupère le chemin de la sauvegarde chargée
			ischarged = true; // on met true pour différencier avec la partie non chargée
			i = 0; // on remet le compteur à 0
			
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(400);
		}
		if ( pKeyStates[SDL_SCANCODE_C] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur C */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			// on free les listes pour éviter les fuites mémoires avant de charger la sauvegarde
			libererListe(&unite_horde, jeu);
			libererListe(&unite_tour, jeu);
			libererListe(&newunite_horde, jeu);
			libererListe(&newunite_tour, jeu);
			// on récupère les variables de la sauvegarde chargée dans les nouvelles variables
			newchemin = chargerbin(jeu, &newunite_horde, &newunite_tour, &newcases); // on récupère le chemin de la sauvegarde chargée
			ischarged = true; // on met true pour différencier avec la partie non chargée
			i = 0; // on remet le compteur à 0
			
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(300);
		}
		if ( pKeyStates[SDL_SCANCODE_D] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			sauvegarderseq(jeu, unite_horde, unite_tour, tabParcours, nbcase); // on sauvegarde la partie en cours dans un fichier de sauvegarde séquentielle
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(300);
		}
		if ( pKeyStates[SDL_SCANCODE_S] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur S */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			sauvegarderbin(jeu, unite_horde, unite_tour, tabParcours, nbcase); // on sauvegarde la partie en cours dans un fichier de sauvegarde binaire
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(300);
		}
		if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){
			cont = 0;  //sortie de la boucle
			}
		}
        //fin boucle du jeu

        SDL_FreeSurface(pSpriteTourSol); // Lib ration de la ressource occup e par le sprite
        SDL_FreeSurface(pSpriteTourAir);
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout," chec de chargement du sprite (%s)\n",SDL_GetError());
    }
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    freeChemin(tabParcours);
	//freeChemin(newchemin); on l'a enlevé car il fait un segmentfault quand on ferme le programme
    return 0;
}