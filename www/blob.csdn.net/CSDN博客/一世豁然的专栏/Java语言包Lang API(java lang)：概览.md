# Java语言包Lang API(java.lang)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月14日 09:05:47[一世豁然](https://me.csdn.net/Explorer_day)阅读数：731








提供对Java编程语言设计基础的类。 最重要的类是Object，它是类层次结构的根，Class的实例在运行时代表类。




通常需要将原始类型的值表示为对象。 包装类布尔值，字符，整数，长，浮点和双倍用于此目的。 例如，Double类型的对象包含一个类型为double的字段，表示该值，使得该引用可以存储在引用类型的变量中。 这些类还提供了一些用于在原始值之间进行转换的方法，以及支持像equals和hashCode这样的标准方法。 Void类是一个不可实例化的类，它保存对表示类型为void的Class对象的引用。





类Math提供常用的数学函数，如正弦，余弦和平方根。 类String，StringBuffer和StringBuilder类似地提供了对字符串的常用操作。





ClassLoader，Process，ProcessBuilder，Runtime，SecurityManager和System提供了管理类的动态加载，创建外部进程，主机环境查询（如时间和执行安全策略）的“系统操作”。





Throwable类包含可能被throw语句抛出的对象。 Throwable的子类代表错误和异常。





**字符编码**


java.nio.charset.Charset类的规范描述了字符编码的命名约定以及Java平台的每个实现必须支持的一组标准编码。








**注释**


