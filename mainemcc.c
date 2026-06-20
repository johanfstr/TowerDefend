#include "SDL.h"
#include "maSDL.h"
#include "towerdefend.h"
#include "sauvegarde.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// ===================== VARIABLES GLOBALES =====================
SDL_Window *pWindow;
SDL_Surface* pWinSurf;
SDL_Surface* TabSprite[11];

TplateauJeu jeu;
int** tabParcours;

int** newchemin = NULL;
int nbcase = 0;
int x, y;

bool ischarged;
TListePlayer newunite_horde;
TListePlayer newunite_tour;
TListePlayer unite_horde;
TListePlayer unite_tour;

int i = 0;
int cont = 1;
int newcases = 0;

// ===================== LOOP =====================
void loop() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            cont = 0;
    }

    const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);

    if (pKeyStates[SDL_SCANCODE_ESCAPE])
        cont = 0;

    if (!cont) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    efface_fenetre(pWinSurf);

    if (!ischarged) {

        prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf, nbcase);

        int random = rand() % 101;

        if (random >= 15 && random <= 50)
            unite_horde = creer_rand_unite(jeu, tabParcours, x, y, unite_horde);

        else if (random >= 5 && random <= 60 && tailleListe(unite_tour) < 11)
            unite_tour = creer_rand_tour(jeu, tabParcours, unite_tour, nbcase);

        if (unite_horde != NULL) {
            deplacer_horde(jeu, tabParcours, unite_horde, nbcase);

            TListePlayer a_portee = quiEstAPortee(jeu, unite_horde->pdata);
            if (a_portee != NULL) {
                printf("PV tour: %d PV horde: %d\n",
                       a_portee->pdata->pointsDeVie,
                       unite_horde->pdata->pointsDeVie);
            }

            peut_attaquer(pWinSurf, i, &unite_horde, unite_tour, jeu);

            if (tourRoiDetruite(unite_tour)) {
                printf("FIN DU JEU\n");
                cont = 0;
            }
        }

        i++;
    }
    else {

        prepareAllSpriteDuJeu(jeu, newchemin, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf, newcases);

        int random = rand() % 101;

        if (random >= 15 && random <= 50)
            newunite_horde = creer_rand_unite(jeu, newchemin, x, y, newunite_horde);

        else if (random >= 5 && random <= 60 && tailleListe(newunite_tour) < 11)
            newunite_tour = creer_rand_tour(jeu, newchemin, newunite_tour, newcases);

        if (newunite_horde != NULL) {
            deplacer_horde(jeu, newchemin, newunite_horde, newcases);
            peut_attaquer(pWinSurf, i, &newunite_horde, newunite_tour, jeu);

            if (tourRoiDetruite(newunite_tour)) {
                printf("FIN DU JEU\n");
                cont = 0;
            }
        }

        i++;
    }

    maj_fenetre(pWindow);

    // IMPORTANT : en web, pas de gros delay
    SDL_Delay(200);
}

// ===================== MAIN =====================
int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Tower Defense Web",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU * 40,
        HAUTEURJEU * 40,
        SDL_WINDOW_SHOWN
    );

    pWinSurf = SDL_GetWindowSurface(pWindow);

    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data/TourSol.bmp");
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data/TourAir.bmp");
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp");
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp");
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp");
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp");
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp");
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp");
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp");
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont.bmp");
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp");

    TabSprite[0] = pSpriteTourSol;
    TabSprite[1] = pSpriteTourAir;
    TabSprite[2] = pSpriteTourRoi;
    TabSprite[3] = pSpriteArcher;
    TabSprite[4] = pSpriteChevalier;
    TabSprite[5] = pSpriteDragon;
    TabSprite[6] = pSpriteGargouille;
    TabSprite[7] = pSpriteEau;
    TabSprite[8] = pSpriteHerbe;
    TabSprite[9] = pSpritePont;
    TabSprite[10] = pSpriteTerre;

    tabParcours = initChemin(&nbcase, &x, &y);

    jeu = AlloueTab2D(LARGEURJEU, HAUTEURJEU);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU, tabParcours);

    unite_tour = creer_tour_roi(jeu, tabParcours, x, y);

    srand(time(NULL));
    ischarged = false;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (cont) loop();
#endif

    SDL_Quit();
    return 0;
}