
# java入门教程-3.7Java包装类、拆箱和装箱详解 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:46:27[seven-soft](https://me.csdn.net/softn)阅读数：122个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



虽然 Java 语言是典型的面向对象编程语言，但其中的八种基本数据类型并不支持面向对象编程，基本类型的数据不具备“对象”的特性——不携带属性、没有方法可调用。 沿用它们只是为了迎合人类根深蒂固的习惯，并的确能简单、有效地进行常规数据处理。
这种借助于非面向对象技术的做法有时也会带来不便，比如引用类型数据均继承了 Object 类的特性，要转换为 String 类型（经常有这种需要）时只要简单调用 Object 类中定义的toString()即可，而基本数据类型转换为
 String 类型则要麻烦得多。为解决此类问题 ，Java为每种基本数据类型分别设计了对应的类，称之为包装类(Wrapper Classes)，也有教材称为外覆类或数据类型类。
基本数据类型及对应的包装类|基本数据类型|对应的包装类|
|byte|Byte|
|short|Short|
|int|Integer|
|long|Long|
|char|Character|
|float|Float|
|double|Double|
|boolean|Boolean|
每个包装类的对象可以封装一个相应的基本类型的数据，并提供了其它一些有用的方法。包装类对象一经创建，其内容（所封装的基本类型数据值）不可改变。
基本类型和对应的包装类可以相互装换：由基本类型向对应的包装类转换称为装箱，例如把 int 包装成 Integer 类的对象；
包装类向对应的基本类型转换称为拆箱，例如把 Integer 类的对象重新简化为 int。
## 包装类的应用
八个包装类的使用比较相似，下面是常见的应用场景。
#### 1) 实现 int 和 Integer 的相互转换
可以通过 Integer 类的构造方法将 int 装箱，通过 Integer 类的 intValue 方法将 Integer 拆箱。例如：publicclassDemo{
publicstaticvoidmain(String[]args){
intm=500;
Integerobj=newInteger(m);// 手动装箱
intn=obj.intValue();// 手动拆箱
System.out.println("n = "+n);
Integerobj1=newInteger(500);
System.out.println("obj 等价于 obj1？"+obj.equals(obj1));
}
}

运行结果：
n = 500
obj 等价于 obj1？true
#### 2) 将字符串转换为整数
Integer 类有一个静态的 paseInt() 方法，可以将字符串转换为整数，语法为：parseInt(Strings,intradix);

s 为要转换的字符串，radix 为进制，可选，默认为十进制。
下面的代码将会告诉你什么样的字符串可以转换为整数：publicclassDemo{
publicstaticvoidmain(String[]args){
Stringstr[]={"123","123abc","abc123","abcxyz"};
for(Stringstr1:str){
try{
intm=Integer.parseInt(str1,10);
System.out.println(str1+" 可以转换为整数 "+m);
}catch(Exceptione){
System.out.println(str1+" 无法转换为整数");
}
}
}
}

运行结果：
123 可以转换为整数 123
123abc 无法转换为整数
abc123 无法转换为整数
abcxyz 无法转换为整数
#### 3) 将整数转换为字符串
Integer 类有一个静态的 toString() 方法，可以将整数转换为字符串。例如：publicclassDemo{
publicstaticvoidmain(String[]args){
intm=500;
Strings=Integer.toString(m);
System.out.println("s = "+s);
}
}

运行结果：
s = 500
## 自动拆箱和装箱
上面的例子都需要手动实例化一个包装类，称为手动拆箱装箱。Java 1.5(5.0) 之前必须手动拆箱装箱。
Java 1.5 之后可以自动拆箱装箱，也就是在进行基本数据类型和对应的包装类转换时，系统将自动进行，这将大大方便程序员的代码书写。例如：publicclassDemo{
publicstaticvoidmain(String[]args){
intm=500;
Integerobj=m;// 自动装箱
intn=obj;// 自动拆箱
System.out.println("n = "+n);
Integerobj1=500;
System.out.println("obj 等价于 obj1？"+obj.equals(obj1));
}
}

运行结果：
n = 500
obj 等价于 obj1？true
自动拆箱装箱是常用的一个功能，读者需要重点掌握。

