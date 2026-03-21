#!/usr/bin/bash

echo -e "introduzca el tipo de semaforo a monitorear\nSYS V: 1\nposix: 2"
read -p "Opcion: " opcion

# Mostrar los segmentos de la memoria y semaforos
if [ $opcion -eq 1 ]; then
    ipcs -s
    read -p "Introdice el id del semáforo a monitorear: " idSemaforo
fi
ipcs -m

# Solicitar id del proceso al usuario
read -p "Ingresé el ID del proceso a monitoriar: " idSegmento

numProc=$( ipcs -m $idSegmento | tail -n 2 | head -n 1 | awk '{print $6}')
idSegmento=$( ipcs -m $idSegmento | tail -n 2 | head -n 1 | awk '{print $2}')

# Obtener el porceso creador, último proceso y usuario
idProcCreate=$(ipcs -m -p $idSegmento | tail -n 2 | head -n 1 | awk '{print $3}')
idUltimoProceso=$(ipcs -m -p $idSegmento | tail -n 2 | head -n 1 | awk '{print $4}')
usuarioProceso=$(ipcs -m -p $idSegmento | tail -n 2 | head -n 1 | awk '{print $2}')

echo -e "Hay $numProc procesos que usan el segmento $idSegmento \n"
echo -e "Creador $idProcCreate \nUltimo  $idUltimoProceso \nUsuario $usuarioProceso\n"

pids=()
nombres=()
recursos=()

# Buscamos en todos los procesos (/proc/[1-9]*)
for proc_dir in /proc/[1-9]*/; do
    pid=$(basename $proc_dir)

    # Para memoria compartida
    if grep -q "SYSV" "$proc_dir/maps" 2>/dev/null; then
        nombres+=("$(cat "$proc_dir/comm")")
        pids+=("$pid")

    fi
done

{
  echo "PID Nombre"
  for i in "${!pids[@]}"; do
      echo "${pids[$i]} ${nombres[$i]}"
  done
} | column -t

echo -e "\nLos semaforos creados por la app son"
if [ $opcion == 2 ]; then # POSIX
    for semaforo in /dev/shm/*; do
        echo $semaforo
    done
else # SYSV
    echo "Estado del conjunto de semáforos SYSV ($idSemaforo):"
    ipcs -s -i $idSemaforo
fi