# java多线程之wait,notify的使用 - 零度的博客专栏 - CSDN博客
2016年04月14日 10:54:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：493标签：[java																[多线程																[wait																[notify](https://so.csdn.net/so/search/s.do?q=notify&t=blog)](https://so.csdn.net/so/search/s.do?q=wait&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java并发编程](https://blog.csdn.net/zmx729618/article/category/6181602)
                
Java是第一个内置对多线程支持的主流编程语言。在Java5之前，对多线程的支持主要是通过对块结构的同步实现的（synchronized配合wait，notify，notifyAll），Java5引入了java.util.concurrent包，提供了对多线程编程的更高层的支持。
在Java中，除了int等基本类型之外，一切皆为对象。synchronized关键字以及Object类中的wait，notify和notifyAll方法为我们编写多线程程序提供了原始的支持。
例如：
|`1`|`public``class``A {`|
|`2`|`    ``public``synchronized``void``fun() {`|
|`3`|`        ``...`|
|`4`|`    ``}`|
|`5`|`}`|
在调用方法fun之前，调用该方法的执行线程必须要先获得类A的实例（a）的对象锁。上面的方法fun在功能上等同于：
|`1`|`public``void``fun() {`|
|`2`|`    ``synchronized``(``this``) {`|
|`3`|`        ``...`|
|`4`|`    ``}`|
|`5`|`}`|
在执行fun之前，要获取a的对象锁，在方法fun返回之前，要释放a的对象锁。
通常可以使用synchronized和notify，notifyAll以及wait方法来实现线程之间的数据传递及控制。对于对象obj来说：
- obj.wait()：该方法的调用，使得调用该方法的执行线程（T1）放弃obj的对象锁并阻塞，直到别的线程调用了obj的notifyAll方法、或者别的线程调用了obj的notify方法且JVM选择唤醒（T1），被唤醒的线程（T1）依旧阻塞在wait方法中，与其它的线程一起争夺obj的对象锁，直到它再次获得了obj的对象锁之后，才能从wait方法中返回。（除了notify方法，wait还有带有时间参数的版本，在等待了超过所设时间之后，T1线程一样会被唤醒，进入到争夺obj对象锁的行列；另外中断可以直接跳出wait方法）
- obj.notify()：该方法的调用，会从所有正在等待obj对象锁的线程中，唤醒其中的一个（选择算法依赖于不同实现），被唤醒的线程此时加入到了obj对象锁的争夺之中，然而该notify方法的执行线程此时并未释放obj的对象锁，而是离开synchronized代码块时释放。因此在notify方法之后，synchronized代码块结束之前，所有其他被唤醒的，等待obj对象锁的线程依旧被阻塞。
- obj.notifyAll()：与notify的区别是，该方法会唤醒所有正在等待obj对象锁的线程。（不过同一时刻，也只有一个线程可以拥有obj的对象锁）
要注意的是，wai，notify以及notifyAll方法的调用必须在相应的synchronized代码块之中。例如：
|`1`|`synchronized``(obj) {`|
|`2`|`    ``while``(xxx) {`|
|`3`|`        ``obj.wait();`|
|`4`|`    ``}`|
|`5`|`    ``...`|
|`6`|`}`|
下面写个小例子，来说明wait方法返回之前要重新获得对象锁，以及notify方法执行之后并不立即释放对象锁。
|`01`|`public``class``MThread ``implements``Runnable {`|
|`02`|`    ``public``synchronized``void``assign() {`|
|`03`|`        ``this``.notifyAll();`|
|`04`|`        ``System.err.println(``"out of notifyAll"``);`|
|`05`|`        ``while``(``true``){}`|
|`06`|`    ``}`|
|`07`|`    `|
|`08`|`    ``public``synchronized``void``await() ``throws````InterruptedException {```|
|`09`|`        ``this``.wait(``5000``);`|
|`10`|`        ``System.err.println(``"out of wait"``);`|
|`11`|`    ``}`|
|`12`||
|`13`|`    ``@Override`|
|`14`|`    ``public``void``run() {`|
|`15`|`            ``try``{`|
|`16`|`                ``await();`|
|`17`|`            ``} ``catch``(InterruptedException e) {`|
|`18`|`            ``}`|
|`19`|`    ``}`|
|`20`||
|`21`|`}`|
|`22`||
|`23`||
|`24`|`public``class``Main {`|
|`25`|`    ``public``static``void``main(String[] args) {`|
|`26`|`        ``MThread mThread = ``new``MThread();`|
|`27`|`        ``Thread thread = ``new``Thread(mThread);`|
|`28`|`        ``thread.start();`|
|`29`|`        `|
|`30`|`        ``try``{`|
|`31`|`            ``Thread.sleep(``1000``);`|
|`32`|`        ``} ``catch``(InterruptedException e) {`|
|`33`|`            ``e.printStackTrace();`|
|`34`|`        ``}`|
|`35`|`        ``mThread.assign();`|
|`36`|`    ``}`|
|`37`|`}`|
主线程中，先创建子线程，并启动，主线程sleep一秒钟是为了让子线程中的this.wait(5000)得到执行，而wait(5000)又保证在五秒钟结束之前，主线程中的mThread.assign()方法得到调用，进而调用子线程的notifyAll方法，来唤醒wait方法。从执行结果中可以看到“out of
 notifyAll”的输出，却始终看不到“out of wait”，这说明notifyAll方法执行完成，而wait方法并未返回。一个合理的解释就是wait还在等待mThread的对象锁，而notifyAll执行完成后也并未立即释放mThread的对象锁，sun的官方文档的说明也是如此。
