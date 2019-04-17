# JAVA设计模式 —— 观察者模式 - DEVELOPER - CSDN博客





2015年01月16日 18:03:52[学术袁](https://me.csdn.net/u012827205)阅读数：846标签：[JAVA设计模式																[观察者模式](https://so.csdn.net/so/search/s.do?q=观察者模式&t=blog)](https://so.csdn.net/so/search/s.do?q=JAVA设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)








![](https://img-blog.csdn.net/20150116180243210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


为了便于理解，首先我举一个现实生活中的例子：在好声音歌手选拔的过程其实就是观察者的一个体现，可以这样说姚贝娜是一个被观察者，而汪峰、那英，还有张惠妹就是3个观察者，被观察者（姚贝娜）操作（唱歌）时，观察者们就开始操作（评分），被观察者唱歌就是通知观察者们进行评分。



有说道：Observer模式的意图是“定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新”。从这段话里我们可以得到两个信息，如下：



1， 观察者（具体执行操作的对象，有多个） 

2， 被观察者（顾名思义是被观察的对象，如果该对象发生某些变化则通知观察者执行对应的操）有两个类，java.util.Observable，在我们实现观察者模式的时候，我们的被观察者应该继承这个类，这个observable类把持住了被观察者所持有的观察者列表：



```java
public class Observable {
    private boolean changed = false;
    private Vector obs;
   
    //创建被观察者时就创建一个它持有的观察者列表，注意，这个列表是需要同步的。
    public Observable() {
	obs = new Vector();
    }

    /**
     * 添加观察者到观察者列表中去
     */
    public synchronized void addObserver(Observer o) {
        if (o == null)
            throw new NullPointerException();
	if (!obs.contains(o)) {
	    obs.addElement(o);
	}
    }

    /**
     * 删除一个观察者
     */
    public synchronized void deleteObserver(Observer o) {
        obs.removeElement(o);
    }

    /**
     * 通知操作，即被观察者发生变化，通知对应的观察者进行事先设定的操作，不传参数的通知方法
     */
    public void notifyObservers() {
	notifyObservers(null);
    }

    /**
     * 与上面的那个通知方法不同的是，这个方法接受一个参数，这个参数一直传到观察者里，以供观察者使用
     */
    public void notifyObservers(Object arg) {
	
        Object[] arrLocal;

	synchronized (this) {
	    if (!changed)
                return;
            arrLocal = obs.toArray();
            clearChanged();
        }

        for (int i = arrLocal.length-1; i>=0; i--)
            ((Observer)arrLocal[i]).update(this, arg);
    }
}
```


讲完了被观察者再让我们来看看观察者，在上面的代码中有一个addObserver(Observer o)方法，由此我们可以看出来，给被观察者添加观察者时就是用这个方法，那么也就是说观察者一定是Observer的子类或者实现，我们看一下java.util.Observer吧：




```java
public interface Observer {
    /**
     * This method is called whenever the observed object is changed. An
     * application calls an <tt>Observable</tt> object's
     * <code>notifyObservers</code> method to have all the object's
     * observers notified of the change.
     *
     * @param   o     the observable object.
     * @param   arg   an argument passed to the <code>notifyObservers</code>
     *                 method.
     */
    void update(Observable o, Object arg);
}
}
```


很好，这是一个接口，接口中就只有一个方法，update，方法中有两个参数，Observable和一个object，第一个参数就是被观察的对象，而第二个参数就得看业务需求了，需要什么就传进去什么。我们自己的观察者类必须实现这个方法，这样在被观察者调用notifyObservers操作时被观察者所持有的所有观察者都会执行update操作了（当然如果你override这个方法，你甚至可以指定何种情况下只执行某种observer了，是不是比较像责任链模式了）。


那么到这里我们也差不多可以把观察者模式应用到项目中去了。首先让我们来实现被观察者（因为实现被观察者非常简单） 

首先让我们来实现一个发送邮件的观察者：



```java
/**
 * @author 张荣华(ahuaxuan)
* @version $Id$
 */
public class MailObserver implements Observer{

	/**
	 * 这个类取名为MailObserver，顾名思义，她是一个用来发送邮件的观察者
	 */
	public void update(Observable o, Object arg) {
		System.out.println("发送邮件的观察者已经被执行");
	}
}
```
接着再让我们来实现一个发送jms消息的观察者：



```java
/**
 * @author 张荣华(ahuaxuan)
 * @version $Id$
 */
public class JMSObserver implements Observer{

	public void update(Observable o, Object arg) {
		System.out.println("发送消息给jms服务器的观察者已经被执行");
	}
}
```
如上所见，观察者的实现完全跟业务相关。是否复杂就得看你得业务是否复杂了。

接下来让我们再来实现被观察者，示例如下：



```java
/**
 * @author 张荣华(ahuaxuan)
* @version $Id$
 */
public class Subject extends Observable{
	
	/**
	 * 业务方法，一旦执行某个操作，则通知观察者
	 */
	public void doBusiness(){
		if (true) {
			super.setChanged();
		}
		notifyObservers("现在还没有的参数");
	}

	
	public static void main(String [] args) {
		//创建一个被观察者
		Subject subject = new Subject();
		
		//创建两个观察者
		Observer mailObserver = new MailObserver();
		Observer jmsObserver = new JMSObserver();
		
		//把两个观察者加到被观察者列表中
		subject.addObserver(mailObserver);
		subject.addObserver(jmsObserver);
		
		//执行业务操作
		subject.doBusiness();
	}
}
```



到此为止，我们已经简单得实现了观察者模式，让我们来运行一下上面main方法，运行结果如下： 

发送消息给jms服务器的观察者已经被执行 

发送邮件的观察者已经被执行 










