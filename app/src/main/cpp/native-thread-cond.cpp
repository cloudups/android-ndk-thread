#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>

struct msg {
    struct msg *next;
    int num;
};

struct msg *head;

//定义并初始化mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//定义并初始化cond
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *p) {
    struct msg *mp;
    for (int i = 0; i < 20; i++) {
        mp = (msg *) malloc(sizeof(struct msg));
        mp->num = rand() % 1000 + 1;

        //上锁
        pthread_mutex_lock(&lock);

        //begin 临界区操作
        mp->next = head;
        head = mp;
        __android_log_print(ANDROID_LOG_INFO, "jni", "produce: %d", mp->num);
        //end 临界区操作

        //解锁
        pthread_mutex_unlock(&lock);

        //条件变量通知在该信号量上等待的线程
        pthread_cond_signal(&cond);

        sleep(rand() % 2);
    }

    return NULL;
}

void *consumer(void *p) {

    struct msg *mp;
    for (int i = 0; i < 20; i++) {
        //上锁
        pthread_mutex_lock(&lock);
        while (head == NULL)
            //在该条件变量上等待，个人理解：该线程先解锁，然后当条件变量通知该线程时，再上锁，等同于解锁->阻塞等待->接收通知->上锁
            pthread_cond_wait(&cond, &lock);

        //begin 临界区操作
        mp = head;
        head = mp->next;
        __android_log_print(ANDROID_LOG_INFO, "jni", "consume: %d", mp->num);
        free(mp);
        //end 临界区操作

        //解锁
        pthread_mutex_unlock(&lock);

        sleep(rand() % 2);
    }
    return NULL;
}

extern "C"
void Java_com_cloudups_thread_MainActivity_cond(JNIEnv *env, jobject instance) {

    pthread_t pt, ct;

    pthread_create(&pt, NULL, producer, NULL);
    pthread_create(&ct, NULL, consumer, NULL);

    pthread_join(pt, NULL);
    pthread_join(ct, NULL);

    return;
}




