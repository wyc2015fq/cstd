# Java代码执行顺序

 

### 前言

YY：Kitty，我最近在看*Thinking in Java* 这本书
 Kitty：喔？是么，你不是一直觉得那本书又厚又乏味，代码还非常不用户友好，难以阅读，而总是停留在第一章么，这次不会还是停留在第一章吧（_ _）
 YY：好啦，这不人家感觉Java基础还是需要打打扎实么，所以就只能硬着头皮看这本被誉为Java届圣经的神书咯！好不容易下定决心一定要认认真真看，你就别拿人家打趣儿了（捧脸害羞状）
 Kitty：好样的嘛，那祝你收获大大的咯！
 YY：嘿嘿！昨天看了第五章--初始化和清理，才发现写了那么久的Java，都还没认真研究过从点击“运行”那一刻开始，我们的代码都是以怎样的规则和顺序一条一条被JVM执行的呢，你有想过这个问题么？
 Kitty：呃呃。。。这个嘛，还真没考虑过，那么你说说是怎么回事呗！
 YY：好呀，正好我昨天写了一个小Demo帮助我理解，那么我就用这个小Demo和你讲讲，顺便巩固一下我的知识吧，要是有讲错的或者讲得不好的地方，你可得帮我指出来哈，不然我还糊里糊涂的以为自己都理解对了呢！
 Kitty：好的好的，木有问题（亲爱的读者朋友们，你们要是发现文中有不足之处，一定一定要给小编我指出来哈，在此先行谢过了_）
 YY：先上小Demo

<a name="demo">小Demo</a>

```
// 父类
public class Father {
    // 非静态变量
    C fa = new C("fa");
    
    // 静态变量
    static C fb = new C("fb");

    // 静态语句块
    static {
        OutUtil.print("Static blocks 1 in Father! ");
    }

    // 静态语句块
    static {
        OutUtil.print("Static blocks 2 in Father! ");
    }
    
    // 静态常量
    static final int T = 28;

    // 构造方法
    public Father() {
        super();
        OutUtil.print("Construct method in Father! ");
    }

    // 带参构造器
    public Father(String name) {
        OutUtil.print("Construct method in Father! " + "Name = " + name);
    }

    // 非静态代码块
    {
        OutUtil.print("Common blocks in Father! ");
    }

    // 静态方法
    static void staticShow() {
        OutUtil.print("Static method in Father! ");
    }

    // 非静态方法
    void show() {
        OutUtil.print("Common method in Father! ");
    }

}
```

```
// 子类
public class Child extends Father {

    // 非静态变量
    C ca = new C("ca");

    // 静态常量
    static final int T = 28;

    // 构造方法
    public Child() {
        super();
        OutUtil.print("Construct method in Child! ");
    }

    // 带参构造器
    public Child(String name) {
        OutUtil.print("Construct method in Child! " + "Name = " + name);
    }

    // 非静态代码块
    {
        OutUtil.print("Common blocks in Child! ");
    }

    // 静态方法
    static void staticShow() {
        OutUtil.print("Static method in Child! ");
    }

    // 非静态方法
    void show() {
        OutUtil.print("Common method in Child! ");
    }

    // 静态变量
    static C cb = new C("cb");

    // 静态语句块
    static {
        OutUtil.print("Static blocks 1 in Child! ");
    }

    // 静态语句块
    static {
        OutUtil.print("Static blocks 2 in Child! ");
    }

}
```

```
// 辅助类
public class C {
    public static final String A = "A in C";
    
    public static String showC() {
        return "showC method in C!";
    }
    
    public C(){
        OutUtil.print("Construct method in C!");
    }
    
    public C(String msg){
        OutUtil.print("Construct method in C! " + msg);
    }
}
```

