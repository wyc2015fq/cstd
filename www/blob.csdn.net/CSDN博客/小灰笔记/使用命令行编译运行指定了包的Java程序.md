# 使用命令行编译运行指定了包的Java程序 - 小灰笔记 - CSDN博客





2017年03月15日 23:32:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：220








       曾经因为这个问题，我由命令行转向了eclipseIDE。近段时间学习的平台转换了一下，看了一点其它的介绍。今天把简单能够使用的方式给稍微练习总结一下。

       首先，包的指定方式是通过关键字package。如果使用了包，相应的代码文件需要放到一个跟包名相同的文件夹中。把之前的代码修改如下：

package PkgDemo;



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

       按照前面所说，这段代码的源文件需要放到一个名为PkgDemo的文件夹中。这样，进行编译的时候需要切换到相应的目录切换。而执行的时候需要切换到包所在的根目录执行，并且执行的时候需要先指明包然后通过包访问类。具体的操作如下：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>cd PkgDemo



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象\PkgDemo>javac StaticDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象\PkgDemo>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象\PkgDemo 的目录



2017/03/15  23:26   <DIR>          .

2017/03/15  23:26   <DIR>          ..

2017/03/15  23:26               329 Demo.class

2017/03/15  23:26               951 StaticDemo.class

2017/03/15  23:26               607 StaticDemo.java

               3 个文件          1,887 字节

               2 个目录79,102,382,080 可用字节



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象\PkgDemo>cd ../



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>java PkgDemo.StaticDemo

static value ofvar1:123

static value ofvar2:123

normal value ofvar1:234

normal value ofvar2:0

       从上面的结果看，一切运行正常。不过这距离自己构建makefile进行自动化编译还缺少一点东西，那就是对包的引用。后期这部分定然还会攻克下来，到时候的软件开发工具选择上就会更加自耦一些了。



