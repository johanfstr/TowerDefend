#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include "towerdefend.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "maSDL.h"
#include <SDL2/SDL_surface.h>
#include "sauvegarde.h"

void affiche_liste (TListePlayer player){
    TListePlayer tmp = player;
    while (tmp!=NULL){ // parcours tous les éléments de la liste
        printf ("%d  ",tmp->pdata->pointsDeVie);
        tmp=tmp->suiv;
    }
    printf ("\n");
}

TplateauJeu AlloueTab2D(int largeur, int hauteur){
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;  //tab2D contenant des pointeurs
}

//Ajout en tête de liste
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite) {
    if (player == NULL || nouvelleUnite == NULL){
        return;
    }
   TListePlayer nouv=(TListePlayer)malloc(sizeof(struct T_cell)); 
   if (nouv == NULL){
        return;
   }
   nouv->pdata=nouvelleUnite; 
   nouv->suiv=*player;
   *player=nouv;
}

//Parcours la liste jusqu'à trouver l'unité à supprimer, puis la supprime de la liste
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite){
    if (player == NULL || *player == NULL || UniteDetruite == NULL){
        return;
    }
    TListePlayer courant = *player;
    TListePlayer precedent = NULL;
    // Parcourir la liste pour trouver l'unité à supprimer
    while (courant != NULL && courant->pdata != UniteDetruite){
        precedent = courant;
        courant = courant->suiv;
    }
    // Si on a trouvé l'unité
    if (courant != NULL){
        // Si c'est le premier élément
        if (precedent == NULL){
            *player = courant->suiv;
        }
        else{
            // Sinon, on réajuste le pointeur du précédent
            precedent->suiv = courant->suiv;
        }

        // Libérer la mémoire
        free(courant->pdata);
        free(courant);
    }
}

bool tourRoiDetruite(TListePlayer player){
    while (player != NULL){ //parcours la liste des tours 
        if (player->pdata->nom == tourRoi){ //jusqu'à trouver la tour du roi
            if ((player->pdata->pointsDeVie) <= 0){ // si elle a un pv <= 0, elle est détruite
                return true;
            }
            else {
                return false;
            }
        }
        player = player->suiv;
    }
    return false;
}

TListePlayer creer_tour_roi (TplateauJeu jeu, int ** Parcours, int x , int y){
    TListePlayer tmp = NULL; // on initilise la liste des tours à NULL
    Tunite *t = creeTourRoi(x, y);
    t->indiceParcours = 0;
    AjouterUnite(&tmp,t);
    jeu[tmp->pdata->posX][tmp->pdata->posY] = tmp->pdata;
    return tmp;
}

//fonction de départ pour créer un nombre donné d'hordes aléatoires
TListePlayer creerhorde(TplateauJeu jeu, int x, int y, int nb_horde){
    TListePlayer nouv = NULL;
    for (int i = 0; i < nb_horde; i++){ //on crée nb_horde unités, chacune sur une case du parcours l'une après l'autre 
        Tunite *u;
        int rand_unite = rand()%4; // on a 4 choix d'unités possibles : archer, chevalier, dragon ou gargouille
        int PosY = y - i;
        if (rand_unite == 0){
            u = creeDragon(x, PosY);
        } else if (rand_unite == 1){
            u = creeGargouille(x, PosY);
        } else if (rand_unite == 2){
            u = creeChevalier(x, PosY);
        } else {
            u = creeArcher(x, PosY);
        }
    u->indiceParcours = i;
    AjouterUnite(&nouv, u);
    }
    tri_selection(&nouv, nb_horde);

    printf("horde creee : \n");
    TListePlayer tmp = nouv;;
    affiche_liste(tmp);
    return nouv;
}

