# 最佳新秀SSH十六Struts2它是如何工作的内部 - weixin_33985507的博客 - CSDN博客
2015年06月06日 17:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

前面说完了Spring、Hibernate，非常自然今天轮到struts了。struts的核心原理就是通过拦截器来处理client的请求，经过拦截器一系列的处理后，再交给Action。以下先看看struts官方的工作原理图：
![](https://img-blog.csdn.net/20140730153054875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1c2h1aWppbmdlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图1 struts原理图
简单分析一下：首先client发来HttpServletRequest请求，传递给FilerDispatcher（ActionMapper是訪问静态资源（struts的jar文件等）时用的，平时非常少用），然后FilerDispatcher会为我们创建一个ActionProxy，ActionProxy会通过ConfigurationManager获得struts.xml文件里的信息，ActionProxy拥有一个ActionInvocation实例，通过调用ActionInvocation的invoke()方法，来挨个处理Interceptor，最后处理Action，接着Result返回，再逆序经过Interceptor，最后得到HttpServletResponse返回给client。
假设不太明确呢，那就看看以下这张时序图，或许你就懂了：
![](https://img-blog.csdn.net/20140730153310322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1c2h1aWppbmdlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2 struts原理时序图
上面的时序图逻辑就比較清晰了，我就只是多解释了。看完struts的原理图，我们还是须要通过代码来进一步了解它详细是怎么实现的。首先，我们须要一个ActionInvocation：
```java
package com.tgb.struts;
import java.util.ArrayList;
import java.util.List;
public class ActionInvocation {
	List<Interceptor> interceptors = new ArrayList<Interceptor>();
	int index = -1;
	Action a = new Action();
	
	public ActionInvocation() {
		this.interceptors.add(new FirstInterceptor());
		this.interceptors.add(new SecondInterceptor());		
	}
	
	public void invoke() {
		index ++;
		if(index >= this.interceptors.size()) {
			a.execute();
		}else {
			
			this.interceptors.get(index).intercept(this);
		}
	}
}
```
我们实现的ActionInvocation是将Interceptor写在里面的，但实际上是通过反射载入的，原理同之前写的Spring与Hibernate的博客，同样的代码就不在这里占用篇幅了，也没啥意思。不知道怎么实现的朋友请查看前面几篇博客。
接下来是我们的Interceptor接口以及两个简单的实现：
```java
package com.tgb.struts;
public interface Interceptor {
	public void intercept(ActionInvocation invocation) ;
}
package com.tgb.struts;
public class FirstInterceptor implements Interceptor {
	public void intercept(ActionInvocation invocation) {
		System.out.println("FirstInterceptor Begin...");
		invocation.invoke();
		System.out.println("FirstInterceptor End...");
	}
}
package com.tgb.struts;
public class SecondInterceptor implements Interceptor {
	public void intercept(ActionInvocation invocation) {
		System.out.println("SecondInterceptor Begin...");
		invocation.invoke();
		System.out.println("SecondInterceptor End...");
	}
}
```
然后就是我们的Action：
```java
package com.tgb.struts;
public class Action {
	public void execute() {
		System.out.println("Action Run...");
	}
}
```
最后是我们的client调用：
```java
package com.tgb.struts;
public class Client {
	public static void main(String[] args) {
		new ActionInvocation().invoke();
	}
}
```
差点忘了，还有我们最后的运行结果：
```
FirstInterceptor Begin...
SecondInterceptor Begin...
Action Run...
SecondInterceptor End...
FirstInterceptor End...
```
通过上面的运行结果，我们能够非常清晰的看到，请求来的时候会依照顺序被全部配置的拦截器拦截一遍，然后返回的时候会依照逆序再被拦截器拦截一遍。这跟数据结构中的“栈”非常相似（FIFO-先进先出），数据结构我不太懂，或许这样比喻有些不妥。各位依据自己的认识理解吧。
近期一直在研究这三大框架，折腾半天它们都离不开集合，离不开反射。事实上它们道理都是想通的，搞懂一个，其它的也就非常好懂了。等着吧，早晚咱们自己写一个更好用的。
