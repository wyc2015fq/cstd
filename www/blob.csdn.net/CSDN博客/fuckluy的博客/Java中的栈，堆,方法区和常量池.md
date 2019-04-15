# Java中的栈，堆,方法区和常量池 - fuckluy的博客 - CSDN博客












2016年01月31日 16:32:40[Zcc?](https://me.csdn.net/fuckluy)阅读数：12774








 要说Java中的栈，堆,方法区和常量池就要提到HotSpot,HotSpot是Sun JDK 和 Open JDK中所带的虚拟机。   （Sun JDK 和 Open JDK除了注释不同，代码实现基本上是一样的）

以下说的内容都是围绕HotSpot。



Stack(栈)：分为VM Stack(虚拟机栈)和Native Method Stack（本地方法栈），不过HotSpot虚拟机直接把本地方法栈和虚拟机栈合二为一了。 

                  虚拟机栈： 线程私有的， 描述的是Java**方法执行的内存模型**，方法调用的同时创建一个**栈帧**（储存局部变量表，操作数栈，方法出口等等），每个方法的调用直到执行完成对应的是**栈帧**在虚拟机中入栈和出栈的过程。

**局部变量表（通常说的栈其实是栈里面的局部变量表）**：存放基本数据类型变量和对象的引用（所需内存在编译时期完成分配，方法运行时期不改变局部变量表大小，四个字节占用一个局部变量空间）



 --------栈中的数据可以共享：

             int a = 3;       int b = 3；   编译器先处理int a = 3；首先它会在栈中创建一个变量为a的引用，然后查找栈中是否有3这个值，如果没找到，就将3存放 进来，然后将a指向3。接着  处理int b = 3；在创建完b的引用变量后，因为在栈中已经有3这个值，便将b直接指向3。这样，就出现了a与b同时均指向3的情况。这时，如果再令  a=4；那么编译器会重新搜索栈中是否有4值，如果没有，则将4存放进来，并令a指向4；如果已经有了，则直接将a指向这个地址。因此a值的改变不  会影响到b的值。要注意这种数据的共享与两个对象的引用同时指向一个对象的这种共享是不同的，因为这种情况a的修改并不会影响到b, 它是由编译  器完成的，它有利于节省空间。而一个对象引用变量修改了这个对象的内部状态，会影响到另一个对象引用变量。 

包装类数据，如Integer, String, Double等将相应的基本数据类型包装起来的类。这些类数据全部存在于堆中，Java用new()语句来显示地告诉编译器，在运行时才根据需要动态创建，因此比较灵活，但缺点是要占用更多的时间。





==比较的是对象的地址，也就是是否是同一个对象；


equal比较的是对象的值。

int 变为Integer 时  如果值在-128～127之间  则不会创建新的integer对象 储存常量池中，这么做的目的是提高效率----->记得在别的地方看过不知道对不对



Java代码  public class Test {        public static void main(String[] args)        {  int a1=1;          int b1=1;          int c1=2;          int d1=a1+b1;          Integer a = 1;            Integer b = 2;            Integer c = 3;            Integer d = 3;            Integer e = 321;            Integer f = 321;            Long g = 3L;            System.out.println(a1==b1);   //true  结果1            System.out.println(c1==d1);   //true  结果2          System.out.println(c==d);   //true  结果3             System.out.println(e==f);   //false  结果4           }    }     分析：结果1：a1==b1如上面所述,会在栈 中开辟存储空间存放数据。          结果2：首先它会在栈 中创建一个变量为c1的引用，然后查找有没有字面值为2的地址，没找到，就开辟一个存放2这个字面值的地址，然后将c1指向2的地址,d1为两个字面值相加也为2， 由于在栈中已经有2这个字面值，便将d1直接指向2的地址。这样，就出现了c1与d1同时均指向3的情况。        在分析下面结果以前让我们先对Java自动拆箱和装箱做个了结：在自动装箱时，把int变成Integer的时候，是有规则的，当你的int的值在-128-IntegerCache.high(127) 时，返回的不是一个新new出来的Integer对象，而是一个已经缓存在堆 中的Integer对象，（我们可以这样理解，系统已经把-128到127之 间的Integer缓存到一个Integer数组中去了，如果你要把一个int变成一个Integer对象，首先去缓存中找，找到的话直接返回引用给你就 行了，不必再新new一个），如果不在-128-IntegerCache.high(127) 时会返回一个新new出来的Integer对象。         结果3：由于3是在范围内所以是从缓存中取数据的，c和d指向同一个对象，结果为true;         结果4：由于321不是在范围内所以不是从缓存中取数据的而是单独有new对象，e和f并没有指向同一个对象，结果为false;

JAVA堆：

Java堆是被所有线程共享的一块区域，在虚拟机启动时创建 ，此内存的唯一目的就是存放对象实例和数组 ** GC** 管理的主要区域。

**分为新生代（Eden Survivor Survivor8:1:1）和老年代**

**Java堆可以处于物理上不连续的内存空间中，只要逻辑连续即可**

关于对象的创建请参考：[http://note.youdao.com/yws/public/redirect/share?id=5177014ee5ad1ac3f0af9fdab3b011a3&type=false](http://note.youdao.com/yws/public/redirect/share?id=5177014ee5ad1ac3f0af9fdab3b011a3&type=false)关于GC回收请参考：[http://note.youdao.com/yws/public/redirect/share?id=96928e82082b4dec8831e5099769172b&type=false](http://note.youdao.com/yws/public/redirect/share?id=96928e82082b4dec8831e5099769172b&type=false)



方法区：

不等价于永久代hotspot用永久代实现方法区（在jdk1.7的HotSpot中 已经把原本放在永久代中的字符串常量池移出）

**与堆一样是线程共享的一块内存区域**。

              用于储存已被虚拟机加载的**类**信息、常量、静态变量、即时编译器编译后的代码等数据。

             垃圾收集行为在方法区很少出现，这块区域回收的主要目标是针对**常量池的回收和对类型的卸载  **



运行时常量池：

**                       方法区的一部分**

**常量池**用于存放编译期生成的各种**字面量**和**符号引用（还有翻译出来的直接引用）**，这部分内容在类加载后进入方法区的运行时常量池中存放。 

                       运行时常量池相对于Class文件常量池的另一个重要特征是具备动态性，运行期间也可能将新的常量放入池中

 字面量：如文本字符串，声明为final的常量值等。

public stick final int i =3;

String s="abc";

  符号引用：类和接口的全限定名  

字段的名称和描述符

                                方法的名称和描述符



String 的本质是字符数组。 

String 的标准实现含有4个实例变量 

                                        指向字符数组的引用       

                                        int类型    偏移量

                                        int类型    字符串的长度

                                        int类型     散列值




public class String{

      private char[]value;

      private int offset;

      private int count;

      private int hash;

...

}




一个String对象总共使用40字节（16字节对象本身开销+8字节的引用+三个int类型（12字节）+4字节（填充字节））,因为String的char数组常常是在多个字符串之间共享的，因此String对象是不可变的



String是一个特殊的包装类数据。即可以用String str = new String("abc");的形式来创建，也可以用String str = "abc"；的形式来创建

String str = "abc"创建对象的过程1 首先在常量池中查找是否存在内容为"abc"字符串对象2 如果不存在则在常量池中创建"abc"，并让str引用该对象3 如果存在则直接让str引用该对象


至 于"abc"是怎么保存，保存在哪？常量池属于类信息的一部分，而类信息反映到JVM内存模型中是对应存在于JVM内存模型的方法区，也就是说这个类信息 中的常量池概念是存在于在方法区中，而方法区是在JVM内存模型中的堆中由JVM来分配的，所以"abc"可以说存在于堆中（而有些资料，为了把方法区的 堆区别于JVM的堆，把方法区称为栈）。一般这种情况下，"abc"在编译时就被写入字节码中，所以class被加载时，JVM就为"abc"在常量池中 分配内存，所以和静态区差不多。4(2)String str = new String("abc")创建实例的过程1 首先在堆中（不是常量池）创建一个指定的对象"abc"，并让str引用指向该对象2 在字符串常量池中查看，是否存在内容为"abc"字符串对象3 若存在，则将new出来的字符串对象与字符串常量池中的对象联系起来4 若不存在，则在字符串常量池中创建一个内容为"abc"的字符串对象，并将堆中的对象与之联系起来

String str1 = "abc"; String str2 = "ab" + "c"; str1==str2是ture是因为String str2 = "ab" + "c"会查找常量池中时候存在内容为"abc"字符串对象，如存在则直接让str2引用该对象，显然String str1 = "abc"的时候，上面说了，会在常量池中创建"abc"对象，所以str1引用该对象，str2也引用该对象，所以str1==str2String str1 = "abc"; String str2 = "ab"; String str3 = str2 + "c"; str1==str3是false                ---------//可参考Java编程思想第四版P284，285是因为String str3 = str2 + "c"涉及到变量（不全是常量）的相加，所以会生成新的对象，其内部实现是先new一个StringBuilder，然后 append(str2),append("c");然后让str3引用toString()返回的对象







    String s = new String(“abc”); 产生几个对象？

    一个或两个，如果常量池中原来没有 ”abc”, 就是两个（参考栈中的数据共享）。








