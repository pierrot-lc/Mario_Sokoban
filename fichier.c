#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "selectionnerNiveau.h"
#include "fichier.h"
#include "constantes.h"

int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR], int numero, int type)
{
    FILE* fichier = NULL;
    char ligneFichier[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};
    int i = 0, j = 0, compteur = 0;
    char lettre = 0;

    if(type == HISTOIRE)//Ouvre le fichier correspondant au mode de jeu
        fichier = fopen("data/niveaux.lvl", "r");
    else if(type == EDITEUR)
        fichier = fopen("data/niveauxEdites.lvl", "r");

    if(fichier == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir les fichiers niveaux");
        return 0;
    }

    rewind(fichier);
    while(compteur != (numero - 1) )//Se deplace jusqu'au bon niveau
    {
        lettre = fgetc(fichier);
        if(lettre == '\n')//Fin d'un niveau
            compteur++;//Compte les niveaux
    }

    fgets(ligneFichier, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1, fichier);//Lit la ligne complete

    for(i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for(j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
        {
            switch (ligneFichier[(i * NB_BLOCS_LARGEUR) + j])//Place dans le niveau la bonne case
            {
                case '0':
                    niveau[j][i] = VIDE;
                    break;
                case '1':
                    niveau[j][i] = MUR;
                    break;
                case '2':
                    niveau[j][i] = CAISSE;
                    break;
                case '3':
                    niveau[j][i] = OBJECTIF;
                    break;
                case '4':
                    niveau[j][i] = MARIO;
                    break;
                case '5':
                    niveau[j][i] = CAISSE_OK;
                    break;
            }
        }
    }
    fclose(fichier);
    return 1;
}

int sauvegarderNiveau(int carte[][NB_BLOCS_HAUTEUR], int niveau)
{
    FILE* nivOriginaux = NULL, *nouvNiveaux = NULL;
    int i = 0, j = 0, compteur = 0, niveauMax = 0;
    char lettre = 0;

    niveauMax = niveauMaximal(EDITEUR);

    nivOriginaux = fopen("data/niveauxEdites.lvl", "r");
    nouvNiveaux = fopen("data/newNiveaux.lvl", "w+");//Fichier temporaire qui recopie et modifie les niveaux originaux
    if (nivOriginaux == NULL || nouvNiveaux == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir les fichiers de sauvegarde");
        return 0;
    }
    while(compteur < (niveau - 1) )//Copie les niveaux un par un jusqu'à arriver au niveau à modifier
    {
        lettre = fgetc(nivOriginaux);
        fputc(lettre, nouvNiveaux);

        if(lettre == '\n')
            compteur++;
    }

    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)//Copie le nouveau niveau dans un nouveau fichier
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
            fprintf(nouvNiveaux, "%d", carte[j][i]);
    }

    fputc('\n', nouvNiveaux);//Manque le retour à la ligne dans la carte
    compteur++;//On a écrit un niveau de plus
    lettre = 0;//Reinitialise la variable

    if(compteur < niveauMax)
    {
        while(lettre != '\n')//On se déplace d'un niveau dans le fichier d'origine(le niveau qui a été modifié)
        lettre = fgetc(nivOriginaux);
    }

    //Si on a ajouté un nouveau niveau, on devrait avoir dépassé compteur et niveauMax
    while(compteur < niveauMax)//On copie la fin du fichier
    {
        lettre = fgetc(nivOriginaux);
        fputc(lettre, nouvNiveaux);

        if(lettre == '\n')
            compteur++;
    }

    fclose(nivOriginaux);
    fclose(nouvNiveaux);

    remove("data/niveauxEdites.lvl");
    rename("data/newNiveaux.lvl", "data/niveauxEdites.lvl");//Remplace l'ancien fichier par le nouveau
    return 1;
}

void supprimerNiveau(int niveau)
{
    int compteur = 0, nivMax = 0;
    char lettre = 0;
    FILE *nivEdites = NULL, *nouvNiv = NULL;

    nivMax = niveauMaximal(EDITEUR);
    nivEdites = fopen("data/niveauxEdites.lvl", "r");
    nouvNiv = fopen("data/newNiveaux.lvl", "w+");//Fichier temporaire qui copie les niveaux edites

    if(nivEdites == NULL || nouvNiv == NULL)
    {
        fprintf(stderr, "Echec de l'ouverture des fichiers pour supprimer un niveau");
        return 0;
    }

    while(compteur < (niveau - 1))//Se deplace sur le fichier en le recopiant jusqu'au niveau à recopier
    {
        lettre = fgetc(nivEdites);
        fputc(lettre, nouvNiv);

        if(lettre == '\n')//Detecte la fin d'un niveau
            compteur++;//Compte les niveaux
    }

    lettre = 0;//Reinitialise la variable
    if(compteur < nivMax)
    {
        while(lettre != '\n')//Se deplace d'un niveau
            lettre = fgetc(nivEdites);

        compteur++;//Vient de passer un niveau (le niveau à ne pas recopier)
    }

    while(compteur < nivMax)
    {
        lettre = fgetc(nivEdites);
        fputc(lettre, nouvNiv);

        if(lettre == '\n')
            compteur++;
    }

    fclose(nouvNiv);
    fclose(nivEdites);

   remove("data/niveauxEdites.lvl");//Supprime les niveaux de l'editeur
   rename("data/newNiveaux.lvl", "data/niveauxEdites.lvl");//Remplace l'ancien fichier par le nouveau
}

void afficherJeu(int carte[][NB_BLOCS_HAUTEUR])
{
    int i = 0, j = 0;
    SDL_Surface *mur = NULL, *caisse = NULL, *objectif = NULL, *caisse_ok = NULL, *mario = NULL;
    SDL_Rect position;

    mur = IMG_Load("data/Images/mur.jpg");
    caisse = IMG_Load("data/Images/caisse.jpg");
    caisse_ok = IMG_Load("data/Images/caisse_ok.jpg");
    objectif = IMG_Load("data/Images/objectif.png");
    mario = IMG_Load("data/Images/mario_bas.gif");

    //Placement des objets à l'écran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
        {
            position.x = i * TAILLE_BLOC;
            position.y = j * TAILLE_BLOC;

            switch(carte[i][j])
            {
                case MUR:
                    SDL_BlitSurface(mur, NULL, ecran, &position);
                    break;
                case CAISSE:
                    SDL_BlitSurface(caisse, NULL, ecran, &position);
                    break;
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, ecran, &position);
                    break;
                case MARIO:
                    SDL_BlitSurface(mario, NULL, ecran, &position);
                    break;
                case CAISSE_OK:
                    SDL_BlitSurface(caisse_ok, NULL, ecran, &position);
                    break;
            }
        }
    }

    SDL_FreeSurface(caisse_ok);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(mario);
}

int modeInfini(int mode)
{
    char lettre;
    FILE *fichier = NULL;

    if(mode == ACTIVER)
    {
        fichier = fopen("data/infini.ms", "w+");
        fputc('1', fichier);
        fclose(fichier);
    }
    else if(mode == DESACTIVER)
    {
        fichier = fopen("data/infini.ms", "w+");
        fputc('0', fichier);
        fclose(fichier);
    }


    fichier = fopen("data/infini.ms", "r");

    lettre = fgetc(fichier);
    if(lettre == '1')
        return 1;//Mode infini activé

    return 0;
}

void recommencerHistoire()
{
    FILE* fichier = NULL;

    fichier = fopen("data/histoire.ms", "w+");

    fputc('1', fichier);

    fclose(fichier);
}
