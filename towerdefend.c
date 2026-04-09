#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include "towerdefend.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ENTREE1 "partieseq.txt"
#define SORTIE1 "partieseq.txt"

void libererListe(TListePlayer *liste, TplateauJeu jeu) {
    if (liste == NULL) return;
    TListePlayer tmp = *liste;
    while (tmp != NULL) {
        TListePlayer suivant = tmp->suiv;
        // Effacer la case du plateau
        if (jeu != NULL && tmp->pdata != NULL) {
            jeu[tmp->pdata->posX][tmp->pdata->posY] = NULL;
        }
        free(tmp->pdata);   // libère la Tunite
        free(tmp);          // libère la cellule de liste
        tmp = suivant;
    }
    *liste = NULL;
}


int sauvegarderseq(TplateauJeu jeu, TListePlayer horde, TListePlayer tour, int** tabParcours, int nbcase){
    FILE *f_out;

    // DEMO D ECRITURE DANS UN FICHIER SEQUENTIEL
    if ((f_out = fopen(SORTIE1,"w")) == NULL){
        fprintf(stderr, "\nErreur: Impossible d'ecrire dans le fichier %s\n",SORTIE1);
        return EXIT_FAILURE;
    }

    printf("on commence a écire \n");
    //écriture de l'entier lu dans le fichier SORTIE ("sortie.txt")
    int nb_horde = tailleListe(horde);
    fprintf(f_out,"%d\n", nb_horde);  //on mémorise dans le fichier le nombre d'entier qui y seront mis ensuite

    for (int i=0;i<nb_horde;i++){
        printf("on fait hordre num %d\n",i);
        //écriture de l'entier lu dans le fichier SORTIE ("sortie.txt")
        char* nom_horde = enumtochar(horde->pdata);
        fprintf(f_out,"%s, ",nom_horde); // TRES IMPORTANT : REPERER L ESPACE APRES LE %d, celui-ci va permetre de séparer les entiers pour lors de la lecture future du fichier
        fprintf(f_out,"pos x :%d, ",horde->pdata->posX);
        fprintf(f_out,"pos y :%d, ",horde->pdata->posY);
        fprintf(f_out,"indice :%d, ",horde->pdata->indiceParcours);
        fprintf(f_out,"pv :%d; \n",horde->pdata->pointsDeVie);
        horde = horde->suiv;
    }
    TListePlayer tmp = tour;
    int nb_tour = tailleListe(tmp);
    fprintf(f_out,"%d\n", nb_tour);  //on mémorise dans le fichier le nombre d'entier qui y seront mis ensuite
    for (int i=0;i<nb_tour;i++){
        printf("on fait tour num %d\n",i);
        //écriture de l'entier lu dans le fichier SORTIE ("sortie.txt")
        char* nom_tour = enumtochar(tmp->pdata);
        fprintf(f_out,"%s, ",nom_tour); // TRES IMPORTANT : REPERER L ESPACE APRES LE %d, celui-ci va permetre de séparer les entiers pour lors de la lecture future du fichier
        fprintf(f_out,"pos x :%d, ",tmp->pdata->posX);
        fprintf(f_out,"pos y :%d;\n",tmp->pdata->posY);
        tmp = tmp->suiv;
    }
    fprintf(f_out,"%d\n", nbcase);
    for (int i=0; i<nbcase; i++){
        fprintf(f_out,"%d,%d\n",tabParcours[i][X], tabParcours[i][Y]);
    }
    fprintf(f_out,";");


    //fermeture du fichier
    fclose(f_out);
    printf("Le fichier sortie.txt a ete cree, essayez de le lire avec un notepad++, gedit, etc.\n");
    return 0;
}

char* enumtochar(Tunite *unite){
    switch (unite->nom) {
        case tourSol:
            return "tourSol";
        case tourAir:
            return "tourAir";
        case tourRoi:
            return "tourRoi";
        case archer:
            return "archer";
        case chevalier:
            return "chevalier";
        case dragon:
            return "dragon";
        case gargouille:
            return "gargouille";
        default:
            return "inconnu";
    }
}

