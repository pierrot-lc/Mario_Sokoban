#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED
    #include <fmodex/fmod.h>
    #include <SDL/SDL.h>

    #define TAILLE_BLOC 34
    #define NB_BLOCS_HAUTEUR 12
    #define NB_BLOCS_LARGEUR 12
    #define LARGEUR_FENETRE NB_BLOCS_LARGEUR * TAILLE_BLOC
    #define HAUTEUR_FENETRE NB_BLOCS_HAUTEUR * TAILLE_BLOC

    enum {HAUT, BAS, GAUCHE, DROITE};
    enum {VIDE, MUR, CAISSE, OBJECTIF, MARIO, CAISSE_OK};
    enum {RIEN, STEP, BELL};
    enum {QUITTER, NIVEAU_TERMINE, RECOMMENCER};
    enum {HISTOIRE, EDITEUR, EDITEUR_SAUVEGARDE, CHANGER_MUSIQUE};

    FMOD_SYSTEM *systemAudio;//Variables Globales
    SDL_Surface *ecran;

#endif // CONSTANTES_H_INCLUDED
