# FFmpeg(13)-创建OpenSLES混音器CreateOutputMix初始化 - 夜行过客 - 博客园







# [FFmpeg(13)-创建OpenSLES混音器CreateOutputMix初始化](https://www.cnblogs.com/yongdaimi/p/9836357.html)





**一.包含头文件和库文件**

CMakeLists

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       OpenSLES

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```

代码

```
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
```

**二.示例代码**

```
#include <jni.h>
#include <string>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


#define LOG_TAG "xp.chen"
#ifdef LOG_TAG
    #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
    #define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#else
    #define LOGI(...)
    #define LOGE(...)
    #define LOGV(...)
#endif


static SLObjectItf engineSL = NULL;
SLEngineItf CreateSL()
{
    SLresult ret;
    SLEngineItf en;
    // 创建引擎
    ret = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("slCreateEngine() failed ");
        return NULL;
    }
    // 实例化
    ret = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("Realize failed");
        return NULL;
    }
    // 获取接口
    ret = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("GetInterface failed");
        return NULL;
    }
    return en;
}


extern "C" JNIEXPORT jstring
JNICALL
Java_com_yuneec_yongdaimi_testopensl_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    // 创建引擎
    SLEngineItf eng = CreateSL();
    if (eng) {
        LOGI("CreateSL success");
    } else {
        LOGE("CreateSL failed");
    }
    // 创建混音器
    SLObjectItf mix = NULL;
    SLresult ret = 0;
    ret = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("CreateOutputMix failed");
    }
    ret = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (ret != SL_BOOLEAN_FALSE) {
        LOGE("(*mix)->Realize failed!");
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};

    return env->NewStringUTF(hello.c_str());
}
```














