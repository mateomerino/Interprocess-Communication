#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "server_structures.h"

/**
 * @brief   La función Main del Cliente 1 se encarga de inicializar
 *          una comunicación con el Servidor mediante el mecanismo
 *          IPC FIFO.
 *
 * @details Se crea una variable para las requests de tipo request_t
 *          y un file descriptor requests_fd.
 *          Luego, se intenta abrir la FIFO para comunicarse con el servidor.
 *          En caso exitoso, se ingresa a un ciclo While en donde constantemente
 *          se le pide al usuario que ingrese un string por consola y se lo envía
 *          al servidor haciendo uso de las funciones proporcionadas por FIFO.
 *          Si el cliente ingresa "end", se finaliza la comunicación y 
 *          el cliente se cierra.
 */

int main(void){
  
  request_t request;

  int requests_fd;


  if((requests_fd=open( FIFO_REQUEST, O_WRONLY)) == -1){

    perror("Error al intentar abrir la fifo");

    exit(EXIT_FAILURE);

  }

  while(1){

    printf("Write in FIFO Channel: ");

    fgets(request.query,BUF_SIZE,stdin);
    
    if(write( requests_fd, &request, sizeof(request_t)) == -1){

      perror("Error in Client trying to write in FIFO:");

      return(EXIT_FAILURE);

    }   

    if(strncmp( request.query, "end", 3) == 0){

      return(EXIT_SUCCESS);
      
    }          
  }
}