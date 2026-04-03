#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include "towerdefend.h"
#include <stdbool.h>
#include <time.h>


//typedef Tunite* ** TplateauJeu;


void affiche_liste (TListePlayer player)
{
    while (player!=NULL)
    {
        printf ("%d  ",player->pdata->pointsDeVie);
        player=player->suiv;
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


void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite)
{
   TListePlayer nouv=(TListePlayer)malloc(sizeof(struct T_cell));
   if (nouv == NULL){
        return;
   }
   nouv->pdata=nouvelleUnite;
   nouv->suiv=*player;
   *player=nouv;
}




void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite)
{
    if (player == NULL || *player == NULL || UniteDetruite == NULL)
    {
        return;
    }

    TListePlayer courant = *player;
    TListePlayer precedent = NULL;

    // Parcourir la liste pour trouver l'unité à supprimer
    while (courant != NULL && courant->pdata != UniteDetruite)
    {
        precedent = courant;
        courant = courant->suiv;
    }

    // Si on a trouvé l'unité
    if (courant != NULL)
    {
        // Si c'est le premier élément
        if (precedent == NULL)
        {
            *player = courant->suiv;
        }
        else
        {
            // Sinon, on réajuste le pointeur du précédent
            precedent->suiv = courant->suiv;
        }

        // Libérer la mémoire
        free(courant->pdata);
        free(courant);
    }
}

