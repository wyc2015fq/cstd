# Java JNI开发时常用数据类型与C++中数据类型转换 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年10月08日 16:33:48[gloomyfish](https://me.csdn.net/jia20003)阅读数：3766








JavaJNI开发时常用数据类型与C++中数据类型转换

常见的数据类型对应关系如下：

![](https://img-blog.csdn.net/20161008162542475)




此外我们经常用String类型，它可以通过如下的API实现jstring到char*之间的相互转换constchar* szString = env-> GetStringUTFChars(jstring, 0);这种转换出来的char*类型数据，使用完之后一定要调用

env-> ReleaseStringUTFChars(jstring, szString);释放掉，不然会导致内存泄漏，如果忘记JVM会崩溃的。从C++中创建一个新的字符串然后返回的代码如下：

```cpp
// create jstring
	jstring computerName = env->NewStringUTF(chRtn);
	return computerName;
```

其中chRtn是char数组类型的指针。
下面是一个Java通过JNI接口调用C++的Windows接口实现电脑名称和用户名称查询的例子，首先定义JNI接口类如下

```java
package com.gloomyfish.jnidemo;


public class HelloJNI {
	
	public native int helloJNI(String name);
	
	public native String getComputerName();
	
	public native String getUserName();
	
	public native double calculateDistance(double x, double y);
	
	public static void main(String[] args) {
		System.loadLibrary("jnitest");
		HelloJNI happ = new HelloJNI();
		happ.helloJNI("gloomyfish");
		double sum = happ.calculateDistance(3, 4);
		String computerName = happ.getComputerName();
		String user = happ.getUserName();
		
		System.out.println("sum : " + sum);
		System.out.println("computer name : " + computerName);
		System.out.println("current user : " + user);
		System.out.println();
	}
}
```

C++中实现如下：

```cpp
// jnitest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include "math.h"
#include "com_gloomyfish_jnidemo_HelloJNI.h"

JNIEXPORT jint JNICALL Java_com_gloomyfish_jnidemo_HelloJNI_helloJNI
(JNIEnv *env, jobject obj, jstring param) {
	// convert java string type to c++ char* type
	const char* name = env->GetStringUTFChars(param, 0);
	printf("%s %s\n", "Hello JNI, I am ", name);
	// release memory
	env->ReleaseStringUTFChars(param, name);
	// return 0;
	return 0;
}

JNIEXPORT jstring JNICALL Java_com_gloomyfish_jnidemo_HelloJNI_getComputerName
(JNIEnv *env, jobject obj) {
	// define the buffer size
	const int MAX_BUFFER_LEN = 500;
	TCHAR  infoBuf[MAX_BUFFER_LEN];
	DWORD  bufCharCount = MAX_BUFFER_LEN;
	GetComputerName(infoBuf, &bufCharCount);

	// conver to jstring
	printf("computer name : %ls\n", infoBuf);
	int nLen = WideCharToMultiByte(CP_ACP, 0, infoBuf, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, infoBuf, -1, chRtn, nLen, NULL, NULL);

	// create jstring
	jstring computerName = env->NewStringUTF(chRtn);
	return computerName;
}

JNIEXPORT jstring JNICALL Java_com_gloomyfish_jnidemo_HelloJNI_getUserName
(JNIEnv *env, jobject obj) {
	// define the buffer size
	const int MAX_BUFFER_LEN = 500;
	TCHAR  infoBuf[MAX_BUFFER_LEN];
	DWORD  bufCharCount = MAX_BUFFER_LEN;
	GetUserName(infoBuf, &bufCharCount);

	// conver to jstring
	printf("current user : %ls\n", infoBuf);
	int nLen = WideCharToMultiByte(CP_ACP, 0, infoBuf, -1, NULL, 0, NULL, NULL);   
	char* chRtn = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, infoBuf, -1, chRtn, nLen, NULL, NULL);

	// create jstring
	jstring username = env->NewStringUTF(chRtn);
	return username;
}

JNIEXPORT jdouble JNICALL Java_com_gloomyfish_jnidemo_HelloJNI_calculateDistance
(JNIEnv *env, jobject obj, jdouble x, jdouble y) {
	double cx = x;
	double cy = y;
	double sum = pow(cx, 2) + pow(cy, 2);
	return sqrt(sum);
}
```

运行结果如下：
![](https://img-blog.csdn.net/20161008162927643)
其中通过命令行实现JNI头文件生成，命令如下：

**javah com.gloomyfish.jnidemo.HelloJNI**





