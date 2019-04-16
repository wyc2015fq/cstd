# Java面试题总结 - 早点变成刘强东就好 - CSDN博客





2018年07月06日 21:40:06[caymant](https://me.csdn.net/cayman_2015)阅读数：34
个人分类：[java基础](https://blog.csdn.net/cayman_2015/article/category/6045212)









# Java面试总结

对自己找工作以来遇到的Java面试题做一个总结。

#### 1  举例说明几种常见的异常，并说明分析某些异常时toString(),getMessage(),printStackTrace()的异同点。

###### java中异常分为运行时异常和非运行时异常两种。

常见异常如下： 

    （1）NullPointerException: 空指针异常，属于运行时异常。 

    （2）ClassCastException: 类型强制转换异常。 

    （3）ArrayIndexOutOfBoundsException: 数组下标越界异常。 

    （4）NoSuchElementException:没有这个元素异常。 

异同点： 

toString()  输出异常的种类和异常内容； 

getMessage() 只输出异常内容； 

printStackTrace() 输出异常的种类，内容，以及发生异常的地方；
#### 2  简述HTTP协议，请求都有哪些？

HTTP协议是超文本传输协议，是一个应用层协议，由请求和响应构成。标准的客户端服务器模型。它可以使浏览器更加高效，使网络传输减少。它不仅保证计算机正确快速地传输超文本文档，还确定传输文档中的哪一部分，以及哪部分内容首先显示(如文本先于图形)等。 

HTTP协议通常承载于TCP协议之上，有时也承载于TLS或SSL协议层之上，这个时候，就成了我们常说的HTTPS。如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180707112600511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

默认HTTP的端口号为80，HTTPS的端口号为443。 

HTTP协议永远都是客户端发起请求，服务器回送响应。 

HTTP协议是一个无状态的协议，同一个客户端的这次请求和上次请求是没有对应关系。 

HTTP1.0定义了三种请求方法： GET, POST 和 HEAD方法。 

HTTP1.1新增了五种请求方法：OPTIONS, PUT, DELETE, TRACE 和 CONNECT 方法。
#### 3TCP/IP协议三次握手四次挥手的粗略解释

TCP协议提供一种面向连接的、可靠的字节流服务。面向连接意味着两个使用TCP的应用（通常是一个客户和一个服务器）在彼此交换数据之前必须先建立一个TCP连接。 

3次握手过程详解： 

所谓三次握手（Three-Way Handshake）即建立TCP连接，就是指建立一个TCP连接时，需要客户端和服务端总共发送3个包以确认连接的建立。在socket编程中，这一过程由客户端执行connect来触发。 

（1）第一次握手： 

Client将标志位SYN置为1，随机产生一个值seq=J，并将该数据包发送给Server，Client进入SYN_SENT状态，等待Server确认。 

（2）第二次握手： 

Server收到数据包后由标志位SYN=1知道Client请求建立连接，Server将标志位SYN和ACK都置为1，ack=J+1，随机产生一个值seq=K，并将该数据包发送给Client以确认连接请求，Server进入SYN_RCVD状态。 

（3）第三次握手： 

Client收到确认后，检查ack是否为J+1，ACK是否为1，如果正确则将标志位ACK置为1，ack=K+1，并将该数据包发送给Server，Server检查ack是否为K+1，ACK是否为1，如果正确则连接建立成功，Client和Server进入ESTABLISHED状态，完成三次握手，随后Client与Server之间可以开始传输数据了。
SYN攻击： 

在三次握手过程中，Server发送SYN-ACK之后，收到Client的ACK之前的TCP连接称为半连接（half-open connect），此时Server处于SYN_RCVD状态，当收到ACK后，Server转入ESTABLISHED状态。SYN攻击就是Client在短时间内伪造大量不存在的IP地址，并向Server不断地发送SYN包，Server回复确认包，并等待Client的确认，由于源地址是不存在的，因此，Server需要不断重发直至超时，这些伪造的SYN包将产时间占用未连接队列，导致正常的SYN请求因为队列满而被丢弃，从而引起网络堵塞甚至系统瘫痪。SYN攻击时一种典型的DDOS攻击，检测SYN攻击的方式非常简单，即当Server上有大量半连接状态且源IP地址是随机的，则可以断定遭到SYN攻击了，使用如下命令可以让之现行：

6、4次挥手过程详解 

三次握手耳熟能详，四次挥手估计就少有人知道了。所谓四次挥手（Four-Way Wavehand）即终止TCP连接，就是指断开一个TCP连接时，需要客户端和服务端总共发送4个包以确认连接的断开。在socket编程中，这一过程由客户端或服务端任一方执行close来触发。

由于TCP连接时全双工的，因此，每个方向都必须要单独进行关闭，这一原则是当一方完成数据发送任务后，发送一个FIN来终止这一方向的连接，收到一个FIN只是意味着这一方向上没有数据流动了，即不会再收到数据了，但是在这个TCP连接上仍然能够发送数据，直到这一方向也发送了FIN。首先进行关闭的一方将执行主动关闭，而另一方则执行被动关闭。 

第一次挥手： 

Client发送一个FIN，用来关闭Client到Server的数据传送，Client进入FIN_WAIT_1状态。 

第二次挥手： 

Server收到FIN后，发送一个ACK给Client，确认序号为收到序号+1（与SYN相同，一个FIN占用一个序号），Server进入CLOSE_WAIT状态。 

第三次挥手： 

Server发送一个FIN，用来关闭Server到Client的数据传送，Server进入LAST_ACK状态。 

第四次挥手： 

Client收到FIN后，Client进入TIME_WAIT状态，接着发送一个ACK给Server，确认序号为收到序号+1，Server进入CLOSED状态，完成四次挥手。 

(2) 为什么建立连接是三次握手，而关闭连接却是四次挥手呢？ 

这是因为服务端在LISTEN状态下，收到建立连接请求的SYN报文后，把ACK和SYN放在一个报文里发送给客户端。而关闭连接时，当收到对方的FIN报文时，仅仅表示对方不再发送数据了但是还能接收数据，己方也未必全部数据都发送给对方了，所以己方可以立即close，也可以发送一些数据给对方后，再发送FIN报文给对方来表示同意现在关闭连接，因此，己方ACK和FIN一般都会分开发送。
#### 4 redirect和forward跳转有什么区别?

Forward和Redirect代表了两种请求转发方式：直接转发和间接转发。对应到代码里，分别是RequestDispatcher类的forward()方法和HttpServletRequest类的sendRedirect()方法。 

　　对于间接方式，服务器端在响应第一次请求的时候，让浏览器再向另外一个URL发出请求，从而达到转发的目的。它本质上是两次HTTP请求，对应两个request对象。 

　　对于直接方式，客户端浏览器只发出一次请求，Servlet把请求转发给Servlet、HTML、JSP或其它信息资源，由第2个信息资源响应该请求，两个信息资源共享同一个request对象。
举个通俗的例子： 

　　直接转发就相当于：“A找B借钱，B说没有，B去找C借，借到借不到都会把消息传递给A”； 

　　间接转发就相当于：”A找B借钱，B说没有，让A去找C借”。
**一**：间接转发方式，有时也叫重定向，它一般用于避免用户的非正常访问。例如：用户在没有登录的情况下访问后台资源，Servlet可以将该HTTP请求重定向到登录页面，让用户登录以后再访问。在Servlet中，通过调用response对象的SendRedirect()方法，告诉浏览器重定向访问指定的URL。

```
//Servlet中处理get请求的方法
public void doGet(HttpServletRequest request,HttpServletResponse response){
//请求重定向到另外的资源
    response.sendRedirect("资源的URL");
}
```

![这里写图片描述](https://img-blog.csdn.net/20180708104356951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图所示的间接转发请求的过程如下：
1.浏览器向Servlet1发出访问请求； 

2.Servlet1调用sendRedirect()方法，将浏览器重定向到Servlet2； 

3.浏览器向servlet2发出请求； 

4.最终由Servlet2做出响应。 
**二**： 直接转发方式用的更多一些，一般说的请求转发指的就是直接转发方式。Web应用程序大多会有一个控制器。由控制器来控制请求应该转发给那个信息资源。然后由这些信息资源处理请求，处理完以后还可能转发给另外的信息资源来返回给用户，这个过程就是经典的MVC模式。 

javax.serlvet.RequestDispatcher接口是请求转发器必须实现的接口，由Web容器为Servlet提供实现该接口的对象，通过调用该接口的forward()方法到达请求转发的目的，示例代码如下：
```
//Servlet里处理get请求的方法
 public void doGet(HttpServletRequest request , HttpServletResponse response){
     //获取请求转发器对象，该转发器的指向通过getRequestDisPatcher()的参数设置
   RequestDispatcher requestDispatcher =request.getRequestDispatcher("资源的URL");
    //调用forward()方法，转发请求      
   requestDispatcher.forward(request,response);    
}
```

![这里写图片描述](https://img-blog.csdn.net/20180708105158381?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NheW1hbl8yMDE1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图所示的直接转发请求的过程如下： 

1.浏览器向Servlet1发出访问请求； 

2.Servlet1调用forward()方法，在服务器端将请求转发给Servlet2； 

3.最终由Servlet2做出响应。
#### 5.session和cookie有什么区别？

1、Cookie 在客户端（浏览器），Session 在服务器端。 

2、Cookie的安全性一般，他人可通过分析存放在本地的Cookie并进行Cookie欺骗。在安全性第一的前提下，选择Session更优。重要交互信息比如权限等就要放在Session中，一般的信息记录放Cookie就好了。 

3、单个Cookie保存的数据不能超过4K，很多浏览器都限制一个站点最多保存20个Cookie。 

4、Session 可以放在 文件、数据库或内存中，比如在使用Node时将Session保存在redis中。由于一定时间内它是保存在服务器上的，当访问增多时，会较大地占用服务器的性能。考虑到减轻服务器性能方面，应当适时使用Cookie。 

5、Session 的运行依赖Session ID，而 Session ID 是存在 Cookie 中的，也就是说，如果浏览器禁用了 Cookie，Session 也会失效（但是可以通过其它方式实现，比如在 url 中传递 Session ID）。 

6、用户验证这种场合一般会用 Session。因此，维持一个会话的核心就是客户端的唯一标识，即Session ID。
#### 6.简述进程和线程的关系，线程创建的方式有哪些？为什么wait(),notify(),notifyall()必须在同步方法或者同步快中被调用。

1.一个程序至少有一个进程,一个进程至少有一个线程. 进程是系统进行资源分配和调度的一个独立单位. 线程是进程的一个实体,是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位.线程自己基本上不拥有系统资源,只拥有一点在运行中必不可少的资源(如程序计数器,一组寄存器和栈),但是它可与同属一个进程的其他的线程共享进程所拥有的全部资源. 一个线程可以创建和撤销另一个线程;同一个进程中的多个线程之间可以并发执行. 

2.Java使用Thread类代表线程，所有的线程对象都必须是Thread类或其子类的实例。Java创建线程的四种方式： 

1）继承Thread类创建线程 

2）实现Runnable接口创建线程 

3）使用Callable和Future创建线程 

4）使用线程池例如用Executor框架 

3.wait()与sleep()的区别 

sleep()方法是Thread类的静态方法，不涉及到线程间同步概念，仅仅为了让一个线程自身获得一段沉睡时间。sleep可以在任何地方使用。 

wait()方法是object类的方法，解决的问题是线程间的同步，该过程包含了同步锁的获取和释放，调用wait方法将会将调用者的线程挂起，直到其他线程调用同一个对象的notify()方法才会重新激活调用者。 

注意:线程调用notify()之后，只有该线程完全从 synchronized代码里面执行完毕后，monitor才会被释放，被唤醒线程才可以真正得到执行权。
今天同学写个手机游戏，用蓝牙传输数据的时候丢包，问我解决方案，我提出的方案是：用多线程发送数据并要求对方回送ack号，如果在一定时间内没收到就要重发，如果收到了就要自身wait，那么这要用到多线了，开始写了几个老是报错，就在网上找了找这方面的资料，终于解决了，呵呵！下面我把这篇写的比较全面的文章转载过来，做个笔记，希望能帮助更多的用多线程出现问题的朋友们。

wait与notify是java同步机制中重要的组成部分。结合与synchronized关键字使用，可以建立很多优秀的同步模型。 

      synchronized(this){}等价与public synchronized void method(){…..} 

      同步分为类级别和对象级别，分别对应着类锁和对象锁。类锁是每个类只有一个，如果static的方法被synchronized关键字修饰，则在这个方法被执行前必须获得类锁；对象锁类同。 

      首先，调用一个Object的wait与notify/notifyAll的时候，必须保证调用代码对该Object是同步的，也就是说必须在作用等同于synchronized(obj){……}的内部才能够去调用obj的wait与notify/notifyAll三个方法，否则就会报错： 

      java.lang.IllegalMonitorStateException: current thread not owner 

      在调用wait的时候，线程自动释放其占有的对象锁，同时不会去申请对象锁。当线程被唤醒的时候，它才再次获得了去获得对象锁的权利。 

      所以，notify与notifyAll没有太多的区别，只是notify仅唤醒一个线程并允许它去获得锁，notifyAll是唤醒所有等待这个对象的线程并允许它们去获得对象锁，只要是在synchronied块中的代码，没有对象锁是寸步难行的。其实唤醒一个线程就是重新允许这个线程去获得对象锁并向下运行。 

       顺便说一下notifyall，虽然是对每个wait的对象都调用一次notify，但是这个还是有顺序的，每个对象都保存这一个等待对象链，调用的顺序就是这个链的顺序。其实启动等待对象链中各个线程的也是一个线程，在具体应用的时候，需要注意一下。
```
class ThreadA{
    public static void main(String[] args)
    {
      ThreadB b=new ThreadB();
      b.start();
      System.out.println("b is start....");
      synchronized(b)//括号里的b是什么意思,起什么作用?
      {
        try
        {
System.out.println("Waiting for b to complete...");
b.wait();//这一句是什么意思，究竟让谁wait?
          System.out.println("Completed.Now back to main thread");
        }catch (InterruptedException e){}
      }
      System.out.println("Total is :"+b.total);
     }
}
class ThreadB extends Thread{
    int total;
    public void run()
    {
      synchronized(this)
      {
        System.out.println("ThreadB is running..");
        for (int i=0;i<100;i++ )
        {
          total +=i;
          System.out.println("total is "+total);
        }
        notify();
      }
    }
}
```

要分析这个程序,首先要理解notify()和wait(),为什么在前几天纪录线程的时候没有纪录这两个方法呢,因为这两个方法本来就不属于Thread类,而是属于最底层的object基础类的,也就是说不光是Thread，每个对象都有notify和wait的功能，为什么？因为他们是用来操纵锁的,而每个对象都有锁,锁是每个对象的基础,既然锁是基础的,那么操纵锁的方法当然也是最基础了.

再往下看之前呢,首先最好复习一下Think in Java的14.3.1中第3部分内容：等待和通知,也就是wait()和notify了.

按照Think in Java中的解释:”wait()允许我们将线程置入“睡眠”状态，同时又“积极”地等待条件发生改变.而且只有在一个notify()或notifyAll()发生变化的时候，线程才会被唤醒，并检查条件是否有变.”

我们来解释一下这句话. 

    “wait()允许我们将线程置入“睡眠”状态”,也就是说,wait也是让当前线程阻塞的,这一点和sleep或者suspend是相同的.那和sleep,suspend有什么区别呢? 

     区别在于”(wait)同时又“积极”地等待条件发生改变”,这一点很关键,sleep和suspend无法做到.因为我们有时候需要通过同步（synchronized）的帮助来防止线程之间的冲突，而一旦使用同步,就要锁定对象，也就是获取对象锁,其它要使用该对象锁的线程都只能排队等着,等到同步方法或者同步块里的程序全部运行完才有机会.在同步方法和同步块中,无论sleep()还是suspend()都不可能自己被调用的时候解除锁定,他们都霸占着正在使用的对象锁不放. 

     而wait却可以,它可以让同步方法或者同步块暂时放弃对象锁,而将它暂时让给其它需要对象锁的人(这里应该是程序块,或线程)用,这意味着可在执行wait()期间调用线程对象中的其他同步方法!在其它情况下(sleep啊,suspend啊),这是不可能的. 

    但是注意我前面说的,只是暂时放弃对象锁,暂时给其它线程使用,我wait所在的线程还是要把这个对象锁收回来的呀.wait什么?就是wait别人用完了还给我啊！ 

     好,那怎么把对象锁收回来呢? 

     第一种方法,限定借出去的时间.在wait()中设置参数,比如wait(1000),以毫秒为单位,就表明我只借出去1秒中,一秒钟之后,我自动收回. 

     第二种方法,让借出去的人通知我,他用完了,要还给我了.这时,我马上就收回来.哎,假如我设了1小时之后收回,别人只用了半小时就完了,那怎么办呢?靠!当然用完了就收回了,还管我设的是多长时间啊. 

     那么别人怎么通知我呢?相信大家都可以想到了,notify(),这就是最后一句话”而且只有在一个notify()或notifyAll()发生变化的时候，线程才会被唤醒”的意思了. 

     因此,我们可将一个wait()和notify()置入任何同步方法或同步块内部，无论在那个类里是否准备进行涉及线程的处理。而且实际上,我们也只能在同步方法或者同步块里面调用wait()和notify(). 

     这个时候我们来解释上面的程序,简直是易如反掌了. 

     synchronized(b){…}；的意思是定义一个同步块,使用b作为资源锁。b.wait();的意思是临时释放锁，并阻塞当前线程,好让其他使用同一把锁的线程有机会执行,在这里要用同一把锁的就是b线程本身.这个线程在执行到一定地方后用notify()通知wait的线程,锁已经用完,待notify()所在的同步块运行完之后,wait所在的线程就可以继续执行。
#### 7.redis支持的数据类型？redis使用场景，线程是安全的吗？

1.Redis支持五种数据类型：string（字符串），hash（哈希），list（列表），set（集合）及zset(sorted set：有序集合)。 

2.一：缓存——热数据 

热点数据（经常会被查询，但是不经常被修改或者删除的数据)。 

二：计数器 

诸如统计点击数等应用。由于单线程，可以避免并发问题，保证不会出错，而且100%毫秒级性能！爽。 

命令：INCRBY当然爽完了，别忘记持久化，毕竟是redis只是存了内存！ 

三：队列 

四：位操作（大数据处理） 

五：分布式锁与单线程机制 

六：最新列表 

七：排行榜 

3.redis 是线程安全,Redis是一个开源，先进的key-value存储，并用于构建高性能，可扩展的Web应用程序的完美解决方案，是线程安全的。Redis三个主要特点：①Redis数据库完全在内存中，使用磁盘仅用于持久性。②相比许多键值数据存储，Redis拥有一套较为丰富的数据类型。③Redis可以将数据复制到任意数量的从服务器。
















