# Android4.0.3源码分析——开机流程之Zygote - SHINING的博客 - CSDN博客
2012年04月27日 10:40:53[snsn1984](https://me.csdn.net/snsn1984)阅读数：4043标签：[android																[jni																[socket																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[C++																[Android](https://blog.csdn.net/snsn1984/article/category/1814733)](https://blog.csdn.net/snsn1984/article/category/488621)
Zygote
Zygote启动是从
/frameworks/base/cmds/app_process/app_main.cpp
中的main()函数开始的。
启动JavaVM:
main()函数中有启动VM：
if(zygote) {
runtime.start("com.android.internal.os.ZygoteInit",
startSystemServer? "start-system-server" : "");
而runtime是AppRuntime的对象，同样在main()中：
AppRuntimeruntime;
而appRuntime是AndroidRuntime的子类，它的定义就在app_main.cpp中：
classAppRuntime : public AndroidRuntime
所以，这里的runtime.start（）其实调用的是AndroidRuntime::start().
下面对AndroidRuntime::start()进行分析：
/frameworks/base/core/jni/AndroidRuntime.cpp
start()函数中启动VM：
/* start the virtual machine */[JNIEnv](http://androidxref.com/source/s?defs=JNIEnv&project=frameworks)* [env](http://androidxref.com/source/s?refs=env&project=frameworks);**if** ([startVm](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#startVm)(&[mJavaVM](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#mJavaVM), &[env](http://androidxref.com/source/s?defs=env&project=frameworks)) != 0) {**return**;}[onVmCreated](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#onVmCreated)([env](http://androidxref.com/source/s?defs=env&project=frameworks));
其中startVm()函数和onVmCreated()函数也都在AndroidRuntime.cpp中。startVm()函数中通过有如下代码调用JNI创建JavaVM:/** Initialize the VM.** The JavaVM* is essentially per-process, and the JNIEnv* is per-thread.* If this call succeeds, the VM is ready, and we can start issuing* JNI calls.*/**if** ([JNI_CreateJavaVM](http://androidxref.com/source/s?defs=JNI_CreateJavaVM&project=frameworks)([pJavaVM](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#pJavaVM), [pEnv](http://androidxref.com/source/s?defs=pEnv&project=frameworks), &[initArgs](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#initArgs)) < 0) {[LOGE](http://androidxref.com/source/s?defs=LOGE&project=frameworks)("JNI_CreateJavaVM failed\n");**goto**[bail](http://androidxref.com/source/s?defs=bail&project=frameworks);}onVmCreated()函数其实是个空函数。**void**[AndroidRuntime](http://androidxref.com/source/xref/frameworks/base/core/jni/AndroidRuntime.cpp#AndroidRuntime)::[onVmCreated](http://androidxref.com/source/s?refs=onVmCreated&project=frameworks)([JNIEnv](http://androidxref.com/source/s?defs=JNIEnv&project=frameworks)* [env](http://androidxref.com/source/s?refs=env&project=frameworks)){// If AndroidRuntime had anything to do here, we'd have done it in 'start'.}同时在start()函数中调用startReg()函数注册JNI接口：if (startReg(env) < 0) {LOGE("Unable to register all android natives\n");return;}
然后在start()函数中：
env->CallStaticVoidMethod(startClass,startMeth, strArray);
调用com.android.internal.os.ZygoteInit中的main()函数。
/fameworks/base/core/java/com/android/internal/os/ZygoteInit.java
main()调用了：
registerZygoteSocket();//来注册Socket的Listen端口，用来接受请求。
preload();
startSystemServer();//启动SystemServer。
其中preload()函数定义如下：
**static void**[preload](http://androidxref.com/source/s?refs=preload&project=frameworks)() {[preloadClasses](http://androidxref.com/source/xref/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java#preloadClasses)();[preloadResources](http://androidxref.com/source/xref/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java#preloadResources)();}
它主要进行预加载类和资源，以加快启动速度。preload的class列表保存在/frameworks/base/preloaded-classes文件中。
经过以上步骤，Zygote就建立完成。
