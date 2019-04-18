# Java nio 客户端连接Server - z69183787的专栏 - CSDN博客
2016年10月27日 14:37:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1198
在做通信系统的开发过程中，经常需要使用Socket通信。java新的io机制给我提供了一个很好的异步socket通信方式，这段时间用java写了一个客户端用来连接server。发现运行效率还比较让人满意。下面是我实现的部分功能。
连接服务器的socket，多线程启动。如果连接失败就重连。
Java代码  ![收藏代码](http://fokman.iteye.com/images/icon_star.png)
- publicclass CommonSocket extends Thread {  
- private SocketChannel socketChannel;  
- privateboolean stop = false;  
- privateint port = 0;  
- private String ip = "";  
- private Selector selector = null;  
- private SocketAddress socketAddress = null;  
- private Logger logger = Logger.getLogger(CommonSocket.class);  
- 
- public CommonSocket() {  
- this.ip = SocketInfoUtils.TCP_IP;  
- this.port = SocketInfoUtils.TCP_PORT;  
-     }  
- 
- publicvoid run() {  
- while (!stop) {  
-             socketConnet();  
- try {  
-                 sleep(5000);  
-             } catch (InterruptedException e) {  
-                 logger.error("SocketConnect run error: InterruptedException");  
-             }  
-         }  
-     }  
- 
- publicvoid socketBuilder() {  
- try {  
-             selector = Selector.open();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-             logger.error("Open to selector failed: IOException");  
-         }  
-     }  
- 
- privatevoid openSocketChannel() {  
- try {  
-             socketAddress = new InetSocketAddress(ip, port);  
-             socketChannel = SocketChannel.open();  
-             socketChannel.socket().setReuseAddress(true);  
-             socketChannel.connect(socketAddress);  
-         } catch (ClosedChannelException e) {  
-             logger.warn("Channel is closed: ClosedChannelException");  
-         } catch (IOException e) {  
-             logger  
-                     .warn("Connet is failed or time out,the system will automatically re-connected : IOException");  
-         }  
-     }  
- 
- /**
-      * do ClientBuilder if socket conncte success
-      */
- publicvoid socketConnet() {  
- try {  
-             openSocketChannel();  
- if (socketChannel.isOpen()) {  
- this.stop = true;  
-                 socketBuilder();  
-                 socketChannel.configureBlocking(false);  
-                 socketChannel.register(selector, SelectionKey.OP_READ  
-                         | SelectionKey.OP_WRITE);  
-                 PackageBuilder clientBuilder = new PackageBuilder(socketChannel,  
-                         selector);  
-                 clientBuilder.start();  
-                 logger.info("Has been successfully connected to " + ip  
-                         + "and port:    " + port);  
-             } else {  
-                 socketChannel.close();  
-             }  
-         } catch (ClosedChannelException e) {  
-             logger.warn("Channel is closed: ClosedChannelException");  
-         } catch (IOException e) {  
-             logger  
-                     .warn("Connet is failed or time out,the system will automatically re-connected : IOException");  
-         }  
- 
-     }  
- }  
 发送和接收事件处理，NIO是基于事件的驱动模型，这个类就是专门处理收发的。
Java代码  ![收藏代码](http://fokman.iteye.com/images/icon_star.png)
- publicclass PackageBuilder  extends Thread{  
- private SocketChannel socketChannel = null;  
- private Selector selector = null;  
- privateboolean stop = false;  
- privatebyte[] array = newbyte[1024];  
- private ByteBuffer byteBuffer;  
- private PackageQueue packageQueue;  
- private Logger logger = Logger.getLogger(PackageBuilder.class);  
- 
- public PackageBuilder(SocketChannel socketChannel,Selector selectore){  
- this.socketChannel = socketChannel;  
- this.selector = selectore;  
-         packageQueue=new PackageQueue();  
-     }  
- publicvoid run(){  
- try {  
- while (!stop) {  
-                 Thread.sleep(1);  
- if(!socketChannel.isOpen()){  
-                     reconnect();//通道没打开或者断开执行重连工作(Channel did not open the work of the implementation of re-connection )
- break;  
-                 }  
- if (selector.select(30) > 0) {  
-                     doSelector();  
-                 }  
-             }  
-         } catch (IOException e) {  
-             logger.error("CameraBuilder run error: IOException");  
-         } catch (InterruptedException e){  
-             logger.error("CameraBuilder run error: InterruptedException");  
-         }  
-     }  
- publicvoid doSelector(){  
- for(SelectionKey key:selector.selectedKeys()){  
-             selector.selectedKeys().remove(key);  
- if(!key.isValid()){  
- continue;  
-             }  
-             doKeys(key);  
-         }  
-     }  
- 
- publicvoid doKeys(SelectionKey key){  
-         SocketChannel channel = (SocketChannel)key.channel();  
- if(key.isReadable()){  
-             readResponse(channel);  
-         }  
- if(key.isWritable()){  
-             sendRequest(channel);  
-         }  
-     }  
- privatevoid readResponse(SocketChannel channel) {  
-         byteBuffer=ByteBuffer.wrap(array);  
-         byteBuffer.clear();  
- int count = 0;  
- try {  
-             count = channel.read(byteBuffer);  
-         } catch (IOException e) {  
-             reconnect();//通道没打开或者断开执行重连工作(Channel did not open the work of the implementation of re-connection )
-             logger.error("Connection reset by peer: IOException");  
-         }  
- if(count != -1){  
-             byteBuffer.flip();  
- byte[] bs = newbyte[count];  
-             byteBuffer.get(bs);  
-             ByteBuffer returnBuffer = ByteBuffer.allocate(count);  
-             returnBuffer.clear();  
-             returnBuffer.put(bs);  
-             returnBuffer.flip();  
-             PrintUtil.printBf(returnBuffer.array());  
-             ParseBufferData parseData=new ParseBufferData(returnBuffer);          
-             parseData.parseBuffer();              
-       }  
- if(count < 0){  
-             reconnect();  
-         }  
-     }  
- /**
-      * send pakcet of request
-      * @param channel
-      */
- publicvoid sendRequest(SocketChannel channel){  
- byte[] array = packageQueue.takeMsgs();  
- if(array!=null){  
-         ByteBuffer byteBuffer = ByteBuffer.wrap(array);  
- try {  
-                 channel.write(byteBuffer);  
-              } catch (IOException e) {  
-                  reconnect();//通道没打开或者断开执行重连工作(Channel did not open the work of the implementation of re-connection )
-                 logger.warn("socket not connected or has been closed: IOException");  
-              }  
-          }  
-     }  
- 
- publicvoid reconnect(){  
-         stopClient();  
-         logger.warn("socket not connected or has been closed");  
-         ThreadPoolUtil.getExecutor().execute(new CameraSocket());  
-     }  
- 
- publicvoid stopClient(){  
- this.stop = true;  
- if(socketChannel.isConnected() && !socketChannel.isOpen()){  
- try {  
-                 socketChannel.close();  
-                 logger.info("server_socket has connected");  
-             } catch (IOException e) {  
-                 logger.warn("Channel closed to failed: IOException");  
-             }  
-         }  
-     }  
- }  
 发送和接收数据存放在缓存中
Java代码  ![收藏代码](http://fokman.iteye.com/images/icon_star.png)
- publicclass PackageQueue {  
- privatestatic  List<byte[]> queue = new ArrayList<byte[]>();  
- 
- public PackageQueue(){    
-     }  
- 
- publicvoid pushMsgs(byte[] array){  
- synchronized(queue){  
-             queue.add(array);  
-         }  
-     }  
- 
- publicbyte[] takeMsgs() {  
- synchronized (queue) {  
- byte[] sd=null;  
- if(queue != null){  
- if(queue.size() > 0){  
-                     sd = queue.get(0);  
-                     queue.remove(0);  
-                 }  
-             }  
- return sd;  
-         }  
- 
-     }  
- 
- publicstatic List<byte[]> getQueue() {  
- return queue;  
-     }  
- 
- publicstaticvoid setQueue(List<byte[]> queue) {  
-         PackageQueue.queue = queue;  
-     }  
- }  
```java
public class CameraSocket extends Thread {
    private int cmdPort = SocketInfoUtils.CMD_PORT; // 5554
    private String host = SocketInfoUtils.HOST; // 172.16.163.38
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    //	DatagramChannel dataChannel;
    DatagramChannel cmdChannel;
    Selector selector;
    CameraQueue cameraQueue;
    public CameraSocket() throws Exception {
        selector = Selector.open();
        cameraQueue = new CameraQueue();
        cmdChannel = DatagramChannel.open();
        cmdChannel.configureBlocking(false);
        SocketAddress target = new InetSocketAddress(host, cmdPort);
        cmdChannel.connect(target);
        cmdChannel.register(selector, SelectionKey.OP_WRITE);
    }
    @Override
    public void run() {
        boolean flag = true;
        while (flag) {
            try {
                doSelector();
            } catch (IOException e) {
                flag = false;
                e.printStackTrace();
            }
        }
    }
    private void doSelector() throws IOException {
        if (selector.select(1000) > 0) {
            for (SelectionKey key : selector.selectedKeys()) {
                if (key.isWritable()) {
                    writeEvent(cmdChannel);
                }
            }
            selector.selectedKeys().clear();
        }
    }
//	private void readEvent(SelectionKey key) throws IOException {
//	ByteBuffer buffer = ByteBuffer.allocate(1024);
//	dataChannel.receive(buffer);
//	buffer.flip();
//	ParseBufferData parseBufferData=new ParseBufferData(buffer);
//	parseBufferData.parseBuffer();
//	}
    private void writeEvent(DatagramChannel channel) throws IOException {
        byte[] array = cameraQueue.takeMsgs();
        if (array != null) {
            ByteBuffer byteBuffer = ByteBuffer.wrap(array);
            try {
                channel.write(byteBuffer);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
```java
public class SocketInfoUtils {
    public static Properties factory = SocketPropertiesFactory.getInstance().getBoundle();
    public static String TCP_IP = factory.getProperty("tcp_ip");
    public static int TCP_PORT = Integer.parseInt(factory.getProperty("tcp_port"));
    public static int CAMERA_PORT=Integer.parseInt(factory.getProperty("camera_port"));
    //public static int UDP_PORT = Integer.parseInt(factory.getProperty("udp_port"));
    public static int HIS_UDP_PORT = Integer.parseInt(factory.getProperty("his_udp_port"));
    public static int CMD_PORT = Integer.parseInt(factory.getProperty("cmd_port"));
    public static int DATA_PORT = Integer.parseInt(factory.getProperty("data_port"));
    public static final String HOST = factory.getProperty("host");
}
public class ThreadPoolUtil {
    private static ThreadPoolExecutor executor;
    static{
        BlockingQueue<Runnable> queue = new ArrayBlockingQueue<Runnable>(1);
        executor = new ThreadPoolExecutor(5,100,500,TimeUnit.MILLISECONDS,queue);
        RejectedExecutionHandler rejected = new RejectedExecutionHandler() {
            public void rejectedExecution(Runnable r, ThreadPoolExecutor executor) {
                System.out.println(String.format("======= Task %d rejected.======", r.hashCode()));
            }
        };
        executor.setRejectedExecutionHandler(rejected);
    }
    public static ThreadPoolExecutor getExecutor() {
        return executor;
    }
    public static void setExecutor(ThreadPoolExecutor executor) {
        ThreadPoolUtil.executor = executor;
    }
}
```

 以上就是客户端连接、发送、接收的代码。希望对大家有所帮助
