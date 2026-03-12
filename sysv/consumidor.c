#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#include "memoria_compartida.h"

int main(void) {
    /* Crear el id del segmento de la memoria compartida */
    int shmid = shmget(SHM_KEY, sizeof(Datos), 0666);
    if (shmid == -1) {
        perror("Error al acceder a memoria compartida");
        exit(1);
    }

    Datos *shm = (Datos *)shmat(shmid, NULL, 0);

    /* Obtener los semaforos */
    int semid = semget(SEM_KEY, 2, 0666);
    if (semid == -1) {
        perror("Error al acceder al semáforo");
        exit(1);
    }

    struct sembuf wait_dato = {1,-1,0};
    struct sembuf signal_espacio = {0,1,0};

    while(1) {
        semop(semid, &wait_dato, 1);

        printf("Consumidor leyó: %s\n", shm->mensaje);
        memset(shm->mensaje, 0, SHM_SIZE);

        semop(semid, &signal_espacio, 1);

        sleep(1);
    }

    shmdt(shm);
    return 0;
}