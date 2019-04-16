# 线程第六课——同步锁synchronized - PeterBishop - CSDN博客





2018年11月28日 11:40:25[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：13
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来讲讲同步锁synchronized



  先来说说为什么要有这个玩意。多线程在并发访问共享资源时可能会引发安全问题(上节课演示过了)，为此我们需要在某一个线程访问共享资源时拒绝让其他线程继续访问。



  举个例子，厕所是公共资源吧，每个人就是一个线程，当一个人使用一个厕所的时候是不是要在自己使用期间阻止其他人使用？那么怎么阻止？把门锁上即可。这把锁就是我们今天要讲的synchronized



```
public class SynchronizedTeach {



    public static void main(String[] args) {



        MineThread thread1 = new MineThread();

        MineThread thread2 = new MineThread();



        thread1.start();

        thread2.start();



        while (true)

        {

            if (thread1.getState() == Thread.State.TERMINATED && thread2.getState() == Thread.State.TERMINATED)

            {

                System.out.println(MineThread.i.get());

                break;

            }

        }



        /**

         * 在没有枷锁的情况下 如预料般的结果出错了

         * 下面说下synchronized可以加在哪里:

         * 1. 普通方法上，上锁的是当前对象

         *        相当于在方法里写synchronized(this)

         * 2. 静态方法上，上说的是这个类的Class对象

         *        相当于在方法里写synchronized(this.getClass())或者synchronized(本类.class)

         * 3. (普通/静态)方法里写

         *      synchronized(obj){

         *          需要上锁的代码

         *      }

         *      obj是要被锁的对象

         */

    }

}



//自己写个线程吧

class MineThread extends Thread{



    public static Int i = new Int(0);



    public MineThread() {

    }



    public MineThread(Runnable target) {

        super(target);

    }



    public MineThread(String name) {

        super(name);

    }



    public MineThread(Runnable target, String name) {

        super(target, name);

    }



    /*



        当synchronized被放在方法上时，锁的是当前对象(也就是this)，但是i是静态成员，锁当前对象是锁不住的



    @Override

    public synchronized void run() {

        for (int j = 0; j < 4000; j++)

        {

            i.add();

        }

    }

    */



    /*



        这次锁的对象是静态成员i，在操作i时对i上锁了，其他线程就无法在该线程访问i期间访问i，所以不会引发安全问题



    @Override

    public  void run() {

        for (int j = 0; j < 4000; j++)

        {

            synchronized (i)

            {

                i.add();

            }



        }

    }

    */



    /*



    这次锁的是类的Class对象，在锁Class对象时一切对该类的静态成员的操作都会上锁，即当一个线程操作这个类的静态成员时，其他线程无法操作这个类的任何静态成员



    @Override

    public void run() {

        for (int j = 0; j < 4000; j++) {

            add();

        }

    }



    public synchronized static void add(){

        i.add();

    }

    */

}



class Int{

    private int i;



    public Int(int i)

    {

        this.i = i;

    }



    public int get()

    {

        return i;

    }



    public void add()

    {

        i++;

    }

}
```





