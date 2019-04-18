# Java NIO使用及原理分析（三） - z69183787的专栏 - CSDN博客
2014年04月14日 10:34:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：690
个人分类：[IO-NIO](https://blog.csdn.net/z69183787/article/category/2191483)
转载自：[李会军•宁静致远](http://terrylee.me/blog/post/2010/10/01/java-nio-internal-part1.aspx)
在上一篇文章中介绍了缓冲区内部对于状态变化的跟踪机制，而对于NIO中缓冲区来说，还有很多的内容值的学习，如缓冲区的分片与数据共享，只读缓冲区等。在本文中我们来看一下缓冲区一些更细节的内容。
**缓冲区的分配**
在前面的几个例子中，我们已经看过了，在创建一个缓冲区对象时，会调用静态方法allocate()来指定缓冲区的容量，其实调用 allocate()相当于创建了一个指定大小的数组，并把它包装为缓冲区对象。或者我们也可以直接将一个现有的数组，包装为缓冲区对象，如下示例代码所示：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612263#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612263#)[print](http://blog.csdn.net/wuxianglong/article/details/6612263#)[?](http://blog.csdn.net/wuxianglong/article/details/6612263#)
- publicclass BufferWrap {  
- 
- publicvoid myMethod()  
-     {  
- // 分配指定大小的缓冲区
-         ByteBuffer buffer1 = ByteBuffer.allocate(10);  
- 
- // 包装一个现有的数组
- byte array[] = newbyte[10];  
-         ByteBuffer buffer2 = ByteBuffer.wrap( array );  
-     }  
- }  
**缓冲区分片**
在NIO中，除了可以分配或者包装一个缓冲区对象外，还可以根据现有的缓冲区对象来创建一个子缓冲区，即在现有缓冲区上切出一片来作为一个新的缓冲区，但现有的缓冲区与创建的子缓冲区在底层数组层面上是数据共享的，也就是说，子缓冲区相当于是现有缓冲区的一个视图窗口。调用slice()方法可以创建一个子缓冲区，让我们通过例子来看一下：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612263#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612263#)[print](http://blog.csdn.net/wuxianglong/article/details/6612263#)[?](http://blog.csdn.net/wuxianglong/article/details/6612263#)
- import java.nio.*;  
- 
- publicclass Program {  
- staticpublicvoid main( String args[] ) throws Exception {  
-         ByteBuffer buffer = ByteBuffer.allocate( 10 );  
- 
- // 缓冲区中的数据0-9
- for (int i=0; i<buffer.capacity(); ++i) {  
-             buffer.put( (byte)i );  
-         }  
- 
- // 创建子缓冲区
-         buffer.position( 3 );  
-         buffer.limit( 7 );  
-         ByteBuffer slice = buffer.slice();  
- 
- // 改变子缓冲区的内容
- for (int i=0; i<slice.capacity(); ++i) {  
- byte b = slice.get( i );  
-             b *= 10;  
-             slice.put( i, b );  
-         }  
- 
-         buffer.position( 0 );  
-         buffer.limit( buffer.capacity() );  
- 
- while (buffer.remaining()>0) {  
-             System.out.println( buffer.get() );  
-         }  
-     }  
- }  
在该示例中，分配了一个容量大小为10的缓冲区，并在其中放入了数据0-9，而在该缓冲区基础之上又创建了一个子缓冲区，并改变子缓冲区中的内容，从最后输出的结果来看，只有子缓冲区“可见的”那部分数据发生了变化，并且说明子缓冲区与原缓冲区是数据共享的，输出结果如下所示：
![](http://hi.csdn.net/attachment/201107/17/0_13108892665O14.gif)
**只读缓冲区**
只读缓冲区非常简单，可以读取它们，但是不能向它们写入数据。可以通过调用缓冲区的asReadOnlyBuffer()方法，将任何常规缓冲区转 换为只读缓冲区，这个方法返回一个与原缓冲区完全相同的缓冲区，并与原缓冲区共享数据，只不过它是只读的。如果原缓冲区的内容发生了变化，只读缓冲区的内容也随之发生变化：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612263#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612263#)[print](http://blog.csdn.net/wuxianglong/article/details/6612263#)[?](http://blog.csdn.net/wuxianglong/article/details/6612263#)
- import java.nio.*;  
- 
- publicclass Program {  
- staticpublicvoid main( String args[] ) throws Exception {  
-         ByteBuffer buffer = ByteBuffer.allocate( 10 );  
- 
- // 缓冲区中的数据0-9
- for (int i=0; i<buffer.capacity(); ++i) {  
-             buffer.put( (byte)i );  
-         }  
- 
- // 创建只读缓冲区
-         ByteBuffer readonly = buffer.asReadOnlyBuffer();  
- 
- // 改变原缓冲区的内容
- for (int i=0; i<buffer.capacity(); ++i) {  
- byte b = buffer.get( i );  
-             b *= 10;  
-             buffer.put( i, b );  
-         }  
- 
-         readonly.position(0);  
-         readonly.limit(buffer.capacity());  
- 
- // 只读缓冲区的内容也随之改变
- while (readonly.remaining()>0) {  
-             System.out.println( readonly.get());  
-         }  
-     }  
- }  
如果尝试修改只读缓冲区的内容，则会报ReadOnlyBufferException异常。只读缓冲区对于保护数据很有用。在将缓冲区传递给某个 对象的方法时，无法知道这个方法是否会修改缓冲区中的数据。创建一个只读的缓冲区可以保证该缓冲区不会被修改。只可以把常规缓冲区转换为只读缓冲区，而不能将只读的缓冲区转换为可写的缓冲区。
**直接缓冲区**
直接缓冲区是为加快I/O速度，使用一种特殊方式为其分配内存的缓冲区，JDK文档中的描述为：给定一个直接字节缓冲区，Java虚拟机将尽最大努 力直接对它执行本机I/O操作。也就是说，它会在每一次调用底层操作系统的本机I/O操作之前(或之后)，尝试避免将缓冲区的内容拷贝到一个中间缓冲区中 或者从一个中间缓冲区中拷贝数据。要分配直接缓冲区，需要调用allocateDirect()方法，而不是allocate()方法，使用方式与普通缓冲区并无区别，如下面的拷贝文件示例：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612263#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612263#)[print](http://blog.csdn.net/wuxianglong/article/details/6612263#)[?](http://blog.csdn.net/wuxianglong/article/details/6612263#)
- import java.io.*;  
- import java.nio.*;  
- import java.nio.channels.*;  
- 
- publicclass Program {  
- staticpublicvoid main( String args[] ) throws Exception {  
-         String infile = "c:\\test.txt";  
-         FileInputStream fin = new FileInputStream( infile );  
-         FileChannel fcin = fin.getChannel();  
- 
-         String outfile = String.format("c:\\testcopy.txt");  
-         FileOutputStream fout = new FileOutputStream( outfile );      
-         FileChannel fcout = fout.getChannel();  
- 
- // 使用allocateDirect，而不是allocate
-         ByteBuffer buffer = ByteBuffer.allocateDirect( 1024 );  
- 
- while (true) {  
-             buffer.clear();  
- 
- int r = fcin.read( buffer );  
- 
- if (r==-1) {  
- break;  
-             }  
- 
-             buffer.flip();  
- 
-             fcout.write( buffer );  
-         }  
-     }  
- }  
**内存映射文件I/O**
内存映射文件I/O是一种读和写文件数据的方法，它可以比常规的基于流或者基于通道的I/O快的多。内存映射文件I/O是通过使文件中的数据出现为 内存数组的内容来完成的，这其初听起来似乎不过就是将整个文件读到内存中，但是事实上并不是这样。一般来说，只有文件中实际读取或者写入的部分才会映射到内存中。如下面的示例代码：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612263#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612263#)[print](http://blog.csdn.net/wuxianglong/article/details/6612263#)[?](http://blog.csdn.net/wuxianglong/article/details/6612263#)
- import java.io.*;  
- import java.nio.*;  
- import java.nio.channels.*;  
- 
- publicclass Program {  
- staticprivatefinalint start = 0;<span style="font-family:FangSong_GB2312;font-size:13px;">  
- staticprivatefinalint size = 1024;  
- 
- staticpublicvoid main( String args[] ) throws Exception {  
-         RandomAccessFile raf = new RandomAccessFile( "c:\\test.txt", "rw" );  
-         FileChannel fc = raf.getChannel();  
- 
-         MappedByteBuffer mbb = fc.map( FileChannel.MapMode.READ_WRITE,  
-           start, size );  
- 
-         mbb.put( 0, (byte)97 );  
-         mbb.put( 1023, (byte)122 );  
- 
-         raf.close();  
-     }  
- }</span>  
关于缓冲区的细节内容，我们已经用了两篇文章来介绍。在下一篇中将会介绍NIO中更有趣的部分Nonblocking I/O。
**(未完待续)**
