#ifndef JOUER_H_INCLUDED
#define JOUER_H_INCLUDED

int jouer(int niveau, int type, int rejouer);
int deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction);
void deplacerCaisse(int *premiereCase, int *secondeCase, int *deplacement);
int finNiveau(int carte[][NB_BLOCS_HAUTEUR]);
void selectionMusique(FMOD_SOUND *sound);

#endif // JOUER_H_INCLUDED
