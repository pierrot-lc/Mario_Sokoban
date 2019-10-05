#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fmodex/fmod.h>
#include <time.h>
#include <SDL/SDL_ttf.h>

#include "main.h"
#include "selectionnerNiveau.h"
#include "constantes.h"
#include "jouer.h"
#include "editeur.h"
#include "commentJouer.h"
#include "options.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);//Initialisation de la SDL
    SDL_ShowCursor(SDL_DISABLE);

    FMOD_System_Create(&systemAudio);
    FMOD_System_Init(systemAudio, 32, FMOD_INIT_NORMAL, NULL);//Initialisation systemAudio FMOD

    SDL_WM_SetIcon(IMG_Load("Mario Sokoban.ico"), NULL);
    ecran = SDL_SetVideoMode(408, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL);//Creation de la fenetre

    lancement();
    menuPrincipal();


    TTF_Quit();//Quitte la TTF

    SDL_FreeSurface(ecran);
    SDL_Quit();//Quitte la SDL

    FMOD_System_Close(systemAudio);
    FMOD_System_Release(systemAudio);//Quitte FMOD

    return EXIT_SUCCESS;
}

void menuPrincipal()
{
    int continuer = 1;

    FMOD_SOUND *sound = NULL;
    FMOD_CHANNEL *canal;

    SDL_Event event; //Variables

    FMOD_System_GetChannel(systemAudio, 1, &canal);//Association avec le canal 1 du systemAudio
    FMOD_System_CreateSound(systemAudio, "data/Sounds/loading_screen.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound);
    FMOD_System_PlaySound(systemAudio, 1, sound, 0, NULL);//Lecture sur le canal 1
    FMOD_Sound_SetLoopCount(sound, -1);

    afficherMenuPrincipal();
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
                case SDLK_1://Clavier
                    menuNiveaux(canal, HISTOIRE, NULL);//Selection des niveaux normaux
                    break;
                case SDLK_2:
                    menuNiveaux(canal, EDITEUR, NULL);//Selection des niveaux edites
                    break;
                case SDLK_3:
                    break;
                case SDLK_ESCAPE:
                    continuer = 0;
                    break;
                case SDLK_KP1://Pad numerique
                    menuNiveaux(canal, HISTOIRE, NULL);//Menu Histoire
                    break;
                case SDLK_KP2:
                    menuNiveaux(canal, EDITEUR, NULL);//Menu Editeur
                    break;
                case SDLK_KP3:
                    menuOptions();
                    break;
                default :
                    break;
                }
            break;
        default:
            break;
        }
        afficherMenuPrincipal();
        SDL_Flip(ecran);
    }

    FMOD_Sound_Release(sound);
}

void afficherMenuPrincipal()//affichage du menu principal
{
    SDL_Surface *menu = NULL, *texte = NULL;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};
    SDL_Rect position;

    TTF_Font *police = NULL;//Variables

    position.x = 0;
    position.y = 0;

    menu = IMG_Load("data/Images/menu.jpg");
    police = TTF_OpenFont("data/Limousines.ttf", 40);//Initialisations


    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));//Debut de l'affichage
    SDL_BlitSurface(menu, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "1. Jouer", couleurBlanche, couleurNoire);
    position.x = 120;
    position.y = 160;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "2. Editeur", couleurBlanche, couleurNoire);
    position.y += texte->h + 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "3. Options", couleurBlanche, couleurNoire);
    position.y += texte->h + 10;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    TTF_CloseFont(police);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(texte);
}
void lancement()
{
    unsigned int lancements = 0;
    FILE* fichier = NULL;//Variables

    if(TTF_Init() == -1)//Initialisation de la TTF
    {
        fprintf(stderr, "Erreur lors de l'initialisation de la TTF : %s\n", TTF_GetError);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));//Initalise l'horloge pour la fonction random

    fichier = fopen("data/compteur.ms", "r");//Fichier contenant le nombre de lancements du programme depuis qu'il est installé
    if(fichier == NULL)
        fprintf(stderr, "Erreur lors de l'ouverture du fichier compteur");
    else
        fscanf(fichier, "%d", &lancements);//Lit le nombre de lancements

    if(lancements == NULL)
        lancements = 0;

    lancements++;//Augmente le nombre de lancements (1 de plus car on vient de lancer le programme !)
    if(fichier != NULL)
        fclose(fichier);

    fichier = fopen("data/compteur.ms", "w");//réouvre le fichier en effaçant tout
    if(fichier == NULL)
        fprintf(stderr, "Erreur lors de l'ouverture du fichier compteur");
    else
        fprintf(fichier, "%d", lancements);//Réécrit le nombre de lancements en actualisant

    if(fichier != NULL)
        fclose(fichier);
}
