# JAVA设计模式（抽象工厂模式） - DEVELOPER - CSDN博客





2017年04月12日 12:22:07[学术袁](https://me.csdn.net/u012827205)阅读数：279标签：[JAVA设计模式](https://so.csdn.net/so/search/s.do?q=JAVA设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









今天开始，从新整理一下闹心java的设计模式；其实我不想去整理它，但是过一段时间老是忘记，因为。。。。不经常手敲！！

闲话少喷！！首先了解一下什么是“抽象工厂模式”，他是干什么用的哦？？他用来创建一组相关或者相互依赖的对象。

抽象工厂模式都适用在怎样的使用场景呢？当需要创建的对象是一系列相互关联或相互依赖的产品族时，便可以使用抽象工厂模式。说的更明白一点，就是一个继承体系中，如果存在着多个等级结构（即存在着多个抽象类），并且分属各个等级结构中的实现类之间存在着一定的关联或者约束，就可以使用抽象工厂模式。假如各个等级结构中的实现类之间不存在关联或约束，则使用多个独立的工厂来对产品进行创建，则更合适一点。


在了解什么是抽象模式之前，我们先来了解一下什么是“产品族”、“产品结构”。因为我们在学习她的时候，会在很多地方看到。就像玩王者荣耀手游一样，我就喜欢使用大乔，嘿嘿，跑题了！！好了， 那 “产品族”、“产品结构”到底什么意思？？简单的从网上整合了一下：所谓的产品族，是指位于不同产品等级结构中功能相关联的产品组成的家族。抽象工厂模式所提供的一系列产品就组成一个产品族；而工厂方法提供的一系列产品称为一个等级结构。

接下来，我不画图，我只上代码解释！！画图太麻烦！

先了解一下代码思路：抽象工厂模式，比如说：首先要有一个IXimiFactory工厂（产品族），工厂生产 IXimiProduct产品（产品等级结构）；完成这些工作与过程需要具体的实现工厂、类和方法去实现完成；

抽象工厂（产品族）：

```java
public interface IXimiFactory {
    //获取生产电脑的对象
    public IXimiComputor getComputerInstance();

    public IXimiPhone getPhoneInstance();

    public IXimiTV getTVInstance();

    public IXimiLight getLightInstance();
}
```




具体工厂类（产品族）：

```java
public class XimiFactory implements IXimiFactory {
    @Override
    public IXimiComputor getComputerInstance() {
        return new XimiComputor() ;
    }

    @Override
    public IXimiPhone getPhoneInstance() {
        return new XimiPhone();
    }

    @Override
    public IXimiTV getTVInstance() {
        return new XimiTV();
    }

    @Override
    public IXimiLight getLightInstance() {
        return new XimiLight();
    }
}
```




工厂中要生成的抽象产品（三个产品结构）：



```java
//Ximi灯泡
public interface IXimiLight {
    public void productRedLight();
    public void productBlueLight();
    public void productSmartLight();
    public void productCup();
}
```


```java
//Ximi手机
public interface IXimiPhone {
    public void productPhone();
}
```


```java
//Ximi电视
public interface IXimiTV {
    public void productTV();
}
```


工厂中要具体进行生产的产品（三个产品结构）：
```java
//Ximi灯泡
public class XimiLight implements IXimiLight{
    @Override
    public void productRedLight() {

    }

    @Override
    public void productBlueLight() {

    }

    @Override
    public void productSmartLight() {

    }

    @Override
    public void productCup() {

    }
}
```

```java
//Ximi手机
public class XimiPhone implements IXimiPhone{

    @Override
    public void productPhone() {

    }
}
```

```java
//Ximi电视
public class XimiTV implements IXimiTV{

    @Override
    public void productTV() {

    }
}
```


然后，具体怎么去生产，我们不用管！只要调用封装的方法实现即可。这样就完成了使用抽象模式实现的功能！





```java
public class MainTest {
    public static void main(String[] args){
        XimiFactory ximiFactory = new XimiFactory();
        ximiFactory.getComputerInstance().productComputor();
        ximiFactory.getPhoneInstance().productPhone();
        ximiFactory.getTVInstance().productTV();
        XimiLight light = (XimiLight) ximiFactory.getLightInstance();
        light.productBlueLight();
        light.productRedLight();
        light.productSmartLight();
        light.productCup();

    }
}
```


附图理解：![](https://img-blog.csdn.net/20170412122504738)









