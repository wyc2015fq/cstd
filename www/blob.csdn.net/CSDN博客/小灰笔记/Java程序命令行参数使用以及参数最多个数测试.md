# Java程序命令行参数使用以及参数最多个数测试 - 小灰笔记 - CSDN博客





2017年03月10日 00:59:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2626








       在写Perl程序的时候我时常会使用命令行参数，这会让我的脚本使用起来十分灵活。使用了一段时间Python，这个功能考虑过但是从来没有去学习一下具体应该如何实现。今天看Java看到了命令行参数，感觉学到了一个令人十分高兴的新特性。Java中的命令行参数原来是通过主函数的参数实现的。这让我想到了之前看过的Python发明者的一篇文章，里面介绍了他说自己写的主函数中加入了各种参数。现在我还不是很清楚这两者是否是同一个事情，不过后期倒是可以去落实查看一下了。而今天，先看一下Java。

       写一段Java代码如下：

![](https://img-blog.csdn.net/20170310005901439?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



       上面的代码中用到了之前学习过程中一直没有使用到的主函数args参数。接下来程序编译执行如下：



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\args\src\args>javac ArgsDemo.java



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\args\src\args>java ArgsDemo 1 2

第1个参数为：1

第2个参数为：2



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\args\src\args>java ArgsDemo 1 2 34

第1个参数为：1

第2个参数为：2

第3个参数为：3

第4个参数为：4



E:\WorkSpace\02_技术实践\01_编程语言\05_Java\02_Java从入门到精通\04_数组\args\src\args>java ArgsDemo 1 2 34 Five

第1个参数为：1

第2个参数为：2

第3个参数为：3

第4个参数为：4

第5个参数为：Five

       从上面的结果倒是可以看出一个很有意思的地方：那就是主函数的参数个数以及参数的类型其实并没有限制。学习Java的基础知识的时候可以知道，数组中存储的对象类型必须一致。由此猜测，即使是参数中写入的是数字也会按照String的类型进行强制数据类型转换。还有一点就是，在一般的Java程序中，数组使用似乎需要先进行指定大小的数组对象创建，也就是内存分配。如果使用的过程中出现了数组的越界，编译虽说没有问题但是执行的时候会遇到数组越界的错误。很有意思的是，这个地方确实是没有遇到，不知道这个数组的大小在程序编译的时候到底分配了多大的内存空间？关于这个问题，回头倒是可以写个小脚本测试一下咯！

       先用Python写了一个不断测试的脚本，后来发现输出的信息太多，测试速度太慢。修改了原来的Java代码如下：



public classArgsDemo {



       public static void main(String[] args) {

              // TODO Auto-generated method stub

              int i;

              /* for(i = 0; i < args.length;i++)

              {

                     System.out.println("第"+ (i + 1) +"个参数为：" +args[i]);

              }*/

              System.out.println("第" + args.length + "个参数为: " +args[args.length -1]);



       }



}

而测试的Python脚本如下：

#!/usr/bin/python



import os



args = ''

num = 0



while True:

       num += 1

       args += (str(num) + ' ')

       try:

              os.system("java ArgsDemo%s" % args)

       except:

              print("size is: %d" %num)

              break

       其实，到我终止程序我才考虑到一个问题：Python不一定能够捕捉到Java的错误！不过这也没关系，如果真的出现了数组越界等问题，在不断回显的Java程序结果输出中我应该能够捕捉到相应的信息。我等了好长一段时间，一直到测试过了1000，到了1024依然没有报错。我终止了程序，接下来的测试即使是有一点意义也没有什么必要了。毕竟，我考虑不到我什么时候会在命令行中使用这么多的参数。而看着CPU的温度不断上升，我想程序运行的最后可能是诸如Java虚拟机分配不到足够的内存等错误吧？




