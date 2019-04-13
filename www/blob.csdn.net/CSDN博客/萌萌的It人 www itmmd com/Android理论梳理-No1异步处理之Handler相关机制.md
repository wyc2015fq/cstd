
# Android理论梳理-No1异步处理之Handler相关机制 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月09日 16:58:01[Jlins](https://me.csdn.net/dyllove98)阅读数：1828


1 Handler的基本概念：
什么是handler？
Handler是安卓系统的一种回调机制。
handler的作用？
他的作用是用于不同线程之间的通讯。一是可以用于子线程与UI线程的数据通信，因为只有UI线程可以更新UI,子线程如果想要更新界面就需要用Handler通过发送Message来更新UI，而UI线程在handlerMessage方法中处理传过来的Message数据。二是可以用于Handler绑定的线程延迟发送消息。
2 那么为什么只有UI线程才能更新UI呢？
主要是为了解决多线程并发的问题。如果所有的线程都能更新UI，为了解决并发问题，线程会对UI对象进行加锁，那可能出现死锁现象导致界面卡住。另外只有UI线程进行更新也让设计和编程更简单。
3 Handler、Looper和MessageQueue之间的关系？
Handler负责向绑定的Looper发送消息。而MessageQueue是Looper内部的一个消息队列容器，Looper用MessageQueue消息队列来装载这些消息，并通过一个死循环方法Looper.loop从消息队列中取出消息并通过dispatchMessage方法把取出的消息发送给handlerMessage方法来进行处理。
4 能在子线程直接更新UI吗？
在网上能看到很多文章说android里子线程不能刷新UI。其实线程能否刷新UI的关键在于ViewRoot是否属于该线程。而实际上子线程在特定情况下是可以刷新UI的。ViewRoot实例要通过ViewRootImpl（）方法来创建，在Activity.onResume前，ViewRoot实例没有建立，所以没有ViewRoot.checkThread检查。所以在onCreate方法的早期是可以在子线程更新UI的。
如下面例子：
[?](http://www.cnblogs.com/liuying3013/p/4281734.html#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|public|class|MainActivity|extends|Activity {
|TextView textView1;
|@Override
|protected|void|onCreate(Bundle savedInstanceState) {
|super|.onCreate(savedInstanceState);
|setContentView(R.layout.activity_main);
|textView1 = (TextView) findViewById(R.id.textView1);
|new|Thread(|new|Runnable() {
|public|void|run() {
|textView1.setText(|"子线程更新UI"|);
|}
|}).start();
|}
|}
|

参考资料：

## android学习笔记（60）使用Bundle在Activity之间文换教接
## android学习笔记（59） android开发启动和关闭activity

## [android学习笔记（58）Activity学习过程](http://www.itmmd.com/201502/597.html)
## 更多
## [android开发](http://www.itmmd.com)
## 教程




