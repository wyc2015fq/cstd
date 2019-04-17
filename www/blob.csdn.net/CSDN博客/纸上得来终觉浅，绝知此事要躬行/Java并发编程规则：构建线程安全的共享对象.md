# Java并发编程规则：构建线程安全的共享对象 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月09日 18:02:56[boonya](https://me.csdn.net/boonya)阅读数：473
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









构建线程安全的共享对象，使其在多线程环境下能够提供安全的访问。编写正确的并发程序关键在于控制共享、可变的状态进行访问管理。synchornized关键字既可以阻塞程序，也可以维护操作的原子性，它是一个线程安全与非线程安全的临界区标识，通过它我们可以控制对象的内存可见性。不得不提到volatile，volatile仅仅是控制可见性，而同步性却不及synchornized。

## 多线程访问共享变量的诡异结果：



```java
package net.jcip.examples;

/**
 * NoVisibility
 * <p/>
 * Sharing variables without synchronization
 *
 * @author Brian Goetz and Tim Peierls
 */

public class NoVisibility {
    private static boolean ready;
    private static int number;

    private static class ReaderThread extends Thread {
        public void run() {
            while (!ready)
                Thread.yield();
            System.out.println(number);
        }
    }

    public static void main(String[] args) {
        new ReaderThread().start();
        number = 42;
        ready = true;
    }
}
```


static的变量意味着，在内存中它是一个公用变量；在多线程中，这里输出的结果是难于预估的，可能是42也可能是0，甚至线程不停止；因为这里没用使用任何的同步机制加以控制。处理方法是：需要跨线程的变量，需要加入恰当的同步。



## 解决多线程同步的可见性问题：

锁不仅仅是关于同步与互斥的，也是关于内存可见的。为了保证所有线程都能够看到共享的、可变变量的最新值，读取和写入必须使用公共的锁进行同步。根据上述多线程诡异问题，下面来加入适当的同步：



```
package net.jcip.examples;

/**
 * NoVisibility
 * <p/>
 * Sharing variables without synchronization
 *
 * @author Brian Goetz and Tim Peierls
 */

public class NoVisibility {
	private static boolean ready;
	private static int number;

	private static class ReaderThread extends Thread {
		public void run() {
			synchronized (NoVisibility.class) {
				while (!ready)
					Thread.yield();

				System.out.println(this.getName() + number);
			}
		}
	}

	public static void resetValue(boolean ready, int number) {
		synchronized (NoVisibility.class) {
			NoVisibility.number = number;
			NoVisibility.ready = ready;
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < 200; i++) {
			Thread thread = new ReaderThread();
			thread.setName("THREAD:"+i + "---");
			thread.start();
			NoVisibility.resetValue(true, i);
		}
	}
}
```



最简单的改法是将变量的可见性改了，使用volatile关键字（这里不推荐，请看代码后面的注意）：



```java
package net.jcip.examples;

/**
 * NoVisibility
 * <p/>
 * Sharing variables without synchronization
 *
 * @author Brian Goetz and Tim Peierls
 */

public class NoVisibility {
	private static volatile boolean ready;
	private static volatile int number;

	private static class ReaderThread extends Thread {
		public void run() {
			while (!ready)
				Thread.yield();
			System.out.println(this.getName() + number);
		}
	}

	public static void resetValue(boolean ready, int number) {
		NoVisibility.number = number;
		NoVisibility.ready = ready;
	}

	public static void main(String[] args) {
		for (int i = 0; i < 200; i++) {
			Thread thread = new ReaderThread();
			thread.setName("THREAD:"+i + "---");
			thread.start();
			NoVisibility.resetValue(true, i);
		}
	}
}
```



注意：只有当volatile变量能够简化实现和同步策略的验证时，才使用它们。当验证正确性必须推断可见性问题时，应当避免使用volatile变量。正确使用volatile变量的方式包括：用于确保它们所引用对象的状态的可见性，或者用于标识重要的生命周期事件的发生（如，状态：启用、关闭）。**加锁可以保证可见性和原子性，volatile只能保证可见性。**

**Volatile变量使用的条件：**

1.写入变量不依赖于当前值；或者确保只有单一线程在修改该值；

2、变量不需要与其他状态参与不变约束；

3、访问变量时，没有其他原因需要加锁；


## 多线程访问造成过期数据问题：

当一个线程在没有同步的情况下读取变量，它可能会得到一个过期值。还是上面那段代码中的例子。在多线程中，可能ready在其它线程中的值还未被修改。所以很有可能ready=false;如果在多线程中处理业务，那么变量的状态将是数据出错的致命元凶。有时候数据不能快速地实现同步并不伤大雅，但是是以牺牲体验效果为代价的。如果我们多个指令在更新设备的GPS位置，可能偏差不是很大，但是效果不佳是显而易见的。来看个例子：



