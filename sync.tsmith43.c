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
    int len;
    char *chp;
    char input[BUFFERLEN];
    pthread_mutex_lock(&tinfo->lock);
    printf("(P) Starts \n");
    tinfo->done = 0;
    while ( tinfo->done == 0 ) {
        printf("%s", PROMPT);
        chp = fgets(input, BUFFERLEN, stdin);
        if (chp != NULL) {
            len = strlen(input);
            if (input[len-1] == '\n') {
                input[len-1] = '\0';
                len = len - 1;
            }
            printf("(P) got '%s'\n", input);
            if ( ! strcmp(input, "quit") || ! strcmp(input, "exit") )
                tinfo->done = 1;
        } else {
            printf("error or end of file: exiting\n");
            return 0;
        }
        char inputNoSpaces[BUFFERLEN];
        int buf_idx = 0;
        for (int i = 0; i < strlen(input); i++){
            if (input[i] != ' ') {
                inputNoSpaces[buf_idx] = input[i];
                buf_idx++;
            }
        }

        //    printf("(P) got '%s'\n", inputNoSpaces);

        if (strlen(tinfo->buf) == 0){
            for (int i = 0; i < strlen(inputNoSpaces); i++){
                tinfo->buf[i] = inputNoSpaces[i];
            }
            tinfo -> dataReady = 1;
        }
        pthread_mutex_unlock(&tinfo->lock);

        printf("(P) wrote '%s' to buffer\n", inputNoSpaces);

        pthread_mutex_lock(&tinfo->lock);
        if (tinfo->reponseReady = 1){
            char response[BUFFERLEN];
            strcpy(response, tinfo->buf);
            printf("(P) response is '%s'\n", response);
        }
        for (int i = 0; i < strlen(tinfo->buf); i++){
            tinfo->buf[i] = '\0';
        }
    }
}

void *consumer(void *param){
    ThreadInfo *tinfo = (ThreadInfo *) param;
    pthread_mutex_lock(&tinfo->lock);
    while(tinfo->done == 0){
        char *response = "";
        if(tinfo->dataReady == 1) {
            printf("(C) read '%s'\n", tinfo->buf);
            char backwards[BUFFERLEN];
            int bkwd_idx = 0;
            for (int i = strlen(tinfo->buf) - 1; i >= 0; i--) {
                backwards[bkwd_idx] = tinfo->buf[i];
                bkwd_idx++;
            }
            backwards[bkwd_idx] = '\0';
            int pallindrome = 1;
            for (int i = 0; i < strlen(backwards); i++) {
                if (tinfo->buf[i] != backwards[i]) {
                    pallindrome = 0;
                }

            }
            for (int i = 0; i < strlen(tinfo->buf); i++){
                tinfo->buf[i] = '\0';
            }
            char yes[BUFFERLEN] = "yes";
            char no[BUFFERLEN] = "no";

            if (pallindrome){
                strcpy(tinfo->buf, yes);
                response = "yes";
//                printf("(C) sending back 'yes'\n");
            }
            else{
                strcpy(tinfo->buf, no);
                response = "no";
//                printf("(C) sending back 'no'\n");
            }
            tinfo->reponseReady = 1;
        }
        pthread_mutex_unlock(&tinfo->lock);
        printf("(C) Sending back '%s'\n", response);
        pthread_mutex_lock(&tinfo->lock);

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

    printf("Hello, World!\n");
    return 0;
}
