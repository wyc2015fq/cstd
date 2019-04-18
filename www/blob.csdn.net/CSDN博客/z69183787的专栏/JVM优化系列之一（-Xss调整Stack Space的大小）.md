# JVM优化系列之一（-Xss调整Stack Space的大小） - z69183787的专栏 - CSDN博客
2018年02月01日 14:34:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2536
[http://blog.csdn.net/zhuyijian135757/article/details/38025339](http://blog.csdn.net/zhuyijian135757/article/details/38025339)
Java程序中，每个线程都有自己的Stack Space(堆栈)。这个Stack Space不是来自Heap的分配。所以Stack
 Space的大小不会受到-Xmx和-Xms的影响，这2个JVM参数仅仅是影响Heap的大小。
Stack Space用来做方法的递归调用时压入Stack Frame(栈帧)。所以当递归调用太深的时候，就有可能耗尽Stack Space，爆出StackOverflow的错误。
-Xss128k：设置每个线程的堆栈大小。JDK5.0以后每个线程堆 栈大小为1M，以前每个线程堆栈大小为256K。根据应用的线程所需内存大小进行调整。在相同物理内存下，减小这个值能生成更多的线程。但是操作系统对一 个进程内的线程数还是有限制的，不能无限生成，经验值在3000~5000左右。
线程栈的大小是个双刃剑，如果设置过小，可能会出现栈溢出，特别是在该线程内有递归、大的循环时出现溢出的可能性更大，如果该值设置过大，就有影响到创建栈的数量，如果是多线程的应用，就会出现内存溢出的错误．
JVM可创建的最大线程数限制因素:
线程堆栈大小——》进程的最大内存——》操作系统位数
package org.thinking.jvm;
publicclass TestXSS {
privatestaticlong count = 0;
publicstaticvoid main(String[] args) {
        infinitelyRecursiveMethod(1);
    }
publicstaticvoid infinitelyRecursiveMethod(long a){
        System.out.println(count++);
        infinitelyRecursiveMethod(a);
    }
}
通过上面的程序，你可以观察到你的JVM Stack Space是如何耗尽的，以及进入嵌套的层深。
