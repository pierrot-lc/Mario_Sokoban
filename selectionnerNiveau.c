#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fmodex/fmod.h>

#include "main.h"
#include "selectionnerNiveau.h"
#include "fichier.h"
#include "constantes.h"
#include "editeur.h"
#include "jouer.h"
#include "commentJouer.h"

void menuNiveaux(FMOD_CHANNEL *canal, int type, int carte[][NB_BLOCS_HAUTEUR])//Carte pour le mode sauvegarde
{
    int continuer = 1, niveau = 1, nivMax = 0, retour = 0;

    SDL_Event event;

    nivMax = niveauMaximal(type);

    SDL_EnableKeyRepeat(100, 100);

    afficherNiveaux(niveau, type);
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
            case SDLK_DOWN:
                deplacement(&niveau, BAS, type);
                break;
            case SDLK_UP:
                deplacement(&niveau, HAUT, type);
                break;
            case SDLK_LEFT:
                deplacement(&niveau, GAUCHE, type);
                break;
            case SDLK_RIGHT:
                deplacement(&niveau, DROITE, type);
                break;
            case SDLK_RETURN:
                retour = menuEntree(type, niveau, canal, carte);//Fonctions qui se passent selon le type
                if(retour == QUITTER)
                    continuer = 0;
                break;
            case SDLK_e:
                if(type == EDITEUR){//En mode editeur uniquement
                FMOD_Channel_SetPaused(canal, 1);//Pause
                editeur(niveau);
                FMOD_Channel_SetPaused(canal, 0);//Play

                nivMax = niveauMaximal(type);//Le niveau max a peut etre changé lors de l'edition du niveau
                if(niveau > nivMax)//Si on a supprimer beaucoup de niveau ça peut arriver
                    niveau = nivMax;
                }
                break;
            case SDLK_DELETE:
                if(type == EDITEUR_SAUVEGARDE && niveau != nivMax && nivMax != 2)//Pas activable si on est sur la derniere case (Nouv niv) et si il ne reste plus que celle la et 1 autre niveau
                {
                    supprimerNiveau(niveau);
                    nivMax = niveauMaximal(type);
                }
                break;
            case SDLK_d://Dicdacticiel correspondant au mode actuel
                if(type == HISTOIRE || type == EDITEUR)
                    commentJouer(type);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        afficherNiveaux(niveau, type);//Actualise l'écran après avoir fini d'édité
        SDL_Flip(ecran);
    }
}