bool tourRoiDetruite(TListePlayer player){
    while (player != NULL){
        if (player->pdata->nom == tourRoi){
            if ((player->pdata->pointsDeVie) <= 0){

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

TListePlayer creerTour(TplateauJeu jeu, int **tabParcours, int nb_tour, TuniteDuJeu nouvelleUnite,int x, int y)
{
   //TListePlayer nouv=(TListePlayer)malloc(sizeof(struct T_cell));
    TListePlayer nouv= NULL;

    for (int i = 0 ; i<nb_tour ; i++)
    {
        if (nouvelleUnite==tourAir)
        {
            Tunite *t = creeTourAir(6,3); //c'est un teste
            t->indiceParcours = 0;
            AjouterUnite(&nouv,t);
        }
        else if (nouvelleUnite==tourSol)
        {
            Tunite *t = creeTourSol(5,2); //teste
            t->indiceParcours = 0;
            AjouterUnite(&nouv,t);
        }
        else if (nouvelleUnite==tourRoi)
        {
            Tunite *t = creeTourRoi(x, y);
            t->indiceParcours = 0;
            AjouterUnite(&nouv,t);
        }
    }
    TListePlayer tmp = nouv;
    while (tmp != NULL)
    {
        jeu[tmp->pdata->posX][tmp->pdata->posY] = tmp->pdata;
        tmp = tmp->suiv;
    }
    return nouv;
}
TListePlayer creerhorde(TplateauJeu jeu, int x, int y, int nb_horde){
    TListePlayer nouv = NULL;
    for (int i = 0; i < nb_horde; i++){
        int rand_unite = rand()%4;
        Tunite *u;
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
    PositionnePlayerOnPlateau(nouv, jeu);
    return nouv;
}

void deplacer_horde(TplateauJeu jeu, int** tabParcours, TListePlayer horde, int nbcase){
    if (horde == NULL){
        return;
    }
    while (horde != NULL){
        int i = horde->pdata->indiceParcours;
        printf("indice parcours : %d\n", nbcase);
        if (i >= nbcase-1){  //si la horde a atteint la fin du parcours
            //printf("la horde a atteint le roi\n");
            return;
        }
        int x = horde->pdata->posX;
        int y = horde->pdata->posY;
        jeu[x][y] = NULL;

        i++;
        horde->pdata->indiceParcours = i;

        int newX = tabParcours[i][X];
        int newY = tabParcours[i][Y];

        horde->pdata->posX = newX;
        horde->pdata->posY = newY;
        jeu[newX][newY] = horde->pdata;

        //printf("pos x : %d, y : %d\n", newX, newY);
        horde = horde->suiv;
    }
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur, int** tabParcours){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }

    //POUR LA DEMO D'AFFICHAGE UNIQUEMENT, A SUPPRIMER
    //(les tours ici ne sont pas li es aux listes des unit s de vos joueurs)
    //jeu[5][3]=creeTourSol(5,3);
    //jeu[3][3]=creeTourAir(3,3);
    //jeu[4][1]=creerTour(jeu,4,1,1,tourRoi);
    //jeu[4][15]=creeTourAir(4,15);
    //jeu[5][17]=creeDragon(5,17);

    //test pour un dragon

    //jeu[5][17]=creerhorde(5,17,1);
    //jeu[5][17]=creeDragon(5,17);
    //jeu[5][17]=creeDragon(5,17);
    //jeu[5][16]=creeDragon(5,16);*


    //si on a une liste de dragon :



    //FIN DEMO AFFICHAGE
}

/*TListePlayer init_horde()
    TListePlayer unite_horde = creerhorde(5,17,1,gargouille);
    TListePlayer tmp = unite_horde;
    while (tmp != NULL)
    {
        jeu[tmp->pdata->posX][tmp->pdata->posY] = tmp->pdata;
        tmp = tmp->suiv;
    }

*/
/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction,   partir d'un point x y donn

int **chemin  : tab de coordonn es x y du chemin
int *ichemin  : indice de la case du chemin d'o  on part
int *xdepart, int *ydepart : valeur en x y de d part pouri la premiere case
int distance  : distance sur laquelle on va  crire des coordonn es dans le tab chemin
int *distanceMaxRestante : securit  pour ne pas sortir de la plage d'indice de chemin
*/

void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;  // ← MANQUAIT
        int y;
        for (y = *ydepart; y > *ydepart - distance; y--){
            if (*ichemin >= NBCOORDPARCOURS) break;  // ← sécurité
            chemin[*ichemin][X] = *xdepart;
            chemin[*ichemin][Y] = y;
            (*ichemin)++;
        }
        *ydepart = y;
    }
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;  // ← MANQUAIT
        int x;
        for (x = *xdepart; x < *xdepart + distance; x++){
            if (*ichemin >= NBCOORDPARCOURS) break;  // ← sécurité
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
}

void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance) >= 0){
        *distanceMaxRestante -= distance;  // ← MANQUAIT
        int x;
        for (x = *xdepart; x > *xdepart - distance; x--){
            if (*ichemin >= NBCOORDPARCOURS) break;  // ← sécurité
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
}

/*
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int y;
        for (y=*ydepart;y>*ydepart-distance;y--){

            chemin[*ichemin][X]= *xdepart;
            chemin[*ichemin][Y]= y;
            (*ichemin)++;
        }
        *ydepart=y;
    }
    else return;
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x<*xdepart+distance;x++){

            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else return;
}
void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x>*xdepart-distance;x--){

            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else return;
}
*/
/*
int **initChemin(int *nbcase){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    *nbcase = i;

    return chemin;  //tab2D contenant des pointeurs
}

*/

int **initChemin(int *nbcase, int *x, int *y){
    srand(time(NULL));  //pour que le chemin soit différent à chaque lancement du programme
    int cheminmax = 50;
    int cheminmin = 34;
    int cheminalea = rand()%(cheminmax-cheminmin+1)+cheminmin;
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int brider = 0;
    int distanceMaxRestante = NBCOORDPARCOURS;
    int nbdistance = 0;
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 2, &distanceMaxRestante);
    int d = 0;
    while (d < 20){
        int alea = rand()%3;
        nbdistance = rand()%5+2;
        if (alea == 0 && (ydepart - nbdistance >= 1) && brider == 0){
            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            d+= nbdistance;
        } 
        else if(alea == 1 && (xdepart - nbdistance) >= 0 && brider == 0){
            ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            brider+=1;
        } 
        else if (alea == 2 && (xdepart + nbdistance) < 11 && brider == 0){
            ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            brider+=1;
        } 
        else if (brider == 1 && (ydepart - nbdistance >= 1)){
            nbdistance = rand()%3+2;
            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, nbdistance, &distanceMaxRestante);
            brider = 0;
            d+= nbdistance;
        }
        else {
            ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, abs(1 - ydepart), &distanceMaxRestante);
            break;
        }
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
    nouv->degats = 120;
    nouv->portee = 5;
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
    nouv->degats = 100;
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
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 150;
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
    nouv->degats = 180;
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
    nouv->degats = 120;
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
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}





