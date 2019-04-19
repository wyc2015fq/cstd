# Java的SPI机制浅析与简单示例 - 零度的博客专栏 - CSDN博客
2016年11月22日 14:33:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：5391
### 一、SPI机制
        这里先说下SPI的一个概念，SPI英文为Service Provider Interface单从字面可以理解为Service提供者接口，正如从SPI的名字去理解SPI就是Service提供者接口；我对SPI的定义：提供给服务提供厂商与扩展框架功能的开发者使用的接口。
       在我们日常开发的时候都是对问题进行抽象成Api然后就提供各种Api的实现，这些Api的实现都是封装与我们的Jar中或框架中的虽然当我们想要提供一种Api新实现时可以不修改原来代码只需实现该Api就可以提供Api的新实现，但我们还是生成新Jar或框架（虽然可以通过在代码里扫描某个目录已加载Api的新实现，但这不是Java的机制，只是hack方法），而通过Java SPI机制我们就可以在不修改Jar包或框架的时候为Api提供新实现。
     很多框架都使用了java的SPI机制，如java.sql.Driver的SPI实现（[MySQL](http://lib.csdn.net/base/14)驱动、oracle驱动等）、common-logging的日志接口实现、dubbo的扩展实现等等框架；
SPI机制的约定：
1)         在META-INF/services/目录中创建以接口全限定名命名的文件该文件内容为Api具体实现类的全限定名
2)         使用ServiceLoader类动态加载META-INF中的实现类
3)         如SPI的实现类为Jar则需要放在主程序classPath中
4)         Api具体实现类必须有一个不带参数的构造方法
![](http://images.cnitblog.com/blog2015/84976/201503/081719062588772.jpg)
SPI机制结构图
### 二、SPI机制示例
![](https://img-blog.csdn.net/20161122142515602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                        实例结构图
IOperation接口：
```
package org.nercita.ltxx.spiTest;
/**
 * this is a Interface for two data 
 * @author zhangwenchao
 *
 */
public interface IOperation {
	 public int operation(int numberA, int numberB);
}
```
SPI接口的实现类：PlusOperationImpl
```
package org.nercita.ltxx.spiTest;
public class PlusOperationImpl implements IOperation {
    public int operation(int numberA, int numberB) {
        return numberA + numberB;
    }
}
```
SPI接口的实现类：DivisionOperationImpl
```
package org.nercita.ltxx.spiTest;
public class DivisionOperationImpl implements IOperation{
    public int operation(int numberA, int numberB) {
        return numberA / numberB;
    }
}
```
META-INF/Services目录中的文件：
文件名：org.nercita.ltxx.spiTest.IOperation
内容：
       org.nercita.ltxx.spiTest.DivisionOperationImpl
       org.nercita.ltxx.spiTest.PlusOperationImpl
测试工程引入上述jar包，测试Main类如下：
```
package org.nercita.itxx.spiClient;
import java.util.Iterator;
import java.util.ServiceLoader;
import org.nercita.ltxx.spiTest.DivisionOperationImpl;
import org.nercita.ltxx.spiTest.IOperation;
import org.nercita.ltxx.spiTest.PlusOperationImpl;
public class Main {
	
	public static void main(String[] args) {
        IOperation plus = new PlusOperationImpl();
        IOperation division = new DivisionOperationImpl();
        System.out.println(plus.operation(6, 3));
        System.out.println(division.operation(6, 3));
        ServiceLoader<IOperation> operations = ServiceLoader.load(IOperation.class);
        Iterator<IOperation> operationIterator = operations.iterator();
        System.out.println("classPath:"+System.getProperty("java.class.path"));
        while (operationIterator.hasNext()) {
            IOperation operation = operationIterator.next();
            System.out.println(operation.operation(6, 3));
        }
	}
}
```
运行结果：
9
2
classPath:D:\Workspaces\projects\spiClient\bin;D:\Workspaces\projects\spiClient\lib\spiTest-0.0.1-SNAPSHOT.jar
2
9
