# nio FileChannel中文乱码问题 - z69183787的专栏 - CSDN博客
2017年08月11日 17:48:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1043
[http://www.voidcn.com/blog/chuyouyinghe/article/p-5979076.html](http://www.voidcn.com/blog/chuyouyinghe/article/p-5979076.html)
最近用nio读取文件时，英文正常，读取中文时会出现乱码，经查可以用Charset类来解决：
代码如下：
```java
package com.example.demo;
import java.io.FileNotFoundException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
public class NioDemo {
	public static void main(String[] args) {
		try {
			Charset charset = Charset.forName("GBK");//Java.nio.charset.Charset处理了字符转换问题。它通过构造CharsetEncoder和CharsetDecoder将字符序列转换成字节和逆转换。  
	        CharsetDecoder decoder = charset.newDecoder();
			
			RandomAccessFile raf = new RandomAccessFile("D:\\d\\test.txt", "rw");
			FileChannel fc = raf.getChannel();
			
			ByteBuffer buffer = ByteBuffer.allocate(512); 
			CharBuffer cb = CharBuffer.allocate(512
					);
			
			int count = fc.read(buffer);
			while (count != -1) { 
				System.out.println("count = "+count); 
				 buffer.flip();
				 decoder.decode(buffer, cb, false);
				 cb.flip();
				 while (cb.hasRemaining()) {
						System.out.print(cb.get());
					} 
				 System.out.println();
				buffer.clear();
				cb.clear();
				count = fc.read(buffer);
			}
			raf.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```
test..txt文体如下：
```
据英国《每日快报》报道，爱因斯坦提出理论称，通过时间旅行前往未来的一种方式就是在宇宙中以光速旅行。这个广泛接受的理论称，实现时间旅行必须打造一艘能够以光速飞行的太空船，并且进入太空。
理论物理学家、哥伦比亚大学的briangreene称：“你可以打造出一艘宇宙飞船，进入太空并且以接近光速的速度旅行，转个圈然后回来。设想一下你飞行了六个月，然后掉头再飞六个月回来。”
```
运行结果：
![](http://img.voidcn.com/vcimg/000/005/012/353_5fc_dc4.jpg)
```java
package com.lp.liuan;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
public class FileChannelExample {
    public static void main(String args[]){
    	Charset charset = Charset.forName("GBK");//Java.nio.charset.Charset处理了字符转换问题。它通过构造CharsetEncoder和CharsetDecoder将字符序列转换成字节和逆转换。
    	CharsetDecoder decoder = charset.newDecoder();
        FileInputStream fis = null;
        try {
            fis = new FileInputStream("E:/学习总结/java乱码本质.txt");
            FileChannel fileChannel = fis.getChannel();
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
            CharBuffer charBuffer = CharBuffer.allocate(1024);
            int bytes = fileChannel.read(byteBuffer);
            while(bytes!=-1){
                byteBuffer.flip();
                decoder.decode(byteBuffer, charBuffer, false);
                charBuffer.flip();
              
                System.out.println(charBuffer);
                charBuffer.clear();
                byteBuffer.clear();
                bytes = fileChannel.read(byteBuffer);
            }
            if(fis!=null){
                fis.close();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
