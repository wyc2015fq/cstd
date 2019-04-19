# Java JNI深度分析与实践 - xqhrs232的专栏 - CSDN博客
2010年12月22日 11:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：730标签：[jni																[java																[jvm																[buffer																[任务																[internet](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://edu.xvna.com/html/67711.html](http://edu.xvna.com/html/67711.html)
Java 环境和语言对于应用程序开发来说是非常安全和高效的。但是，一些应用程序却需要执行纯 Java 程序无法完成的一些任务，比如：
    与旧有代码集成，避免重新编写。
    实现可用类库中所缺少的功能。举例来说，在 Java 语言中实现 ping 时，您可能需要 Internet Control Message Protocol （ICMP） 功能，但基本类库并未提供它。
    最好与使用 C/C++ 编写的代码集成，以充分发掘性能或其他与环境相关的系统特性。
    解决需要非 Java 代码的特殊情况。举例来说，核心类库的实现可能需要跨包调用或者需要绕过其他 Java 安全性检查。
    JNI 允许您完成这些任务。它明确分开了 Java 代码与本机代码（C/C++）的执行，定义了一个清晰的 API 在这两者之间进行通信。从很大程度上说，它避免了本机代码对 JVM 的直接内存引用，从而确保本机代码只需编写一次，并且可以跨不同的 JVM 实现或版本运行。
    借助 JNI，本机代码可以随意与 Java 对象交互，获取和设计字段值，以及调用方法，而不会像 Java 代码中的相同功能那样受到诸多限制。这种自由是一把双刃剑：它牺牲 Java 代码的安全性，换取了完成上述所列任务的能力。在您的应用程序中使用 JNI 提供了强大的、对机器资源（内存、I/O 等）的低级访问，因此您不会像普通 Java 开发人员那样受到安全网的保护。JNI 的灵活性和强大性带来了一些编程实践上的风险，比如导致性能较差、出现 bug 甚至程序崩溃。您必须格外留意应用程序中的代码，并使用良好的实践来保障应用程序的总体完整性。
    本文介绍 JNI 用户最常遇到的 10 大编码和设计错误。其目标是帮助您认识到并避免它们，以便您可以编写安全、高效、性能出众的 JNI 代码。本文还将介绍一些用于在新代码或已有代码中查找这些问题的工具和技巧，并展示如何有效地应用它们。
    Java JNI 编程缺陷可以分为两类：
    性能：代码能执行所设计的功能，但运行缓慢或者以某种形式拖慢整个程序。
    正确性：代码有时能正常运行，但不能可靠地提供所需的功能；最坏的情况是造成程序崩溃或挂起。
    性能缺陷
    程序员在使用 JNI 时的 5 大性能缺陷如下：
    ◆不缓存方法 ID、字段 ID 和类
    ◆触发数组副本
    ◆回访（Reaching back）而不是传递参数
    ◆错误认定本机代码与 Java 代码之间的界限
    ◆使用大量本地引用，而未通知 JVM
    ◆不缓存方法 ID、字段 ID 和类
    要访问 Java 对象的字段并调用它们的方法，本机代码必须调用 FindClass（）、GetFieldID（）、GetMethodId（） 和 GetStaticMethodID（）。对于 GetFieldID（）、GetMethodID（） 和 GetStaticMethodID（），为特定类返回的 ID 不会在 JVM 进程的生存期内发生变化。但是，获取字段或方法的调用有时会需要在 JVM 中完成大量工作，因为字段和方法可能是从超类中继承而来的，这会让 JVM 向上遍历类层次结构来找到它们。由于
 ID 对于特定类是相同的，因此您只需要查找一次，然后便可重复使用。同样，查找类对象的开销也很大，因此也应该缓存它们。
    举例来说，清单 1 展示了调用静态方法所需的 JNI 代码：
    清单 1. 使用 JNI 调用静态方法
int　val=1;
jmethodID　method;
jclass　cls;
cls　=　(*env)->FindClass(env,　"com/ibm/example/TestClass");
if　((*env)->ExceptionCheck(env))　
{
    return　ERR_FIND_CLASS_FAILED;
}
method　=　(*env)->GetStaticMethodID(env,　cls,　"setInfo",　"(I)V");
if　((*env)->ExceptionCheck(env))　
{
    return　ERR_GET_STATIC_METHOD_FAILED;
}
(*env)->CallStaticVoidMethod(env,　cls,　method,val);
if　((*env)->ExceptionCheck(env))　
{
   return　ERR_CALL_STATIC_METHOD_FAILED;
}
    当我们每次希望调用方法时查找类和方法 ID 都会产生六个本机调用，而不是第一次缓存类和方法 ID 时需要的两个调用。
缓存会对您应用程序的运行时造成显著的影响。考虑下面两个版本的方法，它们的作用是相同的。清单 2 使用了缓存的字段 ID：
    清单 2. 使用缓存的字段 ID
int　sumValues2(JNIEnv*　env,　jobject　obj,　jobject　allValues)
{
jint　avalue　=　(*env)->GetIntField(env,　allValues,　a);
jint　bvalue　=　(*env)->GetIntField(env,　allValues,　b);
jint　cvalue　=　(*env)->GetIntField(env,　allValues,　c);
jint　dvalue　=　(*env)->GetIntField(env,　allValues,　d);
jint　evalue　=　(*env)->GetIntField(env,　allValues,　e);
jint　fvalue　=　(*env)->GetIntField(env,　allValues,　f);
return　avalue　+　bvalue　+　cvalue　+　dvalue　+　evalue　+　fvalue;
}
    清单 3. 未缓存字段 ID
int　sumValues2(JNIEnv*　env,　jobject　obj,　jobject　allValues)
{
jclass　cls　=　(*env)->GetObjectClass(env,allValues);
jfieldID　a　=　(*env)->GetFieldID(env,　cls,　"a",　"I");
jfieldID　b　=　(*env)->GetFieldID(env,　cls,　"b",　"I");
jfieldID　c　=　(*env)->GetFieldID(env,　cls,　"c",　"I");
jfieldID　d　=　(*env)->GetFieldID(env,　cls,　"d",　"I");
jfieldID　e　=　(*env)->GetFieldID(env,　cls,　"e",　"I");
jfieldID　f　=　(*env)->GetFieldID(env,　cls,　"f",　"I");
jint　avalue　=　(*env)->GetIntField(env,　allValues,　a);
jint　bvalue　=　(*env)->GetIntField(env,　allValues,　b);
jint　cvalue　=　(*env)->GetIntField(env,　allValues,　c);
jint　dvalue　=　(*env)->GetIntField(env,　allValues,　d);
jint　evalue　=　(*env)->GetIntField(env,　allValues,　e);
jint　fvalue　=　(*env)->GetIntField(env,　allValues,　f);
return　avalue　+　bvalue　+　cvalue　+　dvalue　+　evalue　+　fvalue ;
}
    清单 2 用 3，572 ms 运行了 10，000，000 次。清单 3 用了 86，217 ms — 多花了 24 倍的时间。
**    触发数组副本**
    JNI 在 Java 代码和本机代码之间提供了一个干净的接口。为了维持这种分离，数组将作为不透明的句柄传递，并且本机代码必须回调 JVM 以便使用 set 和 get 调用操作数组元素。Java 规范让 JVM 实现决定让这些调用提供对数组的直接访问，还是返回一个数组副本。举例来说，当数组经过优化而不需要连续存储时，JVM 可以返回一个副本。（参见 参考资料 获取关于 JVM 的信息）。
    随后，这些调用可以复制被操作的元素。举例来说，如果您对含有 1，000 个元素的数组调用 GetLongArrayElements（），则会造成至少分配或复制 8，000 字节的数据（每个 long 1，000 元素 * 8 字节）。当您随后使用 ReleaseLongArrayElements（） 更新数组的内容时，需要另外复制 8，000 字节的数据来更新数组。即使您使用较新的 GetPrimitiveArrayCritical（），规范仍然准许 JVM 创建完整数组的副本。
    GetTypeArrayRegion（） 和 SetTypeArrayRegion（） 方法允许您获取和更新数组的一部分，而不是整个数组。通过使用这些方法访问较大的数组，您可以确保只复制本机代码将要实际使用的数组部分。
    举例来说，考虑相同方法的两个版本，如清单 4 所示：
    清单 4. 相同方法的两个版本
jlong　getElement(JNIEnv*　env,　jobject　obj,　jlongArray　arr_j,int　element)
{
jboolean　isCopy;
jlong　result;
jlong*　buffer_j　=　(*env)->GetLongArrayElements(env,　arr_j,　&isCopy);
result　=　buffer_j[element];
(*env)->ReleaseLongArrayElements(env,　arr_j,　buffer_j,　0);
return　result;
}
jlong　getElement2(JNIEnv*　env,　jobject　obj,　jlongArray　arr_j,int　element)
{
jlong　result;
(*env)->GetLongArrayRegion(env,　arr_j,　element,1,　&result);
return　result;
}

