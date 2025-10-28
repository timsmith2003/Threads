#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define PROMPT " >"
#define BUFFERLEN 256
typedef struct {
    char buf[BUFFERLEN]; // the buffer
    int dataReady; // whether the producer has put data in the shared buffer
    int reponseReady; // whether the consumer has put a reply in the shared buffer
    int done;   // whether we’re done
    pthread_mutex_t *lock; // a pointer to the lock
} ThreadInfo;

void *producer(void *param){
    ThreadInfo *data;
    data = (ThreadInfo *) param;
    int done, len;
    char *chp;
    char buffer[BUFFERLEN];
    done = 0;
    while ( ! done ) {
        printf("%s", PROMPT);
        chp = fgets(buffer, BUFFERLEN, stdin);
        if (chp != NULL) {
            len = strlen(buffer);
            if (buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
                len = len - 1;
            }
            printf("I read the string '%s'\n", buffer);
            if ( ! strcmp(buffer, "quit") || ! strcmp(buffer, "exit") )
                done = 1;
        } else {
            printf("error or end of file: exiting\n");
            return 0;

        }
    }

}

int main() {

    printf("Hello, World!\n");
    return 0;
}
