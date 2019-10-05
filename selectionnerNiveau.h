#ifndef SELECTIONNERNIVEAU_H_INCLUDED
#define SELECTIONNERNIVEAU_H_INCLUDED

#include <fmodex/fmod.h>//Bug à regler
#include "constantes.h"

void menuNiveaux(FMOD_CHANNEL *canal, int type, int carte[][NB_BLOCS_HAUTEUR]);
void afficherNiveaux(int niveau, int type);
void deplacement(int *niveau, int direction, int type);
void partie(int niveau);
void partieEditeur(int niveau);
int niveauMaximal(int type);
int niveauxDebloques();
int menuEntree(int type, int niveau, FMOD_CHANNEL *canal, int carte[][NB_BLOCS_HAUTEUR]);

#endif // SELECTIONNERNIVEAU_H_INCLUDED
