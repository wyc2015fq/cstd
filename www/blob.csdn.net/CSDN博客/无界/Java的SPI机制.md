
# Java的SPI机制 - 无界 - CSDN博客

2017年03月27日 21:04:54[21aspnet](https://me.csdn.net/21aspnet)阅读数：2526个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



Dubbo等框架使用到必须掌握。
java.sql.Driver 是 Spi，com.mysql.jdbc.Driver 是 Spi 实现，其它的都是 Api。

```python
package org.hadoop.java;
public interface IService {
    public String sayHello();      
    public String getScheme();
}
```

```python
package org.hadoop.java;
public class HDFSService implements IService {
    @Override
    public String sayHello() {
        return "Hello HDFS!!";
    }
    @Override
    public String getScheme() {
        return "hdfs";
    }
}
```

```python
package org.hadoop.java;
public class LocalService implements IService {
    @Override
    public String sayHello() {
        return "Hello Local!!";
    }
    @Override
    public String getScheme() {
        return "local";
    }
}
```

```python
package org.hadoop.java;
import java.util.ServiceLoader;

public class ServiceLoaderTest {
    public static void main(String[] args) {  
        //need to define related class full name in /META-INF/services/....  
        ServiceLoader<IService> serviceLoader = ServiceLoader.load(IService.class);  
        for (IService service : serviceLoader) {
            System.out.println(service.getScheme()+"="+service.sayHello());  
        }  
    }  
}
```
需要在项目的\src\main\resources\下增加services目录，然后再增加一个文件这个文件是前面的接口的包名+接口名，例如：org.hadoop.java.IService
![](https://img-blog.csdn.net/20170327204434656)

整个项目
![](https://img-blog.csdn.net/20170327204453922)

输出：

```python
------------------------------------------------------------------------
Building SPITest 1.0-SNAPSHOT
------------------------------------------------------------------------
--- exec-maven-plugin:1.2.1:exec (default-cli) @ SPITest ---
hdfs=Hello HDFS!!
local=Hello Local!!
------------------------------------------------------------------------
BUILD SUCCESS
------------------------------------------------------------------------
Total time: 0.570s
Finished at: Mon Mar 27 20:45:01 CST 2017
Final Memory: 5M/123M
------------------------------------------------------------------------
```
参考：设计原则：[小议 SPI 和 API](http://www.cnblogs.com/happyframework/p/3325560.html)
[Java spi机制浅谈](http://singleant.iteye.com/blog/1497259)

[java中的SPI机制](http://www.cnblogs.com/javaee6/p/3714719.html)
1 SPI机制简介
SPI的全名为Service Provider Interface.大多数开发人员可能不熟悉，因为这个是针对厂商或者插件的。在java.util.ServiceLoader的文档里有比较详细的介绍。简单的总结下java spi机制的思想。我们系统里抽象的各个模块，往往有很多不同的实现方案，比如日志模块的方案，xml解析模块、jdbc模块的方案等。面向的对象的设计里，我们一般推荐模块之间基于接口编程，模块之间不对实现类进行硬编码。一旦代码里涉及具体的实现类，就违反了可拔插的原则，如果需要替换一种实现，就需要修改代码。为了实现在模块装配的时候能不在程序里动态指明，这就需要一种服务发现机制。 java spi就是提供这样的一个机制：为某个接口寻找服务实现的机制。有点类似IOC的思想，就是将装配的控制权移到程序之外，在模块化设计中这个机制尤其重要。
2 SPI具体约定
java spi的具体约定为:当服务的提供者，提供了服务接口的一种实现之后，在jar包的META-INF/services/目录里同时创建一个以服务接口命名的文件。该文件里就是实现该服务接口的具体实现类。而当外部程序装配这个模块的时候，就能通过该jar包META-INF/services/里的配置文件找到具体的实现类名，并装载实例化，完成模块的注入。 基于这样一个约定就能很好的找到服务接口的实现类，而不需要再代码里制定。jdk提供服务实现查找的一个工具类：java.util.ServiceLoader

[java.util.ServiceLoader使用](http://blog.csdn.net/kokojhuang/article/details/8273303)
今天在看Hadoop源代的时候发现，在FileSystem中用到了Java.util.ServiceLoader这个类来从配置文件中加载子类或者接口的实现类。以前从来没有使用过这个类，进去大概看了一下具体的实现。主要是从META-INF/services这个目录下的配置文件加载给定接口或者基类的实现，ServiceLoader会根据给定的类的full name来在META-INF/services下面找对应的文件，在这个文件中定义了所有这个类的子类或者接口的实现类，返回一个实例。


