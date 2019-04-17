# 设计模式 —— 简单工厂模式（Simple Factory） - Alex_McAvoy的博客 - CSDN博客





2019年03月23日 21:35:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：23
所属专栏：[设计模式学习笔记](https://blog.csdn.net/column/details/35125.html)









# 【概述】

简单工厂模式根据提供给它的数据，动态的返回几个可能类中的一个类的实例，通常它返回的类都有一个公共的父类和公共的方法，其本质是**选择实现**。

# 【UML】

![](https://img-blog.csdnimg.cn/20190323211157660.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

简单工厂模式涉及到了三个角色：
- 工厂类角色：Factory，工厂类**在客户端的直接控制**下，通过 createProduct() 方法创建产品对象
- 具体产品角色：Product，定义工厂**具体加工**出的对象。
- 抽象产品角色：IProduct，定义工厂创建的**对象的父类或共同拥有的类、抽象类或接口**。

# 【优缺点】

优点：
- 工厂类含有必要的判断逻辑，可以决定在什么时候创建哪一个产品类的实例。
- 客户端可以免除直接创建产品对象的责任，而仅仅"消费"产品，从而实现了对责任的分割。

缺点：
- 当产品有复杂的多层等级结构时，工厂类只有自己，由于工厂类集中了所有产品创建逻辑，一旦不能正常工作，整个系统都要受到影响。
- 系统扩展困难，一旦添加新产品就不得不修改工厂逻辑，有可能造成工厂逻辑过于复杂。
- 通常使用静态工厂方法，使得无法由子类继承，造成工厂角色无法形成基于继承的等级结构。 

# 【实现】

```java
public class SimpleFactory {
    public Operation createOperate(String operate) {//返回实例化后的抽象产品角色
        Operation oper=null;//抽象产品角色
        switch(operate) {
            case "operate1":{
                oper=new Operation1();//具体产品1
                break;
            }
            case "operate2":{
                oper=new Operation2();//具体产品2
                break;
            }
            case "operate3":{
                oper=new Operation3();//具体产品3
                break;
            }
			...
        }
        return oper;
    }
}
```





