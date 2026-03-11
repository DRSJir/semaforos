# Práctica: Semáforos nombrados y semáforos SYSV
Considere una aplicación de comunicación entre procesos que utiliza memoria compartida para intercambiar información. La memoria compartida estará organizada mediante la siguiente estructura:
```c
struct Datos {
	int numeros[4]; // arreglo de 4 enteros
	float valor; // número flotante
	char letra; // carácter
	char cadena[50]; // cadena de caracteres
};
```
El acceso a esta estructura debe realizarse utilizando mecanismos de sincronización mediante semáforos, de tal manera que:
- Un proceso productor sea el encargado de llenar la estructura con datos.
- Un proceso consumidor sea el encargado de leer o consumir la información almacenada en la estructura.

**La práctica debe implementarse utilizando dos tipos de semáforos:**
1. Semáforos nombrados (POSIX)
2. Semáforos del sistema V (SYSV)

Adicionalmente, se debe elaborar un script en Bash que permita recolectar información del sistema relacionada con la ejecución del programa, incluyendo:
- Segmentos de memoria compartida existentes.
- Semáforos creados por la aplicación.
- Procesos que están utilizando dichos recursos.

El script deberá mostrar información que permita identificar los recursos creados y su
estado durante la ejecución de la aplicación, además de los procesos que lo utilizan. **Entregables**:
- 2 aplicaciones productor-consumidor
	- Semáforos Nombrados
	- Semáforos SYSV
- 2 script BASH para monitoreo
- 1 reporte de máximo 3 páginas explicando la solución.