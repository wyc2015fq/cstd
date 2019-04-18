# 通过反编译字节码来理解 Java 枚举 - z69183787的专栏 - CSDN博客
2017年01月06日 17:36:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1139
枚举的声明很简单, 像 
```
enum
 Gender { Male, Female }
```
, 其余事情就是 Java 编译器帮我们干的了，所以 enum 也就是一块语法糖。有了枚举确实是很方便，避免了传统常量的无范围性。那么编译器到底在后面做了什么呢？以及理解了这个之后我们可以怎么去使用 Java 的枚举, 下面就从这个例子说起：
```java
public enum Gender {
    Male,
    Female
}
```
把上面的编译成 Gender.class, 然后用  javap -c Gender 反编译出来就是
```java
public final class Gender extends java.lang.Enum {
  public static final Gender Male;
 
  public static final Gender Female;
 
  public static Gender[] values();
    Code:
       0: getstatic     #1                  // Field $VALUES:[LGender;
       3: invokevirtual #2                  // Method "[LGender;".clone:()Ljava/lang/Object;
       6: checkcast     #3                  // class "[LGender;"
       9: areturn
 
  public static Gender valueOf(java.lang.String);
    Code:
       0: ldc_w         #4                  // class Gender
       3: aload_0
       4: invokestatic  #5                  // Method java/lang/Enum.valueOf:(Ljava/lang/Class;Ljava/lang/String;)Ljava/lang/Enum;
       7: checkcast     #4                  // class Gender
      10: areturn
 
  static {};
    Code:
       0: new           #4                  // class Gender
       3: dup
       4: ldc           #7                  // String Male
       6: iconst_0
       7: invokespecial #8                  // Method "":(Ljava/lang/String;I)V
      10: putstatic     #9                  // Field Male:LGender;
      13: new           #4                  // class Gender
      16: dup
      17: ldc           #10                 // String Female
      19: iconst_1
      20: invokespecial #8                  // Method "":(Ljava/lang/String;I)V
      23: putstatic     #11                 // Field Female:LGender;
      26: iconst_2
      27: anewarray     #4                  // class Gender
      30: dup
      31: iconst_0
      32: getstatic     #9                  // Field Male:LGender;
      35: aastore
      36: dup
      37: iconst_1
      38: getstatic     #11                 // Field Female:LGender;
      41: aastore
      42: putstatic     #1                  // Field $VALUES:[LGender;
      45: return
}
```
从上面的字节码能够粗略的知道以下几点
- 
Gender 是 final 的
- 
Gender 继承自 java.lang.Enum 类
- 
声明了字段对应的两个 static final Gender 的实例
- 
实现了 values() 和  valueOf(String) 静态方法
- 
static{} 对所有成员进行初始化
有了以上的字节码，我们作进一步还原出 Gender 的普通类大概是这样的：
```java
public final class Gender extends java.lang.Enum {
 
  public static final Gender Male;
  public static final Gender Female;
 
  private static final Gender[] $VALUES;
 
  static {
    Male = new Gender("Male", 0);
    Female = new Gender("Female", 1);
 
    $VALUES = new Gender[] {Male, Female};
  }
 
  private Gender(String name, int original) {
    super(name, original)
  }
 
  public static Gender[] values() {
    return $VALUE.clone();
  }
 
  public static Gender valueOf(String name) {
    return Enum.valueOf(Gender.class, name);
  }
}
```
private Gender(String name, int origianl) 是我加上去的，是为了模拟枚举实例的创建，其实实例都是在 static 块中创建的。
当然上面的那个类是无法被编译的，因为 Java  编译器限制了我们显式的继承自 java.Lang.Enum 类, 报错 "The type Gender may not subclass Enum explicitly", 虽然 java.Lang.Enum 声明的是
```java
public abstract class Enum>
        implements Comparable, Serializable
```
这样看来枚举类其实用了多例模式，枚举类的实例是有范围限制的。它同样像我们的传统常量类，只是它的元素是有限的枚举类本身的实例。它继承自 java.lang.Enum, 所以可以直接调用 java.lang.Enum 的方法，如 name(), original() 等，name 就是常量名称, original 与 C 的枚举一样的编号。
可以在枚举类中自定义构造方法，但必须是  private 或  package protected, 因为枚举本质上是不允许在外面用 new Gender() 方式来构造实例的(Cannot instantiate the type Gender)，否则也是要乱了套的。
我们可以在枚举中定义自己的类，或实例方法，比如对于   `enum Gender`, 方法会直接定义给
 Gender 类或实例上去。
