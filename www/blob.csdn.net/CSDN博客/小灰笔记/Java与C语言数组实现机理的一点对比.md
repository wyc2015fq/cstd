# Java与C语言数组实现机理的一点对比 - 小灰笔记 - CSDN博客





2017年03月12日 12:31:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：830








       想看一下Java的数组实现机制是否与C语言一致，同时看看是不是被隐藏掉的指针功能实现的类似数组访问功能。Java中是不是还有什么类似简写这样的表示说明呢？写了两段简单的代码示意一下。

C语言代码：

#include"stdio.h"



int array_data[] ={1,5,3,6,2,4};



int main(void)

{

       printf("%d\n",array_data[3]);

       printf("%d\n",3[array_data]);



       return 0;

}

编译运行结果：

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\c_array_demo>gcc c_array_demo.c

E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\c_array_demo>a

6

6

       程序的运行是没有问题的，之所有两种数组元素的索引方式相同是因为C语言的数组访问本质上来说是指针的操作。而取数组元素本来也只是对指针表示的一种简写的方式，因此数组名在前在后并没有什么差异。

Java代码：

**package**JavaArrayPack;



**publicclass** JavaArrayClass {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[]
array_data = {1,5,3,6,2,4};



        System.***out***.println(array_data[3]);

        System.***out***.println(3[array_data]);

    }



}

    实际上，编写完之后IDE就已经提示这种方式不正确了。强制编译运行一下报错如下：

Exceptionin thread "main" java.lang.Error: Unresolved compilation problems:


    The type of the expression must be an arraytype but it resolved to int

    Type mismatch: cannot convert from int[] toint



    atJavaArrayPack.JavaArrayClass.main(JavaArrayClass.java:10)

       这么看来，Java中数组的实现机制确实是没有什么隐藏的指针的影子。因此，也就不存在什么等效的表示方法。把上面的错误代码屏蔽掉：

**package**JavaArrayPack;



**publicclass** JavaArrayClass {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[]
array_data = {1,5,3,6,2,4};



        System.***out***.println(array_data[3]);

/* System.out.println(3[array_data]); */

    }



}

       重新编译运行结果如下：

6

       这么看来，Java中省掉了指针的麻烦确实是有道理的。C语言中的指针虽然好用，给设计者很大的灵活性，但是也在这方面埋藏了很多风险。类似Java这种直接把相应的风险规避掉的语言，在软件实现上还是有一定的安全性保障的。



