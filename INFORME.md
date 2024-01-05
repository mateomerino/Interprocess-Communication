**DESARROLLO DEL TRABAJO PRÁCTICO NRO 1**

Se implementó una arquitectura básica de *Cliente-Servidor* en lenguaje C utilizando distintos mecanismos
IPC para comunicar los procesos que intervienen.

El primer paso fue dividir los programas en 4 archivos .c: 

-*server.c*: Este archivo corresponde al servidor. El mismo se encarga de administrar las peticiones de los
distintos tipos de clientes; respondiendo a dichas solicitudes. Se pensó que el servidor debe proveer los canales
de comunicación "abiertos" y disponibles para cualquier cliente que intente comunicarse con él. Por esto, el servidor
se encarga de crear las variables para gestionar los canales de comunicación y dejar procesos "corriendo" para 
atender los mensajes de los clientes. En líneas generales; crea tres hijos correspondientes a cada tipo de cliente,
el cliente 1, el 2 y el 3.

-*client1.c*: Este archivo corresponde al cliente 1. El mismo trabaja con el mecanismo de comunicación FIFO. Al
ejecutarse, intenta abrir el canal de comunicación FIFO que el server creó y se comunica con él.

-*client2.c*: Este archivo corresponde al cliente 2. El mismo trabaja con el mecanismo de comunicación Message Queue. Al ejecutarse, intenta abrir la cola de mensajes que el server creó y se comunica con él.

-*client3.c*: Este archivo corresponde al cliente 3. El mismo trabaja con el mecanismo de comunicación POSIX Message
Queue. Al ejecutarse, intenta abrir el canal de comunicación POSIX Message Queue que el server creó y se comunica con él.

Durante el desarrollo del proyecto se encontraron diversas dificultades. En un primer momento se intentó establecer
el mecanismo IPC "Shared Memory" pero al tener complicaciones con la implementación, se decidió descartarlo y 
continuar con las otras opciones que estaban a disposición

El servidor interactúa con cada tipo de cliente y los atiende en tiempo de ejecución sin problemas, almacenando
cada string que los clientes envían en un archivo distinto para cada cliente.

Los datos enviados al servidor pueden verse en los archivos "fifo.txt", "message_queue_posix.txt" y "message_queue.txt"
los cuales almacenan cada string de los *n* clientes instanciados que se comunican con él

Se obtuvieron los resultados deseados y el programa compila correctamente sin tener los errores que contemplan
las flags de compilación indicadas en la consigna.
