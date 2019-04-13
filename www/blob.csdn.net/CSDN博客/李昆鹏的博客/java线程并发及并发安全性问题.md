
# java线程并发及并发安全性问题 - 李昆鹏的博客 - CSDN博客


2018年04月13日 22:52:22[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：52


-----------------------------------java线程并发及并发安全性问题------------------------------------
**并发**
互联网的项目中存在着大量的并发案例。如：卖火车票，电商网站。
范例：火车站有10张票，4个窗口同时卖票。
分析：四个窗口是4个线程同时在运行，10张票是4个线程的共享资源。
采用继承Thread的方式来实现。
![](https://img-blog.csdn.net/2018041322481917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**public****class**SaleTicket**extends**Thread {

**public**SaleTicket() {
}
**public**SaleTicket(Stringname) {
**super**(name);
}
//共享资源要为情态属性才能被多个线程所共享
**private****static****int***tickets*= 10;
@Override
**public****void**run() {
**while**(**true**) {
**try**{
Thread.*sleep*(10);
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
**if**(*tickets*> 0 ) {
System.**out**.println(**this**.getName() +"正在买票"+*tickets*-- +"张票");
}**else**{
System.**out**.println("票已售完");
**break**;
}
}
}
}
-----------------------------------------------
创建多个线程卖票
**public****class**TicketTest {
**public****static****void**main(String[]args) {
SaleTicketst1=**new**SaleTicket("窗口1");
SaleTicketst2=**new**SaleTicket("窗口2");
SaleTicketst3=**new**SaleTicket("窗口3");
SaleTicketst4=**new**SaleTicket("窗口4");
st1.start();
st2.start();
st3.start();
st4.start();
//我们会发现多个线程在卖同一张票这就是并发安全问题
/*窗口2正在买票9张票
窗口4正在买票8张票
窗口1正在买票7张票
窗口3正在买票10张票
窗口4正在买票6张票
窗口2正在买票5张票
窗口1正在买票6张票
窗口3正在买票5张票
窗口4正在买票4张票
窗口2正在买票4张票
窗口1正在买票4张票
窗口3正在买票4张票
窗口2正在买票3张票
窗口1正在买票2张票
窗口3正在买票2张票
窗口4正在买票3张票
窗口4正在买票1张票
窗口3正在买票1张票
窗口2正在买票1张票
窗口1正在买票0张票
票已售完
票已售完
票已售完
票已售完*/

}
}

-------------------------------------------------------------
**解决并发安全问题**

**解决方案：**
![](https://img-blog.csdn.net/2018041322495228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

针对线程的安全性问题，我们需要使用同步锁（就是要加锁，共享资源只能一个人同时访问）。
语法：
Synchronized（锁对象）{
//操作共享资源的代码
}
同步代码加在什么地方？
1.    代码被多个线程访问
2.    代码中有共享的数据
3.    共享数据被多条语句操作

**Synchronized同步代码块的锁对象可以是任意对象（线程的实现方式是使用继承于Thread），这个对象必须是线程类共享的（静态的）.**
**Synchronized是可以加在方法上，如果是静态方法synchronized的锁对象时类的类对象。如果不是静态的方法，synchronized如果加在对象方法上，那么它的锁是this。**

**实例：**
**静态方法同步锁**
**public****class**SaleTicket**extends**Thread {
**public**SaleTicket() {
}
**public**SaleTicket(Stringname) {
**super**(name);
}
//共享资源要为情态属性才能被多个线程所共享
**private****static****int***tickets*= 100;
//同步锁对象(同步钥匙)
//private static Objectobj= new Object();
**private****static**A*a*=**new**A();
@Override
**public****void**run() {
**while**(**true**) {
//同步代码块
**synchronized**(*a*) {
**try**{
Thread.*sleep*(10);
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
**if**(*tickets*> 0) {
System.**out**.println(**this**.getName() +"正在买票"+*tickets*-- +"张票");
}**else**{
System.**out**.println("票已售完");
**break**;
}
}
}
}
}
/**
*创建一个类用作同步锁
*/
**class**A{
}
-------------------------------------------------------
**非静态方法同步锁：**
**public****class**SaleTicket**implements**Runnable{

//因为是共享对象所以不用静态属性
**private****int**tickets= 100;
//创建一个同步锁
**private**Objectobj=**new**Object();
@Override
**public****void**run() {
**while**(**true**) {
//同步代码块
**synchronized**(**this**) {
**try**{
Thread.*sleep*(10);
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
/**
*非静态方法的synchornized的锁是当前的对象(this)
*/
**public****synchronized****void**saleTicket() {
**while**(**true**) {
//同步代码块
**try**{
Thread.*sleep*(10);
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


