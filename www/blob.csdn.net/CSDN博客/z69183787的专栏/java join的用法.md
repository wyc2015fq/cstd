# java join的用法 - z69183787的专栏 - CSDN博客
2014年04月09日 11:27:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1087
个人分类：[Java-知识																[多线程](https://blog.csdn.net/z69183787/article/category/2176989)](https://blog.csdn.net/z69183787/article/category/2175279)
join的用法，先看几个例子在说。
程序1：
public class ThreadTest implements Runnable {
 public static int a = 0;
 public synchronized void inc() {
  a++;
 }
 public void run() {
  for (int i = 0; i < 5; i++) {
   inc();
  }
 }
 public static void main(String[] args) throws Exception {
  Runnable r = new ThreadTest();
  Thread t1 = new Thread(r);
  t1.start();
**System.out.println(a);**
 }
}
咋一看以为结果是：5  ，其实不然
程序2：在一基础上添加点东西 **System.out.println(a+",");**
public class ThreadTest implements Runnable {
 public static int a = 0;
 public synchronized void inc() {
  a++;
**System.out.println(a+",");**
 }
 public void run() {
  for (int i = 0; i < 5; i++) {
   inc();
  }
 }
 public static void main(String[] args) throws Exception {
  Runnable r = new ThreadTest();
  Thread t1 = new Thread(r);
  t1.start();
**System.out.println(a);**
 }
}
结果：
0
1，2，3，4，5，
说明a已经被加了**5**次。但有两个问题
1）为什么main方法中的打印语句还是**0**呢？
2）为什么main方法中的打印语句先执行了呢？
程序3
public class ThreadTest implements Runnable {
 public static int a = 0;
 public synchronized void inc() {
  a++;
 }
 public void run() {
  for (int i = 0; i < 5; i++) {
   inc();
  }
 }
 public static void main(String[] args) throws Exception {
  Runnable r = new ThreadTest();
  Thread t1 = new Thread(r);
  t1.start();
**   for (int i=0; i<300; i++) {      System.out.print(i);   }   System.out.println();   System.out.println(a);**
 }
}
结果：
0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253254255256257258259260261262263264265266267268269270271272273274275276277278279280281282283284285286287288289290291292293294295296297298299
5
此时的结果5正确了。**说明线程在i循环100次后，全部都结束了。**
之所以会产生**1）**和**2）原因是：**由于有两个线程,我们不知道主线程执行到什么时候开始执行子线程,这是由于虽然你调用start方法但是调用该方法只是准备线程并不是马上启动.所以,当程序执行的时候,很随机地执行这两个线程中的一个,也就很随机地执行System.out.println(a); 这个语句了.于是,有可能子线程只执行了1次a=+1这个语句,也可能是2次或者其它多少次,还可能是0次,所以打印的结果是个随机数
程序4：在程序1的基础上添加 **t1.join();** 达到与程序3同样的效果
public class ThreadTest implements Runnable {
 public static int a = 0;
 public synchronized void inc() {
  a++;
 }
 public void run() {
  for (int i = 0; i < 5; i++) {
   inc();
  }
 }
 public static void main(String[] args) throws Exception {
  Runnable r = new ThreadTest();
  Thread t1 = new Thread(r);
  t1.start();
** t1.join();   System.out.println(a);**
 }
}
结果：5
总结：join方法的功能就是使异步执行的线程变成同步执行。也就是说，当调用线程实例的start方法后，这个方法会立即返回，如果在调用start方法后后需要使用一个由这个线程计算得到的值，就必须使用join方法。如果不使用join方法，就不能保证当执行到start方法后面的某条语句时，这个线程一定会执行完。而使用join方法后，直到这个线程退出，程序才会往下执行。
