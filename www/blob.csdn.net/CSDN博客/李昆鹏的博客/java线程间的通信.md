
# java线程间的通信 - 李昆鹏的博客 - CSDN博客


2018年04月20日 16:23:02[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：101标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


-----------------------------------java线程间的通信------------------------------------

生产者和消费者
![](https://img-blog.csdn.net/20180420162024256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

生产者生产水果，如果水果没有被买走就不生产处于等待状态，如果水果被消费者买走的时候，消费者就会通知生产者告诉他我们已经把水果买走了请生产。
消费者同理，如果如果水果已经生产出来那么就买走，买走之后再通知生产者水果已经没有了请生产。
**注意：**
**1.****线程间的通信共享数据一定要有同步代码块synchronized**
**2.****一定要有wait****和notify，二者一定是成对出现的。**
**3.****生产者和消费者的线程实现一定是在while(true)****死循环里面**。

**实例：**
**水果类**
**public****class**Fruit {
**private**Stringname;
**private****boolean**isExist;
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public****boolean**isExist() {
**return**isExist;
}
**public****void**setExist(**boolean**isExist) {
**this**.isExist=isExist;
}
}

**生产者类**
**public****class**ProductFruit**implements**Runnable {
**private**Fruitfruit;
**public**ProductFruit(Fruitfruit) {
**super**();
**this**.fruit=fruit;
}
@Override
**public****void**run() {
**while**(**true**) {
//有共享的数据，多个线程操作共享的数据，必须使用锁
**synchronized**(fruit) {
//如果水果已经存在那么生产者就不生产，等待着消费者买走水果再生产
**if**(fruit.isExist()) {
**try**{
//当前生产的水果被挂起变成阻塞状态
fruit.wait();
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
}
**try**{
Thread.*sleep*(100);
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
System.**out**.println(fruit.getName() +"水果被生产出来");
//把水果的状态变成存在
fruit.setExist(**true**);
//唤醒等待买水果的线程
fruit.notify();
}
}
}
}
**购买者类**
**public****class**BuyFruit**implements**Runnable{
**private**Fruitfruit;
**public**BuyFruit(Fruitfruit) {
**super**();
**this**.fruit=fruit;
}
@Override
**public****void**run() {
**while**(**true**) {
//有共享的数据，多个线程操作共享的数据，必须使用锁
**synchronized**(fruit) {
**if**(!fruit.isExist()) {
**try**{
fruit.wait();
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
}
**try**{
Thread.*sleep*(100);
}**catch**(InterruptedExceptione) {
e.printStackTrace();
}
System.**out**.println(fruit.getName() +"水果被买走");
//把水果状态设为false
fruit.setExist(**false**);
//唤醒生产者生产水果
fruit.notify();
}
}
}
}
**客服端类**
**public****class**Client {
**public****static****void**main(String[]args) {
//创建水果对象
Fruitf=**new**Fruit();
f.setName("苹果");
f.setExist(**false**);
//创建生产者
ProductFruitpf=**new**ProductFruit(f);
//创建消费者
BuyFruitbf=**new**BuyFruit(f);
//创建线程
Threadt1=**new**Thread(pf);
Threadt2=**new**Thread(bf);
//启动线程
t1.start();
t2.start();
/*苹果水果被生产出来
苹果水果被买走
苹果水果被生产出来
苹果水果被买走
苹果水果被生产出来
苹果水果被买走
苹果水果被生产出来*/
}
}


