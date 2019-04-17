# Java —— 异常处理 - Alex_McAvoy的博客 - CSDN博客





2018年10月06日 23:20:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【异常】

在 Java 中，异常是指程序执行时的不正常情况（开发时的语法错误、逻辑错误不是异常），对于可能出现的异常，需要进行预处理，保证程序的有效运行，否则程序会出错。

Java 程序在执行过程中发生的异常事件分为两类：
- Error：错误，Java 虚拟机无法解决的严重问题，一般不编写针对性的代码进行处理，如：JVM 系统内部错误、资源耗尽等。
- Exception：异常，因编程错误或偶然的外在因素导致的一般性问题，可使用针对性的代码进行处理，如：空指针访问、试图读取不存在的文件等。

异常又分为编译时异常、运行时异常，前者是在编译时出现的异常（执行 javac.exe 时），后者是在运行期间出现的异常（执行 java.exe 时）。

对于这些错误，一般有两种解决方法，一是遇到错误就终止程序，二是在编写程序时就考虑到错误的检测、错误消息的提示、错误的处理。·

# 【异常类】

在 Java 中异常也是通过一个对象来表示的，程序运行时抛出的异常，实际上就是一个异常对象，该对象中不仅封装了错误信息，还提供了处理方法，而所有的异常对象都来自于内置的异常类。

异常类是 Java 中内置的用来描述经常性错误的类，这些类均继承自 java.lang.Throwable 类，具体结构如下

