# 使用Memory Analyzer tool(MAT)分析内存泄漏（二） - xqhrs232的专栏 - CSDN博客
2011年02月17日 11:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1470标签：[classloader																[class																[string																[java																[exception																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=classloader&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/kepoon/archive/2011/01/07/6122660.aspx](http://blog.csdn.net/kepoon/archive/2011/01/07/6122660.aspx)
前言
在使用Memory Analyzer tool(MAT)分析内存泄漏（一）中，我介绍了内存泄漏的前因后果。在本文中，将介绍MAT如何根据heap dump分析泄漏根源。由于测试范例可能过于简单，很容易找出问题，但我期待借此举一反三。
一开始不得不说说ClassLoader，本质上，它的工作就是把磁盘上的类文件读入内存，然后调用java.lang.ClassLoader.defineClass方法告诉系统把内存镜像处理成合法的字节码。Java提供了抽象类ClassLoader，所有用户自定义类装载器都实例化自ClassLoader的子类。system class loader在没有指定装载器的情况下默认装载用户类，在Sun Java 1.5中既sun.misc.Launcher$AppClassLoader。更详细的内容请参看下面的资料。
准备heap dump
请看下面的Pilot类，没啥特殊的。
/**
 * Pilot class
 * @author rosen jiang
 */
package org.rosenjiang.bo;
public class Pilot{
    String name;
    int age;
    public Pilot(String a, int b){
        name = a;
        age = b;
    }
}
然后再看OOMHeapTest类，它是如何撑破heap dump的。
/**
 * OOMHeapTest class
 * @author rosen jiang
 */
package org.rosenjiang.test;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import org.rosenjiang.bo.Pilot;
public class OOMHeapTest {
    public static void main(String[] args){
        oom();
    }
    private static void oom(){
        Map<String, Pilot> map = new HashMap<String, Pilot>();
        Object[] array = new Object[1000000];
        for(int i=0; i<1000000; i++){
            String d = new Date().toString();
            Pilot p = new Pilot(d, i);
            map.put(i+"rosen jiang", p);
            array[i]=p;
        }
    }
}
是的，上面构造了很多的Pilot类实例，向数组和map中放。由于是Strong Ref，GC自然不会回收这些对象，一直放在heap中直到溢出。当然在运行前，先要在Eclipse中配置VM参数-XX:+HeapDumpOnOutOfMemoryError。好了，一会儿功夫内存溢出，控制台打出如下信息。
java.lang.OutOfMemoryError: Java heap space
Dumping heap to java_pid3600.hprof 
Heap dump file created [78233961 bytes in 1.995 secs]
Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
java_pid3600.hprof既是heap dump，可以在OOMHeapTest类所在的工程根目录下找到。
MAT安装
话分两头说，有了heap dump还得安装MAT。可以在[http://www.eclipse.org/mat/downloads.php](http://www.eclipse.org/mat/downloads.php)选择合适的方式安装。安装完成后切换到Memory Analyzer视图。在Eclipse的左上角有Open Heap Dump按钮，按照刚才说的路径找到java_pid3600.hprof文件并打开。解析hprof文件会花些时间，然后会弹出向导，直接Finish即可。稍后会看到下图所示的界面。
MAT工具分析了heap dump后在界面上非常直观的展示了一个饼图，该图深色区域被怀疑有内存泄漏，可以发现整个heap才64M内存，深色区域就占了99.5%。接下来是一个简短的描述，告诉我们main线程占用了大量内存，并且明确指出system class loader加载的"java.lang.Thread"实例有内存聚集，并建议用关键字"java.lang.Thread"进行检查。所以，MAT通过简单的两句话就说明了问题所在，就算使用者没什么处理内存问题的经验。在下面还有一个"Details"链接，在点开之前不妨考虑一个问题：为何对象实例会聚集在内存中，为何存活（而未被GC）？是的——Strong
 Ref，那么再走近一些吧。
点击了"Details"链接之后，除了在上一页看到的描述外，还有Shortest Paths To the Accumulation Point和Accumulated Objects部分，这里说明了从GC root到聚集点的最短路径，以及完整的reference chain。观察Accumulated Objects部分，java.util.HashMap和java.lang.Object[1000000]实例的retained heap(size)最大，在上一篇文章中我们知道retained heap代表从该类实例沿着reference
 chain往下所能收集到的其他类实例的shallow heap(size)总和，所以明显类实例都聚集在HashMap和Object数组中了。这里我们发现一个有趣的现象，既Object数组的shallow heap和retained heap竟然一样，通过Shallow and retained sizes一文可知，数组的shallow heap和一般对象（非数组）不同，依赖于数组的长度和里面的元素的类型，对数组求shallow heap，也就是求数组集合内所有对象的shallow heap之和。好，再来看org.rosenjiang.bo.Pilot对象实例的shallow
 heap为何是16，因为对象头是8字节，成员变量int是4字节、String引用是4字节，故总共16字节。
接着往下看，来到了Accumulated Objects by Class区域，顾名思义，这里能找到被聚集的对象实例的类名。org.rosenjiang.bo.Pilot类上头条了，被实例化了290,325次，再返回去看程序，我承认是故意这么干的。还有很多有用的报告可用来协助分析问题，只是本文中的例子太简单，也用不上。以后如有用到，一定撰文详细叙述。
又是perm gen
我们在上一篇文章中知道，perm gen是个异类，里面存储了类和方法数据（与class loader有关）以及interned strings（字符串驻留）。在heap dump中没有包含太多的perm gen信息。那么我们就用这些少量的信息来解决问题吧。
看下面的代码，利用interned strings把perm gen撑破了。
/**
 * OOMPermTest class
 * @author rosen jiang
 */
package org.rosenjiang.test;
public class OOMPermTest {
    public static void main(String[] args){
        oom();
    }
    private static void oom(){
        Object[] array = new Object[10000000];
        for(int i=0; i<10000000; i++){
            String d = String.valueOf(i).intern();
            array[i]=d;
        }
    }
}
控制台打印如下的信息，然后把java_pid1824.hprof文件导入到MAT。其实在MAT里，看到的状况应该和“OutOfMemoryError: Java heap space”差不多（用了数组），因为heap dump并没有包含interned strings方面的任何信息。只是在这里需要强调，使用intern()方法的时候应该多加注意。
java.lang.OutOfMemoryError: PermGen space
Dumping heap to java_pid1824.hprof 
Heap dump file created [121273334 bytes in 2.845 secs]
Exception in thread "main" java.lang.OutOfMemoryError: PermGen space
倒是在思考如何把class loader撑破废了些心思。经过尝试，发现使用ASM来动态生成类才能达到目的。ASM([http://asm.objectweb.org](http://asm.objectweb.org))的主要作用是处理已编译类(compiled class)，能对已编译类进行生成、转换、分析（功能之一是实现动态代理），而且它运行起来足够的快和小巧，文档也全面，实属居家必备之良品。ASM提供了core API和tree API，前者是基于事件的方式，后者是基于对象的方式，类似于XML的SAX、DOM解析，但是使用tree
 API性能会有损失。既然下面要用到ASM，这里不得不啰嗦下已编译类的结构，包括：
    1、修饰符（例如public、private）、类名、父类名、接口和annotation部分。
    2、类成员变量声明，包括每个成员的修饰符、名字、类型和annotation。
    3、方法和构造函数描述，包括修饰符、名字、返回和传入参数类型，以及annotation。当然还包括这些方法或构造函数的具体Java字节码。
    4、常量池(constant pool)部分，constant pool是一个包含类中出现的数字、字符串、类型常量的数组。
已编译类和原来的类源码区别在于，已编译类只包含类本身，内部类不会在已编译类中出现，而是生成另外一个已编译类文件；其二，已编译类中没有注释；其三，已编译类没有package和import部分。
这里还得说说已编译类对Java类型的描述，对于原始类型由单个大写字母表示，Z代表boolean、C代表char、B代表byte、S代表short、I代表int、F代表float、J代表long、D代表double；而对类类型的描述使用内部名(internal name)外加前缀L和后面的分号共同表示来表示，所谓内部名就是带全包路径的表示法，例如String的内部名是java/lang/String；对于数组类型，使用单方括号加上数据元素类型的方式描述。最后对于方法的描述，用圆括号来表示，如果返回是void用V表示，具体参考下图。
下面的代码中会使用ASM core API，注意接口ClassVisitor是核心，FieldVisitor、MethodVisitor都是辅助接口。ClassVisitor应该按照这样的方式来调用：visit visitSource? visitOuterClass? ( visitAnnotation | visitAttribute )*( visitInnerClass | visitField | visitMethod )* visitEnd。就是说visit方法必须首先调用，再调用最多一次的visitSource，再调用最多一次的visitOuterClass方法，接下来再多次调用visitAnnotation和visitAttribute方法，最后是多次调用visitInnerClass、visitField和visitMethod方法。调用完后再调用visitEnd方法作为结尾。
注意ClassWriter类，该类实现了ClassVisitor接口，通过toByteArray方法可以把已编译类直接构建成二进制形式。由于我们要动态生成子类，所以这里只对ClassWriter感兴趣。首先是抽象类原型：
/**
 * @author rosen jiang
 * MyAbsClass class
 */
package org.rosenjiang.test;
public abstract class MyAbsClass {
    int LESS = -1;
    int EQUAL = 0;
    int GREATER = 1;
    abstract int absTo(Object o);
}
其次是自定义类加载器，实在没法，ClassLoader的defineClass方法都是protected的，要加载字节数组形式（因为toByteArray了）的类只有继承一下自己再实现。
/**
 * @author rosen jiang
 * MyClassLoader class
 */
package org.rosenjiang.test;
public class MyClassLoader extends ClassLoader {
    public Class defineClass(String name, byte[] b) {
        return defineClass(name, b, 0, b.length);
    }
}
最后是测试类。
/**
 * @author rosen jiang
 * OOMPermTest class
 */
package org.rosenjiang.test;
import java.util.ArrayList;
import java.util.List;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
public class OOMPermTest {
    public static void main(String[] args) {
        OOMPermTest o = new OOMPermTest();
        o.oom();
    }
    private void oom() {
        try {
            ClassWriter cw = new ClassWriter(0);
            cw.visit(Opcodes.V1_5, Opcodes.ACC_PUBLIC + Opcodes.ACC_ABSTRACT,
            "org/rosenjiang/test/MyAbsClass", null, "java/lang/Object",
            new String[] {});
            cw.visitField(Opcodes.ACC_PUBLIC + Opcodes.ACC_FINAL + Opcodes.ACC_STATIC, "LESS", "I",
            null, new Integer(-1)).visitEnd();
            cw.visitField(Opcodes.ACC_PUBLIC + Opcodes.ACC_FINAL + Opcodes.ACC_STATIC, "EQUAL", "I",
            null, new Integer(0)).visitEnd();
            cw.visitField(Opcodes.ACC_PUBLIC + Opcodes.ACC_FINAL + Opcodes.ACC_STATIC, "GREATER", "I",
            null, new Integer(1)).visitEnd();
            cw.visitMethod(Opcodes.ACC_PUBLIC + Opcodes.ACC_ABSTRACT, "absTo",
            "(Ljava/lang/Object;)I", null, null).visitEnd();
            cw.visitEnd();
            byte[] b = cw.toByteArray();
            List<ClassLoader> classLoaders = new ArrayList<ClassLoader>();
            while (true) {
                MyClassLoader classLoader = new MyClassLoader();
                classLoader.defineClass("org.rosenjiang.test.MyAbsClass", b);
                classLoaders.add(classLoader);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
不一会儿，控制台就报错了。
java.lang.OutOfMemoryError: PermGen space
Dumping heap to java_pid3023.hprof 
Heap dump file created [92593641 bytes in 2.405 secs]
Exception in thread "main" java.lang.OutOfMemoryError: PermGen space
打开java_pid3023.hprof文件，注意看下图的Classes: 88.1k和Class Loader: 87.7k部分，从这点可看出class loader加载了大量的类。
更进一步分析，点击上图中红框线圈起来的按钮，选择Java Basics——Class Loader Explorer功能。打开后能看到下图所示的界面，第一列是class loader名字；第二列是class loader已定义类(defined classes)的个数，这里要说一下已定义类和已加载类(loaded classes)了，当需要加载类的时候，相应的class loader会首先把请求委派给父class loader，只有当父class loader加载失败后，该class loader才会自己定义并加载类，这就是Java自己的“双亲委派加载链”结构；第三列是class
 loader所加载的类的实例数目。
在Class Loader Explorer这里，能发现class loader是否加载了过多的类。另外，还有Duplicate Classes功能，也能协助分析重复加载的类，在此就不再截图了，可以肯定的是MyAbsClass被重复加载了N多次。
最后
其实MAT工具非常的强大，上面故弄玄虚的范例代码根本用不上MAT的其他分析功能，所以就不再描述了。其实对于OOM不只我列举的两种溢出错误，还有多种其他错误，但我想说的是，对于perm gen，如果实在找不出问题所在，建议使用JVM的-verbose参数，该参数会在后台打印出日志，可以用来查看哪个class loader加载了什么类，例：“[Loaded org.rosenjiang.test.MyAbsClass from org.rosenjiang.test.MyClassLoader]”。
全文完。
参考资料
memoryanalyzer Blog
java类加载器体系结构
ClassLoader
请注意！引用、转贴本文应注明原作者：Rosen Jiang 以及出处： [http://www.blogjava.net/rosen](http://www.blogjava.net/rosen)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/kepoon/archive/2011/01/07/6122660.aspx](http://blog.csdn.net/kepoon/archive/2011/01/07/6122660.aspx)

