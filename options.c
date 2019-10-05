#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <dirent.h>

#include "options.h"
#include "constantes.h"
#include "fichier.h"
#include "changer_musique.h"
#include "supprimer_musique.h"
#include "commentJouer.h"

void menuOptions()
{
    int continuer = 1;

    SDL_Event event;
    afficherOptions();
    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_KP1:
                if(modeInfini(RIEN_FAIRE) == 1)//Si mode infini activé on le désactive
                    modeInfini(DESACTIVER);
                else
                    modeInfini(ACTIVER);//Sinon on l'active
                break;
            case SDLK_KP2:
                recommencerHistoire();
                continuer = 0;//Quitte automatiquement une fois que c'est fait
                break;
            case SDLK_KP3:
                menuChangerMusiques();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        afficherOptions();
        SDL_Flip(ecran);
    }
}

void afficherOptions()
{
    char modeInf[30] = {0};

    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeEnTete = NULL;

    police = TTF_OpenFont("data/Limousines.ttf", 30);
    policeEnTete = TTF_OpenFont("data/Limousines.ttf", 40);

    if(modeInfini(RIEN_FAIRE) == 1)
        sprintf(modeInf, "1. Desactiver le mode infini");
    else if(modeInfini(RIEN_FAIRE) == 0)
        sprintf(modeInf, "1. Activer le mode infini");

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    texte = TTF_RenderText_Shaded(policeEnTete, "Options", couleurBlanche, couleurNoire);//Affichage du menu
    position.x = ecran->w / 2 - texte->w / 2;
    position.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, modeInf, couleurBlanche, couleurNoire);
    position.x = 30;
    position.y = 200;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "2. Redémarrer l'histoire", couleurBlanche, couleurNoire);
    position.y += texte->h + 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "3. Changer les musiques", couleurBlanche, couleurNoire);
    position.y += texte->h + 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);//Fin de l'affichage

    SDL_FreeSurface(texte);
}

void menuChangerMusiques()
{
    int continuer = 1;

    SDL_Event event;
    afficherMenuChangerMusiques();
    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_KP1:
                control();
                break;
            case SDLK_KP2:
                controlSuppr();
                break;
            case SDLK_d:
                commentJouer(CHANGER_MUSIQUE);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        afficherMenuChangerMusiques();
        SDL_Flip(ecran);
    }
}

void afficherMenuChangerMusiques()
{

    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeEnTete = NULL, *petitePolice = NULL;

    police = TTF_OpenFont("data/Limousines.ttf", 30);
    policeEnTete = TTF_OpenFont("data/Limousines.ttf", 40);
    petitePolice = TTF_OpenFont("data/Limousines.ttf", 18);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    texte = TTF_RenderText_Shaded(policeEnTete, "Changer les musiques", couleurBlanche, couleurNoire);//Affichage du menu
    position.x = ecran->w / 2 - texte->w / 2;
    position.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "1. Selectionner des musiques", couleurBlanche, couleurNoire);
    position.x = 30;
    position.y = 200;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "2. Supprimer des musiques", couleurBlanche, couleurNoire);
    position.y += texte->h + 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(petitePolice, "Appuyez sur D pour le didacticiel", couleurBlanche, couleurNoire);
    position.x = ecran->w - texte->w - 10;
    position.y = ecran->h - texte->h - 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    SDL_FreeSurface(texte);

    TTF_CloseFont(police);
    TTF_CloseFont(policeEnTete);
    TTF_CloseFont(petitePolice);
}
