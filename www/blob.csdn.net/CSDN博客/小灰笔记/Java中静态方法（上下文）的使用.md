# Java中静态方法（上下文）的使用 - 小灰笔记 - CSDN博客





2017年03月15日 23:16:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：863








       从书中看了一下Java中静态方法的使用以及使用注意事项，总结下来的注意事项义工有两点：

       1，静态方法中不能够直接调用非静态变量；

       2，非静态方法中可以直接调用静态变量；

       至于其他的情况，之前基本的代码中都做了测试，不再赘述了。接下来写一段代码，主要是为了验证前面的第一点：

public classStaticMethod

{

       int my_num = 123;



       public static void main(String[] args)

       {

              StaticMethod obj = newStaticMethod();

              obj.MyStaticFunc();

       }



       static void MyStaticFunc()

       {

              System.out.println("staticfunction!");

              System.out.println("my num:" + my_num);

       }

}public classStaticMethod

{

       int my_num = 123;



       public static void main(String[] args)

       {

              StaticMethod obj = newStaticMethod();

              obj.MyStaticFunc();

       }



       static void MyStaticFunc()

       {

              System.out.println("staticfunction!");

              System.out.println("my num:" + my_num);

       }

}

       编译运行结果;

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>javac StaticMethod.java

StaticMethod.java:14:错误: 无法从静态上下文中引用非静态 变量 my_num

                System.out.println("mynum: " + my_num);

                                               ^

1 个错误

       从上面的错误提示中可以看到比较直接的结论，从静态方法（上下文）中引用非静态变量！修改代码如下：

public classStaticMethod

{

       static int my_num = 123;



       public static void main(String[] args)

       {

              StaticMethod obj = newStaticMethod();

              obj.MyStaticFunc();

       }



       static void MyStaticFunc()

       {

              System.out.println("staticfunction!");

              System.out.println("my num:" + my_num);

       }

}

       代码编译运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>javac StaticMethod.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>java StaticMethod

static function!

my num: 123

       从上面的结果中可以看出，静态方法（上下文）调用静态变量是没有问题的。至于其他的验证，暂时应该是没有必要了，先前在学习其他的基础时在主函数中都写过类似的功能。简单只测试一下前面的第2点，修改代码如下：

public classStaticMethod

{

       static int my_num = 123;

       int my_normal_num = 456;



       public static void main(String[] args)

       {

              StaticMethod obj = newStaticMethod();

              obj.MyStaticFunc();

              obj.TestFunc();

       }



       static void MyStaticFunc()

       {

              System.out.println("staticfunction!");

              System.out.println("my num:" + my_num);

       }



       void TestFunc()

       {

              System.out.println("normalfunction!");

              System.out.println("my staticnum: " + my_num);

              System.out.println("my normalnum: " + my_normal_num);

       }

}

       代码的编译运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>javac StaticMethod.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\05_类和对象>java StaticMethod

static function!

my num: 123

normal function!

my static num: 123

my normal num: 456

       由上面的结果可以看出，非静态方法中对各种变量的调用都是自由的。