```
// 入口程序所在类
public class Main {
    
    C ma = new C("ma");     // 打印结果显示ma并未进行初始化
    static C mb = new C("mb");  
    
    public Main(){
        OutUtil.print("I am Main!");
    }
    
    static{ 
        OutUtil.print(mb.getClass().getCanonicalName());
    }
    
    public static void main(String[] args) {
        OutUtil.print("Main");
        Child child = new Child();
        child.show();
        OutUtil.print(C.A);
        OutUtil.print(C.showC());
    }
    
    static Child mc = new Child("mc");
}
```

```
// 打印输出的工具类
public class OutUtil {
     public static void print(Object o) {
        System.out.println(o);
    }
}
```

```
// 程序运行输出结果
Construct method in C! mb
classloadertest.C
Construct method in C! fb
Static blocks 1 in Father! 
Static blocks 2 in Father! 
Construct method in C! cb
Static blocks 1 in Child! 
Static blocks 2 in Child! 
Construct method in C! fa
Common blocks in Father! 
Construct method in Father! 
Construct method in C! ca
Common blocks in Child! 
Construct method in Child! Name = cb
Main
Construct method in C! fa
Common blocks in Father! 
Construct method in Father! 
Construct method in C! ca
Common blocks in Child! 
Construct method in Child! 
Common method in Child! 
A in C
showC method in C!
```

Kitty：控制台输出了好多语句呀，看得我眼睛都花了，你给我指点指点撒
 YY：OK，先给你画个图吧！



