#define SHM_KEY 1234
#define SEM_KEY 5678

typedef struct {
    int numeros[4]; // arreglo de 4 enteros
	float valor; // número flotante
	char letra; // carácter
	char cadena[50]; // cadena de caracteres
} Datos;

union semaforo_union {
    int val;
};