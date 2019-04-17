# Java多线程：实现一个死锁案例 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 10:35:41[QuJack](https://me.csdn.net/A784586)阅读数：443








package cn.itcast.com.javase;

class myLock{ //构建两把不同的锁，实现两把锁的嵌套

public static final Object locka=new Object();

public static final Object lockb=new Object();

      }




   class ContentofSyn implements Runnable

   { 

  private boolean flag;

  ContentofSyn(boolean flag){

  this.flag=flag;

  }



public void run(){

 if(flag)

 while(true){

 synchronized(myLock.locka){

 System.out.println(Thread.currentThread().getName()+"...locka...");

 synchronized(myLock.lockb){

 System.out.println(Thread.currentThread().getName()+".....lockb....");

 }

 }

 }

 else while(true)

 {

 synchronized(myLock.lockb){

 System.out.println(Thread.currentThread().getName()+"...lockb...");

 synchronized(myLock.locka){

 System.out.println(Thread.currentThread().getName()+".....locka....");

 }

 }

 }

   }

}










public class DeadLocktest {

public static void main(String[] args) {

ContentofSyn a=new ContentofSyn(true);

ContentofSyn b=new ContentofSyn(false);

Thread t1=new Thread(a);

Thread t2=new Thread(b);

            t1.start();

            t2.start();

}




}





