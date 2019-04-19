# 深入浅出Android消息系统之一 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月22日 10:52:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1369
**深入浅出Android消息系统之一**
# 1.VC6.0 MFC消息机制
通过PreTranslateMessage处理消息；
**2.Activity内嵌的消息处理机制有：**
a.onKeyDown；
b.各式各样的lister,如onKeyDownListener；
**3.有哪些工作必须通过消息分发处理机制来完成呢**：
a.事件的定时触发；
b.异步的循环事件的处理；
c.高耗时的工作；
# 4.Handler处理哪个线程下的消息：
默认情况下，Handler接受的是当前线程下的消息循环实例
# 5.一个消息循环可以被多个对象分发与处理
一个消息循环可以被当前线程中的多个对象来分发，来处理（在UI线程中，系统已经有一个Activity来处理了，你可以再起若干个Handler来处理...）。
# 6. Android的CPU分配的最小单元：
线程
**7.Handler与Thread的关系**：
Handler一般是在某个线程里创建的，因而Handler和Thread就是相互绑定的，一一对应的；
# 8.Runnable与Thread之间的关系：
Runnable是一个接口，Thread是Runnable的子类。所以说，他俩都算一个线程；
# 9.Thread(HandlerThread)的作用是什么？
HandlerThread顾名思义就是可以处理消息循环的线程，他是一个拥有Looper的线程，可以处理消息循环。
# 10.Handler是在UI线程之内运行的，并没有新建线程：
在UI线程（主线程）中，有如下代码：
mHandler=new Handler();
mHandler.post(new Runnable(){
void run(){
//执行代码...
}
});
这个线程其实是在UI线程之内运行的，并没有新建线程。
**11.常见的新建线程的方法是**：
方法一：
Thread thread = new Thread();
thread.start();
方法二:
HandlerThread thread = new HandlerThread("string");
thread.start();
# 12.主线程就是UI线程；
# 13.UI线程何时创建：
当应用程序启动时，Android首先会开启一个主线程 (也就是UI线程)
# 14.主线程的作用：
主线程为管理界面中的UI控件，进行事件分发, 比如说, 你要是点击一个Button,Android会分发事件到Button上，来响应你的操作。
# 15.如果把耗时的工作，放在主线程中处理，有什么后果？
如果放在主线程中的话，界面会出现假死现象, 如果5秒钟还没有完成的话，会收到Android系统的一个错误提示"强制关闭".
# 16.有哪些是耗时的工作：
a.联网读取数据； 
b.读取本地的一个大文件；
c.硬件初始化；
# 17.如果遇到耗时的工作，如何处理？
需要把这些耗时的操作放在一个子线程中
# 18.更新UI只能在主线程中更新；
Notes：主线程就是UI线程；
# 19. Handler的作用：
Handler的作用：
1.接受子线程传过来的Message对象(里面包含数据)；
Notes：子线程用sedMessage()方法传递；
2.把这些消息放入主线程队列中，
3.配合主线程进行更新UI。；
# 20.Handler与Thread如何传递数据：
Handler与子线程可以通过Message对象来传递数据。
