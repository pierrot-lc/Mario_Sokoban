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

void control()
{
    int continuer = 1, position = 0, limite = 0, positionFleche = 0, retour = 0;
    char chemin[1000] = {0};

    struct dirent *ent;
    DIR* dir = NULL;

    ImagesMusiques images;

    SDL_Event event;//Variables

    cheminBase(chemin);
    initialiserStructure(&images);//Initialisations images/police
    dir = opendir(chemin);
    if(dir == NULL)
        return;

    ent = readdir(dir);
    ent = readdir(dir);

    limite = limiteDossier(dir) - 1;//positionFleche commence à 0 et non à 1 donc on soustrait 1 à limite

    SDL_EnableKeyRepeat(100, 100);

    afficherMusiques(dir, position, images, chemin, positionFleche);
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
                if(positionFleche < MAXFLECHE && (positionFleche + position < limite) )
                    positionFleche++;
                else if(positionFleche + position < limite)
                    position++;
                break;
            case SDLK_RETURN:
                seekdir(dir, position + positionFleche + 2);
                ent = readdir(dir);
                retour = entree(chemin, ent->d_name);
                if(retour == DOSSIER)
                {
                    closedir(dir);
                    dir = opendir(chemin);
                    limite = limiteDossier(dir) - 1;
                    position = 0;
                    positionFleche = 0;
                }
                else if(retour == MP3)
                    ajouterMusique(chemin, ent->d_name);
                break;
            case SDLK_BACKSPACE:
                dossierPrecedent(chemin);
                closedir(dir);
                dir = opendir(chemin);
                ent = readdir(dir);
                ent = readdir(dir);
                position = 0;
                positionFleche = 0;
                limite = limiteDossier(dir) - 1;
                break;
            case SDLK_SPACE:
                changerCheminBase(chemin);
                break;
            default:
                position = isLetter(ecran, &event, dir);//Si une autre touche est appuyée on vérifie si c'est une lettre et si la lettre est trouvée on déplace jusuq'à la lettre
                if(!position)
                    positionFleche = 0;
                else if(position < 13){
                    positionFleche = position;
                    position = 0;
                }
                break;
            }
            break;
        default:
            break;
        }

        afficherMusiques(dir, position, images, chemin, positionFleche);
        SDL_Flip(ecran);
    }

    SDL_FreeSurface(images.fleche);
    SDL_FreeSurface(images.mp3);
    SDL_FreeSurface(images.dossier);
    SDL_FreeSurface(images.exe);
    SDL_FreeSurface(images.playlist);

    closedir(dir);
}

void afficherMusiques(DIR *dir, int position, ImagesMusiques images, char *chemin, int positionFleche)
{
    int retour = 0, i = 0;
    struct dirent *ent;

    SDL_Surface *texte = NULL;
    SDL_Rect positionTexte, posFleche;
    SDL_Color couleurBlanche = {255, 255, 255}, couleurNoire = {0, 0, 0};
    TTF_Font *police;

    rewinddir(dir);
    ent = readdir(dir);
    ent = readdir(dir);

    for(i = 0; i < position; i++)
        ent = readdir(dir);

    police = TTF_OpenFont("data/Limousines.ttf", 20);

    posFleche.x = 5;
    positionTexte.x = images.fleche->w + 10;
    positionTexte.y = 0;

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    while((ent = readdir(dir)) != NULL)
    {
        texte = TTF_RenderText_Shaded(police, ent->d_name, couleurBlanche, couleurNoire);//Attribution de ent->d_name avec TTF

        retour = isMP3(ent->d_name, chemin);
        switch(retour)//On colle l'icon correspondant
        {
        case DOSSIER://Dossier
            SDL_BlitSurface(images.dossier, NULL, ecran, &positionTexte);
            positionTexte.x = positionTexte.x + 25;
            SDL_BlitSurface(texte, NULL, ecran, &positionTexte);//Puis on colle le texte
            break;
        case MP3://Musique
            SDL_BlitSurface(images.mp3, NULL, ecran, &positionTexte);
            positionTexte.x = positionTexte.x + 25;
            SDL_BlitSurface(texte, NULL, ecran, &positionTexte);//Puis on colle le texte

            if(isInLlv(chemin, ent->d_name) == 1)
            {
                positionTexte.x += texte->w + 5;
                SDL_BlitSurface(images.playlist, NULL, ecran, &positionTexte);
            }
            break;
        case FICHIER_QUELCONQUE://Fichier
            SDL_BlitSurface(images.exe, NULL, ecran, &positionTexte);
            positionTexte.x = positionTexte.x + 25;
            SDL_BlitSurface(texte, NULL, ecran, &positionTexte);//Puis on colle le texte
            break;
        case LISTE_LECTURE://Liste de lecture
            SDL_BlitSurface(images.playlist, NULL, ecran, &positionTexte);
            positionTexte.x = positionTexte.x + 25;
            SDL_BlitSurface(texte, NULL, ecran, &positionTexte);//Puis on colle le texte
            break;
        }

        positionTexte.y += texte->h + 8;//On décale la position.y puis on recommence
        positionTexte.x = images.fleche->w + 10;
    }
    posFleche.y = positionFleche * (texte->h + 8);
    SDL_BlitSurface(images.fleche, NULL, ecran, &posFleche);

    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
}

