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
    ThreadInfo *tinfo = (ThreadInfo *) param;
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

    char inputNoSpaces[BUFFERLEN];
    int buf_idx = 0;
    for (int i = 0; i < input; i++){
        if (input[i] != " ") {
            inputNoSpaces[buf_idx] = input[i];
            buf_idx++;
        }
    }
    printf("(P) got '%s'\n", inputNoSpaces);
    pthread_mutex_lock(&tinfo->lock);
    if (strlen(tinfo->buf) == 0){
        for (int i = 0; i < inputNoSpaces; i++){
            tinfo->buf[i] = input[i];
        }
        tinfo -> dataReady = 1;
    }
    printf("(P) wrote '%s' to buffer\n", inputNoSpaces);
    pthread_mutex_unlock(&tinfo->lock);

    pthread_mutex_lock(&tinfo->lock);
    if (tinfo->responseReady){

    }

}

void *consumer(void *param){
    ThreadInfo *tinfo = (ThreadInfo *) param;
    pthread_mutex_lock(tinfo->lock);
    while(tinfo->dataReady == 1){

    }

}



int main() {
    ThreadInfo tinfo;
    pthread_t producerTid, consumerTid;

    pthread_mutex_init(&tinfo.lock, NULL);

    tinfo.dataReady = 0;
    tinfo.reponseReady = 0;
    tinfo.done = 0;

    pthread_create(&producerTid, NULL, producer, &tinfo);
    pthread_create(&consumerTid, NULL, consumer, &tinfo);

    pthread_join(producerTid, NULL);
    pthread_join(consumerTid, NULL);
    return 0;

    tinfo->buf =
    printf("Hello, World!\n");
    return 0;
}
