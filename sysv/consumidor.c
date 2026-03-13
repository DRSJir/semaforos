#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#include "memoria_compartida.h"

void consumir_datos(Datos *shm);
void imprimir_tabla(Datos *shm);

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

    printf("│%-25s │ %-11s │ %-10s │ %-10s│\n", "Cadena", "Números", "Valor", "Letra");

    while(1) {
        semop(semid, &wait_dato, 1);

        imprimir_tabla(shm);
        consumir_datos(shm);

        semop(semid, &signal_espacio, 1);

        sleep(1);
    }

    shmdt(shm);
    return 0;
}

/* En realidad limpia la estructura Datos ;) */
void consumir_datos(Datos *shm) {
    memset(shm->numeros, 0, sizeof(shm->numeros));
    shm->valor = 0.0;
    shm->letra = '?';
    memset(shm->cadena, 0, sizeof(shm->cadena));

    return;
}

void imprimir_tabla(Datos *shm) {
    printf("├──────────────────────────┼────────────┼────────────┼───────────┤\n");
    printf("│%-25s │ %-10d │ %-10f │ %-10c│\n", shm->cadena, shm->numeros[0], shm->valor, shm->letra); 
    return;
}
