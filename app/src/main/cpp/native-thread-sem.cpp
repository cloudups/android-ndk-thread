#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

//信号量
sem_t full_sem, empty_sem;

//线程锁
pthread_mutex_t lock_sem;

//缓冲区数量
#define BUFFERSIZE 5

//缓冲区数据结构
struct msgbuf {
    pid_t id;
    time_t time;
};

//缓冲区数组
struct msgbuf msg_sem[BUFFERSIZE];

int in_sem = 0, out_sem = 0;

/**
 * 生产者
 */
void *producer_sem(void *) {

    __android_log_print(ANDROID_LOG_INFO, "jni", "produce begin");

    for (int i = 0; i < 10; i++) {
        //消费信号量empty，empty = empty - 1；
        sem_wait(&empty_sem);

        //上锁
        pthread_mutex_lock(&lock_sem);

        //begin 临界区操作
        msg_sem[in_sem].id = pthread_self();
        time(&msg_sem[in_sem].time);
        in_sem = (++in_sem) % 5;
        __android_log_print(ANDROID_LOG_INFO, "jni", "produce: id = %d,time = %s,",
                            msg_sem[in_sem].id,
                            ctime(&(msg_sem[in_sem].time)));
        //end 临界区操作

        //解锁
        pthread_mutex_unlock(&lock_sem);

        //增加信号量full,full = full + 1;
        sem_post(&full_sem);

        sleep(rand() % 3);
    }
    return NULL;
}

/**
 * 消费者
 */
void *consumer_sem(void *) {

    __android_log_print(ANDROID_LOG_INFO, "jni", "consumer begin");

    for (int i = 0; i < 10; i++) {
        //消费信号量full，若 full <= 0，则阻塞本线程，否则 full = full - 1。
        sem_wait(&full_sem);

        //上锁
        pthread_mutex_lock(&lock_sem);

        //begin 临界区操作
        __android_log_print(ANDROID_LOG_INFO, "jni", "consume: id = %d,time = %s",
                            msg_sem[out_sem].id,
                            ctime(&(msg_sem[out_sem].time)));
        out_sem = (++out_sem) % 5;
        //end 临界区操作

        //解锁
        pthread_mutex_unlock(&lock_sem);

        //生产信号量empty，empty = empty + 1；
        sem_post(&empty_sem);

        sleep(rand() % 3 + 1);
    }
    return NULL;
}

extern "C"
void Java_com_cloudups_thread_MainActivity_sem(JNIEnv *env, jobject instance) {

    pthread_t pid1, pid2;
    pthread_t cid1, cid2;

    //初始化信号量
    sem_init(&full_sem, 0, 0);
    sem_init(&empty_sem, 0, 5);

    pthread_create(&pid1, NULL, producer_sem, NULL);
    pthread_create(&cid1, NULL, consumer_sem, NULL);
    pthread_create(&pid2, NULL, producer_sem, NULL);
    pthread_create(&cid2, NULL, consumer_sem, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(cid1, NULL);
    pthread_join(cid2, NULL);

    //销毁锁
    pthread_mutex_destroy(&lock_sem);

    //销毁信号量
    sem_destroy(&full_sem);
    sem_destroy(&empty_sem);
    return;
}