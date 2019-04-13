
# 线程安全的AtomicLong使用 - leofionn的博客 - CSDN博客


2018年05月21日 18:10:13[leofionn](https://me.csdn.net/qq_36142114)阅读数：55个人分类：[spark																](https://blog.csdn.net/qq_36142114/article/category/7640197)


看一个计数的类：
[java][view plain](https://blog.csdn.net/yaqingwa/article/details/17737771#)[copy](https://blog.csdn.net/yaqingwa/article/details/17737771#)


[java][view plain](https://blog.csdn.net/yaqingwa/article/details/17737771#)[copy](https://blog.csdn.net/yaqingwa/article/details/17737771#)

publicclassCounter {
privatestaticlongcounter =0;
publicstaticlongaddOne(){
return++counter;
}
}

初看感觉没啥问题，但这个类在多线程的环境下就会有问题了。
假如开多个线程都来使用这个计数类，它会表现的“不稳定”

[java][view plain](https://blog.csdn.net/yaqingwa/article/details/17737771#)[copy](https://blog.csdn.net/yaqingwa/article/details/17737771#)

publicstaticvoidmain(String[] args) {
for(inti=0;i<100;i++){
Thread thread =newThread(){
@Override
publicvoidrun() {
try{
Thread.sleep(100);
if(Counter.addOne() ==100){
System.out.println("counter = 100");
}
}catch(InterruptedException e) {
e.printStackTrace();
}
}
};
thread.start();
}
}

程序会开100个线程，每个线程都会把counter 加一。那么应该有一个线程拿到counter =100的值，但实际运行情况是大多数据情况下拿不到100，在少数情况能拿到100.
因为 Counter 类在 addOne()方法被调用时，并不能保证线程的安全，即它不是原子级别的运行性，而是分多个步骤的，打个比方：
线程1首先取到counter 值，比如为10，然后它准备加1，这时候被线程2占用了cpu，它取到counter为10，然后加了1，得到了11。这时线程1 又拿到了CPU资源，继续它的步骤，加1为11，然后也得到了11。这就有问题了。
那么怎么解决它呢？JDK在concurrent包里提供了一些线程安全的基本数据类型的实现，比如 Long型对应的concurrent包的类是AtomicLong。
现在修改下代码:
[java][view plain](https://blog.csdn.net/yaqingwa/article/details/17737771#)[copy](https://blog.csdn.net/yaqingwa/article/details/17737771#)

importjava.util.concurrent.atomic.AtomicLong;

publicclassCounter {
privatestaticAtomicLong counter =newAtomicLong(0);

publicstaticlongaddOne() {
returncounter.incrementAndGet();
}
}


运行了多次，结果都是能输出counter = 100。

所以在多线程环境下，可以简单使用AtomicXXX 使代码变得线程安全。
转载请注明出处：[http://blog.csdn.net/yaqingwa/article/details/17737771](http://blog.csdn.net/yaqingwa/article/details/17737771)

