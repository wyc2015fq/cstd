
# Java Web并发访问的线程安全问题 - wangzhiguo9261的博客 - CSDN博客


2018年02月27日 16:17:35[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：47个人分类：[javaweb](https://blog.csdn.net/wangzhiguo9261/article/category/7470762)



## 一、Servlet的线程安全问题
Java web服务器下，每个Servlet只有一个实例（即单例模式），导致有多个Http请求发给一个Servlet实例，每个请求是一个线程。如果Servlet有类变量或实例变量，那么该变量就变成了共享资源，当多个线程访问操作该变量时，就有可能存在安全隐患。例如，当一个Http请求在访问该变量的时候，另一个Http请求可能在修改它的值。
解决方法：在Servlet中不要定义类变量或实例变量，如果非定义不可，就需要为操作该变量的所有方法加synchronized修饰符，进行同步控制。
注意：
1. 在Servlet中可以定义常量
2. 如果定义的变量只用查询操作，可以不加同步控制
3. 被Servlet调用的类中(如值对象、领域模型类)中是否可以安全的使用实例变量？如果你在每次方法调用时新建一个对象，再调用它们的方法，则不存在同步问题。因为它们不是多个线程共享的资源，只有共享的资源才需要同步，而Servlet的实例对于多个线程是共享的。
## 二、Struts并发访问的线程安全问题
同Servlet一样，Struts中的Action也是单例的，所以多Http请求访问时，也存在线程安全问题。
解决方法：同Servlet
## 三、Spring并发访问的线程安全性问题
和Struts一样，Spring的Controller默认是Singleton的（非线程安全的），这意味着每个request过来，系统都会用原有的instance去处理，这样导致了两个结果：一是我们不用每次创建Controller，二是减少了对象创建和垃圾收集的时间；由于只有一个Controller的instance，当多个线程调用它的时候，它里面的instance变量就不是线程安全的了，会发生窜数据的问题。
当然大多数情况下，我们根本不需要考虑线程安全的问题，除非在Controller中声明了实例变量。因此，我们在使用spring mvc 的contrller时，应避免在controller中定义类变量或实例变量。
如：
```python
public
```
```python
class
```
```python
Controller
```
```python
extends
```
```python
AbstractCommandController
```
```python
{
```
```python
......
```
```python
protected
```
```python
Company company;
```
```python
protected
```
```python
ModelAndView
```
```python
handle
```
```python
(HttpServletRequest                  request,HttpServletResponse response,
            Object command,BindException errors)
```
```python
throws
```
```python
Exception {
    company = ................;
    }
}
```
1
2
3
4
5
6
7
8
在这里有声明一个变量company，这里就存在并发线程安全的问题。
如果控制器是使用单例形式，且controller中有一个私有的变量a,所有请求到同一个controller时，使用的a变量是共用的，即若是某个请求中修改了这个变量a，则在别的请求中能够读到这个修改的内容。。
有几种解决方法：
1、在Controller中使用ThreadLocal变量
2、在spring配置文件Controller中声明 scope=”prototype”，每次都创建新的controller。
注意：在Spring mvc中，如果dao、service也是单例的，要避免在Dao和Service类中建立类变量或实例变量。
## 四、SpringMVC多线程环境中如何保证对象的安全性？
[http://linapex.blog.163.com/blog/static/189237516201381493441799/](http://linapex.blog.163.com/blog/static/189237516201381493441799/)
总结：多线程环境下如果访问单例对象，当对象内部有类变量或实例变量时，就可能存在安全性问题。
解决方法：
1.对操作共享变量的所用方法进行同步控制；
2.同步共享变量，例如Collections.synchronizedMap()可以同步共享的Map。
3.使用同步对象，例如ConcurrentMap、AtomicInteger等对象都是线程安全的，使用AtomicInteger可以统计系统的并发量。

