#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>

void *thread_fun1(void *message);

void *thread_fun2(void *message);

void *thread_fun3(void *message);

void *thread_fun4(void *message);

extern "C"
void
Java_com_cloudups_thread_MainActivity_createThread(JNIEnv *env, jobject instance) {

    int error;
    void *ret1;
    void *ret2;
    void *ret3;

    //创建第一个线程
    pthread_t pthread1;
    error = pthread_create(&pthread1, NULL, thread_fun1, (void *) "this is a new thread ");

    //判断线程创建成功与否
    if (error == 0) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 1 success");
        //等待子线程返回
        pthread_join(pthread1, &ret1);//调用此函数，主线程会阻塞在这个地方直到子线程返回
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 1 fail");


    //创建第二个线程
    pthread_t pthread2;
    error = pthread_create(&pthread2, NULL, thread_fun2, (void *) "this is a new thread ");

    //判断第二个线程创建成功与否
    if (error == 0) {
        //等待子线程返回
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 2 success");
        pthread_join(pthread2, &ret2);
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 2 fail");


    //创建第三个线程
    pthread_t pthread3;
    error = pthread_create(&pthread3, NULL, thread_fun3, (void *) "this is a new thread ");

    //判断第三个线程创建成功与否
    if (error == 0) {
        //等待子线程返回
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 3 success");
        pthread_join(pthread3, &ret3);
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 3 fail");

    //创建第四个线程
    pthread_t pthread4;
    error = pthread_create(&pthread4, NULL, thread_fun4, (void *) "this is a new thread ");

    //判断第四个线程创建成功与否
    if (error == 0) {
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 4 success");
    }
    else
        __android_log_print(ANDROID_LOG_INFO, "jni", "create thread 4 fail");


    //
    __android_log_print(ANDROID_LOG_INFO, "jni",
                        "this is main thread: pid = %d,tid = %d,thread1 ret: %d,thread2 ret: %d,thread3 ret: %d",
                        getppid(), gettid(), ret1, ret2, ret3);
    return;
}

void *thread_fun1(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    __android_log_print(ANDROID_LOG_INFO, "jni", "%s 2,pid = %d,tid = %d", message, pid, tid);

    //子线程正常结束
    return (void *) 1;
}


void *thread_fun2(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    __android_log_print(ANDROID_LOG_INFO, "jni", "%s 3,pid = %d,tid = %d", message, pid, tid);

    //调用中介子线程方法，子线程非正常结束
    pthread_exit((void *) 2);
}

void *thread_fun3(void *message) {

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    for (int i = 0; i < 3; i++) {

        __android_log_print(ANDROID_LOG_INFO, "jni", "%s 3,pid = %d,tid = %d", message, pid, tid);
        sleep(1);
    }


    //调用中介子线程方法，子线程非正常结束
    pthread_exit((void *) 3);
}

void *thread_fun4(void *message) {

//    pthread_detach(pthread_self());

    pid_t pid;
    pthread_t tid;
    pid = getppid();
    tid = gettid();

    for (int i = 0; i < 3; i++) {

        __android_log_print(ANDROID_LOG_INFO, "jni", "%s 4,pid = %d,tid = %d", message, pid, tid);
        sleep(1);
    }


    //调用中介子线程方法，子线程非正常结束
    pthread_exit((void *) 3);
}