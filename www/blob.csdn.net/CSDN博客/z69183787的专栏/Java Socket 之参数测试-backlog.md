# Java Socket 之参数测试-backlog - z69183787的专栏 - CSDN博客
2018年07月25日 11:45:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：197
[https://www.cnblogs.com/hippo0918/p/4652173.html](https://www.cnblogs.com/hippo0918/p/4652173.html)
之前在做项目的时候，刚好用到Spring-Integration的TCP/IP组件，在定义ServerSocket的过程中，有一个参数backlog比较突出，通过网上的查阅，才知道这是原生Java中ServerSocket的参数。通过查API得知，ServerSocket的构造参数：public ServerSocket(int port,int backlog)，API对于backlog的解析是这样的：requested maximum length of the queue of incoming connections；大意就是说TCP连接请求队列的最大容量。最初的理解就是如果ServerSocket由于请求太多处理不过来，后续的客户端连接就会放到阻塞队列里面，当队列满了（超过backlog定义的容量，默认为50），就会拒绝后续的连接。
　　1. 先上代码，再说结论：
**Server端：**
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class TestBackLog {
    public static void main(String[] args) throws IOException, InterruptedException {
        int backlog = 3;
        ServerSocket serverSocket = new ServerSocket(5000, backlog);
        //此处会造成客户端的连接阻塞，这时就会把request connection放到请求队列，而请求队列的容量就是backlog的值
        //当程序启动，此处睡眠50秒，马上启动客户端，客户端每一秒钟起一个，起到第二个的时候，第三个就无法获得到ServerSocket
        //只能等待，前两个连接已获取ServerSocket连接，只有等这两个处理完了，后续第三个才会拿到连接，进行处理（可从客户端输出得出此结论）
        Thread.sleep(50000);//模拟服务端处理高延时任务
        while (true) {
            Socket socket = serverSocket.accept();
            InputStream is = socket.getInputStream();
            OutputStream os = socket.getOutputStream();
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            PrintWriter out = new PrintWriter(new OutputStreamWriter(os));
            int length = -1;
            char[] buffer = new char[200];
            while (-1 != (length = br.read(buffer, 0, buffer.length))) {
                String string = new String(buffer, 0, length);
                System.out.println("TestBackLog receive String "
                        + socket.getInetAddress() + ":" + socket.getLocalPort() + string);
                out.write("server welcome!");
                out.flush();
                socket.shutdownOutput();
                
            }
            out.close();
            br.close();
            socket.close();
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
**Client 端：**
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class TcpClient {
    public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException {
        for(int i=0; i<10; i++) {
            Thread.sleep(1000);//创建端口太快，降低创建速率
            new Thread(new ClientThread()).start();
        }
    }
}
class ClientThread implements Runnable {
    @Override
    public void run() {
        try {
            Socket client = new Socket("127.0.0.1", 5000);
            System.out.println("client connected server");
            InputStream is = client.getInputStream();
            OutputStream os = client.getOutputStream();
            
            os.write("clinet hello world!".getBytes());
            client.shutdownOutput();//这一句非常重要啊，如果没有这句，服务端的read()会一直阻塞
            
            int length = 0;
            byte[] buffer = new byte[200];
            
            while(-1 != (length = is.read(buffer, 0, buffer.length))) {
                System.out.println("client");
                String receiveString = new String(buffer, 0, length);
                System.out.println("receiveString : " + receiveString);
            }
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　2. 步骤：在win7 64位的机器上运行的，先启动TestBackLog，TestBackLog创建ServerSocket之后，会进入睡眠状态。马上再运行TcpClient，一旦客户端连上服务端，将会在控制台输出：client connected server
　　3. 观察现象并得出结论：当服务端创建服务之后，main线程会睡眠，会睡眠50秒，那如果现在运行TcpClient，将会创建10个客户端，这时将会出现什么情况？实际情况就是：在控制台只输出了三次：client connected server，那就意味着只有三个客户端连上服务端，刚好与backlog所设定的请求队列容量一致，后续的客户端再进行连接，则会抛出异常：java.net.ConnectException: Connection refused: connect（这是在windows会发生的情况，在mac上运行也是只有三次输出，但是其他的客户端没有被拒绝，而是直到连接超时）。当服务端睡眠结束，处理最初三个客户端的请求，之后再把后面的客户端请求处理完（前提是客户端的连接没有超时）。
　　这个实验刚好验证了请求的缓存队列，队列里的客户端已经跟服务端建立连接，等待服务端处理。但是后面未进入的队列的客户端，会进行new Socket(ip, port)，还在苦苦地跟服务端进行连接啊~
