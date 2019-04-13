
# java线程死锁 - 李昆鹏的博客 - CSDN博客


2018年04月20日 16:31:27[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：30


---------------------------------java线程死锁----------------------------
**在做线程开发的时候要避免出现死锁**

**示例代码**
**public****class**DieLockDemo**implements**Runnable{
**private****boolean**flag;
**public**DieLockDemo(**boolean**flag) {
**super**();
**this**.flag=flag;
}
@Override
**public****void**run() {
**if**(flag) {
**synchronized**(Lock.*lock1*) {
System.**out**.println("线程1执行拿到第一把锁");
**synchronized**(Lock.*lock2*) {
System.**out**.println("线程1执行拿到第二把锁");
}
}
}**else**{
**synchronized**(Lock.*lock2*) {
System.**out**.println("线程2拿到第二把锁");
**synchronized**(Lock.*lock1*) {
System.**out**.println("线程2拿到第一把锁");
}
}
}
}
}
----------------------------------------------------------------

**public****class**DieLockTest {
**public****static****void**main(String[]args) {
DieLockDemodl1=**new**DieLockDemo(**true**);
DieLockDemodl2=**new**DieLockDemo(**false**);
Threadt1=**new**Thread(dl1);
Threadt2=**new**Thread(dl2);
t1.start();
t2.start();
}
}


