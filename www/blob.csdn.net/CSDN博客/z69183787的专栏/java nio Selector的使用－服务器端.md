# java nio Selector的使用－服务器端 - z69183787的专栏 - CSDN博客
2014年04月14日 11:21:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1410
      前些时候花了一些时间在研究java.nio的api使用机制,看了好久,也觉得不习惯它的使用方式和用法.毕竟自己对C语言了解太少,也不太了解C语言在网络编程上的用法。对这种底层下的编程太不习惯，还是应该好好了解下底层的东西，要不然就光会使用别人的东西，如果是自己写一个，就写不出来了。
　　从java1.4以来,java nio就出现在java的api中，在日常的使用当中，基本上都是围绕着java.io中的几个inputStream(outputStream)和reader(writer)在转，要想编写一些其他形式的调用，还真不会。我也看了下最新的springframework中的FileCopyUtils中的代码，也是将各种操作集合给java.io来做。好像java.nio用得不是很多。看了下java.nio的描述信息，感觉这是用在网络编程上的。比如文件下载服务，通信服务等地方。自己暂时还用不上网络上的编程，不过等到用的时候还去学，就太晚了。
　　看了下关于Selector的使用方法，官方的说法是一个“多路复用器”，从我自己的角度来讲就感觉像一个服务总线，负责统一各个处理程序的消息注册，统一接收客户端或服务器消息信息，再分发给不同的事件处理程序进行处理。整个流程就一个注册->接收->处理的过程，从使用者的角度来讲，直接使用这些api还不太成熟，毕竟这些api都太底层了，需要了解太多的技术细节，也不太适合像我这种不了解C语言网络编程的人。这周花了三天的时候专门研究了下整个java.nio包，重点看了下关于Selector的运用（datagram和pipe还不太会用），结合了网络上的很多例子（尤其是《java
 nio》这本书上的例子），对selector总算有了很大的认识，对底层的io编程也有了新的了解。
　　写了个模拟下载的例子，服务器端模拟一个拥有整个硬盘资源的处理程序。客户端通过发送要下载的文件（通过完整文件路径），从而实现由服务器写文件到客户端，客户端保存接收的整体流程。其中，仅涉及到了数据传输的基本运用，即没有运用到网络编程上的urlConnection，也没有用到专门的socket,客户端也没有实现一个文件多线程下载的机制。仅仅作为一个selector的下载练习使用（当然，如果要求不高，也可以用到实际编程的）。
　　服务器端基本思路就是打开链接,绑定端口,接收信息,处理信息。详细过程如下：
　　第一步：创建服务器端socketChannel，并绑定指定端口，注册到selector上。
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- 
selector = Selector.open();   
- 
ServerSocketChannel serverSocketChannel=ServerSocketChannel.open();   
- 
serverSocketChannel.configureBlocking(**false**);
   
- 
serverSocketChannel.socket().bind(**new** InetSocketAddress(1234));
   
