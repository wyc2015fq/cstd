# LinkedBlockingQueue的put,add跟offer的区别 - z69183787的专栏 - CSDN博客
2015年07月21日 15:27:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：31221
LinkedBlockingQueue的put,add和offer的区别 
      最近在学习<<Java并发编程实践>>，有很多java.util.concurrent包下的新类。LinkedBlockingQueue就是其中之一，顾名思义这是一个阻塞的线程安全的队列，底层应该采用链表实现。
       看其API的时候发现，添加元素的方法竟然有三个：add,put,offer。
且这三个元素都是向队列尾部添加元素的意思。于是我产生了兴趣，要仔细探究一下他们之间的差别。
1.首先看一下add方法：
```java
Inserts the specified element into this queue if it is possible to do so immediately without violating capacity restrictions, returning true upon success and throwing an IllegalStateException if no space is currently available. 
    This implementation returns true if offer succeeds, else throws an IllegalStateException.
```
        LinkedBlockingQueue构造的时候若没有指定大小，则默认大小为Integer.MAX_VALUE，当然也可以在构造函数的参数中指定大小。LinkedBlockingQueue不接受null。
       add方法在添加元素的时候，若超出了度列的长度会直接抛出异常：
```java
public static void main(String args[]){
		try {
			LinkedBlockingQueue<String> queue=new LinkedBlockingQueue(2);
			
			queue.add("hello");
			queue.add("world");
			queue.add("yes");
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
//运行结果：
java.lang.IllegalStateException: Queue full
	at java.util.AbstractQueue.add(Unknown Source)
	at com.wjy.test.GrandPather.main(GrandPather.java:12)
```
2.再来看一下put方法：
```java
Inserts the specified element at the tail of this queue, waiting if necessary for space to become available.
```
      对于put方法，若向队尾添加元素的时候发现队列已经满了会发生阻塞一直等待空间，以加入元素。
```java
public static void main(String args[]){
		try {
			LinkedBlockingQueue<String> queue=new LinkedBlockingQueue(2);
			
			queue.put("hello");
			queue.put("world");
			queue.put("yes");
			
			System.out.println("yes");
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
//运行结果：
//在queue.put("yes")处发生阻塞
//下面的“yes”无法输出
```
3.最后看一下offer方法：
```java
Inserts the specified element at the tail of this queue if it is possible to do so immediately without exceeding the queue's capacity, returning true upon success and false if this queue is full. When using a capacity-restricted queue, this method is generally preferable to method add, which can fail to insert an element only by throwing an exception.
```
    offer方法在添加元素时，如果发现队列已满无法添加的话，会直接返回false。
```java
public static void main(String args[]){
		try {
			LinkedBlockingQueue<String> queue=new LinkedBlockingQueue(2);
			
			boolean bol1=queue.offer("hello");
			boolean bol2=queue.offer("world");
			boolean bol3=queue.offer("yes");
			
			System.out.println(queue.toString());
			System.out.println(bol1);
			System.out.println(bol2);
			System.out.println(bol3);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
//运行结果：
[hello, world]
true
true
false
```
    好了，竟然说了这么多了，就把从队列中取元素的方法也顺便一说。
从队列中取出并移除头元素的方法有：poll，remove，take。
poll: 若队列为空，返回null。
remove:若队列为空，抛出NoSuchElementException异常。
take:若队列为空，发生阻塞，等待有元素。