由于 enum 隐式继承自 java.lang.Enum 类，且 Java 不接受类的多重继承，所以我们不能让 enum 再继承别的类，下面那样写是错误的
> 
enum Gender extends Security {} //Syntax error on token "extends", implements expected
不能类多重继承，那枚举可以实现接口啊，因此下面的写法是正确的
> 
public enum Gender implements Behavior {}
让枚举实现接口有时候可以让设计变得巧妙。
结合枚举实现接口以及自定义方法，可以写出下面那样的代码：
```java
interface Behavior {
  void print();
}
 
public enum Gender implements Behavior {
  Male {
    @Override
    public void print() {
      System.out.println("Male special behavior");
    }
  },
  Female;
   
  @Override
  public void print() {
    System.out.println("Common behavior");
  }
   
  public static void main(String[] args) {
    Gender.Male.print(); //Male special behavior
    Gender.Female.print();  //Common behavior
  }
}
```
方法可以定义成所有实例公有，也可以让个别元素独有。
需要特别注明一下，上面在 Male {} 声明一个 print() 方法后实际产生一个 Gender 的匿名子类，编译后的 Gender$1，反编译它 javap -c Gender\$1：
```java
final class Gender$1 extends Gender {
  Gender$1(java.lang.String, int);
    Code:
       0: aload_0
       1: aload_1
       2: iload_2
       3: aconst_null
       4: invokespecial #1                  // Method Gender."":(Ljava/lang/String;ILGender$1;)V
       7: return
 
  public void print();
    Code:
       0: getstatic     #2                  // Field java/lang/System.out:Ljava/io/PrintStream;
       3: ldc           #3                  // String Male special behavior
       5: invokevirtual #4                  // Method java/io/PrintStream.println:(Ljava/lang/String;)V
       8: return
}
```
所以在 emum Gender 那个枚举中的成员  Male 相当于是
```java
public static final Male = new Gender$1("Male", 0); //而不是 new Gender("Male", 0)
```
上面 4: Invokespecial #1 要调用到下面的 Gender(java.lang.String, int, Gender$1) 方法。
若要研究完整的 Male 元素的初始化过程就得 javap -c Gender 看 Gender.java 产生的所有字节码，在此列出片断
```java
Gender(java.lang.String, int, Gender$1);
    Code:
       0: aload_0
       1: aload_1
       2: iload_2
       3: invokespecial #1                  // Method "":(Ljava/lang/String;I)V
       6: return
 
  static {};
    Code:
       0: new           #14                 // class Gender$1
       3: dup
       4: ldc           #15                 // String Male
       6: iconst_0
       7: invokespecial #16                 // Method Gender$1."":(Ljava/lang/String;I)V
      10: putstatic     #11                 // Field Male:LGender;
```
在 static{} 中大致看下 Male 的初始过程：加载 Gender$1, 并调用它的 Gender$1(java.lang.String, int) 构造函数生成一个 Gender$1 实例赋给 Male 属性。
JDK 提供有两个关于枚举的工具类  [EnumSet](https://docs.oracle.com/javase/7/docs/api/java/util/EnumSet.html) 和 [EnumMap](https://docs.oracle.com/javase/7/docs/api/java/util/EnumMap.html),
 实际中可以好好发掘发掘一下。
附：1. [Java enum的用法详解](/)
      2. [Java
 语言中 Enum 类型的使用介绍](https://www.ibm.com/developerworks/cn/java/j-lo-enum/)
      3. [Guide
 for understanding enum in java](http://howtodoinjava.com/2012/12/07/guide-for-understanding-enum-in-java/)
本文链接 [http://unmi.cc/understand-java-enum-with-bytecode/](http://unmi.cc/understand-java-enum-with-bytecode/),
 来自 [隔叶黄莺 Unmi Blog](http://unmi.cc/)
