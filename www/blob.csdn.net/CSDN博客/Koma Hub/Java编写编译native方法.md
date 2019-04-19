# Java编写编译native方法 - Koma Hub - CSDN博客
2018年01月19日 15:48:29[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：252
我的环境是CentOS，linux系统。
#编译带有native方法的java程序，生成class文件
`javac writeFloat.java`
#javah -jni生成.h文件
`javah -jni model`
#用编译好的.h文件中的函数定义，编辑你所需要的.c文件（也就是你的native方法），linux生成.so, windows生成.dll
```
//linux
gcc -shared -fpic -o libmodel.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux model.c
//windows
gcc -shared -fpic -o libmodel.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/win32 model.c
```
#运行你的调用本机方法的java.class
`java -Djava.library.path=. Main`
