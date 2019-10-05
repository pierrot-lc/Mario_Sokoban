#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "constantes.h"
#include "commentJouer.h"

void commentJouer(int type)
{
    int continuer = 1;

    SDL_Event event;

    if(type == HISTOIRE)
        aideJeu();
    else if(type == EDITEUR)
        aideEditeur();
    else if(type == CHANGER_MUSIQUE)
        aideChangerMusique();

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
            continuer = 0;
            break;
        default:
            break;
        }
    }
}

void aideJeu()
{
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeMenu = NULL;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    police = TTF_OpenFont("data/Limousines.ttf", 20);
    policeMenu = TTF_OpenFont("data/Limousines.ttf", 40);

    texte = TTF_RenderText_Shaded(policeMenu, "Didacticiel de jeu", couleurBlanche, couleurNoire);
    position.x = ecran->w/2 - texte->w/2;
    position.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Pour gagner, Mario doit pousser les caisses sur les", couleurBlanche, couleurNoire);
    position.x = 10;
    position.y = 100;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "objectifs. Mario se d�place gr�ce aux fl�ches", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "directionnelles. Les niveaux se d�bloquent au fur et", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "� mesure que vous finissez les niveaux pr�c�dents.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "La touche R permet de recommencer un niveau. Le", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    position.y += texte->h + 5;//Saut de ligne
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "mode infini (activable dans les options) emp�che", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "la fermeture et un nouvel essai du niveau actuel.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Ce mode une fois activ� n'a qu'une chance sur dix", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "de se lancer. Pour changer de musique, appuyez", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "sur espace.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    TTF_CloseFont(police);
    TTF_CloseFont(policeMenu);

    SDL_FreeSurface(texte);
}

void aideEditeur()
{
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeMenu = NULL;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    police = TTF_OpenFont("data/Limousines.ttf", 20);
    policeMenu = TTF_OpenFont("data/Limousines.ttf", 40);

    texte = TTF_RenderText_Shaded(policeMenu, "Dictaticiel �diteur", couleurBlanche, couleurNoire);
    position.x = ecran->w/2 - texte->w/2;
    position.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "L'�diteur de niveau vous permet de cr�er pleins de", couleurBlanche, couleurNoire);
    position.x = 10;
    position.y = 95;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "niveaux. Le clic gauche place l'objet selectionn�", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "sur la case et le clic droit permet de supprimer. ", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Pour changer d'objet, vous pouvez utiliser les", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "touches 1 � 5 du clavier ou bien la molette de la", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "souris.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "La touche S permet de sauvegarder et SUPPR de", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;//Saut de ligne
    position.y += texte->h;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "supprimer. La touche C redessine le niveau comme", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "il �tait � la pr�c�dente sauvegarde. Vous pouvez", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "tester le niveau en appuyant sur T, une sauvegarde", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "est effectu�e automatiquement.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    TTF_CloseFont(police);
    TTF_CloseFont(policeMenu);

    SDL_FreeSurface(texte);
}

void aideChangerMusique()
{
    SDL_Surface *texte = NULL;
    SDL_Rect position;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeMenu = NULL;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    police = TTF_OpenFont("data/Limousines.ttf", 20);
    policeMenu = TTF_OpenFont("data/Limousines.ttf", 40);

    texte = TTF_RenderText_Shaded(policeMenu, "Dictaticiel musiques", couleurBlanche, couleurNoire);
    position.x = ecran->w/2 - texte->w/2;
    position.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Vous pouvez changer les musiques lues lors des", couleurBlanche, couleurNoire);
    position.x = 10;
    position.y = 95;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "niveaux (qui seront selectionn�es al�atoirement).", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Dans le menu des selections de nouvelle musiques,", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "appuyez sur entr�e pour se d�placer dans les", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "dossiers ou ajouter une musique dans la liste. La", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "touche retour permet de reculer d'un dossier et la", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "touche espace de sauvegarder le chemin de base", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "lors des prochains ajouts. De plus en appuyant sur", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "une lettre vous vous d�placez jusqu'� celle ci.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, " ", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Pour supprimer les musiques dans le menu ad�quat,", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "appuyez sur entr�e.", couleurBlanche, couleurNoire);
    position.y += texte->h + 5;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    TTF_CloseFont(police);
    TTF_CloseFont(policeMenu);

    SDL_FreeSurface(texte);
}
