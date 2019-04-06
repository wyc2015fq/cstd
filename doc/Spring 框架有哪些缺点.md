# Spring 框架有哪些缺点

刚刚开始了解一些spring。常常看到网上各种对spring赞美之词。但有时候看身边的程序员用spring等等框架写出来的代码一坨一坨的。究竟是我功力…

关注者

1,304

被浏览

389,571















#### 77 个回答



[![Night Silent](https://pic1.zhimg.com/bb7666726_xs.jpg)](https://www.zhihu.com/people/night-silent)

[Night Silent](https://www.zhihu.com/people/night-silent)

千人Java自学群179536933(互助答疑)151280557(专业答疑)



缺点当然有，现在 Spring 越来越大，都开始使用 Gradle 构建了，支持的东西越来越多，这就导致大多数程序员对 Spring 的体系及结构都一知半解，网上乱七八糟的文章来回粘来粘去，搞得小码农都分不清楚到底哪种使用方式是对的，这归根到底就是因为

\1. 对 Spring 的版本变更不清楚

\2. 对 Spring 和 J2EE 规范的关系不清楚

\3. 对 Spring 那堆配置和注解背后的原理不清楚

你可能知道 DI 是个什么概念，但你可能不知道 DI 的正确使用方式：

\1. 你也许不知道 Spring 有自己的测试体系，我们可以直接使用

\2. Spring 的事务管理优越在哪里，和 Hibernate 自己的事务管理有啥区别，怎么配置才对。

\3. Spring 为啥提倡非侵入式（non-invasive）设计，又是从哪个版本开始这么干的，起因是什么

\4. Spring 那些注解的使用原理和适用场景是什么，哪些是Spring 自己的注解，哪些是 J2EE DI 的标准注解，使用哪个好，为什么好。

...........

上述的一切，当你把 Spring 官方提供的 Spring Reference 看的差不多了，也就明白了，大概 500 多页吧。

Spring 越来越大，衍生出其他的分支项目，虽然臃肿，但如果你知道如何筛选自己想要的功能，就不会臃肿，反而会很简洁，例如：

\1. 你想实现 Rest 服务，那么 Spring MVC 就预置了这个功能，你没必要再去使用 CXF / Restlet 之类的 Rest 框架，还得忙着想怎么和 Spring 整合，这样，Struts 就出局了，Rest 框架也省了。

\2. 你想实现权限功能，那么 Spring Security 可以替你搞定，你没必要使用 Apache Shiro 来做，也没必要苦逼地自己设计，而且人家还原生支持 OAuth 和 SAML

\3. 你不想苦逼地写 DAO, 那么 Spring Data 可以替你做，这些 boilerplate code 全部替你实现了，而且对 JPA 或 NO-SQL 的支持也是相当方便。

\4. 你无法忍受也无法记住 spring 的配置方式和配置逻辑，你只想专心写自己的业务代码而不愿关心工具到底如何配置，那么 SpringBoot 可以搞定一切。

...........

其他的，还有很多很多，你只需要记住，是不是臃肿，很大程度上是由你的认知和使用能力决定的，不是框架本身，具体如何学习这个优秀的框架，建议：

\1. 视频教程点到为止，不可全信，网上流传的课程，本身就有错误存在

\2. 各种帖子看看就好，别拷过来自己完事

\3. 有问题，先去查阅 Spring Reference，懂原理才能游刃有余

P.S

感谢知友提醒，修正答案

1. Spring Roo 和 Spring Boot 都是 Spring 组件，没有演变关系
2. 基于 SpringBoot 的 micro-service 架构目前在国外很流行，学习 SpringBoot 尤为必要
3. 推荐使用 Spring Boot，选择 Spring Roo 还是 Spring Boot，引用一段话如下：

Spring Boot, Spring Roo, Spring Framework (Spring forum at JavaRanch)

```text
First, whether you use Spring Boot or Spring Roo, the reality is you're using 
Spring. So to answer your question of whether you'd be able to use other
 Spring Framework components...yes, absolutely. Again, Spring Boot *is* 
Spring. Spring Roo *is* Spring. They're just different approaches to 
simplifying Spring development.

Now, choosing between Spring Boot and Spring Roo. I was once a fan of Spring 
Roo, but Spring Boot outshines it in so many ways. Spring Roo was big on code
 generation and used aspects to perform some of its magic. It also forced you
 into a design model that you may or may not feel comfortable with. Spring 
Boot, on the other hand, uses no code generation and does not force any 
particular design choice. It is primarily autoconfiguration saving you from 
having to write a lot of common configuration yourself

In short...Go with Spring Boot. You won't regret it. 
```

我的其他回答：

- [哪些企业或项目在用Spring boot,或者它会在多大程度上替代Spring? - Night Silent 的回答](https://www.zhihu.com/question/49649311/answer/117080397)
- [会计转行从事IT，如何在一年时间内全职学习？ - Night Silent 的回答](https://www.zhihu.com/question/21427478/answer/18227060)
- [SpringDataJPA持久层问题? - Night Silent 的回答](https://www.zhihu.com/question/32327775/answer/55577060)
- [spring4+hibernate4 事务管理 非常难受的问题 ? - Night Silent 的回答](https://www.zhihu.com/question/35470753/answer/62939826)
- [spring 中为何存在import注解，同一个包中的类直接使用就行了 用import注解感觉有点怪？ - Night Silent 的回答](https://www.zhihu.com/question/47178104/answer/120134322)
- 更多回答，请关注我，获取最新动态
- 如果此回答帮到了你，一个小小的赞，一次分享，都会让更多人受益

此外，有兴趣的话，欢迎加入我的 

Java EE 自学群

- **一号群：151280557（即将满额）**：[二维码页面](https://link.zhihu.com/?target=http%3A//qm.qq.com/cgi-bin/qm/qr%3Fk%3DO_AAjKe3irM0TY6ayvKIrvtvEMNk9rTT) (二维码自动识别)

- 二号群：521909033（新群，限时免费加入）: [二维码页面](https://link.zhihu.com/?target=http%3A//qm.qq.com/cgi-bin/qm/qr%3Fk%3DWUu4I0-EtyXlUysdvJ1juYlFiFnZY9wQ) (二维码自动识别)

  

如果觉得我的回答对你很有帮助，可以考虑微信打赏：

[https://wx.tenpay.com/f2f?t=AQAAANljM%2FunWdckY8a%2Fz7m%2BeOE%3D](https://link.zhihu.com/?target=https%3A//wx.tenpay.com/f2f%3Ft%3DAQAAANljM%252FunWdckY8a%252Fz7m%252BeOE%253D) (二维码自动识别)

[编辑于 2016-09-12](https://www.zhihu.com/question/25255223/answer/31961636)





[![铅笔](https://pic2.zhimg.com/3d2e644752cd99eafa7b23abc63ffe6f_xs.jpg)](https://www.zhihu.com/people/qian-bi-37-9)

[铅笔](https://www.zhihu.com/people/qian-bi-37-9)





我看下面有些人的回答，充满了趣味性。
     举个例子:
     问:既然spring这么臃肿，用什么替代啊
     答:用jetty啊
     你怎么不用http代替啊
还有某位大神的原话:
      我写的orm框架完爆mybatis。
我还能怎么办，我也很绝望啊
       某些知友回答问题之前能不能仔细的考虑考虑，知乎不是单机游戏。你的回答，别人是可以看到的。不存在的事情你帮他说一遍，等于也是有责任的吧！
分割线，说一说我的态度。
        首先，spring不是为某一个用户，某一个企业设计的。spring也许为了那百分之一的需求就要增加百分之五十的代码量。你觉得那百分之一的需求没用，人家觉得那是刚需啊！
        还有人说spring影响了他java功力。这个我就不评论了，我相信知乎还是有会独立思考的人
       还有人随随便便造个轮子就能比肩spring mvc。虽然我也造过mvc框架，知道mvc就那么回事。但是你猜一猜新增一个功能你的mvc框架要改百分之几十的代码，spring mvc要改百分之几十的代码？
       当然也许你能用spring mvc十分之一的代码量达到spring mvc的便捷性，稳定性和可维护性以及扩展性。那上面那些话当我没说，顺便请留下你的github让大家瞻仰瞻仰。

[发布于 2018-02-13](https://www.zhihu.com/question/25255223/answer/318224757)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户





最大的缺点在于没有一个总架构图，比利于初学者掌握全貌。

其次似乎官网的CMS做得极差！做得极差！做得极差！搜索成本非常高

第三，做什么都要和springboot绑起来，一点都不大气

[发布于 2018-01-08](https://www.zhihu.com/question/25255223/answer/291406463)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户





Spring框架的依赖注入是家喻户晓的，但是在实际的开发中我们想使用便捷的依赖注入功能，但是又不想引入Spring框架的笨重和复杂性，该怎么办呢？

有了Google Guice，这个问题便简单了，首先在你的maven项目里引入

```text
<dependency>
    <groupId>com.google.inject</groupId>
    <artifactId>guice</artifactId>
    <version>4.0</version>
</dependency>
```

官方文档里给出的例子又臭又长，我不使用官方的例子，下面我们来写个最简单的HelloWorld

```text
import javax.inject.Singleton;

import com.google.inject.Guice;
import com.google.inject.Inject;
import com.google.inject.Injector;

@Singleton
class HelloPrinter {

	public void print() {
		System.out.println("Hello, World");
	}

}

@Singleton
public class Sample {

	@Inject
	private HelloPrinter printer;

	public void hello() {
		printer.print();
	}

	public static void main(String[] args) {
		Injector injector = Guice.createInjector();
		Sample sample = injector.getInstance(Sample.class);
		sample.hello();
	}

}
```

我们使用Guice创建了一个注射器Injector，然后从Injector拿到你想要的对象就可以了，Guice会自动装配依赖树。Guice的启动速度是很快的，在一个大型应用中，Guice装配所有的模块决不会超过1s。Guice是一个非常干净的依赖注入框架，框架除了依赖注入功能之外，没有任何其它非相关模块功能。

Guice里最常用的两个注解就是@Singleton和@Inject，Singleton表示构建的对象是单例的，Inject表示被标注的字段将使用Guice自动注入。在一般的项目中这两个注解一般可以完成90%以上的装配工作。

[Guice使用入门](https://zhuanlan.zhihu.com/p/32299568)

[编辑于 2017-12-24](https://www.zhihu.com/question/25255223/answer/282166546)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户





Spring给我的感觉像个老妈子，自己不用操心，它衣食住行样样帮你打理得井井有条。

忽然有一天，你表示今晚想出去吃饭，不在家吃了，可不可以？

Spring：饭已经做好了。你必须吃。

[发布于 2017-06-24](https://www.zhihu.com/question/25255223/answer/188503609)





[![施建航](https://pic4.zhimg.com/da8e974dc_xs.jpg)](https://www.zhihu.com/people/shigebeyond)

[施建航](https://www.zhihu.com/people/shigebeyond)

韬光养晦, 求道自明, 有 jkmvc 等框架



我可以说我很讨厌spring么，做java的前三年没有打通任督二脉都是spring闹的。

spring是个八宝箱，啥都有，翻翻宝贝查查手册都能直接用。

国内大部分java程序员都很懒（其他语言也类似），随大流吧，虽然没啥好处，也没啥坏处，既然团队用公司用，那么自己也凑合着用呗，不用动脑子，省事省时省心。

关键是spring复杂的要命，现在又不停的往里面加东西，这些复杂度已经不是箱子能装得下，杂物间也装不下了，中央仓库也装不下了，简直是复杂度的深渊。

千万不要看他的源码，千万不要看他的源码，千万不要看他的源码，虽然注释很好，关键是太多了太庞大了太复杂了，复杂度失控了，我看完都可以升天了。

ioc很难么？aop很难么？原理就那么点，干嘛实现那么复杂，看spring的时间都足够重新造一个了。你可以争辩，但是请先阅读spring源码，我保证你不枉此行。

性能？这么简单的事情这么复杂的实现，这个还用说么？

易用性？spring boot包一层纸就简单了？纸捅破了还不是一样复杂？

挑灯夜读spring源码，满腹牢骚，吐之为快。

总之，spring就是一个过度设计的庞然大物。

注：我没有否定spring的价值（虽然他的配置比代码还难写），我只是吐槽他的实现而已。教科书式的经典设计，挽救不了工程化上的难以维护，试想想你来维护这个项目，Oh my lady gaga

========================================

补充：看了下面好多讨论，我觉得是技术理念的碰撞，接下来我要阐述一下个人的技术理念，一个好的框架不但要好用，而且要好看。

好用这个不用详谈，大家都清楚，譬如注解比xml好用，约定比注解好用。

而对于好看，我觉得最关键的是设计要简单。

其实将一件简单的事情做复杂是很容易的，我们大部分程序员都不自觉的将设计复杂化，这跟我们的接受的教育有关，包括重构/设计模式/高内聚低耦合等等设计理念，你考虑的情况越多，你支持的特性越多，那么你的设计会越复杂，这个过程又往往伴随着生搬硬套与过度设计，这是每个程序员都要经历的过程。但是复杂化的结果是大家都很痛苦，难以把控这套代码。

要打破这种僵局，就是要打破代码，打破设计，甚至打破问题。

要打破问题，需要扪心自问：我到底需要什么？我需不需要这么复杂的东西？将问题本质化简单化，会让我们更有效的解决问题。

要打破设计，这往往需要一个长久的重构过程，关于类族的设计，类与类关系的设计，甚至于类与变量的命名，我都会纠结很久，不停的取舍，不停的重写，以便抽象为更简单更容易理解的设计。

优雅往往伴随着复杂，而简单则意味着把控力。优雅与简单是一对矛盾体，如阴与阳，是一个不断妥协，不断取舍，不断折中，以致于中庸的过程，这就是程序设计的艺术，也是智慧。

简单的魅力是巨大的，能让你更快更好的把控整套架构，没有多余的复杂度，能让你更快更好的解决问题。如我了然ORM的设计，我可以在几分钟内让他支持读写分离。如我了然SOA的设计，我可以很方便的更换序列化策略。这生产力，不是你啃api手册就能够带来的。

如果你仅是止于用，权当我的话是卖油翁的唠叨，生命短暂，拿来即用，省点时间陪家人也是不错的选择。

如果你探索原理，希望我的唠叨能有点营养，路漫漫其修远兮，希望你能耐得住寂寞。

大道至简，知易行难。

[编辑于 2018-01-11](https://www.zhihu.com/question/25255223/answer/290571480)





[![潜龙勿用](https://pic1.zhimg.com/v2-68267c14411ed15300f59651d6e10078_xs.jpg)](https://www.zhihu.com/people/qian-long-wu-yong-81-57)

[潜龙勿用](https://www.zhihu.com/people/qian-long-wu-yong-81-57)





大部分项目都少不了Spring的身影，我们看一个框架是否优秀，首先看这个框架是否是非侵入式的，其次是可扩展性，再次是解耦性和开发周期。

Spring是一个轻量级的DI和AOP容器框架。说它轻量级有一大部分原因是相对与EJB的（虽然本人从没有接触过EJB的应用），重要的是，Spring是非侵入式的(所谓非侵入式就是远程调试  而不植入，spring不再用new的方式来创建对象，而是使用依赖注入的方式)，基于spring开发的应用一般不依赖于spring的类。

DI:称作依赖注入(Dependency   Injection),和控制反转一个概念，具体的讲，当一个角色需要另外一个角色协助的时候，在传统的程序设计中，通常有调用者来创建被调用者的实例。但是在spring中创建被调用者将不再有调用者完成，因此叫控制反转。创建被调用对象有Spring来完成，在容器实例化对象的时候主动的将被调用者（或者说它的依赖对象）注入给调用对象，因此又叫依赖注入。

AOP：Spring对面向切面编程提供了强有力的支持，通过它让我们将业务逻辑从应用服务（如事务管理）中分离出来，实现了高内聚开发(所谓高内聚是指一个软件模块是由相关性很强的代码组成，只负责一项任务，也就是常说的单一责任原则。)，应用对象只关注业务逻辑，不再负责其它系统问题（如日志、事务等）。Spring支持用户自定义切面。

面向切面编程是面向对象编程的有力补充。面向对象编程将程序分成各个层次的对象，面向切面的程序将运行过程分解成各个切面。AOP是从运行程序的角度去考虑程序的结构，提取业务处理过程的切面，OOP是静态的抽象，AOP是动态的抽象，是对应用执行过程的步骤进行抽象，从而获得步骤之间的逻辑划分。

容器：Spring是个容器，因为它包含并且管理应用对象的生命周期和配置。如对象的创建、销毁、回调等。

框架：Spring作为一个框架，提供了一些基础功能，（如事务管理，持久层集成等），使开发人员更专注于开发应用逻辑。

![img](https://pic1.zhimg.com/80/v2-94112bbe31f0f4b0870d11fec384c7e0_hd.jpg)

在了解了Spring及其相关的概念之后，再来看看Spring有哪些优点：

1.使用Spring的IOC容器，将对象之间的依赖关系交给Spring，降低组件之间的耦合性，让我们更专注于应用逻辑；

2.可以提供众多服务，事务管理，WS等；

3.AOP的很好支持，方便面向切面编程；

4.对主流的框架提供了很好的集成支持，如Hibernate,Struts2,JPA等；

5.Spring DI机制降低了业务对象替换的复杂性；

6.Spring属于低侵入，代码污染极低；

7.Spring的高度可开放性，并不强制依赖于Spring，开发者可以自由选择Spring部分或全部。

再来看看Spring有哪些缺点：

1.jsp中要写很多代码、控制器过于灵活，缺少一个公用控制器；

2.Spring不支持分布式，这也是EJB仍然在用的原因之一。

[发布于 2017-09-18](https://www.zhihu.com/question/25255223/answer/231858357)





[![张国钰](https://pic4.zhimg.com/c737aee3b_xs.jpg)](https://www.zhihu.com/people/zhang-guo-yu)

[张国钰](https://www.zhihu.com/people/zhang-guo-yu)

技术各来一点的小白



明明一个很轻量级的框架，却给人感觉大而全，这就是它的缺点。
Spring的核心概念是啥，最主要的，IoC和AOP，这两个核心服务的对象是啥，就是那堆beans，Spring我们可以理解为就是存放这些bean的容器，它最核心的就干这些事。
IoC干嘛的，就是为了让开发者关注的重心放在如何使用这些bean。不管bean怎么来，只要@Autowired就给我老老实实出现，让我调接口就好，不需要每次用之前都要手动构建，尽量减少手动实例化bean的次数。Spring  Boot以前，主要在beans.xml中注册bean，Spring  Boot的话只要在Configuration中定义一个@Bean方法即可，就为了做好这件事，Spring弄了多少东西，BeanDefinition，BeanFactory，FactoryBean，然后Bean的生命周期接口好几个，Aware接口又好几个，等等，不反复使用，其实不太好理解。
然后是AOP，AOP是很好的特性，在Spring中其主要目的就是以一种非浸入的方式对bean进行扩展，看到没，又是bean。为了对bean支持，Spring弄了一套AspectJ的子集，既有Java原生的接口代理，又有更加灵活的Cglib，光是AOP的概念就不少，更不要说实现的原理。简单的大家都可以说出一二三四五，但深入细节谁有把握？
所以，Spring就是围绕着bean展开的，它主要就是作为bean的容器，在这个基础上，有了SpringMVC，有了JPA，有了Cloud，有了Data   Flow，以及其他模块。但这些模块的本质，不全是Spring，比如JPA，核心还是Hibernate，Cloud，核心是另外一套微服务框架。Spring就好比胶水，通过bean把这些模块粘在一起。实践过程中，反复的提醒自己，不要用Spring玩花，让它老老实实的做bean的容器就好了。我这么说，不看源码，你会觉得Spring重吗？
可就这么些个东西，Spring弄出了多少名堂。随便找个地方打断点，调用就不下二十来层，在现在这种强调轻量化服务的大环境下，不被吐槽才怪。
当然，我认为也不能全怪Spring。Java这门语言动态性本来就是它的弱项，Spring能把Java包装的像个动态化的语言已经蛮拼了。Spring背的最大的锅，我认为就是J2EE，就凭那套在当时有一定价值但放现在已经淘汰的标准，没有Spring，Java的企业级市场未必有今天。
不过话说回来，从学习企业级Java开发的角度看，Spring的代码依然是教科书般的代码。面向对象封装，编程原则，设计模式，都是具有很高的参考价值。如果你才开始，我建议先沉住气，老老实实的敲代码，看文档，不管最后还在不在Java圈，将来一定会收获不小。
有时候，你不复杂一些，又怎么能体会到简单的价值呢？

[编辑于 2018-01-09](https://www.zhihu.com/question/25255223/answer/292653680)





[![wkGCaSS](https://pic3.zhimg.com/8e4eb903ab66c5eead07ebfa9442b3ee_xs.jpg)](https://www.zhihu.com/people/wang-kui-gang-5)

[wkGCaSS](https://www.zhihu.com/people/wang-kui-gang-5)

latte-lang.org



缺点就是门槛太低，以至于会有这样的人：

![img](https://pic1.zhimg.com/80/v2-a99c624702cc5738d27b4d8c1c123de4_hd.jpg)



[@旋律秋凉](https://www.zhihu.com/people/f3b8ea016352506c56dafd8b70695d13)



[发布于 2018-03-07](https://www.zhihu.com/question/25255223/answer/336027238)





[![Yifen Hao](https://pic4.zhimg.com/45f34162eb9575774da1e6466a2d05c4_xs.jpg)](https://www.zhihu.com/people/yifen-hao-22)

[Yifen Hao](https://www.zhihu.com/people/yifen-hao-22)

每个都懂一点



不同公司，不同量级，不同业务，不同接口，都有不同的性能需求，内存需求，语言需求。
Spring对于世界上绝大绝大部分接口来说，没啥缺点，值得学习。
对于有极度性能需求，机器消耗限制的海量访问请求的接口来说，太重，内存消耗太大，性能低（单机上不了1w）

[发布于 2018-03-05](https://www.zhihu.com/question/25255223/answer/334891025)





[![白马](https://pic2.zhimg.com/v2-5641bb3d2fa8f8b827f058dc0047dfbd_xs.jpg)](https://www.zhihu.com/people/bai-ma-27-67)

[白马](https://www.zhihu.com/people/bai-ma-27-67)

Java新手指南/交流群；472052538



大部分项目都少不了**spring**的身影，为什么大家对他如此青睐，而且对他的追捧丝毫没有减退之势呢

Spring是什么?

Spring是一个轻量级的DI和AOP容器框架。

说它轻量级有一大部分原因是相对与EJB的（虽然本人从没有接触过EJB的应用），重要的是，Spring是非侵入式的(所谓非侵入式就是远程调试  而不植入，spring不再用new的方式来创建对象，而是使用依赖注入的方式)，基于spring开发的应用一般不依赖于spring的类。

DI:称作依赖注入(Dependency   Injection),和控制反转一个概念，具体的讲，当一个角色需要另外一个角色协助的时候，在传统的程序设计中，通常有调用者来创建被调用者的实例。但是在spring中创建被调用者将不再有调用者完成，因此叫控制反转。创建被调用对象有Spring来完成，在容器实例化对象的时候主动的将被调用者（或者说它的依赖对象）注入给调用对象，因此又叫依赖注入。

AOP：Spring对面向切面编程提供了强有力的支持，通过它让我们将业务逻辑从应用服务（如事务管理）中分离出来，实现了高内聚开发(所谓高内聚是指一个[软件](https://link.zhihu.com/?target=http%3A//baike.baidu.com/view/37.htm)模块是由相关性很强的代码组成，只负责一项任务，也就是常说的单一责任原则。)，应用对象只关注业务逻辑，不再负责其它系统问题（如日志、事务等）。Spring支持用户自定义切面。

面向切面编程是面向对象编程的有力补充。面向对象编程将程序分成各个层次的对象，面向切面的程序将运行过程分解成各个切面。AOP是从运行程序的角度去考虑程序的结构，提取业务处理过程的切面，OOP是静态的抽象，AOP是动态的抽象，是对应用执行过程的步骤进行抽象，从而获得步骤之间的逻辑划分。

容器：Spring是个容器，因为它包含并且管理应用对象的生命周期和配置。如对象的创建、销毁、回调等。

框架：Spring作为一个框架，提供了一些基础功能，（如事务管理，持久层集成等），使开发人员更专注于开发应用逻辑。



看完了Spring是什么，再来看看Spring有哪些优点

1.使用Spring的IOC容器，将对象之间的依赖关系交给Spring，降低组件之间的耦合性，让我们更专注于应用逻辑

2.可以提供众多服务，事务管理，WS等。

3.AOP的很好支持，方便面向切面编程。

4.对主流的框架提供了很好的集成支持，如**hibernate**,Struts2,JPA等

5.Spring DI机制降低了业务对象替换的复杂性。

6.Spring属于低侵入，代码污染极低。

7.Spring的高度可开放性，并不强制依赖于Spring，开发者可以自由选择Spring部分或全部

  再来看看Spring有哪些缺点

​       1.jsp中要写很多代码、控制器过于灵活，缺少一个公用控制器

​       2.Spring不支持分布式，这也是EJB仍然在用的原因之一。







**首先Spring 是一个框架，使用Spring并不代表代码质量的提高，就像盖房子选择用上海的地皮还是北京的地皮一样，房子质量与土地所在的城市无关，与房子的具体设计方案和选料有关。** **使用Spring 等框架可以简化很多基础性的工作，配置好后可以方便构建业务应用。** **框架使用多了会有局限的感觉，像小鸟被套在笼子里，无法飞出去，虽然在笼子里面吃喝不愁。目前编程的门槛越来越低，诸多开源框架广泛传播，几乎没有什么技术门槛，会配置就会编程，而一个好的DBA对软件性能会有很大提高，软件的核心逻辑最终会转移到对数据库的操作上，而且对目前从事的工作来讲，感觉技术的瓶颈越来越多的局限在对数据库的操作上，下一步要认真提高下了。**

**Spring的优势不言而喻：**

　　**1. 提供了一种管理对象的方法，可以把中间层对象有效地组织起来。一个完美的框架“黏合剂”。**

　　**2. 采用了分层结构，可以增量引入到项目中。**

　　**3. 有利于面向接口编程习惯的养成。**

　　**4. 目的之一是为了写出易于测试的代码。**

　　**5. 非侵入性，应用程序对Spring API的依赖可以减至最小限度。**

　　**6. 一致的数据访问介面。**

　　**6. 一个轻量级的架构解决方案。**

　　**对Spring的理解**

　　**Spring致力于使用POJOs来构建应用程序。由框架提供应用程序的基础设施，将只含有业务逻辑的POJOs作为组件来管理。从而在应用程序中形成两条相对独立发展的平行线，并且在各自的抽象层面上延长了各自的生命周期。**

　**Spring的工作基础是Ioc。Ioc将创建对象的职责从应用程序代码剥离到了框架中，通常2中注入方式：setter 和 ctor参数。**

　　**每个Bean定义被当作一个POJO(通过类名和JavaBean的初始属性或构造方法参数两种方式定义的Bean)。**

　　**Spring的核心在org.springframework.beans，更高抽象层面是BeanFactory. BeanFactory是一个非常轻量级的容器。**

　　**关于可维护性的思考**

　　**Spring之类的技术确实带来了应用系统的可维护性的提高吗?**

　　**Ioc， AOP之类的技术，本质上都是将原本位于应用程序代码中"硬编码"逻辑，剥离出来放到了配置文件中(或者其他形式)。主流声音都是认为提高了应用程序的可维护性。**

　　**但如果从以下方面观察，结合项目实际经验，个人感觉这些技术的应用大大降低了应用程序的可维护性，尤其是面对一个陌生的系统，或者项目人员变动频繁的时候。**

　　**1. 中断了应用程序的逻辑，使代码变得不完整，不直观。此时单从Source无法完全把握应用的所有行为。**

　　**2. 将原本应该代码化的逻辑配置化，增加了出错的机会以及额外的负担。**

　**3. 时光倒退，失去了IDE的支持。在目前IDE功能日益强大的时代，以往代码重构等让人头痛的举动越来越容易。而且IDE还提供了诸多强大的辅助功能，使得编程的门槛降低很多。通常来说，维护代码要比维护配置文件，或者配置文件+代码的混合体要容易的多。** 　　**4. 调试阶段不直观，后期的bug对应阶段，不容易判断问题所在。** 

**首先Spring 是一个框架，使用Spring并不代表代码质量的提高，就像盖房子选择用上海的地皮还是北京的地皮一样，房子质量与土地所在的城市无关，与房子的具体设计方案和选料有关。**

**使用Spring 等框架可以简化很多基础性的工作，配置好后可以方便构建业务应用。**



**框架使用多了会有局限的感觉，像小鸟被套在笼子里，无法飞出去，虽然在笼子里面吃喝不愁。目前编程的门槛越来越低，诸多开源框架广泛传播，几乎没有什么技术门槛，会配置就会编程，而一个好的DBA对软件性能会有很大提高，软件的核心逻辑最终会转移到对数据库的操作上，而且对目前从事的工作来讲，感觉技术的瓶颈越来越多的局限在对数据库的操作上，下一步要认真提高下了。**



**Spring的优势不言而喻：**



　　**1. 提供了一种管理对象的方法，可以把中间层对象有效地组织起来。一个完美的框架“黏合剂”。**



　　**2. 采用了分层结构，可以增量引入到项目中。**



　　**3. 有利于面向接口编程习惯的养成。**



　　**4. 目的之一是为了写出易于测试的代码。**

　**5. 非侵入性，应用程序对Spring API的依赖可以减至最小限度。**

　　**6. 一致的数据访问介面。**

　　**6. 一个轻量级的架构解决方案。**

　　**对Spring的理解**

　　**Spring致力于使用POJOs来构建应用程序。由框架提供应用程序的基础设施，将只含有业务逻辑的POJOs作为组件来管理。从而在应用程序中形成两条相对独立发展的平行线，并且在各自的抽象层面上延长了各自的生命周期。**

　　**Spring的工作基础是Ioc。Ioc将创建对象的职责从应用程序代码剥离到了框架中，通常2中注入方式：setter 和 ctor参数。**

　　**每个Bean定义被当作一个POJO(通过类名和JavaBean的初始属性或构造方法参数两种方式定义的Bean)。**

　　**Spring的核心在org.springframework.beans，更高抽象层面是BeanFactory. BeanFactory是一个非常轻量级的容器。**

　　**关于可维护性的思考**

　　**Spring之类的技术确实带来了应用系统的可维护性的提高吗?**

**Ioc， AOP之类的技术，本质上都是将原本位于应用程序代码中"硬编码"逻辑，剥离出来放到了配置文件中(或者其他形式)。主流声音都是认为提高了应用程序的可维护性。**

　　**但如果从以下方面观察，结合项目实际经验，个人感觉这些技术的应用大大降低了应用程序的可维护性，尤其是面对一个陌生的系统，或者项目人员变动频繁的时候。**

　　**1. 中断了应用程序的逻辑，使代码变得不完整，不直观。此时单从Source无法完全把握应用的所有行为。**

　　**2. 将原本应该代码化的逻辑配置化，增加了出错的机会以及额外的负担。**

　　**3. 时光倒退，失去了IDE的支持。在目前IDE功能日益强大的时代，以往代码重构等让人头痛的举动越来越容易。而且IDE还提供了诸多强大的辅助功能，使得编程的门槛降低很多。通常来说，维护代码要比维护配置文件，或者配置文件+代码的混合体要容易的多。**

　　**4. 调试阶段不直观，后期的bug对应阶段，不容易判断问题所在。**

**注：**希望继续在IT行业突破提升自己的各位朋友，也欢迎加(qun)472052538，不管你自我感觉牛不牛B。





[编辑于 2018-03-05](https://www.zhihu.com/question/25255223/answer/334469876)





[![铁原](https://pic1.zhimg.com/v2-9c5e93bf16d16c5a2aeb6d344f8874cc_xs.jpg)](https://www.zhihu.com/people/tie-yuan-89)

[铁原](https://www.zhihu.com/people/tie-yuan-89)

领域设计，业务架构



**先说结论：Spring本质是对组件的管理，欠缺的是对资源（主机）的管理。因此缺乏部署弹性能力。**



一个理想架构是什么？是最傻瓜化的，又最灵活最强大的。它能用最简单的方式实现业务目的；不限制任何业务；透明的事务支持；透明的消息传输和协议支持；服务自适应调整能力；透明的服务增强；极强的动态弹性部署能力（类似docker）。可能有些人认为这个东西不存在，事实上已经存在很久了——TUXEDO。

Java方面最接近的技术是EJB——EJB在动态弹性上要差TUXEDO很多。主机节点的扩展还没有那么透明和弹性。但是其他要素已经基本具备，你可以用EJB实现任何业务，没有限制；透明的事务增强；透明的消息传输增强；服务自适应能力；透明服务增强。它已经很接近完美了。

EJB最为人所诟病的是它最初要依赖一个接口，这个小小的接口成为EJB体系崩溃的蚂穴。因为这个依赖让EJB的测试变的比较困难，给了Spring颠覆它的机会。

我最喜欢的技术经典书籍是《Devolope without EJB》，它告诉所有的开发们：分布式第一原则——如非必要，不要分布式你的系统——你不需要EJB，Spring就够了。Spring实际上是一个EJB的阉割品。

不用我提示现在Spring发生了什么吧——10多年后，它通过其他方式回到了自己反对的起点，用ZK等注册中心替代了JNDI，用SpringBean替代了EJB——除了名字不一样，实质没有多大的差异。那么它到底是一种进步呢？还是退步呢？从不同的角度看，也许会有不同的答案。但是站在理想架构角度呢？

但是我们就此可以否定Spring在当时那个时代的意义么？什么是慎重和守旧的差别，什么是冒进和创新的差别，什么是进步和不进步？也许技术世界的真正完美好像进化论一样，根本没有什么是先进和落后，只有适应性罢了。

今天，Spring欠缺的分布式除了用Dubbo/SpringCloud等方式弥补回来，Spring欠缺的弹性部署能力似乎也要通过Docker等技术实现——那么，在架构上，它算完美么？没有缺点么？

远远不是。Spring核心的理念是组件依赖管理——他们本质上都是一些线程，运行在主机上，但是它欠缺对主机资源的集成管理；从整体架构的角度上来看，是不完美的。





至于谈Spring过于庞大的，其实我并不能认同。Spring相对于它面对的问题的复杂度，在实现层面它已经接近完美了。在它解决的问题领域，它几乎已经做到了它能做到的极限了——所有谈及替代它的那些框架要做到同样的地步，不可能做的比他好多少。这些都是关注一些极细节的问题。大多数回答都没有到关键的点子上，@白马   是少有的认识很深刻的——spring真的在解决分布式、弹性问题上更简单么？降低成本么？不，它更复杂，因为它本质上就是基于那个特定的目的，从细节开始，从局部优化开始，慢慢意识到全局问题的框架——而不是TUXEDO那样从全局角度考虑的架构，是一个问题的局部最优解。



如果站在业务的角度，而不是Spring体积本身。你会发现Spring可以非常的让代码关注于业务本身了。是非常瘦，非常灵活的、非常成功的。如果你们觉得spring重，要自造轮子，为什么不嫌操作系统重，写代码不从硬件写起？



除此以外，还有几个不太大的小缺点

1，缺乏内置分布式支持，透明的分布式支持

2，现有分布式框架缺乏透明的消息传递，透明的传输协议支持（我希望通过声明书的方式让业务无需关注RMI,HTTP,SOCKET差异）

3，缺乏对完全异步，响应式、无阻塞编程模型的支持。

[编辑于 2019-03-06](https://www.zhihu.com/question/25255223/answer/615089402)





[![kanmars](https://pic3.zhimg.com/v2-b49d957f9c171f5d972fd6ecbccf8ee0_xs.jpg)](https://www.zhihu.com/people/kanmars-42)

[kanmars](https://www.zhihu.com/people/kanmars-42)

越是追寻幸福的人，越是会常常感到孤独



让我们来回顾一下JAVA的历史：

## **上古时代**：

1995年前，蓝星人的浏览器还比较古老，甚至不能展示图像。随着1995年JAVA1.0的发布，蓝星人能看到APPLET画出来的图像了，这是划时代的进步。

## 中古**时代**：

1999年，随着万维网的爆发，JAVA2.0发布，即J2SE+J2ME+J2EE。很多年轻人不知道这是啥玩意儿。我们这么来解释：

中古时期，地球上做个应用程序语言混乱，有C、C++、PB、VB、DELPHI等等杂牌军。J2SE是JAVA系的杂牌军之一（最终被大公司IBM的各种手段变相的策反、吞并、掺沙子，下套、贴金，最终使SWING+SWT成为了J2SE的核心，最原生的AWT已几乎无人问津，加之IBM的Eclipse系统在近古时代火爆全球，J2SE基本上是SWING+SWT的天下了）。

中古时期，地球上还没有手机，大家都是手持大哥大的存在。一些比较先进的公司生产出了有显示屏的系统，例如：1999年的Symbian OS v5.x，塞班。随着塞班系统的成长，以及移动设备时期的到来，J2ME很是火了七八年。那会儿，下载个电子书都是jar包的

中古时期，JAVA也得挣钱啊，于是搞出个J2EE企业版。这是市场细分战略，专攻企业级市场，很明显的这是最成功的JAVA版本，直至19年后的今天很多人就靠J2EE吃饭。其实J2EE是抄袭，抄自ASP和PHP，所谓的JSP  = Java Server Page ，抄袭自1996年的asp（Active Server Page）和1996年的PHP（Personal  Home  Page），然后增加了自身的语法啊，类啊，对象啊，JDBC啊之类的东西，最终创造了世界上和C语言长期并驾齐驭的神器JAVA。（请各程序员先不要争论谁抄谁的，谁卑鄙谁下流，谁先进谁落伍。从历史发展观来看，语言是否流行，取决于资本市场和大资金财团，JAVA的流行本质上是由于IBM+ORACLE+SUN为JAVA站队，请了大批专业的人员来优化JAVA，掏了大笔的钱来宣传，后来谷歌的Android也增加进来，也在这JAVA市场上投入了大量资金，这才导致了JAVA从1999年至今的19年繁荣。大家想想：如若没有IBM的eclipse，没有ORACLE专门针对JDBC优化出的强力性能，没有SUN的顶级语言专家支持，没有GOOGLE在后移动时代对Android的大力投放，怎么可能会有这辉煌的19年？所以：程序员经常争论的X语言比X语言优秀等言论，从历史发展观来看，很是弱智）

## **近古时代：**

2007年前堪称近古时代，为什么这么划分呢？因为那年我刚刚上大学，刚刚学会用塞班手机，我在最年轻的岁月遇到了最辉煌的诺基亚，也一步一步看见了诺基亚的倒闭。

我们先说企业市场J2EE，近古时代的J2EE有最重要的一块内容，叫做Enterprise   JavaBean，它的简称就是EJB，企业级JAVA对象，很是复杂，我没有研究过，但大概的用法是“听闻过”的，将企业级对象按照EJB标准创建出来，然后托管在专用EJB服务器上，客户端可以通过存根或者其他的手段访问EJB对象，进而完成业务需求。------------听起来很简单，但是很多人在EJB的领域遇到了很多问题，最常见的抱怨是“EJB大概是J2EE架构中唯一没有兑现其能够简单开发并提高生产力承诺的组件”。开发效率低下就是EJB最严重的问题。

因此Spring出现了，也是我们这篇文章的主角。它的作用仅仅是为了“提升开发效率”。这一点它很成功，2004年开始Spring1.0诞生了[一文读懂Spring框架和Spring Boot的历史](https://link.zhihu.com/?target=https%3A//baijiahao.baidu.com/s%3Fid%3D1596689465873929353%26wfr%3Dspider%26for%3Dpc) 。它解决了EJB的一系列问题，**提升了开发效率最终席卷全球**。

再说安卓，安卓是一家小公司开发的，基于delvik虚拟机（寄存器型虚拟机），而非基于HotSpot（堆栈型虚拟机），可以认为它拿走了JAVA的灵魂，而肉体是新造的。所以ORACLE一直在和Google打官司，最终Google输了。安卓、塞班、苹果，也有一段纠结的三角恋。第*一*代*iPhone*于2007发售，第一代安卓手机与2008年发售。iPhone+Android一起在几年内把诺基亚手机事业打入了黄泉路。新闻数据显示，iPhone1的研发时间为2004~2007年，Android的研发时间为2005~2008年。区别在于iPhone为苹果内部召集了1000名工程师开发的，而Android是谷歌收购了Android.lnk，后在系统基础之上增加修改，开发源代码给多家公司最终完成的。这一点上，我看好Android，那些无法控制的东西最终一定会组成洪流冲破一切。

## **近现代：**

2008年后的事情大家大多了解了，从08年至18年这十年，因为种种奇遇，IT界流传一种“新技术能改变世界”的幻想和臆想，纵然：

新技术Android成功了

新技术大数据貌似成功了（骗钱中）

新技术云计算看似成功了（由天下皆云最终迫于现实，只能做共有云和私有云）

新技术高并发高可用成功了，基于此大规模的电子商务流行了五年之久，更是诞生出了阿里、京东等电商巨头

大规模缓存技术诞生了nginx、varnish、F5、CDN网络

搜索引擎技术诞生了百度（昙花一现的百度.......）

虚拟化技术诞生了docker、vmware、openstack等公司

AI技术诞生了面部识别和语音识别

可惜15年后，近三年来，我真的真的没看到什么能改变世界的新技术了

------

## 现代



现代的故事就是17年后了，基本上IT界分为三拨人：

A）相信技术能改变世界的，这波在憋大招，三年了，期望他们成功，让我有生之年赶上这个伟大的时代。（诚然几率很小，我持悲观论者）

B）想挣点钱的，使用现有技术对现实世界进行改造的，如腾讯改造了社交、阿里改造了商务、美团改变了地区结构，京东的时代好像也要过去了？

C）盲目的，对世界一无所知的，只知道学会技术能挣打钱的，殊不知：现实中的技术领域：

一线：实体技术，如新能源、新材料，新制造

二线：人工智能、机器学习

三线：高并发、高可用、虚拟化、大数据

四线：分库分表、消息队列、缓存、服务化、移动APP、H5应用

五线：日志、框架、组件、、、、、、、、、、

C类的人，大部分沉浸于二线半和三线半之后，这已经是混的不错的了，还有一些是四线和五线的小可爱们。

比如楼主所说的Spring，从分类上说属于四线半的技术，挣钱养自己够啦，25岁前你学学这个还行，你一定要迅速精通，30岁的时候你一定要提到三线之前，否则只能告别IT行业了。（那些说35岁退休的，基本上是小于等于四线的，到达三线后混到40岁还不成问题）



\----------------------------------------------------------------

让我们回到主题：Spring一坨一坨的有什么缺点？

从上述历史中我们看到：推动人类文明进化的只有“钱”，JAVA的高歌猛进来源于IBM+ORACLE+SUN+GOOGLE的大幅投入。

Spring流行全球的原因也是“钱”，它**提升了开发效率最终席卷全球**。

那么Spring现在有什么问题呢？任何事务总会有如下历程：生于改革旧时代，兴于解决实际问题，衰老于期望解决更多问题而导致的臃肿，死于太胖了没人再用。



Spring太臃肿了，期望解决全部问题，而不可得。这和人生挺像的，渴望而不可得皆是痛苦。

佛教曰放下即见佛，编程也一样：Spring这么肿，有用的好好学，没用的部分，保持关注但是不深入，你就解决了Spring的臃肿问题。









[编辑于 2018-04-13](https://www.zhihu.com/question/25255223/answer/365759497)





![匿名用户](https://pic1.zhimg.com/aadd7b895_xs.jpg)

匿名用户





缺点还不明显？techempower上spring的得分比php都低了，你能接受这种效率吗？

TechEmpower Framework Benchmarks

再看看vert.x的得分……

还有JEE是一个明显过时已经接近淘汰的玩意，之前不是有传言说Oracle打算放弃JEE么？JEE后来几个版本的标准，得到企业界响应的数量越来越少，这年头还有人想学习JEE，他的脑袋一定是进水了

所以提问者，你的感觉没有错，如果你稍微会一点点英语，去看看国外对于spring的批判，那叫一个铺天盖地，java现在发展最大的制肘就是spring以及jee这一套破烂，硅谷的企业早不用这些东西了，只有一些过时的，陈旧的系统还在跑这些东西

[编辑于 2017-06-15](https://www.zhihu.com/question/25255223/answer/184029598)





[![方志宏](https://pic4.zhimg.com/v2-9573a9f810f25517a81d045c16796ab6_xs.jpg)](https://www.zhihu.com/people/fang-zhi-hong-8)

[方志宏](https://www.zhihu.com/people/fang-zhi-hong-8)

十五年码农



回答可能不太切题，我说的是整个java开发栈的问题。

当你用最流行的springboot+springmvc+spring+mybatis(hibernate)终于完成了一个简单的增查删改，运行起来一看程序调用栈，居然有几十层调用，其中只有一层是你的业务代码，其他都是各种框架的。

[发布于 2018-01-30](https://www.zhihu.com/question/25255223/answer/307445777)





[![kai liu](https://pic2.zhimg.com/135af115125887dc1cc943b480a8bfe6_xs.jpg)](https://www.zhihu.com/people/tittop)

[kai liu](https://www.zhihu.com/people/tittop)

梦想着改变整个中国基础IT环境的妄想者



让很多本不大适合做IT行业的人进入了这个行业

[编辑于 2018-03-06](https://www.zhihu.com/question/25255223/answer/335584818)





[![find goo](https://pic2.zhimg.com/v2-51695b3364c4eee98f5c456f1521f238_xs.jpg)](https://www.zhihu.com/people/find-goo)

[find goo](https://www.zhihu.com/people/find-goo)

MOV AX, BX



好处网上一大堆。

我说一下坏处。

spring技术更新淘汰快，今天这个包依赖，明天新包依赖，各种兼容性问题，今天这个版本明天新版本又有新功能，现在spring就是个大杂会有各种杂技技巧，软件严重依赖spring的特定版本，不方便移植，显然这不适合软件工程要求管理简单的目标。



spring依赖反射，反射影响性能，反射需要执行的代码指令量是纯代码的好几倍，你可以测试一下单纯用java操作数组可能性能直逼c++，但用了spring在调用各个函数时各种反射可能比python还要慢。



spring过度依赖设计模式，没有设计模式一样可以写代码，参考linux这么大的系统可以用c语言写得好好的，偏偏要玩几十个设计模式，这些设计模式并没有解决什么实际问题的痛点，这不是人为增加软件工程的复杂度吗？真正复杂的东西需要算法来解决，而不是设计模式滥用，如语言识别，图像识别，机器学习，设计模式根本解决不了这种复杂性问题。编程本质上是体力活，你老想着发明永动机，玩各种设计模式的技巧，试图代替人来干活根本不现实。



spring过度强调oop，面向对象只是编程的一种模式，不用oop一样可以设计开发大型软件，没有必要过度oop，过度oop会导致软件工程的复杂度上升，oop的封装不是粒度越小越好，适当使用oop封闭代码可以提高软件的可复用性，但过度oop会导致代码逻辑碎片化，增加调试测试的复杂性，本来个函数可以搞定的问题，需要调用interface，需要调用工厂，需要调用反射，这不是有病吗？



spring过度强调动态性，java本身就是静态语言，适合编译前发现问题，spring的出现是弥补这种动态性的不足，但AOP这种把代码写入XML完全是走邪路，一个系统的复杂性不会因为你采用什么语言而降低，虽然语言可以降低额外的复杂度，但对系统的复杂性无能为力，无论你是用汉语还是用英语写红楼梦，写莎士比亚都是一个复杂问题。把代码写入XML会引起XML代码管理问题，增加系统的复杂性，如果你要把代码写入XML不如引入一种脚本语言来解决这种动态性，在动态性要求很高的领域需要的是动态语言而不是用spring去模拟动态语言。如果一味强调动态性，我还不如用python，用pypy加速的python代码速度和java也不相上下。



spring过度强调xml，面向xml编程没有代码调试没有代码编译前检查没有代码提示，无疑是在代码的墙上打洞，根本不适合软件工程追求的简单管理目标，一开始xml代替ini文件用于配置参数，后来疯狂地使用xml来配置orm，再到后来出现orm直接在xml中编程，xml是真的难用根本没有代码调试功能，软件测试对xml的代码测试也很弱，很多网站出现问题都是显示xml配置错误，出现问题根本不如代码可以打断点写日志来得直观。面向xml编程真的是巨坑，在spring中如果你的程序出现问题80%是xml那一段出现了问题。



spring越来越强调云，但云系统在中国根本就不好用，你能用google吗？国内访问国外网站慢一半都是因为googleapi的加速变慢，下载个东西慢得要死，全部从网上下载来历不明的jar包安全性差，而且上国外网站经常出现断网，速度慢的问题，还要花钱买vpn，我还是比较认可微软那种把全家桶做成安装包，需要什么安装什么，安装包大不要紧，我们可以用bt加速下载，不要搞什么网络一个包一个包下载安装，都是拖慢大家的开发速度。在中国任何搞云安装的就是根本不考虑用户体验。























[编辑于 2018-09-24](https://www.zhihu.com/question/25255223/answer/496903383)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户



基于A弄出B，那么如果A有问题的时候B也会有，而B本身也会有问题存在！

方便了，但是风险多了~ 

[编辑于 2019-03-08](https://www.zhihu.com/question/25255223/answer/617490696)





[![dust dream](https://pic1.zhimg.com/04e5285df_xs.jpg)](https://www.zhihu.com/people/food)

[dust dream](https://www.zhihu.com/people/food)

食色性也，唯有爱与美食不可辜负



Spring  现在越来越走向Ruby style 的convention over configuration. Spring Boot 和Spring  IO platform解决了很多问题。但是一旦配置出错，debug  起来对于完全不懂的人难度比较大。但是这仍然是一条正确的路，对大多数程序员和社区是利大于弊的

[发布于 2016-02-11](https://www.zhihu.com/question/25255223/answer/85716304)





[![Vincent Zhang](https://pic4.zhimg.com/20458cff9_xs.jpg)](https://www.zhihu.com/people/vincent-zhang-7)

[Vincent Zhang](https://www.zhihu.com/people/vincent-zhang-7)

马儿被草原困住脚步，人们被星空遮住视线。



Java是静态强类型语言，目标是尽量在编译时发现错误。
用了spring之后，错误都被留到了运行时。

一个框架做的事和这个语言希望达到的目标完全是南辕北辙的，然后它还成为了这个语言最流行的框架，这难道没有很讽刺么？

[编辑于 2018-03-07](https://www.zhihu.com/question/25255223/answer/336094174)





[![Magellans](https://pic2.zhimg.com/f3d0d2de21f0f5310341d63c746bf9be_xs.jpg)](https://www.zhihu.com/people/magellans)

[Magellans](https://www.zhihu.com/people/magellans)





我觉得对于刚开始了解spring的开发者来讲，不太适合去探究spring的缺点，spring毕竟是个应用广泛的成熟框架，自然有他的优点，在足够了解了他的优点后，再去评判他的缺点也不迟。

[发布于 2017-11-26](https://www.zhihu.com/question/25255223/answer/265961659)





[![maintk](https://pic2.zhimg.com/f92d586028854c6a1dc4976ae98cd885_xs.jpg)](https://www.zhihu.com/people/ocmt)

[maintk](https://www.zhihu.com/people/ocmt)

圣堂需要守卫，我正因此而来



好吧. 我是因为觉得spring过于强大找不到缺点才进来看看. 

评论都看了,感觉说缺点的人对spring的理解深度还不够. 我赞同 

[@hitsmaxft](https://www.zhihu.com/people/1ed08b5881d2f866a1b409861e51204a)

的观点

[发布于 2016-04-28](https://www.zhihu.com/question/25255223/answer/97969785)





[![胡翀](https://pic1.zhimg.com/v2-d9ca65a91bfd56c8399f2049a8cd47d6_xs.jpg)](https://www.zhihu.com/people/hu-chong-91-32)

[胡翀](https://www.zhihu.com/people/hu-chong-91-32)





用了spring，你的项目就和spring绑在一起了，想脱离spring做单元测试、mock以及别的一些操作就很困难，你的项目不知不觉会和spring耦合的越来越深，为了方便会引入越来越多的spring的类库，在没研究源码的情况下你对你的项目的把控越来越少，不过这也不是多大的缺点，毕竟业务上的内容能够解耦就行了，关键还是调教不好的情况下很容易莫名其妙起一堆无关的类，完全取决于用的人的水平。

[发布于 2018-01-25](https://www.zhihu.com/question/25255223/answer/303290666)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户





听到好多人说Spring的配置太复杂了，我真的觉得不复杂，使用起来太简单的东西容易隐藏很多细节。

[编辑于 2018-01-11](https://www.zhihu.com/question/25255223/answer/293578315)





[![宝藏在哪](https://pic1.zhimg.com/f2a0bf54de8b5f0c14b5abf2324630ec_xs.jpg)](https://www.zhihu.com/people/love_science)

[宝藏在哪](https://www.zhihu.com/people/love_science)





基本上大体设计是很好的，有一些缺点。
我说几个吧、
1、配置太繁琐，不过这个问题在后续的版本引入annotation配置的机制得到了极大的改善。spring  boot也是构建与这个的基础之上，只不过spring boot做的更多更彻底。spring  boot算是颠覆了spring使用的一般姿势，也由此引入了新的问题，让整个配置逻辑成为一个黑盒，你都不知道哪些bean配置了，当然也不是没发知道，只是这种方式隐藏了太多细节，会让你没办法很直观的知道。
2、spring发展到现在已经提供了很多的产品出来，除了spring-integration和spring-framework，spring-boot之外其他的产品版本策略也基本算是各自为政，完全看不出规律，也不统一。举个例子来说，你如果用了spring-data的某一个类库，那么你将不知道它会依赖spring-data-commons的哪个版本，不知道它会依赖哪个spring-framework的版本。当然这些我们也可以按图索骥根据maven依赖分析出来，只是确实很不直观，很奇怪的版本策略。那这个问题也由spring  boot的出现得到了很大的改善。

还有一些就比如说体现在一些细节的API设计上面了，如果你对spring使用得很深入你也会发现它们有一些API的设计并不合理，对程序员不友好，不过很多API在后续的版本升级中也做了改进，比如提供一个更好的实现了

[编辑于 2018-01-08](https://www.zhihu.com/question/25255223/answer/121297730)





![匿名用户](https://pic1.zhimg.com/aadd7b895_xs.jpg)

匿名用户





比如我想在 spring 下DDD？
谁能告诉我怎么写? 

[发布于 2016-07-04](https://www.zhihu.com/question/25255223/answer/109353718)





[![王锐](https://pic4.zhimg.com/da8e974dc_xs.jpg)](https://www.zhihu.com/people/wang-rui-38-28)

[王锐](https://www.zhihu.com/people/wang-rui-38-28)





spring就没有啥优点，你看到的优点都是作者自己写，国内都是以讹传讹。凡事要保持独立思考能力。

[发布于 2018-01-13](https://www.zhihu.com/question/25255223/answer/295072075)





[![宋欣建](https://pic1.zhimg.com/v2-3664d7f22819dc3007a8f2b25b331f0b_xs.jpg)](https://www.zhihu.com/people/song-xin-jian-74)

[宋欣建](https://www.zhihu.com/people/song-xin-jian-74)

Java Developer



那不用Spring的话，就只能考虑EJB？更复杂。
Vert.x？似乎不够流行，尤其是大企业。
追求简洁的话也许Python更好吧，Java定位于企业级开发，很难简洁下来的，哪怕SpringBoot也是这样。

[编辑于 2018-01-07](https://www.zhihu.com/question/25255223/answer/290944231)





[![叶小怜](https://pic1.zhimg.com/2d4241964b60888e75357ae06a66c7fc_xs.jpg)](https://www.zhihu.com/people/xie-xiao-lian-20)

[叶小怜](https://www.zhihu.com/people/xie-xiao-lian-20)





如果有明显的缺点，早就出现其他技术代替它了

[发布于 2018-03-06](https://www.zhihu.com/question/25255223/answer/335078378)





[![土豆牛肉秋刀鱼](https://pic4.zhimg.com/da8e974dc_xs.jpg)](https://www.zhihu.com/people/tu-dou-niu-rou-qiu-dao-yu)

[土豆牛肉秋刀鱼](https://www.zhihu.com/people/tu-dou-niu-rou-qiu-dao-yu)





开发微服务占用资源大

[编辑于 2018-03-06](https://www.zhihu.com/question/25255223/answer/335076348)





[![Jinglei](https://pic4.zhimg.com/da8e974dc_xs.jpg)](https://www.zhihu.com/people/gu-jing-lei-65)

[Jinglei](https://www.zhihu.com/people/gu-jing-lei-65)

Developer / Hardcore RPG Gamer



Spring刚出来时是和EJB来做对比的 轻量级框架简单易用 但是现在的Spring太庞大了太臃肿了 什么火跟进什么 一大片的组件没有一个总架构图 以至于对于很多人来说只知其然不知其所以然

[编辑于 2018-01-26](https://www.zhihu.com/question/25255223/answer/304371812)





[![Paco Li](https://pic4.zhimg.com/v2-3ce083cac288e90375cdbead2a6b899b_xs.jpg)](https://www.zhihu.com/people/li-jun-ting-98)

[Paco Li](https://www.zhihu.com/people/li-jun-ting-98)

自封DevOps



重，配置地狱。。。
很多本来可以成长为程序员的年轻人，被debuff成春配置员。

[发布于 2018-01-24](https://www.zhihu.com/question/25255223/answer/302498842)





[![五行缺火](https://pic3.zhimg.com/v2-be8d0fd6753688ed6d0c951ff071539d_xs.jpg)](https://www.zhihu.com/people/wu-xing-que-huo-42-5)

[五行缺火](https://www.zhihu.com/people/wu-xing-que-huo-42-5)

java



spring越来越大了，官方源码也是基于gradle模块化构建了，spring  bean jar包  最上层接口resourceloader和factorybean的实现代码也太多了，整整理了我7张纸，bean容器是放到spring上下文中的，spring上下文又放到servlet上下文中，Web结构的系统承受大容量并发访问，Web层要拥有自己的对象池和缓存池，然后spring弄了个beanfactory工厂，还好spring  webmvc和sprinng mvc jar 包就没有那么多类了，看起来整理背后逻辑方便很多了，mvc的jar整体结构清晰，没有bean  jar包那么多不清楚的类了，等我理清了，我就可以把mvc抽出来了，原理也就弄明白了，好费时间哦，慢慢来吧，工具还是要吃透

[发布于 2018-01-16](https://www.zhihu.com/question/25255223/answer/296759947)





[![贾贾贾](https://pic4.zhimg.com/da8e974dc_xs.jpg)](https://www.zhihu.com/people/jia-jia-jia-35-34)

[贾贾贾](https://www.zhihu.com/people/jia-jia-jia-35-34)



菜鸟网络科技有限公司 java高级开发工程师



Spring声称解决类与类之间的耦合问题。现在好了，不但类与类之间会耦合，类与   spring容器还会耦合，真是剪不断理还乱。就当抖个机灵吧，毕竟即便没有Spring，类与类的耦合问题也好不哪去，不过Spring官方是不是也需要考虑一下这个问题并拿出一个比较创新的优化方案呢，左右为难

[编辑于 2018-01-10](https://www.zhihu.com/question/25255223/answer/293339678)





![匿名用户](https://pic1.zhimg.com/aadd7b895_xs.jpg)

匿名用户





动手实现一个除IOC之外的框架。然后你就要考虑整合的问题了。这个时候你就会感受到Spring的好处。

之前在公司偷偷实现了一个简单的ORM框架，后来觉得太辣鸡了就全删掉了。

还有一点就是为什么要用框架吧。

我来这里吐个槽，公司来了个有一两年工作经验的人，徒手代码那叫一个难看。算了算了突然就觉得用框架限制一些根本没有设计功力的人写代码是个很卓越的想法......

而且这样的人很多，包括现在的组长......

现在一般组的分配都是一个组长+副手+新人。我能明显感觉到即使是我们之中最菜的新人，都比他们有想法。:-(

参加工作之前幻想着处处被大佬教做人的场景，现在看来这个行业鱼龙混杂是真的。

[发布于 2018-09-05](https://www.zhihu.com/question/25255223/answer/486161587)





[![eddyzhou](https://pic2.zhimg.com/d5edc836b_xs.jpg)](https://www.zhihu.com/people/eddychou)

[eddyzhou](https://www.zhihu.com/people/eddychou)

programmer



说spring轻量级的，明显是没用过其他语言吧
事实上，包括spring在内的大部分java主流框架，都是over engineering的例子

[编辑于 2017-05-05](https://www.zhihu.com/question/25255223/answer/164935355)





[![Mr Brooks](https://pic4.zhimg.com/v2-f87878238b8903b81f95a8e9d9036d13_xs.jpg)](https://www.zhihu.com/people/winso-5)

[Mr Brooks](https://www.zhihu.com/people/winso-5)

如果无能为力，那便推波助澜



> 程序员用spring等等框架写出来的代码一坨一坨的

那不是  spring的问题，也不是对 spring不熟悉，而是对 java se 及 java ee（jdbc | servlet）不熟悉而已，先把  core java ⅠⅡ 看5遍以上，面试官再问你什么是 bean的控制权翻转和实现原理以及面向切面编程（aop）、面向元（xml |  注解）编程你猜都能说个八九不离十

总之，spring只是工具，没有好坏之分

[编辑于 2018-01-10](https://www.zhihu.com/question/25255223/answer/293115970)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户





缺点就是 把很多JavaEE的东西封装了，在满足快速开发高质量程序的同时，隐藏了实现细节。
你要花一定的时间去理解它。
所以一个好的设计师要对JavaEE还有Spring都有相当的了解才成。
而不是光学些JavaEE的东西。

[发布于 2016-03-24](https://www.zhihu.com/question/25255223/answer/92136088)





[![hitsmaxft](https://pic2.zhimg.com/001d61cf9572e66219a9fac433410f41_xs.jpg)](https://www.zhihu.com/people/hitsmaxft)

[hitsmaxft](https://www.zhihu.com/people/hitsmaxft)

研究语言特性解解闷



同意  

[@Night Silent](https://www.zhihu.com/people/bb0d221dc5704962639ebfc487cd1247)

  观点



大部分人说的 spring， 指的是 spring bean ，  解决了工程中的 DI  。 而Spring 已经是一个庞大无比的框架体系， 从基础的 DI 到事务到 application framework，包罗万象。 

如果说 spring 有什么缺点话， 就是 java 语言本身带来的开发效率问题， 光靠 spring 是解决不了的。

最后， 再好的语言和框架， 也救不了烂程序员，要嘛靠高级程序员管住， 要么找合适的时机换人重构

[编辑于 2015-08-08](https://www.zhihu.com/question/25255223/answer/58321788)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户



配置方式太多，N个公司不重样

[发布于 2018-04-26](https://www.zhihu.com/question/25255223/answer/376987312)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户



缺点是相对的，看和谁比。比如对手是jboss，那一切感觉都挺完美的

[发布于 2018-03-26](https://www.zhihu.com/question/25255223/answer/350740817)





[![何足道](https://pic3.zhimg.com/v2-aa58612097407a92cd15e9c9942c25d1_xs.jpg)](https://www.zhihu.com/people/diqye)

[何足道](https://www.zhihu.com/people/diqye)

何足道哉

我觉得Spring缺点即使再多，它也是不可替代的。   如果有一个足够轻量级别的框架替代Spring，或许你会发现很轻松，但是你一定会发现不用Java更轻松，Ruby,Python这些里面的框架都比Java所谓的轻量框架要爽。不用Spring你会怀疑人生，用Spring你会感叹Java这么每秒！

[发布于 2018-03-09](https://www.zhihu.com/question/25255223/answer/337496467)





[![牛二](https://pic1.zhimg.com/27a2089de027ccd9ad1884cbc13bf3ec_xs.jpg)](https://www.zhihu.com/people/niu-er-20-58)

[牛二](https://www.zhihu.com/people/niu-er-20-58)

显著性物体检测

越发膨胀是最大的缺点。
导致滥用。
滥用中也包括了很多对Spring一知半解甚至不解的同行。

[发布于 2018-03-08](https://www.zhihu.com/question/25255223/answer/336675652)





[![Moonn](https://pic1.zhimg.com/v2-e58fd797cb8b92f64c3f989e37a5395e_xs.jpg)](https://www.zhihu.com/people/moonshining)

[Moonn](https://www.zhihu.com/people/moonshining)

中间件工程师 长期接受内推，Ruby、Golang

培养了大量xml程序员？

[发布于 2018-03-07](https://www.zhihu.com/question/25255223/answer/336252762)





[![风吹裤衩jj凉](https://pic4.zhimg.com/v2-02ea107c17de0082a746b5e194f52bfe_xs.jpg)](https://www.zhihu.com/people/fa-e-13)

[风吹裤衩jj凉](https://www.zhihu.com/people/fa-e-13)

学习JVM的逗逼

有些不需要的东西也被自动化的load进去了，自己都不清楚加载了哪些东西。。。。。。。。

[发布于 2018-03-06](https://www.zhihu.com/question/25255223/answer/335413349)





[![Markorg](https://pic1.zhimg.com/v2-ad8af2d7ab645a8c47a4aa9b7636d95e_xs.jpg)](https://www.zhihu.com/people/mkdiresb)

[Markorg](https://www.zhihu.com/people/mkdiresb)

尽情驰骋吧，纵使天地也太狭小。

缺点就是配置麻烦，但是springboot很好的改善了这个情况，希望spring可以坐的更好。

[发布于 2018-03-06](https://www.zhihu.com/question/25255223/answer/334927569)





![知乎用户](https://pic4.zhimg.com/da8e974dc_xs.jpg)

知乎用户



有啊。所以才要用spring boot

[发布于 2018-03-05](https://www.zhihu.com/question/25255223/answer/334898788)





[![嘎吱喀吧](https://pic4.zhimg.com/7a55a2c8f_xs.jpg)](https://www.zhihu.com/people/ga-zhi-ka-ba)

[嘎吱喀吧](https://www.zhihu.com/people/ga-zhi-ka-ba)

择其善者而无视之，则其不善者而鄙视之。

号称是一个便利的工具箱，实际是一个塞满了各种工具的集装箱，同样一个功能有好多种工具，但是没点资历很难找到最合适的那个。

[发布于 2018-02-28](https://www.zhihu.com/question/25255223/answer/330413213)