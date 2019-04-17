# Java 基础语法 - miner_zhu的博客 - CSDN博客





2018年07月13日 08:59:35[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：377











一个Java程序可以认为是一系列对象的集合，而这些对象通过调用彼此的方法来协同工作。

·        **对象**：对象是类的一个实例，有状态和行为。例如，一条狗是一个对象，它的状态有：颜色、名字、品种；行为有：摇尾巴、叫、吃等。

·        **类**：类是一个模板，它描述一类对象的行为和状态。

·        **方法**：方法就是行为，一个类可以有很多方法。逻辑运算、数据修改以及所有动作都是在方法中完成的。

·        **实例变量**：每个对象都有独特的实例变量，对象的状态由这些实例变量的值决定。

**基本语法**

编写Java程序时，应注意以下几点：

·        **大小写敏感**：Java是大小写敏感的，这就意味着标识符Hello与hello是不同的。

·        **类名**：对于所有的类来说，类名的首字母应该大写。如果类名由若干单词组成，那么每个单词的首字母应该大写，例如 MyFirstJavaClass。

·        **方法名**：所有的方法名都应该以小写字母开头。如果方法名含有若干单词，则后面的每个单词首字母大写。

·        **源文件名**：源文件名必须和类名相同。当保存文件的时候，你应该使用类名作为文件名保存（切记Java是大小写敏感的），文件名的后缀为.java。（如果文件名和类名不相同则会导致编译错误）。

·        **主方法入口**：所有的Java 程序由**publicstatic void main(String []args)**方法开始执行。

## Java标识符

Java所有的组成部分都需要名字。类名、变量名以及方法名都被称为标识符。

关于Java标识符，有以下几点需要注意：

·        所有的标识符都应该以字母（A-Z或者a-z）,美元符（$）、或者下划线（_）开始

·        首字符之后可以是字母（A-Z或者a-z）,美元符（$）、下划线（_）或数字的任何字符组合

·        关键字不能用作标识符

·        标识符是大小写敏感的



## Java修饰符

像其他语言一样，Java可以使用修饰符来修饰类中方法和属性。主要有两类修饰符：
- 访问控制修饰符 : default, public ,     protected, private
- 非访问控制修饰符 : final, abstract,     strictfp

## Java变量

Java中主要有如下几种类型的变量
- 局部变量
- 类变量（静态变量）
- 成员变量（非静态变量）

## Java枚举

Java 5.0引入了枚举，枚举限制变量只能是预先设定好的值。使用枚举可以减少代码中的bug

例如，我们为果汁店设计一个程序，它将限制果汁为小杯、中杯、大杯。这就意味着它不允许顾客点除了这三种尺寸外的果汁。

**注意：**枚举可以单独声明或者声明在类里面。方法、变量、构造函数也可以在枚举中定义

## Java 关键字

下面列出了Java关键字。这些保留字不能用于常量、变量、和任何标识符的名称。
|**类别**|**关键字**|**说明**|
|----|----|----|
|访问控制|private|私有的|
|protected|受保护的| |
|public|公共的| |
|类、方法和变量修饰符|abstract|声明抽象|
|class|类| |
|extends|扩允,继承| |
|final|最终值,不可改变的| |
|implements|实现（接口）| |
|interface|接口| |
|native|本地，原生方法（非Java实现）| |
|new|新,创建| |
|static|静态| |
|strictfp|严格,精准| |
|synchronized|线程,同步| |
|transient|短暂| |
|volatile|易失| |
|程序控制语句|break|跳出循环|
|case|定义一个值以供switch选择| |
|continue|继续| |
|default|默认| |
|do|运行| |
|else|否则| |
|for|循环| |
|if|如果| |
|instanceof|实例| |
|return|返回| |
|switch|根据值选择执行| |
|while|循环| |
|错误处理|assert|断言表达式是否为真|
|catch|捕捉异常| |
|finally|有没有异常都执行| |
|throw|抛出一个异常对象| |
|throws|声明一个异常可能被抛出| |
|try|捕获异常| |
|包相关|import|引入|
|package|包| |
|基本类型|boolean|布尔型|
|byte|字节型| |
|char|字符型| |
|double|双精度浮点| |
|float|单精度浮点| |
|int|整型| |
|long|长整型| |
|short|短整型| |
|变量引用|super|父类,超类|
|this|本类| |
|void|无返回值| |
|保留关键字|goto|是关键字，但不能使用|
|const|是关键字，但不能使用| |
|null|空| |



**继承**

在Java中，一个类可以由其他类派生。如果你要创建一个类，而且已经存在一个类具有你所需要的属性或方法，那么你可以将新创建的类继承该类。

利用继承的方法，可以重用已存在类的方法和属性，而不用重写这些代码。被继承的类称为超类（super class），派生类称为子类（subclass）。

**接口**

在Java中，接口可理解为对象间相互通信的协议。接口在继承中扮演着很重要的角色。

接口只定义派生要用到的方法，但是方法的具体实现完全取决于派生类。









# Java 对象和类

**访问实例变量和方法**

通过已创建的对象来访问成员变量和成员方法，如下所示：

/* 实例化对象 */ObjectReference = newConstructor();

/* 访问类中的变量 */ObjectReference.variableName;

/* 访问类中的方法 */ObjectReference.MethodName();

**实例**

下面的例子展示如何访问实例变量和调用成员方法：

public classPuppy{

   int puppyAge;

   public Puppy(String name){

      // 这个构造器仅有一个参数：name

      System.out.println("小狗的名字是 : " + name ); 

   }

   public void setAge( int age ){

       puppyAge = age;

   }

   public int getAge( ){

       System.out.println("小狗的年龄为 : " + puppyAge ); 

       return puppyAge;

   }

   public static void main(String []args){

      /* 创建对象 */

      Puppy myPuppy = new Puppy("tommy" );

      /* 通过方法来设定age */

      myPuppy.setAge( 2 );

      /* 调用另一个方法获取age */

      myPuppy.getAge( );

      /*你也可以像下面这样访问成员变量 */

      System.out.println("变量值 : " + myPuppy.puppyAge ); 

   }

}

编译并运行上面的程序，产生如下结果：


小狗的名字是: tommy

小狗的年龄为:2

变量值:2


**源文件声明规则**

在本节的最后部分，我们将学习源文件的声明规则。当在一个源文件中定义多个类，并且还有import语句和package语句时，要特别注意这些规则。

·        一个源文件中只能有一个public类

·        一个源文件可以有多个非public类

·        源文件的名称应该和public类的类名保持一致。例如：源文件中public类的类名是Employee，那么源文件应该命名为Employee.java。

·        如果一个类定义在某个包中，那么package语句应该在源文件的首行。

·        如果源文件包含import语句，那么应该放在package语句和类定义之间。如果没有package语句，那么import语句应该在源文件中最前面。

·        import语句和package语句对源文件中定义的所有类都有效。在同一源文件中，不能给不同的类不同的包声明。

类有若干种访问级别，并且类也分不同的类型：抽象类和final类等。这些将在访问控制章节介绍。

除了上面提到的几种类型，Java还有一些特殊的类，如：内部类、匿名类。

**Java 基本数据类型**

**内置数据类型**

Java语言提供了八种基本类型。六种数字类型（四个整数型，两个浮点型），一种字符类型，还有一种布尔型。

**引用类型**

·        在Java中，引用类型的变量非常类似于C/C++的指针。引用类型指向一个对象，指向对象的变量是引用变量。这些变量在声明时被指定为一个特定的类型，比如 Employee、Puppy 等。变量一旦声明后，类型就不能被改变了。

·        对象、数组都是引用数据类型。

·        所有引用类型的默认值都是null。

·        一个引用变量可以用来引用任何与之兼容的类型。

**Java语言支持一些特殊的转义字符序列。**
|**符号**|**字符含义**|
|----|----|
|\n|换行 (0x0a)|
|\r|回车 (0x0d)|
|\f|换页符(0x0c)|
|\b|退格 (0x08)|
|\0|空字符 (0x20)|
|\s|字符串|
|\t|制表符|
|\"|双引号|
|\'|单引号|
|\\|反斜杠|
|\ddd|八进制字符 (ddd)|

# Java 变量类型

Java语言支持的变量类型有：

·        类变量：独立于方法之外的变量，用 static 修饰。

·        实例变量：独立于方法之外的变量，不过没有 static 修饰。

·        局部变量：类的方法中的变量。

## Java 局部变量

·        局部变量声明在方法、构造方法或者语句块中；

·        局部变量在方法、构造方法、或者语句块被执行的时候创建，当它们执行完成后，变量将会被销毁；

·        访问修饰符不能用于局部变量；

·        局部变量只在声明它的方法、构造方法或者语句块中可见；

·        局部变量是在栈上分配的。

·        局部变量没有默认值，所以局部变量被声明后，必须经过初始化，才可以使用。

## 实例变量

·        实例变量声明在一个类中，但在方法、构造方法和语句块之外；

·        当一个对象被实例化之后，每个实例变量的值就跟着确定；

·        实例变量在对象创建的时候创建，在对象被销毁的时候销毁；

·        实例变量的值应该至少被一个方法、构造方法或者语句块引用，使得外部能够通过这些方式获取实例变量信息；

·        实例变量可以声明在使用前或者使用后；

·        访问修饰符可以修饰实例变量；

·        实例变量对于类中的方法、构造方法或者语句块是可见的。一般情况下应该把实例变量设为私有。通过使用访问修饰符可以使实例变量对子类可见；

·        实例变量具有默认值。数值型变量的默认值是0，布尔型变量的默认值是false，引用类型变量的默认值是null。变量的值可以在声明时指定，也可以在构造方法中指定；

·        实例变量可以直接通过变量名访问。但在静态方法以及其他类中，就应该使用完全限定名：ObejectReference.VariableName。

## 类变量（静态变量）

·        类变量也称为静态变量，在类中以static关键字声明，但必须在方法构造方法和语句块之外。

·        无论一个类创建了多少个对象，类只拥有类变量的一份拷贝。

·        静态变量除了被声明为常量外很少使用。常量是指声明为public/private，final和static类型的变量。常量初始化后不可改变。

·        静态变量储存在静态存储区。经常被声明为常量，很少单独使用static声明变量。

·        静态变量在程序开始时创建，在程序结束时销毁。

·        与实例变量具有相似的可见性。但为了对类的使用者可见，大多数静态变量声明为public类型。

·        默认值和实例变量相似。数值型变量默认值是0，布尔型默认值是false，引用类型默认值是null。变量的值可以在声明的时候指定，也可以在构造方法中指定。此外，静态变量还可以在静态语句块中初始化。

·        静态变量可以通过：*ClassName.VariableName*的方式访问。

·        类变量被声明为public static final类型时，类变量名称一般建议使用大写字母。如果静态变量不是public和final类型，其命名方式与实例变量以及局部变量的命名方式一致。

**Java 修饰符**

Java语言提供了很多修饰符，主要分为以下两类：

·        访问修饰符

·        非访问修饰符

修饰符用来定义类、方法或者变量，通常放在语句的最前端。

**访问控制修饰符**

Java中，可以使用访问控制符来保护对类、变量、方法和构造方法的访问。Java 支持 4 种不同的访问权限。

·        **default** (即缺省，什么也不写）: 在同一包内可见，不使用任何修饰符。使用对象：类、接口、变量、方法。

·        **private** : 在同一类内可见。使用对象：变量、方法。**注意：不能修饰类（外部类）**

·        **public** : 对所有类可见。使用对象：类、接口、变量、方法

·        **protected** : 对同一包内的类和所有子类可见。使用对象：变量、方法。**注意：不能修饰类（外部类）**。

## 非访问修饰符

·        为了实现一些其他的功能，Java 也提供了许多非访问修饰符。

·        static修饰符，用来修饰类方法和类变量。

·        final修饰符，用来修饰类、方法和变量，final修饰的类不能够被继承，修饰的方法不能被继承类重新定义，修饰的变量为常量，是不可修改的。

·        abstract修饰符，用来创建抽象类和抽象方法。

·        synchronized和 volatile 修饰符，主要用于线程的编程。

Java String 类

字符串广泛应用在Java编程中，在 Java 中字符串属于对象，Java 提供了 String 类来创建和操作字符串。



创建字符串最简单的方式如下:

Stringgreeting = "菜鸟教程";

**注意:**String 类是不可改变的，所以你一旦创建了 String 对象，那它的值就无法改变了。

如果需要对字符串做很多修改，那么应该选择使用[StringBuffer & StringBuilder 类](http://www.runoob.com/java/java-stringbuffer.html)。



## JavaStringBuffer 和StringBuilder 类

当对字符串进行修改的时候，需要使用 StringBuffer 和 StringBuilder 类。

和 String 类不同的是，StringBuffer 和 StringBuilder 类的对象能够被多次的修改，并且不产生新的未使用对象。

StringBuilder 类在 Java 5 中被提出，它和 StringBuffer 之间的最大不同在于 StringBuilder 的方法不是线程安全的（不能同步访问）。

由于 StringBuilder 相较于 StringBuffer 有速度优势，所以多数情况下建议使用 StringBuilder 类。然而在应用程序要求线程安全的情况下，则必须使用 StringBuffer 类。

**StringBuffer 方法**

以下是 StringBuffer 类支持的主要方法：
|**序号**|**方法描述**|
|----|----|
|1|public StringBuffer append(String s)将指定的字符串追加到此字符序列。|
|2|public StringBuffer reverse()将此字符序列用其反转形式取代。|
|3|public delete(int start, int end)移除此序列的子字符串中的字符。|
|4|public insert(int offset, int i)将int参数的字符串表示形式插入此序列中。|
|5|replace(int start, int end, String str)使用给定String中的字符替换此序列的子字符串中的字符。|



**Java 数组**

## 声明数组变量

dataType[]arrayRefVar; // 首选的方法

## 创建数组

Java语言使用new操作符来创建数组，语法如下：


arrayRefVar =new dataType[arraySize];


上面的语法语句做了两件事：

·        一、使用dataType[arraySize] 创建了一个数组。

·        二、把新创建的数组的引用赋值给变量 arrayRefVar。

数组变量的声明，和创建数组可以用一条语句完成，如下所示：


dataType[] arrayRefVar =new dataType[arraySize];


**Java Scanner 类**

java.util.Scanner是 Java5 的新特征，我们可以通过 Scanner 类来获取用户的输入。

下面是创建 Scanner 对象的基本语法：

Scanners = newScanner(System.in);

**使用 nextLine 方法：**

ScannerDemo.java文件代码：

importjava.util.Scanner;



public classScannerDemo {

    public static void main(String[] args) {

        Scanner scan = new Scanner(System.in);

    // 从键盘接收数据

   //nextLine方式接收字符串

        System.out.println("nextLine方式接收：");

 // 判断是否还有输入

        if (scan.hasNextLine()) {

            String str2 = scan.nextLine();

            System.out.println("输入的数据为：" + str2);

        }

        scan.close();

    }

}

**next() 与 nextLine() 区别**

next():

·        1、一定要读取到有效字符后才可以结束输入。

·        2、对输入有效字符之前遇到的空白，next() 方法会自动将其去掉。

·        3、只有输入有效字符后才将其后面输入的空白作为分隔符或者结束符。

·        next() 不能得到带有空格的字符串。

nextLine()：

·        1、以Enter为结束符,也就是说 nextLine()方法返回的是输入回车之前的所有字符。

·        2、可以获得空白。

如果要输入 int 或 float 类型的数据，在 Scanner 类中也有支持，但是在输入之前最好先使用 hasNextXxx() 方法进行验证，再使用 nextXxx() 来读取：





