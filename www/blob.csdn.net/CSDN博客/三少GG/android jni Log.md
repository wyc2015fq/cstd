# android jni Log - 三少GG - CSDN博客
2011年12月23日 10:48:45[三少GG](https://me.csdn.net/scut1135)阅读数：1494标签：[android																[jni																[shell																[java																[null																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
[http://hi.baidu.com/underalight/blog/item/c2d3d0f1742917b1a50f526f.html](http://hi.baidu.com/underalight/blog/item/c2d3d0f1742917b1a50f526f.html)
#define LOG_TAG    "jnilog" // 这个是自定义的LOG的标识，可用来定位
#undef LOG // 取消默认的LOG
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) // 定义LOG类型
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__) // 定义LOG类型
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__) // 定义LOG类型
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__) // 定义LOG类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__) // 定义LOG类型
最后再Androi.mk文件中加上：
LOCAL_LDLIBS := -llog
C文件中加入
#include "utils/Log.h"  //这个是在android源码环境使用
#include <android/log.h>//这个在NDK编译时使用
++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://blog.csdn.net/wenmang1977/article/details/6916222](http://blog.csdn.net/wenmang1977/article/details/6916222)
Android Native C Log
//Rocky@20110524
我们在做Android java的时候，为了调试我们经常使用Log.i,Log.d等等log函数，这样非常方便我们调试使用。但是
如果是我们在使用Native C在做的时候，实际上也是可以利用LOG来输出调试信息的。查看方式一样。
有两种方式：
方式一：
    包含#include <utils/Log.h>
    我们查看这个文件system/core/include/cutils/log.h
    在开头发现
    #ifndef LOG_TAG
    #define LOG_TAG NULL
    #endif
    对于这个词我们的敏感度，我们知道，需要设置TAG信息。
    那么我们可肯定是需要在使用这个log.h前，定义一个LOG_TAG
    那么在Native C中我们常用的做法如下：
    #define LOG_TAG "kris_ril"
    #include <utils/Log.h>
    然后使用LOGI，LOGD，LOGV，LOGW，LOGE来输出log信息，例如：
    LOGI("Rocky:requestRadioPower=%d/n",onOff);
    然后在adb shell 中使用
    logcat -s kris_ril 
    即可查看到输入结果。
方式二：
    第一步：在对应的mk文件中加入:LOCAL_LDLIBS := -llog
    第二步：在要使用LOG的cpp文件中加入：
    #include <android/log.h>
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "keymatch", __VA_ARGS__)
    第三步：这样就可以使用了：LOGD("我要看到的调试信息^_^");
    这样，在logcat端看到的输出是：
    D/keymatch（ 32）:我要看到的调试信息^_^
    如果想改变输出中的各项内容，可以参考相应颜色的标示，比如，如果想定义LOGE，就可以把上面的ANDROID_LOG_DEBUG改成ANDROID_LOG_ERROR，同理，LOGI什么的也都以此类推：
    #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ProjectName", __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG ,  "ProjectName", __VA_ARGS__)
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO  ,  "ProjectName", __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN  ,  "ProjectName", __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "ProjectName", __VA_ARGS__)
    当然，如果不嫌麻烦，也可以直接使用__android_log_print函数，而不define定义LOGxxx    
记录了以上两种方法，方便查阅。   
参考文献：
[Android在JNI中输出输出LOG](http://hi.baidu.com/theonlyking/blog/item/58dcc61373a98ac8f7039ebc.html)
