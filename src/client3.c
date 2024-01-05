// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "server_structures.h"


  
/**
 * @brief   La función Main del Cliente 3 se encarga de inicializar
 *          una comunicación con el Servidor mediante el mecanismo
 *          IPC Message Queue
 *
 * @details Se crean las variables key y msgid y se crea la cola de mensajes
 *          para iniciar la comunicación. 
 *          En caso exitoso, se ingresa a un ciclo While en donde constantemente
 *          se le solicita al usuario que ingrese el string a enviar por consola.
 *          Acto seguido, se envía el string al servidor haciendo uso de las funciones
 *          proporcionadas por el mecanismo IPC Message Queue. Si el cliente ingresa
 *          "end", se finaliza la comunicación y el cliente se cierra.
 */

int main(void)
{

  key_t key;

  int msgid;

  key = ftok("progfile", 65);

  if((msgid = msgget(key, 0666 | IPC_CREAT))==-1){

    perror("Error in Client trying to open Message Queue");

  }                      

  while(1){

    message.mesg_type = 1;

    printf("Write in Message Queue Channel: ");

    fgets(message.mesg_text,BUF_SIZE,stdin);

    if(strncmp(message.mesg_text,"end",3) == 0){

      return(EXIT_SUCCESS);

    } 

    if(msgsnd( msgid, &message, sizeof(message), 0) == -1){

      perror("Error in client trying to send a message in Message Queue");

      return(EXIT_FAILURE);

    }
                     
  }
    
}
