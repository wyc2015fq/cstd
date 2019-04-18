# 实现Android-JNI本地C++调试 - wishchinYang的专栏 - CSDN博客
2015年08月31日 13:57:43[wishchin](https://me.csdn.net/wishchin)阅读数：479
1.       原文链接：[NDK单步调试方法](http://blog.csdn.net/wutianyin222/article/details/8222838)
       如有问题或者版权要求，请拜访原作者或者通知本人.
最近为了性能需求，开始搞JNI，白手起搞真心不容易。中间差点崩溃了好几次，最终总算得到一点心得。
JNI对性能的提升没有我预想中的大，对于for循环的速度提升大概在1倍左右，所以如果数量级不大的话，性能提升不会很明显
JNI编完之后，不能调试，是不是很蛋疼，不像android Java可以看出错信息。JNI crash之后，界面上表示为没有任何反应，过段时间直接退出应用，没有提示、也没有XXX已停止运行。第一次遇到真是无从下手，有没有!!!，经过对Java层的log研究发现，原来JNI crash后，cpu就直接死在那里（相当于assert，程序停止运行）。果断时间后JAVA层发现程序已挂之后（进程僵死），就强制杀死该进程。于是程序直接退出而无提示。（以上分析纯属个人分析，如有不对，敬请指正）。这时发现如果能有单步调试功能那该多好！！
 百度一下 ndk单步调式 一般能搜出如下结果：
1、安装CDT插件
2、安装Sequoyah插件
3、使用插件装入JNI文件、导入JNI头文件
4、配置eclipse JNI编译环境
5、修改ndk-gdb脚本
6、运行ndk-gdb脚本，并修改生成的文件（windows需安装cygwin）
7、配置eclipse native调试环境
吓尿了，有没有！！！单步调试这么麻烦！！！麻烦不说，windows下使用ndk-gdb效率还特别低
另外，对于修改google release的ndk脚本，我一直持保留态度，安说release的脚本应该直接能用的，为什么要做修改后才能用呢？
以上步骤，我花了2~3小时搞定，仅仅为了能打个断点~~~
在机缘巧合下，我看到一篇google官方的文章“Using NDK Plugins”
令人兴奋的是，ADTr20已经支持JNI单步调试，再也不需要如上这么麻烦的步骤了
你现在需要做的只需以下2步：
1、使用ndk-build编译时，加上如下参数NDK_DEBUG=1 V=1，之后生成so文件之外，还会生成gdbobserver,gdb.setup调式文件
2、在项目的Debug Configuration中选择Android Native Apllication，点击下方Debug
3、Enjoy your Debugging！
如何？是不是超简单，而且调试速度还相当快哦
ADTr20已经比较完美支持NDK开发了。可以集成ndk编译，只需在项目右键Add Native Support，就能自动生成jni文件，并部署编译环境（注意这个过程是不可逆的，手动删除jni文件后，会编译不过）。如果需要生成NDK调试文件，需要右键项目->properties->C/C++ build->Builder Setting 把默认的Build Command : ndk-build 修改成 ndk-build NDK_DEBUG=1
V=1
最后推荐一个很有用的android官方网站[http://tools.android.com/](http://tools.android.com/) 这个网站包含大多数ADT使用方法和新版ADT的介绍，获益匪浅
关于如何使用ADT进行ndk单步调式就在[http://tools.android.com/recent/usingthendkplugin](http://tools.android.com/recent/usingthendkplugin)
只可惜这种方法只能在ndk环境下使用，如果要对framework中的jni进行单步调试，应该就行不通了，推荐的一种方法是把它在NDK中编译调试完毕，然后将生成的so放在system/lib下
文章补充：
       若出现 找不到ABI 的问题，测可能是 目标平台没有对齐，统一一下.
       参考：[http://stackoverflow.com/questions/18245867/getting-unknown-application-abi-when-trying-to-debug-some-c-code-in-my-android](http://stackoverflow.com/questions/18245867/getting-unknown-application-abi-when-trying-to-debug-some-c-code-in-my-android)
[http://stackoverflow.com/questions/20047348/unknown-application-abi-while-debug-as-native-application](http://stackoverflow.com/questions/20047348/unknown-application-abi-while-debug-as-native-application)
It was apparently due to the subliminal message shown between the `Unknown Application ABI` lines. So I added
```
APP_PLATFORM := android-10
```
to my `Android.mk`, and it works.
**2. 在工程右键 属性--C/C++Build-build setting **
ndk-build  修改为  ndk-build NDK_DEBUG=1 V=1
