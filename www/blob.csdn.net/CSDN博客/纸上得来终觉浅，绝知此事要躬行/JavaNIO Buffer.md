# JavaNIO:Buffer - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月11日 14:54:21[boonya](https://me.csdn.net/boonya)阅读数：445标签：[java																[nio																[buffer](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
个人分类：[JavaNIO](https://blog.csdn.net/boonya/article/category/6807386)





文章来源：[http://blog.csdn.net/guoxilen/article/details/25511283](http://blog.csdn.net/guoxilen/article/details/25511283)和[http://blog.csdn.net/guoxilen/article/details/26887811](http://blog.csdn.net/guoxilen/article/details/26887811)


## 一、关于Buffer

Java NIO中的Buffer用于和NIO通道进行交互。如你所知，数据是从通道读入缓冲区，从缓冲区写入到通道中的。

缓冲区本质上是一块可以写入数据，然后可以从中读取数据的内存。这块内存被包装成NIO Buffer对象，并提供了一组方法，用来方便的访问该块内存。


## 二、Buffer类型

Java NIO 中有以下Buffer类型：ByteBuffer、MappedByteBuffer、CharBuffer、DoubleBuffer、FloatBuffer、IntBuffer、LongBuffer和ShortBuffer等。 这些Buffer类型代表了不同的基本数据类型，可以通过char、short、int、long、float或double类型来操作缓冲区中的字节。


这些类没有一种能够直接实例化。它们都是抽象类，但是都包含静态工厂方法用来创建相应类的新实例。

新的缓冲区是由分配或包装操作创建的。分配操作创建一个缓冲区对象并分配一个私有的空间来储存容量大小的数据元素；包装操作创建一个缓冲区对象但是不分配任何空间来储存数据元素，它使用提供的数组作为存储空间来储存缓冲区中的数据元素。

## 三、使用示例

 使用Buffer读写数据一般遵循以下四个步骤：

（1）写入数据到Buffer；

（2）调用flip()方法；

（3）从Buffer中读取数据；

（4）调用clear()方法或者compact()方法


    当向buffer写入数据时，buffer会记录下写了多少数据。一旦要读取数据，需要通过flip()方法将Buffer从写模式切换到读模式。在读模式下，可以读取之前写入到buffer的所有数据。一旦读完了所有的数据，就需要清空缓冲区，让它可以再次被写入。



```java
// 获取文件句柄（相对于项目根路径）
RandomAccessFile file = new RandomAccessFile("nio-data.txt", "rw");

// 获取Channel
FileChannel inChannel = file.getChannel();

// 定义Buffer缓冲区，大小 48byte
ByteBuffer buf = ByteBuffer.allocate(48);

// 从Channel中读取数据写到Buffer
int bytesRead = inChannel.read(buf);

// 判断是文件否读取完毕
while (bytesRead != -1) {

	// 输出读取到的数据大小
	System.out.println("Read Size：" + bytesRead);

	// 将buffer由写模式切换到读模式
	buf.flip(); 

	//判断Buffer是否读完
	while (buf.hasRemaining()) {
		//输出从Buffer中读取的内容
		System.out.print((char) buf.get());
	}

	// 清空缓冲区，让它可以再次被写入
	buf.clear();

	// 继续从Channel中读取数据到Buffer
	bytesRead = inChannel.read(buf);
}
//关闭
file.close();
```


## 四、Buffer四个属性的说明

为了理解Buffer的工作原理，需要熟悉它的四个属性：capacity（容量）、position（位置）、limit（限制）、mark（标记）。


### Capacity

 作为一个内存块，Buffer有一个固定的大小值，也叫“capacity”.你只能往里写capacity个byte、long，char等类型。一旦Buffer满了，需要将其清空（通过读数据或者清除数据）才能继续写数据往里写数据。



### Position



当你写数据到Buffer中时，position表示当前的位置。初始的position值为0.当一个byte、long等数据写到Buffer后， position会向前移动到下一个可插入数据的Buffer单元。position最大可为capacity – 1。


当将Buffer从写模式切换到读模式时，position会被重置为0. 从Buffer的position处读取数据时，position向前移动到下一个可读的位置。


### Limit

 在写模式下，Buffer的limit表示你最多能往Buffer里写多少数据。即limit等于Buffer的capacity。

 当切换Buffer到读模式时， limit表示你最多能读到多少数据。（因此，当切换Buffer到读模式时，limit会被设置成写模式下的position值。换句话说，你能读到之前写入的所有数据(limit被设置成已写数据的数量，这个值在写模式下就是position)）。



### Mark

 一个备忘位置。标记在设定前是未定义的(undefined)。使用场景是，假设缓冲区中有 10 个元素，position 目前的位置为 2，现在只想发送 6 - 10 之间的缓冲数据，此时我们可以 buffer.mark(buffer.position())，即把当前的 position 记入 mark 中，然后 buffer.postion(6)，此时发送给 channel 的数据就是 6 - 10 的数据。发送完后，我们可以调用 buffer.reset() 使得 position = mark，因此这里的
 mark 只是用于临时记录一下位置用的。


## 五、Buffer创建

通过allocate()或者wrap()函数创建的缓冲区通常都是间接的 ,间接的缓冲区使用备份数组。




```java
/**
 * 堆空间中分配了一个char型数组作为备份存储器来储存100个char变量 
 */
CharBuffer charBuffer1 = CharBuffer.allocate (100);


/**
 * 构造了一个新的缓冲区对象，但数据元素会存在于数组中。
 * 通过调用put()函数造成的对缓冲区的改动会直接影响这个数组，而且对这个数组的任何改动也会对这个缓冲区对象可见
 */
char [] charArray = new char [100];
CharBuffer charbuffer2 = CharBuffer.wrap(charArray);

/**
 * 创建了一个position值为 12，limit值为54，容量为charArray.length的缓冲区
 * 并非创建了一个只占用了一个数组子集的缓冲区，offset和length参数只是设置了初始的状态
 */
CharBuffer charbuffer3 = CharBuffer.wrap (charArray, 12, 42);

/**
 * 关于wrap(array, offset, length)的一些注意事项
 */
char[] charArray2 = new char[10];
CharBuffer charbuffer4 = CharBuffer.wrap(charArray2, 4, 4);
/* 可以执行 */
charbuffer4.put("abcd");  
/* 不可以，buffer越界 */
charbuffer4.put("abcde");
/* 可以执行， buffer状态已经重置，position为0 limit为10 */
charbuffer4.clear();
charbuffer4.put("abcdefgh");
```

## 六、读写数据

**写入数据到Buffer有两种方式：**

（1） 从Channel写到Buffer；（2）通过Buffer的put()方法写到Buffer里。



```java
//  从Channel写到Buffer  
int bytesRead = inChannel.read(buf);   
      
//  通过put方法写Buffer  
buf.put(127);
```

**从Buffer中读取数据有两种方式：**
（1）从Buffer读取数据到Channel；（2）使用get()方法从Buffer中读取数据。



```java
//  从Buffer读取数据到Channel的例子  
int bytesWritten = inChannel.write(buf);  
      
//  使用get()方法从Buffer中读取数据的例子。get方法有很多版本，允许你以不同的方式从Buffer中读取数据  
byte aByte = buf.get();
```

**一些注意的事项：**

-  put操作如果操作会导致位置超出上界 (Limit)， 就会抛出BufferOverflowException异常
-  get操作如果位置大于或等于上界 (Limit)，就会抛出BufferUnderflowException异常
-  对只读的缓冲区的修改尝试将会导致ReadOnlyBufferException异常



觉得存取不会影响缓冲区的位置属性，但是如果您所提供的索引超出范围(负数或不小于上界)，也将抛出IndexOutOfBoundsException异常。




```java
/** 
 * 创建一个ByteBuffer并写入字符串Hello 
 */  
ByteBuffer buffer = ByteBuffer.allocate(10);  

buffer.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');
```

**![](https://img-blog.csdn.net/20140524212733500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveGlsZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**




```java
/** 
 * 更改ByteBuffer的数据，将'H'变更为'M'并追加'w' 
 */  
buffer.put(0,(byte)'M').put((byte)'w');
```
![](https://img-blog.csdn.net/20140524212821593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveGlsZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```java
/** 
 * 翻转读写模式，这里由写转为读 
 */  
buffer.flip();
```
![](https://img-blog.csdn.net/20140524212907125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveGlsZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```java
/** 
 * 读取数据 
 * hasRemaining()会在释放(读)缓冲区时告诉您是否已经达到缓冲区的上界即读完 
 */  
while(buffer.hasRemaining()){  
  /** 
   * 相对方式读取buffer中的数据 
   */  
  System.out.println((char)buffer.get());  
}
```
特别需要注意的是，读取操作不涉及清理。相对方式(get())仅读取当前数据和更新position的值，绝对方式(get(index))仅读取指定位置的数据，他们都不会将读取过的数据清除。

## 七、翻转读写模式



flip方法将Buffer从写模式切换到读模式。调用flip()方法会将position设回0，并将limit设置成之前position的值。也就是说，position在翻转之后用于标记读的位置，limit表示之前写进了多少个(byte、char等)现在能读取多少个


注意：如果将缓冲区翻转两次，大小会变为0；尝试对缓冲区上位置和上界都为0的get()操作会导致BufferUnderflowException异常，而put()则会导致BufferOverflowException异常


## 八、位置标记



使缓冲区能够记住一个位置并在之后将其返回。缓冲区的标记在mark()函数被调用之前是未定义的，调用时标记被设为当前位置的值。reset()函数将位置设为当前的标记值。如果标记值未定义，调用reset()将导致InvalidMarkException异常。




```java
/** 
 * 初始化数据 
 */  
ByteBuffer buffer = ByteBuffer.allocate(10);  
buffer.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');  //写入hello  
  
/** 
 * 翻转模式 
 */  
buffer.flip();  
  
/** 
 *buffer现在的position是在H的位置，Mark它  
 */  
buffer.mark();   
  
/** 
 * 手动调整position到第一个l的位置 
 */  
buffer.position(2);   
  
/** 
 * 开始读取数据 
 */  
while(buffer.hasRemaining()){  
   System.out.print((char)buffer.get());  
}  
  
System.out.println();  
  
/** 
 * 通过reset()回到Mark记住的位置即H的位置 
 */  
buffer.reset();  
  
//  
/** 
 * 再次读取数据，从H的位置  
 */  
while(buffer.hasRemaining()){  
   System.out.print((char)buffer.get());  
}
```

## 九、压缩



如果从Buffer中读取了一部分数据之后仍有部分未读的数据，且后续还需要这些数据，但是此时想要先再写些数据，那么使用压缩。



```java
/** 
 * 准备数据 
 */  
ByteBuffer buffer = ByteBuffer.allocate(10);  
buffer.put((byte)'M').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o').put((byte)'w');  
  
/** 
 * 翻转模式 
 */  
buffer.flip();  
  
/** 
 * 释放一部分('M'和'e')，剩下的保留 
 */  
for (int i = 0; i < 2; i++) {  
  System.out.print((char)buffer.get());
}  
  
/** 
 * 释放之后，进行压缩（自动翻转为写的模式） 
 */  
buffer.compact(); 
  
/** 
 * 在剩下的部分的后面继续填充 
 */  
buffer.put((byte)'s');
  
/** 
 * 翻转模式 
 */  
buffer.flip();  
  
System.out.println();  
  
while (buffer.hasRemaining()) {  
  System.out.print((char)buffer.get());  
}
```


需要注意的是：当执行释放一部分('M'和'e')之后，如图：
![](https://img-blog.csdn.net/20140524221332265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveGlsZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而执行compact()压缩操作，将所有未读的数据拷贝到Buffer起始处(第一个未读元素的索引为0)，同时翻转为写模式，并将当前的位置(position)指定到未读数据元素的末尾，如下图所示："llow"被拷贝，开始的'l'索引为0，position指向"llow"之后，至于位置4和5中对应的'o'和'w'，因为现在正在或已经超出了当前位置，所以是死的，会被之后的put()所重写。

![](https://img-blog.csdn.net/20140524222749890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveGlsZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 十、清理

当Buffer中的数据被读取完了之后，需要让Buffer准备好再次被写入，通过clear()可以实现。


调用的clear()之后，position将被设回0，limit被设置成 capacity的值。换句话说，Buffer 被清空了。Buffer中的数据并未清除，只是这些标记告诉我们可以从哪里开始往Buffer里写数据。如果Buffer中有一些未读的数据，调用clear()方法，数据将“被遗忘”，意味着不再有任何标记会告诉你哪些数据被读过，哪些还没有。



## 十一、Buffer比较之equals



两个缓冲区被认为相等的充要条件是：


        两个对象类型相同。包含不同数据类型的buffer永远不会相等，而且buffer绝不会等于非buffer对象

        两个对象都剩余同样数量的元素（Buffer的容量不需要相同，而且缓冲区中剩余数据的索引也不必相同；但每个缓冲区中剩余元素的数目(从位置到上界)必须相同）

        在每个缓冲区中应被get()函数返回的剩余数据元素序列必须一致



```java
/** 
 * 准备数据 
 */  
ByteBuffer buffer1 = ByteBuffer.allocate(10);  
buffer1.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');  //写入Hello  
ByteBuffer buffer2 = ByteBuffer.allocate(12);  
buffer2.put((byte)'A').put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o').put((byte)'Y');  //写入AHelloY  
  
/** 
 * 结果为true 
 * 因为1和2类型都为byte, 都还有5的空余（l-p）且空余的都是没有被填充的byte 
 * 所以比较的结果跟读写的模式也有关系 
 */  
System.out.println(buffer1.equals(buffer2)); 
  
buffer1.flip();  
buffer2.flip();  
  
/** 
 * 结果为false 
 */  
System.out.println(buffer1.equals(buffer2));  
  
/* p移动到H的位置 */  
buffer2.get();  
/* l设置为Y的位置(不包含Y了) */  
buffer2.limit(6);   
/** 
 * 结果为true 
 */  
System.out.println(buffer1.equals(buffer2));
```


## 十二、Buffer比较之compareTo



CompareTo是针对每个缓冲区内剩余数据进行的，与它们在equals()中的方式相同，直到不相等的元素被发现或者到达缓冲区的上界。如果一个缓冲区在不相等元素发现前已经被耗尽，较短的缓冲区被认为是小于较长的缓冲区。



```java
/** 
 * 准备数据 
 */  
ByteBuffer buffer1 = ByteBuffer.allocate(10);  
buffer1.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');  //写入hello  
ByteBuffer buffer2 = ByteBuffer.allocate(12);  
buffer2.put((byte)'A').put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o').put((byte)'Y');  //写入AHelloY  
  
/** 
 * 结果：0 
 */  
System.out.println(buffer1.compareTo(buffer2));  
  
buffer1.flip();  
buffer2.flip();  
  
/** 
 * 结果：7 
 */  
System.out.println(buffer1.compareTo(buffer2)); 
  
/* p移动到H的位置 */  
buffer2.get();  
/* l设置为Y的位置(不包含Y了) */  
buffer2.limit(6);   
  
/** 
 * 结果：0 
 */  
System.out.println(buffer1.compareTo(buffer2));
```


## 十三、批量读写



 如果所要求的数量的数据不能被传送，那么不会有数据被传递，缓冲区的状态保持不变，同时抛出BufferUnderflowException异常。



```java
/** 
 * 准备数据 
 */  
ByteBuffer buffer = ByteBuffer.allocate(10);  
buffer.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');
  
buffer.flip();  
  
/** 
 * 等价为buffer.get(byteArr,0,byteArr.length);但是buffer不能提供byteArr长度的数据，所以BufferUnderflowException异常 
 */  
byte[] byteArr = new byte[10];  
buffer.get(byteArr);
  
/** 
 * 可以通过，buffer的状态将改变 
 */  
byte[] byteSmall = new byte[3];  
buffer.get(byteSmall);
```


如果您想将一个小型缓冲区传入一个大型数组，您需要明确地指定缓冲区中剩余的数据长度；如果缓冲区存有比数组能容纳的数量更多的数据，您可以重复利用数组读取。



```java
/** 
 * 准备数据 
 */  
ByteBuffer buffer = ByteBuffer.allocate(10);  
buffer.put((byte)'H').put((byte)'e').put((byte)'l').put((byte)'l').put((byte)'o');  //写入hello  
  
buffer.flip();  
  
  
/** 
 * 小缓冲区数据传入大型数据  
 */  
byte [] bigArray = new byte [15];  
  
// 通过remaining()获取缓冲区剩余的量  
int length = buffer.remaining();  
  
// 指定获取的量  
buffer.get(bigArray, 0, length);  
  
for (int i = 0; i < bigArray.length; i++) {  
  System.out.print((char)bigArray[i]);  
}  
  
  
System.out.println("-------------------");  
  
  
/** 
 * 重置位置，为测试方便 
 */  
buffer.position(0);  
  
  
/** 
 * 大缓冲区数据到小数组  
 */  
byte[] smallArray = new byte[2];  
  
// 循环从buffer中读取  
while (buffer.hasRemaining()) {  
  
  // smallArray长度的范围内，buffer最多还能提供多少量的数据  
  int slength = Math.min(buffer.remaining(), smallArray.length);  
  
  // 从Buffer中读取  
  buffer.get(smallArray, 0, slength);  
  
  //对获取的数据执行处理  
  for (int i = 0; i < smallArray.length; i++) {  
    System.out.print((char)smallArray[i]);  
  
    //清空已经处理的位置  
    smallArray[i] = 0;
  }  
}
```


 补充：put是类似的；另外，dstBuffer.put(srcBuffer);是两个缓冲区之前的传递，如果成功，两个缓冲区的状态都将改变

## 十四、复制缓冲区



    使用duplicate()用于创建一个与原始缓冲区相似的新缓冲区。


    两个缓冲区共享数据元素，拥有同样的容量，但每个缓冲区拥有各自的位置，上界和标记属性，但初始时是一样的。复制一个缓冲区会创建一个新的Buffer对象，但并不复制数据。原始缓冲区和副本都会操作同样的数据元素。



```java
/** 
 * 准备数据 
 */  
CharBuffer buffer = CharBuffer.allocate (8);  
buffer.put("abcdefgh");  
  
/** 
 * 执行之后，buffer的c为8,p为5,m为3,l为6 
 */  
buffer.position(3).limit(6).mark().position (5);  
  
//   
/** 
 * 执行之后，dupeBuffer的c,p,m,l和buffer一样 
 */  
CharBuffer dupeBuffer = buffer.duplicate();  
  
//  
/** 
 * 复制完成之后，各自维护一份自己的四大c,p,m,l属性 
 */  
buffer.position(2);  
dupeBuffer.position(4);
```


## 十五、分割缓冲区



 使用slice()用于创建一个从原始缓冲区的当前位置开始的新缓冲区，并且其容量是原始缓冲区的剩余元素数量(limit-position)

 这个新缓冲区与原始缓冲区共享一段数据元素子序列。分割出来的缓冲区也会继承只读和直接属性 


```java
/** 
 * 准备数据 
 */  
CharBuffer buffer = CharBuffer.allocate (8);  
buffer.put("abcdefgh");  
  
/** 
 * 设置属性 
 */  
buffer.position(2).limit(5);  
  
/** 
 * 分割 
 */  
CharBuffer sliceBuffer = buffer.slice();  
  
  
/** 
 * sliceBuffer只含有'c'、'd'和'e' 
 */  
for (int i = 0; sliceBuffer.hasRemaining(); i++) {  
System.out.print(sliceBuffer.get());  
}  
  
/** 
 * 修改 sliceBuffer的一个数据 
 */  
sliceBuffer.put(0, 'z');  
  
sliceBuffer.flip();  
  
System.out.println("");  
  
/** 
 * 'z'、'd'和'e' 
 */  
while(sliceBuffer.hasRemaining()){  
  System.out.print((char)sliceBuffer.get());  
}  
  
System.out.println("");  
  
buffer.position(0).limit(8);  
  
/** 
 * a b z d e f g h 
 */  
while(buffer.hasRemaining()){  
  System.out.print((char)buffer.get());  
}
```


## 十六、CharBuffer两个特殊的方法



```java
CharBuffer cb = CharBuffer.allocate(10);  
String str = "abcdefgh";  
  
/** 
 * 填充整个String字符串 
 */  
cb.put(str);  
  
cb.flip();  
  
/** 
 * a b c d e 
 */  
while (cb.hasRemaining()) {  
  System.out.print(cb.get());  
}  
  
cb.clear();  
  
/** 
 * 填充String字符串中指定的开始和结束之间的字符 
 * str中第1到第3的字符 start -- end-1 
 */  
cb.put(str, 1, 4);
  
cb.flip();  
  
System.out.println();  
  
/** 
 * b c d 
 */  
for (int i = 0; cb.hasRemaining(); i++) {  
  System.out.print(cb.get());  
}
```
附：ByteBuffer转String的方法  

```java
public static String getString(ByteBuffer buffer) {  
  Charset charset = null;  
  CharsetDecoder decoder = null;  
  CharBuffer charBuffer = null;  
  try {  
    charset = Charset.forName("UTF-8");  
    decoder = charset.newDecoder();  
    charBuffer = decoder.decode(buffer.asReadOnlyBuffer());  
    return charBuffer.toString();  
  } catch (Exception ex) {  
     ex.printStackTrace();  
    return "";  
  }  
}
```](https://so.csdn.net/so/search/s.do?q=nio&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