void peut_attaquer(int i, TListePlayer *UniteAttaquante, TListePlayer Unitecible, TplateauJeu jeu) {
    if (*UniteAttaquante == NULL || Unitecible == NULL) {
        return;
    }

    TListePlayer tmp = *UniteAttaquante;
    while (tmp != NULL) {
        TListePlayer suivant = tmp->suiv;

        TListePlayer a_portee = quiEstAPortee(jeu, tmp->pdata);

        if (a_portee != NULL) {
            // L'unité attaque la tour du roi
            combat(i, tmp->pdata, a_portee->pdata);

            // La tour du roi attaque si l'unité est dans sa portée
            TListePlayer tmpTour = Unitecible;
            while (tmpTour != NULL) {
                int dx = abs(tmp->pdata->posX - tmpTour->pdata->posX); // les abs c'est pour eviter les problemes de distance negative
                int dy = abs(tmp->pdata->posY - tmpTour->pdata->posY);
                if (dx <= tmpTour->pdata->portee && dy <= tmpTour->pdata->portee) {
                    combat(i, tmpTour->pdata, tmp->pdata);
                }
                tmpTour = tmpTour->suiv;
            }
            TListePlayer taille = a_portee;
            tri_selection(&a_portee, tailleListe(taille));
            affiche_liste(a_portee);
            // free (haha c'est a cause de ça que ça marchait pas avant)
            TListePlayer toFree = a_portee;
            while (toFree != NULL) {
                TListePlayer next = toFree->suiv;
                free(toFree);
                toFree = next;
            }
        }

        // si l'unité attaquante a plus de pv, on supprime
        if (tmp->pdata->pointsDeVie <= 0) {
            //affiche_liste(a_portee);
            jeu[tmp->pdata->posX][tmp->pdata->posY] = NULL;
            printf("une unite a ete detruite\n");
            affiche_liste(*UniteAttaquante);
            supprimerUnite(UniteAttaquante, tmp->pdata);
        }


        tmp = suivant;
    }
}


void combat(int i, Tunite * UniteAttaquante, Tunite * UniteCible){
    if (UniteAttaquante == NULL || UniteCible == NULL){
        return;
    }

    if (UniteAttaquante->pointsDeVie > 0){
        if (UniteAttaquante->vitesseAttaque <= UniteCible->vitesseAttaque && i%2==0){
            UniteCible->pointsDeVie -= UniteAttaquante->degats;
            UniteAttaquante->peutAttaquer = 0;
            UniteCible->peutAttaquer = 1;
        } else {
            UniteAttaquante->pointsDeVie -= UniteCible->degats;
            UniteCible->peutAttaquer = 0;
            UniteAttaquante->peutAttaquer = 1;
        }
    } else {
        UniteAttaquante->peutAttaquer = 0;
    }
}


TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer a_portee = NULL;
    if (UniteAttaquante == NULL) return NULL;

    int portee = UniteAttaquante->portee;

    for (int i = UniteAttaquante->posX - portee; i <= UniteAttaquante->posX + portee; i++) {
        for (int j = UniteAttaquante->posY - portee; j <= UniteAttaquante->posY + portee; j++) {
            if (i >= 0 && i < 11 && j >= 0 && j < 19
                && jeu[i][j] != NULL
                && jeu[i][j]->nom == tourRoi) {
                AjouterUnite(&a_portee, jeu[i][j]);
            }
        }
    }
    return a_portee;
}
/*
void tri_selection(TListePlayer *UniteAttaquante){
    if(UniteAttaquante == NULL){
        return;
    }

    TListePlayer actuel = *UniteAttaquante;

    while(actuel != NULL){
        TListePlayer min = actuel;
        TListePlayer ind = actuel->suiv;

        while(ind != NULL && min->pdata != NULL){
            if(ind->pdata->pointsDeVie < min->pdata->pointsDeVie){
                min = ind;
            }
            ind = ind->suiv;
        }

        if(min != actuel){
            Tunite *tmp = actuel->pdata;
            actuel->pdata = min->pdata;
            min->pdata = tmp;
        }

        actuel = actuel->suiv;
    }
}
*/
void tri_selection(TListePlayer *UniteAttaquante, int taille){
    if(UniteAttaquante == NULL) return;

    TListePlayer actuel = *UniteAttaquante;
    while(actuel != NULL){
        TListePlayer min = actuel;
        TListePlayer ind = actuel->suiv;

        while(ind != NULL){
            if(ind->pdata->pointsDeVie < min->pdata->pointsDeVie){
                min = ind;
            }
            ind = ind->suiv;
        }

        if(min != actuel){
            Tunite *tmp = actuel->pdata;
            actuel->pdata = min->pdata;
            min->pdata = tmp;
        }

        actuel = actuel->suiv;
    }

    // réassigne les indiceParcours et positions selon l'ordre de la liste
    int i = 5;
    TListePlayer tmp = *UniteAttaquante;
    while(tmp != NULL){
        tmp->pdata->indiceParcours = i;
        tmp->pdata->posX = 5;
        tmp->pdata->posY = 17 - i;
        i--;
        tmp = tmp->suiv;
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
    int taille = 0;
    while (player != NULL){
        taille++;
        player = player->suiv;
    }
    return taille;
}


int tailletab(int **tab, int max){
    int i = 0;
    while(i < max && tab[i] != NULL){
        i++;

    }
    return i;
}


