//fonction utilisée pour créer une unité aléatoire qu'on ajoute dans la liste tout le long du jeu à chaque tour
TListePlayer creer_rand_unite(TplateauJeu jeu, int **tabParcours, int x, int y, TListePlayer horde){
        int rand_unitee = rand()%4;
        Tunite *u;
        int startX = tabParcours[0][X];
        int startY = tabParcours[0][Y];
        if (rand_unitee == 0){ // on a 4 choix d'unités possibles : archer, chevalier, dragon ou gargouille
            u = creeDragon(startX, startY);
        } else if (rand_unitee == 1){
            u = creeGargouille(startX, startY);
        } else if (rand_unitee == 2){
            u = creeChevalier(startX, startY);
        } else {
            u = creeArcher(startX, startY);
        }
        u->indiceParcours = 0;
        jeu[startX][startY] = u;
        AjouterUnite(&horde, u);
        return horde;
}

//calcule le score d'un emplacement pour une tour en fonction du nombre de cases du parcours qui sont à portée de la tour
int score_emplacement(int** tabParcours, int nbcase, int x, int y){
    int score = 0;
    int portee = 3; // Portée maximale d'une tour
    for (int i = 0; i < nbcase; i++) { //on parcours chaque case du tabparcours
        int dx = abs(tabParcours[i][X] - x); //distance entre les coordonnées X choisies dans creer_rand_tour et la case du parcours 
        int dy = abs(tabParcours[i][Y] - y); //distance entre les coordonnées Y choisies dans creer_rand_tour et la case du parcours 
        if (dx <= portee && dy <= portee) { //si la portée est <= distance calculée
            score++; // on augmente le score de 1 pour la case qui est à portée de la tour
        }
    }
    return score; // retourne le score correspondant au nombre de cases dans la portée de la tour 
}

TListePlayer creer_rand_tour(TplateauJeu jeu, int **tabParcours, TListePlayer tour, int nbcase){
    int meilleur_score = -1;
    int meilleur_scoreX = -1;
    int meilleur_scoreY = -1;
    bool surParcours = true;
    Tunite *t;
    int dx[] = {1, -1}; // Emplacement tour (droite, gauche)
    int dy[] = {1, -1}; // Emplacement tour (bas, haut)
    for (int i = 0; i < nbcase; i++) {
        int x = tabParcours[i][X];
        int y = tabParcours[i][Y];
        for (int j = 0; j < 2; j++) {
            int newX = x + dx[j]; // coordonnées X de la tour en dehors du chemin (droite ou gauche du chemin)
            int newY = y + dy[j]; // coordonnées Y de la tour en dehors du chemin (haut ou bas du chemin)
            if (newX >= 0 && newX < LARGEURJEU && newY >= 0 && newY < HAUTEURJEU) {
                if (newX <= 0 || newX >= LARGEURJEU || newY <= 0 || newY >= HAUTEURJEU) continue; // Si la case est en dehors du plateau, on passe à la suivante
                if (jeu[newX][newY] != NULL) continue; // Si la case n'est pas vide, on passe à la suivante
                // Vérifier que ce n'est pas une case du parcours
                surParcours = false; // on suppose qu'on est pas sur le parcours on vérifie ensuite
                for (int k = 0; k < nbcase; k++) { // parcours le chemin
                    if (tabParcours[k][X] == newX && tabParcours[k][Y] == newY) { // vérifie que les nouvelles coordonnées ne sont pas sur le chemin
                        surParcours = true;
                        break; // Si c'est le cas, on sort de la boucle et on passe à la suivante
                    }
                }
                if (surParcours == false) {
                    int score = score_emplacement(tabParcours, nbcase, newX, newY);
                    int score_avec_variation = score + (rand() % 3); // Ajouter une variation aléatoire au score pour éviter de réutiliser le même score
                    if (score_avec_variation > meilleur_score) { // compare avec le score précédent pour trouver le meilleur emplacement
                        meilleur_score = score_avec_variation; //si le nouveau score est supérieur on le garde comme meilleur score
                        meilleur_scoreX = newX;
                        meilleur_scoreY = newY;
                    }
                }
            }
        }
    }   
    if (meilleur_score != -1){ // si on a trouvé un score correspondant à un emplacement pour une tour
        int random = rand()%2;
        if (random == 0){
            t = creeTourAir(meilleur_scoreX, meilleur_scoreY);
        }
        else if (random == 1){
            t = creeTourSol(meilleur_scoreX, meilleur_scoreY);
        }
        t->indiceParcours = 0;
        jeu[t->posX][t->posY] = t;
        AjouterUnite(&tour,t);
        return tour;
    }
    printf("Pas de bon emplacement pour une tour\n");
    return tour; // si on n'a pas trouvé d'emplacement pour une tour, on retourne la liste de tours sans en ajouter de nouvelle
}

