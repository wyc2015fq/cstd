# 设计模式 —— 策略模式（Strategy Pattern） - Alex_McAvoy的博客 - CSDN博客





2019年03月23日 22:33:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79
个人分类：[设计模式](https://blog.csdn.net/u011815404/article/category/8856120)

所属专栏：[设计模式学习笔记](https://blog.csdn.net/column/details/35125.html)









# 【概述】

策略模式是一**可插入式**的模式，其针对一组算法，将每一个算法封装到具有共同接口的独立的类中，从而使得它们可以相互替换，使得算法可以在不影响到客户端的情况下发生变化，其本质是**分离算法，选择实现**。

使用情景：
- 同一个算法有很多不同的实现的情况时：使用策略模式来把这些 “不同的实现” 实现成为一个算法的类层次。
- 抽象一个定义了很多行为的类，且通过多个 if-else 语句来选择这些行为：使用策略模式来代替这些条件语句。 

# 【UML】

策略模式涉及到了三个角色：
- 环境角色：Context，持有一个 Strategy 类的引用（上下文对象），负责和具体的策略类交互。
- 抽象策略角色：Strategy，是一个抽象角色，通常由一个接口或抽象类实现，给出所有的具体策略类所需的接口。
- 具体策略角色：ConcreteStrategy，包装了相关的算法或行为。 

![](https://img-blog.csdnimg.cn/20190323220334355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【基本思想】

使用策略模式可以把行为和环境分割开来，环境类负责维持和查询行为类，各种算法则在具体策略类中提供。

策略模式是对算法的封装，将使用算法的责任和算法本身分割开，委派给不同的对象管理，使得具体算法从具体业务中独立，是**依靠多态实现的形同行为的不同实现**。

由于算法和环境独立开来，算法的增减、修改都不会影响环境和客户端，当出现新变化时，只需要实现新的策略类，并在客户端登记即可，而具体策略算法的选择，则交由客户端的上下文对象来选择。

#  【优缺点】

优点：
- 避免让客户端涉及到不必接触到的、复杂的、只与算法有关的数据。
- 避免使用难以维护的多重条件选择语句
- 便于更好的扩展

缺点：
- 将分支判断又放回到客户端，要改变需求算法时，还是要去更改客户端。 
- 客户端必须知道所有的策略类，并自行决定使用哪一个策略类，这就意味着客户端必须理解这些算法的区别，以便适时选择恰当的算法类。
- 增加了对象的数目
- 只适合扁平的算法结构。

# 【实现】

1.抽象策略角色

```java
public interface Strategy {//抽象策略接口
    public void operate(Context ctx);//针对不同的上下文对象进行具体策略的调用
}
```

2.具体策略角色

```java
public class ConcreteStrategyA implements Strategy {//具体策略1
    public void operate(Context ctx) {
        ...
    }
}
public class ConcreteStrategyB implements Strategy {//具体策略2
    public void operate(Context ctx) {
        ...
    }
}
```

 3.环境角色

```java
public class Context {//上下文对象的具体实现
    private String variable1=null;
    private String variable2=null;
    private Strategy strategy=null;

    public Context(String variable1,String variable2,Strategy strategy) {
        this.variable1=variable1;
        this.variable2=variable2;
        this.strategy=strategy;//选择相应策略
    }
    public void operate() {//返回当前context对象所选择的策略
        this.strategy.operate(this);
    }
}
```

4.客户端

```java
public class Program {
    public static void main(String[] args) {
        //读入变量
        Scanner input=new Scanner(System.in);
        String variable1=input.nextLine();
        String variable2=input.nextLine();

        Strategy strategyA=new ConcreteStrategyA();//具体策略A
        Strategy strategyB=new ConcreteStrategyB();//具体策略B
		
        Context ctxA=new Context(variable1,variable2,strategyA);//存入Context环境对象
        ctxA.operate();//具体操作
        Context ctxB=new Context(variable1,variable2,strategyB);//存入Context环境对象
        ctxB.operate();//具体操作
    }
}
```





