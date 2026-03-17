#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SHM_KEY 4321
#define SHM_SIZE 1024

#define SEM_DATOS "/sem_datos"
#define SEM_ESPACIOS "/sem_espacios"

typedef struct {
    char mensaje[SHM_SIZE];
} MemoriaCompartida;

int main(void) {
    /* Crear segmento de memoria compartida*/
    int shmid = shmget(SHM_KEY, sizeof(MemoriaCompartida), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Error al crear memoria compartida");
        exit(1);
    }

    /* Vincular memoria compartida */
    MemoriaCompartida *shm = (MemoriaCompartida *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("Error al asignar memoria compartida");
        exit(1);
    }

    // Crear semáforo nombrado
    sem_t *sem_datos = sem_open(SEM_DATOS, 0);
    sem_t *sem_espacios = sem_open(SEM_ESPACIOS, 0);
    
    if (sem_espacios == SEM_FAILED || sem_datos == SEM_FAILED) {
        perror("Error al crear semáforos");
        exit(1);
    }

    srand(time(NULL));

    while(1) {
        // Esperar espacio disponible
        sem_wait(sem_espacios);

        int numero = rand() % 100;

        sprintf(shm->mensaje, "Mensaje del productor: %d", numero);

        printf("Productor escribió: %s\n", shm->mensaje);

        // Avisar al consumidor que hay datos
        sem_post(sem_datos);

        sleep(2);
    }

    shmdt(shm);

    sem_close(sem_espacios);
    sem_close(sem_datos);

    // opcional: eliminar semáforos al terminar
    sem_unlink(SEM_ESPACIOS);
    sem_unlink(SEM_DATOS);

    return 0;

}