/* variante de la fonction pour créer tour aléatoirement sans utiliser le score d'emplacement (moins opti)
TListePlayer creer_rand_tour(TplateauJeu jeu, int **tabParcours, TListePlayer tour, int nbcase){
    bool boolen = false;
        Tunite *t;
        int rand_case = rand() % (nbcase - 2) + 1;
        int posX = tabParcours[rand_case][X];
        int posY = tabParcours[rand_case][Y];
        if ((posX + 1 != tabParcours[rand_case + 1][X])
        && (posX + 1 != tabParcours[rand_case][X])
        && (posX + 1 != tabParcours[rand_case - 1][X])
        && (posX + 1 < LARGEURJEU) && (jeu[posX+1][posY] == NULL)){
            posX++;
            boolen = true;
        } else if ((posX - 1 != tabParcours[rand_case + 1][X])
        && (posX - 1 != tabParcours[rand_case][X])
        && (posX - 1 != tabParcours[rand_case - 1][X])
        && (posX - 1 >= 0) && (jeu[posX-1][posY] == NULL)){
            posX--;
            boolen = true;
        } else if ((posY + 1 != tabParcours[rand_case + 1][Y])
        && (posY + 1 != tabParcours[rand_case][Y])
        && (posY + 1 != tabParcours[rand_case - 1][Y])
        && posY + 1 > 0 && (rand_case - 1 > 0) && (jeu[posX][posY+1] == NULL)){
            posY++;
            boolen = true;
        } else if ((posY - 1 != tabParcours[rand_case + 1][Y])
        && (posY - 1 != tabParcours[rand_case][Y])
        && (posY - 1 != tabParcours[rand_case - 1][Y])
        && posY - 1 < HAUTEURJEU && (rand_case - 1 > 0) && (jeu[posX][posY-1] == NULL)){
            posY--;
            boolen = true;
 }
        if (boolen){
            int random = rand()%2;
            if (random == 0){
                t = creeTourAir(posX,posY);
            }
            else if (random == 1){
                t = creeTourSol(posX,posY);
            }
            t->indiceParcours = 0;
            jeu[t->posX][t->posY] = t;
            AjouterUnite(&tour,t);
            return tour;
    }
   return tour;
}
*/

//déplace la horde d'une case sur le parcours, en vérifiant que la case d'arrivée est libre
void deplacer_horde(TplateauJeu jeu, int** tabParcours, TListePlayer horde, int nbcase){
    if (horde == NULL){
        return;
    }
    while (horde != NULL){//parcours la liste de la horde
        int i = horde->pdata->indiceParcours;
        if (i >= nbcase-1){  //si la horde a atteint la fin du parcours
            printf("la horde a atteint le roi\n");
            if (horde->suiv != NULL){ //si la suite de la liste n'est pas vide
                horde = horde -> suiv; // on va au suivant
                continue;
            }
            else return;
        }
        // on stocke les coordonnées actuelles de l'unité 
        int x = horde->pdata->posX;
        int y = horde->pdata->posY;
        i++; // on incrémente l'indice du parcours pour aller à la case suivante
        int newX = tabParcours[i][X]; // on donne les coordonnées de la case suivante
        int newY = tabParcours[i][Y]; 

        if (jeu[newX][newY] == NULL){ // si la case suivante est libre, on déplace l'unité
            jeu[x][y] = NULL; // on libère la case actuelle pour effacer l'unité de son ancienne position
            horde->pdata->posX = newX; // on met à jour les coordonnées de l'unité
            horde->pdata->posY = newY;
            jeu[newX][newY] = horde->pdata;
            horde->pdata->indiceParcours = i; // on met à jour l'indice du parcours de l'unité
        } else {
            i--; //si la case d'après est occupée, on ne peut pas déplacer l'unité, on remet son indice de parcours à celui d'avant
        }
        horde = horde->suiv; // on passe à l'unité suivante de la horde
    }
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur, int** tabParcours){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}

