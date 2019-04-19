# 【Java】内存问题——内存溢出 - Big Smile - CSDN博客
2018年08月31日 20:42:16[王啸tr1912](https://me.csdn.net/tr1912)阅读数：241标签：[java																[内存](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
# 模拟内存溢出
        我们通过自己写代码，模拟一下两种内存溢出的情况，一种是堆内存溢出，一种是栈内存溢出。
## 堆内存溢出
代码：
```java
public class Heap
{
    public static void main(String[] args)
    {
        ArrayList list=new ArrayList();
        while(true)
        {
            list.add(new Heap());
        }
    }
}
```
        代码的意思就是，定义了一个ArrayList，然后往里面不停的插入的list元素，直到内存溢出为止，由于Arraylist类型是引用类型的变量，所以这个类的实例就是放在堆中的，我们不停的往里面放入元素的话，最终肯定会造成堆溢出。
溢出报错为：java.lang.OutOfMemoryError: Java heap space
## 栈内存溢出
代码：
```java
public class Stack
{
    public static void main(String[] args)
    {
        new Stack().test();
    }
    public void test()
    {
        test();
    }
}
```
我们可以看出来，这是一个没有出口的递归函数，最终，每次递归的循环都在栈里面进行记录，会造成栈的溢出。
# 实际
实际上，我们的内存溢出，不仅仅只是这一层的关系，还有很多java封装了的新的错误在这里出现，下面我们列出几种常见的内存溢出错误提示：
-  tomcat:java.lang.OutOfMemoryError: PermGen space
- tomcat:java.lang.OutOfMemoryError: Java heap space
- weblogic:Root cause of ServletException java.lang.OutOfMemoryError
- resin:java.lang.OutOfMemoryError
- java:java.lang.OutOfMemoryError
这些异常常见的引发的原因是什么呢？
- 内存中加载的数据量过于庞大，如一次从数据库取出过多数据；
- 集合类中有对对象的引用，使用完后未清空，使得JVM不能回收；
- 代码中存在死循环或循环产生过多重复的对象实体；
- 使用的第三方软件中的BUG；
- 启动参数内存值设定的过小；
## **解决方案：**
### 1、优化程序，释放垃圾
         主要包括避免死循环，应该及时释放种资源：内存, 数据库的各种连接，防止一次载入太多的数据。导致java.lang.OutOfMemoryError的根本原因是程序不健壮。因此，从根本上解决Java内存溢出的唯一方法就是修改程序，及时地释放没用的对象，释放内存空间。 遇到该错误的时候要仔细检查程序。
### 2、Java代码导致OutOfMemoryError错误的解决
**需要重点排查以下几点：**
    1. 检查代码中是否有死循环或递归调用。
    2. 检查是否有大循环重复产生新对象实体。
    3. 检查对数据库查询中，是否有一次获得全部数据的查询。一般来说，如果一次取十万条记录到内存，就可能引起内存溢出。这个问题比较隐蔽，在上线前，数据库中数据较少，不容易出问题，上线后，数据库中数据多了，一次查询就有可能引起内存溢出。因此对于数据库查询尽量采用分页的方式查询。
    4. 检查List、MAP等集合对象是否有使用完后，未清除的问题。List、MAP等集合对象会始终存有对对象的引用，使得这些对象不能被GC回收。
### 3、tomcat中java.lang.OutOfMemoryError: PermGen space异常处理 
        PermGen space的全称是Permanent Generation space,是指内存的永久保存区域,这块内存主要是被JVM存放Class和Meta信息的,Class在被Loader时就会被放到PermGen space中, 它和存放类实例(Instance)的Heap区域不同,GC(Garbage Collection)不会在主程序运行期对PermGen space进行清理，所以如果你的应用中有很多CLASS的话,就很可能出现PermGen space错误, 这种错误常见在web服务器对JSP进行pre compile的时候。如果你的WEB APP下都用了大量的第三方jar, 其大小超过了jvm默认的大小(4M)那么就会产生此错误信息了。
**解决方法：** 手动设置MaxPermSize大小修改TOMCAT_HOME/bin/catalina.sh在
`echo "Using CATALINA_BASE:   $CATALINA_BASE"`
上面加入以下行：
```
JAVA_OPTS="-server -XX:PermSize=64M -XX:MaxPermSize=128m
```
**建议：**将相同的第三方jar文件移置到tomcat/shared/lib目录下，这样可以达到减少jar 文档重复占用内存的目的。
