# java NIO的多路复用及reactor模式 - z69183787的专栏 - CSDN博客
2014年04月14日 11:14:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3151
java nio从1.4版本就出现了，而且依它优异的性能赢得了广大java开发爱好者的信赖。我很纳闷，为啥我到现在才接触，难道我不是爱好者，难道nio不优秀。经过长达半分钟的思考，我意识到：时候未到。以前总是写那些老掉牙的web程序，唉，好不容易翻身啦，现在心里好受多了。因为真不想自己到了30岁，还在说，我会ssh，会ssi，精通javascript，精通数据库，精通。。。人生苦短，要开拓点不是吗？列为兄弟姐妹，没看到外国的和尚已经开始鼓吹“云里雾里”的？没看到百度进入“框”啦，没看到oracle的“格”啦。人家的经，随他念，但是我们的确有好多路要走哦（牢骚怎么这么多呀）。
现在终于到了我了解nio的时候了，突然发现有很多美妙程序的源码，不得不爽一把（有邪念者，该打住啦，像我这样）。
以下描述，为了说明问题，就提提历史（类似的东西，网上一搜一大把，但是希望你能在这里止步，知道到底是怎么回事。如果还是不清楚，咱就站内沟通！）。
在我（刚）看nio的这段时间里，主要接触了几个东西，就是关于server和client。java之前的io完全可以胜任，但是效率不高，为何效率不高呢？
===============history==start===============
//TODO:finish the old style of server and socket data transion.
ServerSocket socket = new ServerSocket(80); 
        while (true) { 
            final Socket connection = socket.accept(); 
            handleRequest(connection); 
        }
===============history==end in the future================
在上面的代码片段中，我们只能是一个request一个的进行处理。这使得所有的请求都阻塞了。如果我们再改变一下，将handleRequest方法封装到线程中处理：
if(connection = null){
new Thread(new Runnable(){
  public void run(){
      handleRequest(connection);
  }
  public void handleRequest(Socket conn){
   //do actions
  }
}).start();
}
服务器端的资源是有限的，我们这里仅仅是从线程角度来扩展，但是这种处理依然是阻塞的处理方式。首先，仅仅是建立连接（socket），就占用了服务器的线程资源。如果客户端还没有发出相应的数据请求，那么服务器就要一直等待他们的数据流过来，然后再进行读取，如此往复。。。一直都blocking。服务器处在一个高负荷状态中。
NIO出来之后，进入改革开放时期，有了这么几个角色，ServerSocketChannel,SelectionKey,Selector.
这几个角色都是做什么用的呢？需要了解一下reactor模式（反应堆模式）。
作为服务端，如果什么操作都要依赖于客户端，很多操作都阻塞，如上面的代码片段所示。reactor模式提供了一种很好的事件处理机制，以分离事件处理对象与事件之间的耦合。如下图示（详细请看参考资料（1））：
说明：
Acceptor就是我们Server端的主要任务消化者；
Initiation Dispatcher是事件（Event）的分发者；
HTTP Handler是具体操作人。
首先，在Initiation Dispatcher中注册一个acceptor（1：register Acceptor），这个Acceptor是跟事件绑定的，它仅仅关心某种事件（event）。
Initiation Dispatcher不断地循环获取请求过来的事件（2：handle events），如果发现有对应Acceptor关心的事件（3：connect），通知Acceptor有事件发生（4：new connection）。
Acceptor针对此事件进行处理，创建了新的HTTP Handler（（5：create handler））
一轮事件获取和分发完成。
那么handler是不是就抓住这个connection不放，然后一直苦苦等待数据流的到来呢？
不是的，它也是将自己和自己关心的事件注册到Initiation Dispatcher。如果Initiation Dispatcher在handle Events时发现了它关心的事件，那么就会交由它去进行相应处理。如下图示，在连接完成后，browser提交的get请求，handler的处理过程：
这里面尤其要注意到，2：read ready，之后才read request，也就是说，handler在dispatcher中注册了自己关心的事件（READ）,然后在写的时候，也是类似情况。
以上的过程就实现了非阻塞的处理方式，客户端的连接可以非阻塞（这是意思是，acceptor不必一直苦苦等候），然后对客户端过来的request内容，也是非阻塞（这里是不必苦苦等待其数据的到来），都是不必一直眼巴巴的看着那个连接，那些数据，而是如果有我关心的事件了，我再进行处理，期间完全相信Initiation Dispatcher就行了。
这里有一点要注意，就是现在的reactor模式都是建立在操作系统的基础上实现的，不同的操作系统有不同的实现方式。而且都不支持多线程（针对Initiation Dispatcher而言）。
有了上面的理解之后，来给NIO中的对象跟reactor的对象对个象。
Acceptor：ServerSocketChannel；
Initiation Dispatcher：Selector；
HTTP Handler：针对SocketChannel进行实际处理的个性化对象；
Events：在SelectionKey中：
|`static int`|`**OP_ACCEPT**`          Operation-set bit for socket-accept operations.|
|----|----|
|`static int`|`**OP_CONNECT**`          Operation-set bit for socket-connect operations.|
|`static int`|`**OP_READ**`          Operation-set bit for read operations.|
|`static int`|`**OP_WRITE**`          Operation-set bit for write operations.|
经过上面的描述，我们就可以写出基于NIO的非阻塞Server啦。具体的代码示例大家可以看参考资料（2）。