void afficherNiveaux(int niveau, int type)
{
    int moitieNiveau = 0, numeroBase = 0, continuer = 1, numeroNiveauMax = 0, nivDebloques = 0, numeroFleche = 0;
    char nomNiveau[20] = "";

    SDL_Surface *texte = NULL, *fleche = NULL, *cadenas_block = NULL, *cadenas_deblock = NULL;
    SDL_Rect positionTexte, positionFleche, positionCadenas;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};

    TTF_Font *police = NULL, *policeGrande = NULL, *policePetite = NULL;//Variables

    numeroNiveauMax = niveauMaximal(type);
    nivDebloques = niveauxDebloques();

    police = TTF_OpenFont("data/Limousines.ttf", 30);
    policeGrande = TTF_OpenFont("data/Limousines.ttf", 40);
    policePetite = TTF_OpenFont("data/Limousines.ttf", 20);

    numeroFleche = niveau%10;//Garde que les unites du niveau
    if(numeroFleche == 0)//Le 0 veut dire 10 (la fleche varie entre 1 et 9)
        numeroFleche = 10;

    numeroBase = niveau;

    do
    {
        numeroBase--;
    }
    while( (numeroBase%10) != 0);//Pour afficher des tranches de 10
    numeroBase++;//Commence par 1

    moitieNiveau = numeroBase + 5;//Pour la séparation des colonnes (nombre de 1 à 6 = premiere colonne)

    fleche = IMG_Load("data/Images/fleche.png");
    cadenas_block = IMG_Load("data/Images/cadenas_block.png");
    cadenas_deblock = IMG_Load("data/Images/cadenas_deblock.png");
    texte = TTF_RenderText_Shaded(policeGrande, "Selection des niveaux", couleurBlanche, couleurNoire);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));//Debut de l'affichage

    positionTexte.x = ecran->w/2 - texte->w/2;//Milieu sur l'axe x
    positionTexte.y = 30;
    SDL_BlitSurface(texte, NULL, ecran, &positionTexte);

    if(type == EDITEUR || type == HISTOIRE)
    {
        texte = TTF_RenderText_Shaded(policePetite, "Appuyez sur D pour le didacticiel", couleurBlanche, couleurNoire);
        positionTexte.x = ecran->w - texte->w - 10;
        positionTexte.y = ecran->h - texte->h - 10;
        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
    }

    positionTexte.x = 55;
    positionTexte.y = 170;//positions de base adéquates pour l'écriture des niveaux

    positionFleche.x = positionTexte.x - fleche->w - 2;
    positionFleche.y = positionTexte.y + 5;

    if(numeroFleche < 6)//Colonne 1
        positionFleche.y += 35 * (numeroFleche - 1);//Pour ne pas additioner avec la pos 1
    else//Colonne 2
    {
        positionFleche.y += 35 * (numeroFleche - 6);//Pour compter uniquement le decalage sur x
        positionFleche.x += 197;//Position sur la deuxieme colonne
    }
    SDL_BlitSurface(fleche, NULL, ecran, &positionFleche);

    while(continuer)
    {
        sprintf(nomNiveau, "Niveau %d", numeroBase);//Ecriture dans la chaine le numero du niveau

        texte = TTF_RenderText_Shaded(police, nomNiveau, couleurBlanche, couleurNoire);//Colle la chaine dans le texte
        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);

        if(type == HISTOIRE){
        positionCadenas.y = positionTexte.y;
        positionCadenas.x = positionTexte.x + 120;//Decalage par rapport au texte
        if(numeroBase <= nivDebloques)
            SDL_BlitSurface(cadenas_deblock, NULL, ecran, &positionCadenas);
        else if(numeroBase > nivDebloques)
            SDL_BlitSurface(cadenas_block, NULL, ecran, &positionCadenas);
        }//Mode histoire on rajoute l'affichage du cadenas

        if(type == EDITEUR_SAUVEGARDE && numeroBase == numeroNiveauMax)//Dernier niveau en mode sauvegarde on le remplace par un nouveau niveau
        {
            sprintf(nomNiveau, "Nouv niv %d", numeroBase);
            texte = TTF_RenderText_Shaded(police, nomNiveau, couleurBlanche, couleurNoire);
            SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
        }

        numeroBase++;//Passe au niveau suivant
        positionTexte.y = positionTexte.y + 35;//Descend l'affichage

        if(numeroBase == moitieNiveau)//Deuxieme colonne ?
        {
            positionTexte.x = positionTexte.x + 197;//On décale à droite
            positionTexte.y = 170;//On repart d'en haut
        }

        //Comparaisons effectues après l'incrementation de numeroBase
        if(numeroBase == 11)//Si on a fait les 10 niveaux
            continuer = 0;
        if(numeroBase > numeroNiveauMax)//Si on a depassé le niveau maximal
            continuer = 0;
    }

    TTF_CloseFont(police);
    TTF_CloseFont(policeGrande);
    TTF_CloseFont(policePetite);

    SDL_FreeSurface(texte);
    SDL_FreeSurface(fleche);
    SDL_FreeSurface(cadenas_block);
    SDL_FreeSurface(cadenas_deblock);
}

void deplacement(int *niveau, int direction, int type)
{
    int niveauMax = niveauMaximal(type);
    if(direction == HAUT)
        *niveau = *niveau - 1;
    else if(direction == BAS)
        *niveau = *niveau + 1;
    else if(direction == DROITE)
        *niveau += 5;
    else if(direction == GAUCHE)
        *niveau -= 5;

    if(*niveau > niveauMax)//Si on a dépassé le niveau maximimal on se place dessus
        *niveau = niveauMax;
    else if(*niveau < 1)//Si on a dépassé le niveau minimal on se place dessus
        *niveau = 1;
}