void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;
        int y;
        for (y = *ydepart; y > *ydepart - distance; y--){
            if (*ichemin >= NBCOORDPARCOURS) break;
            chemin[*ichemin][X] = *xdepart;
            chemin[*ichemin][Y] = y;
            (*ichemin)++;
        }
        *ydepart = y;
    }
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;
        int x;
        for (x = *xdepart; x < *xdepart + distance; x++){
            if (*ichemin >= NBCOORDPARCOURS) break;
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
}

void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;
        int x;
        for (x = *xdepart; x > *xdepart - distance; x--){
            if (*ichemin >= NBCOORDPARCOURS) break;
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
}

int **initChemin(int *nbcase, int *x, int *y){
    srand(time(NULL));  //pour que le chemin soit différent à chaque lancement du programme
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);
    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }
    int ydepart = 18;  //et non 19
    int xdepart = rand()%9+1;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int brider = 0;
    int distanceMaxRestante = NBCOORDPARCOURS;
    int nbdistance = 0;
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 2, &distanceMaxRestante);
    int d = 0;
    while (d < 20){
        nbdistance = rand()%4+2;  // entre 2 et 5
        int alea;
        if (brider == 0){
            // favorise les virages : 1 chance sur 3 de monter, 2 sur 3 de virer
            alea = rand()%3;  // 0 = haut, 1 = gauche, 2 = droite
        } else {
            // après un virage, on est forcé de monter
            alea = 0;
        }
        if (alea == 0 && (ydepart - nbdistance >= 1)){ // vers le haut
            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            d += nbdistance;
            brider = 0;
        }
        else if (alea == 1 && (xdepart - nbdistance) >= 0 && brider == 0){ // vers la gauche et force à ne pas aller encore à gauche
            ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            brider = 1;
        }
        else if (alea == 2 && (xdepart + nbdistance) < LARGEURJEU && brider == 0){ // vers la droite et force à ne pas aller encore à droite
            ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            brider = 1;
        }
        else if (ydepart - 2 >= 1){ // si on ne peut pas aller à gauche ou à droite, on est obligé de monter
            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 2, &distanceMaxRestante);
            d += 2;
            brider = 0;
        }
        else {
            break;
        }
        if (i >= NBCOORDPARCOURS - 1) break; // evite de dépasser 34 cases
    }
    *nbcase = i;
    *x = xdepart;
    *y = ydepart;
    return chemin;  //tab2D contenant des pointeurs
}

void afficheCoordonneesParcours(int **chemin, int nbcoord){
    printf("Liste coordonnees: ");
    for (int i=0; i<nbcoord; i++){
        printf("(%d, %d)",chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonn es\n");
}

void freeChemin(int **tab){
    for (int j=0;j<NBCOORDPARCOURS;j++){
        free(tab[j]);  //libere chaque case, qui est un tableau de 2 cases
    }
    free(tab);
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};
    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
                // A ne pas donner aux etudiants
            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(" ");  //cad pas d'unit  sur cette case
        }
        printf("\n");
    }
}