[Deprecated](https://developer.android.google.cn/reference/java/lang/Deprecated.html)


注释@Deprecated的程序元素是程序员不鼓励使用的程序元素，通常是因为它是危险的，或者因为存在更好的选择。





[FunctionalInterface](https://developer.android.google.cn/reference/java/lang/FunctionalInterface.html)


用于指示接口类型声明旨在作为Java语言规范定义的功能接口的信息性注释类型。





[Override](https://developer.android.google.cn/reference/java/lang/Override.html)


表示方法声明旨在覆盖超类型中的方法声明。





[SafeVarargs](https://developer.android.google.cn/reference/java/lang/SafeVarargs.html)


程序员断言注释方法或构造函数的正文不会对其varargs参数执行潜在的不安全操作。





[SuppressWarnings](https://developer.android.google.cn/reference/java/lang/SuppressWarnings.html)


表示在注释元素（以及注释元素中包含的所有程序元素）中应该抑制命名的编译器警告。








接口

[Appendable](https://developer.android.google.cn/reference/java/lang/Appendable.html)


可以附加字符序列和值的对象。





[AutoCloseable](https://developer.android.google.cn/reference/java/lang/AutoCloseable.html)


可以保存资源的对象（如文件或套接字句柄），直到它关闭。





[CharSequence](https://developer.android.google.cn/reference/java/lang/CharSequence.html)


CharSequence是一个可读的char值序列。





[Cloneable](https://developer.android.google.cn/reference/java/lang/Cloneable.html)


一个类实现了Cloneable接口来指示clone（）方法，该方法对于该类的实例进行字段的字段复制是合法的。





[Comparable](https://developer.android.google.cn/reference/java/lang/Comparable.html)<T>


该接口对实现它的每个类的对象强加一个整体排序。





[Iterable](https://developer.android.google.cn/reference/java/lang/Iterable.html)<T>


实现此接口允许对象成为“for-each loop”语句的目标。





[Readable](https://developer.android.google.cn/reference/java/lang/Readable.html)


可读是一个字符的来源。





[Runnable](https://developer.android.google.cn/reference/java/lang/Runnable.html)


Runnable接口应该由其实例旨在由线程执行的任何类来实现。





[Thread.UncaughtExceptionHandler](https://developer.android.google.cn/reference/java/lang/Thread.UncaughtExceptionHandler.html)


当线程由于未捕获的异常突然终止而调用处理程序的接口。








类

[Boolean](https://developer.android.google.cn/reference/java/lang/Boolean.html)


布尔类将一个对象中的基本类型boolean的值包装起来。





[Byte](https://developer.android.google.cn/reference/java/lang/Byte.html)


字节类在对象中包装一个原始类型字节的值。





[Character](https://developer.android.google.cn/reference/java/lang/Character.html)


Character类包装一个对象中的基元类型char的值。





[Character.Subset](https://developer.android.google.cn/reference/java/lang/Character.Subset.html)


该类的实例表示Unicode字符集的特定子集。





[Character.UnicodeBlock](https://developer.android.google.cn/reference/java/lang/Character.UnicodeBlock.html)


一系列字符子集，表示Unicode规范中的字符块。





[Class](https://developer.android.google.cn/reference/java/lang/Class.html)<T>


类Class的实例表示正在运行的Java应用程序中的类和接口。





[ClassLoader](https://developer.android.google.cn/reference/java/lang/ClassLoader.html)


类加载器是负责加载类的对象。





[Compiler](https://developer.android.google.cn/reference/java/lang/Compiler.html)


在Android上没什么





[Double](https://developer.android.google.cn/reference/java/lang/Double.html)


Double类包装一个对象中的基元类型double的值。





[Enum](https://developer.android.google.cn/reference/java/lang/Enum.html)<E extends [Enum](https://developer.android.google.cn/reference/java/lang/Enum.html)<E>>


这是所有Java语言枚举类型的公共基类。





[Float](https://developer.android.google.cn/reference/java/lang/Float.html)


Float类在对象中包含一个基本类型float的值。





[InheritableThreadLocal](https://developer.android.google.cn/reference/java/lang/InheritableThreadLocal.html)<T>


该类扩展了ThreadLocal以提供从父线程到子线程的值的继承：当子线程被创建时，子接收到父值具有值的所有可继承的线程局部变量的初始值。





[Integer](https://developer.android.google.cn/reference/java/lang/Integer.html)


Integer类在对象中包装一个基本类型int的值。





[Long](https://developer.android.google.cn/reference/java/lang/Long.html)


Long类在对象中包含一个long类型的原始类型的值。





[Math](https://developer.android.google.cn/reference/java/lang/Math.html)


类Math包含执行基本数字运算的方法，如基本指数，对数，平方根和三角函数。





[Number](https://developer.android.google.cn/reference/java/lang/Number.html)


抽象类Number是表示可转换为基本类型byte，double，float，int，long和short的数值的平台类的超类。





[Object](https://developer.android.google.cn/reference/java/lang/Object.html)


Class Object是类层次结构的根。





[Package](https://developer.android.google.cn/reference/java/lang/Package.html)


包对象包含有关Java包的实现和规范的版本信息。





[Process](https://developer.android.google.cn/reference/java/lang/Process.html)


start（）和Runtime.exec方法创建一个本机进程，并返回一个可以用于控制进程并获取有关它的信息的Process子类的实例。





[ProcessBuilder](https://developer.android.google.cn/reference/java/lang/ProcessBuilder.html)


此类用于创建操作系统进程。





[ProcessBuilder.Redirect](https://developer.android.google.cn/reference/java/lang/ProcessBuilder.Redirect.html)


表示子进程输入的源或子进程输出的目的地。





[Runtime](https://developer.android.google.cn/reference/java/lang/Runtime.html)


每个Java应用程序都有一个Runtime类的实例，允许应用程序与运行应用程序的环境进行接口。





[RuntimePermission](https://developer.android.google.cn/reference/java/lang/RuntimePermission.html)


旧版安全码; 不使用。





[SecurityManager](https://developer.android.google.cn/reference/java/lang/SecurityManager.html)


旧版安全码; 不使用。





[Short](https://developer.android.google.cn/reference/java/lang/Short.html)


Short类在对象中包含基本类型short的值。





[StackTraceElement](https://developer.android.google.cn/reference/java/lang/StackTraceElement.html)


由getStackTrace（）返回的堆栈跟踪中的一个元素。





[StrictMath](https://developer.android.google.cn/reference/java/lang/StrictMath.html)


类StrictMath包含执行基本数字运算的方法，如基本指数，对数，平方根和三角函数。





[String](https://developer.android.google.cn/reference/java/lang/String.html)


String类表示字符串。





[StringBuffer](https://developer.android.google.cn/reference/java/lang/StringBuffer.html)


线程安全，可变的字符序列。





[StringBuilder](https://developer.android.google.cn/reference/java/lang/StringBuilder.html)


一个可变的字符序列。





[System](https://developer.android.google.cn/reference/java/lang/System.html)


System类包含几个有用的类字段和方法。





[Thread](https://developer.android.google.cn/reference/java/lang/Thread.html)


线程是程序中执行的线程。





[ThreadGroup](https://developer.android.google.cn/reference/java/lang/ThreadGroup.html)


线程组代表一组线程。





[ThreadLocal](https://developer.android.google.cn/reference/java/lang/ThreadLocal.html)<T>


此类提供线程局部变量。





[Throwable](https://developer.android.google.cn/reference/java/lang/Throwable.html)


Throwable类是Java语言中所有错误和异常的超类。





[Void](https://developer.android.google.cn/reference/java/lang/Void.html)


Void类是一个不可抗拒的占位符类，用于保存对表示Java关键字void的Class对象的引用。








宏

[Character.UnicodeScript](https://developer.android.google.cn/reference/java/lang/Character.UnicodeScript.html)


一组字符子集，表示Unicode标准附件24：脚本名称中定义的字符脚本。





[ProcessBuilder.Redirect.Type](https://developer.android.google.cn/reference/java/lang/ProcessBuilder.Redirect.Type.html)


ProcessBuilder.Redirect的类型。





[Thread.State](https://developer.android.google.cn/reference/java/lang/Thread.State.html)


线程状态。








异常

[ArithmeticException](https://developer.android.google.cn/reference/java/lang/ArithmeticException.html)


当出现异常算术条件时抛出。





[ArrayIndexOutOfBoundsException](https://developer.android.google.cn/reference/java/lang/ArrayIndexOutOfBoundsException.html)


抛出以表示已使用非法索引访问数组。





[ArrayStoreException](https://developer.android.google.cn/reference/java/lang/ArrayStoreException.html)


被抛出以表示尝试将错误类型的对象存储到对象数组中。





[ClassCastException](https://developer.android.google.cn/reference/java/lang/ClassCastException.html)


抛出以表示代码尝试将对象转换为不属于实例的子类。





[ClassNotFoundException](https://developer.android.google.cn/reference/java/lang/ClassNotFoundException.html)


当应用程序尝试通过其字符串名称加载到类中时抛出：


类Class中的forName方法。





[CloneNotSupportedException](https://developer.android.google.cn/reference/java/lang/CloneNotSupportedException.html)


抛出以表示类Object中的克隆方法已被调用以克隆对象，但该对象的类不实现Cloneable接口。





[EnumConstantNotPresentException](https://developer.android.google.cn/reference/java/lang/EnumConstantNotPresentException.html)


当应用程序尝试按名称访问枚举常量时抛出，枚举类型不包含指定名称的常量。





[Exception](https://developer.android.google.cn/reference/java/lang/Exception.html)


类Exception及其子类是Throwable的一种形式，表示合理应用程序可能想要捕获的条件。





[IllegalAccessException](https://developer.android.google.cn/reference/java/lang/IllegalAccessException.html)


当应用程序尝试反射创建实例（数组除外），设置或获取字段或调用方法时，会抛出IllegalAccessException，但是当前执行的方法无法访问指定类，字段的定义， 方法或构造函数。





[IllegalArgumentException](https://developer.android.google.cn/reference/java/lang/IllegalArgumentException.html)


被抛出来表示一种方法已经通过了非法或不正确的参数。





[IllegalMonitorStateException](https://developer.android.google.cn/reference/java/lang/IllegalMonitorStateException.html)


抛出以表明线程已尝试在对象的监视器上等待，或通知其他线程等待对象的监视器，而不拥有指定的监视器。





[IllegalStateException](https://developer.android.google.cn/reference/java/lang/IllegalStateException.html)


表示在非法或不适当的时间调用了一种方法。





[IllegalThreadStateException](https://developer.android.google.cn/reference/java/lang/IllegalThreadStateException.html)


抛出以指示线程不处于所请求操作的适当状态。





[IndexOutOfBoundsException](https://developer.android.google.cn/reference/java/lang/IndexOutOfBoundsException.html)


抛出以表示某种索引（例如数组，字符串或向量）的索引超出范围。





[InstantiationException](https://developer.android.google.cn/reference/java/lang/InstantiationException.html)


当应用程序尝试使用类Class中的newInstance方法尝试创建类的实例时抛出，但是指定的类对象不能被实例化。





[InterruptedException](https://developer.android.google.cn/reference/java/lang/InterruptedException.html)


在线程等待，睡眠或以其他方式占用之前抛出，线程在活动之前或活动期间中断。





[NegativeArraySizeException](https://developer.android.google.cn/reference/java/lang/NegativeArraySizeException.html)


抛出一个应用程序尝试创建一个负数大小的数组。





[NoSuchFieldException](https://developer.android.google.cn/reference/java/lang/NoSuchFieldException.html)


表示该类没有指定名称的字段。





[NoSuchMethodException](https://developer.android.google.cn/reference/java/lang/NoSuchMethodException.html)


当无法找到特定方法时抛出。





[NullPointerException](https://developer.android.google.cn/reference/java/lang/NullPointerException.html)


当应用程序尝试在需要对象的情况下使用null时抛出。





[NumberFormatException](https://developer.android.google.cn/reference/java/lang/NumberFormatException.html)


抛出以表示应用程序已尝试将字符串转换为其中一个数字类型，但该字符串不具有相应的格式。





[ReflectiveOperationException](https://developer.android.google.cn/reference/java/lang/ReflectiveOperationException.html)


反射操作在核心反射中抛出的常见超类异常。





[RuntimeException](https://developer.android.google.cn/reference/java/lang/RuntimeException.html)


RuntimeException是在Java虚拟机的正常操作期间可以抛出的异常的超类。





[SecurityException](https://developer.android.google.cn/reference/java/lang/SecurityException.html)


被安全经理抛出，表示安全违规。





[StringIndexOutOfBoundsException](https://developer.android.google.cn/reference/java/lang/StringIndexOutOfBoundsException.html)


通过String方法抛出，以指示索引为负或大于字符串的大小。





[TypeNotPresentException](https://developer.android.google.cn/reference/java/lang/TypeNotPresentException.html)


当应用程序尝试使用表示类型名称的字符串访问类型时抛出，但没有找到具有指定名称的类型的定义。





[UnsupportedOperationException](https://developer.android.google.cn/reference/java/lang/UnsupportedOperationException.html)


抛出以表示不支持请求的操作。








错误

[AbstractMethodError](https://developer.android.google.cn/reference/java/lang/AbstractMethodError.html)


当应用程序尝试调用抽象方法时抛出。





[AssertionError](https://developer.android.google.cn/reference/java/lang/AssertionError.html)


抛出表示断言失败。





[BootstrapMethodError](https://developer.android.google.cn/reference/java/lang/BootstrapMethodError.html)


抛出以表示invokedynamic指令未能找到其引导方法，或引导方法无法为具有正确方法类型的目标提供调用站点。





[ClassCircularityError](https://developer.android.google.cn/reference/java/lang/ClassCircularityError.html)


当Java虚拟机检测到正在加载的类的超类层次结构中的循环性时抛出。





[ClassFormatError](https://developer.android.google.cn/reference/java/lang/ClassFormatError.html)


当Java虚拟机尝试读取类文件并确定文件格式不正确或不能解释为类文件时抛出。





[Error](https://developer.android.google.cn/reference/java/lang/Error.html)


一个错误是Throwable的一个子类，表明一个合理的应用程序不应该尝试捕获的严重问题。





[ExceptionInInitializerError](https://developer.android.google.cn/reference/java/lang/ExceptionInInitializerError.html)


表示在静态初始化程序中发生意外异常。





[IllegalAccessError](https://developer.android.google.cn/reference/java/lang/IllegalAccessError.html)


如果应用程序尝试访问或修改某个字段，或调用无法访问的方法，则抛出此异常。





[IncompatibleClassChangeError](https://developer.android.google.cn/reference/java/lang/IncompatibleClassChangeError.html)


当一些类定义发生不兼容的类更改时抛出。





[InstantiationError](https://developer.android.google.cn/reference/java/lang/InstantiationError.html)


当应用程序尝试使用Java新构造来实例化抽象类或接口时抛出。





[InternalError](https://developer.android.google.cn/reference/java/lang/InternalError.html)


抛出，表示在Java虚拟机中发生了一些意外的内部错误。





[LinkageError](https://developer.android.google.cn/reference/java/lang/LinkageError.html)


LinkageError的子类表示一个类对另一个类有一些依赖; 然而，后期课程在前班的汇编之后已经不相容。





[NoClassDefFoundError](https://developer.android.google.cn/reference/java/lang/NoClassDefFoundError.html)


如果Java虚拟机或ClassLoader实例尝试在类的定义中加载（作为普通方法调用的一部分或作为使用新表达式创建新实例的一部分），并且不会找到该类的定义。





[NoSuchFieldError](https://developer.android.google.cn/reference/java/lang/NoSuchFieldError.html)


如果应用程序尝试访问或修改对象的指定字段，并且该对象不再具有该字段，则抛出。





[NoSuchMethodError](https://developer.android.google.cn/reference/java/lang/NoSuchMethodError.html)


如果一个应用程序尝试调用一个类（静态或实例）的指定方法，并且该类不再具有该方法的定义，则抛出此异常。





[OutOfMemoryError](https://developer.android.google.cn/reference/java/lang/OutOfMemoryError.html)


当Java虚拟机由于内存不足而无法分配对象时抛出，并且垃圾回收器不再有可用的内存。





[StackOverflowError](https://developer.android.google.cn/reference/java/lang/StackOverflowError.html)


当堆栈溢出发生时抛出，因为应用程序的深度过深。





[ThreadDeath](https://developer.android.google.cn/reference/java/lang/ThreadDeath.html)


当调用（不建议使用的）stop（）方法时，ThreadDeath的一个实例被抛出在受害者线程中。





[UnknownError](https://developer.android.google.cn/reference/java/lang/UnknownError.html)


当Java虚拟机中发生一个未知但严重的异常时抛出。





[UnsatisfiedLinkError](https://developer.android.google.cn/reference/java/lang/UnsatisfiedLinkError.html)


如果Java虚拟机找不到声明为native的方法的适当的本地语言定义，则抛出。





[UnsupportedClassVersionError](https://developer.android.google.cn/reference/java/lang/UnsupportedClassVersionError.html)


当Java虚拟机尝试读取类文件并确定文件中的主要和次要版本号不受支持时抛出。





[VerifyError](https://developer.android.google.cn/reference/java/lang/VerifyError.html)


当“验证者”检测到一个类文件，尽管形成良好时，会抛出一些内部不一致或安全问题。





[VirtualMachineError](https://developer.android.google.cn/reference/java/lang/VirtualMachineError.html)


抛出以表明Java虚拟机已损坏或已耗尽资源以使其继续运行。




