#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>

void *thread_fun1(void *message);

void *thread_fun2(void *message);

extern "C"
void
Java_com_cloudups_thread_MainActivity_createThread(JNIEnv *env, jobject instance) {

    int error;
    void *ret1;
    void *ret2;
    pthread_t pthread1;
    error = pthread_create(&pthread1, NULL, thread_fun1, (void *) "this is a new thread: ");

    if (error == 0) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 1 success");
        pthread_join(pthread1, &ret1);//调用此函数，主线程会阻塞在这个地方直到子线程返回
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 1 fail");


    pthread_t pthread2;
    error = pthread_create(&pthread2, NULL, thread_fun2, (void *) "this is a new thread: ");

    if (error == 0) {
        pthread_join(pthread2, &ret2);
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 2 success");
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 2 fail");

    __android_log_print(ANDROID_LOG_INFO, "jni",
                        "this is main thread: pid = %d,tid = %d,thread1 ret: %d,thread2 ret: %d",
                        getppid(), gettid(), ret1, ret2);

    return;
}

void *thread_fun1(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    sleep(7);
    __android_log_print(ANDROID_LOG_INFO, "jni", "%s,pid = %d,tid = %d", message, pid, tid);

    return (void *) 1;
}


void *thread_fun2(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    __android_log_print(ANDROID_LOG_INFO, "jni", "%s,pid = %d,tid = %d", message, pid, tid);

    pthread_exit((void *) 2);
}