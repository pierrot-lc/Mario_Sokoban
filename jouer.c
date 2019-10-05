#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fmodex/fmod.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <string.h>

#include "fichier.h"
#include "selectionnerNiveau.h"
#include "constantes.h"
#include "jouer.h"

int jouer(int niveau, int type, int rejouer)//Rejouer permet de savoir si il faut relancer la musique ou pas
{
    int continuer = 1, i = 0, j = 0, tempsActuel, tempsPrecedent, sonDeplacement = 0, niveauInfini = 0, recommencer = 0, objectifs;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};
    const int MIN = 1, MAX = 10;

    FMOD_SOUND *deplacement = NULL, *success = NULL, *bell = NULL, *background = NULL;
    FMOD_CHANNEL *canal;

    SDL_Surface *mario[4] = {NULL};
    SDL_Surface *marioActuel = NULL, *you_win = NULL;
    SDL_Rect position, positionJoueur, posWin;
    SDL_Event event;//Variables

    you_win = IMG_Load("data/Images/you_win.jpg");
    mario[BAS] = IMG_Load("data/Images/mario_bas.gif");
    mario[GAUCHE] = IMG_Load("data/Images/mario_gauche.gif");
    mario[HAUT] = IMG_Load("data/Images/mario_haut.gif");
    mario[DROITE] = IMG_Load("data/Images/mario_droite.gif");

    marioActuel = mario[BAS];

    posWin.x = 0;
    posWin.y = 0;

    if(!chargerNiveau(carte, niveau, type))//Chargement du niveau
        exit(EXIT_FAILURE);

    for(i = 0; i < NB_BLOCS_LARGEUR; i++)//Remplace la case MARIO par une case VIDE
    {
        for(j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            if(carte[i][j] == MARIO)
            {
                positionJoueur.x = i;//Sauvegarde la position de MARIO
                positionJoueur.y = j;
                carte[i][j] = VIDE;
            }

        }
    }

    if(modeInfini(RIEN_FAIRE) == 1)//Se lance uniquement si le mode est actif
        niveauInfini = (rand() % (MAX - MIN + 1)) + MIN;//1 = niveau infini

    FMOD_System_CreateSound(systemAudio, "data/Sounds/bell.wav", FMOD_CREATESAMPLE, 0, &bell);
    FMOD_System_CreateSound(systemAudio, "data/Sounds/step.wav", FMOD_CREATESAMPLE, 0, &deplacement);
    if(niveau == niveauMaximal(type))//Choix de la musique de fin
        FMOD_System_CreateSound(systemAudio, "data/Sounds/won_a_coffee.wav", FMOD_CREATESAMPLE, 0, &success);
    else
        FMOD_System_CreateSound(systemAudio, "data/Sounds/success.wav", FMOD_CREATESAMPLE, 0, &success);

    if(rejouer == 0)//Dans le cas où ce n'est pas un nouvel essaie (donc que il n'y a pas une musique de lancé)
        selectionMusique(background);

    FMOD_System_GetChannel(systemAudio, 2, &canal);//Association du canal numero 2 du system à la variable

    SDL_EnableKeyRepeat(100, 100);

    //Initialisations finies
    while(continuer)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
        case SDL_QUIT:
            if(niveauInfini != 1)
                exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                if(niveauInfini != 1)
                    continuer = 0;
                break;
            case SDLK_UP:
                marioActuel = mario[HAUT];
                sonDeplacement = deplacerJoueur(carte, &positionJoueur, HAUT);//Deplace le joueur et renvoi le son à jouer

                switch(sonDeplacement)
                {
                case STEP:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, deplacement, 0, NULL);
                    break;
                case BELL:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, bell, 0, NULL);
                    break;
                }

                break;
            case SDLK_DOWN:
                marioActuel = mario[BAS];
                sonDeplacement = deplacerJoueur(carte, &positionJoueur, BAS);

                switch(sonDeplacement)
                {
                case STEP:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, deplacement, 0, NULL);
                    break;
                case BELL:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, bell, 0, NULL);
                    break;
                }

                break;
            case SDLK_RIGHT:
                marioActuel = mario[DROITE];
                sonDeplacement = deplacerJoueur(carte, &positionJoueur, DROITE);

                switch(sonDeplacement)
                {
                case STEP:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, deplacement, 0, NULL);
                    break;
                case BELL:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, bell, 0, NULL);
                    break;
                }

                break;
            case SDLK_LEFT:
                marioActuel = mario[GAUCHE];
                sonDeplacement = deplacerJoueur(carte, &positionJoueur, GAUCHE);

                switch(sonDeplacement)
                {
                case STEP:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, deplacement, 0, NULL);
                    break;
                case BELL:
                    FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, bell, 0, NULL);
                    break;
                }
                break;
            case SDLK_r:
                if(niveauInfini != 1)
                {
                    recommencer = 1;
                    continuer = 0;
                }
                break;
            case SDLK_a:
                continuer = 0;
                break;
            case SDLK_SPACE:
                FMOD_Channel_Stop(canal);
                selectionMusique(background);
            default:
                break;
            }
            break;
        default:
            break;
        }

        afficherJeu(carte);
        position.x = positionJoueur.x * TAILLE_BLOC;
        position.y = positionJoueur.y * TAILLE_BLOC;
        SDL_BlitSurface(marioActuel, NULL, ecran, &position);
        SDL_Flip(ecran);//Actualise l'ecran

        objectifs = finNiveau(carte);//Renvoie le nombre d'objectif restants
        if (objectifs == 0)
        {
            continuer = 0;
            niveauInfini = 0;
        }
    }

    if(recommencer == 0 || type == EDITEUR)//Si l'utilisateur n'a pas voulu recommencer la partie
        FMOD_Channel_Stop(canal);//Stop la musique de fond

    if(objectifs == 0)
    {
        tempsPrecedent = SDL_GetTicks();//Releve le temps
        FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, success, 0, NULL);//Affiche le message de fin de niveau

        continuer = 1;
        if(niveau == niveauMaximal(type))//Fin lorsque vous avez fini le dernier niveau du tyoe
        {
            SDL_BlitSurface(you_win, NULL, ecran, &posWin);
            SDL_Flip(ecran);

            while(continuer)
            {
                tempsActuel = SDL_GetTicks();//Actualise le temps
                if(tempsActuel - tempsPrecedent >= 2000)//Si il y a au moins 2000ms d'ecart avec le temps
                    continuer = 0;
            }
        }
        else
        {
            while(continuer)//Fin du niveau normal
            {
                tempsActuel = SDL_GetTicks();
                if(tempsActuel - tempsPrecedent >= 1000)
                    continuer = 0;
            }
        }
    }

    SDL_FreeSurface(you_win);

    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(mario[i]);
    if(background != NULL && recommencer != 1)
        FMOD_Sound_Release(background);
    FMOD_Sound_Release(deplacement);
    FMOD_Sound_Release(bell);
    FMOD_Sound_Release(success);

    if(objectifs == 0)
        return NIVEAU_TERMINE;//Le joueur a reussi le niveau
    else if(recommencer)
        return RECOMMENCER;//Le joueur veut recommencer le niveau

    return QUITTER;//Sinon on quitte
}

int deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction)
{
    int deplacement = RIEN;//Son à jouer
    switch(direction)
    {
        case HAUT:
            if (pos->y - 1 < 0) // Si le joueur dépasse l'écran, on arrête
                {
                    return deplacement;
                    break;
                }
            if (carte[pos->x][pos->y - 1] == MUR) // S'il y a un mur, on arrête
                {
                    return deplacement;
                    break;
                }
            // Si on veut pousser une caisse, il faut vérifier qu'il n'y a pas de mur derrière (ou une autre caisse, ou la limite du monde)
            if ((carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y - 1] == CAISSE_OK) &&
                (pos->y - 2 < 0 || carte[pos->x][pos->y - 2] == MUR ||
                carte[pos->x][pos->y - 2] == CAISSE || carte[pos->x][pos->y - 2] == CAISSE_OK))
                {
                    return deplacement;
                    break;
                }

            // Si on arrive là, c'est qu'on peut déplacer le joueur !
            // On vérifie d'abord s'il y a une caisse à déplacer

            deplacement = STEP;
            deplacerCaisse(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2], &deplacement);

            pos->y--; // On peut enfin faire monter le joueur (oufff !)
            return deplacement;
            break;


        case BAS:
            if (pos->y + 1 >= NB_BLOCS_HAUTEUR)
                {
                    return deplacement;
                    break;
                }
            if (carte[pos->x][pos->y + 1] == MUR)
                {
                    return deplacement;
                    break;
                }

            if ((carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK) &&
                (pos->y + 2 >= NB_BLOCS_HAUTEUR || carte[pos->x][pos->y + 2] == MUR ||
                carte[pos->x][pos->y + 2] == CAISSE || carte[pos->x][pos->y + 2] == CAISSE_OK))
                {
                    return deplacement;
                    break;
                }

            deplacement = STEP;
            deplacerCaisse(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2], &deplacement);

            pos->y++;
            return deplacement;
            break;


        case GAUCHE:
            if (pos->x - 1 < 0)
                {
                    return deplacement;
                    break;
                }
            if (carte[pos->x - 1][pos->y] == MUR)
                {
                    return deplacement;
                    break;
                }

            if ((carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK) &&
                (pos->x - 2 < 0 || carte[pos->x - 2][pos->y] == MUR ||
                carte[pos->x - 2][pos->y] == CAISSE || carte[pos->x - 2][pos->y] == CAISSE_OK))
                {
                    return deplacement;
                    break;
                }

            deplacement = STEP;
            deplacerCaisse(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y], &deplacement);

            pos->x--;
            return deplacement;
            break;


        case DROITE:
            if (pos->x + 1 >= NB_BLOCS_LARGEUR)
                {
                    return deplacement;
                    break;
                }
            if (carte[pos->x + 1][pos->y] == MUR)
                {
                    return deplacement;
                    break;
                }

            if ((carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK) &&
                (pos->x + 2 >= NB_BLOCS_LARGEUR || carte[pos->x + 2][pos->y] == MUR ||
                carte[pos->x + 2][pos->y] == CAISSE || carte[pos->x + 2][pos->y] == CAISSE_OK))
                {
                    return deplacement;
                    break;
                }

            deplacement = STEP;
            deplacerCaisse(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y], &deplacement);

            pos->x++;
            return deplacement;
            break;
    }
    return 1;
}

