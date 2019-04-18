# spring中自定义Event事件的使用和浅析 - z69183787的专栏 - CSDN博客
2018年08月08日 20:51:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：73
个人分类：[Spring-Filter&Listener&Event](https://blog.csdn.net/z69183787/article/category/7068775)
[https://blog.csdn.net/tuzongxun/article/details/53637159](https://blog.csdn.net/tuzongxun/article/details/53637159)
在我目前接触的项目中，用到了许多spring相关的技术，框架层面的spring、spring mvc就不说了，细节上的功能也用了不少，如schedule定时任务、Filter过滤器、 interceptor拦截器等等，而这一篇我要说的是spring Event自定义事件，目前的项目中似乎没怎么用，但是这一项技术貌似还蛮重要，所以也不能不掌握。 
对于事件驱动模型的解释和理解，我觉得有一篇博客里说的非常好，尤其是在解释这个关系的时候，举的交通信号灯的例子非常贴切，这里就引用做一个简单的解释：
> 
事件驱动模型也就是我们常说的观察者，或者发布-订阅模型；理解它的几个关键点： 
1. 首先是一种对象间的一对多的关系；最简单的如交通信号灯，信号灯是目标（一方），行人注视着信号灯（多方）； 
2. 当目标发送改变（发布），观察者（订阅者）就可以接收到改变； 
3. 观察者如何处理（如行人如何走，是快走/慢走/不走，目标不会管的），目标无需干涉；所以就松散耦合了它们之间的关系。
引用的原文地址：[http://xls9577087.iteye.com/blog/2121752](http://xls9577087.iteye.com/blog/2121752)（这篇文章中还讲解了有序监听和无序监听、异步事件等等，有兴趣的也可以去那里了解学习一下） 
当我们对事件驱动有一个简单的理解之后，就能大概知道它应该什么时候用，然后再来研究它该怎么用，单一的文字或许不太容易解释，还是先把代码弄上来，然后再结合起来解释。
首先自定义一个事件，需要继承ApplicationEvent类，相当于安装了一个没有通电，没有灯光的信号灯，需要具有信号灯的基本特征。
```
package springTest5;
import org.springframework.context.ApplicationEvent;
public class EventTest extends ApplicationEvent {
    private static final long serialVersionUID = 1L;
    private String message;
    public EventTest(Object source, String message) {
        super(source);
        this.message = message;
    }
    public String getMessage() {
        return message;
    }
    public void setMessage(String message) {
        this.message = message;
    }
}
```
然后再创建一个监听类，相当于行人（不管是否使用交通工具），需要实现ApplicationListener接口，并且重写onApplicationEvent方法，可以理解成这个行人需要看信号灯，并且能理解信号灯的意思才行。否则不看信号灯跟没有信号灯没有区别，看了不理解也没用。
```
package springTest5;
import org.springframework.context.ApplicationListener;
import org.springframework.stereotype.Component;
@Component
public class ListenerTest1 implements ApplicationListener<EventTest> {
    public void onApplicationEvent(EventTest event) {
        System.out.println("test1:" + event.getMessage());
    }
}
```
这里的注解就只是简单的声明一个bean，应该不需要太多的解释。 
那么第三步自然是需要一个控制信号灯变化的东西，相当于是给他接好电线，给他一个正常变换红黄绿的程序和电路。
```
package springTest5;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;
@Component
public class EventPbulish {
    @Autowired
    ApplicationContext context;
    public void publish(String message) {
        context.publishEvent(new EventTest(this, message));
    }
}
```
到这里，实际上已经写完了，但是呢很明显，我们合理没有配置文件，那么这里的注解也是不能被spring使用的，纯粹是个摆设，所以还需要一个配置文件，或者说相当于配置文件的配置类，要让相关的类生效。
```
package springTest5;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
@Configuration
@ComponentScan("springTest5")
public class ConfigTest {
}
```
对于这里两个注解的意思，前几天的文章曾多次解释，因此这里便不说了，真有不懂得，可以翻一下前几天的博客。 
走到这里，相当于我们创建好了一个可以正常运行的信号灯，创建好了一个正常的行人，但是呢都是静止不动的，我们需要让他动起来，也就是main方法的测试，相当于让行人开始看灯，让电路开始通电。
```
package springTest5;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
public class MainTest {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ConfigTest.class);
        EventPbulish eventPbulish = context.getBean(EventPbulish.class);
        eventPbulish.publish("zhangsan");
        context.close();
    }
}
```
运行main方法后结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161214121220921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHV6b25neHVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据上边的代码可以知道代表这个事件正常运行了，也就是信号灯正常发出了光，行人正常接收到了消息。 
那么还有一个细节在于，信号灯对行人，是一对多的关系，那么这里的事件是否确实如此呢？为了验证，我便再写一个监听，再new一个行人，其他一切不变。
```
package springTest5;
import org.springframework.context.ApplicationListener;
@Component
public class ListenerTest2 implements ApplicationListener<EventTest> {
    public void onApplicationEvent(EventTest event) {
        System.out.println("tst2:" + event.getMessage());
    }
}
```
再次运行main方法，结果如下：‘ 
![这里写图片描述](https://img-blog.csdn.net/20161214121245167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHV6b25neHVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
很明显，两个行人都正常接收到了信号灯的信号。 
那么，根据上边一开始的解释，再加上之后的例子，我们应该大概知道了这里的一个完整的事件包含些什么内容：即要有目标，也就是一个事件；还要有接受目标信息的对象，也就是一种监听；还要有改变或者说发出信息的一个控制体。 
到这里基本上就算是完工了，这算是最简单的实现方式，像一些细节上的，把配置类改配置文件等等，都可以自己适当的变型。 
在结尾处，结合@PropertySource注解，我把上边的列子做了个小小的变型，模拟一个找人的广播，喊一个人的名字，然后听到的人进行相应的回答。 
这个例子和上边的不同在于，增加了一个properties配置，用来给接收对象初始化名称，顺便练习@PropertySource注解。
```
name1=zhangsan
name2=lisi
```
然后修改了一下监听类，从properties文件中获取自己的名字。
```
package springTest5;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.ApplicationListener;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.PropertySource;
import org.springframework.context.support.PropertySourcesPlaceholderConfigurer;
import org.springframework.stereotype.Component;
@Component
@PropertySource("test.properties")
public class ListenerTest1 implements ApplicationListener<EventTest> {
    @Value("${name1}")
    String name;
    public void onApplicationEvent(EventTest event) {
        if (event.getMessage().equals(name)) {
            System.out.println("you need to find " + event.getMessage() + ",yes, I'am " + name);
        } else {
            System.out
                .println("you need to find " + event.getMessage() + ",but I'am not " + event.getMessage() + ",I'am " + name);
        }
    }
    @Bean
    public static PropertySourcesPlaceholderConfigurer propertyConfigure() {
        return new PropertySourcesPlaceholderConfigurer();
    }
}
```
其中propertyConfigure()是必须的，只有写了这个才能正产刚从properties中获取数据，但是经过测试，这一段代码只需要有一个地方出现了就可以，因此第二个监听类就不用再写：
```
package springTest5;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.ApplicationListener;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;
@Component
@PropertySource("test.properties")
public class ListenerTest2 implements ApplicationListener<EventTest> {
    @Value("${name2}")
    String name;
    public void onApplicationEvent(EventTest event) {
        if (event.getMessage().equals(name)) {
            System.out.println("you need to find " + event.getMessage() + ",yes, I'am" + name);
        } else {
            System.out
                .println("you need to find " + event.getMessage() + ",but I'am not " + event.getMessage() + ",I'am " + name);
        }
    }
}
```
在运行之前的main方法后，结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161214121325734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHV6b25neHVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
