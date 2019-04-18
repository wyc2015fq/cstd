# java线程技术7_线程中断 - z69183787的专栏 - CSDN博客
2014年08月05日 21:18:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：702
个人分类：[多线程-线程中断](https://blog.csdn.net/z69183787/article/category/6822115)
1.中断概述
      在多线程编程中经常会遇到需要中止线程的情况，比如启动多个线程去数据库中搜索，如果有一个线程返回了结果，其他线程就可以取消了。
中断线程的三个相差函数
            1.通过成员方法Thread.interrupt()来设置中断状态为true
2.通过成员方法Thread.isInterrupted()来获取中断状态
3.通过静态方法Thread.interrupted()来获取中断状态，并且清除中断状态（当然获取的是清除之前的值），也就是说连续两次调用此方法，第二次一定会返回false。
对正在运行的线程调用interrupt()，并不会使线程停止运行，而只是让线程暂停一会，详见《例1：中断但不停止线程的运行》。因为Thread.interrupt()
 对正在运行的线程是不起作用的，只有对阻塞的线程有效。
离开线程有三种常用的方法：
1.在阻塞操作时如Thread.sleep()时被中断会抛出InterruptedException
Thread.interrupt()方法实际上只是设置了一个中断状态，当该线程由于下列原因而受阻时，这个中断状态就起作用了：
      （1）如果线程在调用 Object 类的 wait()、wait(long) 或 wait(long, int) 方法，或者该类的 join()、join(long)、join(long, int)、sleep(long) 或 sleep(long, int) 方法过程中受阻，则其中断状态将被清除，它还将收到一个InterruptedException异常。这个时候，我们可以通过捕获
 InterruptedException异常来终止线程的执行，具体可以通过return等退出或改变共享变量的值使其退出。
      详见《例2：线程在sleep时调用interrupt》
      （2）如果该线程在可中断的通道上的 I/O 操作中受阻，则该通道将被关闭，该线程的中断状态将被设置并且该线程将收到一个
 ClosedByInterruptException，而不是InterruptedException 异常。
      （3）如果使用Java1.0之前就存在的传统的I/O操作，并且线程处于阻塞状态，Thread.interrupt()将不起作用，线程并不能退出阻塞状态。
               例如对于socket，通过调用阻塞该线程的套接字的close()方法。如果线程被I/O操作阻塞，该线程将接收到一个SocketException异常，这与使用interrupt()方法引起一个InterruptedException异常被抛出非常相似。
      详见《例6：通过SocketException异常中断阻塞线程》
2.Thread.interrupted()检查是否发生中断
      对于正在运行的线程，如果调用thread.interrupt()。可以通过Thread.interrupted()能告诉你线程是否发生中断,并将清除中断状态标记，所以程序不会两次通知你线程发生了中断。 
      详见《例3：通过interrupted中断线程》
      如果sleep和interrupted检查结合使用，可能会产生两个结果。
      详见《例4： 通过interrupted和sleep中断线程，停止线程的执行》
3.使用共享变量控制
      使用共享变量（shared variable）发出信号，告诉线程必须停止正在运行的任务。线程必须周期性的核查这一变量（尤其在冗余操作期间），然后有秩序地中止任务。
      详见《例5：通过共享变量中断线程，停止线程的执行》
      这个方法虽然给予线程机会进行必要的清理工作，这在任何一个多线程应用程序中都是绝对需要的。请确认将共享变量定义成volatile 类型或将对它的一切访问封入同步的块/方法（synchronized blocks/methods）中。
      但是，当线程等待某些事件发生而被阻塞，又会发生什么？当然，如果线程被阻塞，它便不能核查共享变量，也就不能停止。
      他们都可能永久的阻塞线程。即使发生超时，在超时期满之前持续等待也是不可行和不适当的，所以，要使用某种机制使得线程更早地退出被阻塞的状态。
总结
      不存在这样一种机制对所有的情况都适用，但是，根据情况不同却可以使用特定的技术。
      详见《java线程技术8_线程中断最佳实践》
例1：中断但不停止线程的运行
假设我们想中断一个正在执行的线程。代码如下
- /**
- 
 * 中断一个正在执行的线程，但是并未实现中断.
- 
 * @version V1.0 ,2011-4-15
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread1 extendsThread{
- privatedouble d = 0.0;
- publicvoidrun(){
- // 死循环执行打印"I am running!" 和做消耗时间的浮点计算
- while(true){
- System.out.println("I
 am running!");
- 
- for(int i = 0; i < 900000; i++){
- 
                d = d +(Math.PI +Math.E)/ d;
- }
- // 给线程调度器可以切换到其它进程的信号
- Thread.yield();
- }
- }
- publicstaticvoid main(String[] args)throwsException{
- // 将任务交给一个线程执行
- 
        InterruptThread1 t =new InterruptThread1();
- 
        t.start();
- // 运行一断时间中断线程
- Thread.sleep(100);
- System.out.println("****************************");
- System.out.println("Interrupted
 Thread!");
- System.out.println("****************************");
- 
        t.interrupt();
- }
- }
运行结果
- I am 
- 
I am 
- 
I am 
- ****************************
- Interrupted
- ****************************
- 
I am 
- 
I am
从结果可以看出，中断的调用并未让线程停止。
例2：线程在sleep时调用interrupt，停止线程的运行
- /**
- 
 * 通过线程sleep时调用Interrupt引发异常，停止线程的运行.
- 
 * @version V1.0 ,2011-4-15
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread1 extendsThread{
- privatedouble d = 0.0;
- 
- publicvoidrun(){
- try{
- // 死循环执行打印"I am running!" 和做消耗时间的浮点计算
- while(true){
- System.out.println("I
 am running!");
- 
- for(int i = 0; i < 900000; i++){
- 
                d = d +(Math.PI +Math.E)/ d;
- }
- //休眠一断时间,中断时会抛出InterruptedException 
- Thread.sleep(50);
- }
- }catch(InterruptedException e){
- System.out.println("InterruptThread1.run()
 interrupted!");
- }
- }
- 
- publicstaticvoid main(String[] args)throwsException{
- // 将任务交给一个线程执行
- 
        InterruptThread1 t =new InterruptThread1();
- 
        t.start();
- 
- // 运行一断时间中断线程
- Thread.sleep(100);
- System.out.println("****************************");
- System.out.println("Interrupted
 Thread!");
- System.out.println("****************************");
- 
        t.interrupt();
- }
- }
运行结果
- 
I am running!
- 
****************************
- 
Interrupted Thread!
- 
****************************
- 
I am running!
- 
ATask.run() interrupted!
例3：通过interrupted中断线程
- /**
- 
 * 通过interrupted中断线程，停止线程的执行.
- 
 * 
- 
 * @version V1.0 ,2011-4-15
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread1 extendsThread{
- privatedouble d = 0.0;
- 
- publicvoidrun(){
- // 检查程序是否发生中断
- while(!Thread.interrupted()){
- System.out.println("I
 am running!");
- 
- for(int i = 0; i < 900000; i++){
- 
                d = d +(Math.PI +Math.E)/ d;
- }
- }
- }
- 
- publicstaticvoid main(String[] args)throwsException{
- // 将任务交给一个线程执行
- 
        InterruptThread1 t =new InterruptThread1();
- 
        t.start();
- 
- // 运行一断时间中断线程
- Thread.sleep(100);
- System.out.println("****************************");
- System.out.println("Interrupted
 Thread!");
- System.out.println("****************************");
- 
        t.interrupt();
- }
- }
运行结果
- 
I am running!
- 
I am running!
- 
****************************
- 
Interrupted Thread!
- 
****************************
例4： 通过interrupted和sleep中断线程，停止线程的执行
- /**
- 
 * 通过interrupted和sleep中断线程，停止线程的执行.
- 
 * 
- 
 * @version V1.0 ,2011-4-15
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread1 extendsThread{
- privatedouble d = 0.0;
- 
- publicvoidrun(){
- try{
- // 检查程序是否发生中断
- while(!Thread.interrupted()){
- System.out.println("I
 am running!");
- // before sleep
- Thread.sleep(20);
- //after sleep
- System.out.println("Calculating");
- for(int i = 0; i < 900000; i++){
- 
                    d = d +(Math.PI +Math.E)/ d;
- }
- }
- 
- }catch(InterruptedException e){
- System.out.println("InterruptThread1.run()
 Exception!");
- }
- 
- System.out.println("InterruptThread1.run()
 end!");
- }
- 
- publicstaticvoid main(String[] args)throwsException{
- // 将任务交给一个线程执行
- 
        InterruptThread1 t =new InterruptThread1();
- 
        t.start();
- 
- // 运行一断时间中断线程
- Thread.sleep(200);
- System.out.println("****************************");
- System.out.println("Interrupted
 Thread!");
- System.out.println("****************************");
- 
        t.interrupt();
- }
- }
运行结果1
- 
I am running!
- 
Calculating
- 
I am running!
- 
****************************
- 
Interrupted Thread!
- 
****************************
- 
InterruptThread1.run() Exception!
- 
InterruptThread1.run() end!
运行结果2
- 
I am running!
- 
Calculating
- 
****************************
- 
Interrupted Thread!
- 
****************************
- 
InterruptThread1.run() end!
如果在睡眠之前产生中断，则调用Thread.sleep()时抛出InterruptedException，结束线程，参见运行结果1
如果在睡眠之后产生中断，则线程会继续执行到下一次while判断中断状态时，结束线程，参见运行结果2
例5：通过共享变量中断线程，停止线程的执行
- /**
- 
 * 通过共享变量中断线程，停止线程的执行.
- 
 * @version V1.0 ,2011-4-15
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread1 extendsThread{
- privatedouble d = 0.0;
- volatilebooleanstop= false;
- 
- publicvoidrun(){
- // 检查程序是否发生中断
- while(!stop){
- System.out.println("I
 am running!");
- 
- for(int i = 0; i < 900000; i++){
- 
                d = d +(Math.PI +Math.E)/ d;
- }
- }
- //做一些清理工作
- System.out.println("Thread
 is exiting under request...");
- }
- 
- publicstaticvoid main(String[] args)throwsException{
- // 将任务交给一个线程执行
- 
        InterruptThread1 t =new InterruptThread1();
- 
        t.start();
- 
- // 运行一断时间中断线程
- Thread.sleep(100);
- System.out.println("Asking
 thread to stop...");
- 
        t.stop= true;
- 
- Thread.sleep(1000 );
- System.out.println("Stopping
 application...");
- }
- }
例6：通过SocketException异常中断阻塞线程
- importjava.io.IOException;
- importjava.net.ServerSocket;
- importjava.net.Socket;
- 
- /**
- 
 * 通过SocketException异常中断阻塞线程.
- 
 * @version V1.0 ,2011-4-17
- 
 * @author xiahui
- 
 */
- publicclass InterruptThread2 extendsThread{
- volatilebooleanstop= false;
- volatileServerSocketsocket;
- 
- publicvoidrun(){
- try{
- socket=newServerSocket(7856);
- }catch(IOException e){
- System.out.println("Could
 not create the socket...");
- return;
- }
- while(!stop){
- System.out.println("Waiting
 for connection...");
- try{
- Socket sock =socket.accept();
- }catch(IOException e){
- System.out.println("accept()
 failed or interrupted...");
- }
- }
- System.out.println("Thread
 exiting under request...");
- }
- 
- publicstaticvoid main(String args[])throwsException{
- 
        InterruptThread2 thread=new InterruptThread2();
- System.out.println("Starting
 thread...");
- thread.start();
- Thread.sleep(3000);
- System.out.println("Asking
 thread to stop...");
- 
- /*由于线程处理阻塞状态，interrupt不产生任何作用*/
- //System.out.println( "Interrupting thread..." );
- //thread.interrupt();
- 
- thread.stop= true;
- thread.socket.close();
- Thread.sleep(3000);
- System.out.println("Stopping
 application...");
- 
- }
- }
运行结果
- 
Starting thread...
- 
Waiting for connection...
- 
Asking thread to stop...
- 
accept() failed or interrupted...
- 
Thread exiting under request...
- 
Stopping application...
参考文献
1.用interrupt()中断Java线程. http://hapinwater.iteye.com/blog/310558
2.Java Thread.interrupt 害人！ 中断JAVA线程. http://www.blogjava.net/jinfeng_wang/archive/2008/04/27/196477.html