![](https://img-blog.csdn.net/20181006231836676?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 1.Error 类

Error 类被称为致命异常类，其及其子类通常用来描述 Java 运行系统中的内部错误及资源耗尽的错误，用 Error 表示的异常是较严重的，仅靠修改程序本身无法恢复执行，大多数情况下，发生该异常时，终止程序是最好的选择。

## 2.Exception 类

Exception 类被称为非致命异常类，发生该异常的程序，经过捕获后处理可正常运行，在开发过程中的异常处理主要就是针对该类及其子类的异常处理，对于程序中可能发生的该类异常，应尽可能的进行处理，以保证程序在运行时可被顺利执行。

### 1）不检查异常

不检查异常是由程序中编写的错误导致的，修改了错误后即可正常运行，因此其又被称为运行时异常，对应的类为 RuntimeException 类。

当 RuntimeException 类及其子类所描述的异常发生后，不通过异常处理语句抛出，在编译时可以通过，只是在运行时由 JVM 来抛出。

常见的运行时异常

① ArithmeticException 类：用于描述算术异常，如除法或求模运算中除数为 0 时，会抛出此异常

```java
int n=9/0;    //除数为0，抛出 ArithmeticException 异常
```

② NullPointerException 类：用于描述空指针异常，当引用变量值为 null 时，试图通过 " . " 操作符对其进行访问，会抛出此异常

```java
Date now=null;    //声明一 Date 型变量 now，但未引用任何对象
String today=now.toString();    //抛出 NullPointerException 异常
```

③ NumberFormatException 类：用于描述字符串转数字时的异常，当字符串不是数字格式时，若将其转换为数字，会抛出此异常

```java
String str="123L";
int age=Integer.parseInt();    //抛出 NumberFormatException 异常
```

④ IndexOutOfBoundsException 类：用于描述对象的索引超出范围的异常，其有两个子类，ArrayIndexOutOfBoundsException 异常、StringIndexOutBoundException 异常，分别表示数组下标越界异常与字符串索引超出范围异常

```java
//抛出 ArrayIndexOutOfBoundsException 异常
int a[]=new int[3];
a[3]=123;

//抛出 StringIndexOutBoundException 异常
String str="abc";
char c=str.charAt(3);
```

⑤ ArrayStoreException 类：用于描述数组试图存储类型不兼容的值

```java
int a[]=new int[3];
a[0]="abc";//抛出 ArrayStoreException 异常
```

⑥ ClassCastException 类：用于描述强制类型转换时的异常

```java
Object obj=new String("abc");//引用变量 obj 引用 String 型对象
Integer num=(Integer)obj;//抛出 ClassCastException 异常
```

### 2）检查异常

检查异常是指编译器要检查这类异常，检查的目的一方面是因为该类异常的发生难以避免，另一方面就是让开发者去解决掉这类异常，所以该类异常必须通过异常处理语句进行处理，如果不处理这类异常，集成开发环境中的编译器一般会给出错误提示。

常见检查异常类列表
|异常类名称|异常类含义|
|----|----|
|ClassNotFoundException|未找到相应类异常|
|EOFException|文件已结束异常|
|FileNotFoundException|文件未找到异常|
|IllegalAccessException|访问某类被拒绝抛出的异常|
|IOException|输入输出异常|
|NoSuchFieldException|字段未找到异常|
|NoSuchMethodException|方法未找到异常|
|SQLException|操作数据库异常|

# 【异常处理】

Java 提供的是异常处理的抓抛模型，若某个方法抛出异常，既可以在当前方法中捕获，然后处理该异常，也可以将异常向上抛出，由方法的调用者来处理。

抛出是指执行代码时，一旦出现异常，就会在异常代码处生成一个对应的异常类型对象，并将对象抛出给方法的调用者，一旦抛出此异常类的对象，那么程序就会终止执行。

抓是指抓住上一步抛出来的异常类的对象，如何抓即为捕获处理方式。

## 1.捕获处理

### 1）try...catch 语句

try 语句块中可能同时存在多种异常，具体捕获哪类异常则由 catch 中异常类参数来决定，异常类参数要求必须是 Throwable 类的子类，用于指定 catch 语句要捕获的异常。

若不知代码抛出的哪类异常，可以指定他们的父类 Throwable 类或 Exception 类。

try...catch 语句可以进行嵌套。

```java
try{
    可能发生异常的代码
}catch(异常类1 异常对象){
    异常1处理代码
}catch(异常类2 异常对象){
    异常2处理代码
}catch(异常类3 异常对象){
    异常3处理代码
}
    ...
```

在使用多个 catch 语句捕捉异常时，要注意 catch 语句的顺序，因为若 try 代码块中发现异常，则会由上自下一次查找能够捕获该异常的 catch 语句块，当匹配后不会再执行其他的 catch 语句块。因此若多个 catch 语句所捕获的异常类之间有继承关系，则先捕获子类的 catch 要放在捕获父类的 catch 语句之前，否则则会报错。

```java
try{
    int a=Integer.parseInt("123L");
}catch(Exception e){
    System.out.println(e.getMessage());
}catch(NumberFormatException e){//会发生异常，找不到 NumberFormatException 的 catch块
    System.out.println(e.getMessage());
}
```

### 2）finally 子句

finally 语句需要与 try...catch 语句一同使用，不管程序中有无异常发生，且不管之前的 try...catch 是否执行完毕，最终都会执行 finally 语句块中的语句，使得一些不管任何情况下都必须执行的步骤被执行，保证了程序的健壮性。

```java
try{
    int a=Integer.parseInt("123L");
}catch(NumberFormatException e){
    System.out.println(e.getMessage());
}finally{
    System.out.println("Hello World!");
}
```

### 3）try、catch、finally 语句注意事项

① 不能单独使用 try、catch、finally 语句块。

② 当 catch、finally 语句块同时存在时，finally 语句块必须放在 catch 语句块之后。

③ try 可只与 finally 语句块使用，可以使得程序发生异常后抛出异常，并继续执行方法中的其他代码。

④ try 语句块中声明的变量是局部变量，仅在当前的 try 语句块中有效。

⑤ 在使用 throw 语句抛出一个异常对象后，该语句后面的代码将不会被执行。

## 2.异常抛出

若某个方法可能会产生异常，但不想在当前方法中处理异常，可将异常抛出，然后在调用该方法的代码中捕获该异常并进行处理。

将异常抛出可使用 throws 关键字或 throw 关键字，要注意的是，若抛出的异常是 Error、RuntimeException 及其子类，则无需使用 throws 关键字或 try...catch 语句，JVM 会自动捕获异常。

### 1）throws 关键字

throws 关键字被应用于声明方法时，用于指定方法可能抛出的异常，多个异常直接用 "," 来分隔，将异常通过 throws 抛给上一级后，若仍不想处理该异常，可继续向上抛出。

```java
class Main{
    public static void main(String[] args){
        try{
            test("123L");
        }catch(NumberFormatException e){
            System.out.println(e.getMessage());
        }
    }
    public static void test(String str) throws NumberFormatException{
        a=Integer.parseInt(str);
        System.out.println(a);
    }
}
```

### 2）throw 关键字

throw 关键字也可用于抛出异常，与 throws 不同的是，throws 用于方法声明中可抛出多个异常，而 throw 用于方法体内只能抛出一个异常。

通过 throw 抛出异常后，若想由上一级代码来捕获处理异常，需要使用 throws 关键字在方法声明中指明要抛出的异常，若想在当前方法中处理 throw 抛出的异常，则需要使用 try...catch 语句。

在使用 throw 抛出一个异常后，其后的语句不再被执行。

```java
class Main{
    public static void main(String[] args){
        try{
            test(-123);
        }catch(Exception e){
            System.out.println(e.getMessage());
        }
    }
    public static void test(int a) throws Exception{
        if(a<0)
            throw Exception("No negative!");
        System.out.println(a);
    }
}
```

# 【自定义异常】

根据需要，有时需要建立自己的异常类，并将它们用于程序中描述 Java 内置异常类所不能描述的一些特殊情况。

创建的异常类必须继承自 Throwable 类，一般继承于其子类的 Exception 类或Exception 类的子孙类，除此外，与创建一个普通类的语法相同。

创建自定义异常类并使用大体分为以下几个步骤：
- 创建自定义异常类
- 在方法中通过 throw 抛出异常对象
- 若在当前抛出异常的方法中处理异常，可使用 try...catch 语句捕获并处理；否则在方法的声明处通过 throws 指明要抛出给方法调用者的异常，并继续进行下一步操作
- 在出现异常的方法调用代码中捕获并处理异常

```java
public class MyException extends Exception{//继承Exception类
    private String content;
    public MyException(String content){//构造方法
        this.content=content;
    }
    public String getContent(){//获取描述信息
        return this.content;
    }
}
public class Main{
    public static void mian(String[] args){
        String str="hello world!";
        try{
            check(str);//调用check()方法
        } catch(MyException e){//捕获MyException异常
            System.out.println(e.getContent());//输出异常描述信息
        }
    }
    public static void check(String str) throws MyException{
        int len=str.length();
        char a[]=str.toChatArray();
        for(int i=0;i<len-1;i++)
            if(!(a[i]>="a"&&a[i]<='z'))
                throw new MyException("字符串中有非法字符！");
    }
}
```





