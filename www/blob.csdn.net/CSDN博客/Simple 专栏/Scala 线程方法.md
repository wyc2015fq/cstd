# Scala  线程方法 - Simple 专栏 - CSDN博客
2018年09月17日 03:40:02[Simple_Zz](https://me.csdn.net/love284969214)阅读数：452
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
线程(`Thread`)类提供了各种方法来处理线程的状态。可以使用这些方法来控制线程的执行。
下表包含`Thread`类常用的方法 -
|方法|描述|
|----|----|
|`public final String getName()`|它返回线程的名称。|
|`public final int getPriority()`|它返回线程的优先级。|
|`public Thread.State getState()`|它返回此线程的状态。该方法设计用于监视系统状态，不用于同步控制。|
|`public final boolean isAlive()`|它测试这个线程是否存活着。如果线程已经启动并且尚未死亡，则该线程仍然存在。|
|`public final void join() throws InterruptedException`|它等待线程死亡。|
|`public void run()`|如果使用单独的`Runnable`运行对象构建此线程，则调用`Runnable`对象的`run()`方法; 否则，此方法不执行任何操作并返回。|
|`public final void setName(String name)`|它用于设置线程名称。|
|`public final void setPriority(int newPriority)`|它用于设置线程的优先级。|
|`public static void sleep(long millis) throws InterruptedException`|它用于执行指定的毫秒数休眠线程。|
|`public static void yield()`|它导致当前执行的线程对象临时暂停并允许其他线程执行。|
**Scala Thread sleep()方法**
`sleep()`方法用于在指定时间内休眠线程，以毫秒为单位作为时间参数。
```
class ThreadExample extends Thread{  
    override def run(){  
    for(i<- 0 to 5){  
            println(i)  
            Thread.sleep(500)  
        }  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
    var t1 = new ThreadExample()  
    var t2 = new ThreadExample()  
        t1.start()  
        t2.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
0
0
1
1
2
2
3
3
4
4
5
5
```
**Scala Thread join()示例**
`join()`方法等待线程死机，换句话说，`join()`方法用于保持当前正在运行的线程的执行，直到指定的线程完成它的执行。
```
class ThreadExample extends Thread{  
    override def run(){  
        for(i<- 0 to 5){  
            println(i)  
            Thread.sleep(500)  
        }  
    }  
}  
object MainObject{  
    def main(args:Array[String]){  
        var t1 = new ThreadExample()  
        var t2 = new ThreadExample()  
        var t3 = new ThreadExample()  
        t1.start()  
        t1.join()  
        t2.start()  
        t3.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
0
1
2
3
4
5
0
0
1
1
2
2
3
3
4
4
5
5
```
**Scala setName()方法示例**
在以下示例中，正在设置和获取线程名称。
```
class ThreadExample() extends Thread{  
    override def run(){  
        for(i<- 0 to 5){  
            println(this.getName()+" - "+i)  
            Thread.sleep(500)  
        }  
    }  
}  
object MainObject{  
    def main(args:Array[String]){  
        var t1 = new ThreadExample()  
        var t2 = new ThreadExample()  
        var t3 = new ThreadExample()  
        t1.setName("First Thread")  
        t2.setName("Second Thread")  
        t1.start()  
        t2.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
First Thread - 0
Second Thread - 0
Second Thread - 1
First Thread - 1
Second Thread - 2
First Thread - 2
Second Thread - 3
First Thread - 3
Second Thread - 4
First Thread - 4
Second Thread - 5
First Thread - 5
```
**Scala线程优先级示例**
可以使用其预定义的方法设置线程优先级，以下示例设置线程的优先级。
```
class ThreadExample() extends Thread{  
    override def run(){  
        for(i<- 0 to 5){  
            println(this.getName())  
            println(this.getPriority())  
            Thread.sleep(500)  
        }  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var t1 = new ThreadExample()  
        var t2 = new ThreadExample()  
        t1.setName("First Thread")  
        t2.setName("Second Thread")  
        t1.setPriority(Thread.MIN_PRIORITY)  
        t2.setPriority(Thread.MAX_PRIORITY)  
        t1.start()  
        t2.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
First Thread
Second Thread
10
1
Second Thread
10
First Thread
1
Second Thread
10
First Thread
1
Second Thread
10
First Thread
1
Second Thread
10
First Thread
1
Second Thread
10
First Thread
1
```
**Scala线程多任务示例**
以下示例通过使用多个线程来运行多个任务，这个例子说明了如何在Scala中实现多任务。
```
class ThreadExample() extends Thread{  
    override def run(){  
        for(i<- 0 to 5){  
        println(i)              
        Thread.sleep(500)  
    }  
}  
def task(){  
    for(i<- 0 to 5){  
            println(i)  
            Thread.sleep(200)  
        }  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var t1 = new ThreadExample()  
        t1.start()  
        t1.task()           
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
First Thread
0
0
1
2
1
3
4
2
5
3
4
5
```