int** chargerseq(TplateauJeu jeu, TListePlayer *horde, TListePlayer *tour, int *nbcase){
    FILE *f_in;
    int nb_horde;
    int** fauxchemin = NULL;
    if ((f_in = fopen(SORTIE1,"r")) == NULL){
        fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n",SORTIE1);
        return fauxchemin;
    }
    initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU,fauxchemin);
    fscanf(f_in,"%d",&nb_horde);  //valable uniquement parceque je sais que j'ai un entier qui represente le nombre d'entiers qui va suivre
    for (int i=0;i<nb_horde;i++){
        char nom[256];
        char c = 0;
        int j = 0;
        
        do {
            fscanf(f_in, "%c", &c);
        } while (c == ' ' || c == '\n' || c == '\r');
        // Premier caractère valide déjà lu, on l'ajoute
        nom[j++] = c;


        while(c != ','){
            fscanf(f_in, "%c", &c);
            if (c == ',') break;
            nom[j++] = c;
        }
        nom[j] = '\0';

        printf("%256s, ", nom);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int posX = 0;
        fscanf(f_in, "%d", &posX);
        printf("posX : %d, ", posX);
        fscanf(f_in, "%c", &c);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int posY = 0;
        fscanf(f_in, "%d", &posY);
        printf("posY : %d, ", posY);
        fscanf(f_in, "%c", &c);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int indice = 0;
        fscanf(f_in, "%d", &indice);
        printf("indice : %d, ", indice);
        fscanf(f_in, "%c", &c);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int pv = 0;
        fscanf(f_in, "%d", &pv);
        printf("pv : %d;\n", pv);

        fscanf(f_in, "%c", &c);

        Tunite *unite_horde = NULL;
        if (strcmp(nom,"gargouille")==0){
            unite_horde=creeGargouille(posX,posY);
        }
        else if (strcmp(nom,"dragon")==0){
            unite_horde=creeDragon(posX,posY);
        }
        else if (strcmp(nom,"archer")==0){
            unite_horde=creeArcher(posX,posY);
        }
        else if (strcmp(nom,"chevalier")==0){
            unite_horde=creeChevalier(posX,posY);
        }
        else {
            printf("BRUUUUUUHHHH\n");
        }
        if (unite_horde != NULL){
            unite_horde->indiceParcours=indice;
            unite_horde->pointsDeVie=pv;
            AjouterUnite(horde,unite_horde);
            jeu[posX][posY]=unite_horde;
        }
     }

    int nb_tour = 0;
    fscanf(f_in, "%d", &nb_tour);
    for (int i = 0; i < nb_tour; i++){
        char nom[256];
        char c = 0;
        int j = 0;

        do {
            fscanf(f_in, "%c", &c);
        } while (c == ' ' || c == '\n' || c == '\r');
        // Premier caractère valide déjà lu, on l'ajoute
        nom[j++] = c;

        while(c != ','){
            fscanf(f_in, "%c", &c);
            if (c == ',') break;
            nom[j++] = c;
        }
        nom[j] = '\0';
        printf("%s, ", nom);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int posX = 0;
        fscanf(f_in, "%d", &posX);
        printf("posX : %d, ", posX);
        fscanf(f_in, "%c", &c);

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int posY = 0;
        fscanf(f_in, "%d", &posY);
        printf("posY : %d;\n", posY);
        fscanf(f_in, "%c", &c);
        Tunite *unite_tour = NULL;
        if (strcmp(nom,"tourRoi") == 0){
            unite_tour = creeTourRoi(posX,posY);
        }
        else if (strcmp(nom,"tourAir") == 0)
        {
            unite_tour=creeTourAir(posX,posY);
        }
        else if (strcmp(nom,"tourSol") == 0)
        {
            unite_tour=creeTourSol(posX,posY);
        } 
        else {
            printf("BRUUUUUUHHHH\n");
        }
        if (unite_tour != NULL){
            AjouterUnite(tour,unite_tour);
            jeu[posX][posY]=unite_tour;
        }

    }
    fscanf(f_in, "%d", nbcase);
    int** newchemin = (int**)malloc(sizeof(int*)*(*nbcase));
    for (int j=0;j<(*nbcase);j++){
        newchemin[j] = (int*)malloc(sizeof(int)*2);  
    }
    for (int i = 0; i < (*nbcase); i++){
        int x = 0;
        int y = 0;
        char c = 0;
        fscanf(f_in, "%d", &x);
        printf("%d, ", x);
        fscanf(f_in, "%c", &c);
        fscanf(f_in, "%d", &y);
        printf("%d\n", y);
        newchemin[i][X]=x;
        newchemin[i][Y]=y;
    }
    //affiche_liste(unite_horde);
    printf("Le fichier sortie.txt a lu\n");
    fclose(f_in);

    return newchemin;
}


