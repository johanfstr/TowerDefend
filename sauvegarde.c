#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "towerdefend.h"
#include "sauvegarde.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ENTREE1 "partieseq.txt"
#define SORTIE1 "partieseq.txt"
#define ENTREE2 "partiebin.bin"
#define SORTIE2 "partiebin.bin"

//Sauvegarde la partie dans un fichier binaire
int sauvegarderbin(TplateauJeu jeu, TListePlayer horde, TListePlayer tour, int** tabParcours, int nbcase){
    FILE *f_out;

    if ((f_out = fopen(SORTIE2,"wb")) == NULL){
        fprintf(stderr, "\nErreur: Impossible d'ecrire dans le fichier %s\n",SORTIE2);
        return(EXIT_FAILURE);
    }

    int nb_horde = tailleListe(horde);
    fwrite(&nb_horde, sizeof(int),1,f_out);

    for (int i=0;i<nb_horde;i++){
        printf("on fait hordre num %d\n",i);
        //écriture de l'entier lu dans le fichier SORTIE ("sortie.txt")
        char* nom_horde = enumtochar(horde->pdata);
        int len_nom = strlen(nom_horde) + 1; // +1 pour le caractère de fin de chaîne
        fwrite(&len_nom, sizeof(int),1,f_out); // écrire la longueur de la chaîne avant d'écrire la chaîne elle-même
        fwrite(nom_horde, sizeof(char),len_nom,f_out); // écrire la chaîne de caractères (y compris le caractère de fin de chaîne)
        fwrite(&horde->pdata->posX, sizeof(int),1,f_out); // écrire la position X de l'unité
        fwrite(&horde->pdata->posY, sizeof(int),1,f_out); // écrire la position Y de l'unité
        fwrite(&horde->pdata->indiceParcours, sizeof(int),1,f_out); // écrire l'indice de parcours de l'unité
        fwrite(&horde->pdata->pointsDeVie, sizeof(int),1,f_out); // écrire les points de vie de l'unité
        horde = horde->suiv; // passer à l'unité suivante dans la liste d'horde
    }

    int nb_tour = tailleListe(tour);
    fwrite(&nb_tour, sizeof(int),1,f_out);

    for (int i=0;i<nb_tour;i++){
        printf("on fait tour num %d\n",i);
        //écriture de l'entier lu dans le fichier SORTIE ("sortie.txt")
        char* nom_tour = enumtochar(tour->pdata);
        int len_nom = strlen(nom_tour) + 1; // +1 pour le caractère de fin de chaîne
        fwrite(&len_nom, sizeof(int),1,f_out); // écrire la longueur de la chaîne avant d'écrire la chaîne elle-même
        fwrite(nom_tour, sizeof(char),len_nom,f_out); // écrire la chaîne de caractères (y compris le caractère de fin de chaîne)
        fwrite(&tour->pdata->posX, sizeof(int),1,f_out); // écrire la position X de la tour
        fwrite(&tour->pdata->posY, sizeof(int),1,f_out); // écrire la position Y de la tour
        fwrite(&tour->pdata->pointsDeVie, sizeof(int),1,f_out); // écrire les points de vie de la tour

        tour = tour->suiv;
    }

    fwrite(&nbcase, sizeof(int),1,f_out); // écrire le nombre de cases du parcours
    for (int i=0; i<nbcase; i++){
        fwrite(&tabParcours[i][X], sizeof(int),1,f_out); // écrire la coordonnée X de la case du parcours
        fwrite(&tabParcours[i][Y], sizeof(int),1,f_out); // écrire la coordonnée Y de la case du parcours
    }

    //fermeture du fichier
    fclose(f_out);
    printf("Le fichier sortie.txt a ete cree, essayez de le lire avec un notepad++, gedit, etc.\n");

    return EXIT_SUCCESS;

}

