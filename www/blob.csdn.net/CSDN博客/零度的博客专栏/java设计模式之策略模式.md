# java设计模式之策略模式 - 零度的博客专栏 - CSDN博客
2016年05月27日 14:44:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：439标签：[设计模式																[Strategy](https://so.csdn.net/so/search/s.do?q=Strategy&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)
个人分类：[设计模式](https://blog.csdn.net/zmx729618/article/category/6246650)
#### 1、什么是策略模式？
        策略模式，又叫算法簇模式，就是定义了不同的算法族，并且之间可以互相替换，此模式让算法的变化独立于使用算法的客户。
#### 2、策略模式有什么好处？
        策略模式的好处在于你可以动态的改变对象的行为。
#### 3、设计原则
        设计原则是把一个类中经常改变或者将来可能改变的部分提取出来，作为一个接口（c++z中可以用虚类），然后在类中包含这个对象的实例，这样类的实例在运行时就可以随意调用实现了这个接口的类的行为。下面是一个例子。 
        策略模式属于对象行为型模式，主要针对一组算法，将每一个算法封装到具有共同接口的独立的类中，从而使得它们可以相互替换。策略模式使得算法可以在不影响 到客户端的情况下发生变化。通常，策略模式适用于当一个应用程序需要实现一种特定的服务或者功能，而且该程序有多种实现方式时使用。
#### 4、 策略模式中有三个对象：
     （1）       环境对象：该类中实现了对抽象策略中定义的接口或者抽象类的引用。
     （2）       抽象策略对象：它可由接口或抽象类来实现。
     （3）       具体策略对象：它封装了实现同不功能的不同算法。
       利用策略模式构建应用程序，可以根据用户配置等内容，选择不同有算法来实现应用程序的功能。具体的选择有环境对象来完成。采用这种方式可以避免由于使用条件语句而带来的代码混乱，提高应用程序的灵活性与条理性。
#### 5、应用场景举例：
       刘备要到江东娶老婆了，走之前诸葛亮给赵云（伴郎）三个锦囊妙计，说是按天机拆开能解决棘手问题，嘿，还别说，真解决了大问题，搞到最后是周瑜陪了夫人又折兵，那咱们先看看这个场景是什么样子的。
先说说这个场景中的要素：三个妙计，一个锦囊，一个赵云，妙计是亮哥给的，妙计放在锦囊里，俗称就是锦囊妙计嘛，那赵云就是一个干活的人，从锦囊取出妙计，执行，然后获胜。用java程序怎么表现这些呢？
那我们先来看看图？
![](http://dl.iteye.com/upload/attachment/350678/06a4b9d7-a392-3a49-a1ff-6b1c2342acdb.jpg)
####  6、代码示例
1、策略接口IStrategy
```
package zmx.designmode.test.strategy;
/**
 * 首先定义一个策略接口，这是诸葛亮老人家给赵云的三个锦囊妙计的接口。   
 * @author zhangwenchao
 *
 */
public interface IStrategy {  
    //每个锦囊妙计都是一个可执行的算法。  
    public void operate();  
  
}
```
2、三个不同的策略实现（三个锦囊妙计）
```
package zmx.designmode.test.strategy;
/**
 * 妙计一：找乔国老帮忙，使孙权不能杀刘备。
 * @author zhangwenchao
 *
 */
public class BackDoor implements IStrategy{
	@Override
	public void operate() {
		System.out.println("找乔国老帮忙，让吴国太给孙权施压，使孙权不能杀刘备！"); 	
	}
}
```
```
package zmx.designmode.test.strategy;
/**
 * 妙计二：求吴国太开个绿灯，放行
 * @author zhangwenchao
 *
 */
public class GivenGreenLight implements IStrategy{
	@Override
	public void operate() {
		
		System.out.println("求吴国太开个绿灯，放行！");  
		
	}
}
```
```
package zmx.designmode.test.strategy;
/**
 * 孙夫人断后，挡住追兵。 
 * @author zhangwenchao
 *
 */
public class BlackEnemy  implements IStrategy{
	@Override
	public void operate() {
		  System.out.println("孙夫人断后，挡住追兵...");  
		
	}
}
```
3、环境对象（锦囊）
```
package zmx.designmode.test.strategy;
/**
 * 使用妙计的环境
 * @author zhangwenchao
 *
 */
public class Context {
	
    private IStrategy strategy;  
    //构造函数，要你使用哪个妙计  
    public Context(IStrategy strategy){  
        this.strategy = strategy;  
    }  
      
    public void operate(){  
        this.strategy.operate();  
    }  
}
```
4、测试类（赵云使用）
```
package zmx.designmode.test.strategy;
public class ZhaoYun {
	
	public static void main(String[] args) {
           Context context = null;  
          
        //刚到吴国的时候拆开第一个  
        System.out.println("----------当到大吴国的时候，拆第一个---------------");  
        context = new Context(new BackDoor());  
        context.operate();//拆开执行  
        System.out.println();  
         
        //当刘备乐不思蜀时，拆开第二个  
        System.out.println("----------当刘备乐不思蜀，拆第二个了---------------");  
        context = new Context(new GivenGreenLight());  
        context.operate();//拆开执行  
        System.out.println();  
          
        //孙权的小追兵了，咋办？拆开第三个锦囊  
        System.out.println("----------当孙权的追兵来了，拆开第三个--------------");  
        context = new Context(new BlackEnemy());  
        context.operate();//拆开执行  
        System.out.println(); 
	}
}
```
5、测试结果
```
----------当到大吴国的时候，拆第一个---------------
找乔国老帮忙，让吴国太给孙权施加压力，使孙权不能杀刘备...
----------当刘备乐不思蜀，拆第二个了---------------
求吴国太开个绿灯，放行！
----------当孙权的追兵来了，拆开第三个--------------
孙夫人断后，挡住追兵...
```
     这就是策略模式，高内聚低耦合的特点也表现出来了，还有一个就是扩展性，也就是OCP原则，策略类可以继续添加下去！