```java
package net.jcip.examples;

import net.jcip.annotations.*;

/**
 * MutableInteger
 * <p/>
 * Non-thread-safe mutable integer holder
 *
 * @author Brian Goetz and Tim Peierls
 */

@NotThreadSafe
public class MutableInteger {
    private int value;

    public int get() {
        return value;
    }

    public void set(int value) {
        this.value = value;
    }
}
```
将其修改为线程安全类的做法，为getter、setter加synchronized同步关键字：





```java
package net.jcip.examples;

import net.jcip.annotations.*;

/**
 * SynchronizedInteger
 * <p/>
 * Thread-safe mutable integer holder
 *
 * @author Brian Goetz and Tim Peierls
 */
@ThreadSafe
public class SynchronizedInteger {
    @GuardedBy("this") private int value;

    public synchronized int get() {
        return value;
    }

    public synchronized void set(int value) {
        this.value = value;
    }
}
```
注：getter也需要同步，否则不能保证其他线程读取到的value是最新的。


## 安全地发布对象：

发布对象 实际上就是扩大对象提供访问范围，如public的属性和方法。安全发布对象的目的是：维护对象的封装性和不可变性，防止不正当地使用对象。下面来看一个对象发布示例：



```java
package net.jcip.examples;

import java.util.*;

/**
 * Secrets
 *
 * Publishing an object
 *
 * @author Brian Goetz and Tim Peierls
 */
class Secrets {
    public static Set<Secret> knownSecrets;

    public void initialize() {
        knownSecrets = new HashSet<Secret>();
    }
}


class Secret {
}
```


最常见的发布对象就是将对象存储到静态变量中提供访问。上面initialize方法实例化存储对象到knownSecrets的引用，将Secret对象添加到knownSecrets就是一个发布对象的操作。发布对象，可以说就是提供对象对外访问的入口。它可以是一个私有属性提供共有方法的访问，这也实现了对象的发布。但是，下面的示例不推荐大家这样写：





```java
package net.jcip.examples;

/**
 * UnsafeStates
 * <p/>
 * Allowing internal mutable state to escape
 *
 * @author Brian Goetz and Tim Peierls
 */
class UnsafeStates {
    private String[] states = new String[]{
        "AK", "AL" /*...*/
    };

    public String[] getStates() {
        return states;
    }
}
```
虽然我们可以获取到状态名称，但是我们不知道对于使用getStates()方法的线程将如何使用它，这是一个线程引用的安全问题。所以，封装使得程序的正确性分析更可行，而且不容易偶然地破坏设计约束。


## 将对象封装好了再发布：

使用一个未完全封装的对象，就破坏了程序的封装性。不要让this构造在构造期间逸出。来看下面一个例子：



```java
package net.jcip.examples;

/**
 * ThisEscape
 * <p/>
 * Implicitly allowing the this reference to escape
 *
 * @author Brian Goetz and Tim Peierls
 */
public class ThisEscape {
    public ThisEscape(EventSource source) {
        source.registerListener(new EventListener() {
            public void onEvent(Event e) {
                doSomething(e);
            }
        });
    }

    void doSomething(Event e) {
    }


    interface EventSource {
        void registerListener(EventListener e);
    }

    interface EventListener {
        void onEvent(Event e);
    }

    interface Event {
    }
}
```


此处的构造函数存在方法的重写，source.registerListener的实现就是不安全的对象发布，因为在发布期间调用了registerListener方法。



修正方法：使用工厂方法防止this引用在构造期间逸出。示例如下：



```java
package net.jcip.examples;

/**
 * SafeListener
 * <p/>
 * Using a factory method to prevent the this reference from escaping during construction
 *
 * @author Brian Goetz and Tim Peierls
 */
public class SafeListener {
    private final EventListener listener;

    private SafeListener() {
        listener = new EventListener() {
            public void onEvent(Event e) {
                doSomething(e);
            }
        };
    }

    public static SafeListener newInstance(EventSource source) {
        SafeListener safe = new SafeListener();
        source.registerListener(safe.listener);
        return safe;
    }

    void doSomething(Event e) {
    }


    interface EventSource {
        void registerListener(EventListener e);
    }

    interface EventListener {
        void onEvent(Event e);
    }

    interface Event {
    }
}
```
只要在初始化期间不要进行业务处理就可以实现线程安全了，即通过其他方法来启动线程的调用。

## 安全地共享对象：

在并发程序中，使用和共享对象的一些最有效的策略如下：

**-------------线程限制：--------------**

一个线程限制的对象，通过限制在线程中，而被线程独占，且只能被占有它的线程修改。

**-------------共享只读(read-only)：-------------**

一个共享的只读对象，在没有额外同步的情况下，可以被多个线程并发地访问，但任何线程都不能修改它。共享只读对象包括可变对象与高效不可变对象。


**-------------共享线程安全(thread-safe)：-------------**

一个线程安全的对象在内部进行同步，所以其它线程无额外同步，就可以通过公共接口随意地访问它。


**-------------被守护（Guarded）：-------------**

一个被守护的对象只能通过特定的锁来访问。被守护的对象包括哪些被线程安全对象封装的对象，和已知特定的锁保护起来的已发布对象。

