//Charge le fichier binaire
int** chargerbin(TplateauJeu jeu, TListePlayer *horde, TListePlayer *tour, int *nbcase){
    FILE *f_in;
    int** fauxchemin = NULL;

    if ((f_in = fopen(SORTIE2,"rb")) == NULL){
        fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n",SORTIE2);
        return fauxchemin;
    }
    initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU,fauxchemin);
    int nb_horde = 0;
    fread(&nb_horde, sizeof(int),1,f_in);
    printf("Nombre d'unités dans la horde : %d\n", nb_horde);

    for (int i=0; i<nb_horde;i++){
        int len_nom = 0;
        fread(&len_nom, sizeof(int),1,f_in);
        printf("longueur nom pour unité %d = %d\n",i+1,len_nom);
        char *nom_horde = (char*)malloc(sizeof(char)*len_nom + 1);
        for (int j=0; j<len_nom; j++){
            fread(&nom_horde[j], sizeof(char),1,f_in);
        }
        nom_horde[len_nom] = '\0'; // Ajouter le caractère de fin de chaîne

        for (int j=0; j<len_nom; j++){
            printf("%c", nom_horde[j]);
        }
        printf(" ");

        int posX = 0;
        fread(&posX, sizeof(int),1,f_in);
        printf("posX : %d, ", posX);

        int posY = 0;
        fread(&posY, sizeof(int),1,f_in);
        printf("posY : %d, ", posY);

        int indice = 0;
        fread(&indice, sizeof(int),1,f_in);
        printf("indice : %d, ", indice);

        int pv = 0;
        fread(&pv, sizeof(int),1,f_in);
        printf("pv : %d;\n", pv);

        Tunite *unite_horde = NULL;
        if (strcmp(nom_horde,"gargouille")==0){
            unite_horde=creeGargouille(posX,posY);
        }
        else if (strcmp(nom_horde,"dragon")==0){
            unite_horde=creeDragon(posX,posY);
        }
        else if (strcmp(nom_horde,"archer")==0){
            unite_horde=creeArcher(posX,posY);
        }
        else if (strcmp(nom_horde,"chevalier")==0){
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
        free(nom_horde); // Libérer la mémoire allouée pour le nom de l'unité
    }

    int nb_tour = 0;
    fread(&nb_tour, sizeof(int),1,f_in);
    printf("Nombre d'unités dans les tours : %d\n", nb_tour);

    for (int i = 0; i < nb_tour; i++){
        int len_nom = 0;
        fread(&len_nom, sizeof(int),1,f_in);
        printf("longueur nom pour unité %d = %d\n",i+1,len_nom);
        char *nom_tour = (char*)malloc(sizeof(char)*len_nom);
        for (int j=0; j<len_nom; j++){
            fread(&nom_tour[j], sizeof(char),1,f_in);
        }
        nom_tour[len_nom] = '\0'; // Ajouter le caractère de fin de chaîne


        for (int j=0; j<len_nom; j++){
            printf("%c", nom_tour[j]);
        }
        printf(" ");

        int posX = 0;
        fread(&posX, sizeof(int),1,f_in);
        printf("posX : %d, ", posX);

        int posY = 0;
        fread(&posY, sizeof(int),1,f_in);
        printf("posY : %d, ", posY);

        int pv = 0;
        fread(&pv, sizeof(int),1,f_in);
        printf("pv : %d;\n", pv);

        Tunite *unite_tour = NULL;
        if (strcmp(nom_tour,"tourRoi") == 0){
            unite_tour = creeTourRoi(posX,posY);
        }
        else if (strcmp(nom_tour,"tourAir") == 0)
        {
            unite_tour=creeTourAir(posX,posY);
        }
        else if (strcmp(nom_tour,"tourSol") == 0)
        {
            unite_tour=creeTourSol(posX,posY);
        }
        else {
            printf("BRUUUUUUHHHH\n");
        }
        if (unite_tour != NULL){
            unite_tour->pointsDeVie=pv;
            AjouterUnite(tour,unite_tour);
            jeu[posX][posY]=unite_tour;
        }
        free(nom_tour); // Libérer la mémoire allouée pour le nom de l'unité
    }

    fread(nbcase, sizeof(int),1,f_in);
    printf("Nombre de cases du parcours : %d\n", *nbcase);


    int** newchemin = (int**)malloc(sizeof(int*)*(*nbcase));
    for (int j=0;j<(*nbcase);j++){
        newchemin[j] = (int*)malloc(sizeof(int)*2);
    }
    for (int i = 0; i < (*nbcase); i++){
        int x = 0;
        fread(&x, sizeof(int),1,f_in);
        int y = 0;
        fread(&y, sizeof(int),1,f_in);
        printf("Case %d : x = %d, y = %d\n", i+1, x, y);
        newchemin[i][X] = x;
        newchemin[i][Y] = y;
    }

    printf("Le fichier sortie.txt a lu\n");
    fclose(f_in);

    return newchemin;
}

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

// Sauvegarde la partie dans un fichier sequentiel
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
        fprintf(f_out,"pos y :%d, ",tmp->pdata->posY);
        fprintf(f_out,"pv :%d; \n",tmp->pdata->pointsDeVie);
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

//Charge le fichier sequentiel qui a ete sauvegarde
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

        while(c != ':'){
            fscanf(f_in, "%c", &c);
        }
        int pv = 0;
        fscanf(f_in, "%d", &pv);
        printf("pv : %d;\n", pv);
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
            unite_tour->pointsDeVie=pv;
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