# Java import 作用 详解 - Simple 专栏 - CSDN博客
2012年01月04日 19:15:56[Simple_Zz](https://me.csdn.net/love284969214)阅读数：290
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
*import与package机制相关，这里先从package入手，再讲述import以及static import的作用。*
## package
- C 中的 #include会把所包含的内容在编译时添加到程序文件中，而 java 的 import 则不同。
- 提起package就能联想到class，那么package与class应该怎么理解？
- 这里我们先了解一下，每个人都有自己的名字，比如张三，欧阳六六，在此基础上我们可以把 java.lang.String 理解为 java.lang (姓/复姓) String (名字)。
- 在学习生活中通常会有重名的时候，比如上学的时候通常有同名的同学，那么Java怎么通过package来解决的呢？
- Java 的package命名规则可以理解为身份证，比如同班同学，同名但是生日不一致，男女性别不一致，住址不一致(这也就是package路径不一致)，所以package这种方式让极大降低了类之间的命名冲突。
- 但Java的名字可以自己取，不像我们的名字都是长辈取得，那么Sun公司怎么规范的给class取名呢？ ([有需要请查看Java编程规范](https://blog.csdn.net/love284969214/article/details/7174222))
可是现在问题来了，因为很多package的名称非常的长，在编程时使用类要将**多个包名.类名**完全写出，会让代码变得冗长，减低了简洁度。例如：
```
java.io.InputStream is = java.lang.System.in;
java.io.InputStreamReader isr= new java.io.InputStreamReader(is);
java.io.BufferedReader br = new java.io.BufferedReader(isr);
java.lang.String str = new java.lang.String();
```
以上代码显得非常麻烦，于是Sun公司就引入了import。
## import
import就是在java文件开头的地方，先说明会用到那些类别。 
接着我们就能在代码中只用类名指定某个类，也就是只称呼名字，不称呼他的姓。
首先，在程序开头写：
```
import java.lang.System;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.lang.String;
```
于是我们就可以在程序中这样写到：
```
InputStream = System.in;
InputStreamReader isr = new InputStreamReader(is);
BufferedReader br = new BufferedReader(isr);
String str = new String("Hello");
System.out.println(str);
```
一个java文件就像一个大房间，我们在门口写着在房间里面的class的姓和名字，所以在房间里面提到某个class就直接用他的名字就可以。例如：System 就是指 java.lang.System，而 InputStream 就是指 java.io.InputStream。
但是如果一个java文件里面有多个同个“姓”，即包名相同的类（例如上面的InputStream，InputStreamReader，BufferedReader都是java.io中的类），我们一一写出显得比较繁杂，所以Sun就让我们可以使用
```
import java.lang.*;
import java.io.*;
```
表示文件里面说到的类不是java.lang包的就是java.io包的。编译器会帮我们选择与类名对应的包。
那我们可不可以再懒一点直接写成下面声明呢？
`import java.*;`
历史告诉我们，这样是不行的。因為那些类别是姓 java.io 而不是姓 java。就像姓『欧阳』的人应该不会喜欢你称他为『欧』 先生吧。这样写的话只会将java包下的类声明，而不会声明子包的任何类。
> 
这里注意，java.lang包里面的类实在是太常太常太常用到了，几乎没有类不用它的， 所以不管你有没有写 import java.lang，编译器都会自动帮你补上，也就是说编译器只要看到没有姓的类别，它就会自动去lang包里面查找。所以我们就不用特别去 import java.lang了。
一开始说 import 跟 #include 不同，是因为import 的功能到此为止，它不像#include 一样，会将其他java文件的内容载入进来。import 只是让编译器编译这个java文件时把没有姓的类别加上姓，并不会把别的文件程序写进来。你开心的话可以不使用import，只要在用到类别的时候，用它的全部姓名来称呼它就行了(就像例子一开始那样)，这样跟使用import功能完全一样。
## import的两种导入声明
- 单类型导入(single-type-import) 
	（例:import java.util.ArrayList; ）
- 按需类型导入(type-import-on-demand) 
	（例:import java.util.*;）
有如下属性：
- 
java以这样两种方式导入包中的任何一个public的类和接口(只有public类和接口才能被导入)
- 
上面说到导入声明仅导入声明目录下面的类而不导入子包，这也是为什么称它们为类型导入声明的原因。
- 
导入的类或接口的简名(simple name)具有**编译单元作用域**。这表示该类型简名可以在**导入语句所在的编译单元的任何地方**使用.这并不意味着你可以使用该类型所有成员的简名,而只能使用类型自身的简名。 
> 
例如: java.lang包中的public类都是自动导入的,包括Math和System类.但是,你不能使用它们的成员的简名PI()和gc(),而必须使用Math.PI()和System.gc().你不需要键入的是java.lang.Math.PI()和java.lang.System.gc()。
- 
程序员有时会导入当前包或java.lang包，这是不需要的，因为当前包的成员本身就在作用域内,而java.lang包是自动导入的。java编译器会忽略这些冗余导入声明(redundant import declarations)。即使像这样 下图 多次导入,也可编译通过。编译器会将冗余导入声明忽略.
```java
import java.util.ArrayList; 
import java.util.*;
```
## static import静态导入
在Java程序中，是不允许定义独立的函数和常量的。即什么属性或者方法的使用必须依附于什么东西，例如使用类或接口作为挂靠单位才行（在类里可以挂靠各种成员，而接口里则只能挂靠常量）。
> 
如果想要直接在程序里面不写出其他类或接口的成员的挂靠单元，有一种变通的做法 : 
将所有的常量都定义到一个接口里面，然后让需要这些常量的类实现这个接口（这样的接口有一个专门的名称，叫(“Constant Interface”)。这个方法可以工作。但是，因为这样一来，就可以从“一个类实现了哪个接口”推断出“这个类需要使用哪些常量”，有“会暴露实现细节”的问题。
于是J2SE 1.5里引入了“Static Import”机制，借助这一机制，可以用略掉所在的类或接口名的方式，来使用静态成员。static import和import其中一个不一致的地方就是static import导入的是**静态成员**，而import导入的是**类或接口类型**。
如下是一个有静态变量和静态方法的类
```
package com.assignment.test;
public class staticFieldsClass {
    static int staticNoPublicField = 0; 
    public static int staticField = 1;
    public static void staticFunction(){}
}
```
- 平时我们使用这些静态成员是用**类名.静态成员**的形式使用，即staticFieldsClass.staticField或者staticFieldsClass.staticFunction()。
现在用import static的方式：
```java
package importStatic.test;
public class StaticFieldsClass {
    public String                   pStr    = "public";
    public static String            psStr   = "public static";
    public static final String      psfStr  = "public static final";
    public static volatile String   psvStr  = "public static volatile";
    private String                  prStr   = "private";
    private static String           prsStr  = "private static";
    private static final String     prsfStr = "private static final";
    private static volatile String  prsvStr = "private static volatile";
    private static void staticPrivateFunction(){
        System.out.println("private function");
    }
    public void publicFunction() {
        StaticFieldsClass.prsStr = prsStr;
    }
    public static void staticPublicFunction() {
        System.out.println("public function");
    }
}
```
```java
package importStatic.test;
//**精准导入**
//直接导入具体的静态变量、常量、方法方法，注意导入方法直接写方法名不需要括号。
import static importStatic.test.StaticFieldsClass.staticPublicFunction;
import static importStatic.test.StaticFieldsClass.psStr;
import static importStatic.test.StaticFieldsClass.psfStr;
import static importStatic.test.StaticFieldsClass.psvStr;
//或者使用如下形式：
//**按需导入**不必逐一指出静态成员名称的导入方式
//import static importStatic.test.StaticFieldsClass.*;
public class StaticTest {
    public static void main(String[] args) {
        //这里直接写静态成员而不需要通过类名调用
        System.out.println(psStr);
        System.out.println(psfStr);
        System.out.println(psvStr);
        staticPublicFunction();
    }
}
```
这里有几个问题需要弄清楚：
- import static只能访问到staticFieldsClass中 **public static** 修饰的成员。
- 导入的静态成员和本地的静态成员名字相同起了冲突，这种情况下的处理规则，**本地优先**。
- 进行 import static 的时候，不同的类包括相同的成员。在这种时候，J2SE 1.5会这样来加以处理： 
- 如果两个语句都是精确导入的形式，或者都是按需导入的形式，那么会造成编译错误。
- 如果一个语句采用精确导入的形式，一个采用按需导入的形式，那么采用精确导入的形式的一个有效。
> 
import static这么叼那它有什么负面影响吗？
> 
答案是肯定的，去掉静态成员前面的类型名，固然有助于在频繁调用时显得简洁，但是同时也失去了关于“这个东西在哪里定义”的提示信息，理解或维护代码就呵呵了。 
但是如果导入的来源很著名（比如java.lang.Math），这个问题就不那么严重了。
## 按需导入机制
使用按需导入声明是否会降低Java代码的**执行效率**?
**绝对不会!**
一、import的按需导入
```
import java.util.*;
public class NeedImportTest {
    public static void main(String[] args) {
        ArrayList tList = new ArrayList();
    }
}
```
编译之后的class文件 :
```
//import java.util.*被替换成import java.util.ArrayList
//即按需导入编译过程会替换成单类型导入。
import java.util.ArrayList;
public class NeedImportTest {
    public static void main(String[] args) {
        new ArrayList();
    }
}
```
二、static import的按需导入
```
import static com.assignment.test.StaticFieldsClass.*;
public class StaticNeedImportTest {
    public static void main(String[] args) {
        System.out.println(staticField);
        staticFunction();
    }
}
```
上面StaticNeedImportTest 类编译之后 :
```
//可以看出 ： 
//1、static import的精准导入以及按需导入编译之后都会变成import的单类型导入
import com.assignment.test.StaticFieldsClass;
public class StaticNeedImportTest {
    public static void main(String[] args) {
    //2、编译之后“打回原形”，使用原来的方法调用静态成员
        System.out.println(StaticFieldsClass.staticField);
        StaticFieldsClass.staticFunction();
    }
}
```
## 附加
这是否意味着你总是可以使用按需导入声明? 
**是,也不是!**
在类似Demo的非正式开发中使用按需导入声明显得很有用。
> 
然而,有这四个理由让你可以放弃这种声明:
- 编译速度：在一个很大的项目中，它们会极大的影响编译速度.但在小型项目中使用在编译时间上可以忽略不计。
- 命名冲突：解决避免命名冲突问题的答案就是使用全名。而按需导入恰恰就是使用导入声明初衷的否定。
- 说明问题：毕竟高级语言的代码是给人看的，按需导入看不出使用到的具体类型。
- 无名包问题：如果在编译单元的顶部没有包声明，Java编译器首选会从无名包中搜索一个类型，然后才是按需类型声明。如果有命名冲突就会产生问题。
Sun的工程师一般不使用按需类型导入声明.这你可以在他们的代码中找到: 
在java.util.Properties类中的导入声明:
```
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.security.AccessController;
import java.security.PrivilegedAction;
```
可以看到他们用单类型导入详细的列出了需要的java.io包中的具体类型。
