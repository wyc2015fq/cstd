
# java线程休眠方法详解 - 李昆鹏的博客 - CSDN博客


2018年04月13日 22:54:47[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：442


------------------------------------java线程休眠方法详解---------------------
**线程的休眠**
线程的休眠是很必要，在做服务器端的时候为了减少服务器的压力我们需要休眠。
如果休眠是在同步代码中执行，休眠不会让出锁对象。
**public****class**SaleTicket**implements**Runnable{
//因为是共享对象所以不用静态属性
**private****int**tickets= 10;
//创建一个同步锁
**private**Objectobj=**new**Object();
@Override
**public****void**run() {
**while**(**true**) {
//同步代码块
**synchronized**(**this**) {
**try**{
//测试线程在休眠的时候会不会让出同步锁
Thread.*sleep*(10);
System.**out**.println("-----------");
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
**if**(tickets> 0) {
System.**out**.println(Thread.*currentThread*().getName()+"正在买票"+tickets-- +"张票");
}**else**{
System.**out**.println("票已售完");
**break**;
}
}
}
}
}
--------------------------------------------------------
**public****class**SleepTest {
**public****static****void**main(String[]args) {
/*      SleepDemosd= new SleepDemo();
Thread t = new Thread(sd);
t.start();*/
SaleTicketst=**new**SaleTicket();
Threadt=**new**Thread(st,"窗口1");
Threadt1=**new**Thread(st,"窗口2");
t.start();
t1.start();
/*      -----------
窗口1正在买票10张票
-----------
窗口1正在买票9张票
-----------
窗口1正在买票8张票
-----------
窗口1正在买票7张票
-----------
窗口1正在买票6张票
-----------
窗口1正在买票5张票
-----------
窗口1正在买票4张票
-----------
窗口2正在买票3张票
-----------
窗口2正在买票2张票
-----------
窗口2正在买票1张票
-----------
票已售完
-----------
票已售完*/
}
}


