# Java NIO读/写入文件 - 零度的博客专栏 - CSDN博客
2017年09月25日 14:48:04[零度anngle](https://me.csdn.net/zmx729618)阅读数：508
**前言**
[Java](http://lib.csdn.net/base/java)NIO(new/inputstream outputstream)使用**通道、缓冲**来操作流，所以要深刻理解这些概念，尤其是，缓冲中的[数据结构](http://lib.csdn.net/base/datastructure)（当前位置(position)、限制(limit)、容量(capacity)）,这些知识点要通过写程序慢慢体会。
**NIO vs  传统IO**
NIO是面向缓冲、通道的；传统IO面向流
通道是双向的既可以写、也可以读；传统IO只能是单向的
NIO可以设置为异步；传统IO只能是阻塞，同步的
**缓冲区结构图**
NIO是面向缓冲区的，缓冲区可以理解为一块内存，有大小。缓冲区有位置、界限、容量几个概念。
capacity：容量，缓冲区的大小
limit：限制，表示最大的可读写的数量
position：当前位置，每当读写，当前位置都会加一
flip和clear方法，内部就操作这三个变量。
![](https://img-blog.csdn.net/20151125223846171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**缓冲区常用方法**
**clear（）：将当前位置设置为0，限制设置为容量，目的是尽最大可能让字节，由通道读取到缓冲中**
**flip（）：将限制**置为当**前位置****，然后将当前位置置为0，目的是将有数据部分的字节，由缓冲写入到通道中。通常用在读与写之间。**
java代码示例：
```
package zmx.nio.test.rw;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
public class TestJavaNIO {
	static void readNIO() {
		String pathname = "E:\\ling.txt";
		FileInputStream fin = null;
		try {
			fin = new FileInputStream(new File(pathname));
			FileChannel channel = fin.getChannel();
			int capacity = 1024;// 字节
			ByteBuffer bf = ByteBuffer.allocate(capacity);
			System.out.println("限制是：" + bf.limit() + "; 容量是：" + bf.capacity()
					+ "; 位置是：" + bf.position()); 
			int length = -1;
			while ((length = channel.read(bf)) != -1) {
				/*
				 * 注意，读取后，将位置置为0，将limit置为容量, 以备下次读入到字节缓冲中，从0开始存储
				 */
				bf.flip();
				System.out.println("限制是：" + bf.limit() + "; 容量是：" + bf.capacity()
						+ "; 位置是：" + bf.position());
				byte[] bytes = bf.array();
				System.out.write(bytes, 0, length);				
				 
			}
			channel.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (fin != null) {
				try {
					fin.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	static void writeNIO() {
		String filename = "out.txt";
		FileOutputStream fos = null;
		try {
			fos = new FileOutputStream(new File(filename));
			FileChannel channel = fos.getChannel();
			ByteBuffer src = Charset.forName("utf8").encode("你好你好你好你好你好");
			// 字节缓冲的容量和limit会随着数据长度变化，不是固定不变的
			System.out.println("初始化容量和limit：" + src.capacity() + ","
					+ src.limit());
			int length = 0;
			while ((length = channel.write(src)) != 0) {
				/*
				 * 注意，这里不需要clear，将缓冲中的数据写入到通道中后 第二次接着上一次的顺序往下读
				 */
				System.out.println("写入长度:" + length);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (fos != null) {
				try {
					fos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	static void testReadAndWriteNIO() {
		String pathname = "E:\\test.txt";
		FileInputStream fin = null;
		
		String filename = "E:\\test-out.txt";
		FileOutputStream fos = null;
		try {
			fin = new FileInputStream(new File(pathname));
			FileChannel channel = fin.getChannel();
			int capacity = 1024;// 字节
			ByteBuffer bf = ByteBuffer.allocate(capacity);
			
			System.out.println("限制是：" + bf.limit() + "容量是：" + bf.capacity()+ "位置是：" + bf.position());
			int length = -1;
			fos = new FileOutputStream(new File(filename));
			FileChannel outchannel = fos.getChannel();
			
			
			while ((length = channel.read(bf)) != -1) {
				
				//将当前位置置为limit，然后设置当前位置为0，也就是从0到limit这块，都写入到同道中
				bf.flip();
				int outlength =0;
				while((outlength=outchannel.write(bf)) != 0){
					System.out.println("读，"+length+"写,"+outlength);
				}
				
				//将当前位置置为0，然后设置limit为容量，也就是从0到limit（容量）这块，
				//都可以利用，通道读取的数据存储到
				//0到limit这块
				bf.clear();
				
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (fin != null) {
				try {
					fin.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (fos != null) {
				try {
					fos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	public static void main(String[] args) {
		//readNIO();
		testReadAndWriteNIO();
	}
}
```
