# Java中的Atomic包使用指南 - z69183787的专栏 - CSDN博客
2015年09月04日 21:51:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：607
个人分类：[多线程-原子变量AtomicX](https://blog.csdn.net/z69183787/article/category/5578339)
## 引言
Java从JDK1.5开始提供了java.util.concurrent.atomic包，方便程序员在多线程环境下，无锁的进行原子操作。原子变量的底层使用了处理器提供的原子指令，但是不同的CPU架构可能提供的原子指令不一样，也有可能需要某种形式的内部锁,所以该方法不能绝对保证线程不被阻塞。
## Atomic包介绍
在Atomic包里一共有12个类，四种原子更新方式，分别是原子更新基本类型，原子更新数组，原子更新引用和原子更新字段。Atomic包里的类基本都是使用Unsafe实现的包装类。
### 原子更新基本类型类
用于通过原子的方式更新基本类型，Atomic包提供了以下三个类：
- AtomicBoolean：原子更新布尔类型。
- AtomicInteger：原子更新整型。
- AtomicLong：原子更新长整型。
AtomicInteger的常用方法如下：
- int addAndGet(int delta) ：以原子方式将输入的数值与实例中的值（AtomicInteger里的value）相加，并返回结果
- boolean compareAndSet(int expect, int update) ：如果输入的数值等于预期值，则以原子方式将该值设置为输入的值。
- int getAndIncrement()：以原子方式将当前值加1，注意：这里返回的是自增前的值。
- void lazySet(int newValue)：最终会设置成newValue，使用lazySet设置值后，可能导致其他线程在之后的一小段时间内还是可以读到旧的值。关于该方法的更多信息可以参考并发网翻译的一篇文章《[AtomicLong.lazySet是如何工作的？](http://ifeve.com/how-does-atomiclong-lazyset-work/)》
- int getAndSet(int newValue)：以原子方式设置为newValue的值，并返回旧值。
AtomicInteger例子代码如下：
|`01`|`import``java.util.concurrent.atomic.AtomicInteger;`|
|`02`||
|`03`|`public``class````AtomicIntegerTest {```|
|`04`||
|`05`|`    ``static````AtomicInteger ai =````new``AtomicInteger(``1``);`|
|`06`||
|`07`|`    ``public``static``void````main(String[] args) {```|
|`08`|`        ``System.out.println(ai.getAndIncrement());`|
|`09`|`        ``System.out.println(ai.get());`|
|`10`|`    ``}`|
|`11`||
|`12`|`}`|
输出
1
2
### 餐后甜点
Atomic包提供了三种基本类型的原子更新，但是Java的基本类型里还有char，float和double等。那么问题来了，如何原子的更新其他的基本类型呢？Atomic包里的类基本都是使用Unsafe实现的，让我们一起看下[Unsafe的源码](http://www.docjar.com/html/api/sun/misc/Unsafe.java.html)，发现Unsafe只提供了三种CAS方法，compareAndSwapObject，compareAndSwapInt和compareAndSwapLong，再看AtomicBoolean源码，发现其是先把Boolean转换成整型，再使用compareAndSwapInt进行CAS，所以原子更新double也可以用类似的思路来实现。
## 原子更新数组类
通过原子的方式更新数组里的某个元素，Atomic包提供了以下三个类：
- AtomicIntegerArray：原子更新整型数组里的元素。
- AtomicLongArray：原子更新长整型数组里的元素。
- AtomicReferenceArray：原子更新引用类型数组里的元素。
AtomicIntegerArray类主要是提供原子的方式更新数组里的整型，其常用方法如下
- int addAndGet(int i, int delta)：以原子方式将输入值与数组中索引i的元素相加。
- boolean compareAndSet(int i, int expect, int update)：如果当前值等于预期值，则以原子方式将数组位置i的元素设置成update值。
实例代码如下：
|`01`|`public``class````AtomicIntegerArrayTest {```|
|`02`||
|`03`|`    ``static``int````[] value =````new``int````[] {````1``, ``2``};`|
|`04`||
|`05`|`    ``static````AtomicIntegerArray ai =````new``AtomicIntegerArray(value);`|
|`06`||
|`07`|`    ``public``static``void````main(String[] args) {```|
|`08`|`        ``ai.getAndSet(``0``, ``3``);`|
|`09`|`        ``System.out.println(ai.get(``0``));`|
|`10`|`                ``System.out.println(value[``0``]);`|
|`11`|`    ``}`|
|`12`||
|`13`|`}`|
输出
3
1
AtomicIntegerArray类需要注意的是，数组value通过构造方法传递进去，然后AtomicIntegerArray会将当前数组复制一份，所以当AtomicIntegerArray对内部的数组元素进行修改时，不会影响到传入的数组。
## 原子更新引用类型
原子更新基本类型的AtomicInteger，只能更新一个变量，如果要原子的更新多个变量，就需要使用这个原子更新引用类型提供的类。Atomic包提供了以下三个类：
- AtomicReference：原子更新引用类型。
- AtomicReferenceFieldUpdater：原子更新引用类型里的字段。
- AtomicMarkableReference：原子更新带有标记位的引用类型。可以原子的更新一个布尔类型的标记位和引用类型。构造方法是AtomicMarkableReference(V initialRef, boolean initialMark)
AtomicReference的使用例子代码如下：
|`01`|`public``class````AtomicReferenceTest {```|
|`02`||
|`03`|`    ``public``static````AtomicReference<user> atomicUserRef =````new``AtomicReference</user><user>();`|
|`04`||
|`05`|`    ``public``static``void````main(String[] args) {```|
|`06`|`        ````User user =````new``User(``"conan"``, ``15``);`|
|`07`|`        ``atomicUserRef.set(user);`|
|`08`|`        ````User updateUser =````new``User(``"Shinichi"``, ``17``);`|
|`09`|`        ````atomicUserRef.compareAndSet(user, updateUser);```|
|`10`|`        ``System.out.println(atomicUserRef.get().getName());`|
|`11`|`        ``System.out.println(atomicUserRef.get().getOld());`|
|`12`|`    ``}`|
|`13`||
|`14`|`    ``static``class````User {```|
|`15`|`        ``private````String name;```|
|`16`|`        ``private``int``old;`|
|`17`||
|`18`|`        ``public````User(String name,````int````old) {```|
|`19`|`            ``this````.name = name;```|
|`20`|`            ``this````.old = old;```|
|`21`|`        ``}`|
|`22`||
|`23`|`        ``public````String getName() {```|
|`24`|`            ``return``name;`|
|`25`|`        ``}`|
|`26`||
|`27`|`        ``public``int````getOld() {```|
|`28`|`            ``return``old;`|
|`29`|`        ``}`|
|`30`|`    ``}`|
|`31`|`}`|
输出
Shinichi
17
## 原子更新字段类
如果我们只需要某个类里的某个字段，那么就需要使用原子更新字段类，Atomic包提供了以下三个类：
- AtomicIntegerFieldUpdater：原子更新整型的字段的更新器。
- AtomicLongFieldUpdater：原子更新长整型字段的更新器。
- AtomicStampedReference：原子更新带有版本号的引用类型。该类将整数值与引用关联起来，可用于原子的更数据和数据的版本号，可以解决使用CAS进行原子更新时，可能出现的ABA问题。
原子更新字段类都是抽象类，每次使用都时候必须使用静态方法newUpdater创建一个更新器。原子更新类的字段的必须使用public volatile修饰符。AtomicIntegerFieldUpdater的例子代码如下：
|`01`|`public``class````AtomicIntegerFieldUpdaterTest {```|
|`02`||
|`03`|`    ``private``static````AtomicIntegerFieldUpdater<User> a = AtomicIntegerFieldUpdater```|
|`04`|`            ``.newUpdater(User.``class``, ``"old"``);`|
|`05`||
|`06`|`    ``public``static``void````main(String[] args) {```|
|`07`|`        ````User conan =````new``User(``"conan"``, ``10``);`|
|`08`|`        ``System.out.println(a.getAndIncrement(conan));`|
|`09`|`        ``System.out.println(a.get(conan));`|
|`10`|`    ``}`|
|`11`||
|`12`|`    ``public``static``class````User {```|
|`13`|`        ``private````String name;```|
|`14`|`        ``public``volatile``int``old;`|
|`15`||
|`16`|`        ``public````User(String name,````int````old) {```|
|`17`|`            ``this````.name = name;```|
|`18`|`            ``this````.old = old;```|
|`19`|`        ``}`|
|`20`||
|`21`|`        ``public````String getName() {```|
|`22`|`            ``return``name;`|
|`23`|`        ``}`|
|`24`||
|`25`|`        ``public``int````getOld() {```|
|`26`|`            ``return``old;`|
|`27`|`        ``}`|
|`28`|`    ``}`|
|`29`|`}`|
输出
10
11