void deplacerCaisse(int *premiereCase, int *secondeCase, int *deplacement)
{
    if (*premiereCase == CAISSE || *premiereCase == CAISSE_OK)
    {
        if (*secondeCase == OBJECTIF)
            {
                *secondeCase = CAISSE_OK;
                *deplacement = BELL;
            }
        else
            *secondeCase = CAISSE;

        if (*premiereCase == CAISSE_OK)
            {
                *premiereCase = OBJECTIF;
                *deplacement = STEP;
            }
        else
            *premiereCase = VIDE;
    }
}

int finNiveau(int carte[][NB_BLOCS_HAUTEUR])
{
    int objetctifs = 0, i = 0, j = 0;

    for(i = 0; i < NB_BLOCS_HAUTEUR; i++)
    {
        for(j = 0; j < NB_BLOCS_LARGEUR; j++)
        {
            if(carte[i][j] == OBJECTIF)
                objetctifs++;
        }
    }

    return objetctifs;
}

void selectionMusique(FMOD_SOUND *sound)
{
    int numeroChanson = 0, max = 0, min = 0, i = 0;
    char cheminMusique[1000] = {0};

    FILE *fichier = NULL;

    fichier = fopen("data/liste_musiques.llv", "r");
    if(fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier 'liste_musique.llv'");
        return;
    }

    while(fgets(cheminMusique, 1000, fichier))
        max++;

    numeroChanson = (rand() % (max - min + 1)) + min;
    rewind(fichier);
    for(i = 0; i < numeroChanson; i++)
        fgets(cheminMusique, 1000, fichier);

    i = strlen(cheminMusique);
    cheminMusique[i - 1] = '\0';

    FMOD_System_CreateSound(systemAudio, cheminMusique, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound);
    FMOD_System_PlaySound(systemAudio, 2, sound, 0, NULL);//Lecture sur le canal 2
    FMOD_Sound_SetLoopCount(sound, -1);//Loop infini

    fclose(fichier);
}
