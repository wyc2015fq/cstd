# NDK开发笔记（二）---JNI的学习 - xqhrs232的专栏 - CSDN博客
2011年05月12日 17:31:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1077标签：[java																[jni																[float																[byte																[class																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx](http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx)
一.JNI设计的目的
1.c/c++不同点在JNI
C 和 C++ 代码几乎相同；唯一的差异在于用在访问JNI函数的方法。
在C中，JNI函数调用由（*env)->作前缀，目的是为了取出函数指针所引用的值。
在C++中JNIEvn类拥有处理函数指针查找的内部成员函数
C 语法：jsize len = (*env)->GetArrayLength(env,array);
C++ 语法：jsize len =env->GetArrayLength(array);
2.解释下面的code
view plaincopy to clipboardprint?
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,   
                                                  jobject thiz )   
{   
    return (*env)->NewStringUTF(env, "Hello World from JNI!");   
}  
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello World from JNI!");
} 
JNIEnv * env--> JNI接口指针
JObject thiz-->相当于this指针
2.基本数据类型
Java C/C++ 字节数 
boolean jboolean 1 
byte jbyte 1 
char jchar 2 
short jshort 2 
int jint 4 
long jlong 8 
float jfloat 4 
double jdouble 8
对于数组型参数，
Java C/C++ 
boolean[ ] JbooleanArray 
byte[ ] JbyteArray 
char[ ] JcharArray 
short[ ] JshortArray 
int[ ] JintArray 
long[ ] JlongArray 
float[ ] JfloatArray 
double[ ] JdoubleArray 
对于上述类型数组，有一组函数与其对应。以下函数中 Xxx 为对应类型。 
xxx * GetXxxArrayElements(xxxArray array, jboolean *isCopy) 
产生一个指向 Java 数组元素的 C 指针。不再需要时，需将此指针传给 ReleaseXxxArrayElemes。
3.JNI相关操作
类的相关操作（获取类） 
  jclass FindClass(JNIEnv *env, const char *name);  查找类
 需要注意的是第二个参数为const char*类型的，我们如果从Java从层传入unicode编码的jstring类型需要使用GetStringUTFChars函数转换成utf8的const char*，记得后面ReleaseStringUTFChars 函数，如果成功返回这个Java类的对象jclass.
  jclass GetSuperclass(JNIEnv *env, jclass clazz);  获取父类或者说超类   
     该函数的第二个参数为jclass类，我们调用时传入的是子类，否则返回将是NULL
  jboolean IsAssignableFrom(JNIEnv *env, jclass clazz1,jclass clazz2);  判断class1对象能否安全的强制转换为class2对象
  jclass GetObjectClass(JNIEnv *env, jobject obj); 通过对象获取这个类
 jboolean IsInstanceOf(JNIEnv *env, jobject obj,jclass clazz); 判断对象是否为某个类的实例
调用Java方法 (call java的方法) 
       jmethodID GetMethodID(JNIEnv *env, jclass clazz,const char *name, const char *sig);  获取一个Java方法的ID
        这个函数将返回非静态类或接口实例方法的方法 ID
 NativeType CallXXXMethod (JNIEnv *env, jobject obj,jmethodID methodID, va_list args); 调用XXX类型的Java      方法
     执行Java类中某个方法，需要注意的是这个里的java类是非静态的，由于Java的方法的类型比较多，所以该函数可能有以下几种形式，    如   CallObjectMethod,CallBooleanMethod,CallByteMethod,CallCharMethod,CallShortMethod,CallIntMethod,CallLongMethod,CallFloatMethod,CallDoubleMethod和CallVoidMethod,
va_list args是代表参数列表。
访问Java对象的域 
  Java对象的域或者说字段、属性(Field) 类似方法的执行
    1. jfieldID GetFieldID(JNIEnv *env, jclass clazz,const char *name, const char *sig);  获取实例对象的域ID
    2. NativeType GetXXXField(JNIEnv *env, jobject obj,jfieldID fieldID);
    类似GetXXXMethod函数，可能有的类型有 GetObjectField,GetBooleanField,GetByteField,GetCharField,GetShortField,GetIntField,GetLongField,GetFloatField,GetDoubleField。
    3. void SetXXXField(JNIEnv *env, jobject obj, jfieldID fieldID,NativeType value);
    Java的域可以赋值的，可能有的类型有 SetObjectField,SetBooleanField,SetByteField,SetCharField,SetShortField,SetIntField,SetLongField,SetFloatField,SetDoubleField
    4.jfieldID GetStaticFieldID(JNIEnv *env, jclass clazz,const char *name, const char *sig);
    5. NativeType GetStaticXXXField(JNIEnv *env, jclass clazz,jfieldID fieldID);
    6. void SetStaticXXXField(JNIEnv *env, jclass clazz,jfieldID fieldID, NativeType value);
4.Java层传入的String不能在JNI中直接转化为jstring，因为Java的内部编码为unicode，中英文的字符都是每个占两个字节，而jni中我们需要使用utf-8来表示，utf-8比较特殊中英文是不等长的，比如英文等符号的ascii占用一个字节，而中文则为三个字节，同时仍然以/n结尾
 而正确的转换unicode到utf-8方法为使用GetStringUTFChars这个函数:
 后面ReleaseStringUTFChars.
参考网站学习
[http://www.ibm.com/developerworks/cn/java/l-linux-jni/](http://www.ibm.com/developerworks/cn/java/l-linux-jni/)
[http://dev.10086.cn/cmdn/bbs/redirect.php?tid=33663&goto=lastpost](http://dev.10086.cn/cmdn/bbs/redirect.php?tid=33663&goto=lastpost)
[http://www.android123.com.cn/androidkaifa/686.html](http://www.android123.com.cn/androidkaifa/686.html)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx](http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx](http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx](http://blog.csdn.net/wong_judy/archive/2011/03/15/6250110.aspx)

