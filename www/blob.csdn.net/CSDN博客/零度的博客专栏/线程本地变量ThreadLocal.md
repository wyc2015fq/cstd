# 线程本地变量ThreadLocal - 零度的博客专栏 - CSDN博客
2017年03月27日 13:50:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：314
首先说明ThreadLocal存放的值是线程内共享的，线程间互斥的，主要用于线程内共享一些数据，避免通过参数来传递，这样处理后，能够优雅的解决一些实际问题，比如[hibernate](http://lib.csdn.net/base/javaee)中的OpenSessionInView，就是使用ThreadLocal保存Session对象，还有我们经常用ThreadLocal存放Connection，代码如：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/15732053#)[copy](http://blog.csdn.net/ghsau/article/details/15732053#)
[print](http://blog.csdn.net/ghsau/article/details/15732053#)[?](http://blog.csdn.net/ghsau/article/details/15732053#)
- /**
-  * 数据库连接管理类
-  * @author 爽
-  *
-  */
- publicclass ConnectionManager {  
- 
- /** 线程内共享Connection，ThreadLocal通常是全局的，支持泛型 */
- privatestatic ThreadLocal<Connection> threadLocal = new ThreadLocal<Connection>();  
- 
- publicstatic Connection getCurrConnection() {  
- // 获取当前线程内共享的Connection
-         Connection conn = threadLocal.get();  
- try {  
- // 判断连接是否可用
- if(conn == null || conn.isClosed()) {  
- // 创建新的Connection赋值给conn(略)
- // 保存Connection
-                 threadLocal.set(conn);  
-             }  
-         } catch (SQLException e) {  
- // 异常处理
-         }  
- return conn;  
-     }  
- 
- /**
-      * 关闭当前数据库连接
-      */
- publicstaticvoid close() {  
- // 获取当前线程内共享的Connection
-         Connection conn = threadLocal.get();  
- try {  
- // 判断是否已经关闭
- if(conn != null && !conn.isClosed()) {  
- // 关闭资源
-                 conn.close();  
- // 移除Connection
-                 threadLocal.remove();  
-                 conn = null;  
-             }  
-         } catch (SQLException e) {  
- // 异常处理
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       这样处理的好处：
- 统一管理Connection；
- 不需要显示传参Connection，代码更优雅；
- 降低耦合性。
       ThreadLocal有四个方法，分别为：
### initialValue
protected [T](http://blog.csdn.net/ghsau/article/details/15732053)**initialValue**()返回此线程局部变量的当前线程的初始值。最多在每次访问线程来获得每个线程局部变量时调用此方法一次，即线程第一次使用 [`get()`](http://blog.csdn.net/ghsau/article/details/15732053) 方法访问变量的时候。如果线程先于 get 方法调用 [`set(T)`](http://blog.csdn.net/ghsau/article/details/15732053) 方法，则不会在线程中再调用 initialValue 方法。
该实现只返回 null；如果程序员希望将线程局部变量初始化为 null 以外的某个值，则必须为 ThreadLocal 创建子类，并重写此方法。通常，将使用匿名内部类。initialValue 的典型实现将调用一个适当的构造方法，并返回新构造的对象。
- **返回：**
返回此线程局部变量的初始值
### get
public [T](http://blog.csdn.net/ghsau/article/details/15732053)**get**()返回此线程局部变量的当前线程副本中的值。如果这是线程第一次调用该方法，则创建并初始化此副本。
- **返回：**
此线程局部变量的当前线程的值
### set
public void **set**([T](http://blog.csdn.net/ghsau/article/details/15732053) value)将此线程局部变量的当前线程副本中的值设置为指定值。许多应用程序不需要这项功能，它们只依赖于 [`initialValue()`](http://blog.csdn.net/ghsau/article/details/15732053) 方法来设置线程局部变量的值。
- **参数：**
`value` - 存储在此线程局部变量的当前线程副本中的值。
### remove
public void **remove**()移除此线程局部变量的值。这可能有助于减少线程局部变量的存储需求。如果再次访问此线程局部变量，那么在默认情况下它将拥有其 initialValue。
       很多人对ThreadLocal存在一定的误解，说ThreadLocal中有一个全局的Map，set时执行map.put(Thread.currentThread(), value)，get和remove时也同理，但SUN的大师们是否是如此实现的，我们只能去看源码了。
       set方法：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/15732053#)[copy](http://blog.csdn.net/ghsau/article/details/15732053#)
[print](http://blog.csdn.net/ghsau/article/details/15732053#)[?](http://blog.csdn.net/ghsau/article/details/15732053#)
- /**
-  * Sets the current thread's copy of this thread-local variable
-  * to the specified value.  Most subclasses will have no need to
-  * override this method, relying solely on the {@link #initialValue}
-  * method to set the values of thread-locals.
-  *
-  * @param value the value to be stored in the current thread's copy of
-  *        this thread-local.
-  */
- publicvoid set(T value) {  
- // 获取当前线程对象
-     Thread t = Thread.currentThread();  
- // 获取当前线程本地变量Map
-     ThreadLocalMap map = getMap(t);  
- // map不为空
- if (map != null)  
- // 存值
-         map.set(this, value);  
- else
- // 创建一个当前线程本地变量Map
-         createMap(t, value);  
- }  
- 
- /**
-  * Get the map associated with a ThreadLocal. Overridden in
-  * InheritableThreadLocal.
-  *
-  * @param  t the current thread
-  * @return the map
-  */
- ThreadLocalMap getMap(Thread t) {  
- // 获取当前线程的本地变量Map
- return t.threadLocals;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       这里注意，ThreadLocal中是有一个Map，但这个Map不是我们平时使用的Map，而是ThreadLocalMap，ThreadLocalMap是ThreadLocal的一个内部类，不对外使用的。当使用ThreadLocal存值时，首先是获取到当前线程对象，然后获取到当前线程本地变量Map，最后将当前使用的ThreadLocal和传入的值放到Map中，也就是说ThreadLocalMap中存的值是[ThreadLocal对象, 
 存放的值]，这样做的好处是，每个线程都对应一个本地变量的Map，所以**一个线程可以存在多个线程本地变量**。
       get方法：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/15732053#)[copy](http://blog.csdn.net/ghsau/article/details/15732053#)
[print](http://blog.csdn.net/ghsau/article/details/15732053#)[?](http://blog.csdn.net/ghsau/article/details/15732053#)
- /**
-  * Returns the value in the current thread's copy of this
-  * thread-local variable.  If the variable has no value for the
-  * current thread, it is first initialized to the value returned
-  * by an invocation of the {@link #initialValue} method.
-  *
-  * @return the current thread's value of this thread-local
-  */
- public T get() {  
-     Thread t = Thread.currentThread();  
-     ThreadLocalMap map = getMap(t);  
- if (map != null) {  
-         ThreadLocalMap.Entry e = map.getEntry(this);  
- if (e != null)  
- return (T)e.value;  
-     }  
- // 如果值为空，则返回初始值
- return setInitialValue();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://static.blog.csdn.net/images/save_snippets.png)
       有了之前set方法的分析，get方法也同理，需要说明的是，如果没有进行过set操作，那从ThreadLocalMap中拿到的值就是null，这时get方法会返回初始值，也就是调用initialValue()方法，ThreadLocal中这个方法默认返回null。当我们有需要第一次get时就能得到一个值时，可以继承ThreadLocal，并且覆盖initialValue()方法。