int isMP3(char *chaine, char *cheminBase)//Permet de savoir si c'est une musique ou un dossier
{
    char chemin[1000] = {0};
    int nombreLettres;

    DIR *dir;//Variables

    sprintf(chemin, cheminBase);
    strcat(chemin, chaine);//Crée un double du chemin et colle le nom du fichier à tester

    dir = opendir(chemin);//Initialisations

    if(dir != NULL)
    {
        closedir(dir);
        return DOSSIER;//Dossier
    }
    else
    {
        closedir(dir);
        if(strchr(chaine, '.') == NULL)
            return FICHIER_QUELCONQUE; //Fichier quelconque
    }

    nombreLettres = strlen(chaine);

    while(chaine[nombreLettres] != '.')
        nombreLettres--;

    chaine[nombreLettres + 1] = tolower(chaine[nombreLettres + 1]);//On passe les lettres en majuscules
    chaine[nombreLettres + 2] = tolower(chaine[nombreLettres + 2]);
    chaine[nombreLettres + 3] = tolower(chaine[nombreLettres + 3]);
    chaine[nombreLettres + 4] = tolower(chaine[nombreLettres + 4]);

    if( (chaine[nombreLettres + 1] == 'm' && chaine[nombreLettres + 2] == 'p' && chaine[nombreLettres + 3] == '3') ||//Fichier .mp3
        (chaine[nombreLettres + 1] == 'f' && chaine[nombreLettres + 2] == 'l' && chaine[nombreLettres + 3] == 'a' && chaine[nombreLettres + 4] == 'c') ||//Fichier .flac
        (chaine[nombreLettres + 1] == 'w' && chaine[nombreLettres + 2] == 'a' && chaine[nombreLettres + 3] == 'v') )//Fichier .wav
        return MP3;//Fichier musique
    else if(chaine[nombreLettres + 1] == 'l' && chaine[nombreLettres + 2] == 'l' && chaine[nombreLettres + 3] == 'v')
        return LISTE_LECTURE;//Liste de lecture
    else
    {
        return FICHIER_QUELCONQUE;//Fichier quelconque
    }
}

int isInLlv(char *chemin, char *nomMusique)
{
    char cheminMusique[1000] = {0}, cheminTemp[1000] = {0};
    int retour = 0, i = 0;

    FILE *fichier = NULL;

    sprintf(cheminMusique, chemin);
    strcat(cheminMusique, nomMusique);

    fichier = fopen("data/liste_musiques.llv", "r");
    if(fichier == NULL){
        fprintf(fichier, "Erreur lors de l'ouverture du fichier musique.\n");
        return 0;
    }
    while(fgets(cheminTemp, 1000, fichier))
    {
        i = strlen(cheminTemp);
        cheminTemp[i - 1] = '\0';//Remplace '\n' de la fin de la ligne
        if(strcmp(cheminMusique, cheminTemp) == 0)
            retour = 1;
    }

    fclose(fichier);
    return retour;
}

int entree(char *chemin, char *ent_name)
{
    int retour = 0;

    retour = isMP3(ent_name, chemin);

    if(retour == DOSSIER)
    {
        strcat(chemin, ent_name);
        ajouterSlash(chemin);
        return retour;
    }
    else
        return retour;
}

void ajouterSlash(char *chemin)
{
    int compteur = 0;
    while(chemin[compteur] != '\0')//On se place à la fin du chemin
        compteur++;

    chemin[compteur] = '/';//Place un '/' à la fin
    chemin[compteur + 1] = '\0';//Ajoute le '\0'
}

void dossierPrecedent(char *chemin)
{
    int compteur = 0;
    while(chemin[compteur] != '\0')//Fin du chemin
        compteur++;
    if(chemin[compteur - 1] != '/')
        exit(EXIT_FAILURE);

    compteur -= 2;//On se place juste avant le dernier '/'
    while(chemin[compteur] != '/')//Puis on va jusqu'au '/' précedant
        compteur--;
    chemin[compteur + 1] = '\0';//On coupe la chaine juste après le '/' trouvé
}

int limiteDossier(DIR *dir)
{
    int nombreFichiers = 0;

    rewinddir(dir);
    readdir(dir);
    readdir(dir);

    while(readdir(dir) != NULL)
          nombreFichiers++;

    return nombreFichiers;
}