- 
serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- selector = Selector.open();  
- ServerSocketChannel serverSocketChannel=ServerSocketChannel.open();  
- serverSocketChannel.configureBlocking(false);  
- serverSocketChannel.socket().bind(new InetSocketAddress(1234));  
- serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);  
　　都是标准的步骤，先open,再配置block为异步的，服务器socket绑定本机端口，注册到selector上，并指定key为ACCEPT。
　　第二步：接收消息，处理信息咯。
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **for**(; ;) {
   
- 
    selector.select();   
- 
    Iterator<SelectionKey> keyIterator = selector.selectedKeys().iterator();   
- **while**(keyIterator.hasNext()) {
   
- 
    SelectionKey key = keyIterator.next();   
- **if**(key.isValid())
   
- 
        handle(key);   
- 
    keyIterator.remove();   
- 
    }   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- for(; ;) {  
-     selector.select();  
-     Iterator<SelectionKey> keyIterator = selector.selectedKeys().iterator();  
- while(keyIterator.hasNext()) {  
-     SelectionKey key = keyIterator.next();  
- if(key.isValid())  
-         handle(key);  
-     keyIterator.remove();  
-     }  
- }  
　　这也是标准步骤，先进行select，再获得selectedKey,迭代，处理，再remove掉。
在网上，看到有些例子中，对selector.select()中返回的值进行判断，如果返回为o则continue,在我这个程序中，经测试当selector.select()返回为０时，而selector.selectedKeys()确不为０，这样就没有处理信息了。从官方doc上来看，关于select()的返回值解释为“已更新其准备就绪操作集的键的数目，该数目可能为零”，即这个数目指已更新的键集，故在处理中可能键集没有更新，而选择的消息处理keys却不为０，这种情况是正确的。不清楚是不是这个意思，还望高人来解释一下。
　　第三步：就是handle方法了，处理消息事件。
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **if**(key.isAcceptable()) {
   
- 
    ServerSocketChannel channel = (ServerSocketChannel) key.channel();   
- 
    SocketChannel socketChannel = channel.accept();   
- 
    socketChannel.configureBlocking(**false**);
   
- 
    socketChannel.register(selector, SelectionKey.OP_READ);//注册读事件 
- 
    map.put(socketChannel, **new** Handle());//把socket和handle进行绑定 
- 
}   
- //用map中的handle处理read和write事件,以模拟多个文件同时进行下载 
- **if**(key.isReadable() || key.isWritable()) {
   
- 
    SocketChannel socketChannel = (SocketChannel) key.channel();   
- **final** Handle handle = map.get(socketChannel);
   
- **if**(handle != **null**)
   
- 
    　　　　handle.handle(key);   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- if(key.isAcceptable()) {  
-     ServerSocketChannel channel = (ServerSocketChannel) key.channel();  
-     SocketChannel socketChannel = channel.accept();  
-     socketChannel.configureBlocking(false);  
-     socketChannel.register(selector, SelectionKey.OP_READ);//注册读事件
-     map.put(socketChannel, new Handle());//把socket和handle进行绑定
- }  
- //用map中的handle处理read和write事件,以模拟多个文件同时进行下载
- if(key.isReadable() || key.isWritable()) {  
-     SocketChannel socketChannel = (SocketChannel) key.channel();  
- final Handle handle = map.get(socketChannel);  
- if(handle != null)  
-     　　　　handle.handle(key);  
- }  
　　在以上方法中，我在主方法中仅处理appcet事件，再为每个连接到的socketChannel注册读事件，再在读消息处理中注册写事件。而读和写消息处理，我用了一个内部类来处理，即每个内部类来绑定一个socketChannel,单独处理每个socketChannel。这样的处理，是满足客户端对服务器端发起多个请求，来下载不同的文件，这样服务器端就可为不同的客户端socketChannel定制不同的处理程序了。内部类的定义如下：
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **private****class** Handle{
   
- **private** StringBuilder message;
   
- **private****boolean** writeOK = **true**;
   
- **private** ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
   
- **private** FileChannel fileChannel;
   
- **private** String fileName;
   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- privateclass Handle{  
- private StringBuilder message;  
- privateboolean writeOK = true;  
- private ByteBuffer byteBuffer = ByteBuffer.allocate(1024);  
- private FileChannel fileChannel;  
- private String fileName;  
- }  
 　　message指由客户端发送的信息，在此定义此信息为客户端请求的文件信息。由message来得到服务器端的文件名路径信息，并保存到fileName中，fileChannel即为由此fileName取得的channel。byteBuffer就是用来写数据的字节数据缓冲器了。
　　handle单独处理读和写事件，在读事件中，解析文件名，并注册写事件，代码如下：
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **if**(key.isReadable()) {
   
- 
    SocketChannel socketChannel = (SocketChannel) key.channel();   
- **if**(writeOK)
   
- 
        message = **new** StringBuilder();
   
- **while**(**true**) {
   
- 
        byteBuffer.clear();   
- **int** r = socketChannel.read(byteBuffer);
   
- **if**(r == 0)
   
- **break**;
   
- **if**(r == -1) {
   
- 
            socketChannel.close();   
- 
            key.cancel();   
- **return**;
   
- 
        }   
- 
        message.append(**new** String(byteBuffer.array(), 0, r));
   
- 
    }   
- //将接收到的信息转化成文件名,以映射到服务器上的指定文件 
- **if**(writeOK && invokeMessage(message)) {
   
- 
        socketChannel.register(selector, SelectionKey.OP_WRITE);   
- 
        writeOK = **false**;
   
- 
    }   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- if(key.isReadable()) {  
-     SocketChannel socketChannel = (SocketChannel) key.channel();  
- if(writeOK)  
-         message = new StringBuilder();  
- while(true) {  
-         byteBuffer.clear();  
- int r = socketChannel.read(byteBuffer);  
- if(r == 0)  
- break;  
- if(r == -1) {  
-             socketChannel.close();  
-             key.cancel();  
- return;  
-         }  
-         message.append(new String(byteBuffer.array(), 0, r));  
-     }  
- //将接收到的信息转化成文件名,以映射到服务器上的指定文件
- if(writeOK && invokeMessage(message)) {  
-         socketChannel.register(selector, SelectionKey.OP_WRITE);  
-         writeOK = false;  
-     }  
- }  
 以上代码就主要是读信息，并解析信息成一个文件名，并注册写事件了。当然还处理客户端断开连接事件，读到信息为－１时，断开连接。其中处理文件信息代码如下：
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- 
String m = message.toString();   
- **try** {
   
- 
    File f = **new** File(m);
   
- **if**(!f.exists())
   
- **return****false**;
   
- 
    fileName = m;   
- **return****true**;
   
- 
} **catch**(Exception e) {
   
- **return****false**;
   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- String m = message.toString();  
- try {  
-     File f = new File(m);  
- if(!f.exists())  
- returnfalse;  
-     fileName = m;  
- returntrue;  
- } catch(Exception e) {  
- returnfalse;  
- }  
其中就是将message转化成一个fileName,以供在写的时候能够从fileName中取得fileChannel,此方法保存fileName是存在的。
　　下面看写事件的处理：
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- //向客户端写数据 
- **if**(key.isWritable()) {
   
- **if**(!key.isValid())
   
- **return**;
   
- 
    SocketChannel socketChannel = (SocketChannel) key.channel();   
- **if**(fileChannel == **null**)
   
- 
        fileChannel = **new** FileInputStream(fileName).getChannel();
   
- 
    byteBuffer.clear();   
- **int** w = fileChannel.read(byteBuffer);
   
- //如果文件已写完,则关掉key和socket 
- **if**(w <= 0) {
   
- 
        fileName = **null**;
   
- 
        fileChannel.close();   
- 
        fileChannel = **null**;
   
- 
        writeOK = **true**;
   
- 
        socketChannel.close();   
- 
        key.channel();   
- **return**;
   
- 
    }   
- 
　　　　　byteBuffer.flip();   
- 
　　　　　socketChannel.write(byteBuffer);   
- 
}  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711352#)[copy](http://blog.csdn.net/kangojian/article/details/5711352#)
- //向客户端写数据
- if(key.isWritable()) {  
- if(!key.isValid())  
- return;  
-     SocketChannel socketChannel = (SocketChannel) key.channel();  
- if(fileChannel == null)  
-         fileChannel = new FileInputStream(fileName).getChannel();  
-     byteBuffer.clear();  
- int w = fileChannel.read(byteBuffer);  
- //如果文件已写完,则关掉key和socket
- if(w <= 0) {  
-         fileName = null;  
-         fileChannel.close();  
-         fileChannel = null;  
-         writeOK = true;  
-         socketChannel.close();  
-         key.channel();  
- return;  
-     }  
- 　　　　　byteBuffer.flip();  
- 　　　　　socketChannel.write(byteBuffer);  
- }  
 　　写处理中，主要就是打开本地的文件channel将fileChannel中的数据写到socketChannel中，如果数据已经写完毕，则关掉相应channel。
　　至此，服务器端的信息就处理完毕，运行这个程序就只需要在main方法中，new().call()就可以了。当然，有服务器端还需要客户端才行，客户端信息请参照下一笔记。
　　服务器端代码随附件中。
#    多路复用
       多路复用是指两个或多个用户共享公用信道的一种机制。通过多路复用技术，多个终端能共享一条高速信道，从而达到节省信道资源的目的，多路复用有频分多路复用（FDMA），时分多路复用（TDMA），码分多路复用（CDMA）几种。
**频分多路复用（FDMA）**    频分制是将传输频带分成N部分，每一个部分均可作为一个独立的传输信道使用。,如图所示。这样在一对传输线路上可有N对话路信息传送，而每一对话路所占用的只是其中的一个频段。频分制通信又称载波通信，它是模拟通信的主要手段。
** 时分多路复用（TDMA）**    时分制是把一个传输通道进行时间分割以传送若干话路的信息，如图所示。把N个话路设备接到一条公共的通道上，按一定的次序轮流的给各个设备分配一段使用通道的时间。当轮到某个设备时，这个设备与通道接通，执行操作。与此同时，其它设备与通道的联系均被切断。待指定的使用时间间隔一到，则通过时分多路转换开关把通道联接到下一个要连接的设备上去。时分制通信也称时间分割通信，它是数字[电话](http://product.it168.com/list/b/0149_1.shtml)多路通信的主要方法，因而[PC](http://pc.it168.com/)M通信常称为时分多路通信。
** 码分多路复用（CDMA）**    CDMA技术不是一项新技术，作为一种多址方案它已经成功地应用于卫星通信和蜂窝电话领域，并且显示出许多优于其他技术的特点。但是，由于卫星通信和移动通信中带宽的限制，所以CDMA技术尚未充分发挥优点。[光纤](http://product.it168.com/list/b/0422_1.shtml)通信具有丰富的带宽，能够很好地弥补这个缺陷。近年来，OCDMA已经成为一项备受瞩目的热点技术。 
    OCDMA技术在原理上与电码分复用技术相似。OCDMA通信系统给每个用户分配一个唯一的光正交码的码字作为该用户的地址码。在发送端，对要传输的数据该地址码进行光正交编码，然后实现信道复用；在接收端，用与发端相同的地址码进行光正交解码。
