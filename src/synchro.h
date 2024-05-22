#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdbool.h>
#include <pthread.h>
#include "ensitheora.h"

extern bool fini;

/* Les extern des variables pour la synchro ici */

extern pthread_cond_t tailleFenetreCond;
extern pthread_mutex_t tailleFenetreMut;
extern bool tailleFenetreEnd;
extern pthread_cond_t fenetreTextureCond;
extern pthread_mutex_t fenetreTextureMut;
extern bool fenetreTextureEnd;
extern pthread_cond_t textureCond;
extern pthread_mutex_t textureMut;

extern pthread_t thrd_sdl;

/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
