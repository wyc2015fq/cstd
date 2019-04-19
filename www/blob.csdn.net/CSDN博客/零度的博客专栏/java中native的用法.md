# java中native的用法 - 零度的博客专栏 - CSDN博客
2016年03月02日 14:58:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：12489
Java开发中会碰到声明为 native的方法，如：public native int hashCode（）; 这是一个native方法。为什么存在native方法呢？Java不是完美的，Java的不足除了体现在运行速度上要比传统的C++慢许多之外，Java无法直接访问到操作系统底层（如系统硬件等)，为此Java使用native方法来扩展Java程序的功能。
　　可以将native方法比作Java程序同C程序的接口，其实现步骤：
　　1、在Java中声明native()方法，然后编译。
　　2、用javah产生一个.h文件。
　　3、写一个.cpp文件实现native导出方法，其中需要包含第二步产生的.h文件（注意其中又包含了JDK带的jni.h文件）。
　　4、将第三步的.cpp文件编译成动态链接库文件。
　　5、在Java中用System.loadLibrary()方法加载第四步产生的动态链接库文件，这个native()方法就可以在Java中被访问了。
　　JAVA的native方法适用的情况：
　　1、为了使用底层的主机平台的某个特性，而这个特性不能通过JAVA API访问。
　　2、为了访问一个老的系统或者使用一个已有的库，而这个系统或这个库不是用JAVA编写的。
　　3、为了加快程序的性能，而将一段时间敏感的代码作为本地方法实现。
这就是Java JNI的应用，小例子如下：
**1) 编写**
java程序：这里以HelloWorld为例。
代码1：
```
```java
public
```
```java
class
```
```java
HelloWorld {
```
```java
```
```java
public
```
```java
native
```
```java
void
```
```java
displayHelloWorld();
```
```java
//所有native关键词修饰的都是对本地的声明
```
```java
```
```java
static
```
```java
{
```
```java
```
```java
System.loadLibrary(
```
```java
"hello"
```
```java
);
```
```java
//载入本地库
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
static
```
```java
void
```
```java
main(String[] args) {
```
```java
```
```java
new
```
```java
HelloWorld().displayHelloWorld();
```
```java
```
```java
}
```
```java
}
```
```
        声明[native](http://baike.baidu.com/subview/1935765/12649506.htm)方法：如果你想将一个方法做为一个本地[方法](http://baike.baidu.com/subview/169819/5086683.htm)的话，那么你就必须声明该方法为native的，并且不能实现。其中方法的参数和返回值在后面讲述。
 Load动态库：System.loadLibrary("hello");加载[动态库](http://baike.baidu.com/view/6892743.htm)（我们可以这样理解：我们的方法 displayHelloWorld()没有实现，但是我们在下面就直接使用了，所以必须在使用之前对它进行初始化）这里一般是以[static](http://baike.baidu.com/view/536145.htm)块进行加载的。同时需要注意的是System.[loadLibrary](http://baike.baidu.com/view/1286902.htm)();的参数“hello”是动态库的名字。
**2) 编译**
没有什么好说的了[javac](http://baike.baidu.com/view/1006557.htm)  HelloWorld.java
**3) 生成扩展名为h的头文件**javah HelloWorld
jni HelloWorld 头文件的内容：
```
```cpp
/*DO NOT EDI TTHIS FILE - it is mach inegenerated*/
```
```cpp
#include<jni.h>
```
```cpp
/*Header for class HelloWorld*/
```
```cpp
#ifndef_Included_HelloWorld
```
```cpp
#define_Included_HelloWorld
```
```cpp
#ifdef__cplusplus
```
```cpp
extern
```
```cpp
"C"
```
```cpp
{
```
```cpp
#endif
```
```cpp
/*
```
```cpp
*Class:HelloWorld
```
```cpp
*Method:displayHelloWorld
```
```cpp
*Signature:()V
```
```cpp
*/
```
```cpp
JNIEXPORTvoidJNICALL
```
```cpp
Java_HelloWorld_displayHelloWorld(JNIEnv*,jobject);
```
```cpp
#ifdef__cplusplus
```
```cpp
}
```
```cpp
#endif
```
```cpp
#endif
```
```
        （这里我们可以这样理解：这个h文件相当于我们在java里面的接口，这里声明了一个Java_HelloWorld_displayHelloWorld (JNIEnv *, jobject);方法，然后在我们的本地方法里面实现这个方法，也就是说我们在编写[C/C++](http://baike.baidu.com/view/1107274.htm)程序的时候所使用的方法名必须和这里的一致）。
**4) 编写本地方法**
        实现和由javah命令生成的头文件里面声明的方法名相同的方法。
代码2：
```
```cpp
#include"jni.h"
```
```cpp
#include"HelloWorld.h"
```
```cpp
//#includeotherheaders
```
```cpp
JNIEXPORT
```
```cpp
void
```
```cpp
JNICALL
```
```cpp
Java_HelloWorld_displayHelloWorld(JNIEnv*env,jobject obj)
```
```cpp
{
```
```cpp
printf
```
```cpp
(
```
```cpp
"Helloworld!\n"
```
```cpp
);
```
```cpp
return
```
```cpp
;
```
```cpp
}
```
```
        注意代码2中的第1行，需要将jni.h（该文件可以在%JAVA_HOME%/include文件夹下面找到）文件引入，因为在程序中的JNIEnv、 jobject等类型都是在该头文件中定义的；另外在第2行需要将HelloWorld.h头文件引入（我是这么理解的：相当于我们在编写java程序的时候，实现一个[接口](http://baike.baidu.com/subview/159864/15347640.htm)的话需要声明才可以，这里就是将HelloWorld.h头文件里面声明的方法加以实现。当然不一定是这样）。然后保存为
 HelloWorldImpl.c就ok了。
**5) 生成动态库**
        这里以在Windows中为例，需要生成dll文件。在保存HelloWorldImpl.c文件夹下面，使用[VC](http://baike.baidu.com/subview/10642/5375396.htm)的[编译器](http://baike.baidu.com/view/487018.htm)cl成。
 cl -I%java_home%\include -I%java_home%\include\win32 -LD HelloWorldImp.c -Fehello.dll 注意：生成的dll文件名在选项-Fe后面配置，这里是hello，因为在HelloWorld.java文件中我们loadLibary的时候使用的名字是hello。当然这里修改之后那里也需要修改。另外需要将-I%java_home%\include -I%java_home%\include\win32参数加上，因为在第四步里面编写本地方法的时候引入了jni.h文件。
如果配置了MinGW,也可以这样来编译：gcc -Wall -D_JNI_IMPLEMENTATION_ -Wl,--kill-at -Id:/java/include –Id:/java/include/win32 -shared -o (输出的dll文件名，如sum.dll) (输入的c/c++源文件,如abc.c)。
**6) 运行程序**
        javaHelloWorld就ok.
        如果用eclipse，需将dll或so文件放在项目下，而不是src及其子目录下。
        如果用命令行编译，把dll文件放在该包的同目录下。
