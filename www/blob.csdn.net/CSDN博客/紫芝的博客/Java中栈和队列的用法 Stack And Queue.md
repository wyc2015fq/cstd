# Java中栈和队列的用法  Stack  And  Queue - 紫芝的博客 - CSDN博客





2019年01月18日 23:26:01[紫芝](https://me.csdn.net/qq_40507857)阅读数：69








# Java中栈和队列的用法

## 栈的实现

### 使用Java的集合类Stack
- `boolean isEmpty();`//判断当前栈是否为空，等价于empty();
- `synchronized E peek();`//获得当前栈顶元素
- `Synchronized E pop();`//获得当前栈顶元素并删除
- `E push(E object);`//将元素加入栈顶
- `Synchronized int search(Object o);`//查找元素在栈中的位置，由栈底向栈顶方向数

```java
import java.util.Stack;
public class Solution{
    public static void main(String[] args){
        Stack<Integer> sk=new Stack<Integer>();
        System.out.println(sk.isEmpty());//判断栈是否为空，true
        for(int i=0;i<10;i++){
            int e=sk.push(i+20);//将元素加入栈顶，并返回栈顶元素
            System.out.println(i+":  "+e);
        }
        System.out.println(sk.empty());//判断栈是否为空，false
        System.out.println(sk.pop());//返回栈顶元素，并删除
        System.out.println(sk.peek());//返回当前栈顶元素
        
        System.out.println("first:"+sk.search(20));//查找栈中元素的位置
        System.out.println("last:"+sk.search(29));
    }
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190118231239958.png)

### 借用LinkedList来间接实现Stack

`LinkedList`是一个继承于`AbstractSequentialList`的双向链表，也可以当作堆栈、队列、双端队列来使用
- **栈方法**:`push(e);`**等效方法**:`addFirst(e);`//向栈顶添加元素
- **栈方法**:`pop();`**等效方法**:`removeFirst();`//获得当前栈顶元素，并删除
- **栈方法**:`peek();`**等效方法**:`peekFirst();`//获得当前栈顶元素

```java
import java.util.LinkedList;
import java.util.Stack;
public class Solution{
    public static void main(String[] args){
        LinkedList<Integer> sk=new LinkedList<Integer>();
        System.out.println(sk.isEmpty());//判断是否为空，true
        //System.out.println(sk.empty());错误，没有这个方法
        for(int i=0;i<10;i++){
            //int e=sk.addFirst(i+20);错误，没有返回值
            sk.addFirst(i+20);//向栈顶添加元素
            System.out.println(i+":  "+(i+20));
        }
        System.out.println("peekFirst:"+sk.peekFirst());
        System.out.println("removeFirst:"+sk.removeFirst());//获得栈顶元素并删除
        System.out.println("peekFirst:"+sk.peekFirst());//获得栈顶元素
    }
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190118234022354.png)

## 队列的实现

Java中虽然有`Queue`接口 ，但并没有给出队列的实现类，而`LinkedList`实现了`Queue`接口。因为`LinkedList`是双向链表，所以很方便的实现队列的所有功能。使用`Queue`时，尽量避免使用`Collection`的`add()`和`remove()`方法，因为`add()`和`remove()`方法在失败时会抛出异常；要使用`offer()`来加入元素，使用`poll()`来获取并删除元素，可以通过返回值判断成与否。

### 队列定义：`Queue<E> queue=new LinkedList<E>();`
- **队列方法**:`offer(e);`**等效方法**:`offer(e)`  / `offerLast(e);`//向队尾添加元素
- **队列方法**:`poll(e);`**等效方法**:`poll(e)`  / `pollFirst(e);`//获取队首元素并删除
- **队列方法**:`peek(e);`**等效方法**:`peek(e)`  / `peeFirst(e);`//向获取队首元素
- code 1

```java
import java.util.LinkedList;
import java.util.Queue;
public class Solution{
    public static void main(String[] args){
        Queue<Integer> queue=new LinkedList<Integer>();
        System.out.println(queue.isEmpty());//判断是否为空，true
        //System.out.println(queue.empty());//错误，没有这个方法
        for(int i=0;i<10;i++){
            //int e=sk.addFirst(i+20);错误，没有返回值
            queue.offer(i+20);//向队尾添加元素
            System.out.println(i+":  "+(i+20));
        }
        System.out.println("peekFirst:"+queue.peek());
        System.out.println("removeFirst:"+queue.poll());//获得队首元素并删除
        System.out.println("peekFirst:"+queue.peek());//获得队首元素
    }
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190119001112588.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)
- code 2

```java
import java.util.LinkedList;
import java.util.Queue;
public class Solution{
    public static void main(String[] args){
        LinkedList<Integer> queue=new LinkedList<Integer>();
        System.out.println(queue.isEmpty());//判断是否为空，true
        //System.out.println(queue.empty());//错误，没有这个方法
        for(int i=0;i<10;i++){
            //int e=sk.addFirst(i+20);错误，没有返回值
            queue.offer(i+20);//向队尾添加元素
            System.out.println(i+":  "+(i+20));
        }
        System.out.println("peekFirst:"+queue.peek());
        System.out.println("removeFirst:"+queue.poll());//获得队首元素并删除
        System.out.println("peekFirst:"+queue.peek());//获得队首元素
    }
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190119001415742.png)
- code 3

```java
import java.util.LinkedList;
import java.util.Queue;
public class Solution{
    public static void main(String[] args){
        LinkedList<Integer> queue=new LinkedList<Integer>();
        System.out.println(queue.isEmpty());//判断是否为空，true
        //System.out.println(queue.empty());//错误，没有这个方法
        for(int i=0;i<10;i++){
            //int e=sk.addFirst(i+20);错误，没有返回值
            queue.offerLast(i+20);//向队尾添加元素
            System.out.println(i+":  "+(i+20));
        }
        System.out.println("peekFirst:"+queue.peekFirst());
        System.out.println("removeFirst:"+queue.pollFirst());//获得队首元素并删除
        System.out.println("peekFirst:"+queue.peekFirst());//获得队首元素
    }
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190119001721897.png)



