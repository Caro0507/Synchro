#include "synchro.h"
#include "ensitheora.h"

extern bool fini;

/* les variables pour la synchro, ici */

pthread_cond_t tailleFenetreCond;
pthread_mutex_t tailleFenetreMut;
bool tailleFenetreEnd;
pthread_cond_t fenetreTextureCond;
pthread_mutex_t fenetreTextureMut;
bool fenetreTextureEnd;
pthread_cond_t textureCond;
pthread_mutex_t textureMut;
int nbTexture = 0;

pthread_t thrd_sdl;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    windowsx = buffer->width;
    windowsy = buffer->height;
    pthread_mutex_lock(&tailleFenetreMut);
    tailleFenetreEnd = true;
    pthread_cond_signal(&tailleFenetreCond);
    pthread_mutex_unlock(&tailleFenetreMut);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&tailleFenetreMut);
    while (!tailleFenetreEnd)
        pthread_cond_wait(&tailleFenetreCond, &tailleFenetreMut);
    pthread_mutex_unlock(&tailleFenetreMut);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&fenetreTextureMut);
    fenetreTextureEnd = true;
    pthread_cond_signal(&fenetreTextureCond);
    pthread_mutex_unlock(&fenetreTextureMut);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&fenetreTextureMut);
    while (!fenetreTextureEnd)
        pthread_cond_wait(&fenetreTextureCond, &fenetreTextureMut);
    pthread_mutex_unlock(&fenetreTextureMut);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&textureMut);
    while (nbTexture == 0)
        pthread_cond_wait(&textureCond, &textureMut);
}

void finConsommerTexture() {
    nbTexture -= 1;
    pthread_cond_signal(&textureCond);
    pthread_mutex_unlock(&textureMut);
}


void debutDeposerTexture() {
    pthread_mutex_lock(&textureMut);
    while (nbTexture == NBTEX)
        pthread_cond_wait(&textureCond, &textureMut);
}

void finDeposerTexture() {
    nbTexture += 1;
    pthread_cond_signal(&textureCond);
    pthread_mutex_unlock(&textureMut);
}
