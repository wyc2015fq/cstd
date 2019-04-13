
# java线程的加入、等待和守护 - 李昆鹏的博客 - CSDN博客


2018年04月20日 16:29:14[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：44标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


---------------------------------------------java线程的加入、等待和守护-----------------------------------

**加入线程**
|void
|join|()
|等待该线程终止。
|
|void
|join|(long millis)
|等待该线程终止的时间最长为 millis 毫秒。
|
|void
|join|(long millis, int nanos)
|等待该线程终止的时间最长为 millis 毫秒 + nanos  纳秒。
|

**Join****线程会抢先拿到CPU****来执行线程，然后其他的线程再来执行。**
**public****class**JoinTest {
**public****static****void**main(String[]args) {
JoinThreadpt=**new**JoinThread();
JoinThreadpt1=**new**JoinThread();
JoinThreadpt2=**new**JoinThread();
pt2.start();
**try**{
//join线程会抢先拿到cpu来执行线程，然后其他线程再来执行
//通过join方法让线程2优先执行
pt2.join();
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
pt.start();
pt1.start();
}
}

---------------------------------------------------------
**等待线程**
|static void
|yield|()
|暂停当前正在执行的线程对象，并执行其他线程。
|
当前的线程从运行阶段回到就绪阶段，目的是把CPU资源让给其他的线程。

**public****class**YieldThread**extends**Thread{
@Override
**public****void**run() {
**for**(**int**i= 0;i<50;i++) {
System.**out**.println(**this**.getName() +"正在运行"+i);
//当前线程回到就绪状态，然后其他线程来执行
Thread.*yield*();
}
}
}
----------------------------------------------------------------

**守护线程**
|void
|setDaemon|(boolean on)
|将该线程标记为守护线程或用户线程。
|
守护线程会随着主线程的结束而结束。
**public****class**DaemonTest {
**public****static****void**main(String[]args) {
DaemonThreadpt=**new**DaemonThread();
DaemonThreadpt1=**new**DaemonThread();
//设置成守护线程
pt.setDaemon(**true**);
pt1.setDaemon(**true**);
pt.start();
pt1.start();
**for**(**int**i= 0;i<10;i++) {
System.**out**.println(Thread.*currentThread*().getName()+"主线程正在运行"+i);
}
}
}


