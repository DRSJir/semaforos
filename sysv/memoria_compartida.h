#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 1024

struct Datos {
	int numeros[4]; // arreglo de 4 enteros
	float valor; // número flotante
	char letra; // carácter
	char cadena[50]; // cadena de caracteres
};

typedef struct {
    char mensaje[1024];
    int numeros[4]; // arreglo de 4 enteros
	float valor; // número flotante
	char letra; // carácter
	char cadena[50]; // cadena de caracteres
} MemoriaCompartida;

union semaforo_union {
    int val;
};