Tunite *creeTourSol(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 58;
    nouv->portee = 1;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourAir(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 55;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 1000;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 80;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 0.6;
    nouv->degats = 90;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeGargouille(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 150;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 90;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeChevalier(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 250;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 180;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

void peut_attaquer(SDL_Surface *surface, int i, TListePlayer *UniteAttaquante, TListePlayer Unitecible, TplateauJeu jeu) {
    if (*UniteAttaquante == NULL || Unitecible == NULL) {
        return;
    }
    TListePlayer tmp = *UniteAttaquante; // on stocke la liste dans une autre pour éviter de la modifier
    while (tmp != NULL) {
        TListePlayer suivant = tmp->suiv; // raccourci pour éviter de faire tmp->suiv 

        TListePlayer a_portee = quiEstAPortee(jeu, tmp->pdata); // on stocke la liste dans une autre pour éviter de la modifier
        TListePlayer a_portee_head = a_portee;  // garder la tête
        TListePlayer taille = *UniteAttaquante;
        int tailleportee = tailleListe(a_portee); // taille de la liste a portée pour la parcourir
        if (tailleportee > 0){ // si la liste a_portee n'est pas vide
            //printf("Liste pv à portée : ");
            tri_selection(UniteAttaquante, tailleListe(taille)); //on la trie
            //TListePlayer tmpPortee = *UniteAttaquante;
            //affiche_liste(tmpPortee);
            //printf("\n");
        }
        if (a_portee != NULL) {// si la liste a_portee n'est pas vide
            // L'unité attaque la tour du roi
            while (a_portee != NULL){// on parcours la liste des unités à portée juqu'à trouver la tour du roi
                if (a_portee->pdata->nom == tourRoi){
                    combat(surface, i, tmp->pdata, a_portee->pdata); // l'unité attaque la tour du roi si elle est à portée
                }
                a_portee = a_portee -> suiv;
            }
            // La tour du roi attaque si l'unité est dans sa portée
            TListePlayer tmpTour = Unitecible;
            while (tmpTour != NULL) { // parcours la liste des tours
                int dx = abs(tmp->pdata->posX - tmpTour->pdata->posX); // les abs c'est pour eviter les problemes de distance negative
                int dy = abs(tmp->pdata->posY - tmpTour->pdata->posY); // calcule la distance entre l'unité et la tour
                if (dx <= tmpTour->pdata->portee && dy <= tmpTour->pdata->portee) { // si la distance est dans la portée de la tour
                    combat(surface,i, tmpTour->pdata, tmp->pdata); // la tour attaque l'unité 
                }
                tmpTour = tmpTour->suiv;
            }
            TListePlayer toFree = a_portee_head;
            while (toFree != NULL) { // on libère la mémoire de la liste a_portee une fois qu'on a fini de l'utiliser
                TListePlayer next = toFree->suiv;
                free(toFree);
                toFree = next;
            }
        }
        // si l'unité attaquante a plus de pv, on supprime
        if (tmp->pdata->pointsDeVie <= 0) {
            //affiche_liste(a_portee);
            jeu[tmp->pdata->posX][tmp->pdata->posY] = NULL;
            printf("unite detruite : ");
            affiche_liste(*UniteAttaquante);
            printf("\n");
            supprimerUnite(UniteAttaquante, tmp->pdata);
        }
        tmp = suivant;
    }
}

void combat(SDL_Surface *surface, int i, Tunite *UniteAttaquante, Tunite *UniteCible) {
    if (UniteAttaquante == NULL || UniteCible == NULL) return; // si les listes sont vides, on ne fait rien
    if (UniteAttaquante->pointsDeVie <= 0) return; //si l'attaquant a plus de pv, il ne peut pas attaquer
    if (UniteCible->pointsDeVie <= 0) return; //si la cible a plus de pv, elle ne peut pas être attaquée

    // vérifie que l'attaquant peut cibler le type de la cible
    if (UniteAttaquante->cibleAttaquable != solEtAir) {
        if (UniteAttaquante->cibleAttaquable != UniteCible->maposition) return;
    }
    // attaque tous les 1 tours sur 2 et selon la vitesse (pour équilibrer le jeu)
    if (i % 2 == 0) { // les tours également attaque une fois sur 2 
        UniteCible->pointsDeVie -= UniteAttaquante->degats; // on enlève les points de vie de la cible avec dégâts de l'attaquant
        //dessineAttaque(surface, UniteAttaquante, UniteCible); on a fait la fonction combat avec dessine attaque mais ça rame beaucoup donc on l'a commente
        printf("combat : %s attaque %s, PV restants : %d\n",
               enumtochar(UniteAttaquante), enumtochar(UniteCible), UniteCible->pointsDeVie);
    }
}

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer a_portee = NULL; // on initialise la liste des unités a_portee à NULL
    if (UniteAttaquante == NULL) return NULL; // si l'unité attaquante est NULL, on retourne NULL
    int portee = UniteAttaquante->portee; // raccourci pour la portée de l'unité attaquante

    for (int i = UniteAttaquante->posX - portee; i <= UniteAttaquante->posX + portee; i++) { // parcours le jeu pour detecter ce qui est a portee pour chaque X du jeu
        for (int j = UniteAttaquante->posY - portee; j <= UniteAttaquante->posY + portee; j++) { // parcours le jeu pour detecter ce qui est a portee pour chaque Y du jeu
            if ((i >= 0 && i < 11 && j >= 0 && j < 19
                && jeu[i][j] != NULL) // s'il y a une unité dans la case du jeu
                && (jeu[i][j]->nom == tourRoi || jeu[i][j]->nom == tourAir || jeu[i][j]->nom == tourSol)) { // si l'unité est une tour
                AjouterUnite(&a_portee, jeu[i][j]); // on ajoute la tour à la liste a_portee
            }
        }
    }
    return a_portee;
}
//tri selection des points de vies qui échange les pointeurs pdata à leurs Tunité correspondants
void tri_selection(TListePlayer *UniteAttaquante, int taille){
    if(UniteAttaquante == NULL) return;

    TListePlayer actuel = *UniteAttaquante;
    while(actuel != NULL){
        TListePlayer min = actuel;
        TListePlayer ind = actuel->suiv;

        while(ind != NULL){
            if(ind->pdata->pointsDeVie < min->pdata->pointsDeVie){ // compare les points de vie pour trouver le minimum
                min = ind;
            }
            ind = ind->suiv;
        }

        if(min != actuel){
            // Echange les pdata des unités actuel et min
            Tunite *tmp = actuel->pdata; // Sauvegarde des données de l'unité actuelle
            int tmpIndice = actuel->pdata->indiceParcours;  // Sauvegarde de l'indice de parcours
            int posx = actuel->pdata->posX;  // Sauvegarde de la position X
            int posy = actuel->pdata->posY;  // Sauvegarde de la position Y
            actuel->pdata = min->pdata; // Échange des données
            actuel->pdata->indiceParcours = min->pdata->indiceParcours;  // Réassignation de l'indice de parcours
            actuel->pdata->posX = min->pdata->posX;  // Réassignation de la position X
            actuel->pdata->posY = min->pdata->posY;  // Réassignation de la position Y
            min->pdata = tmp; // Échange des données
            min->pdata->indiceParcours = tmpIndice;  // Réassignation de l'indice de parcours
            min->pdata->posX = posx;  // Réassignation de la position X
            min->pdata->posY = posy;  // Réassignation de la position Y
        }

        actuel = actuel->suiv;
    }
}

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu){
TListePlayer tmp = player;
    while (tmp != NULL){
        jeu[tmp->pdata->posX][tmp->pdata->posY] = tmp->pdata;
        tmp = tmp->suiv;
    }
}

int tailleListe(TListePlayer player){
    TListePlayer tmp = player;
    int taille = 0;
    while (tmp != NULL){
        if (tmp->pdata->pointsDeVie>0){
            taille++;
        }
        tmp = tmp->suiv;
    }
    return taille;
}
