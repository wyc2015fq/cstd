# 【经典Java系列】JNI || ABI - 三少GG - CSDN博客
2011年10月26日 21:21:04[三少GG](https://me.csdn.net/scut1135)阅读数：894
Jvm封装了各种操作系统实际的差异性的同时，提供了jni技术，使得开发者可以通过java程序（代码）调用到操作系统相关的技术实现的库函数，从而与其他技术和系统交互，使用其他技术实现的系统的功能；同时其他技术和系统也可以通过jni提供的相应原生接口开调用java应用系统内部实现的功能。
+++++++++++++++++++++++++++++
# 针对 ARM 体系结构的应用程序二进制接口 (ABI)   Application Binary Interface
用于针对 ARM 体系结构的应用程序二进制接口 (ABI) 的 ARM 文档集，这些接口包括 ARM 过程调用标准 (APCS)、ARM ELF、ARM DWARF、基础平台 ABI (BPABI)、C++ ABI、异常处理 ABI、运行时 ABI 和 C 库 ABI。
“针对 ARM 体系结构的 ABI”是一组规范，其中包括几个 PDF 文档、一份建议性声明，还有一份声明指出了“针对 ARM 体系结构的 ABI”第 1 版（2003 年 12 月发布）和第 2 版（2005 年第 1 季度发布）之间的实质性更改。
++++++++++++++++++++++++++++++++++++++++++++++++
## [Android JNI开发入门之一](http://my.unix-center.net/~Simon_fu/?p=833)
JNI 权威[http://java.sun.com.sixxs.org/docs/books/jni/html/jniTOC.html](http://java.sun.com.sixxs.org/docs/books/jni/html/jniTOC.html)
引言
Java 的出现给大家开发带来的极大的方便。但是，如果我们有大量原有的经过广泛测试的非 Java 代码，将它们全部用 Java 来重写，恐怕会带来巨大的工作量和长期的测试；如果我们的应用中需要访问到特定的设备，甚至是仅符合公司内部信息交互规范的设备，或某个特定的操作系统才有的特性，Java 就显得有些力不从心了。面对这些问题，Sun 公司在 JDK1.0 中就定义了 JNI 规范，它规定了 Java 应用程序对本地方法的调用规则。
所有的 JNI 调用都使用了 JNIEnv * 类型的指针，习惯上在 CPP 文件中将这个变量定义为 evn，它是任意一个本地方法的第一个参数。env 指针指向一个函数指针表，在 VC 中可以直接用"->"操作符访问其中的函数。
jobject 指向在此 Java 代码中实例化的 Java 对象 LocalFunction 的一个句柄，相当于 this 指针。
后续的参数就是本地调用中有 Java 程序传进的参数，本例中只有一个 String 型参数。对于字符串型参数，因为在本地代码中不能直接读取 Java 字符串，而必须将其转换为 C /C++ 字符串或 Unicode。以下是三个我们经常会用到的字符串类型处理的函数：
`const char* GetStringUTFChars(jstring string,jboolean* isCopy)`
返回指向字符串 UTF 编码的指针，如果不能创建这个字符数组，返回 null。这个指针在调用 ReleaseStringUTFChar() 函数之前一直有效。
参数：string 		 Java 字符串对象
 isCopy 		如果进行拷贝，指向以 JNI_TRUE 填充的 jboolean, 否则指向以 JNI_FALSE 填充的 jboolean。
 void ReleaseStringUTFChars(jstring str, const char* chars) 
通知虚拟机本地代码不再需要通过 chars 访问 Java 字符串。
参数：string 		 Java 字符串对象
 chars 		由 GetStringChars 返回的指针
 jstring NewStringUTF(const char *utf) 
返回一个新的 Java 字符串并将 utf 内容拷贝入新串，如果不能创建字符串对象，
返回 null。通常在反值类型为 string 型时用到。
参数：utf 		 UTF 编码的字符串指针
对于数值型参数，在 C/C++ 中可直接使用，其字节宽度如下所示：|Java|C/C++|字节数|
|----|----|----|
|boolean|jboolean|1|
|byte|jbyte|1|
|char|jchar|2|
|short|jshort|2|
|int|jint|4|
|long|jlong|8|
|float|jfloat|4|
|double|jdouble|8|
对于数组型参数， 
|Java|C/C++|
|----|----|
|boolean[ ]|JbooleanArray|
|byte[ ]|JbyteArray|
|char[ ]|JcharArray|
|short[ ]|JshortArray|
|int[ ]|JintArray|
|long[ ]|JlongArray|
|float[ ]|JfloatArray|
|double[ ]|JdoubleArray|
对于上述类型数组，有一组函数与其对应。以下函数中 Xxx 为对应类型。 
xxx * GetXxxArrayElements(xxxArray array, jboolean *isCopy) 
产生一个指向 Java 数组元素的 C 指针。不再需要时，需将此指针传给 ReleaseXxxArrayElemes。 
参数：array 			数组对象
 isCopy 		如果进行拷贝，指向以 JNI_TRUE 填充的 jboolean, 否则指向以 JNI_FALSE 填充的 jboolean。
例如： jboolean * GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy) 
 void ReleaseXxxArrayElements(xxxArray array,xxx *elems, jint mode) 
通知虚拟机不再需要从 GetXxxArrayElements 得到的指针。参数：array 			数组对象
 elems 		不再需要的指向数组元素的指针
 mode 		 0 ＝在更新数组元素后释放 elems 缓冲器
 JNI_COMMIT ＝在更新数组元素后不释放 elems 缓冲器
 JNI_ABORT ＝不更新数组元素释放 elems 缓冲器
例如：void ReleaseBooleanArrayElements(jbooleanArray array,jboolean *elems,     jint mode) 
 xxxArray NewXxxArray(jsize len) 
产生一个新的数组，通常在反值类型为数组型时用到参数：len 		数组中元素的个数。
例如：jbooleanArray NewBooleanArray(jsize len)用中注意事项
1 ． 如果可以通过 TCP/IP 实现 Java 代码与本地 C/C++ 代码的交互工作，那么最好不使用以上提到的 JNI 的方式，因为一次 JNI 调用非常耗时，大概要花 0.5 ～ 1 个毫秒。
2 ． 在一个 Applet 应用中，不要使用 JNI。因为在 applet 中可能引发安全异常。
3 ． 将所有本地方法都封装在单个类中，这个类调用单个 DLL。对于每种目标操作系统，都可以用特定于适当平台的版本替换这个 DLL。这样就可以将本地代码的影响减至最小，并有助于将以后所需的移植问题包含在内。
4 ． 本地方法要简单。尽量将生成的 DLL 对任何第三方运行时 DLL 的依赖减到最小。使本地方法尽量独立，以将加载 DLL 和应用程序所需的开销减到最小。如果必须要运行时 DLL，则应随应用程序一起提供它们。
5 ． 本地代码运行时，没有有效地防数组越界错误、错误指针引用带来的间接错误等。所以必须保证保证本地代码的稳定性，因为，丝毫的错误都可能导致 Java 虚拟机崩溃。