void ajouterMusique(char *chemin, char *nomMusique)
{
    char cheminMusique[1000];
    char lettre = 0;

    FILE *fichier = NULL;

    sprintf(cheminMusique, chemin);
    strcat(cheminMusique, nomMusique);

    fichier = fopen("data/liste_musiques.llv", "r+");
    if(fichier == NULL){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des musiques.\n");
        return;
    }

    while(lettre != EOF)
        lettre = fgetc(fichier);

    fprintf(fichier, "%s\n", cheminMusique);
    fclose(fichier);
}

void changerCheminBase(char *chemin)
{
    FILE *fichier = NULL;

    fichier = fopen("data/chemin_base.ms", "w+");
    if(fichier == NULL){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier 'chemin_base.ms'");
        return;
    }
    fputs(chemin, fichier);

    fclose(fichier);
}

void initialiserStructure(ImagesMusiques *images)
{
    images->fleche = IMG_Load("data/Images/fleche.png");
    images->mp3 = IMG_Load("data/Images/mp3.png");
    images->dossier = IMG_Load("data/Images/dossier.png");
    images->playlist = IMG_Load("data/Images/playlist.png");
    images->exe = IMG_Load("data/Images/exe.png");
}

void cheminBase(char *chemin)
{
    FILE *fichier = NULL;

    fichier = fopen("data/chemin_base.ms", "r");
    if(fichier == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier 'chemin_base.ms'");
        exit(EXIT_FAILURE);
    }

    fscanf(fichier, "%s", chemin);

    fclose(fichier);
}

int deplacerCaractere(DIR *dir, char caractere, int *posBase)//On modifie la pos jusqu'a la lettre trouvée
{
    char chaine[100] = {0};
    int pos = 0;

    struct dirent *ent;//Variables

    rewinddir(dir);//Initialisation du DIR
    ent = readdir(dir);
    ent = readdir(dir);

    while( (ent = readdir(dir)) != NULL )//On lit tout le dossier
    {
        pos++;
        sprintf(chaine, ent->d_name);
        chaine[0] = toupper(chaine[0]);

        if(chaine[0] == caractere)//Si le caractere a été trouvé
        {
            pos--;
            *posBase = pos;//On modifie avec la nouvelle pos
            return 1;
        }
    }

    rewinddir(dir);

    return 0;
}

int isLetter(SDL_Surface *ecran, SDL_Event *event, DIR *dir)
{
    int resultat, pos;

    switch(event->key.keysym.sym)//Si une lettre a été appuyée
    {
    case SDLK_a:
        resultat = deplacerCaractere(dir, 'Q', &pos);//On regarde si on peut se déplacer et on modifie les valeures de pos et positionFleche
        if(!resultat)
            pos = 0;
        break;
    case SDLK_b:
        resultat = deplacerCaractere(dir, 'B', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_c:
        resultat = deplacerCaractere(dir, 'C', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_d:
        resultat = deplacerCaractere(dir, 'D', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_e:
        resultat = deplacerCaractere(dir, 'E', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_f:
        resultat = deplacerCaractere(dir, 'F', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_g:
        resultat = deplacerCaractere(dir, 'G', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_h:
        resultat = deplacerCaractere(dir, 'H', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_i:
        resultat = deplacerCaractere(dir, 'I', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_j:
        resultat = deplacerCaractere(dir, 'J', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_k:
        resultat = deplacerCaractere(dir, 'K', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_l:
        resultat = deplacerCaractere(dir, 'L', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_SEMICOLON:
        resultat = deplacerCaractere(dir, 'M', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_n:
        resultat = deplacerCaractere(dir, 'N', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_o:
        resultat = deplacerCaractere(dir, 'O', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_p:
        resultat = deplacerCaractere(dir, 'P', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_q:
        resultat = deplacerCaractere(dir, 'A', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_r:
        resultat = deplacerCaractere(dir, 'R', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_s:
        resultat = deplacerCaractere(dir, 'S', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_t:
        resultat = deplacerCaractere(dir, 'T', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_u:
        resultat = deplacerCaractere(dir, 'U', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_v:
        resultat = deplacerCaractere(dir, 'V', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_w:
        resultat = deplacerCaractere(dir, 'Z', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_x:
        resultat = deplacerCaractere(dir, 'X', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_y:
        resultat = deplacerCaractere(dir, 'Y', &pos);
        if(!resultat)
            pos = 0;
        break;
    case SDLK_z:
        resultat = deplacerCaractere(dir, 'W', &pos);
        if(!resultat)
            pos = 0;
        break;
    default:
        pos = 0;
        break;
    }
    if(!resultat)//Si aucune lettre n'a été trouvé
        return 0;


    return pos;//On retourne la nouvelle pos
}
