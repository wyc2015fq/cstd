# 多线程Socket服务器 - 三少GG - CSDN博客
2011年10月25日 14:25:20[三少GG](https://me.csdn.net/scut1135)阅读数：1265标签：[socket																[多线程																[服务器																[linux																[jdk																[生活](https://so.csdn.net/so/search/s.do?q=生活&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Coding实战](https://blog.csdn.net/scut1135/article/category/909756)
在网上无意看到一个多线程的Socket服务器端例子，觉得非常不错。特别是其中的线程池的思想，简单而且高效。虽然JDK1.5开始已经自带了线程池包，但该代码不失为学习Socket和多线程的一个好的入门例子。
下面的代码是对该例子的简单整理，补充了注释。
【代码一】PooledConnectionHandler：后台处理类
package server;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;
/**
 * <pre>
 * PooledConnectionHandler实现了Runnable接口，它的用途是处理服务器端传来的Socket连
 * 接。该类维护了一个被称为"连接池"的全局链式列表(静态)，该列表在类被加载时创建。
 * 
 * 当该类的run()方法被调用时，它首先检查该"连接池"中是否有需要待处理的客户端连接，如果
 * 没有(可能是请求尚未到来)则调用wait()方法等待，而另一个静态方法processRequest则负
 * 责接收客户端请求并添加到"连接池"的末尾，然后通知所有正在等待的线程，各个等待的线程则
 * 以"互斥"的方式竞争资源(请求)当某个线程率先获取到对象锁，拿到一个连接后，将释放锁，然
 * 后在自己的线程中处理请求。各个线程之间彼此不会互相影响。
 * 
 * 需要注意的是这个类的一个方法：processRequest这个方法第一个为静态方法，因为对于这个
 * 方法来说，它只是一个负责通知的角色，所以没有必要是对象级的。将其修饰符置为static是合
 * 理的。
 * 
 * 其次我们看到在对全局资源"连接池"进行操作时，不管是往池中添加请求，还是从池中取出请求，
 * 都需要在关键的语句块中增加synchronized{![](http://www.blogjava.net/Images/dot.gif)}，来保证同一时刻不会有多个线程竞争同一个
 * 资源，或者在添加资源未完成前有另一个线程试图读取该资源。
 * 
 * 最后一个要注意的地方是其中的wait()和notifyAll()方法，wait()方法的调用发送在线程创建
 * 完成，但请求尚未到来之前，这时线程并不持有对List的锁，而notifyAll()方法唤起所有等待
 * 的线程，所有等待线程将一起竞争锁，此时只有一个线程可能检测到池非空而进入池中请求资
 * 源。
 * </pre>
*/
publicclass PooledConnectionHandler implements Runnable {
/** The client connection of Socket. */
protected Socket connection;
/** The request pool. */
protectedstatic List pool =new LinkedList();
/**
     * Instantiates a new pooled connection handler.
*/
public PooledConnectionHandler() {
    }
/*
     * (non-Javadoc)
     * 
     * @see java.lang.Runnable#run()
*/
publicvoid run() {
while (true) {
// 因为可能有多个线程同时去Pool中取Socket进行处理。
// 所以这里我们需同步，防止同一个请求被多次处理
synchronized (pool) {
while (pool.isEmpty()) {
try {
                        pool.wait();// 没有请求到来则等待
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
// 从池中取出一个Socket，准备进行处理
                connection = (Socket) pool.remove(0);
            }
// 取到Socket后则不需要同步了，因为此时是Connection是对象
// 级属性，在线程内部自己处理，不涉及公共资源的访问
            handleConnection();
        }
    }
/**
     * Process request, append Socket to pool and notify all waitting thread
     * 
     * @param requestToHandle the request to handle
*/
publicstaticvoid processRequest(Socket requestToHandle) {
// 因为有可能在向池中塞请求的时候，另外一个线程
// 正在从池中取Socket，所以这里需要同步一下
synchronized (pool) {
// 将来自客户端的请求添加到请求队列末尾
            pool.add(pool.size(), requestToHandle);
// 通知其它正在等待的线程有新请求来到，
// 此时所有处于wait状态的线程将被唤醒
            pool.notifyAll();
        }
    }    
/**
     * Handle connection.
*/
publicvoid handleConnection() {
try {
            PrintWriter streamWriter =new PrintWriter(connection
                    .getOutputStream());
            BufferedReader streamReader =new BufferedReader(
new InputStreamReader(connection.getInputStream()));
            String fileToRead = streamReader.readLine();
            BufferedReader fileReader =new BufferedReader(new FileReader(
                    fileToRead));
            String line =null;
while ((line = fileReader.readLine()) !=null)
                streamWriter.println(line);
            fileReader.close();
            streamWriter.close();
            streamReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("");
        } catch (IOException e) {
            System.out.println(""+ e);
        }
    }    
}
【代码二】PooledRemoteFileServer：多线程服务器端，负责创建线程池并等待客户端的连接请求
package server;
import java.io.IOException;
import java.net.BindException;
import java.net.ServerSocket;
import java.net.Socket;
/**
 * <pre>
 * PooledRemoteFileServer是一个多线程、池化的Socket服务器。它能够在指定的端口
 * 监听来自客户端的连接请求，同时它限定了允许同时连接的数目。
 * 
 * 在服务器端，服务器启动时创建指定定数量的后台处理类实例，这些实例实际上是实现了
 * Runnable接口的类，它们在创建完成后将立刻唤起，不停地监控来自客户端的连接。
 * 
 * 另一方面，服务器在创建线程之后，将在指定的端口监听。一旦有客户端连接上，立刻将
 * 该连接交给后台在等待的线程去处理，接着立刻返回继续在端口监听。这样的话后台线程
 * 的处理将不会造成前端服务器监听的阻塞。
 * </pre>
*/
publicclass PooledRemoteFileServer {
/** The max connections. */
protectedint maxConnections;
/** The listen port. */
protectedint listenPort;
/** The server socket. */
protected ServerSocket serverSocket;
/**
     * Instantiates a new pooled remote file server.
     * 
     * @param aListenPort the a listen port
     * @param maxConnections the max connections
*/
public PooledRemoteFileServer(int aListenPort, int maxConnections) {
        listenPort = aListenPort;// 监听端口
this.maxConnections = maxConnections;// 最大同时连接
    }
/**
     * 初始化池：每次创建一个Runnable实例，然后创建线程对象
*/
publicvoid setUpHandlers() {
for (int i =0; i < maxConnections; i++) {
            PooledConnectionHandler currentHandler =new PooledConnectionHandler();
// 线程启动后将一直监控Socket队列，以轮询的方式
// 监控是否有新的客户端请求到来，如果有的话则取
// 出处理，无的话则继续等待直至请求到来
new Thread(currentHandler, "Handler"+ i).start();
        }
    }
/**
     * 接收客户端连接
*/
publicvoid acceptConnections() {
try {
            ServerSocket server =new ServerSocket(listenPort, 5);
            Socket incomingConnection =null;
while (true) {
                incomingConnection = server.accept();
                handleConnection(incomingConnection);
            }
        } catch (BindException be) {
            System.out.println("");
        } catch (IOException ioe) {
            System.out.println(""+ listenPort);
        }
    }
/**
     * 在池中处理Socket请求
     * 
     * @param connectionToHandle the connection to handle
*/
protectedvoid handleConnection(Socket connectionToHandle) {
        PooledConnectionHandler.processRequest(connectionToHandle);
    }
publicstaticvoid main(String args[]) {
        PooledRemoteFileServer server =new PooledRemoteFileServer(1001, 3);
// 初始化线程池
        server.setUpHandlers();
// 开始在指定端口等待到来的请求
        server.acceptConnections();
    }
}
这个例子的精髓是在PooledConnectionHandler类，它首先创建一个公共的全局“线程池”(LinkList)，然后启动线程监控线程池，与此同时服务器端在接收到客户端请求后将请求加到“线程池”中，这两个动作是异步的，在加的时候不允许读，在读得到时候不允许加(通过synchronized关键字控制)，而且多个线程之间并不会互相影响，因为其中的connection属性是对象级的。
**从这个例子中我们也可以学到在多线程的情况下，哪些变量是必须设置为全局的(static)，哪些是必须设置为对象级的：即会被多个线程访问的资源必须设置为全局的，而跟线程处理状态，结果有关的属性一般必须设置为对象级的，以防止互相干扰。其次就是在多线程情况下，哪些方法是可以设置为static的而不会出现线程安全的问题，哪些方法是不能设置为静态方法的：如果方法是属于控制流程，通知，派发的，那么一般可以设置为静态的。因为这些方法一般不需要多个，一个就够了。就如同控制器只要一个就够了。而业务逻辑实现方法一般不能设置为静态的，因为静态方法不能引用对象变量(非静态变量)，但业务逻辑通常是需要针对不同的用户做出不同的处理的，所以几乎可以肯定的说是绝对会出现对象变量的。**
-------------------------------------------------------------
**生活就像打牌，不是要抓一手好牌，而是要尽力打好一手烂牌。**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
其余：
1.                           
### [Linux下多线程socket监听的c程序(例子)](http://blog.163.com/yuan_peng/blog/static/12606811920103141081182/?fromdm&fromSearch&isFromSearchEngine=yes)
2.

