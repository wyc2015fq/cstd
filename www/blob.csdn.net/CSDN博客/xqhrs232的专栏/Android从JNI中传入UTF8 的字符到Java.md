# Android从JNI中传入UTF8 的字符到Java - xqhrs232的专栏 - CSDN博客
2010年12月24日 14:38:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1138
原文地址::[http://blog.csdn.net/vrix/archive/2010/08/20/5825705.aspx](http://blog.csdn.net/vrix/archive/2010/08/20/5825705.aspx)
由于env->NewStringUTF支持的是 Modified UTF-8，所以这个API不能用来从JNI
　　中传入标准的UTF8字符到Java。
　　我们有两种解决方法：
　　Java代码
　　ReturnUTF8.java
　　view plaincopy to clipboardprint?
　　public class ReturnUTF8 {
　　static {
　　System.loadLibrary("ReturnUTF8Jni");
　　}
　　public static native byte[] method1();
　　public static native Object method2();
　　public static void main(String[] args) {
　　try {
　　byte[] buffer = ReturnUTF8.method1();
　　String s1 = new String(buffer, "UTF-8");
　　System.out.println("s1 = " + s1);
　　String s2 = ReturnUTF8.method2().toString();
　　System.out.println("s2 = " + s2);
　　} catch (java.io.UnsupportedEncodingException e) {
　　e.printStackTrace();
　　}
　　}
　　}
　　public class
　　ReturnUTF8 {
　　static {
　　System.loadLibrary("ReturnUTF8Jni");
　　}
　　public static native byte[] method1();
　　public static native Object method2();
　　public static void main(String[] args) {
　　try {
　　byte[] buffer = ReturnUTF8.method1();
　　String s1 = new String(buffer, "UTF-8");
　　System.out.println("s1 = " + s1);
　　String s2 = ReturnUTF8.method2().toString();
　　System.out.println("s2 = " + s2);
　　} catch (java.io.UnsupportedEncodingException e) {
　　e.printStackTrace();
　　}
　　}
　　}
　　# javac ReturnUTF8.java
　　# javah ReturnUTF8
　　-> 这里会生成一个ReturnUTF8.h头文件
　　C++代码
　　ReturnUTF8.cpp
　　view plaincopy to clipboardprint?
　　#include "ReturnUTF8.h"
　　JNIEXPORT jbyteArray JNICALL Java_ReturnUTF8_method1
　　(JNIEnv *env, jclass cls)
　　{
jbyte buffer[] = /* UTF8 encoding buffer */
　　jbyteArray bytes = env->NewByteArray(sizeof(buffer));
　　env->SetByteArrayRegion(bytes, 0, sizeof(buffer), buffer);
　　return bytes;
　　}
　　JNIEXPORT jbyteArray JNICALL Java_ReturnUTF8_method2
　　(JNIEnv *env, jclass cls)
　　{
　　jbyte buffer[] = /* UTF8 encoding buffer */
　　jbyteArray bytes = env->NewByteArray(sizeof(buffer));
　　env->SetByteArrayRegion(bytes, 0, sizeof(buffer), buffer);
　　jchar encoding_name[] = { 'U', 'T', 'F', '-', '8', '' };
　　jstring encoding = env->NewString(encoding_name, 5);
　　jclass theStringClass = env->FindClass("Ljava/lang/String;");
　　jmethodID mid = env->GetMethodID(theStringClass, "", "([BLjava/lang/String;)V");
　　jobject result = env->NewObject(theStringClass, mid, bytes, encoding);
　　return result;
　　}
　　#include
　　"ReturnUTF8.h"
　　JNIEXPORT jbyteArray JNICALL Java_ReturnUTF8_method1
　　(JNIEnv *env, jclass cls)
　　{
　　jbyte buffer[] = /* UTF8 encoding buffer */
　　jbyteArray bytes = env->NewByteArray(sizeof(buffer));
　　env->SetByteArrayRegion(bytes, 0, sizeof(buffer), buffer);
　　return bytes;
　　}
　　JNIEXPORT jbyteArray JNICALL Java_ReturnUTF8_method2
　　(JNIEnv *env, jclass cls)
　　{
　　jbyte buffer[] = /* UTF8 encoding buffer */
　　jbyteArray bytes = env->NewByteArray(sizeof(buffer));
　　env->SetByteArrayRegion(bytes, 0, sizeof(buffer), buffer);
　　jchar encoding_name[] = { 'U', 'T', 'F', '-', '8', '' };
　　jstring encoding = env->NewString(encoding_name, 5);
　　jclass theStringClass = env->FindClass("Ljava/lang/String;");
　　jmethodID mid = env->GetMethodID(theStringClass, "",
　　"([BLjava/lang/String;)V");
　　jobject result = env->NewObject(theStringClass, mid, bytes,
　　encoding);
　　return result;
　　}
　　# g++ -shared -I /usr/lib/jvm/java-1.5.0-sun/include -I /usr/lib/jvm/java-1.5.0-sun/include/linux ReturnUTF8.cpp -o libReturnUTF8Jni.so
　　# export LD_LIBRARY=.
　　# java ReturnUTF8
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/vrix/archive/2010/08/20/5825705.aspx](http://blog.csdn.net/vrix/archive/2010/08/20/5825705.aspx)
