# 线程第八课——sleep方法 - PeterBishop - CSDN博客





2018年11月28日 11:40:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：14
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们就讲一个方法，Thread.sleep(long millis)



  首先补充一个知识点


public static void main(String[] args) {

    System.*out*.println(Thread.*currentThread*().getName());

}


运行结果；

main



可以看到其实main方法自启动了一个名为main的线程。



接下来讲讲sleep



Demo01:
public static void main(String[] args) throws InterruptedException {

    for (int i = 0; i < 5; i++) {

        Thread.*sleep*(1000);

        System.*out*.println(i);

    }

}


运行结果:

每隔一秒输出一个数(0,1,2,3,4)



讲解:
public static native void sleep(long millis) throws InterruptedException;


又看到了一个新的关键字，native



由native标识的方法叫本地方法，该方法的功能是由额外的C++代码实现的，而这个C++代码其实就在你的电脑本地里



下面讲讲sleep的作用:

暂停当前线程的运行，被暂停的线程会进入等待状态，当暂停时间过后自定进入就绪队列。



Demo02 :
public static void main(String[] args) throws InterruptedException {

    Thread thread1 = new Thread(new Runnable() {

        @Override

        public void run() {

            try {

                Thread.*sleep*(1000);

                System.*out*.println("线程1");

            } catch (InterruptedException e) {

                e.printStackTrace();

            }

        }

    },"线程1");



    Thread thread2 = new Thread(new Runnable() {

        @Override

        public void run() {

            try {

                Thread.*sleep*(1000);

            } catch (InterruptedException e) {

                e.printStackTrace();

            }

            System.*out*.println("线程2");

        }

    },"线程2");



    thread1.start();

    thread2.start();

}


运行结果:

在等待1s后输出了线程1和线程2(顺序不确定)



