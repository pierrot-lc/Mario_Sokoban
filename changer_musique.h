#ifndef CHANGER_MUSIQUE_H_INCLUDED
#define CHANGER_MUSIQUE_H_INCLUDED

#define MAXFLECHE   13//La fleche ne doit pas dépasser le 13eme dossier

typedef struct ImagesMusiques ImagesMusiques;
struct ImagesMusiques
{
    SDL_Surface *fleche;
    SDL_Surface *mp3;
    SDL_Surface *dossier;
    SDL_Surface *playlist;
    SDL_Surface *exe;
};

void control();
void afficherMusiques(DIR *dir, int position, ImagesMusiques images, char *chemin, int positionFleche);
int isMP3(char *chaine, char *cheminBase);
int isInLlv(char *chemin, char *nomMusique);
int limiteDossier(DIR *dir);
int entree(char *chemin, char *ent_name);
void ajouterSlash(char *chemin);
void dossierPrecedent(char *chemin);
void ajouterMusique(char *chemin, char *nomMusique);
void changerCheminBase(char *chemin);
void initialiserStructure(ImagesMusiques *images);
void cheminBase(char *chemin);
int deplacerCaractere(DIR *dir, char caractere, int *posBase);
int isLetter(SDL_Surface *ecran, SDL_Event *event, DIR *dir);

enum{DOSSIER, MP3, LISTE_LECTURE, FICHIER_QUELCONQUE};

#endif // CHANGER_MUSIQUE_H_INCLUDED
