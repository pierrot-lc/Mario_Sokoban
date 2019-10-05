#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fmodex/fmod.h>

#include "selectionnerNiveau.h"
#include "constantes.h"
#include "editeur.h"
#include "fichier.h"
#include "jouer.h"

void editeur(int niveau)
{
    int continuer = 1;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    EtatEditeur editeur = {MUR, 0, 0};//Creation d'une structure et initialisation
    SonsEditeur sons;
    Images images;

    SDL_Rect positionSouris;
    SDL_Event event;

    FMOD_SOUND *musique;
    FMOD_CHANNEL *canal;//Variables

    //Chargement des musiques
    FMOD_System_CreateSound(systemAudio, "data/Sounds/editeur.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &musique);
    chargerSon(&sons);//Charge les sons de la structure

    FMOD_System_GetChannel(systemAudio, 3, &canal);

    //Chargement des objets et du niveau
    chargerImages(&images);

    SDL_WarpMouse(ecran->w / 2, ecran->h / 2);//Place la souris au milieu de l'ecran
    positionSouris.x = ecran->w / 2;//Actualise la position de la souris
    positionSouris.y = ecran->h / 2;

    if (!chargerNiveau(carte, niveau, EDITEUR))
        exit(EXIT_FAILURE);

    FMOD_System_PlaySound(systemAudio, 3, musique, 0, NULL);
    FMOD_Sound_SetLoopCount(musique, -1);

    while (continuer)
    {
    SDL_WaitEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            exit(EXIT_SUCCESS);
            break;
        case SDL_MOUSEBUTTONDOWN:
            positionSouris.x = event.button.x;
            positionSouris.y = event.button.y;
            switch(event.button.button)
            {
            case SDL_BUTTON_LEFT:
                clicGauche(carte, &editeur, positionSouris, &sons);
                break;
            case SDL_BUTTON_RIGHT:
                editeur.clicDroitEnCours = 1;
                carte[positionSouris.x / TAILLE_BLOC][positionSouris.y / TAILLE_BLOC] = VIDE;
                break;
            case SDL_BUTTON_WHEELUP:
                if(editeur.objetActuel != 5)
                    editeur.objetActuel++;
                break;
            case SDL_BUTTON_WHEELDOWN:
                if(editeur.objetActuel != 1)
                    editeur.objetActuel--;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP://On désactive le booléen qui disait qu'un bouton était enfoncé
            if (event.button.button == SDL_BUTTON_LEFT)
                editeur.clicGaucheEnCours = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                editeur.clicDroitEnCours = 0;
            break;
        case SDL_MOUSEMOTION:
            positionSouris.x = event.motion.x;//Actualise las pos de la souris
            positionSouris.y = event.motion.y;

            if(editeur.clicGaucheEnCours)//Si on déplace la souris et que le bouton gauche de la souris est enfoncé
                clicGauche(carte, &editeur, positionSouris, &sons);
            else if(editeur.clicDroitEnCours)
                carte[positionSouris.x / TAILLE_BLOC][positionSouris.y / TAILLE_BLOC] = VIDE;//Vide la case
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    continuer = 0;
                    break;
                case SDLK_s:
                    menuNiveaux(NULL, EDITEUR_SAUVEGARDE, carte);
                    break;
                case SDLK_t://Sauvegarde puis test le jeu
                    sauvegarderNiveau(carte, niveau);
                    FMOD_Channel_SetPaused(canal, 1);
                    jouer(niveau, EDITEUR, 0);
                    FMOD_Channel_SetPaused(canal, 0);
                    chargerNiveau(carte, niveau, EDITEUR);
                    editeur.objetActuel = MUR;
                    break;
                case SDLK_c://Recharge le niveau à la derniere sauvegarde
                    chargerNiveau(carte, niveau, EDITEUR);
                    editeur.objetActuel = MUR;
                    break;
                case SDLK_KP1:
                    editeur.objetActuel = MUR;
                    break;
                case SDLK_KP2:
                    editeur.objetActuel = CAISSE;
                    break;
                case SDLK_KP3:
                    editeur.objetActuel = OBJECTIF;
                    break;
                case SDLK_KP4:
                    editeur.objetActuel = MARIO;
                    break;
                case SDLK_KP5:
                    editeur.objetActuel = CAISSE_OK;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    afficherJeu(carte);//Affiche la carte actuelle
    afficherSouris(&editeur, &images, positionSouris);//Colle la souris
    SDL_Flip(ecran);//Mise à jour de l'écran
    }

    FMOD_Channel_Stop(canal);

    FMOD_Sound_Release(musique);
    FMOD_Sound_Release(sons.box);
    FMOD_Sound_Release(sons.wall);
    FMOD_Sound_Release(sons.bell);
    FMOD_Sound_Release(sons.step);

    SDL_FreeSurface(images.caisse_ok);
    SDL_FreeSurface(images.mur);
    SDL_FreeSurface(images.caisse);
    SDL_FreeSurface(images.objectif);
    SDL_FreeSurface(images.mario);
}

void chargerSon(SonsEditeur *sons)
{
    FMOD_System_CreateSound(systemAudio, "data/Sounds/bell.wav", FMOD_CREATESAMPLE, 0, &sons->bell);
    FMOD_System_CreateSound(systemAudio, "data/Sounds/step.wav", FMOD_CREATESAMPLE, 0, &sons->step);
    FMOD_System_CreateSound(systemAudio, "data/Sounds/box.wav", FMOD_CREATESAMPLE, 0, &sons->box);
    FMOD_System_CreateSound(systemAudio, "data/Sounds/wall.wav", FMOD_CREATESAMPLE, 0, &sons->wall);
}

void chargerImages(Images *images)
{
    images->caisse = IMG_Load("data/Images/caisse.jpg");
    images->mur = IMG_Load("data/Images/mur.jpg");
    images->objectif = IMG_Load("data/Images/objectif.png");
    images->mario = IMG_Load("data/Images/mario_bas.gif");
    images->caisse_ok = IMG_Load("data/Images/caisse_ok.jpg");
}

void clicGauche(int carte[][NB_BLOCS_HAUTEUR], EtatEditeur *editeur, SDL_Rect positionSouris, SonsEditeur *sons)
{
    int i = 0, j = 0;

    editeur->clicGaucheEnCours = 1; //On active un booléen pour retenir qu'un bouton est enfoncé
    if(editeur->objetActuel != carte[positionSouris.x / TAILLE_BLOC][positionSouris.y / TAILLE_BLOC])//Pour ne pas jouer deux fois le meme son
    {
        switch(editeur->objetActuel)
        {
        case MARIO:
            FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, sons->step, 0, NULL);
        break;
        case OBJECTIF:
            FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, sons->bell, 0, NULL);
            break;
        case CAISSE:
            FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, sons->box, 0, NULL);
            break;
        case CAISSE_OK:
            FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, sons->box, 0, NULL);
            break;
        case MUR:
            FMOD_System_PlaySound(systemAudio, FMOD_CHANNEL_FREE, sons->wall, 0, NULL);
            break;
        }
    }
    if(editeur->objetActuel == MARIO)//Cherche un autre MARIO
    {
        for (i = 0; i < NB_BLOCS_HAUTEUR; i++)
        {
            for (j = 0; j < NB_BLOCS_LARGEUR; j++)
            {
                if(carte[i][j] == MARIO)
                    carte[i][j] = VIDE;//Remplace l'ancien MARIO par VIDE
            }
        }
    }

    //On met l'objet actuellement choisi (mur, caisse...) à l'endroit du clic
    carte[positionSouris.x / TAILLE_BLOC][positionSouris.y / TAILLE_BLOC] = editeur->objetActuel;
}

void afficherSouris(EtatEditeur *editeur, Images *images, SDL_Rect positionSouris)
{
    SDL_Rect positionObjet;

    switch(editeur->objetActuel)
    {
    case MUR:
        images->objetSouris = images->mur;
        break;
    case MARIO:
        images->objetSouris = images->mario;
        break;
    case CAISSE:
        images->objetSouris = images->caisse;
        break;
    case OBJECTIF:
        images->objetSouris = images->objectif;
        break;
    case CAISSE_OK:
        images->objetSouris = images->caisse_ok;
        break;
    }

    SDL_SetAlpha(images->objetSouris, SDL_SRCALPHA, 180);//Change la transparence de la souris

    positionObjet.x = positionSouris.x - TAILLE_BLOC/2;//Décale l'objet sur la souris pour le centrer
    positionObjet.y = positionSouris.y - TAILLE_BLOC/2;
    SDL_BlitSurface(images->objetSouris, NULL, ecran, &positionObjet);//Colle l'objet actuel de la souris
}