void affiche_liste (TListePlayer player)
{
    TListePlayer tmp = player;
    while (tmp!=NULL)
    {
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

TListePlayer creerTour(TplateauJeu jeu, int **tabParcours, int nb_tour,int x, int y)
{
    TListePlayer nouv= NULL;
     srand(time(NULL));
    for (int i = 0 ; i<=nb_tour ; i++)
    { int rand_tour = rand()%2;
        if (rand_tour==0)
        {
            Tunite *t = creeTourAir(x+i,y+i);
            t->indiceParcours = i;
            AjouterUnite(&nouv,t);
        }
        else if (rand_tour==1)
        {
            Tunite *t = creeTourSol(x+i,y+i);
            t->indiceParcours = i;
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

TListePlayer creer_tour_roi (TplateauJeu jeu, int ** Parcours, int x , int y){

    TListePlayer tmp = NULL;
    Tunite *t = creeTourRoi(x, y);
    t->indiceParcours = 0;
    AjouterUnite(&tmp,t);
    jeu[tmp->pdata->posX][tmp->pdata->posY] = tmp->pdata;
    return tmp;
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
    return nouv;
}

TListePlayer creer_rand_unite(TplateauJeu jeu, int **tabParcours, int x, int y, TListePlayer horde, int rand_unite){
   // if (rand_unite >= 15 && rand_unite <= 50){
        int rand_unitee = rand()%4;
        Tunite *u;
        int startX = tabParcours[0][X];
        int startY = tabParcours[0][Y];
        if (rand_unitee == 0){
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
   // }
   // return horde;
}

TListePlayer creer_rand_tour(TplateauJeu jeu, int **tabParcours, TListePlayer tour, int rand_tour, int nbcase){
    bool boolen = false;
    //if (rand_tour >= 5 && rand_tour <= 60){
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
        //}
    }
   return tour;
}

void deplacer_horde(TplateauJeu jeu, int** tabParcours, TListePlayer horde, int nbcase){
    if (horde == NULL){
        return;
    }
    while (horde != NULL){
        int i = horde->pdata->indiceParcours;
        if (i >= nbcase-1){  //si la horde a atteint la fin du parcours
            //printf("la horde a atteint le roi\n");
            if (horde->suiv != NULL){
                horde = horde -> suiv;
                continue;
            }
            else return;
        }
        int x = horde->pdata->posX;
        int y = horde->pdata->posY;
        i++;
        int newX = tabParcours[i][X];
        int newY = tabParcours[i][Y];

        if (jeu[newX][newY] == NULL){
            jeu[x][y] = NULL;
            horde->pdata->posX = newX;
            horde->pdata->posY = newY;
            jeu[newX][newY] = horde->pdata;
            horde->pdata->indiceParcours = i;
        } else {
            i--;
        }
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
    //jeu[5][3]=creeTourSol(5,3);
    //jeu[3][3]=creeTourAir(3,3);
    //jeu[4][1]=creerTour(jeu,4,1,1,tourRoi);
    //jeu[4][15]=creeTourAir(4,15);
    //jeu[5][17]=creeDragon(5,17);
}

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

int **initChemin(int *nbcase, int *x, int *y){
    srand(time(NULL));  //pour que le chemin soit différent à chaque lancement du programme
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = rand()%10;  //5 = milieu de la fenetre de 11 de largeur (0-10)
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
    nouv->degats = 8;
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
    nouv->degats = 5;
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
    nouv->degats = 20;
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
        TListePlayer a_portee_head = a_portee;  // ← garder la tête
        TListePlayer taille = *UniteAttaquante;
        int tailleportee = tailleListe(a_portee);
        if (tailleportee > 0){
            printf("unite a portee : \n");
            tri_selection(UniteAttaquante, tailleListe(taille));
            TListePlayer tmpPortee = *UniteAttaquante;
            affiche_liste(tmpPortee);
        }

        if (a_portee != NULL) {
            // L'unité attaque la tour du roi
            while (a_portee != NULL){
                if (a_portee->pdata->nom == tourRoi){
                    combat(i, tmp->pdata, a_portee->pdata);
                }
                a_portee = a_portee -> suiv;;
            }

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
            TListePlayer toFree = a_portee_head;
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

void combat(int i, Tunite *UniteAttaquante, Tunite *UniteCible) {
    if (UniteAttaquante == NULL || UniteCible == NULL) return;
    if (UniteAttaquante->pointsDeVie <= 0) return;
    if (UniteCible->pointsDeVie <= 0) return;

    // vérifie que l'attaquant peut cibler le type de la cible
    if (UniteAttaquante->cibleAttaquable != solEtAir) {
        if (UniteAttaquante->cibleAttaquable != UniteCible->maposition) return;
    }

    // attaque tous les i tours selon la vitesse
    if (i % 2 == 0) {
        UniteCible->pointsDeVie -= UniteAttaquante->degats;
        printf("combat : %d attaque %d, PV restants : %d\n",
               UniteAttaquante->nom, UniteCible->nom, UniteCible->pointsDeVie);
    }
}


TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer a_portee = NULL;
    if (UniteAttaquante == NULL) return NULL;

    int portee = UniteAttaquante->portee;

    for (int i = UniteAttaquante->posX - portee; i <= UniteAttaquante->posX + portee; i++) {
        for (int j = UniteAttaquante->posY - portee; j <= UniteAttaquante->posY + portee; j++) {
            if ((i >= 0 && i < 11 && j >= 0 && j < 19
                && jeu[i][j] != NULL)
                && (jeu[i][j]->nom == tourRoi || jeu[i][j]->nom == tourAir || jeu[i][j]->nom == tourSol)) {
                AjouterUnite(&a_portee, jeu[i][j]);
            }
        }
    }
    affiche_liste(a_portee);
    return a_portee;
}

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

int tailletab(int **tab, int max){
    int i = 0;
    while(i < max && tab[i] != NULL){
        i++;

    }
    return i;
}