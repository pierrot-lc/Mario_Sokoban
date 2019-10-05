#ifndef FICHIER_H_INCLUDED
#define FICHIER_H_INCLUDED
#include "constantes.h"

int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR], int numero, int type);
int sauvegarderNiveau(int carte[][NB_BLOCS_HAUTEUR], int niveau);
void supprimerNiveau(int niveau);
void afficherJeu(int carte[][NB_BLOCS_HAUTEUR]);
int modeInfini(int mode);
void recommencerHistoire();

enum{RIEN_FAIRE, ACTIVER, DESACTIVER};

#endif // FICHIER_H_INCLUDED
