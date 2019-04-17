# Java线程使用同步锁交替执行打印奇数偶数 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月14日 23:12:14[boonya](https://me.csdn.net/boonya)阅读数：2853








对同一个对象进行多线程操作时，如何保证线程执行结果的一致性？我们需要对线程操作对象加同步锁。（这是一道面试题）

## 需求描述

1-20个数字

**A线程打印奇数：1，3，5，7，9，11，13，15，17，19**

**B线程打印偶数：2，4，6，8，10，12，14，16，18，20**

**C线程在AB两个线程执行完了之后打印结果：“success”。**

## 线程代码实现

Num.java

```java
package com.boonya.thread.test;

/**
 * @ClassName: Num
 * @Description: TODO(加锁计算对象)
 * @author: pengjunlin
 * @motto: 学习需要毅力，那就秀毅力
 * @date 2019-01-14 22:47
 */
public class Num {
    int value=1;
}
```

AThread.java

```java
package com.boonya.thread.test;

/**
* @ClassName: AThread
* @Description: TODO(奇数线程)
* @author: pengjunlin
* @motto: 学习需要毅力，那就秀毅力
* @date 2019-01-14 22:26
*/
public class AThread implements Runnable{

   Num num;

   public AThread(Num num){
       this.num=num;
   }

   public void run() {
       while (num.value<20){
           synchronized (num){
               if(num.value%2!=0){
                   System.out.println("AThread:"+ num.value);
                   num.value++;
               }
           }
       }

   }
}
```

BThread.java

```java
package com.boonya.thread.test;

/**
 * @ClassName: BThread
 * @Description: TODO(偶数线程)
 * @author: pengjunlin
 * @motto: 学习需要毅力，那就秀毅力
 * @date 2019-01-14 22:26
 */
public class BThread implements Runnable {

        Num num;

        public BThread(Num num){
            this.num=num;
        }

        public void run() {
            while (num.value<=20){
                synchronized (num){
                    if(num.value%2==0){
                        System.out.println("BThread:"+ num.value);
                        num.value++;
                    }
                }
            }
        }
 }
```

CThread.java

```java
package com.boonya.thread.test;

/**
 * @ClassName: CThread
 * @Description: TODO(等待结果线程)
 * @author: pengjunlin
 * @motto: 学习需要毅力，那就秀毅力
 * @date 2019-01-14 22:26
 */
public class CThread implements Runnable {

        Num num;

        public CThread(Num num){
            this.num=num;
        }

        public void run() {
            while (num.value<20){
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            System.out.println("CThread:success!");
        }
    }
```

MainTest.java

```java
package com.boonya.thread.test;

import java.util.ArrayList;
import java.util.List;

/**
 * @ClassName: MainTest
 * @Description: TODO(线程测试)
 * @author: pengjunlin
 * @motto: 学习需要毅力，那就秀毅力
 * @date 2019-01-14 21:54
 */
public class MainTest {

    public static void main(String[] args) {
        Num num=new Num();
        Thread a=new Thread(new AThread(num));
        Thread b=new Thread(new BThread(num));
        Thread c=new Thread(new CThread(num));
        a.start();
        b.start();
        c.start();
    }
}
```

## 测试结果

```
"C:\Program Files\Java\jdk1.8.0_121\bin\java" "-javaagent:C:\Users\boonya\AppData\Roaming\JetBrains\IntelliJ IDEA 2017.2.5\lib\idea_rt.jar=51911:C:\Users\boonya\AppData\Roaming\JetBrains\IntelliJ IDEA 2017.2.5\bin" -Dfile.encoding=UTF-8 -classpath "C:\Program Files\Java\jdk1.8.0_121\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\deploy.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\access-bridge-64.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\cldrdata.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\jaccess.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\jfxrt.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\nashorn.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\sunec.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\sunjce_provider.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\sunmscapi.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\sunpkcs11.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\ext\zipfs.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\javaws.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\jfxswt.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\management-agent.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_121\jre\lib\rt.jar;C:\Users\boonya\Desktop\JavaAlgorithm\target\test-classes;C:\Users\boonya\Desktop\JavaAlgorithm\target\classes" com.boonya.thread.test.MainTest
AThread:1
BThread:2
AThread:3
BThread:4
AThread:5
BThread:6
AThread:7
BThread:8
AThread:9
BThread:10
AThread:11
BThread:12
AThread:13
BThread:14
AThread:15
BThread:16
AThread:17
BThread:18
AThread:19
BThread:20
CThread:success!

Process finished with exit code 0
```

注意：synchronized 作为方法块使用时需要只能对对象加锁，不能是常用数据类型。

## AB线程改进:使用线程等待和通知

AThread.java

```java
package com.boonya.thread.test;

/**
* @ClassName: AThread
* @Description: TODO(奇数线程)
* @author: pengjunlin
* @motto: 学习需要毅力，那就秀毅力
* @date 2019-01-14 22:26
*/
public class AThread implements Runnable{

   Num num;

   public AThread(Num num){
       this.num=num;
   }

   public void run() {
       while (num.value<20){
           synchronized (num){
               if(num.value%2!=0){
                   System.out.println("AThread:"+ num.value);
                   num.value++;
                   //num.notify();
                   num.notifyAll();
               }else{
                   try {
                       num.wait();
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
           }
       }

   }
}
```

BThread.java

```java
package com.boonya.thread.test;

/**
 * @ClassName: BThread
 * @Description: TODO(偶数线程)
 * @author: pengjunlin
 * @motto: 学习需要毅力，那就秀毅力
 * @date 2019-01-14 22:26
 */
public class BThread implements Runnable {

        Num num;

        public BThread(Num num){
            this.num=num;
        }

        public void run() {
            while (num.value<=20){
                synchronized (num){
                    if(num.value%2==0){
                        System.out.println("BThread:"+ num.value);
                        num.value++;
                        //num.notify();
                        num.notifyAll();
                    }else{
                        try {
                            num.wait();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
 }
```

注：线程等待和通知的操作是节省CPU运算的一种方式，避免CPU空转（线程方法一直不停地跑类似于死循环是很可怕的），如果是要让线程停下来需要调用线程的wait()。



