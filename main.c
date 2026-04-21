//NOM pr nom  tudiant 1 : Becker Pierre-Antoine
//NOM pr nom  tudiant 2 : Forestier Johan

#include "SDL.h"
#include "maSDL.h"    //biblioth que avec des fonctions d'affichage utilisant la SDL
#include "towerdefend.h"
#include "sauvegarde.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*--------- Main ---------------------*/
int main(int argc, char* argv[]){
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
    "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    LARGEURJEU*40,
    HAUTEURJEU*40,
    SDL_WINDOW_SHOWN
    );

    //SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0); //non utilis , pour m moire

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow); //le sprite qui couvre tout l' cran
    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data/TourSol.bmp"); //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data/TourAir.bmp"); //indice 1 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp"); //indice 2
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp"); //indice 3
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp"); //indice 4
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp"); //indice 5
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp"); //indice 6
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp"); //indice 7 Ne figure pas dans l'enum TuniteDuJeu
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp"); //indice 8 idem
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont.bmp"); //indice 9 idem
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp"); //indice 10 idem

    // ASTUCE : on stocke le sprite d'une unit l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite
    // SAUF pour l'Eau, l''herbe et le pont qui apparaitront en l absence d'unit (NULL dans le plateau) et en foction de certains indices x,y d finissant le chemin central
    SDL_Surface* TabSprite[11]={pSpriteTourSol,pSpriteTourAir,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};
    int nbcase = 0;
    int x, y;
    int** tabParcours=initChemin(&nbcase, &x, &y); //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau 2 cases (indice 0 pour X, indice 1 pour Y)

    if ( pSpriteTourSol ) //si le permier sprite a bien t charg , on suppose que les autres aussi
    {
    TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
    initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU, tabParcours);
    affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

    prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
    maj_fenetre(pWindow);


    /**********************************************************************/
    /*DEFINISSEZ/INITIALISER ICI VOS VARIABLES */

    srand(time(NULL));
    bool ischarged = false;
    TListePlayer newunite_horde = NULL;
    TListePlayer newunite_tour = NULL;
    int** newchemin = NULL;  //initialisation pour éviter un warning, sera réalloué dans chargerbin et charge seq
    int newcases = 0;
    int i = 0;
    TListePlayer unite_horde = NULL;
    TListePlayer unite_tour = creer_tour_roi(jeu,tabParcours, x, y); //Créer la tour roi

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

		if (ischarged == false){
			//APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE JEU
			int random = rand() % 101;
			if (random >= 15 && random <= 50) {
				unite_horde = creer_rand_unite(jeu, tabParcours, x, y, unite_horde);
			}
			else if (random >= 5 && random <= 60 && tailleListe(unite_tour) < 15) {
				unite_tour = creer_rand_tour(jeu, tabParcours, unite_tour, nbcase);
			}
			if (unite_horde != NULL) {
				deplacer_horde(jeu, tabParcours, unite_horde, nbcase);
				TListePlayer a_portee = quiEstAPortee(jeu, unite_horde->pdata);
				if (a_portee != NULL) {
					printf("Tour roi : %d     PV %d : %d\n", a_portee->pdata->pointsDeVie, unite_horde->pdata->nom, unite_horde->pdata->pointsDeVie);
				}
				peut_attaquer(pWinSurf, i, &unite_horde, unite_tour, jeu);
				if (tourRoiDetruite(unite_tour)) {
					printf(" ------------------ Fin du jeu -------------------\n");
					printf("     Tour roi detruite, les hordes ont gagnés\n");
					printf(" -------------------------------------------------\n");
					jeu[x][y] = NULL;
					cont = 0;
				}
			}
			else {
				printf ("Pas de horde\n");
			}
			i++;
		} else {
			prepareAllSpriteDuJeu(jeu,newchemin,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, newcases);
			//APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE JEU
			int random = rand() % 101;
			if (random >= 15 && random <= 50) {
				newunite_horde = creer_rand_unite(jeu, newchemin, x, y, newunite_horde);
			}
			else if (random >= 5 && random <= 60 && tailleListe(newunite_tour) < 15) {
				newunite_tour = creer_rand_tour(jeu, newchemin, newunite_tour, newcases);
			}
			if (newunite_horde != NULL) {
				deplacer_horde(jeu, newchemin, newunite_horde, newcases);
				TListePlayer a_portee = quiEstAPortee(jeu, newunite_horde->pdata);
				if (a_portee != NULL) {
					printf("Tour roi : %d     PV %d : %d\n", a_portee->pdata->pointsDeVie, newunite_horde->pdata->nom, newunite_horde->pdata->pointsDeVie);
				}
				peut_attaquer(pWinSurf,i, &newunite_horde, newunite_tour, jeu);
				if (tourRoiDetruite(newunite_tour)) {
					printf(" ------------------ Fin du jeu -------------------\n");
					printf("     Tour roi detruite, les hordes ont gagnés\n");
					printf(" -------------------------------------------------\n");
					TListePlayer tempo = newunite_tour;
					int posroix = 0;
					int posroiy = 0;
					while (tempo != NULL){
						if (tempo->pdata->nom == tourRoi){
							posroix = tempo->pdata->posX;
							posroiy = tempo->pdata->posY;
							break;
						}
						tempo = tempo->suiv;
					}
					jeu[posroix][posroiy] = NULL;
					cont = 0;
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
			libererListe(&unite_horde, jeu);
			libererListe(&unite_tour, jeu);
			libererListe(&newunite_horde, jeu);
			libererListe(&newunite_tour, jeu);
			newchemin = chargerseq(jeu, &newunite_horde, &newunite_tour, &newcases);
			ischarged = true;
			i = 0;
			
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(400);
		}
		if ( pKeyStates[SDL_SCANCODE_C] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur C */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			libererListe(&unite_horde, jeu);
			libererListe(&unite_tour, jeu);
			libererListe(&newunite_horde, jeu);
			libererListe(&newunite_tour, jeu);
			newchemin = chargerbin(jeu, &newunite_horde, &newunite_tour, &newcases);
			ischarged = true;
			i = 0;
			
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(300);
		}
		if ( pKeyStates[SDL_SCANCODE_D] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			sauvegarderseq(jeu, unite_horde, unite_tour, tabParcours, nbcase);
			//Ne pas modifiez les 4 lignes ci-dessous
			efface_fenetre(pWinSurf);
			prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf, nbcase);
			maj_fenetre(pWindow);
			SDL_Delay(300);
		}
		if ( pKeyStates[SDL_SCANCODE_S] ){
			/* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur S */
			// APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
			sauvegarderbin(jeu, unite_horde, unite_tour, tabParcours, nbcase);
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
    return 0;
}
