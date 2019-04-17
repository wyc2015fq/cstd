# Java中的静态变量使用 - 小灰笔记 - CSDN博客





2017年03月15日 20:20:44[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：374








       同样是static的关键字修饰，但是感觉Java中的静态变量跟C语言中的静态变量还真不是一回事儿。从作用的广度上来说，Java重的static修饰后的静态变量倒是作用的广度更广一些。

       写一段简单的代码：

public classStaticDemo

{

       public static void main(String[] args)

       {

              Demo demo_var1 = new Demo();

              Demo demo_var2 = new Demo();



              demo_var1.static_value = 123;



              System.out.println("staticvalue of var1:" + demo_var1.static_value);

              System.out.println("staticvalue of var2:" + demo_var1.static_value);



              demo_var1.normal_value = 234;



             System.out.println("normalvalue of var1:" + demo_var1.normal_value);

             System.out.println("normalvalue of var2:" + demo_var2.normal_value);

       }

}



class Demo

{

       static int static_value = 0;

       int normal_value = 0;

}

       编译执行如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象 的目录



2017/03/15  20:13   <DIR>          .

2017/03/15  20:13   <DIR>          ..

2017/03/15  19:43               587 StaticDemo.java

               1 个文件            587 字节

               2 个目录79,102,492,672 可用字节



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>javac StaticDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象 的目录



2017/03/15  20:13   <DIR>          .

2017/03/15  20:13    <DIR>          ..

2017/03/15  20:13               321 Demo.class

2017/03/15  20:13               935 StaticDemo.class

2017/03/15  19:43               587 StaticDemo.java

               3 个文件          1,843 字节

               2 个目录79,102,488,576 可用字节



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>java StaticDemo

static value ofvar1:123

static value ofvar2:123

normal value ofvar1:234

normal value ofvar2:0

       从运行的结果来看，静态变量修改过之后所有的此类对象相应的属性全都会修改。相比之下，普通的属性则不是这样。同时值得注意的是，编译之后产生了两个class文件。这是因为java代码文件中可以存在多个类的定义，但是只有一个类是public修饰的类，而且这个类的名称必须与文件名一致。而编译的过程中，每一个类的代码都会对应着一个相应的class文件。