![img](https://upload-images.jianshu.io/upload_images/1744622-a52bcb5b819af14d.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

结果分析.PNG

YY：看了上面的图应该可以大致看出哪些语句被执行了，哪些语句没有被执行，下面再给你分析分析。

### Demo分析

上面的小Demo中含有四个类：Father、Child、C和Main，其中Child类继承自Father类；C类是一个辅助类，主要用于打印输出，便于分析程序的运行流程；Main类是Java程序入口方法main方法所在的类，也是Java程序运行时加载的第一个类。

下面根据上图分析该Demo的运行流程。从图中可以看出，程序首先执行了Main类中的静态域和静态代码块（静态域和静态代码块的具体执行顺序依代码中定义的顺序而定），然后执行main方法中的代码，根据main方法中的代码需要，再加载所需要的类并进行初始化。

**注：**

1. 整个程序执行过程中并没有执行Main类中的非静态域，也没有执行Main类的初始化方法，由此可知，并没有实例化Main类；
2. 图中第一个大矩形框执行的是Main类最后一行语句，即初始化静态域mc。从矩形框中可以看出创建一个类实例的流程：
    a. 首先执行父类中的静态域和静态代码块（静态域和静态代码块的具体执行顺序依代码中定义的顺序而定）；
    b. 然后执行子类中的静态域和静态代码块；
    c. 然后执行父类中的非静态域和非静态代码块；
    d. 接着执行父类的初始化方法（如果子类在构造函数中明确指明了调用父类的哪一个构造函数，则调用相应的构造函数，否则调用父类的无参构造函数）
    e. 然后执行子类中的非静态域和非静态代码块；
    d. 最后执行子类的初始化方法（即被调用的那一个构造函数）。
3. static Child mc = new Child("mc")和Child child = new  Child()这两句都是实例化一个Child对象，从下图可以看出，类中的静态域只进行了一次初始化操作，而非静态域则进行了两次初始化，由此可知，一个类中的静态域和静态代码块只会在类第一次加载时进行初始化，而非静态域和非静态代码块则会在每一次实例化时均执行。



![img](https://upload-images.jianshu.io/upload_images/1744622-4c5d8baeac4e4433.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/865)

对象实例化(1)

Kitty：那也可能是因为上面的代码中第一次是初始化一个静态实例，第二次只是初始化一个非静态实例呀？
 YY：问得很好，要解开这个疑惑很简单，我们变一下程序流程就好了。

```
public class Main {
    
    C ma = new C("ma");
    static C mb = new C("mb");
    
    public Main(){
        OutUtil.print("I am CallbackMain!");
    }
    
    static{ 
        OutUtil.print(mb.getClass().getName());
    }
    
    public static void main(String[] args) {
        OutUtil.print("Main");
        Child mc = new Child("mc");
        Child child = new Child();
        child.show();
        OutUtil.print(C.A);
        OutUtil.print(C.showC());
    }
    
//  static Child mc = new Child("mc");
}
```



![img](https://upload-images.jianshu.io/upload_images/1744622-99d86323ff572b22.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/818)

对象实例化(2)

YY：上面的代码和图显示了同时在main方法中实例化两个Child对象的执行流程，可以看到
 上面的第3点观察结论是正确的--类中的静态域只会在类第一次加载时执行，而非静态域则会在每一次实例化时均执行。
 Kitty：确实是这样的呀，看来！

YY：下面用一个流程图简易表示Java中对象的初始化顺序以加深记忆吧！Java中，没有显式使用extends继承的类都默认继承自Object类，也就是说，除Object类以外，每个类都会有一个显式或隐式的父类，并且任何对象的初始化都会自Object类开始。



![img](https://upload-images.jianshu.io/upload_images/1744622-bc011d9a36beec16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/238)

对象初始化顺序

Kitty：好啦，通过你的小Demo演示以及你的观察结论，我已经大致清楚了Java程序的执行流程以及在Java中，创建一个对象会经历哪些过程了。可是。。。
 YY：可是什么？？？
 Kitty：你不说还好，被你这么一说吧，我的脑子里冒出了一堆？？？
 YY：哈哈，有问号说明你还在思考，说说都有哪些疑问，我们一起把它们变成！！！呗
 Kitty：我现在主要有一下几个困惑：

1. 为什么一个对象的初始化过程是这样的呢？
2. 上面的Demo显示Father类和Child类会执行非静态域、非静态初始化块和构造方法，但是Main类中的非静态域与初始化方法却并未执行，这又是怎么一回事呢？
3. 那么，Java中一个类的初始化过程又是怎样的呢，和对象的初始化过程又有什么区别和联系？
4. static关键字好神奇的样子，可是该怎么用呢？

YY：问题已经抛出来了，接下来就开始解决它们。

### 预备知识

YY：看样子你的Java基础也不咋地呀，那么先给你介绍几个基本概念作为预备知识热热身吧，不然后面你又该冒？？？了（_）

- **普通代码块：**在方法或语句中出现的 **{}** 就称为普通代码块，普通代码块和一般的语句执行顺序由他们在代码中出现的次序决定--**“先出现先执行”**；
- **构造块：**直接在类中定义且没有加static关键字的代码块 **{}** 称为构造代码块（构造块）。用来初始化每一个对象的非静态变量，构造块在每次创建 **对象 **时均会被调用，并且 **构造块的执行次序优先于类构造函数 **；
- **静态代码块：** 在 Java 中使用 static 关键字声明的代码块 **{} 称为静态代码块（静态初始块）。静态块用于初始化类，为类的属性初始化。每个静态代码块只会在Class对象首次加载时执行一次。**由于JVM在加载类时会执行静态代码块，所以**静态代码块先于主方法执行**。如果类中包含多个静态代码块，那么将按照 **"先出现先执行" **的顺序执行。
  - 静态代码块不能存在于任何方法体内
  - 静态代码块不能直接访问非静态实例变量和实例方法，需要通过类的实例对象来访问
  - 即便没有显式使用使用static关键字，构造器实质上也是静态方法（出自Thinking in Java）
- **JVM中的内存区域：** Java 程序执行时需要先被 JVM 加载进内存，为了提高程序运算效率， JVM 会将不同类型的数据加载进内存中的不同区域，因为每一片区域均有不同的内存管理方式和数据处理方式！JVM 中几个比较重要的区域为：
  - 程序计数器：每个线程拥有一个PC寄存器，在线程创建时创建，指向下一条指令的地址，执行本地方法时，PC的值为undefined
  - 方法区：保存装载的类信息，如类型的常量池、字段、方法信息、方法字节码等，通常和永久区(Perm)关联在一起
  - 堆区：用于存放类的对象实例，**为所有线程所共享** 
  - 栈区：  也叫java虚拟机栈，是由一个一个的栈帧组成的后进先出的栈式结构，栈桢中存放方法运行时产生的局部变量、方法出口等信息。当调用一个方法时，虚拟机栈中就会创建一个栈帧存放这些数据，当方法调用完成时，栈帧消失，如果方法中调用了其他方法，则继续在栈顶创建新的栈桢。栈区是  **线程私有** 的，生命周期和线程相同

YY：热身活动完成，进入正题！

### Java类的生命周期



![img](https://upload-images.jianshu.io/upload_images/1744622-f571473c431ac97e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/718)

Java类生命周期

当我们编写一个java的源文件后，经过编译会生成一个后缀名为 **.class  **的文件，这种文件叫做字节码文件，只有这种字节码文件才能够在 Java 虚拟机中运行， **Java  类的生命周期就是指一个class文件从加载到卸载的全过程 **，如上图所示。

1.  **装载：**在装载阶段，JVM会通过一个类的全限定名获取描述此类的.class文件，然后通过这个.class文件将类的信息加载到  JVM 的方法区，然后在堆区中实例化一个java.lang.Class对象，作为方法区中这个类的信息的入口。虚拟机设计团队把加载动作放到  JVM 外部实现，以便让应用程序决定如何获取所需的类，实现这个动作的代码被称为 **“类加载器”** 。至于何时加载一个类， JVM 并没有一个统一的规范，所以不同的虚拟机可能采取不同的加载策略，有些虚拟机会选择 **在执行前就预先加载类** ，而另一些虚拟机则会在 **真正需要使用到一个类的时候才会加载** 。但无论如何，一个类总是会在 JVM “预期”到即将会使用之前被加载。常用的hotspot虚拟机采取的是懒加载原则，即等到真正需要使用到一个类时才加载这个类；
2.  **连接：** JVM 将已读入的二进制文件合并到 JVM 的运行时状态的过程，这个过程由 **验证、准备和解析** 三个子步骤构成

1.  **验证：**确认该类型符合 Java 语言的语义，并且该类型不会危及 JVM 的完整性，主要包括 **格式验证、元数据验证、字节码验证和符号引用验证** 等；
2.  **准备：**在准备阶段，JVM 为 **类变量** （所谓类变量就是被 static 关键字修饰的变量）分配内存，**设置默认的初始值**，（默认值的设置过程是通过将此片内存区清零实现的，即通过将对象内存设为二进制零值而一举生成）此时默认值设置如下，而非在代码中赋予的值（在准备阶段并不会执行 Java 代码）：

- 基本类型（int、long、short、char、byte、boolean、float、double）： 默认值为0；
- 引用类型： 默认值为 null；
- 常量: 默认值为程序中设定的值，比如我们在程序中定义final static int a = 8，则准备阶段中a的初值就是8；

```
private static int a = 8;  
// 上面这句在准备阶段只会将 a 初始化为0，需要等到后面的初始化阶段，才会将 a 赋值为8
private static final int A = 8;
// 这句例外，因为上面这句表明 A 是一个编译期常量
// 所以在编译阶段会为 A 生成 ConstantValue 属性，在准备阶段虚拟机会根据 ConstantValue 属性将 A 赋值为8
```

1. *解析：*

    在类的常量池中寻找类、接口、方法和字段的符号引用，将符号引用替换为直接引用的过程，实质上，在符号引用被程序首次使用以前，这个过程都是可选的。 

   - 符号引用：使用一组符号来描述所引用的目标，可以是任何形式的字面常量，定义在Class文件格式中
   - 直接引用：可以是直接指向目标的指针、相对偏移量或能间接定位到目标的句柄

1. **初始化：**即为 **类变量 **赋予 **“正确” **的初始值的过程，（“正确”的初始值是指代码中希望这个类变量拥有的初始值）也就是上面的小Demo中的执行静态域和静态代码块的过程（后面详述类变量初始化过程）；
2. **对象生命：**这个就很好理解了，一旦一个类完成了装载、连接和初始化这三个过程，这个类就随时可以被使用了，包括调用类变量、类方法以及实例化类等。每次对类进行实例化操作时都会创建该类的一个新的对象，开启该对象的生命周期。对象的生命周期包含三个阶段：

1).*** 对象实例化：***  即对象的初始化阶段，在本阶段完成对象初始化工作，回看上面的Demo，即执行类中的非静态域和非静态代码块部分，然后执行类的构造函数中的代码。具体流程为：当通过显式或隐式的方式创建一个类的实例时，JVM  会首先为该类及其所有超类中的实例变量在堆中分配内存，然后 JVM 会将该块内存空间清零，从而将实例变量初始化为 **默认的初始值** （数字、字符和布尔型变量初始化为0，引用类型变量初始化为null），然后根据我们在代码中书写的内容，为实例变量赋予正确的初始值；完成对象实例化过程后，就可以通过该对象的引用使用对象了，如调用对象的方法、获取对象中某个域的值等；

2). *** 垃圾收集：*** 当一个对象不再被引用的时候，JVM 就可以将这个对象所占据的内存回收，从而使得该部分内存可以被再次使用，垃圾回收时机、策略等是一个非常复杂的过程，具体可以参见*深入Java虚拟机*一书；

3). **对象终结：**当一个对象被垃圾收集器收集后，该对象就不复存在，也就是说该对象的生命周期结束；

1.  **类卸载：**类卸载是类生命周期的最后一个过程，当程序不再引用某一类型时，那么这个类型就无法再对未来的计算过程产生影响，从而该类就可以被 JVM 垃圾回收。

YY：上面大致描述了 Java  中一个类的生命周期流程，本文并不会对每个过程都进行深入细致的分析，那样的话会陷入到细节陷阱中无法自拔，如果你对哪个部分有疑惑或者感兴趣的话，可以去查阅相关资料详加了解，下面详细讲解一下  Java 类的初始化过程，毕竟这才是本文的重点嘛_
 Kitty：好的，好的！我现在正兴趣浓厚，快开始讲吧！
 YY：瞧你那猴急猴急的样，平时也没见你这么认真学习了！那我开始咯，你好好听哈。

### Java类初始化

为了让一个类/接口被**首次主动使用**，在加载、连接过程完成后，JVM 会执行类初始化过程。前面已经简要介绍过，类初始化时会执行类变量初始化语句和静态语句块，将准备阶段赋予类变量的默认初始值替换为“正确”的初始化值。

Kitty：** 首次 **我知道，不就是第一次么，可是 **主动使用** 是个什么鬼？
 YY：别急撒，接下来就为你揭开它的神秘面纱

#### 主动使用 VS 被动使用

- *主动使用：*

  在 Java 中只有如下几种活动被视为主动使用 

  - 创建类的新实例；
  - 调用类中声明的静态方法；
  - 操作类/接口中声明的 **非常量** 静态域；
  - 调用 Java 中的反射方法；
  - 初始化一个类的子类；
  - 指定一个类作为 JVM 启动时的初始化类，即 main 函数所在的类。

-  **被动使用：**不属于上述六种情况的活动均被视之为被动使用

** 注：被动使用一个类时并不会触发类初始化过程**， 如

- 非首次主动使用一个类时不会触发类的初始化过程，也就是所第二次主动使用一个类也不再会触发类的初始化
- 使用一个类的非常量静态字段时，只有当该字段确实是由当前类/接口声明的时才可以称之为主动使用，否则是被动使用。比如说，当通过子类调用父类的  public 的非常量静态域时，对于子类来说这是被动使用，对于父类才是主动使用，所以会触发父类的初始化，而不会触发子类的初始化
- 如果一个变量被 static 和 final  同时修饰，并且使用一个编译期常量表达式进行初始化，那么对这样的字段的使用就不是对声明该字段的类的主动使用，因为  Java编译器会把这样的字段解析成对常量的本地拷贝（该常量存在于引用者的字节码流中或者常量池中，或二者均有）
- 定义一个类的对象数组时并不会触发该类的初始化

Kitty：哎呀，一会儿是主动使用，一会儿又不是主动使用的，这些抽象的概念一点都不好理解，我的脑子里现在就像小燕子说的了--全是浆糊了（衰）
 YY：抽象的东西确实不太好理解和记忆，下面沿用上面小 Demo 中出现的类写几个小例子，你就会有所理解了。

#### 示例说明

- 非首次主动使用一个类时也不会触发类的初始化过程

```
public class InitTest {
    public static void main(String[] args) {
        Father.staticShow();    //首次主动使用
        Father.staticShow();   //非首次主动使用
    }
}
```

程序输出为：

```
Construct method in C! fb
Static blocks 1 in Father! 
Static blocks 2 in Father! 
Static method in Father! 
Static method in Father! 
```

由输出可知，虽然在代码中先后两次执行了Father中的静态方法staticShow()，但是Father类的初始化过程只执行了一次。

- 通过子类调用父类的静态域

```
public class InitTest {
    public static void main(String[] args) {
        OutUtil.print(Child.str);  // str是Father类中的静态字符串，初始值为“str in Father”
    }
}
```

程序输出为：

```
Construct method in C! fb
Static blocks 1 in Father! 
Static blocks 2 in Father! 
str in Father
```

输出显示，只执行了Father类的初始化代码，而未执行Child类的初始化代码。

- 调用编译期常量不会触发类初始化

```
public class InitTest {
    public static void main(String[] args) {
        OutUtil.print(Father.T);
    }
}
```

程序输出为：

```
28
```

由此可见并未触发类Father的初始化操作。

- 定义一个类的对象数组时并不会触发该类的初始化

```
public class InitTest {
    public static void main(String[] args) {
        Father[] cArray = new Father[8];
    }
}
```

执行上面这段代码后，控制台并未产生输出，这就说明并未初始化Father类。可以通过查看这段代码产生的字节码文件加以验证。



![img](https://upload-images.jianshu.io/upload_images/1744622-450493c012fbd610.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/638)

类字节码

由上图可以看到，首先执行了Object类的初始化方法，然后执行InitTest类中的main方法，其中anewarray指令为新数组分配空间，但并未触发类Father的初始化。

Kitty：喔！！！原来主动使用和被动使用是这样的呀，Java 加载类，对类进行初始化的时机为首次主动使用的时候，可是你还是没有讲 JVM 执行类初始化操作的具体流程呀。
 YY：OK，接下来就是了。

#### 如何执行类的初始化操作？

在 Java 类和接口的 class 文件中有一个只能够被 JVM  调用的<clinit>()方法，这个方法会将类/接口的所有类变量初始化语句和静态初始化块均收集起来，然后在需要执行类初始化操作时，JVM  便调用该方法为类变量赋予“正确”的初始值。具体由以下两个步骤构成：

1. 如果类存在直接超类，并且直接超类还未被初始化，则先初始化超类；
2. 如果类存在类初始化方法，则执行该初始化方法；
    在执行超类的初始化时也是通过这两个步骤完成，因此，程序中第一个被初始化的类永远是Object类，其次是被主动使用的类继承层次树上的所有类，超类总是先于子类被初始化。

#### 注：

- 初始化接口时并不需要先初始化其父接口，只有使用父接口中定义的变量时，才会执行父接口的初始化动作
- <clinit>()方法的代码并不会显式调用超类的<clinit>()方法，JVM 在调用类的<clinit>()方法时会先确认超类的<clinit>()方法已经被正确调用
- 为了防止多次执行<clinit>，JVM  会确保<clinit>()方法在多线程环境下被正确的加锁同步执行。当有多个线程需要对一个类执行初始化操作时，只能由一个线程来执行，其它线程均处于等待状态，当活动线程执行完成后，必须通知其它线程
- 并非所有的类均会在class文件中拥有<clinit>()方法，只有那些的确需要执行 Java  代码来赋予类变量正确的初始值的类才会有<clinit>()方法。下面几种情况下，类的class文件中不会包含<clinit>()方法： 
  - 类中没有声明任何类变量，也没有包含静态初始化块；

```
public class Test1 {
    int a = 8;
    int add(){return ++a;}
}
```

- 虽然类声明了类变量，但是并没有明确使用类变量初始化语句或静态初始化语句对它们进行初始化；

```
public class Test2 {
    static int c;
}
```

- 类中仅包含static final变量的初始化语句，并且初始化语句是编译期常量表达式

```
public class Test3 {
    static final int A = 8;
    static final int B = A * 8;
}
```

上面代码中，A和B均是编译时常量，JVM 在加载 test 类时，不会将A、B作为类变量保存到方法区，而是会被当做常量，被 Java 编译器特殊处理。因此，不需要<clinit>()方法来对它们进行初始化。

YY：OK，到此 Java 类的初始化部分就结束啦，下面由Kitty你来说说你的理解，然后再回顾一下上面的小 Demo呗。
 Kitty：好的，正好回顾一下，不然知识都是零零散散的，一下就忘了。

### Demo回顾

下面将 Java 程序入口类 Main 的代码提出来了，其余代码见[小Demo](https://www.jianshu.com/p/55c86e6c5c60#demo)

```
// 入口程序所在类
public class Main {

    C ma = new C("ma");        // 打印结果显示ma并未进行初始化
    static C mb = new C("mb");    

    public Main(){
        OutUtil.print("I am Main!");
    }

    static{    
        OutUtil.print(mb.getClass().getCanonicalName());
    }

    public static void main(String[] args) {
        OutUtil.print("Main");
        Child child = new Child();
        child.show();
        OutUtil.print(C.A);
        OutUtil.print(C.showC());
    }

    static Child mc = new Child("mc");
}
```

当前程序运行流程：

1. main 方法所在的类为 Main， 所以 JVM 会先加载 Main类；
2. 完成 Main 类的连接步骤，将 Main 类中的静态域 mb 和 mc 初始化为null；
3. 初始化 Main 类，执行 Main 类中的静态块，此时会执行如下几句：

```
static C mb = new C("mb");
static{ 
        OutUtil.print(mb.getClass().getName());
    }
static Child mc = new Child("mc");
```

1. 执行 main 方法

static Child mc = new Child("mc")一句可以更加详细地说明类初始化流程。

1. 首先，JVM 会加载该句所需要的类，因为 Child 类是 Father 类的子类，所以首先加载 Father 类；
2. 连接 Father 类；
3. 初始化 Father 类（即会执行 Father 类中的静态块和静态域初始化语句）；
4. 加载 Child 类；
5. 连接 Child 类；
6. 初始化 Child 类；
7. 执行 Father 类的非静态域初始化语句和构造块；
8. 执行 Father 类的构造方法；
9. 执行 Child 类的非静态域初始化语句和构造块；
10. 执行 Child 类的构造方法。

### 总结

1. 只有在应用程序 **首次主动使用** 一个类时，JVM 才会对这个类进行初始化；
2. 类的生命周期主要有如下几个阶段：加载--连接--初始化--[对象生命]--卸载，其中对象生命阶段是可选的，也就是说，一旦完成类的加载、连接和初始化工作，就可以使用类了，当程序中不再有该类的引用时，就可以被 JVM 回收，至此类生命周期结束；
3. 对象的生命周期为：初始化--使用--回收--终结，对象的生命周期依赖于类的生命周期，只有当完成了类的加载、连接和初始化工作后，才会创建对象；
4. Java 类在进行初始化时，会先执行父类的初始化步骤，再执行子类的初始化，所以所有的类初始化工作均起始于 Object 类

YY：Well done！
 Kitty：嘿嘿，疑惑消除，可以愉快地玩耍了~~~