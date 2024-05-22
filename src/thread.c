#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <pthread.h>

#define THREAD_COUNT 10
#define THREAD_LOOP 1000000

// Une ressource partagée sur laquelle synchroniser nos threads.
mtx_t mutex;
unsigned long counter = 0;


// Définition de la fonction sur laquelle amorcer tous nos threads.
int threadFunction(void* data) {

    for( int i = 0; i<THREAD_LOOP; i++ ) {
        mtx_lock( &mutex );
        counter ++;
        mtx_unlock( &mutex );
    }

    printf( "Thread %ld terminé\n", (long) data );

    return thrd_success;
}

int main() {
    int a = 1;
    mtx_init( &mutex, mtx_plain );

    // On démarre tous nos threads sur la fonction définie ci-dessus.
    thrd_t threads[THREAD_COUNT];
    for( long i=0; i<THREAD_COUNT; i++ ) {
        thrd_create( &threads[i], threadFunction, (long *) i);
    }

    // On attend la terminaison de tous nos threads.
    for( int i=0; i<THREAD_COUNT; i++ ) {
        thrd_join( threads[i], NULL );
    }

    // Affichage du compteur et libération du mutex.
    printf( "Counter == %ld\n", counter );
    mtx_destroy( &mutex );

    return EXIT_SUCCESS;
}
