# 玩转Java面试 -- 关键字 - Simple 专栏 - CSDN博客
2018年09月14日 13:24:52[Simple_Zz](https://me.csdn.net/love284969214)阅读数：231
### 1、抽象的 abstract 方法，是否可以被 static 、 native 、synchronized 修饰?
- 都不能，抽象方法需要子类重写。
- 静态的方法是无法被重写的，因此二者是矛盾的。
- 本地方法是由本地代码（如C代码）实现的方法，而抽象方法是没有实现的，也是矛盾的。
- synchronized 和方法的实现细节有关，抽象方法不涉及实现细节，因此也是相互矛盾的。
### 2、是否可以继承String 类?
- String 类是final类，不可以被继承。
- [【final、static、native 关键字】](https://blog.csdn.net/love284969214/article/details/7231246)
### **3、静态变量和实例变量的区别？**
- 静态变量是被 static 修饰的变量，也称为类变量，它属于类，不属于类的任何一个对象。
- 一个类不管创建多少个对象，静态变量在内存中有且仅有一个拷贝。
- 实例变量必须依存于某一实例，需要先创建对象然后通过对象才能访问到它。
- 静态变量可以实现让多个对象共享内存。
- 在Java开发中，上下文类和工具类中通常会有大量的静态成员。
### 4、是否可以从一个静态（static）方法内部发出对非静态（non-static）方法的调用？
- 不可以。
- 静态方法只能访问静态成员。
- 非静态方法的调用要先创建对象。
- 因此在调用静态方法时可能对象并没有被初始化。
### 5、Java 中的final关键字有哪些用法？
- 修饰类：表示该类不能被继承。
- 修饰方法：表示方法不能被重写。
- 修饰变量：表示变量只能一次赋值以后值不能被修改（常量）。
### **6、final, finally, finalize 的区别?**
- final：有三种用法：
- 类被声明为final，不能被继承，因此它和abstract是反义词。
- 变量声明为final，不能被修改，只能被使用，定义时必须初始化。
- 方法声明为final，不能在子类中被重写，只能被使用。
- finally：通常放在try…catch的后面，表示最终执行的代码块，程序无论正常执行还是发生异常，这里的代码都能执行。
- finalize：Object类中定义的方法，Java中允许使用finalize() 方法在垃圾收集器将对象从内存中清除出去之前做必要的清理工作。这个方法是由垃圾收集器在销毁对象时调用的，通过重写finalize() 方法可以整理系统资源或者执行其他清理工作。
### 7、访问修饰符public,private,protected,以及不写（默认）时的区别？
[【public、protected、default、private 的区别】](https://blog.csdn.net/love284969214/article/details/7185127)
||本类|本包|子类|外部包|
|----|----|----|----|----|
|public|可以|可以|可以|可以|
|protected|可以|可以|可以|不可以|
|default|可以|可以|不可以|不可以|
|private|可以|不可以|不可以|不可以|
### 8、如何将字符串转换为基本数据类型？
- 方法1： 包装类 **.**parseXXX(string) 
- 方法2： 包装类 **.**valueOf(string) **.**xxxValue()
### 9、如何将基本数据类型转换为字符串？
- 方法1：基本数据类型 + ""
- 方法2：String.valueOf(基本数据类型)
### 10、float f=3.4;是否正确?
- 不正确
- 可以写成 **float f =3.4F**;
- 3.4是双精度数，将双精度型（double）赋值给浮点型（float）属于下转型（down-casting，也称为窄化）会造成精度损失，因此需要强制类型转换float f =(float)3.4;
### 11、int 和Integer 有什么区别?
- int 是基本数据类型
- Integer是包装类
- [【int/Integer 的区别】](https://blog.csdn.net/love284969214/article/details/7221881)
### 12、short s1 = 1; s1 = s1 + 1; 有错吗? short s1 = 1; s1 += 1;有错吗?
- 前者 short s1 = 1; s1 = s1 + 1; 有错。
- 由于1是 int 类型，因此 s1+1 运算结果也是int 型，需要强制转换类型。
- 后者 short s1 = 1; s1 += 1; 可以注册编译。
- 运行时隐含着强制类型转换。
- [【基本数据类型/运算符】](https://blog.csdn.net/love284969214/article/details/7222259)
### 13、String 是最基本的数据类型吗?
- 不是
- Java中的基本数据类型只有8个：byte、short、int、long、float、double、char、boolean。
- 除了基本类型（primitive type）和枚举类型（enumeration type），剩下的都是引用类型（reference type）。
- [【基本数据类型/运算符】](https://blog.csdn.net/love284969214/article/details/7222259)
### 14、char 型变量中能不能存贮一个中文汉字?为什么?
- char类型可以存储一个中文汉字，因为Java中使用的编码是Unicode（不选择任何特定的编码，直接使用字符在字符集中的编号，这是统一的唯一方法），一个char类型占2个字节（16bit），所以放一个中文是没问题的。
### 15、String s = new String(“xyz”);创建了几个字符串对象？
- 两个对象，一个是静态存储区的 xyz ， 一个是用new创建在堆上的对象。
### 16、如何实现字符串的反转及替换？
- 方法很多，可以自己写实现也可以使用String / StringBuffer / StringBuilder中的方法。
### 17、swtich 是否能作用在 byte 上，是否能作用在 long 上，是否能作用在 String上?
- 早期的JDK中，switch（expr）中，expr可以是byte、short、char、int。
- JDK 1.5版开始，Java中引入了枚举类型（enum），expr也可以是枚举。
- JDK 1.7版开始，还可以是字符串（String）。
- JDK1.8版本，Only convertible int values, strings or enum variables are permitted。
- 长整型（long）还是不可以的。
### 18、数组有没有length()方法?String 有没有length()方法？
- 数组有 length属性，没有方法。
- String 有length()方法。
### **19、如何实现对象克隆？**
- 1.实现Cloneable接口并重写Object类中的clone()方法；
- 2.实现Serializable接口，通过对象的序列化和反序列化实现克隆，可以实现真正的深度克隆，
- 基于序列化和反序列化实现的克隆不仅仅是深度克隆，更重要的是通过泛型限定，可以检查出要克隆的对象是否支持序列化，这项检查是编译器完成的，不是在运行时抛出异常，这种是方案明显优于使用Object类的clone方法克隆对象。
### 20、什么是深拷贝，什么是浅拷贝？
- 浅拷贝：使用一个已知实例对新创建实例的成员变量逐个赋值，这个方式被称为浅拷贝。
- 深拷贝：当一个类的拷贝构造方法，不仅要复制对象的所有非引用成员变量值，还要为引用类型的成员变量创建新的实例，并且初始化为形式参数实例值。这个方式称为深拷贝。
- [【深拷贝、浅拷贝】](https://mp.csdn.net/postedit/7202827)
### 21、Java 中，如何跳出当前的多重嵌套循环？
- 方法1：添加 标签A 在循环外，break A。
- 方法2：return
- [【通过continue/break/标签 实现 goto】](https://blog.csdn.net/love284969214/article/details/7221575)
### 22、两个对象值相同(x.equals(y) == true)，但却可有不同的hash code，这句话对不对？
- 不对。
- 如果两个对象  == ，它们的哈希码相同。
- [【equal 与 == 区别】](https://blog.csdn.net/love284969214/article/details/7211055)
### 23、用最有效率的方法计算2乘以8?
- 2  <<  3（左移3位相当于乘以2的3次方，右移3位相当于除以2的3次方）。
### 24、&和&&的区别？  或者  | 和 || 的区别？
- 两种逻辑与（&& 和 &）的运算规则基本相同，两种逻辑或（|| 和 |）的运算规则也基本相同。
- & 和 | 运算是把逻辑表达式全部计算完，而&&和||运算具有短路计算功能。
- [【短路运算 &&、|| 】](https://blog.csdn.net/love284969214/article/details/7225137)
### 25、Math.round(11.5) 等于多少? Math.round(-11.5)等于多少?
- Math.round(11.5)的返回值是12，Math.round(-11.5)的返回值是-11。
- [【四舍五入与保留位】](https://blog.csdn.net/love284969214/article/details/7231222)
### **26、列出一些你常见的运行时异常？**
- ArithmeticException（算术异常）
- ClassCastException （类转换异常）
- IllegalArgumentException （非法参数异常）
- IndexOutOfBoundsException （下表越界异常）
- NullPointerException （空指针异常）
- SecurityException （安全异常）
### **27、Java 语言如何进行异常处理，关键字：throws、throw、try、catch、finally分别如何使用？**
- 通过面向对象的方法进行异常处理，把各种不同的异常进行分类，并提供了良好的接口。
- 在Java 中，每个异常都是一个对象，它是Throwable 类或其子类的实例。
- 当一个方法出现异常后便抛出一个异常对象，该对象中包含有异常信息.
- 调用这个对象的方法可以捕获到这个异常并进行处理。
- 一般情况下是用 try 来执行一段程序，如果出现异常，系统会抛出（throw）一个异常。
- 这时候你可以通过它的类型来捕捉（catch）它，或最后（finally）由缺省处理器来处理。
- try用来指定一块预防所有“异常”的程序，catch 子句紧跟在try块后面，用来指定你想要捕捉的“异常”的类型。
- throw 语句用来明确地抛出一个 异常。
- throws用来标明一个成员函数可能抛出的各种 异常。
- finally 为确保一段代码不管发生什么 异常 都被执行一段代码。
### **28、运行时异常与受检异常有何异同？**
- 异常表示程序运行过程中可能出现的非正常状态。
- 运行时异常表示虚拟机的通常操作中可能遇到的异常，是一种常见运行错误，只要程序设计得没有问题通常就不会发生。
- 受检异常跟程序运行的上下文环境有关，即使程序设计无误，仍然可能因使用的问题而引发。
- Java编译器要求方法必须声明抛出可能发生的受检异常，但是并不要求必须声明抛出未被捕获的运行时异常。
### 29、异常的使用要遵循那些原则？
- 不要将异常处理用于正常的控制流（设计良好的API不应该强迫它的调用者为了正常的控制流而使用异常）
- 对可以恢复的情况使用受检异常，对编程错误使用运行时异常
- 避免不必要的使用受检异常（可以通过一些状态检测手段来避免异常的发生）
- 优先使用标准的异常
- 每个方法抛出的异常都要有文档
- 保持异常的原子性
- 不要在catch中忽略掉捕获到的异常
### 30、try{}里有一个return语句，那么紧跟在这个try后的finally{}里的code会不会被执行，什么时候被执行，在return前还是后?
- 会执行，在方法返回调用者前执行。
- Java允许在finally中改变返回值的做法是不好的，因为如果存在finally代码块，try中的return语句不会立马返回调用者，而是记录下返回值待finally代码块执行完毕之后再向调用者返回其值，然后如果在finally中修改了返回值，这会对程序造成很大的困扰。
### **31、Error 和Exception 有什么区别?**
- Error 表示系统级的错误和程序不必处理的异常，是恢复不是不可能但很困难的情况下的一种严重问题。
- 比如内存溢出，不可能指望程序能处理这样的情况。
- Exception 表示需要捕捉或者需要程序进行处理的异常，是一种设计或实现问题。
- 也就是说，它表示如果程序运行正常，从不会发生的情况。
### 32、Java 有没有 goto?
- goto 是 Java中的保留字，在目前版本的Java中没有使用。
- 要想做跳转可以使用 continue、break。
- [【通过continue/break/标签 实现 goto】](https://blog.csdn.net/love284969214/article/details/7221575)
