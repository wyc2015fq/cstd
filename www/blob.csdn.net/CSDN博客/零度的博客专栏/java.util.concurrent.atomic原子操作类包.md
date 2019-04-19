# java.util.concurrent.atomic原子操作类包 - 零度的博客专栏 - CSDN博客
2016年10月10日 11:41:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：2062
                
这个包里面提供了一组原子变量类。其基本的特性就是在多线程环境下，当有多个线程同时执行这些类的实例包含的方法时，具有排他性，即当某个线程进入方法，执行其中的指令时，不会被其他线程打断，而别的线程就像自旋锁一样，一直等到该方法执行完成，才由JVM从等待队列中选择一个另一个线程进入，这只是一种逻辑上的理解。实际上是借助硬件的相关指令来实现的，不会阻塞线程(或者说只是在硬件级别上阻塞了)。可以对基本数据、数组中的基本数据、对类中的基本数据进行操作。原子变量类相当于一种泛化的volatile变量，能够支持原子的和有条件的读-改-写操作。
    java.util.concurrent.atomic中的类可以分成4组：
- 标量类（Scalar）：AtomicBoolean，AtomicInteger，AtomicLong，AtomicReference
- 数组类：AtomicIntegerArray，AtomicLongArray，AtomicReferenceArray
- 更新器类：AtomicLongFieldUpdater，AtomicIntegerFieldUpdater，AtomicReferenceFieldUpdater
- 复合变量类：AtomicMarkableReference，AtomicStampedReference
第一组AtomicBoolean，AtomicInteger，AtomicLong，AtomicReference这四种基本类型用来处理布尔，整数，长整数，对象四种数据，其内部实现不是简单的使用synchronized，而是一个更为高效的方式CAS (compare and swap) + volatile和native方法，从而避免了synchronized的高开销，执行效率大为提升。如AtomicInteger的实现片断为：
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- privatestaticfinal Unsafe unsafe = Unsafe.getUnsafe();  
- privatevolatileint value;  
- publicfinalint get() {  
- return value;  
- }  
- publicfinalvoid set(int newValue) {  
-         value = newValue;  
- }  
- publicfinalboolean compareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
- 构造函数（两个构造函数）
- 默认的构造函数：初始化的数据分别是false，0，0，null
- 带参构造函数：参数为初始化的数据
- set( )和get( )方法：可以原子地设定和获取atomic的数据。类似于volatile，保证数据会在主存中设置或读取
- void set()和void lazySet()：set设置为给定值，直接修改原始值；lazySet延时设置变量值，这个等价于set()方法，但是由于字段是volatile类型的，因此次字段的修改会比普通字段（非volatile字段）有稍微的性能延时（尽管可以忽略），所以如果不是想立即读取设置的新值，允许在“后台”修改值，那么此方法就很有用。
- getAndSet( )方法
- 原子的将变量设定为新数据，同时返回先前的旧数据
- 其本质是get( )操作，然后做set( )操作。尽管这2个操作都是atomic，但是他们合并在一起的时候，就不是atomic。在Java的源程序的级别上，如果不依赖synchronized的机制来完成这个工作，是不可能的。只有依靠native方法才可以。
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- publicfinalint getAndSet(int newValue) {  
- for (;;) {  
- int current = get();  
- if (compareAndSet(current, newValue))  
- return current;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
- compareAndSet( ) 和weakCompareAndSet( )方法
- 这 两个方法都是conditional modifier方法。这2个方法接受2个参数，一个是期望数据(expected)，一个是新数据(new)；如果atomic里面的数据和期望数据一 致，则将新数据设定给atomic的数据，返回true，表明成功；否则就不设定，并返回false。JSR规范中说：以原子方式读取和有条件地写入变量但*不* 创建任何 happen-before 排序，因此不提供与除 weakCompareAndSet 目标外任何变量以前或后续读取或写入操作有关的任何保证。大意就是说调用weakCompareAndSet时并不能保证不存在happen- 
 before的发生（也就是可能存在指令重排序导致此操作失败）。但是从Java源码来看，其实此方法并没有实现JSR规范的要求，最后效果和 compareAndSet是等效的，都调用了unsafe.compareAndSwapInt()完成操作。
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- publicfinalboolean compareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
- }  
- publicfinalboolean weakCompareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
- 对于 AtomicInteger、AtomicLong还提供了一些特别的方法。
getAndIncrement( )：以原子方式将当前值加 1，相当于线程安全的i++操作。 
incrementAndGet( )：以原子方式将当前值加 1， 相当于线程安全的++i操作。
getAndDecrement( )：以原子方式将当前值减 1， 相当于线程安全的i--操作。
decrementAndGet ( )：以原子方式将当前值减 1，相当于线程安全的--i操作。 
addAndGet( )： 以原子方式将给定值与当前值相加， 实际上就是等于线程安全的i =i+delta操作。
getAndAdd( )：以原子方式将给定值与当前值相加， 相当于线程安全的t=i;i+=delta;return t;操作。
以实现一些加法，减法原子操作。(注意 --i、++i不是原子操作，其中包含有3个操作步骤：第一步，读取i；第二步，加1或减1；第三步：写回内存)
**使用AtomicReference创建线程安全的堆栈**
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- package thread;  
- import java.util.concurrent.atomic.AtomicReference;  
- publicclass ConcurrentStack<T> {  
- private AtomicReference<Node<T>>    stacks  = new AtomicReference<Node<T>>();  
- public T push(T e) {  
-         Node<T> oldNode, newNode;  
- for (;;) { // 这里的处理非常的特别，也是必须如此的。
-             oldNode = stacks.get();  
-             newNode = new Node<T>(e, oldNode);  
- if (stacks.compareAndSet(oldNode, newNode)) {  
- return e;  
-             }  
-         }  
-     }     
- public T pop() {  
-         Node<T> oldNode, newNode;  
- for (;;) {  
-             oldNode = stacks.get();  
-             newNode = oldNode.next;  
- if (stacks.compareAndSet(oldNode, newNode)) {  
- return oldNode.object;  
-             }  
-         }  
-     }     
- privatestaticfinalclass Node<T> {  
- private T       object;       
- private Node<T>   next;         
- private Node(T object, Node<T> next) {  
- this.object = object;  
- this.next = next;  
-         }  
-     }     
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
    虽然原子的标量类扩展了Number类，但并没有扩展一些基本类型的包装类，如Integer或Long，事实上他们也不能扩展：基本类型的包装类是不可以修改的，而原子变量类是可以修改的。在原子变量类中没有重新定义hashCode或equals方法，每个实例都是不同的，他们也不宜用做基于散列容器中的键值。 
      第二组AtomicIntegerArray，AtomicLongArray还有AtomicReferenceArray类进一步扩展了原子操作，对这些类型的数组提供了支持。这些类在为其数组元素提供 volatile 访问语义方面也引人注目，这对于普通数组来说是不受支持的。
他们内部并不是像AtomicInteger一样维持一个valatile变量，而是全部由native方法实现，如下
AtomicIntegerArray的实现片断：
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- privatestaticfinal Unsafe unsafe = Unsafe.getUnsafe();  
- privatestaticfinalint base = unsafe.arrayBaseOffset(int[].class);  
- privatestaticfinalint scale = unsafe.arrayIndexScale(int[].class);  
- privatefinalint[] array;  
- publicfinalint get(int i) {  
- return unsafe.getIntVolatile(array, rawIndex(i));  
- }  
- publicfinalvoid set(int i, int newValue) {  
-         unsafe.putIntVolatile(array, rawIndex(i), newValue);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
第三组AtomicLongFieldUpdater，AtomicIntegerFieldUpdater，AtomicReferenceFieldUpdater基于反射的实用工具，可以对指定类的指定 `volatile` 字段进行原子更新。API非常简单，但是也是有一些约束：
（1）字段必须是volatile类型的
（2）字段的描述类型（修饰符public/protected/default/private）是与调用者与操作对象字段的关系一致。也就是说 调用者能够直接操作对象字段，那么就可以反射进行原子操作。但是对于父类的字段，子类是不能直接操作的，尽管子类可以访问父类的字段。
（3）只能是实例变量，不能是类变量，也就是说不能加static关键字。
（4）只能是可修改变量，不能使final变量，因为final的语义就是不可修改。实际上final的语义和volatile是有冲突的，这两个关键字不能同时存在。
（5）对于**AtomicIntegerFieldUpdater**和**AtomicLongFieldUpdater** 只能修改int/long类型的字段，不能修改其包装类型（Integer/Long）。如果要修改包装类型就需要使用**AtomicReferenceFieldUpdater** 。
 netty5.0中类ChannelOutboundBuffer统计发送的字节总数，由于使用volatile变量已经不能满足，所以使用**AtomicIntegerFieldUpdater**来实现的，看下面代码：
Java代码  ![收藏代码](http://chenzehe.iteye.com/images/icon_star.png)
- //定义
- privatestaticfinal AtomicLongFieldUpdater<ChannelOutboundBuffer> TOTAL_PENDING_SIZE_UPDATER =  
-             AtomicLongFieldUpdater.newUpdater(ChannelOutboundBuffer.class, "totalPendingSize");  
- 
- privatevolatilelong totalPendingSize;  
- 
- 
- 
- //使用
- long oldValue = totalPendingSize;  
- long newWriteBufferSize = oldValue + size;  
- while (!TOTAL_PENDING_SIZE_UPDATER.compareAndSet(this, oldValue, newWriteBufferSize)) {  
-             oldValue = totalPendingSize;  
-             newWriteBufferSize = oldValue + size;  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
