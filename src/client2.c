#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "server_structures.h"


/**
 * @brief   La función main del Cliente 2 se encarga de inicializar
 *          una comunicación con el Servidor mediante el mecanismo
 *          IPC POSIX Message Queues
 *
 * @details Se inicializa una cola "queue" y se instancia un canal
 *          de comunicacion "qd_server" de tipo mdq_t.
 *          Luego, intenta abrir la comunicación con el server haciendo
 *          uso de la función mq_open() y, en caso de ser exitoso, se ingresa
 *          a un bucle en donde constantemente se le pide al cliente que
 *          ingrese strings por consola para enviárselos al servidor.
 *          Si el cliente ingresa "end", se finaliza la comunicación 
 *          y el cliente se cierra.
 */


int main(void)
{
  char queue [BUF_SIZE];

  mqd_t qd_server;

  if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1){

      perror ("Client: mq_open (server)");

      exit(EXIT_FAILURE);

  }

  while (1){

    printf("Write in POSIX Message Queue Channel: ");

    fgets (queue, BUF_SIZE, stdin);

    if(strncmp( queue, "end", 3) == 0){

      return(EXIT_SUCCESS);

    }

    if(mq_send ( qd_server, queue, strlen (queue) + 1, 0) == -1){

        perror ("Client: Not able to send message to server");

        return(EXIT_FAILURE);

    }
    
  }
  
}