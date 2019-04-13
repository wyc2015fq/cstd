
# java线程概述及实现 - 李昆鹏的博客 - CSDN博客


2018年04月13日 13:57:11[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：37


----------------------------java线程概述及实现-------------------------

**线程概述**
进程：计算机中特定功能的程序在数据集上的一次运行。
线程：线程是进程的一个单元。
多线程：一个进程中有多个线程在同时运行，
如迅雷下载，迅雷软件的一次运行就是一个进程，那么在迅雷中可以同时下载多个电影，这就是多线程（每一个下载都是一个线程）。
JVM是多线程的，在我们运行JVM的时候后台会运行垃圾回收的线程，来清理没有被引用的对象。
![](https://img-blog.csdn.net/20180413135442873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
------------------------------------------------------------
**线程的实现**
创建新执行线程有两种方法。一种方法是将类声明为Thread的子类，
该子类应重写Thread类的run方法。接下来可以分配并启动该子类的实例。
线程启动的时候使用线程的start方法而不是run。
例子：
创建Thread的子类并且重写run方法
**public****class**CountThread**extends**Thread {
**public**CountThread() {
**super**();
}
/**
*给线程取名的构造器
***@param**name
*/
**public**CountThread(Stringname) {
**super**(name);
}
/**
* run方法是线程的执行体
*/
@Override
**public****void**run() {
**for**(**int**i= 0;i< 10;i++) {
System.**out**.println(**this**.getName() +"hello"+i);
}
}
}
----------------------------------------------------
然后创建一个类启动线程
**public****class**ThreadDemo {
**public****static****void**main(String[]args) {
//创建线程的实例对象
//CountThreadct= new  CountThread();
//CountThread ct1 = new  CountThread();
//给线程取名
//ct.setName("线程1");
//ct1.setName("线程2");
//直接通过构造器给线程取名
CountThreadct=**new**CountThread("线程1");
CountThreadct1=**new**CountThread("线程2");
//启动线程
ct.start();
ct1.start();
//当12行代码执行时有3个线程在跑
//结果
/*
线程2hello0
线程2hello1
线程2hello2
线程2hello3
线程1hello0
线程1hello1
线程1hello2
线程2hello4
线程2hello5
线程2hello6
线程2hello7
线程1hello3
线程2hello8
线程2hello9
线程1hello4
线程1hello5
线程1hello6
线程1hello7
线程1hello8
线程1hello9
*/
}
}

==================================================
线程的另一种实现方法：
创建线程的另一种方法是声明实现Runnable接口的类，该类然后实现run方法，然后可以分配该类的实例，在创建Thread时作为一个参数来传递并启动。
Thread类中的currentThread()情态方法可以返回当前运行的线程
例子：
首先创建一个实现Runnable接口的类
**public****class**CountThread2**implements**Runnable {
@Override
**public****void**run() {
**for**(**int**i= 0;i< 10;i++) {
//输出线程的名字
System.**out**.println(Thread.*currentThread*().getName()+"hello"+i);
}
}
}
-----------------------------------------------------------------
然后创建线程
**public****class**ThreadDemo2 {
**public s****tatic****void**main(String[]args) {
//创建2个线程
CountThread2ct=**new**CountThread2();
Threadt1=**new**Thread(ct);
CountThread2ct1=**new**CountThread2();
Threadt11=**new**Thread(ct1);
/*      t1.start();
t11.start();
Thread-1hello0
Thread-1hello1
Thread-0hello0
Thread-1hello2
Thread-0hello1
Thread-0hello2
Thread-0hello3
Thread-0hello4
Thread-0hello5
Thread-1hello3
Thread-0hello6
Thread-1hello4
Thread-0hello7
Thread-0hello8
Thread-0hello9
Thread-1hello5
Thread-1hello6
Thread-1hello7
Thread-1hello8
Thread-1hello9*/
}
}


