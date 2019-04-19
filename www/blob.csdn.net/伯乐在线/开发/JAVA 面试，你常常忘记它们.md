# JAVA 面试，你常常忘记它们 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [顾星竹](http://www.jobbole.com/members/harjoogoo) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 静态类和静态方法
如果一个类要被声明为static的，只有一种情况，就是静态内部类。
静态内部类实际上与普通类（即类名必须与文件名一样的顶级类）一样，只是静态内部类在某一类的内部定义了而已，既然是类，要想使用就必须实例化。概念上与静态变量、静态方法是不一样的，不要被“静态”两个字迷惑了（不要以为凡是静态的东西就不需要实例化就可以直接使用，静态内部类是有区别），而且只有静态内部类，而没有静态类（顶级类）的概念。
e.g.` `


```
public class Singleton{
    private Singleton(){}
    private static class SingletonHolder{
        private final static Singleton instance;
    }
    public Singleton getInstance(){
        return SingletonHolder.instance;
    }
}
```
静态方法只能访问静态成员，实例方法可以访问静态和实例成员。之所以不允许静态方法访问实例成员变量，是因为实例成员变量是属于某个对象的，而静态方法在执行时，并不一定存在对象。静态方法中也不能使用关键字this。
# 倒排索引
Inverted Index
如果翻译成转置索引可能更好懂，它就相当于做了矩阵转置。
倒排索引是一种索引方法，被用来存储在全文搜索下某个单词在一个文档或者一组文档中的存储位置的映射。
# 反射
反射API中提供的动态代理也是非常强大的功能，可以原生实现AOP中 的方法拦截功能。正如英文单词reflection的含义一样，使用反射API的时候就好像在看一个Java类在水中的倒影一样。知道了Java类的内部 结构之后，就可以与它进行交互，包括创建新的对象和调用对象中的方法等。
这种交互方式与直接在源代码中使用的效果是相同的，但是又额外提供了运行时刻的灵活性。使用反射的一个最大的弊端是性能比较差。相同的操作，用反射API所需的时间大概比直接的使用要慢一两个数量级。不过现在的JVM实现中，反射操作的性能已经有了很大的提升。
Java 反射API的第一个主要作用是获取程序在运行时刻的内部结构。
# 多路归并算法
归并排序也是一种使用分治法来实现的有效排序算法，它是现代计算机创始人John von Neumann于1945年发明的。
归并排序在众多排序算法中既是稳定排序，又有不错的效率，同时，归并排序不仅可以用于内排序，还可以用于外排序。
归并排序的思路如下(以二路归并为例)：
- 将数组划均分为两个子数组；
- 对两个字数组进行排序；
- 将排序好的两个字数组归并。
所谓 N路归并 是指将数组均分为N个子数组，将字数组排序后再归并。因此二路归并是归并排序的最一般的情况。
e.g.
![](http://jbcdn2.b0.upaiyun.com/2016/08/63193d47e97385fa22e8005eaa8375ce.png)
*图例*

```
def msort(array):
    length = len(array)
    if length == 1:
        return array
    else:
        mid = length / 2
        left = msort(array[0: mid])
        right = msort(array[mid: length])
        return merge(left, right)
```
非递归


```
def msort(array):
    length = len(array)
    step = 1
    while step < length:
        for left in range(0, length - step, 2 * step):
            result = merge(array[left:left + step],
                           array[left + step: min(left + 2 * step,
                                                  length)])
            array = array[0:left] + result + array[min(left + 2 *
                                                       step, length)]
        step = step * 2
    return array
```


```
def merge(left, right):
    llen = len(left)
    lcur = 0
    rlen = len(right)
    rcur = 0
    result = []
    while lcur < llen and rcur < rlen:
        lone = left[lcur]
        rone = right[rcur]
        result.append(min(lone, rone))
        if lone < rone:
            lcur += 1
        else:
            rcur += 1
    result += left[lcur:]
    result += right[rcur:]
    return result
```
# 枚举类型
Enumerated Type
- enum 类型不支持 public 和 protected 修饰符的构造方法，因此构造函数一定要是 private 或 friendly 的。也正因为如此，所以枚举对象是无法在程序中通过直接调用其构造方法来初始化的。
- 由于 enum 类型的值实际上是通过运行期构造出对象来表示的，所以在 cluster 环境下，每个虚拟机都会构造出一个同义的枚举对象。因而在做比较操作时候就需要注意，如果直接通过使用等号 ( ‘ == ’ ) 操作符，这些看似一样的枚举值一定不相等，因为这不是同一个对象实例。
# 多线程
Java中实现多线程有两种方法：继承Thread类、实现Runnable接口，在程序开发中只要是多线程，肯定永远以实现Runnable接口为主，因为实现Runnable接口相比继承Thread类有如下优势：
> 
1、可以避免由于Java的单继承特性而带来的局限；
2、增强程序的健壮性，代码能够被多个线程共享，代码与数据是独立的；
3、适合多个相同程序代码的线程区处理同一资源的情况。
下面是通过实现Runnable接口实现的多线程程序，代码如下：


```
lass MyThread implements Runnable{  
    private int ticket = 5;  
    public void run(){  
        for (int i=0;i<10;i++)  
        {  
            if(ticket > 0){  
                System.out.println("ticket = " + ticket--);  
            }  
        }  
    }  
}  
public class RunnableDemo{  
    public static void main(String[] args){  
        MyThread my = new MyThread();  
        new Thread(my).start();  
        new Thread(my).start();  
        new Thread(my).start();  
    }  
}
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/477333504f50970253d45c7a6f7c359b.jpg)
