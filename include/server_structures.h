#ifndef AE85B30A_5A9D_4A51_9D65_392584DCC666
#define AE85B30A_5A9D_4A51_9D65_392584DCC666

#define BUF_SIZE 1024
#define SHM_KEY 0x1234
#define FIFO_REQUEST "requests.fifo"
#define MESSAGE_QUEUE_POSIX_FILE "./message_queue_posix.txt"
#define FIFO_FILE "./fifo.txt"
#define MESSAGE_QUEUE_FILE "./message_queue.txt"

#define SERVER_QUEUE_NAME   "/sp-example-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10


#endif /* AE85B30A_5A9D_4A51_9D65_392584DCC666 */


typedef struct {
  char client_fifo[BUF_SIZE];        // filename of FIFO on which to respond
  char query_name[BUF_SIZE];         // look up this person's email
}request_t;

// structure for message queue
struct mesg_buffer {
  long mesg_type;
  char mesg_text[100];
}message;

