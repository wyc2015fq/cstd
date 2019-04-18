# Netty4学习笔记-- AttributeMap - z69183787的专栏 - CSDN博客
2016年09月22日 18:02:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1838
## IoSession
[MINA](http://mina.apache.org/)的[IoSession接口](http://mina.apache.org/mina-project/apidocs/index.html)定义了一组方法，让我们可以利用IoSession来存储一些数据：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicinterface IoSession {  
-     getAttribute(Object key)  
-     getAttribute(Object key, Object defaultValue)  
-     setAttribute(Object key)  
-     setAttribute(Object key, Object value)  
-     setAttributeIfAbsent(Object key)  
-     setAttributeIfAbsent(Object key, Object value)  
-     replaceAttribute(Object key, Object oldValue, Object newValue)  
-     removeAttribute(Object key)  
-     removeAttribute(Object key, Object value)  
-     containsAttribute(Object key)  
-     getAttributeKeys()   
- }  
## AttributeMap接口
Netty将这种看似Map的功能进一步抽象，形成了[AttributeMap](http://netty.io/4.0/api/io/netty/util/AttributeMap.html)接口：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicinterface AttributeMap {  
-     <T> Attribute<T> attr(AttributeKey<T> key);  
- }  
AttributeMap接口只有一个**attr()**方法，接收一个[AttributeKey](http://netty.io/4.0/api/io/netty/util/AttributeKey.html)类型的key，返回一个[Attribute](http://netty.io/4.0/api/io/netty/util/Attribute.html)类型的value。按照Javadoc，AttributeMap实现必须是线程安全的。AttributeMap内部结构看起来像下面这样：
![](https://img-blog.csdn.net/20140102093819875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 谁实现了AttributeMap接口？
答案是所有的[Channel](http://netty.io/4.0/api/io/netty/channel/Channel.html)和[ChannelHandlerContext](http://netty.io/4.0/api/io/netty/channel/ChannelHandlerContext.html)，如下面的类图所示：
![](https://img-blog.csdn.net/20140102095833296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## AttributeKey
AttributeKey有两个地方值得 一提。第一是AttributeKey是个**泛型**类，在我看来，这也是Netty相对于MINA的一处改进。在使用IoSession的时候，你必须进行强制类型转换：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- int userId = (Integer) ioSession.getAttribute("userId");  
但是使用AttributeMap却不需要：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- AttributeKey<Integer> KEY_USER_ID = AttributeKey.valueOf("userId");  
- int userId = channel.attr(KEY_USER_ID).get();  
第二是AttributeKey继承了[UniqueName](http://netty.io/4.0/api/io/netty/util/UniqueName.html)类，也就是说，对于每一个name，应该只有唯一一个AttributeKey与之对应。这一点看起来很奇怪，但是当知道[DefaultAttributeMap](http://netty.io/4.0/api/io/netty/util/DefaultAttributeMap.html)内部使用了IdentityHashMap的时候，就觉得合情合理。下面是与AttributeKey相关的类图，至于UniqueName如何保证name唯一，稍后介绍：
![](https://img-blog.csdn.net/20140102104058328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## UniqueName
UniqueName实际上是靠传入构造函数的一个map来保证name的唯一性：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- @Deprecated
- publicclass UniqueName implements Comparable<UniqueName> {  
- 
- privatestaticfinal AtomicInteger nextId = new AtomicInteger();  
- 
- privatefinalint id;  
- privatefinal String name;  
- 
- public UniqueName(ConcurrentMap<String, Boolean> map, String name, Object... args) {  
- if (map == null) {  
- thrownew NullPointerException("map");  
-         }  
- if (name == null) {  
- thrownew NullPointerException("name");  
-         }  
- if (args != null && args.length > 0) {  
-             validateArgs(args);  
-         }  
- 
- if (map.putIfAbsent(name, Boolean.TRUE) != null) {  
- thrownew IllegalArgumentException(String.format("'%s' is already in use", name));  
-         }  
- 
-         id = nextId.incrementAndGet();  
- this.name = name;  
-     }  
-     ...  
- }  
但是Javadoc说这个类存在跟类加载器相关的问题，所以被废弃了。AttributeKey继承了UniqueName，内部使用ConcurrentHashMap来保证name的唯一性：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicfinalclass AttributeKey<T> extends UniqueName {  
- 
- privatestaticfinal ConcurrentMap<String, Boolean> names = PlatformDependent.newConcurrentHashMap();  
- 
- @SuppressWarnings("deprecation")  
- publicstatic <T> AttributeKey<T> valueOf(String name) {  
- returnnew AttributeKey<T>(name);  
-     }  
- 
- @Deprecated
- public AttributeKey(String name) {  
- super(names, name);  
-     }  
- }  
## Attribute接口
Attribute接口除了有必须的**get()**和**set()**和**remove()**方法外，还有几个**原子**方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicinterface Attribute<T> {  
-     AttributeKey<T> key();  
-     T get();  
- void set(T value);  
-     T getAndSet(T value);  
-     T setIfAbsent(T value);  
-     T getAndRemove();  
- boolean compareAndSet(T oldValue, T newValue);  
- void remove();  
- }  
## DefaultAttributeMap
如前面的类图所示，DefaultAttributeMap实现了AttributeMap接口，[AbstractChannel](http://netty.io/4.0/api/io/netty/channel/AbstractChannel.html)和DefaultChannelHandlerContext通过继承DefaultAttributeMap也实现了AttributeMap接口。下面是DefaultAttributeMap的部分代码：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicclass DefaultAttributeMap implements AttributeMap {  
- 
- @SuppressWarnings("rawtypes")  
- privatestaticfinal AtomicReferenceFieldUpdater<DefaultAttributeMap, Map> updater =  
-             AtomicReferenceFieldUpdater.newUpdater(DefaultAttributeMap.class, Map.class, "map");  
- 
- // Initialize lazily to reduce memory consumption; updated by AtomicReferenceFieldUpdater above.
- @SuppressWarnings("UnusedDeclaration")  
- privatevolatile Map<AttributeKey<?>, Attribute<?>> map;  
- 
- @Override
- public <T> Attribute<T> attr(AttributeKey<T> key) {  
-         Map<AttributeKey<?>, Attribute<?>> map = this.map;  
- if (map == null) {  
- // Not using ConcurrentHashMap due to high memory consumption.
-             map = new IdentityHashMap<AttributeKey<?>, Attribute<?>>(2);  
- if (!updater.compareAndSet(this, null, map)) {  
-                 map = this.map;  
-             }  
-         }  
- 
- synchronized (map) {  
- @SuppressWarnings("unchecked")  
-             Attribute<T> attr = (Attribute<T>) map.get(key);  
- if (attr == null) {  
-                 attr = new DefaultAttribute<T>(map, key);  
-                 map.put(key, attr);  
-             }  
- return attr;  
-         }  
-     }  
-     ...  
- }  
可以看到：
- map是延迟创建的（为了减少内存消耗），更准确的说，map在**attr()**方法第一次被调用的时候创建
- map被声明为**volatile**，再加上**AtomicReferenceFieldUpdater.compareAndSet()**方法的使用，map的null判断和赋值这段代码可以不使用synchronized
- 内部使用的是**IdentityHashMap**，所以AttributeKey必须是唯一的，因为IdentityHashMap使用**==**而不是**equals()**方法来判断两个key是否相同
- attr()方法被调用时，如果key还没有关联attribute，会自动创建一个
## DefaultAttribute
最后，DefaultAttribute通过继承AtomicReference获得了原子操作能力：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17719333#)[copy](http://blog.csdn.net/zxhoo/article/details/17719333#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/138610/fork)
- publicclass DefaultAttributeMap implements AttributeMap {  
- privatestaticfinalclass DefaultAttribute<T> extends AtomicReference<T> implements Attribute<T> {  
- 
- privatestaticfinallong serialVersionUID = -2661411462200283011L;  
- 
- privatefinal Map<AttributeKey<?>, Attribute<?>> map;  
- privatefinal AttributeKey<T> key;  
- 
-         DefaultAttribute(Map<AttributeKey<?>, Attribute<?>> map, AttributeKey<T> key) {  
- this.map = map;  
- this.key = key;  
-         }  
- 
- @Override
- public AttributeKey<T> key() {  
- return key;  
-         }  
- 
- @Override
- public T setIfAbsent(T value) {  
- while (!compareAndSet(null, value)) {  
-                 T old = get();  
- if (old != null) {  
- return old;  
-                 }  
-             }  
- returnnull;  
-         }  
- 
- @Override
- public T getAndRemove() {  
-             T oldValue = getAndSet(null);  
-             remove0();  
- return oldValue;  
-         }  
- 
- @Override
- publicvoid remove() {  
-             set(null);  
-             remove0();  
-         }  
- 
- privatevoid remove0() {  
- synchronized (map) {  
-                 map.remove(key);  
-             }  
-         }  
-     }  
- }  
下面是DefaultAttributeMap的内部结构：
![](https://img-blog.csdn.net/20140102114114437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 结论
- **Channel**和**ChannelHandlerContext**都扩展了**AttributeMap**接口，因此每一个Channel和ChannelHandlerContext实例都可以像Map一样按照key来存取value
- AttributeMap实现必须是**线程安全**的，因此，**attr()**方法可以在任何线程里安全的调用
- AttributeKey必须是**唯一**的，因此最好定义成全局变量（比如**static final**类型）
- 默认的Attribute实现继承自AtomicReference，因此也是线程安全的
