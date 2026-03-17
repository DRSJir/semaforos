#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 4321
#define SHM_SIZE 1024

#define SEM_DATOS "/sem_datos"
#define SEM_ESPACIOS "/sem_espacios"

typedef struct {
    char mensaje[SHM_SIZE];
} MemoriaCompartida;

int main(void) {
    /* Acceder a memoria compartida */
    int shmid = shmget(SHM_KEY, sizeof(MemoriaCompartida), 0666);
    if (shmid == -1) {
        perror("Error al acceder a memoria compartida");
        exit(1);
    }

    /* Vincular memoria compartida */
    MemoriaCompartida *shm = (MemoriaCompartida *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("Error al asignar memoria compartida");
        exit(1);
    }

    /* Abrir semáforos nombrados */
    sem_t *sem_datos = sem_open(SEM_DATOS, 0);
    sem_t *sem_espacios = sem_open(SEM_ESPACIOS, 0);

    if (sem_datos == SEM_FAILED || sem_espacios == SEM_FAILED) {
        perror("Error al abrir semáforos");
        exit(1);
    }


    while (1) {

        // Esperar datos del productor
        sem_wait(sem_datos);

        printf("Consumidor leyó: %s\n", shm->mensaje);

        memset(shm->mensaje, 0, SHM_SIZE);

        // Avisar que hay espacio disponible
        sem_post(sem_espacios);

        sleep(1);
    }

    shmdt(shm);

    sem_close(sem_datos);
    sem_close(sem_espacios);

    return 0;
}
