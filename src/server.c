// em_server.c: server code which contains a name/email pairs and will
// fulfill requests from a client through FIFOs.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include  <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <mqueue.h>
#include "server_structures.h"


/**
 * @brief Función principal del server
 *
 * @details     El proceso padre de la función main
 *              realiza 3 llamadas a fork para atender
 *              los distintos mecanismos de IPC que el server
 *              atiende. 
 *              El server queda constantemente a espera de que algún
 *              cliente intente comunicarse en cualquiera de sus tres
 *              canales disponibles (FIFO, message queue, POSIX message queue).
 *              Una vez que el server detecta que un cliente está comunicándose
 *              por algún canal, toma el string correspondiente y lo escribe
 *              en el archivo .txt dedicado a ese canal específico.
 *
 * @returns Si la ejecución del programa fue exitosa.
 */



int main(void){

  printf("SERVER: Starting up\n");
  
 
/*
  * A continuación se listan las variables a utilizar para el manejo del canal
  * correspondiente al mecanismo IPC. En este caso, FIFO. 
*/ 

  int pd1, pd2, pd3;

  setvbuf(stdout, NULL, _IONBF, 0);

  remove(FIFO_REQUEST);                                        
  
  mkfifo(FIFO_REQUEST, S_IRUSR | S_IWUSR);                             
  
  int requests_fd;
  
  ssize_t nread;                                          
  
  request_t request;                                              

/*
  * Luego, se realiza el manejo del mecanismo FIFO propiamente dicho.
  * El proceso hijo atiende las requests de los clientes y el proceso
  * padre continúa su ejecución
*/

  
  pd1=fork();
  if(pd1==-1){
  
    perror("Error forking for FIFO Channel\n");
  
  }

  if(pd1==0){
  
      FILE *s;
  
      char string1[BUF_SIZE];
  
      while(1){

        if((requests_fd = open(FIFO_REQUEST, O_RDONLY)) == -1){      
          
          perror("Error trying to open FIFO");
          
          exit(EXIT_FAILURE);
        
        }

        if((nread = read( requests_fd, &request, sizeof(request_t))) != sizeof(request_t)){

          if(strncmp( request.query, "end", 3) == 0){
            
            continue;
            
            }
          
          perror("Error trying to read FIFO");
          
          exit(EXIT_FAILURE);

        }

        
        if(strncmp(request.query,"end",3)==0){continue;}
        
        printf("Fifo Channel:%s",request.query);                 
        
        strcpy(string1,request.query);
        
        s=fopen(FIFO_FILE,"a");
        
        fputs(string1,s);
        
        fclose(s);
      
      }
  }

/*
  * A continuación se listan las variables a utilizar para el manejo del canal
  * correspondiente al mecanismo IPC. A continuación; POSIX message queue.
*/ 

  mqd_t qd_server;   
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  if((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1){
    perror ("Error trying to open a POSIX message queue");
    exit (EXIT_FAILURE);
  }

/*
  * Por último, el proceso hijo atiende las solicitudes del último canal de comunicación
  * del server y el padre continúa su ejecución.
*/

  
  pd3=fork();
  
  if(pd3==-1){
  
    perror("Fork");
  
    return (EXIT_FAILURE);
  
    }
  
  if(pd3 == 0){
        
        FILE *p;
        
        char in_buffer [MSG_BUFFER_SIZE]; 

        while(1){
        
          if (mq_receive( qd_server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1){
        
          perror ("Error trying to receive a POSIX message");
        
          exit(EXIT_FAILURE);
        
          }
          
          if(strncmp(in_buffer,"end",3)==0){continue;}
          
          printf("POSIX Message Queue Channel:%s",in_buffer);
          
          p=fopen(MESSAGE_QUEUE_POSIX_FILE,"a");
          
          fputs(in_buffer,p);
          
          fclose(p);
        
        }
  }

/*
  * A continuación se listan las variables a utilizar para el manejo del canal
  * correspondiente al mecanismo IPC. A continuación; message queue.
*/ 

  key_t key;
  
  int msgid;
  
  key = ftok("progfile", 65);
  
  if((msgid = msgget(key, 0666 | IPC_CREAT))==-1){                
  
    perror("Error trying to get a message queue");
  
    exit(EXIT_FAILURE);
  
  }

/*
  * Nuevamente, el proceso hijo atiende las solicitudes del canal de comunicación
  * y el padre continúa su ejecución
*/
  
  pd2=fork();
  
  if(pd2==-1){
  
      perror("Error de fork");
  
  }
  
  if(pd2==0){           
  
    FILE *d;
  
    char string2[BUF_SIZE];
    
    while(1){

      if((msgrcv( msgid, &message, sizeof(message), 1,0)) == -1){        
  
        perror("Error trying to read a message in Message Queue Channel");
  
        exit(EXIT_FAILURE);
  
      }

      if(strncmp(message.mesg_text,"end",3)==0){continue;}                   

      printf("Message Queue Channel:%s",message.mesg_text);          

      strcpy(string2,message.mesg_text);

      d=fopen(MESSAGE_QUEUE_FILE,"a");

      fputs(string2,d);

      fclose(d);

    }

  }

/*
 * Lo unico que hace el proceso padre es quedarse en este while
 * preguntando por si el usuario desea apagar el servidor.
 * En ese caso, se eliminan todos los canales de IPC creados.
*/
  char aux[BUF_SIZE];

  while(1){

    fgets(aux,BUF_SIZE,stdin);

    if(strncmp(aux,"close",5)==0){

      kill(pd1,SIGKILL);

      kill(pd2,SIGKILL);

      kill(pd3,SIGKILL);

      remove(FIFO_REQUEST);

      msgctl(msgid, IPC_RMID, NULL);

      mq_close(qd_server); 

      return(EXIT_SUCCESS);

    }

  }

}
 
  


