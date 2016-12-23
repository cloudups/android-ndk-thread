#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>

void *thread_fun(void *message);

extern "C"
void
Java_com_cloudups_thread_MainActivity_createThread(JNIEnv *env, jobject instance) {

    pthread_t pthread;
    int ret = pthread_create(&pthread, NULL, thread_fun, (void *) "this is a new thread: ");

    if (ret == 0)
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread success");
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread fail");

    __android_log_print(ANDROID_LOG_INFO, "jni", "this is main thread: pid = %d,tid = %d",
                        getppid(), gettid());

    sleep(1);
    return;
}

void *thread_fun(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    __android_log_print(ANDROID_LOG_INFO, "jni", "%s,pid = %d,tid = %d", message, pid, tid);

    return NULL;
}
