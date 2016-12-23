#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>

int count = 0;
//定义线程锁
pthread_mutex_t mutex;

void *thread_fun(void *message);

extern "C"
JNIEXPORT void JNICALL
Java_com_cloudups_thread_MainActivity_mutex(JNIEnv *env, jobject instance) {

    //初始化线程锁
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread_fun, (void *) 1);
    pthread_create(&tid2, NULL, thread_fun, (void *) 2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    __android_log_print(ANDROID_LOG_INFO, "jni", "count: %d", count);

    //销毁线程锁
    pthread_mutex_destroy(&mutex);
    return;
}

void *thread_fun(void *message) {

    int var;
    for (int i = 0; i < 10; i++) {

        //上锁
        pthread_mutex_lock(&mutex);

        var = count;
        sleep((long) message);
        var++;
        count = var;

        //解锁
        pthread_mutex_unlock(&mutex);

        __android_log_print(ANDROID_LOG_INFO, "jni", "tid: %d, count: %d", gettid(), count);
    }

    return NULL;
}