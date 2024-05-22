#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <threads.h>
#include <pthread.h>

#include "stream_common.h"
#include "oggstream.h"
#include "synchro.h"


int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE", argv[0]);
        exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);

    // start the two stream readers
    pthread_t thrd_video;
    pthread_t thrd_audio;

    pthread_cond_init(&tailleFenetreCond, NULL);
    pthread_mutex_init(&tailleFenetreMut, NULL);
    tailleFenetreEnd = false;
    
    pthread_cond_init(&fenetreTextureCond, NULL);
    pthread_mutex_init(&fenetreTextureMut, NULL);
    fenetreTextureEnd = false;
    
    pthread_cond_init(&textureCond, NULL);
    pthread_mutex_init(&textureMut, NULL);;
    
    pthread_create(&thrd_audio, NULL, vorbisStreamReader, (void*) argv[1]);
    pthread_create(&thrd_video, NULL, theoraStreamReader, (void*) argv[1]);

    // wait audio thread
    pthread_join(thrd_audio, NULL);


    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(thrd_video);
    pthread_cancel(thrd_sdl);

    // attendre les 2 threads videos
    pthread_join(thrd_video, NULL);
    pthread_join(thrd_sdl, NULL);

    exit(EXIT_SUCCESS);
}
