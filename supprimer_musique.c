#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <string.h>

#include "changer_musique.h"
#include "constantes.h"
#include "options.h"
#include "supprimer_musique.h"

void controlSuppr()
{
    int continuer = 1, position = 0, limite = 0, positionFleche = 0;

    SDL_Event event;//Variables

    limite = nombreChansons() - 1;//positionFleche commence à 0 et non à 1 donc on soustrait 1 à limite

    SDL_EnableKeyRepeat(100, 100);

    afficherMusiquesEnregistrees(position, positionFleche);
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
            case SDLK_UP:
                if(positionFleche > 0)
                    positionFleche--;
                else if(position > 0)
                    position--;
                break;
            case SDLK_DOWN:
                if(positionFleche < 12 && (positionFleche + position < limite) )
                    positionFleche++;
                else if(positionFleche + position < limite)
                    position++;
                break;
            case SDLK_RETURN:
                if(limite > 0){
                supprimerMusique(position + positionFleche);
                limite = nombreChansons() - 1;
                if(position + positionFleche > limite)
                    positionFleche--;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        afficherMusiquesEnregistrees(position, positionFleche);
        SDL_Flip(ecran);
    }
}

void afficherMusiquesEnregistrees(int position, int positionFleche)
{
    char lettre = 0;
    char chemin[1000] = {0};
    int i = 0;

    SDL_Surface *texte = NULL, *fleche = NULL, *musique = NULL;
    SDL_Rect positionTexte, posFleche;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};
    TTF_Font *police;

    FILE *fichier = NULL;

    fleche = IMG_Load("data/Images/fleche.png");
    musique = IMG_Load("data/Images/mp3.png");

    fichier = fopen("data/liste_musiques.llv", "r");
    if(fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'affichage des musiques enregistrées.\n");
        exit(EXIT_FAILURE);
    }

    while(i < position){
        lettre = fgetc(fichier);
        if(lettre == '\n')
            i++;
    }

    police = TTF_OpenFont("data/Limousines.ttf", 20);

    posFleche.x = 5;
    positionTexte.x = fleche->w + 10;
    positionTexte.y = 0;

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    while(fgets(chemin, 1000, fichier))
    {
        i = strlen(chemin);
        chemin[i - 1] = '\0';

        prendreNom(chemin);
        texte = TTF_RenderText_Shaded(police, chemin, couleurBlanche, couleurNoire);//Attribution de ent->d_name avec TTF

        SDL_BlitSurface(musique, NULL, ecran, &positionTexte);
        positionTexte.x = positionTexte.x + 25;
        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);//Puis on colle le texte


        positionTexte.y += texte->h + 8;//On décale la position.y puis on recommence
        positionTexte.x = fleche->w + 10;
    }
    posFleche.y = positionFleche * (texte->h + 8);
    SDL_BlitSurface(fleche, NULL, ecran, &posFleche);

    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(musique);
    SDL_FreeSurface(fleche);
    fclose(fichier);
}

int nombreChansons()
{
    char temp[1000] = {0};
    int compteur = 0;

    FILE *fichier = NULL;

    fichier = fopen("data/liste_musiques.llv", "r");
    if(fichier == NULL)
    {
        fprintf(stderr,"Erreur lors de l'ouverture du fichier 'liste_musiques.llv'");
        return 0;
    }

    while(fgets(temp, 1000, fichier))
        compteur++;

    fclose(fichier);
    return compteur;
}

void supprimerMusique(int numeroMusique)
{
    char lettre = 0;
    int i = 0, maxChansons = nombreChansons();

    FILE *fichier = NULL, *temp = NULL;
    fichier = fopen("data/liste_musiques.llv", "r");
    temp = fopen("data/liste_temp.llv", "w+");
    if(fichier == NULL || temp == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture 'liste_musiques.llv'.\n");
        return;
    }

    while(i < numeroMusique)
    {
        lettre = fgetc(fichier);
        fputc(lettre, temp);
        if(lettre == '\n')
            i++;
    }

    lettre = 0;
    while(lettre != '\n')
        lettre = fgetc(fichier);
    i++;


    while(i != maxChansons)
    {
        lettre = fgetc(fichier);
        fputc(lettre, temp);

        if(lettre == '\n')
            i++;
    }

    fclose(fichier);
    fclose(temp);

    remove("data/liste_musiques.llv");
    rename("data/liste_temp.llv", "data/liste_musiques.llv");
}

void prendreNom(char *chemin)
{
    char temp[1000] = {0};
    int i = 0, j = 0;

    strcpy(temp, chemin);

    for(i = strlen(temp); i >= 0; i--)
        chemin[i] = 0;

    i = strlen(temp);
    while(temp[i] != '/')
        i--;

    i++;
    while(i != strlen(temp))
    {
        chemin[j] = temp[i];
        j++;
        i++;
    }
}