void partie(int niveau)
{
    int retour = 0, nivDebloques = 0, rejouer = 0;

    FILE *fichier = NULL;

    nivDebloques = niveauxDebloques();
    do
    {
        retour = jouer(niveau, HISTOIRE, rejouer);//Lance et relance le niveau tant que l'utilisateur ne veux pas quitter
        if(retour == NIVEAU_TERMINE)
        {
            rejouer = 0;
            if(nivDebloques < (niveau + 1))//Si on a depassé le niveau maximal debloqué on actualise !
            {
                nivDebloques = niveau + 1;
                if(fichier != NULL)
                    fclose(fichier);
                fichier = fopen("data/histoire.ms", "w+");//Reouvre le fichier et l'effaçant
                fprintf(fichier, "%d", nivDebloques);
            }
            if(niveau != niveauMaximal(HISTOIRE))
                niveau++;//Augmente le niveau qui est relancé automatiquement ensuite
            else
                retour = QUITTER;
        }
        else if(retour == RECOMMENCER)
            rejouer = 1;
    }
    while(retour != QUITTER);

    if(fichier != NULL)
        fclose(fichier);
}

void partieEditeur(int niveau)
{
    int retour = 0, rejouer = 0;

    do
    {
        retour = jouer(niveau, EDITEUR, rejouer);
        if(retour == NIVEAU_TERMINE)
        {
            rejouer = 0;
            if(niveau != niveauMaximal(EDITEUR))
                niveau++;
            else
                retour = QUITTER;
        }
        else if(retour == RECOMMENCER)
            rejouer = 1;
    }
    while(retour != QUITTER);
}

int niveauMaximal(int type)
{
    int nombreNiveaux = 0;
    char lettre = 0;
    FILE *fichier = NULL;

    if(type == HISTOIRE)
        fichier = fopen("data/niveaux.lvl", "r");
    else if(type == EDITEUR || type == EDITEUR_SAUVEGARDE)
        fichier = fopen("data/niveauxEdites.lvl", "r");

    while(lettre != EOF)
    {
        lettre = getc(fichier);
        if(lettre == '\n')
            nombreNiveaux++;//un '\n' est une fin de niveau
    }

    if(type == EDITEUR_SAUVEGARDE)//Pour laisser le choix "Creer un nouveau niveau" dans le menu sauvegarde
        nombreNiveaux++;

    fclose(fichier);
    return nombreNiveaux;
}

int niveauxDebloques()
{
    int niveau = 0;
    FILE* histoire = NULL;

    histoire = fopen("data/histoire.ms", "r");
    fscanf(histoire, "%d", &niveau);
    if(niveau == 0)
        niveau = 1;
    fclose(histoire);

    return niveau;
}

int menuEntree(int type, int niveau, FMOD_CHANNEL *canal, int carte[][NB_BLOCS_HAUTEUR])
{
    int nivDebloques = niveauxDebloques();

    if(type == HISTOIRE){
        if(niveau <= nivDebloques)//Si on est bien dans des niveaux débloqués
        {
            FMOD_Channel_SetPaused(canal, 1);//Pause à la musique du menu
            partie(niveau);//Lance la partie
            FMOD_Channel_SetPaused(canal, 0);//Play
        }
    }//Mode Histoire
    else if(type == EDITEUR){
        FMOD_Channel_SetPaused(canal, 1);//Pause
        partieEditeur(niveau);//Lance la partie
        FMOD_Channel_SetPaused(canal, 0);//Play
    }//Mode Editeur
    else if(type == EDITEUR_SAUVEGARDE){
        sauvegarderNiveau(carte, niveau);//Sauvegarde ou ecrase le niveau selectionner
        return QUITTER;
    }

    return RECOMMENCER;
}
