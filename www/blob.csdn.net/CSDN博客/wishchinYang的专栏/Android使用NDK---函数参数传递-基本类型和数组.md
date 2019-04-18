# Android使用NDK---函数参数传递-基本类型和数组 - wishchinYang的专栏 - CSDN博客
2015年07月01日 16:41:03[wishchin](https://me.csdn.net/wishchin)阅读数：1798
参考链接：[http://www.cnblogs.com/luxiaofeng54/archive/2011/08/19/2145486.html](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/19/2145486.html)
数据传输可分为 基本数据类型传输 和 引用数据类型的传输 ， 因为数组传输也比较特别(其实数组也是引用类型)，所以这里也专门分出来讲讲。
### 一、主要流程
1、 基本数据类型的传输
a)        上层定义一个native的方法，需要一个int 参数 ，返回一个int值
b)       JNI 对应 上层的方法 ， 打印出  上层 传输下来的 int数据，并返回 int数据
c)        上层 收到 native 方法 返回的 值，在UI中显示出来
2、 数组的传输
a)        上层定义一个native的方法，需要一个int数组，返回一个int数组
b)       JNI 对应上层的方法，取出上层传递数组中的数据处理和打印出来，并存入新数组中，最后把该数组返回给 Java层
c)        上层 收到 native返回的 数组，加工成字符串，在UI中显示出来
**二、使用代码**
java文件：
```java
public native String careJuly();
        public native int careJulyIn(int showCode);
	public native int[] careJulyArray(int[] InCode);
    
	static {
		System.loadLibrary("feelJulyGpsMap");
		}
```
cpp文件：
```java
#include <jni.h>
#include <string.h>
#include <android/log.h>
JNIEnv* jniEnv;
extern "C" jstring Java_com_example_feeljulygpsmap_FeelJulyActivity_careJuly(JNIEnv *env, jobject thiz) {
	return env->NewStringUTF( "Hello, July!" );
}
extern "C" jint Java_com_example_feeljulygpsmap_FeelJulyActivity_careJulyIn(JNIEnv *env, jobject thiz,jint showCode) {
	//return env->NewStringUTF( "Hello, July!" );
	if(jniEnv == NULL) {
	        jniEnv = env;
	    }
	//获取 Java 传递下来 数字
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Java -- > C JNI : num = %d",showCode);
	    //返回 2 倍 的数字给 Java
	return showCode*2;
}
extern "C" jintArray Java_com_example_feeljulygpsmap_FeelJulyActivity_careJulyArray(JNIEnv *env, jobject thiz,jintArray InCode) {
	//return env->NewStringUTF( "Hello, July!" );
	if(jniEnv == NULL) {
		        jniEnv = env;
		    }
	if(InCode == NULL){
	        return NULL;
	    }
	//获取 Java 传递下来 数组 的 长度
	//jsize len = (*jniEnv)->GetArrayLength(jniEnv, InCode);
	jsize len = jniEnv->GetArrayLength( InCode);
	 __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Java -- > C JNI : len = %d",len);
	 if(len <= 0) {
	         return NULL;
	     }
	     //新建一个长度为len的jintArray数组
	     //jintArray array = (*jniEnv)-> NewIntArray(jniEnv, len);
	     jintArray array = env->NewIntArray(len);
	     if(array == NULL) {
	         return NULL;
	     }
	     // 把 Java 传递下来的数组 用 jint* 存起来
	     //jint *body = (*env)->GetIntArrayElements(env,InCode, 0);
	     jint *body = env->GetIntArrayElements(InCode, 0);
	     jint i = 0;
	     jint num[len];
	     for (; i < len; i++) {
	         num[i] = body[i] * 2;
	         __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Java -- > C JNI : nums[%d] = %d",i,num[i]);
	     }
	     if(num == NULL){
	         return NULL;
	     }
	     //给 需要返回的数组赋值
	     //(*jniEnv)->SetIntArrayRegion(jniEnv,array, 0, len, num);
	     jniEnv->SetIntArrayRegion(array, 0, len, num);
	     return array;
}
```
错误提示：undefined reference to `__android_log_print'
修改MK文件，添加：
```java
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
```
```java
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_MODULE    := feelJulyGpsMap
LOCAL_SRC_FILES := feelJulyGpsMap.cpp
include $(BUILD_SHARED_LIBRARY)
```
**三、UI设计代码**
```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >
    <SurfaceView
        android:id="@+id/camera_preview"
        android:layout_width="240dp"
        android:layout_height="160dp" />
    <Button
        android:id="@+id/save_pic"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Fly" />
    <Button
			android:layout_width="80dp"
			android:layout_height="wrap_content"
			android:text="计算"
			android:id="@+id/btn_calculate" />
    <Button
			android:layout_width="fill_parent"
			android:layout_height="wrap_content"
			android:text="生成"
			android:id="@+id/btn_calculate_array" />
    <EditText
			android:layout_width="60dp"
			android:layout_height="wrap_content"
			android:id="@+id/et_num" />
    <EditText
			android:layout_width="fill_parent"
			android:layout_height="wrap_content"
			android:id="@+id/et_array_num" />
    <TextView
			android:layout_width="60dp"
			android:layout_height="wrap_content"
			android:id="@+id/tv_double_num"
			android:text="CalNum:" />
    <TextView
			android:id="@+id/tv_array_double_num"
			android:layout_height="wrap_content"
			android:layout_width="fill_parent"
			android:text="CalArray:" />
    
</LinearLayout>
```
**四、运行结果**
![](https://img-blog.csdn.net/20150701171026388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150701171102948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
