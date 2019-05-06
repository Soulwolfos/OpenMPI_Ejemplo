
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <ctime>

/* cabecera de las llamadas a MPI */
#include <mpi.h> 

/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por 
 * el proceso 0, que los imprime
 */

int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    
    char mensaje[100]; // mensaje  
    char hostname[256]; //nombre de la maquina
      
    
    /* devuelve estado al recibir*/
    MPI_Status estado; 

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan 
     * ejecutando nuestro porgrama */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    
    
    if (mi_rango != 0) {
        /* Crea mensaje */
        
        //se obtiene el nombre de la maquina
        gethostname(hostname, sizeof(hostname));
      
        //se obtiene la fecha y hora actual
        time_t tiempoAhora = time(NULL); 
        struct tm fecha = *localtime(&tiempoAhora); 
         
        sprintf(mensaje, "%s %d-%d-%d %d:%d:%d"
                ,hostname 
                ,fecha.tm_year +1900
                ,fecha.tm_mon + 1  
                ,fecha.tm_mday
                ,fecha.tm_hour
                ,fecha.tm_min
                ,fecha.tm_sec  
                );
        
        dest = 0;
        
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
        
    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            
            printf("%s\n", mensaje);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}