# Android Service实现详细过程 - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:21:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：538
原文地址::[http://blog.csdn.net/andy_android/article/details/7301814](http://blog.csdn.net/andy_android/article/details/7301814)
一、Android Service介绍
Android的Service分为两种：Android Service和Native Service。
Android Service：又称为Java Service，是实现在框架层（framework）里的Server。Android Service以Java编写。
Native Service：又称为System Service，是实现在Runtime层里的Server。
以MediaPlayer 为例，从下图我们可以得出两种服务的关系：
![](http://blogimg.chinaunix.net/blog/upfile2/100304133105.jpg)
接下来要讨论的Service是Native Service，与应用程序设计上所讨论的Service（android.app.Service）不同。
二、为什么要写底层的核心服务呢？
（1） 因为底层核心服务是Android 框架里最接近Linux/Driver 的部分。为了充分发挥硬件设备的差异化特性，核心服务是让上层Java 应用程序来使用Driver/HW Device 特色的重要管道。
（2） 在开机过程中，就可以启动核心服务(例如汉字输入法服务等)，让众多应用程序来共享之。
（3） 由于共享，所以能有效降低Java 应用程序的大小(Size)。
三、如何实现一个核心服务呢?
要点如下：
（1）核心服务通常在独立的进程(Process)里执行。
（2）必须提供IBinder 接口，让应用程序可以进行跨进程的绑定(Binding)和呼叫。
（3）因为共享，所以必须确保多线裎安全(Thread-safe)。
（4）以C++类别定义，诞生其对象，透过SM 之协助，将该对象参考值传给IServiceManager::addService()函数，就加入到Binder Driver 里了。
（5）应用程序可透过SM 之协助而远距绑定该核心服务，此时SM 会回传IBinder 接口给应用程序。
（6）应用程序可透过IBinder::transact()函数来与核心服务互传数据。
四、Server实现实践
下面以一个小例子来说明具体实现一个Server的步骤。此实例功能为简单的整数加法(Add)运算，我们将其命名AddService。
Step-1：以C++撰写AddService 类别，其完整程序代码为：
AddService.h文件：
#ifndef ANDROID_GUILH_ADD_SERVICE_H
#define ANDROID_GUILH_ADD_SERVICE_H
#include <utils.h>
#include <utils/RefBase.h>
#include <utils/IInterface.h>
#include <utils/Parcel.h>
namespace android
{
       class AddService : public BBinder
       {
              mutable Mutex mLock;
              int32_t mNextConnId;
              public: 
              static int instantiate();
              AddService();
              virtual ~AddService();
              virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t);
       };
}
#endif
AddService.cpp文件：
#include "AddService.h"
#include <utils/IServiceManager.h>
#include <utils/IPCThreadState.h>
namespace android {
       static struct sigaction oldact;
       static pthread_key_t sigbuskey;
       int AddService::instantiate() {
              LOGE("AddService instantiate");
              int r = defaultServiceManager()->addService(
                            String16("guilh.add"), new AddService());
              LOGE("AddService r = %d/n", r);
              return r;
       }
       AddService::AddService()
       { LOGV("AddService created");
              mNextConnId = 1;
              pthread_key_create(&sigbuskey, NULL);
       }
       AddService::~AddService()
       { pthread_key_delete(sigbuskey);
              LOGV("AddService destroyed");
       }
       status_t AddService::onTransact(
                     uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags){
              switch(code) {
                     case 0: {
                                   pid_t pid = data.readInt32();
                                   int num = data.readInt32();
                                   num = num + 1000;
                                   reply->writeInt32(num);
                                   return NO_ERROR;
                            } break;
                     default:
                            return BBinder::onTransact(code, data, reply, flags);
              }
       }
};
Android.mk文件：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= AddService.cpp
#LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)    
LOCAL_SHARED_LIBRARIES:= libutils
LOCAL_MODULE:= libAddService
LOCAL_PRELINK_MODULE:= false
include $(BUILD_SHARED_LIBRARY)
Step-2：以C++撰写一个可独立执行的addserver.cpp 程序，它的用途是：诞生一个AddService 类别之对象，然后将该对象参考存入Binder Driver 里。其内容为：
addserver.cpp文件：
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <utils/IPCThreadState.h>
#include <utils/ProcessState.h>
#include <utils/IServiceManager.h>
#include <utils/Log.h>
#include <private/android_filesystem_config.h>
#include "../libaddservice/AddService.h"
using namespace android;
int main(int argc, char** argv)
{
sp<ProcessState> proc(ProcessState::self());
sp<IServiceManager> sm = defaultServiceManager();
LOGI("ServiceManager: %p", sm.get());
AddService::instantiate();
ProcessState::self()->startThreadPool();
IPCThreadState::self()->joinThreadPool();
}
Android.mk文件：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= addserver.cpp
LOCAL_SHARED_LIBRARIES:= libutils libAddService
LOCAL_MODULE:= addservice
include $(BUILD_EXECUTABLE)
Step-3：编译上述两个文件分别产出了libAdd.so 类别库和addserver 可执行程序。接着将libAdd.so 拷贝到Android 仿真器的/system/lib/里；也把addserver拷贝到/system/bin/里。
Step-4：执行addserver。其中的指令：AddServer::instantiate()就执行到AddServer 类别的instantiate()函数，其内容为：
int AddService::instantiate() {
LOGE("AddService instantiate");
int r = defaultServiceManager()->addService(
String16("guilh.add"), new AddService());
LOGE("AddService r = %d/n", r);
return r;
}
其先执行到new AddServer()，就诞生一个AddServer 类别之对象；
接着，呼叫defaultServiceManager()函数取得SM 的IServiceManager 接口；
再呼叫IServiceManager::addServer()将该对象参考存入Binder Driver 里。
Step-5：这样就成功地将AddService 服务加入到Binder Driver 里了。现在就可以写个Add 类来使用AddService 核心服务了。以C++撰写Add 类别，其完整程序代码为：
Add.h文件：
#ifndef ANDROID _ADD_H
#define ANDROID _ADD_H
namespace android {
       class Add {
              public:
                     int setN(int n);
              private:
                     static const void getAddService();
       };
}; //namespace
#endif // ANDROID _ADD_H
Add.cpp文件：
#include <utils/IServiceManager.h>
#include <utils/IPCThreadState.h>
#include "Add.h"
namespace android {
       sp<IBinder> binder;
       int Add::setN(int n){
              getAddService();
              Parcel data, reply;
              data.writeInt32(getpid());
              data.writeInt32(n);
              LOGE("BpAddService::create remote()->transact()/n");
              binder->transact(0, data, &reply);
              int i = reply.readInt32();
              return i;
       }
       const void Add::getAddService(){
              sp<IServiceManager> sm = defaultServiceManager();
              binder = sm->getService(String16("guilh.add"));
              LOGE("Add::getAddService %p/n",sm.get());
              if (binder == 0) {
                     LOGW("AddService not published, waiting...");
                     return;
              }
       }
};
Android.mk文件：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=Add.cpp
LOCAL_SHARED_LIBRARIES := libutils libAddService
LOCAL_MODULE := libAdd
LOCAL_PRELINK_MODULE:= false
include $(BUILD_SHARED_LIBRARY)
Step-6：下面写个JNI Native 类别来使用Add 类别之对象。透过JNI Native 函数，就可以与Java 层的Service 服务衔接起来。
首选使用javah命令生成相应头文件。
com_hello_Service_MySer.h文件：
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_hello_Service_MySer */
#ifndef _Included_com_hello_Service_MySer
#define _Included_com_hello_Service_MySer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_hello_Service_MySer
 * Method:    intFromJNI
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hello_Service_MySer_intFromJNI
  (JNIEnv *, jobject);
#ifdef __cplusplus
}
#endif
#endif
然后实现相应函数。
com_hello_Service_MySer.cpp文件：
#include <jni.h>
#include <JNIHelp.h>
#include "../libadd/Add.h"
#include "com_hello_Service_MySer.h"
JNIEXPORT jint JNICALL Java_com_hello_Service_MySer_intFromJNI(JNIEnv * env, jobject thiz)
{
    android::Add myadd;
       int r = myadd.setN(5);
       return r;
}
Android.mk文件：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=com_hello_Service_MySer.cpp
LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)    
LOCAL_SHARED_LIBRARIES := libutils libAdd
LOCAL_MODULE := libJniAdd
LOCAL_PRELINK_MODULE:= false
include $(BUILD_SHARED_LIBRARY)
所有相关文件组织：
a@ubuntu:~/work/android/source_android/development/my_module$ tree service
service
|-- addserver
|   |-- Android.mk
|   `-- addserver.cpp
|-- jni
|   |-- Android.mk
|   |-- com_hello_Service_MySer.cpp
|   `-- com_hello_Service_MySer.h
|-- libadd
|   |-- Add.cpp
|   |-- Add.h
|   `-- Android.mk
`-- libaddservice
    |-- AddService.cpp
    |-- AddService.h
    `-- Android.mk
4 directories, 11 files
在Eclipse中创建一个工程使用以上的Add类，即可使用我们的AddService了。
MySer.java文件：
package com.hello.Service;
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
public class MySer extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        int i = intFromJNI();
        TextView  tv = new TextView(this);
        tv.setText( String.valueOf(i) );
        setContentView(tv);
    }
    public native int intFromJNI();
static {
        System.loadLibrary("JniAdd");
    }
}
五、Service编译问题
Android所用的Toolchain（即交叉编译工具链）可从下面的网址下载：
http://android.kernel.org/pub/android-toolchain-20081019.tar.bz2。如果下载了完整的Android项目的源代码，则可以在“<your_android>/prebuilt/linux-x86/toolchain/arm-eabi-4.2.1/bin”目录下找到交叉编译工具，比如Android所用的arm-eabi-gcc-4.2.1。我们可以直接使用源码包来进行编译。
Android并没有采用glibc作为C库，而是采用了Google自己开发的Bionic Libc，它的官方Toolchain也是基于Bionic Libc而并非glibc的。这使得使用或移植其他Toolchain来用于Android要比较麻烦：在Google公布用于Android的官方Toolchain之前，多数的Android爱好者使用的Toolchain是在http://www.codesourcery.com/gnu_toolchains/arm/download.html 下载的一个通用的Toolchain，它用来编译和移植Android
 的Linux内核是可行的，因为内核并不需要C库，但是开发Android的应用程序时，直接采用或者移植其他的Toolchain都比较麻烦，其他Toolchain编译的应用程序只能采用静态编译的方式才能运行于Android模拟器中，这显然是实际开发中所不能接受的方式。目前尚没有看到说明成功移植其他交叉编译器来编译Android应用程序的资料。
与glibc相比，Bionic Libc有如下一些特点：
l         采用BSD License，而不是glibc的GPL License；
l         大小只有大约200k，比glibc差不多小一半，且比glibc更快；
l         实现了一个更小、更快的pthread；
l         提供了一些Android所需要的重要函数，如”getprop”, “LOGI”等；
l         不完全支持POSIX标准，比如C++ exceptions，wide chars等；
l         不提供libthread_db 和 libm的实现
 另外，Android中所用的其他一些二进制工具也比较特殊：
加载动态库时使用的是/system/bin/linker而不是常用的/lib/ld.so;
prelink工具不是常用的prelink而是apriori，其源代码位于“<your_android>/build/tools/apriori”
strip工具也没有采用常用的strip，而是“<your_android>/prebuilt/linux-x86/toolchain/arm-eabi-4.2.1/bin”目录下的arm-eabi-strip，而是位于<your_android>/out/host/linux-x86/bin/的soslim工具。
下面就具体说一下如何编译我们刚才创建的Service程序。
1. 在$(YOUR_ANDROID)/development 目录下创建my_module目录，然后将我们的server文件夹拷贝到此目录下，其中$(YOUR_ANDROID)指Android源代码所在的目录。
 # mkdir $(YOUR_ANDROID)/development/ my_module 
2. Android.mk这是Android Makefile的标准命名，不要更改。Android.mk文件的格式和内容可以参考其他已有的Android.mk文件的写法，针对Add程序的Android.mk文件内容如下：
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=Add.cpp
LOCAL_SHARED_LIBRARIES := libutils libAddService
LOCAL_MODULE := libAdd
LOCAL_PRELINK_MODULE:= false
include $(BUILD_SHARED_LIBRARY)
注意上面LOCAL_SRC_FILES用来指定源文件；，LOCAL_MODULE指定要编译的模块的名字，下一步骤编译时就要用到；include $(BUILD_SHARED_LIBRARY)表示要编译成动态库，如果想编译成一个可执行文件则可用BUILD_EXECUTABLE，这些可以在$(YOUR_ANDROID)/build/core/config.mk查到。
3. 回到Android源代码顶层目录进行编译：
# cd $(YOUR_ANDROID) && make libAdd
注意make libAdd中的目标名libAdd就是上面Android.mk文件中由LOCAL_MODULE指定的模块名。
4．编译后的可执行文件存放在通过”adb push”将它传送到模拟器上，再通过”adb shell”登录到模拟器终端，就可以执行了。
六、出现问题及及解决办法：
（1）提示缺bison，安装bison：sudo apt-get install bison
（2）出现frameworks/policies/base/PolicyConfig.mk:22: *** No module defined for the given PRODUCT_POLICY (android.policy_phone).  Stop.错误。
解决办法：
在build/tools/findleaves.sh中的第89行，
这一句find "${@:0:$nargs}" $findargs -type f -name "$filename" -print |
改为find "${@:1:$nargs-1}" $findargs -type f -name "$filename" -print |
