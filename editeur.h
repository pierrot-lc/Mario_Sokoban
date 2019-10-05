#ifndef DEF_EDITEUR
#define DEF_EDITEUR

    typedef struct EtatEditeur EtatEditeur;
    struct EtatEditeur
    {
        int objetActuel;
        int clicGaucheEnCours;
        int clicDroitEnCours;
    };

    typedef struct SonsEditeur SonsEditeur;
    struct SonsEditeur
    {
        FMOD_SOUND *bell;
        FMOD_SOUND *step;
        FMOD_SOUND *box;
        FMOD_SOUND *wall;
    };

    typedef struct Images Images;
    struct Images
    {
        SDL_Surface *mur;
        SDL_Surface *caisse;
        SDL_Surface *objectif;
        SDL_Surface *mario;
        SDL_Surface *objetSouris;
        SDL_Surface *caisse_ok;
    };

    void editeur(int niveau);
    void chargerSon(SonsEditeur *sons);
    void chargerImages(Images *images);
    void clicGauche(int carte[][NB_BLOCS_HAUTEUR], EtatEditeur *editeur, SDL_Rect positionSouris, SonsEditeur *sons);
    void afficherSouris(EtatEditeur *editeur, Images *images, SDL_Rect positionSouris);

#endif
