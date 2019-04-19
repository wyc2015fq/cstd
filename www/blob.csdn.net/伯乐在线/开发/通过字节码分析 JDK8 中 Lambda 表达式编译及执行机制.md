# 通过字节码分析 JDK8 中 Lambda 表达式编译及执行机制 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李静瑶](http://www.jobbole.com/members/q186190644) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
关于Lambda字节码相关的文章，很早之前就想写了，[蜂潮运动]APP 产品的后端技术，能快速迭代，除了得益于整体微服架构之外，语言层面上，也是通过Java8的lambda表达式的运用以及rxJava响应式编程框架，使代码更加简洁易维护，调用方式更加便捷。
本文将介绍JVM中的方法调用相关的字节码指令，重点解析JDK7(JSR-292)之后新增的invokedynamic指令给lambda表达式的动态调用特性提供的实现机制，最后再探讨一下lambda性能方面的话题。
## 方法调用的字节码指令
在介绍**invokedynamic**指令前，先回顾一下JVM规范中的所有方法调用的字节码指令。其他关于字节码执行相关的也可以参考po主之前写的[JVM字节码执行模型及字节码指令集](http://blog.csdn.net/lijingyao8206/article/details/46562933)。
在Class文件中，方法调用即是对常量池（ConstantPool）属性表中的一个符号引用，在类加载的解析期或者运行时才能确定直接引用。
- invokestatic 主要用于调用static关键字标记的静态方法
- invokespecial 主要用于调用私有方法，构造器，父类方法。
- invokevirtual 虚方法，不确定调用那一个实现类,比如Java中的重写的方法调用。例子可以参考:[从字节码指令看重写在JVM中的实现](http://blog.csdn.net/lijingyao8206/article/details/47154679)
- invokeinterface 接口方法，运行时才能确定实现接口的对象，也就是运行时确定方法的直接引用，而不是解析期间。
- invokedynamic 这个操作码的执行方法会关联到一个动态调用点对象（Call Site object），这个call site 对象会指向一个具体的bootstrap 方法（方法的二进制字节流信息在BootstrapMethods属性表中）的执行，invokedynamic指令的调用会有一个独特的调用链，不像其他四个指令会直接调用方法，在实际的运行过程也相对前四个更加复杂。结合后面的例子，应该会比较直观的理解这个指令。
关于方法调用的其他详细的解释可以参考官方文档[《The Java® Virtual Machine Specification Java8 Edition》](https://docs.oracle.com/javase/specs/jvms/se8/jvms8.pdf)–**2.11.8 Method Invocation and Return Instructions。**
## lambda表达式运行机制
在看字节码细节之前，先来了解一下lambda表达式如何脱糖（desugar）。lambda的语法糖在编译后的字节流Class文件中，会通过**invokedynamic**指令指向一个bootstrap方法（下文中部分会称作“引导方法”），这个方法就是**java.lang.invoke.LambdaMetafactory**中的一个静态方法。通过debug的方式，就可以看到该方法的执行，此方法源码如下：

Java
```
public static CallSite metafactory(MethodHandles.Lookup caller,
           String invokedName,
           MethodType invokedType,
           MethodType samMethodType,
           MethodHandle implMethod,
           MethodType instantiatedMethodType)
            throws LambdaConversionException {
        AbstractValidatingLambdaMetafactory mf;
        mf = new InnerClassLambdaMetafactory(caller, invokedType,
                                             invokedName, samMethodType,
                                             implMethod, instantiatedMethodType,
                                             false, EMPTY_CLASS_ARRAY, EMPTY_MT_ARRAY);
        mf.validateMetafactoryArgs();
        return mf.buildCallSite();
    }
```
在运行时期，虚拟机会通过调用这个方法来返回一个CallSite（调用点）对象。简述一下方法的执行过程，首先，初始化一个InnerClassLambdaMetafactory对象，这个对象的*buildCallSite*方法会将Lambda表达式先转化成一个内部类，这个内部类是*MethodHandles.Lookup caller*的一个内部类，也即包含此Lambda表达式的类的内部类。这个内部类是通过字节码生成技术（jdk.internal.org.objectweb.asm)生成，再通过UNSAFE类加载到JVM。然后再返回绑定此内部类的CallSite对象，这个过程的源码也可以看一下:

Java
```
CallSite buildCallSite() throws LambdaConversionException {
        // 通过字节码生成技术（jdk asm）生成代表lambda表达式体信息的一个内部类的Class对象，因为是运行期生成，所以在编译后的字节码信息中并没有这个内部类的字节流信息。
        final Class> innerClass = spinInnerClass();
        // incokedType即lambda表达式的调用方法类型，如下面示例中的Consumer方法
        if (invokedType.parameterCount() == 0) {
            final Constructor>[] ctrs = AccessController.doPrivileged(
                    new PrivilegedAction[]>() {
                @Override
                public Constructor>[] run() {
                    Constructor>[] ctrs = innerClass.getDeclaredConstructors();
                    if (ctrs.length == 1) {
                        // 表示lambda表达式的内部类是私有的，所以需要获取这个内部类的访问权限。
                        ctrs[0].setAccessible(true);
                    }
                    return ctrs;
                }
                    });
            if (ctrs.length != 1) {
                throw new LambdaConversionException("Expected one lambda constructor for "
                        + innerClass.getCanonicalName() + ", got " + ctrs.length);
            }
            try {
            // 通过构造函数的newInstance方法，创建一个内部类对象
                Object inst = ctrs[0].newInstance();
                // MethodHandles.constant方法将这个内部类对象的信息组装并绑定到一个MethodHandle对象，作为ConstantCallSite的构造函数的参数“target”返回。后面对于Lambda表达式的调用，都会通过MethodHandle直接调用，不需再次生成CallSite.
                return new ConstantCallSite(MethodHandles.constant(samBase, inst));
            }
            catch (ReflectiveOperationException e) {
                throw new LambdaConversionException("Exception instantiating lambda object", e);
            }
        } else {
            try {
                UNSAFE.ensureClassInitialized(innerClass);
                return new ConstantCallSite(
                        MethodHandles.Lookup.IMPL_LOOKUP
                             .findStatic(innerClass, NAME_FACTORY, invokedType));
            }
            catch (ReflectiveOperationException e) {
                throw new LambdaConversionException("Exception finding constructor", e);
            }
        }
    }
```
这个过程将生成一个代表lambda表达式信息的内部类（也就是方法第一行的innerClass，这个类是一个 functional 类型接口的实现类），这个内部类的Class字节流是通过jdk asm 的ClassWriter,MethodVisitor,生成，然后再通过调用Constructor.newInstance方法生成这个内部类的对象，并将这个内部类对象绑定给一个MethodHandle对象，然后这个MethodHandle对象传给CallSite对象（通过CallSite的构造函数赋值）。所以这样就完成了一个将lambda表达式转化成一个内部类对象，然后将内部类通过MethodHandle绑定到一个CallSite对象。
CallSite对象就相当于lambda表达式的一个勾子。而**invokedynamic**指令就链接到这个CallSite对象来实现运行时绑定，也即**invokedynamic**指令在调用时，会通过这个勾子找到lambda所代表的一个functional接口对象（也即MethodHandle对象）。所以lambda的脱糖也就是在运行期通过bootstrap method的字节码信息，转化成一个MethodHandle的过程。
通过打印consumer对象的className（**greeter.getClass().getName()**）可以得到结果是*eight.Functionnal$$Lambda$1/659748578*前面字符是Lambda表达式的ClassName，后面的659748578是刚才所述内部类的hashcode值。
下面通过具体的字节码指令详细分析一下lambda的脱糖机制，并且看一下**invokedynamic**指令是怎么给lambda在JVM中的实现带来可能。如果前面所述过程还有不清晰，还可以参考下Oracle工程师在设计java8 Lambda表达式时候的一些思考：[Translation of Lambda Expressions](http://cr.openjdk.java.net/~briangoetz/lambda/lambda-translation.html)
## lambda表达式字节码指令示例分析
先看一个简单的示例，示例使用了**java.util.function**包下面的Consumer。
示例代码:（下面的Person对象只有一个String类型属性:name，以及一个有参构造方法）

Java
```
package eight;
import java.util.function.Consumer;
/**
 * Created by lijingyao on 15/11/2 19:13.
 */
public class Functionnal {
    public static void main(String[] args) {
        Consumer greeter = (p) -> System.out.println("Hello, " + p.getName());
        greeter.accept(new Person("Lambda"));
    }
}
```
用verbose命令看一下方法主体的字节码信息，这里暂时省略常量池信息，后面会在符号引用到常量池信息的地方具体展示。

Java
```
public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=4, locals=2, args_size=1
         0: invokedynamic #2,  0              // InvokeDynamic #0:accept:()Ljava/util/function/Consumer;
         5: astore_1
         6: aload_1
         7: new           #3                  // class eight/Person
        10: dup
        11: ldc           #4                  // String Lambda
        13: invokespecial #5                  // Method eight/Person."":(Ljava/lang/String;)V
        16: invokeinterface #6,  2            // InterfaceMethod java/util/function/Consumer.accept:(Ljava/lang/Object;)V
        21: return
      LineNumberTable:
        line 11: 0
        line 12: 6
        line 13: 21
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      22     0  args   [Ljava/lang/String;
            6      16     1 greeter   Ljava/util/function/Consumer;
      LocalVariableTypeTable:
        Start  Length  Slot  Name   Signature
            6      16     1 greeter   Ljava/util/function/Consumer;
```
### invokedynamic指令特性
可以看到第一条指令就是代表了lambda表达式的实现指令,**invokedynamic**指令，这个指令是**JSR-292**开始应用的规范，而鉴于兼容和扩展的考虑(可以参考Oracle工程师对于[使用**invokedynamic**指令的原因](http://cr.openjdk.java.net/~briangoetz/lambda/lambda-translation.html))，**JSR-337**通过这个指令来实现了lambda表达式。也就是说，只要有一个lambda表达式，就会对应一个**invokedynamic**指令。
先看一下第一行字节码指令信息
**0: invokedynamic #2, 0**
- **0:** 代表了在方法中这条字节码指令操作码（Opcode）的偏移索引。
- **invokedynamic**就是该条指令的操作码助记符。
- **#2, 0** 是指令的操作数（Operand），这里的#2表示操作数是一个对于Class常量池信息的一个符号引用。逗号后面的0 是invokedynamic指令的默认值参数，到目前的**JSR-337**规范版本一直而且只能等于0。所以直接看一下常量池中#2的信息。
invokedynamic在常量是有专属的描述结构的（不像其他方法调用指令，关联的是*CONSTANT_MethodType_info*结构）。
invokedynamic 在常量池中关联一个CONSTANT_InvokeDynamic_info结构，这个结构可以明确invokedynamic指令的一个引导方法（bootstrap method），以及动态的调用方法名和返回信息。
常量池索引位置#2的信息如下：

Java
```
#2 = InvokeDyn amic      #0:#44         // #0:accept:()Ljava/util/function/Consumer;
```
结合CONSTANT_InvokeDynamic_info的结构信息来看一下这个常量池表项包含的信息。
**CONSTANT_InvokeDynamic_info**结构如下：

Java
```
CONSTANT_InvokeDynamic_info {
 u1 tag;
 u2 bootstrap_method_attr_index;
 u2 name_and_type_index;
}
```
简单解释下这个CONSTANT_InvokeDynamic_info的结构：
- tag: 占用一个字节（u1）的tag，也即*InvokeDynamic*的一个标记值，其会转化成一个字节的tag值。可以看一下jvm spec中，常量池的tag值转化表（这里tag值对应=18）：
![ Constant pool tags](http://jbcdn2.b0.upaiyun.com/2016/04/16249f1e6ea83bc92808cf24800f5b40.png)
- bootstrap_method_attr_index：指向bootstrap_methods的一个有效索引值，其结构在属性表的 bootstrap method 结构中，也描述在Class文件的二进制字节流信息里。下面是对应索引 **0** 的bootstrap method 属性表的内容：

Java
```
BootstrapMethods:    
  0: #40 invokestatic java/lang/invoke/LambdaMetafactory.metafactory:(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/invoke/MethodType;Ljava/lang/invoke/MethodHandle;Ljava/lang/invoke/MethodType;)Ljava/lang/invoke/CallSite;   
    Method arguments:    
      #41 (Ljava/lang/Object;)V   
      #42 invokestatic eight/Functionnal.lambda$main$0:(Leight/Person;)V   
      #43 (Leight/Person;)V
```
这段字节码信息展示了，引导方法就是**LambdaMetafactory.metafactory**方法。对照着前面**LambdaMetafactory.metafactory**的源码一起阅读。通过debug先看一下这个方法在运行时的参数值：
![LambdaMetafactory.metafactory arguments](http://jbcdn2.b0.upaiyun.com/2016/04/51cc5b786b88d2fdc4fbfc53500a2c6b.png)
这个方法的前三个参数都是由JVM自动链接Call Site生成。方法最后返回一个CallSite对象，对应**invokedynamic**指令的操作数。
– name_and_type_index：代表常量池表信息的一个有效索引值，其指向的常量池属性表结构一定是一个CONSTANT_NameAndType_info属性，代表了方法名称和方法描述符信息。再沿着 **#44** 索引看一下常量池相关项的描述内容:

Java
```
#44 = NameAndType        #64:#65        // accept:()Ljava/util/function/Consumer;
 #64 = Utf8               accept
 #65 = Utf8               ()Ljava/util/function/Consumer;
```
通过以上几项，可以很清楚得到**invokedynamic**的方法描述信息。
### 其余字节码指令解析
综上，已经介绍了lombda表达式在字节码上的实现方式。其他指令，如果对字节码指令感兴趣可以继续阅读，已经了解的可以略过，本小节和lambda本身没有太大关联。
- 第二条指令：**5: astore_1** 指令起始偏移位置是**5**，主要取决于前面一个指令（*invokedynamic*）有两个操作数，每个操作数占两个字节(u2)空间，所以第二条指令就是从字节偏移位置5开始（后续的偏移地址将不再解释）。此指令执行后，当前方法的栈帧结构如下（注：此图没有画出当前栈帧的动态链接以及返回地址的数据结构，图中：左侧局部变量表，右侧操作数栈）：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/0672aafb8bee58ccb2511f191666ab16.png)
这里为了画图方便，所以按照局部变量表和操作数栈的实际分配空间先画出了几个格子。因为字节码信息中已经告知了[**stack=4, locals=2, args_size=1**]。也就是局部变量表的实际运行时空间最大占用两个Slot（一个Slot一个字节，long,double类型变量需占用两个slot），操作数栈是4个slot,参数占一个slot。这里的args是main方法的String[] args参数。因为是个static方法，所以也没有this变量的aload_0 指令。
2. 第三条： **6: aload_1**将greeter 弹出局部变量表，压入操作数栈。
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/354c6b93bf30d4a70779a4ef47e27c77.png)
3. 第四条：**7: new #3**初始化person对象指令，这里并不等同于*new*关键字，**new**操作码只是找到常量池的符号引用，执行到此行命令时，运行时堆区会创建一个有默认值的对象，如果是Object类型，那么默认值是null，然后将这个对于默认值的引用地址压入到操作数栈。其中**#3** 操作数指向的常量池Class属性表的一个引用，可以看到这个常量池项为：*#3 = Class #45 // eight/Person* 。此时的运行时栈帧结构如下：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/87778876bf3feba2bd45e7fa10799f39.png)
4. 第五条：**10: dup** 复制操作数栈栈顶的值，并且将该值入操作数栈栈顶。dup指令是一种对于初始化过程的编译期优化。因前面的*new*操作码并不会真正的创建对象，而是push一个引用到操作数栈，所以dup之后，这个栈顶的复制引用就可以用来给调用初始化方法（构造函数）的**invokespecial**提供操作数时消耗掉，同时原有的引用值就可以给其他比如对象引用的操作码使用。此时栈帧结构如下图：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/edaae97e79046d130786f4ab87bb68f9.png)
5. 第六条：**11: ldc #4** 将运行时常量池的值入操作数栈，这里的值是**Lambda**字符串。**#4** 在常量池属性表中结构信息如下：

Java
```
#4 = String             #46            // Lambda
  #46 = Utf8               Lambda
```
此时运行时栈帧结构如下：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/db8590b61b390a4ff2dce6e60cb80cb4.png)
6. 第七条：**13: invokespecial #5** 初始化Person对象的指令（**#5**指向了常量池Person的初始化方法*eight/Person.””:(Ljava/lang/String;)V*），也即调用Person构造函数的指令。此时”Lambda”常量池的引用以及 *dup* 复制的person引用地址出操作数栈。这条指令执行之后，才在堆中真正创建了一个Person对象。此时栈帧结构如下：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/c3efd5181a445c1debd1443576027232.png)
- 第八条：**16: invokeinterface #6, 2** 调用了Consumer的accept接口方法{greeter.accept(person)}。**#6** 逗号后面的参数**2** 是invokeinterface指令的参数，含义是接口方法的参数的个数加1，因为accpet方法只有一个参数，所以这里是1+1=2。接着再看一下常量池项 **#6** 属性表信息：

Java
```
#6 = InterfaceMethodref #48.#49        // java/util/function/Consumer.accept:(Ljava/lang/Object;)V
   #48 = Class              #67            // java/util/function/Consumer
   #49 = NameAndType        #64:#62        // accept:(Ljava/lang/Object;)V
   #67 = Utf8               java/util/function/Consumer
   #62 = Utf8               (Ljava/lang/Object;)V
   #64 = Utf8               accept
```
以上可以看出Consumer接口的泛型被擦除（编译期间进行，所以字节码信息中并不会包含泛型信息），所以这里并不知道实际的参数操作数类型。但是这里可以得到实际对象的引用值，这里accept方法执行，greeter和person引用出栈，如下图：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/0003ff1b95b4f5625b6f9db5549162b5.png)
8. 第九条：**21: return** 方法返回，因为是void方法，所以就是opcode就是return。此时操作数栈和局部变量表都是空，方法返回。最后再画上一笔：
![运行时栈帧结构](http://jbcdn2.b0.upaiyun.com/2016/04/3b103ed466e3eed9e9d5ba4e6d7efd25.png)
## 结语
本文只是通过Consumer接口分析lambda表达式的字节码指令，以及运行时的脱糖过程。也是把操作码忘得差不多了，也顺便再回顾一下。
从字节码看lambda可以追溯到源头，所以也就能理解运行时的内存模型。
lambda表达式对应一个**incokedynamic** 指令，通过指令在常量池的符号引用，可以得到**BootstrapMethods** 属性表对应的引导方法。在运行时，JVM会通过调用这个引导方法生成一个含有MethodHandle（CallSite的target属性）对象的CallSite作为一个Lambda的回调点。
Lambda的表达式信息在JVM中通过字节码生成技术转换成一个内部类，这个内部类被绑定到MethodHandle对象中。每次执行lambda的时候，都会找到表达式对应的回调点CallSite执行。一个CallSite可以被多次执行（在多次调用的时候）。如下面这种情况，只会有一个**invokedynamic**指令，在comparator调用*comparator.compare*或*comparator.reversed*方法时，都会通过CallSite找到其内部的MethodHandle，并通过MethodHandle调用Lambda的内部表示形式*LambdaForm*。

Java
```
public static void main(String[] args) {
        Comparator comparator = (p1, p2) -> p1.getFirstName().compareTo(p2.getFirstName());
        Person p1 = new Person("John", "Doe");
        Person p2 = new Person("Alice", "Wonderland");
        comparator.compare(p1, p2);            // > 0
        comparator.reversed().compare(p1, p2);  // 
    }
```
Lambda不仅用起来很方便，性能表现在多数情况也比匿名内部类好，性能方面可以参考一下Oracle的Sergey Kuksenko发布的 [Lambda 性能报告](http://www.oracle.com/technetwork/java/jvmls2013kuksen-2014088.pdf)。由上文可知，虽然在运行时需要转化Lambda Form(见MethodHandle的form属性生成过程)，并且生成CallSite，但是随着调用点被频繁调用，通过JIT编译优化等，性能会有明显提升。并且，运行时脱糖也增强了编译期的灵活性（其实在看字节码之前，一直以为Lambda可能是在编译期脱糖成一个匿名内部类的Class，而不是通过提供一个boortrap方法，在运行时链接到调用点）。
运行时生成调用点的方式实际的内存使用率在多数情况也是低于匿名内部类（java8 之前版本的写法）的方式。所以，在能使用lambda表达式的地方，我们尽量结合实际的性能测试情况，写简洁的表达式，尽量减少Lambda表达式内部捕获变量（因为这样会创建额外的变量对象），如果需要在表达式内部捕获变量，可以考虑是否可以将变量写成类的成员变量,也即尽量少给Lambda传多余的参数。希望本文能给Lambda的使用者一些参考。也希望大家下载[蜂潮运动]APP,工作繁忙之余也要适度运动。
## 资源
- [蜂潮运动](http://117sport.com/)
- [Translation of Lambda Expressions](http://cr.openjdk.java.net/~briangoetz/lambda/lambda-translation.html)
- [JVM字节码执行模型及字节码指令集](http://blog.csdn.net/lijingyao8206/article/details/46562933)
- [The Java® Virtual Machine Specification Java8 Edition( JSR-337 )](https://docs.oracle.com/javase/specs/jvms/se8/jvms8.pdf)
- [invoke an interface method](http://cs.au.dk/~mis/dOvs/jvmspec/ref--32.html)
- [Java 8 Lambdas – A Peek Under the Hood](http://www.infoq.com/articles/Java-8-Lambdas-A-Peek-Under-the-Hood)
- [JDK 8: Lambda Performance study](http://www.oracle.com/technetwork/java/jvmls2013kuksen-2014088.pdf)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/0f41ba80f78d64a0cfddec457f8eb852.jpg)
