# Java Socket通信实现文件传输/图片传输 - 三少GG - CSDN博客
2012年08月02日 16:24:18[三少GG](https://me.csdn.net/scut1135)阅读数：1660
这只是一个简单的Demo，实际应用时，可能还需要先将文件名或者文件的后缀名传给接收方，这个难度不大，大家可自行添加
发送端ClientTcpSend.java:
```
View Code
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
public class ClientTcpSend {
    public static void main(String[] args) {
        int length = 0;
        byte[] sendBytes = null;
        Socket socket = null;
        DataOutputStream dos = null;
        FileInputStream fis = null;
        try {
            try {
                socket = new Socket();
                socket.connect(new InetSocketAddress("127.0.0.1", 33456),
                        10 * 1000);
                dos = new DataOutputStream(socket.getOutputStream());
                File file = new File("C:\\Users\\JP\\Pictures\\head.jpg");
                fis = new FileInputStream(file);
                sendBytes = new byte[1024];
                while ((length = fis.read(sendBytes, 0, sendBytes.length)) > 0) {
                    dos.write(sendBytes, 0, length);
                    dos.flush();
                }
            } finally {
                if (dos != null)
                    dos.close();
                if (fis != null)
                    fis.close();
                if (socket != null)
                    socket.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
接收端ServerTcpListener.java：
```
View Code
import java.net.*;
import java.io.*;
public class ServerTcpListener implements Runnable {
    public static void main(String[] args) {
        try {
            final ServerSocket server = new ServerSocket(33456);
            Thread th = new Thread(new Runnable() {
                public void run() {
                    while (true) {
                        try {
                            System.out.println("开始监听...");
                            Socket socket = server.accept();
                            System.out.println("有链接");
                            receiveFile(socket);
                        } catch (Exception e) {
                        }
                    }
                }
            });
            th.run(); //启动线程运行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void run() {
    }
    public static void receiveFile(Socket socket) {
        byte[] inputByte = null;
        int length = 0;
        DataInputStream dis = null;
        FileOutputStream fos = null;
        try {
            try {
                dis = new DataInputStream(socket.getInputStream());
                fos = new FileOutputStream(new File("d:/cc.jpg"));
                inputByte = new byte[1024];
                System.out.println("开始接收数据...");
                while ((length = dis.read(inputByte, 0, inputByte.length)) > 0) {
                    System.out.println(length);
                    fos.write(inputByte, 0, length);
                    fos.flush();
                }
                System.out.println("完成接收");
            } finally {
                if (fos != null)
                    fos.close();
                if (dis != null)
                    dis.close();
                if (socket != null)
                    socket.close();
            }
        } catch (Exception e) {
        }
    }
}
```
分类: [Java文件与IO](http://www.cnblogs.com/alexrain/category/304028.html)
windows：
