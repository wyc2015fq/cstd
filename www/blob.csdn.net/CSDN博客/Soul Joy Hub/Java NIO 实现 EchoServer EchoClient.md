# Java NIO 实现 EchoServer EchoClient - Soul Joy Hub - CSDN博客

2016年12月21日 18:50:49[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1910


# 阻塞模式

EchoServer采用阻塞模式，用线程池中的工作线程处理每个客户连接。 

EchoClient也采用阻塞模式，单线程。

## server

```java
package com.test.socket.nio.echo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 使用阻塞模式的SocketChannel,ServerSocketChannel. 为了同时连接多个客户端，需要使用多线程
 */
public class blockedServer {
    private int port = 8000;
    private ServerSocketChannel serverSocketChannel = null;
    private ExecutorService executorService;
    private static final int POOL_MULTIPLE = 4;

    public blockedServer() throws IOException {
        executorService = Executors.newFixedThreadPool(Runtime.getRuntime()
                .availableProcessors() * POOL_MULTIPLE);
        // 创建一个serverSocketChannel对象
        serverSocketChannel = ServerSocketChannel.open();
        // 使得在同一个主机上关闭了服务器程序，紧接着再启动该服务器程序时，
        // 可以顺利绑定相同的端口
        serverSocketChannel.socket().setReuseAddress(true);
        // 与一个本地端口绑定
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        System.out.println("服务器启动...");
    }

    public void service() {
        while (true) {
            SocketChannel socketChannel = null;
            try {
                socketChannel = serverSocketChannel.accept();
                // 将每个客户连接都使用线程池中的一个线程来处理
                executorService.execute(new Handler(socketChannel));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) throws IOException {
        new blockedServer().service();
    }
}

class Handler implements Runnable {
    private SocketChannel socketChannel;

    public Handler(SocketChannel socketChannel) {
        this.socketChannel = socketChannel;
    }

    public void run() {
        handler(socketChannel);
    }

    public void handler(SocketChannel socketChannel) {
        try {
            Socket socket = socketChannel.socket();
            System.out.println("接收到客户连接，来自:" + socket.getInetAddress() + ":"
                    + socket.getPort());
            BufferedReader br = getReader(socket);
            PrintWriter pw = getWriter(socket);
            String msg = null;
            while ((msg = br.readLine()) != null) {
                System.out.println(msg);
                pw.println(echo(msg));
                if (msg.equals("bye")) {
                    break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (socketChannel != null) {
                    socketChannel.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private PrintWriter getWriter(Socket socket) throws IOException {
        OutputStream socketOut = socket.getOutputStream();
        return new PrintWriter(socketOut, true);
    }

    private BufferedReader getReader(Socket socket) throws IOException {
        InputStream socketIn = socket.getInputStream();
        return new BufferedReader(new InputStreamReader(socketIn));
    }

    public String echo(String msg) {
        return "echo:" + msg;
    }
}
```

## server

```java
package com.test.socket.nio.echo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.channels.SocketChannel;

public class blockedClient {
    private SocketChannel socketChannel = null;

    public blockedClient() throws IOException {
        socketChannel = SocketChannel.open();
        InetAddress ia = InetAddress.getLocalHost();
        InetSocketAddress isa = new InetSocketAddress(ia, 8000);
        socketChannel.connect(isa);
        System.out.println("与服务器的连接建立成功");
    }

    public static void main(String[] args) throws IOException {
        new blockedClient().talk();
    }

    public void talk() throws IOException {
        try {
            BufferedReader br = getReader(socketChannel.socket());
            PrintWriter pw = getWriter(socketChannel.socket());
            BufferedReader localReader = new BufferedReader(
                    new InputStreamReader(System.in));
            String msg = null;
            while ((msg = localReader.readLine()) != null) {
                pw.println(msg);
                System.out.println(br.readLine());// 接收服务器返回的消息
                // 当输出的字符串为"bye"时停止
                if (msg.equals("bye")) {
                    break;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                socketChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private PrintWriter getWriter(Socket socket) throws IOException {
        OutputStream socketOut = socket.getOutputStream();
        return new PrintWriter(socketOut, true);
    }

    private BufferedReader getReader(Socket socket) throws IOException {
        InputStream socketIn = socket.getInputStream();
        return new BufferedReader(new InputStreamReader(socketIn));
    }
}
```

# 非阻塞模式

在非阻塞模式下，EchoServer只需要启动一个主线程，就能同时处理3件事： 

1. 接收客户的连接 

2. 接收客户发送的数据 

3. 接收客户发回响应的数据

## server

```java
package com.test.socket.nio.echo;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Set;

/**
 * 使用非阻塞模式的SocketChannel,ServerSocketChannel.
 */
public class unblockedServer {
    private Selector selector = null;
    private ServerSocketChannel serverSocketChannel = null;
    private int port = 8000;
    private Charset charset = Charset.forName("GBK");

    public unblockedServer() throws IOException {
        // 创建一个selector对象
        selector = Selector.open();
        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.socket().setReuseAddress(true);
        // 使serverSocketChannel工作于非阻塞模式
        serverSocketChannel.configureBlocking(false);
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        System.out.println("服务器启动...");
    }

    public void service() throws IOException {
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
        while (selector.select() > 0) {
            Set<SelectionKey> readyKeys = selector.selectedKeys();
            Iterator<SelectionKey> it = readyKeys.iterator();
            while (it.hasNext()) {
                SelectionKey key = null;
                try {
                    key = it.next();
                    it.remove();
                    if (key.isAcceptable()) {
                        ServerSocketChannel ssc = (ServerSocketChannel) key
                                .channel();
                        SocketChannel socketChannel = (SocketChannel) ssc
                                .accept();
                        System.out.println("接收到客户连接，来自："
                                + socketChannel.socket().getInetAddress() + ":"
                                + socketChannel.socket().getPort());
                        socketChannel.configureBlocking(false);
                        ByteBuffer buffer = ByteBuffer.allocate(1024);
                        socketChannel.register(selector, SelectionKey.OP_READ
                                | SelectionKey.OP_WRITE, buffer);
                    }
                    if (key.isReadable()) {
                        receive(key);
                    }
                    if (key.isWritable()) {
                        send(key);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    try {
                        if (key != null) {
                            key.cancel();
                            key.channel().close();
                        }
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                }
            }
        }
    }

    public void send(SelectionKey key) throws IOException {
        ByteBuffer buffer = (ByteBuffer) key.attachment();
        SocketChannel socketChannel = (SocketChannel) key.channel();
        buffer.flip();// 把极限设为位置，把位置设为0
        String data = decode(buffer);
        if (data.indexOf("\r\n") == -1) {
            return;
        }
        String outputData = data.substring(0, data.indexOf("\n") + 1);
        System.out.print(outputData);
        ByteBuffer outputBuffer = encode("echo:" + outputData);
        while (outputBuffer.hasRemaining()) {
            socketChannel.write(outputBuffer);
        }
        ByteBuffer temp = encode(outputData);
        buffer.position(temp.limit());
        buffer.compact();// 删除已经处理的字符串
        if (outputData.equals("bye\r\n")) {
            key.cancel();
            socketChannel.close();
            System.out.println("关闭与客户端的连接");
        }
    }

    public void receive(SelectionKey key) throws IOException {
        ByteBuffer buffer = (ByteBuffer) key.attachment();
        SocketChannel socketChannel = (SocketChannel) key.channel();
        ByteBuffer readBuff = ByteBuffer.allocate(32);
        socketChannel.read(readBuff);
        readBuff.flip();

        buffer.limit(buffer.capacity());
        buffer.put(readBuff);
    }

    public String decode(ByteBuffer buffer) {
        CharBuffer charBuffer = charset.decode(buffer);
        return charBuffer.toString();
    }

    public ByteBuffer encode(String str) {
        return charset.encode(str);
    }

    public static void main(String[] args) throws IOException {
        new unblockedServer().service();
    }
}
```

## client

```java
package com.test.socket.nio.echo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Set;

public class unblockedClient {
    private SocketChannel socketChannel = null;
    private ByteBuffer sendBuffer = ByteBuffer.allocate(1024);
    private ByteBuffer receiveBuffer = ByteBuffer.allocate(1024);
    private Charset charset = Charset.forName("GBK");
    private Selector selector;

    public unblockedClient() throws IOException {
        socketChannel = SocketChannel.open();
        InetAddress ia = InetAddress.getLocalHost();
        InetSocketAddress isa = new InetSocketAddress(ia, 8000);
        socketChannel.connect(isa);
        socketChannel.configureBlocking(false);
        System.out.println("与服务器的连接建立成功");
        selector = Selector.open();
    }

    public static void main(String[] args) throws IOException {
        final unblockedClient client = new unblockedClient();
        Thread receiver = new Thread() {
            public void run() {
                client.receiveFromUser();
            }
        };
        receiver.start();
        client.talk();
    }

    public void receiveFromUser() {
        try {
            BufferedReader localReader = new BufferedReader(
                    new InputStreamReader(System.in));
            String msg = null;
            while ((msg = localReader.readLine()) != null) {
                synchronized (sendBuffer) {
                    sendBuffer.put(encode(msg + "\r\n"));
                }
                if (msg.equals("bye"))
                    break;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void talk() throws IOException {
        try {
            socketChannel.register(selector, SelectionKey.OP_READ
                    | SelectionKey.OP_WRITE);
            while (selector.select() > 0) {
                Set<SelectionKey> readyKeys = selector.selectedKeys();
                Iterator<SelectionKey> it = readyKeys.iterator();
                while (it.hasNext()) {
                    SelectionKey key = null;
                    try {
                        key = (SelectionKey) it.next();
                        it.remove();
                        if (key.isReadable()) {
                            receive(key);
                        }
                        if (key.isWritable()) {
                            send(key);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                        try {
                            if (key != null) {
                                key.cancel();
                                key.channel().close();
                            }
                        } catch (Exception ex) {
                            ex.printStackTrace();
                        }
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                socketChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void send(SelectionKey key) throws IOException {
        SocketChannel socketChannel = (SocketChannel) key.channel();
        synchronized (sendBuffer) {
            sendBuffer.flip();
            socketChannel.write(sendBuffer);
            sendBuffer.compact();
        }
    }

    public void receive(SelectionKey key) throws IOException {
        // 接收EchoServer发送的数据，把它放到receiveBuffer中
        // 如果receiverBuffer中有一行数据，就打印这行数据，然后把它从receiverBuffer中删除
        SocketChannel socketChannel = (SocketChannel) key.channel();
        socketChannel.read(receiveBuffer);
        receiveBuffer.flip();
        String receiveData = decode(receiveBuffer);
        if (receiveData.indexOf("\n") == -1) {
            return;
        }
        String outputData = receiveData.substring(0,
                receiveData.indexOf("\n") + 1);
        System.out.println(outputData);
        if (outputData.equals("echo:bye\r\n")) {
            key.cancel();
            socketChannel.close();
            System.out.println("关闭与服务器的连接");
            selector.close();
            System.exit(0);
        }
        ByteBuffer temp = encode(outputData);
        receiveBuffer.position(temp.limit());
        receiveBuffer.compact();
    }

    public String decode(ByteBuffer buffer) {
        CharBuffer charBuffer = charset.decode(buffer);
        return charBuffer.toString();
    }

    public ByteBuffer encode(String str) {
        return charset.encode(str);
    }
}
```

# Server的Reactor模式

我们看非阻塞模式的Client实现，会发现起了两个线程，一个为阻塞模式专门接受本地用户输入：

```java
Thread receiver = new Thread() {
            public void run() {
                client.receiveFromUser();
            }
        };
```

```java
******
while ((msg = localReader.readLine()) != null) {
******
}
******
```

而用来发送和接受Server的数据的线程则是非阻塞的。 

其实，这就是一种典型的***Reactor设计模式***。它的优点有：

1）响应快，不必为单个同步时间所阻塞，虽然Reactor本身依然是同步的； 

2）编程相对简单，可以最大程度的避免复杂的多线程及同步问题，并且避免了多线程/进程的切换开销； 

3）可扩展性，可以方便的通过增加Reactor实例个数来充分利用CPU资源； 

4）可复用性，reactor框架本身与具体事件处理逻辑无关，具有很高的复用性；

我们也可以用类似的思想把Server也设计成这样：将接收client连接的操作单独由一个线程完成，把接收数据和发送数据的操作由另一个线程完成，这可以提高server的并发性能。 

负责接收client连线的线程按照阻塞模式工作，如果收到client连接，就向selector注册读就绪和写就绪事件，否则进入阻塞状态，直到接收到了client的连接。负责接收数据和发送数据的线程按照非阻塞模式工作，只有在读就绪和写就绪的事件发生时，才执行相应的接收数据和发送数据操作。

## Server

```java
package com.test.socket.nio.echo;

import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.net.*;
import java.util.*;

public class mixServer {
    private Selector selector = null;
    private ServerSocketChannel serverSocketChannel = null;
    private int port = 8000;
    private Charset charset = Charset.forName("GBK");

    public mixServer() throws IOException {
        selector = Selector.open();
        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.socket().setReuseAddress(true);
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        System.out.println("服务器启动");
    }

    public void accept() {
        for (;;) {
            try {
                SocketChannel socketChannel = serverSocketChannel.accept();
                System.out.println("接收到客户连接，来自:"
                        + socketChannel.socket().getInetAddress() + ":"
                        + socketChannel.socket().getPort());
                socketChannel.configureBlocking(false);

                ByteBuffer buffer = ByteBuffer.allocate(1024);
                synchronized (gate) {
                    selector.wakeup();
                    socketChannel.register(selector, SelectionKey.OP_READ
                            | SelectionKey.OP_WRITE, buffer);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private Object gate = new Object();

    public void service() throws IOException {
        for (;;) {
            synchronized (gate) {
            }
            int n = selector.select();

            if (n == 0)
                continue;
            Set<SelectionKey> readyKeys = selector.selectedKeys();
            Iterator<SelectionKey> it = readyKeys.iterator();
            while (it.hasNext()) {
                SelectionKey key = null;
                try {
                    key = (SelectionKey) it.next();
                    it.remove();
                    if (key.isReadable()) {
                        receive(key);
                    }
                    if (key.isWritable()) {
                        send(key);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    try {
                        if (key != null) {
                            key.cancel();
                            key.channel().close();
                        }
                    } catch (Exception ex) {
                        e.printStackTrace();
                    }
                }
            }// #while
        }// #while
    }

    public void send(SelectionKey key) throws IOException {
        ByteBuffer buffer = (ByteBuffer) key.attachment();
        SocketChannel socketChannel = (SocketChannel) key.channel();
        buffer.flip(); // 把极限设为位置
        String data = decode(buffer);
        if (data.indexOf("\n") == -1)
            return;
        String outputData = data.substring(0, data.indexOf("\n") + 1);
        System.out.print(outputData);
        ByteBuffer outputBuffer = encode("echo:" + outputData);
        while (outputBuffer.hasRemaining())
            socketChannel.write(outputBuffer);

        ByteBuffer temp = encode(outputData);
        buffer.position(temp.limit());
        buffer.compact();

        if (outputData.equals("bye\r\n")) {
            key.cancel();
            socketChannel.close();
            System.out.println("关闭与客户的连接");
        }
    }

    public void receive(SelectionKey key) throws IOException {
        ByteBuffer buffer = (ByteBuffer) key.attachment();

        SocketChannel socketChannel = (SocketChannel) key.channel();
        ByteBuffer readBuff = ByteBuffer.allocate(32);
        socketChannel.read(readBuff);
        readBuff.flip();

        buffer.limit(buffer.capacity());
        buffer.put(readBuff);
    }

    public String decode(ByteBuffer buffer) { // 解码
        CharBuffer charBuffer = charset.decode(buffer);
        return charBuffer.toString();
    }

    public ByteBuffer encode(String str) { // 编码
        return charset.encode(str);
    }

    public static void main(String args[]) throws Exception {
        final mixServer server = new mixServer();
        Thread accept = new Thread() {
            public void run() {
                server.accept();
            }
        };
        accept.start();
        server.service();
    }
}
```

