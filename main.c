#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
 *      _____P4____
 *     /           \
 *   P3------P5-----P8
 *     \_____P6____/
 */

pthread_mutex_t mutex;
pthread_cond_t cond_p3_done, cond_p4p5p6_done;

void simulate_job(int seconds, char process) {
    while (seconds) {
        printf("(%ds) Working from P%c...\n", seconds, process);
        seconds--;
        sleep(1);
    }
    printf("(%ds) Working from P%c...\n", seconds, process);
}

void *P3(void *arg) {
    printf("P3: Starting\n");
    simulate_job(3, '3'); // Simulate P3 work 3sec
    printf("P3: Done\n");
    pthread_cond_signal(&cond_p3_done);
    return NULL;
}

void *P4(void *arg) {
    printf("P4: Starting\n");
    simulate_job(5, '4'); // Simulate P4 work 5sec
    printf("P4: Done\n");
    pthread_cond_signal(&cond_p4p5p6_done);
    return NULL;
}

void *P5(void *arg) {
    printf("P5: Starting\n");
    simulate_job(3, '5'); // Simulate P5 work 3sec
    printf("P5: Done\n");
    pthread_cond_signal(&cond_p4p5p6_done);
    return NULL;
}

void *P6(void *arg) {
    printf("P6: Starting\n");
    simulate_job(10, '6'); // Simulate P6 work 10sec
    printf("P6: Done\n");
    pthread_cond_signal(&cond_p4p5p6_done);
    return NULL;
}

void *P8(void *arg) {
    printf("P8: Starting\n");
    simulate_job(2, '8'); // Simulate P8 work 2sec
    printf("P8: Done\n");
    return NULL;
}

int main() {
    pthread_t p3, p4, p5, p6, p8;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_p3_done, NULL);
    pthread_cond_init(&cond_p4p5p6_done, NULL);

    pthread_create(&p3, NULL, P3, NULL);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond_p3_done, &mutex);
    pthread_mutex_unlock(&mutex);

    pthread_join(p3, NULL);

    pthread_create(&p4, NULL, P4, NULL);
    pthread_create(&p5, NULL, P5, NULL);
    pthread_create(&p6, NULL, P6, NULL);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond_p4p5p6_done, &mutex);
    pthread_mutex_unlock(&mutex);

    pthread_join(p4, NULL);
    pthread_join(p5, NULL);
    pthread_join(p6, NULL);

    pthread_create(&p8, NULL, P8, NULL);
    pthread_join(p8, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_p3_done);
    pthread_cond_destroy(&cond_p4p5p6_done);

    return 0;
}
