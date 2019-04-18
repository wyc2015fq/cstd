# NIO学习笔记(1) - weixin_33985507的博客 - CSDN博客
2017年05月10日 17:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
- NIO:(New IO/No blocking IO)非阻塞式IO
- 新特性:面向缓冲区,Channel,Selector
- 
![143019-640c26148e226d7e.jpg](https://upload-images.jianshu.io/upload_images/143019-640c26148e226d7e.jpg)
# 缓冲区
- 缓冲区 buffer在java NIO中负责数据的存取,缓冲区就是数组,用于存储不同数据类型的数据
- 根据数据类型(布尔除外)提供了相应类型的缓冲区
- ByteBuffer
- CharBuffer
- ShortBuffer
- IntBuffer
- LongBuffer
- FloatBuffer
- DoubleBuffer
- 上述缓冲区的管理方式几乎一致,通过allocate()获取缓冲区
- 缓冲区存取数据的两个核心方法:
- put():存入数据到缓冲区
- get():获取缓冲区中的数据
- 缓冲区中的四个核心属性
- capacity:容量,表示缓冲区中最大的存储数据容量,一旦声明不能改变
- limit:界限,表示缓冲区中可以操作数据的大小
- position:位置,表示缓冲区正在操作数据的位置
- mark:标记,记录当前position,
- 
![143019-bc911bd87d4c0e1d.jpg](https://upload-images.jianshu.io/upload_images/143019-bc911bd87d4c0e1d.jpg)
- 常用方法
- flip()翻转模式,从写转为读模式
- rewind()可重复读
- clear()清空缓冲区,但是缓冲区中的数据依然存在,但是处于被遗忘状态,数据依然存在,只position,limit回到初始状态
- mark()标记
- reset()恢复position到mark位置
```
import org.junit.Test;
import java.nio.ByteBuffer;
/**
 * Created by kun on 2017/5/10.
 */
public class Test1 {
    ByteBuffer buffer=null;
    @Test
    public void test() {
        String hello="hello,NIO,中文呢";
        buffer= ByteBuffer.allocate(1024);
        pringProperties("allocate");
        //存储数据到buffer
        buffer.put(hello.getBytes());
        pringProperties("put");
        //切换为读取模式
        buffer.flip();
        pringProperties("flip");
        //读取一部分数据
        byte[] dst=new byte[5];
        buffer.get(dst,0,5);
        System.out.println(new String(dst));
        pringProperties("get");
        pringProperties("mark");
        buffer.mark();
        //继续读
        byte[] dst2=new byte[4];
        buffer.get(dst2,0,4);
        System.out.println(new String(dst2));
        //反复读
        buffer.reset();
        pringProperties("回到标记位置reset");
        //回到初始位置,读取所有
        buffer.rewind();
        pringProperties("rewind");
        byte[] dst3=new byte[buffer.limit()];
        buffer.get(dst3);
        System.out.println(new String(dst3));
/**
                ---------allocate()---------
                limit:1024,capacity:1024,position:0
                ---------put()---------
                limit:1024,capacity:1024,position:19
                ---------flip()---------
                limit:19,capacity:1024,position:0
                hello
                ---------get()---------
                limit:19,capacity:1024,position:5
                ---------mark()---------
                limit:19,capacity:1024,position:5
                ,NIO
                ---------回到标记位置reset()---------
                limit:19,capacity:1024,position:5
                ---------rewind()---------
                limit:19,capacity:1024,position:0
                hello,NIO,中文呢
 **/
    }
    private void pringProperties(String methodName){
        System.out.println("---------"+methodName+"()---------");
        System.out.println("limit:"+buffer.limit()+",capacity:"+buffer.capacity()+",position:"+buffer.position());
    }
}
```
