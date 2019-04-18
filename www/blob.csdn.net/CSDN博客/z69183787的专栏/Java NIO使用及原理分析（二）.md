# Java NIO使用及原理分析（二） - z69183787的专栏 - CSDN博客
2014年04月14日 10:33:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：734
个人分类：[IO-NIO](https://blog.csdn.net/z69183787/article/category/2191483)
转载自：[李会军•宁静致远](http://terrylee.me/blog/post/2010/10/01/java-nio-internal-part1.aspx)
在第一篇中，我们介绍了NIO中的两个核心对象：缓冲区和通道，在谈到缓冲区时，我们说缓冲区对象本质上是一个数组，但它其实是一个特殊的数组，缓冲区对象内置了一些机制，能够跟踪和记录缓冲区的状态变化情况，如果我们使用get()方法从缓冲区获取数据或者使用put()方法把数据写入缓冲区，都会引起缓冲区状态的变化。本文为NIO使用及原理分析的第二篇，将会分析NIO中的Buffer对象。
在缓冲区中，最重要的属性有下面三个，它们一起合作完成对缓冲区内部状态的变化跟踪：
position：指定了下一个将要被写入或者读取的元素索引，它的值由get()/put()方法自动更新，在新创建一个Buffer对象时，position被初始化为0。
limit：指定还有多少数据需要取出(在从缓冲区写入通道时)，或者还有多少空间可以放入数据(在从通道读入缓冲区时)。
capacity：指定了可以存储在缓冲区中的最大数据容量，实际上，它指定了底层数组的大小，或者至少是指定了准许我们使用的底层数组的容量。
以上四个属性值之间有一些相对大小的关系：0 <= position <= limit <= capacity。如果我们创建一个新的容量大小为10的ByteBuffer对象，在初始化的时候，position设置为0，limit和 capacity被设置为10，在以后使用ByteBuffer对象过程中，capacity的值不会再发生变化，而其它两个个将会随着使用而变化。四个属性值分别如图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310888696L9mG.gif)
现在我们可以从通道中读取一些数据到缓冲区中，注意从通道读取数据，相当于往缓冲区中写入数据。如果读取4个自己的数据，则此时position的值为4，即下一个将要被写入的字节索引为4，而limit仍然是10，如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310888701okkW.gif)
下一步把读取的数据写入到输出通道中，相当于从缓冲区中读取数据，在此之前，必须调用flip()方法，该方法将会完成两件事情：
1. 把limit设置为当前的position值 
2. 把position设置为0
由于position被设置为0，所以可以保证在下一步输出时读取到的是缓冲区中的第一个字节，而limit被设置为当前的position，可以保证读取的数据正好是之前写入到缓冲区中的数据，如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310888706NqLk.gif)
现在调用get()方法从缓冲区中读取数据写入到输出通道，这会导致position的增加而limit保持不变，但position不会超过limit的值，所以在读取我们之前写入到缓冲区中的4个自己之后，position和limit的值都为4，如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310888709lDHB.gif)
在从缓冲区中读取数据完毕后，limit的值仍然保持在我们调用flip()方法时的值，调用clear()方法能够把所有的状态变化设置为初始化时的值，如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310888714P87Y.gif)
最后我们用一段代码来验证这个过程，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612246#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612246#)[print](http://blog.csdn.net/wuxianglong/article/details/6612246#)[?](http://blog.csdn.net/wuxianglong/article/details/6612246#)
- import java.io.*;  
- import java.nio.*;  
- import java.nio.channels.*;  
- 
- publicclass Program {  
- publicstaticvoid main(String args[]) throws Exception {  
-         FileInputStream fin = new FileInputStream("d:\\test.txt");  
-         FileChannel fc = fin.getChannel();  
- 
-         ByteBuffer buffer = ByteBuffer.allocate(10);  
-         output("初始化", buffer);  
- 
-         fc.read(buffer);  
-         output("调用read()", buffer);  
- 
-         buffer.flip();  
-         output("调用flip()", buffer);  
- 
- while (buffer.remaining() > 0) {  
- byte b = buffer.get();  
- // System.out.print(((char)b));
-         }  
-         output("调用get()", buffer);  
- 
-         buffer.clear();  
-         output("调用clear()", buffer);  
- 
-         fin.close();  
-     }  
- 
- publicstaticvoid output(String step, Buffer buffer) {  
-         System.out.println(step + " : ");  
-         System.out.print("capacity: " + buffer.capacity() + ", ");  
-         System.out.print("position: " + buffer.position() + ", ");  
-         System.out.println("limit: " + buffer.limit());  
-         System.out.println();  
-     }  
- }  
完成的输出结果为：
![](http://hi.csdn.net/attachment/201107/17/0_1310888998l3v4.gif)
这与我们上面演示的过程一致。在后面的文章中，我们继续介绍NIO中关于缓冲区一些更高级的使用。
**(未完待续)**
