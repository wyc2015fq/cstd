# Java异步I/O编程实现的两种方式：将来式和回调式 - 零度的博客专栏 - CSDN博客
2017年12月28日 11:02:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：753
```
package org.zwc.test;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousFileChannel;
import java.nio.channels.CompletionHandler;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
/**
 * Created by zhangwenchao on 2017/12/28.
 * 异步IO通道有三种：AsynchronousFileChannel  AsynchronousSocketChannel  AsynchronousServerSocketChannel  
 * 下面以异步I/O 文件通道实现两种方式:将来式和回调式（其他异步通道类似）
 * 1、将来式：主线程发起I/O操作，主线程在读取数据过程继续完成别的事情，之后通过轮询等待结果完成。
 * 2、回调式：采用事件处理机制
 */
public class Test9 {
    public static void main(String[] args) {
        futureManner();
        callbackManner();
    }
    /**
     * 将来式
     */
    public static  void futureManner(){
        try {
            Path file = Paths.get("E:/data/logs/webApp.log");
            AsynchronousFileChannel channel =  AsynchronousFileChannel.open(file, StandardOpenOption.READ,StandardOpenOption.WRITE);
            ByteBuffer byteBuffer = ByteBuffer.allocate(100_000);  //缓冲区大小
            /**
             * 从channel中读取数据到缓冲区
             */
            Future<Integer> result = channel.read(byteBuffer, 0);
            while(!result.isDone()){
                System.out.println("主线程不阻塞，可以干别的事....");
            }
            Integer byteReadNum = result.get();
            System.out.println("读取数据完毕："+byteReadNum);
            byteBuffer.flip(); //从Buffer读出数据前调用
            /**
             * 将缓冲区数据写入通道
             */
            Future<Integer> writeNum = channel.write(byteBuffer, channel.size());
            while(!writeNum.isDone()){
                System.out.println("主线程不阻塞，可以干别的事....");
            }
            channel.close();
            System.out.println("写入数据完毕："+writeNum.get());
        } catch (IOException|InterruptedException|ExecutionException e) {
            e.printStackTrace();
        }
    }
    /**
     * 回调式
     */
    public static void callbackManner(){
        try {
            Path file = Paths.get("E:/data/logs/webApp.log");
            AsynchronousFileChannel channel =  AsynchronousFileChannel.open(file, StandardOpenOption.READ,StandardOpenOption.WRITE);
            ByteBuffer byteBuffer = ByteBuffer.allocate(100_000);  //缓冲区大小
            channel.read(byteBuffer, 0, byteBuffer, new CompletionHandler<Integer, ByteBuffer>() {
                @Override
                public void completed(Integer result, ByteBuffer attachment) {
                    System.out.println("读取数据完成："+result);
                }
                @Override
                public void failed(Throwable exc, ByteBuffer attachment) {
                    exc.printStackTrace();
                }
            });
            try {
                System.out.println("主线程休眠3秒或者处理别的事情，等待IO完成");
                Thread.sleep(3000);  //zhu
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("主线程退出");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
执行结果：
将来式：
主线程不阻塞，可以干别的事....
主线程不阻塞，可以干别的事....
主线程不阻塞，可以干别的事....
主线程不阻塞，可以干别的事....
读取数据完毕：14760
主线程不阻塞，可以干别的事....
主线程不阻塞，可以干别的事....
写入数据完毕：14760
回调式：
主线程休眠3秒或者处理别的事情，等待IO完成
读取数据完成：29520
主线程退出
