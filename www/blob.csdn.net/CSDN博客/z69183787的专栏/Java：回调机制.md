# Java：回调机制 - z69183787的专栏 - CSDN博客
2013年12月27日 20:44:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：620
**1.  什么是回调函数**
    回调函数（callback Function），顾名思义，用于回调的函数。  回调函数只是一个功能片段，由用户按照回调函数调用约定来实现的一个函数。回调函数是一个工作流的一部分，由工作流来决定函数的调用（回调）时机。回调函数包含下面几个特性： 
    1、属于工作流的一个部分；
    2、必须按照工作流指定的调用约定来申明（定义）；
    3、他的调用时机由工作流决定，回调函数的实现者不能直接调用回调函数来实现工作流的功能；    
**2. 回调机制**    回调机制是一种常见的设计模型，他把工作流内的某个功能，按照约定的接口暴露给外部使用者，为外部使用者提供数据，或要求外部使用者提供数据。
=======================================================
**java回调机制：**
软件模块之间总是存在着一定的接口，从调用方式上，可以把他们分为三类：同步调用、回调和异步调用。
同步调用：一种阻塞式调用，调用方要等待对方执行完毕才返回，它是一种单向调用；
回      调：一种双向调用模式，也就是说，被调用方在接口被调用时也会调用对方的接口；
异步调用：一种类似消息或事件的机制，不过它的调用方向刚好相反，接口的服务在收到某种讯息或发生某种事件时，会主动通知客户方（即调用客户方的接口）。
回调和异步调用的关系非常紧密：使用回调来实现异步消息的注册，通过异步调用来实现消息的通知。
========================================================
用Java里的例子：
package callbackexample; public interface ICallBack { //需要回调的方法 public void postExec(); }
另外的一个类：
package callbackexample; public class FooBar { //组合聚合原则 private ICallBack callBack; public void setCallBack(ICallBack callBack) { this.callBack = callBack; doSth(); } public void doSth() { callBack.postExec(); } }
第二个类在测试类里面，是一个匿名类：
package callbackexample; public class Test { public static void main(String[] args) { FooBar foo = new FooBar(); foo.setCallBack(new ICallBack() { public void postExec() { System.out.println("在Test类中实现但不能被Test的对象引用,而由FooBar对象调用"); } }); } }
上诉的代码：
  1.两个类：匿名类和FooBar
  2.匿名类实现接口ICallBack(在test测试的main方法中用匿名类的形式实现)
  3.FooBar 拥有一个参数为ICallBack接口类型的函数setCallBack(ICallBack   o)  
  4.匿名类运行时调用FooBar中setCallBack函数,以自身传入参数  
  5.FooBar已取得匿名类，就可以随时回调匿名类中所实现的ICallBack接口中的方法
 ==================================
1。首先回调方法的概念与“构造方法”的概念是不一样的，它不是指java中某个具有特殊意义或用途的方法。
2。称它为方法的“回调”更恰当一些，它是指方法的一种调用方式。任何一个被“回调”的方法，皆可称之为“回调方法” 
3。方法的回调通常发生在“java接口”和“抽象类”的使用过程中。
假设有接口名为 ICallBack 其中有方法名为postExec()
有类Myclass 实现了该接口，也就是一定实现了postExec()这个方法。现在有另一个类FooBar它有个方法 setCallBack(ICallBack callBack) ,并且setCallBack方法调用了callBack的postExec()方法。
如果现在，我们使用一个Myclass 的实例myClass，将它作为参数带入到setCallBack(ICallBack callBack)方法中，我们就说setCallBack(ICallBack callBack)方法回调了myClass的postExec()方法。 
==========================================================
