# JNI在C和C++中的调用区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月27日 17:46:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：226
C-style JNI looks like (*env)->SomeJNICall(env, param1 ...)
C++ style JNI looks like env->SomeJNICall(param1 ...)
C代码: (*env)->GetStringUTFChars(env, string, 0);
C++代码: env->GetStringUTFChars(string, 0);
C语言中使用的是结构体的函数指针, 而在C++中使用的还是struct, 我们知道struct在C++中和class的功能是几乎一样的, struct也可以用来定义类, 所以env在C++中是个类对象的指针.
## 艺搜参考
[http://stackoverflow.com/questions/935379/jni-calls-different-in-c-vs-c](http://stackoverflow.com/questions/935379/jni-calls-different-in-c-vs-c)
[http://www.cnblogs.com/icejoywoo/archive/2012/02/22/2363709.html](http://www.cnblogs.com/icejoywoo/archive/2012/02/22/2363709.html)
