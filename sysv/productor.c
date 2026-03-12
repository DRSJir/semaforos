#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "memoria_compartida.h"


int main(void) {
    /* Crear segmento de memoria compartida */
    int shmid = shmget(SHM_KEY, sizeof(MemoriaCompartida), 0666 | IPC_CREAT);
    if (shmid ==  -1) {
        perror("Error al crear la memoria compartida");
        exit(1);
    }

    /* Enlazar la memoria compartida */
    MemoriaCompartida *shm = (MemoriaCompartida *)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("Error al enlazar la memoria compartida");
        exit(1);
    }

    /* Crear los semáforos */
    int semid = semget(SEM_KEY, 2, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("Error al crear el semáforo");
        exit(1);
    }

    /* Inicializar los semáforos */
    union semaforo_union arg;

    arg.val = 1;            // semáforo 0 (espacios)
    semctl(semid, 0, SETVAL, arg);

    arg.val = 0;            // semáforo 1 (datos)
    semctl(semid, 1, SETVAL, arg);

    /* Establecer una semilla aleatoria */
    srand(time(NULL));

    /* índice, operación, bandera */
    struct sembuf wait_espacio = {0, -1, 0}; // bloquear, sin operaciones especiales
    struct sembuf signal_dato  = {1, 1, 0};  // despertar y liberar recursos, sin operaciones especiales

    while (1) {
        semop(semid, &wait_espacio, 1);
        
        int numero = rand() % 100;
        
        sprintf(shm->mensaje, "Mensaje del productor: %d", numero);
        printf("Productor escribió: %s\n", shm->mensaje);

        semop(semid, &signal_dato, 1);

        sleep(2);
    }

    /* Desvincular memoria compartida */
    shmdt(shm);

    /* NO eliminamos semáforo para que se observe con ipcs */
    return 0;
}