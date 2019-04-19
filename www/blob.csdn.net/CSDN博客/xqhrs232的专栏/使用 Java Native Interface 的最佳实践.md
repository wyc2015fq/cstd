# 使用 Java Native Interface 的最佳实践 - xqhrs232的专栏 - CSDN博客
2010年12月22日 14:03:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：911标签：[java																[interface																[jni																[jvm																[byte																[buffer](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://java.chinaitlab.com/base/790503.html](http://java.chinaitlab.com/base/790503.html)
[Java](http://java.chinaitlab.com/) 环境和语言对于应用程序开发来说是非常[安全](http://security.chinaitlab.com/)和高效的。但是，一些应用程序却需要执行纯
[Java](http://java.chinaitlab.com/) 程序无法完成的一些任务，比如：
    与旧有代码集成，避免重新编写。
    实现可用类库中所缺少的功能。举例来说，在 Java 语言中实现 ping 时，您可能需要 Internet Control Message Protocol （ICMP） 功能，但基本类库并未提供它。
    最好与使用 C/[C++](http://c.chinaitlab.com/) 编写的代码集成，以充分发掘性能或其他与环境相关的系统特性。
    解决需要非 Java 代码的特殊情况。举例来说，核心类库的实现可能需要跨包调用或者需要绕过其他 Java [安全](http://security.chinaitlab.com/)性检查。
    JNI 允许您完成这些任务。它明确分开了 Java 代码与本机代码（C/[C++](http://c.chinaitlab.com/)）的执行，定义了一个清晰的 API 在这两者之间进行通信。从很大程度上说，它避免了本机代码对 JVM 的直接内存引用，从而确保本机代码只需编写一次，并且可以跨不同的 JVM 实现或版本运行。
    借助 JNI，本机代码可以随意与 Java 对象交互，获取和设计字段值，以及调用方法，而不会像 Java 代码中的相同功能那样受到诸多限制。这种自由是一把双刃剑：它牺牲 Java 代码的安全性，换取了完成上述所列任务的能力。在您的应用程序中使用 JNI 提供了强大的、对机器资源（内存、I/O 等）的低级访问，因此您不会像普通 Java 开发人员那样受到安全网的保护。JNI 的灵活性和强大性带来了一些编程实践上的风险，比如导致性能较差、出现 bug 甚至程序崩溃。您必须格外留意应用程序中的代码，并使用良好的实践来保障应用程序的总体完整性。
    本文介绍 JNI 用户最常遇到的 10 大编码和设计错误。其目标是帮助您认识到并避免它们，以便您可以编写安全、高效、性能出众的 JNI 代码。本文还将介绍一些用于在新代码或已有代码中查找这些问题的工具和技巧，并展示如何有效地应用它们。
**JNI 编程缺陷可以分为两类：**
    性能：代码能执行所设计的功能，但运行缓慢或者以某种形式拖慢整个程序。
    正确性：代码有时能正常运行，但不能可靠地提供所需的功能；最坏的情况是造成程序崩溃或挂起。
**性能缺陷**
    程序员在使用 JNI 时的 5 大性能缺陷如下：
    不缓存方法 ID、字段 ID 和类
** 触发数组副本**
    回访（Reaching back）而不是传递参数
    错误认定本机代码与 Java 代码之间的界限
    使用大量本地引用，而未通知 JVM
    不缓存方法 ID、字段 ID 和类
    要访问 Java 对象的字段并调用它们的方法，本机代码必须调用 FindClass（）、GetFieldID（）、GetMethodId（） 和 GetStaticMethodID（）。对于 GetFieldID（）、GetMethodID（） 和 GetStaticMethodID（），为特定类返回的 ID 不会在 JVM 进程的生存期内发生变化。但是，获取字段或方法的调用有时会需要在 JVM 中完成大量工作，因为字段和方法可能是从超类中继承而来的，这会让 JVM 向上遍历类层次结构来找到它们。由于
 ID 对于特定类是相同的，因此您只需要查找一次，然后便可重复使用。同样，查找类对象的开销也很大，因此也应该缓存它们。
    举例来说，清单 1 展示了调用静态方法所需的 JNI 代码：
    清单 1. 使用 JNI 调用静态方法
 int　val=1;　
jmethodID　method;　
jclass　cls;　
cls　=　(*env)->FindClass(env,　"com/ibm/example/TestClass");　
if　((*env)->ExceptionCheck(env))　{　
　　return　ERR_FIND_CLASS_FAILED;　
}　
method　=　(*env)->GetStaticMethodID(env,　cls,　"setInfo",　"(I)V");　
if　((*env)->ExceptionCheck(env))　{　
　　return　ERR_GET_STATIC_METHOD_FAILED;　
}　
(*env)->CallStaticVoidMethod(env,　cls,　method,val);　
if　((*env)->ExceptionCheck(env))　{　
　　return　ERR_CALL_STATIC_METHOD_FAILED;　
}　
当我们每次希望调用方法时查找类和方法 ID 都会产生六个本机调用，而不是第一次缓存类和方法 ID 时需要的两个调用。
    缓存会对您应用程序的运行时造成显著的影响。考虑下面两个版本的方法，它们的作用是相同的。清单 2 使用了缓存的字段 ID：
    清单 2. 使用缓存的字段 ID
 int　sumValues2(JNIEnv*　env,　jobject　obj,　jobject　allValues){　
　　jint　avalue　=　(*env)->GetIntField(env,　allValues,　a);　
　　jint　bvalue　=　(*env)->GetIntField(env,　allValues,　b);　
　　jint　cvalue　=　(*env)->GetIntField(env,　allValues,　c);　
　　jint　dvalue　=　(*env)->GetIntField(env,　allValues,　d);　
　　jint　evalue　=　(*env)->GetIntField(env,　allValues,　e);　
　　jint　fvalue　=　(*env)->GetIntField(env,　allValues,　f);　
　　return　avalue　+　bvalue　+　cvalue　+　dvalue　+　evalue　+　fvalue;　
}　
    清单 3 没有使用缓存的字段 ID：
    清单 3. 未缓存字段 ID
 int　sumValues2(JNIEnv*　env,　jobject　obj,　jobject　allValues){　
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
　　return　avalue　+　bvalue　+　cvalue　+　dvalue　+　evalue　+　fvalue　
}　
清单 2 用 3，572 ms 运行了 10，000，000 次。清单 3 用了 86，217 ms — 多花了 24 倍的时间。
** 触发数组副本**
    JNI 在 Java 代码和本机代码之间提供了一个干净的接口。为了维持这种分离，数组将作为不透明的句柄传递，并且本机代码必须回调 JVM 以便使用 set 和 get 调用操作数组元素。Java 规范让 JVM 实现决定让这些调用提供对数组的直接访问，还是返回一个数组副本。举例来说，当数组经过优化而不需要连续[存储](http://www.storworld.com/)时，JVM 可以返回一个副本。（参见
 参考资料 获取关于 JVM 的信息）。
    随后，这些调用可以复制被操作的元素。举例来说，如果您对含有 1，000 个元素的数组调用 GetLongArrayElements（），则会造成至少分配或复制 8，000 字节的数据（每个 long 1，000 元素 * 8 字节）。当您随后使用 ReleaseLongArrayElements（） 更新数组的内容时，需要另外复制 8，000 字节的数据来更新数组。即使您使用较新的 GetPrimitiveArrayCritical（），规范仍然准许 JVM 创建完整数组的副本。
    GetTypeArrayRegion（） 和 SetTypeArrayRegion（） 方法允许您获取和更新数组的一部分，而不是整个数组。通过使用这些方法访问较大的数组，您可以确保只复制本机代码将要实际使用的数组部分。
    举例来说，考虑相同方法的两个版本，如清单 4 所示：
    清单 4. 相同方法的两个版本
 jlong　getElement(JNIEnv*　env,　jobject　obj,　jlongArray　arr_j,　
　　　　　　　　　int　element){　
　　jboolean　isCopy;　
　　jlong　result;　
　　jlong*　buffer_j　=　(*env)->GetLongArrayElements(env,　arr_j,　&isCopy);　
　　result　=　buffer_j[element];　
　　(*env)->ReleaseLongArrayElements(env,　arr_j,　buffer_j,　0);　
　　return　result;　
}　
jlong　getElement2(JNIEnv*　env,　jobject　obj,　jlongArray　arr_j,　
　　　　　　　　　int　element){　
　　　jlong　result;　
　　　(*env)->GetLongArrayRegion(env,　arr_j,　element,1,　&result);　
　　　return　result;　
}　
第一个版本可以生成两个完整的数组副本，而第二个版本则完全没有复制数组。当数组大小为 1，000 字节时，运行第一个方法 10，000，000 次用了 12，055 ms；而第二个版本仅用了 1，421 ms.第一个版本多花了 8.5 倍的时间！
    另一方面，如果您最终要获取数组中的所有元素，则使用 GetTypeArrayRegion（） 逐个获取数组中的元素是得不偿失的。要获取最佳的性能，应该确保以尽可能大的块的来获取和更新数组元素。如果您要迭代一个数组中的所有元素，则 清单 4 中这两个 getElement（） 方法都不适用。比较好的方法是在一个调用中获取大小合理的数组部分，然后再迭代所有这些元素，重复操作直到覆盖整个数组。
**回访而不是传递参数**
    在调用某个方法时，您经常会在传递一个有多个字段的对象以及单独传递字段之间做出选择。在面向对象设计中，传递对象通常能提供较好的封装，因为对象字段的变化不需要改变方法签名。但是，对于 JNI 来说，本机代码必须通过一个或多个 JNI 调用返回到 JVM 以获取需要的各个字段的值。这些额外的调用会带来额外的开销，因为从本机代码过渡到 Java 代码要比普通方法调用开销更大。因此，对于 JNI 来说，本机代码从传递进来的对象中访问大量单独字段时会导致性能降低。
    考虑清单 5 中的两个方法，第二个方法假定我们缓存了字段 ID：
    清单 5. 两个方法版本
 int　sumValues(JNIEnv*　env,　jobject　obj,　jint　a,　jint　b,jint　c,　jint　d,　jint　e,　jint　f){　
　　return　a　+　b　+　c　+　d　+　e　+　f;　
}　
int　sumValues2(JNIEnv*　env,　jobject　obj,　jobject　allValues){　
　　jint　avalue　=　(*env)->GetIntField(env,　allValues,　a);　
　　jint　bvalue　=　(*env)->GetIntField(env,　allValues,　b);　
　　jint　cvalue　=　(*env)->GetIntField(env,　allValues,　c);　
　　jint　dvalue　=　(*env)->GetIntField(env,　allValues,　d);　
　　jint　evalue　=　(*env)->GetIntField(env,　allValues,　e);　
　　jint　fvalue　=　(*env)->GetIntField(env,　allValues,　f);　
　　return　avalue　+　bvalue　+　cvalue　+　dvalue　+　evalue　+　fvalue;　
}　
sumValues2（） 方法需要 6 个 JNI 回调，并且运行 10，000，000 次需要 3，572 ms.其速度比 sumValues（） 慢 6 倍，后者只需要 596 ms.通过传递 JNI 方法所需的数据，sumValues（） 避免了大量的 JNI 开销。
    错误认定本机代码与 Java 代码之间的界限
    本机代码和 Java 代码之间的界限是由开发人员定义的。界限的选定会对应用程序的总体性能造成显著的影响。从 Java 代码中调用本机代码以及从本机代码调用 Java 代码的开销比普通的 Java 方法调用高很多。此外，这种越界操作会干扰 JVM 优化代码执行的能力。举例来说，随着 Java 代码与本机代码之间互操作的增加，实时编译器的效率会随之降低。经过测量，我们发现从 Java 代码调用本机代码要比普通调用多花 5 倍的时间。同样，从本机代码中调用 Java 代码也需要耗费大量的时间。
    因此，在设计 Java 代码与本机代码之间的界限时应该最大限度地减少两者之间的相互调用。消除不必要的越界调用，并且应该竭力在本机代码中弥补越界调用造成的成本损失。最大限度地减少越界调用的一个关键因素是确保数据处于 Java/本机界限的正确一侧。如果数据未在正确的一侧，则另一侧访问数据的需求则会持续发起越界调用。
    举例来说，如果我们希望使用 JNI 为某个串行端口提供接口，则可以构造两种不同的接口。第一个版本如清单 6 所示：
    清单 6. 到串行端口的接口：版本 1
 /**　
　*　Initializes　the　serial　port　and　returns　a　java　SerialPortConfig　objects　
　*　that　contains　the　hardware　address　for　the　serial　port,　and　holds　
　*　information　needed　by　the　serial　port　such　as　the　next　buffer　
　*　to　write　data　into　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　comPortName　the　name　of　the　serial　port　
　*　@returns　SerialPortConfig　object　to　be　passed　ot　setSerialPortBit　
　*　　　　　and　getSerialPortBit　calls　
　*/　
jobject　initializeSerialPort(JNIEnv*　env,　jobject　obj,　jstring　comPortName);　
/**　
　*　Sets　a　single　bit　in　an　8　bit　byte　to　be　sent　by　the　serial　port　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　serialPortConfig　object　returned　by　initializeSerialPort　
　*　@param　whichBit　value　from　1-8　indicating　which　bit　to　set　
　*　@param　bitValue　0th　bit　contains　bit　value　to　be　set　
　*/　
void　setSerialPortBit(JNIEnv*　env,　jobject　obj,　jobject　serialPortConfig,　
　jint　whichBit,　jint　bitValue);　
/**　
　*　Gets　a　single　bit　in　an　8　bit　byte　read　from　the　serial　port　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　serialPortConfig　object　returned　by　initializeSerialPort　
　*　@param　whichBit　value　from　1-8　indicating　which　bit　to　read　
　*　@returns　the　bit　read　in　the　0th　bit　of　the　jint　
　*/　
jint　getSerialPortBit(JNIEnv*　env,　jobject　obj,　jobject　serialPortConfig,　
　jint　whichBit);　
/**　
　*　Read　the　next　byte　from　the　serial　port　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*/　
void　readNextByte(JNIEnv*　env,　jobject　obj);　
/**　
　*　Send　the　next　byte　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*/　
void　sendNextByte(JNIEnv*　env,　jobject　obj);　
在 清单 6 中，串行端口的所有配置数据都[存储](http://www.storworld.com/)在由 initializeSerialPort（） 方法返回的 Java 对象中，并且将 Java 代码完全控制对硬件中各数据位的设置。清单 6 所示版本的一些问题会造成其性能差于清单 7 中的版本：
    清单 7. 到串行端口的接口：版本 2
 /**　
　*　Initializes　the　serial　port　and　returns　an　opaque　handle　to　a　native　
　*　structure　that　contains　the　hardware　address　for　the　serial　port　
　*　and　holds　information　needed　by　the　serial　port　such　as　
　*　the　next　buffer　to　write　data　into　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　comPortName　the　name　of　the　serial　port　
　*　@returns　opaque　handle　to　be　passed　to　setSerialPortByte　and　
　*　　　　　getSerialPortByte　calls　
　*/　
jlong　initializeSerialPort2(JNIEnv*　env,　jobject　obj,　jstring　comPortName);　
/**　
　*　sends　a　byte　on　the　serial　port　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　serialPortConfig　opaque　handle　for　the　serial　port　
　*　@param　byte　the　byte　to　be　sent　
　*/　
void　sendSerialPortByte(JNIEnv*　env,　jobject　obj,　jlong　serialPortConfig,　
　　jbyte　byte);　
/**　
　*　Reads　the　next　byte　from　the　serial　port　
　*　
　*　@param　env　JNI　env　that　can　be　used　by　the　method　
　*　@param　serialPortConfig　opaque　handle　for　the　serial　port　
　*　@returns　the　byte　read　from　the　serial　port　
　*/　
jbyte　readSerialPortByte(JNIEnv*　env,　jobject　obj,　jlong　serialPortConfig);
最显著的一个问题就是，清单 6 中的接口在设置或检索每个位，以及从串行端口读取字节或者向串行端口写入字节都需要一个 JNI 调用。这会导致读取或写入的每个字节的 JNI 调用变成原来的 9 倍。第二个问题是，清单 6 将串行端口的配置信息存储在 Java/本机界限的错误一侧的某个 Java 对象上。我们仅在本机侧需要此配置数据；将它存储在 Java 侧会导致本机代码向 Java 代码发起大量回调以获取/设置此配置信息。清单 7 将配置信息存储在一个本机结构中（比如，一个 struct），并向 Java
 代码返回了一个不透明的句柄，该句柄可以在后续调用中返回。这意味着，当本机代码正在运行时，它可以直接访问该结构，而不需要回调 Java 代码获取串行端口硬件地址或下一个可用的缓冲区等信息。因此，使用 清单 7 的实现的性能将大大改善。
** 使用大量本地引用而未通知 JVM**
    JNI 函数返回的任何对象都会创建本地引用。举例来说，当您调用 GetObjectArrayElement（） 时，将返回对数组中对象的本地引用。考虑清单 8 中的代码在运行一个很大的数组时会使用多少本地引用：
    清单 8. 创建本地引用
 void　workOnArray(JNIEnv*　env,　jobject　obj,　jarray　array){　
　　jint　i;　
　　jint　count　=　(*env)->GetArrayLength(env,　array);　
　　for　(i=0;　i　<　count;　i++)　{　
　　　jobject　element　=　(*env)->GetObjectArrayElement(env,　array,　i);　
　　　if((*env)->ExceptionOccurred(env))　{　
　　　　　break;　
　　　}　
　　　/*　do　something　with　array　element　*/　
　　}　
}　
每次调用 GetObjectArrayElement（） 时都会为元素创建一个本地引用，并且直到本机代码运行完成时才会释放。数组越大，所创建的本地引用就越多。
    这些本地引用会在本机方法终止时自动释放。JNI 规范要求各本机代码至少能创建 16 个本地引用。虽然这对许多方法来说都已经足够了，但一些方法在其生存期中却需要更多的本地引用。对于这种情况，您应该删除不再需要的引用，方法是使用 JNI DeleteLocalRef（） 调用，或者通知 JVM 您将使用更多的本地引用。
    清单 9 向 清单 8 中的示例添加了一个 DeleteLocalRef（） 调用，用于通知 JVM 本地引用已不再需要，以及将可同时存在的本地引用的数量限制为一个合理的数值，而与数组的大小无关：
    清单 9. 添加 DeleteLocalRef（）
 void　workOnArray(JNIEnv*　env,　jobject　obj,　jarray　array){　
　　jint　i;　
　　jint　count　=　(*env)->GetArrayLength(env,　array);　
　　for　(i=0;　i　<　count;　i++)　{　
　　　jobject　element　=　(*env)->GetObjectArrayElement(env,　array,　i);　
　　　if((*env)->ExceptionOccurred(env))　{　
　　　　　break;　
　　　}　
　　　/*　do　something　with　array　element　*/　
　　　(*env)->DeleteLocalRef(env,　element);　
　　}　
}　
    您可以调用 JNI EnsureLocalCapacity（） 方法来通知 JVM 您将使用超过 16 个本地引用。这将允许 JVM 优化对该本机代码的本地引用的处理。如果无法创建所需的本地引用，或者 JVM 采用的本地引用管理方法与所使用的本地引用数量之间不匹配造成了性能低下，则未成功通知 JVM 会导致 FatalError.
** 正确性缺陷**
**5 大 JNI 正确性缺陷包括：**
    使用错误的 JNIEnv
    未检测异常
    未检测返回值
    未正确使用数组方法
    未正确使用全局引用
    使用错误的 JNIEnv
    执行本机代码的线程使用 JNIEnv 发起 JNI 方法调用。但是，JNIEnv 并不是仅仅用于分派所请求的方法。JNI 规范规定每个 JNIEnv 对于线程来说都是本地的。JVM 可以依赖于这一假设，将额外的线程本地信息存储在 JNIEnv 中。一个线程使用另一个线程中的 JNIEnv 会导致一些小 bug 和难以调试的崩溃问题。
    线程可以调用通过 JavaVM 对象使用 JNI 调用接口的 GetEnv（） 来获取 JNIEnv.JavaVM 对象本身可以通过使用 JNIEnv 方法调用 JNI GetJavaVM（） 来获取，并且可以被缓存以及跨线程共享。缓存 JavaVM 对象的副本将允许任何能访问缓存对象的线程在必要时获取对它自己的 JNIEnv 访问。要实现最优性能，线程应该绕过 JNIEnv，因为查找它有时会需要大量的工作。
