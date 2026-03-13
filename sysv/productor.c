#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "memoria_compartida.h"

void llenar_datos(Datos *shm);
void imprimir_tabla(Datos *shm);

int main(void) {
    /* Crear segmento de memoria compartida */
    int shmid = shmget(SHM_KEY, sizeof(Datos), 0666 | IPC_CREAT);
    if (shmid ==  -1) {
        perror("Error al crear la memoria compartida");
        exit(1);
    }

    /* Enlazar la memoria compartida */
    Datos *shm = (Datos *)shmat(shmid, NULL, 0);
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

    printf("│%-25s │ %-11s │ %-10s │ %-10s│\n", "Mensaje", "Números", "Valor", "Letra");

    while (1) {
        semop(semid, &wait_espacio, 1);
        
        llenar_datos(shm);
        imprimir_tabla(shm);

        semop(semid, &signal_dato, 1);

        sleep(2);
    }

    /* Desvincular memoria compartida */
    shmdt(shm);

    /* NO eliminamos semáforo para que se observe con ipcs */
    semctl(semid, 0, IPC_RMID);
    return 0;
}

/* Llenar los datos de la estructura Datos */
void llenar_datos(Datos *shm) {
    int i;
    int numero = rand() % 100;
    sprintf(shm->cadena, "Mensaje del productor: %d", numero);

    for(i=0; i<4; i++) {
        shm->numeros[i] = numero;
    }

    float flotante = ((float)rand() / (float)RAND_MAX) * 100.0;
    shm->valor = flotante;
    char caracter = 'A' + (rand() % 26);
    shm->letra = caracter;

    return;
}

void imprimir_tabla(Datos *shm) {
    int i;

    printf("├──────────────────────────┼────────────┼────────────┼───────────┤\n");
    printf("│%-25s │ %-10d │ %-10f │ %-10c│\n", shm->cadena, shm->numeros[0], shm->valor, shm->letra); 
}
