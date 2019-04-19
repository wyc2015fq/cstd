# 架构之路（9）：Session Per Request - 文章 - 伯乐在线
原文出处： [自由飞](http://www.cnblogs.com/freeflying/p/5176123.html)
本系列：
- [架构之路（1）：目标](http://blog.jobbole.com/90661/)
- [架构之路（2）：性能](http://blog.jobbole.com/90664/)
- [野生程序员：优先招聘](http://blog.jobbole.com/90749/)
- [架构之路（4）：测试驱动](http://blog.jobbole.com/92173/)
- [架构之路（5）：忘记数据库](http://blog.jobbole.com/92868/)
- [架构之路（6）：把框架拉出来](http://blog.jobbole.com/95524/)
- [架构之路（7）：MVC点滴](http://blog.jobbole.com/104711/)
- [架构之路（8）：从CurrentUser说起](http://blog.jobbole.com/104754/)
*前面的两篇反应很差：没评论没赞。很伤心啊，为什么呢？搞得我好长一段时间都没更新了——呵呵，好吧，我承认，这只是我的借口。不过，还是希望大家多给反馈。**没有反馈，我就只能猜了：前面两篇是不是写得太“粗”了一点？所以这一篇我们尽量详细点吧。*
### Session Per Request是什么
这是一个使用NHibernate构建Web项目惯用的模式，相关的文章其实很多。我尽量用我的语言（意思是大白话，但可能不精确）来做一个简单的解释。
首先，你得明白什么是session。这**不是**ASP.NET里面的那个session，初学者在这一点上容易犯晕。这是NHibernate的概念。
- 如果你对它特别感兴趣的话，你可以首先搜索“Unit Of Work”关键字，了解这个模式；然后逐步明白：session其实是NHibernate对Unit Of Work的实现。
- 如果你只想了解一个大概，那么你可以把它想象成一个临时的“容器”，装载着从数据库取出来的entity，并一直记录其变化。
- 如果你还是觉得晕乎，就先把它当成一个打开的、活动的数据库连接吧。
我们都知道数据库连接的开销是很大的，为此.NET还特别引入了“连接池”的概念。所以，如果能有效的降低数据库的连接数量，对程序的性能将有一个巨大的提升作用。经过观察和思考，大家（我不知道究竟是谁最先提出这个概念的）觉得，一个HTTP request分配一个数据库连接是一个很不错的方案。于是Session Per Request就迅速流行起来，几乎成为NHibernate构建Web程序的标配。
### 为什么又要考虑性能了
我《[性能](http://www.cnblogs.com/freeflying/p/4788494.html)》篇发布了以后，虽然赞数很多，但评论区中争议也还是很大的。但一是评论区后来歪楼了，二是一句话翻来覆去的讲太没意思了，所以我没有再分辨。但Session Per Request就是一个很好的例子，可以说明什么叫做“**性能让位于可维护性”：**
- 如果为了性能，破坏了代码的可维护性，那么我们宁愿不要性能；
- 在能够保证可维护性的前提下，我们当然应该努力的提高性能；
- 较之于在局部（非性能瓶颈处）纠结发力，不如在架构的层面上保证/促进整体性能的提高。
我说提到的“**性能的问题先不管”**，以及“**忘记数据库**”等，是基于矫枉必须过正的出发点，希望能够有振聋发聩的效果。但结果看来不是很好，评论里我还是看到了“SELECT TOP 1 * FROM TABLE WHERE ID>CURRRID”之类的东西。这说明什么？关系数据库不但已经在你脑子里扎根，而且已经把你脑子都塞满了。不是说这样不行，只是这样的话，实在没办法和你谈论面向“对象”。
Session Per Request就是一个已经被广泛采用，行之有效的，能在架构层面提升性能的一个设计。
### UI还是Service
我们仅从Session Per Request的定义，什么Http啊，Request啊，凭直觉就能想到UI层的范畴吧？
网上的很多示例都确实是这么写的。在Application里BuildSessionFactory，在HttpModule中配置：一旦HTTP request到达，就生成一个session；Http request结束，就调用session.flush()同步所有更改。
但是，我们在架构中就已经确立了这样一个原则：UI层不涉及数据库操作。更直观的看，UI层的project连NHibernate.dll的引用都没有。那怎么办呢？
现在想来很好笑，当年我可是费了不少的脑细胞：其实只需要在Service层封装相关的操作，然后在UI层调用Service层即可。
那些把我绕晕了的不靠谱的想法大家可以不用去理会了。如果确实有兴趣，可以思考一下：NHibernate中session是有上下文环境（context）的，我们这里当然应该设置成web，但Service最后会被编译成一个dll，这个dll里能取到HttpContext么？
但在Service里怎么封装，也是一件值得斟酌的事。
### 变异，些许的性能提高
我最后采用的方案是引入BaseService：
首先，在BaseService中设置一个静态的sessionFactory；而且，在BaseService的静态构造函数中给sessionFactory赋值（Build SessionFactory）。这样，就可以保证SessionFactory只生成一次，因为生成SessionFactory是一个开销很大的过程。
` 引入sessionFactory`
```
ASP
			
			
```
public class BaseService
{
    private static ISessionFactory sessionFactory;
    static BaseService()
    {
        string connStr = ConfigurationManager.ConnectionStrings["dev"].ConnectionString;
        sessionFactory = Fluently.Configure()
            .Database(
                MySQLConfiguration.Standard.ConnectionString(connStr).Dialect<MySQL5Dialect>())
            .Mappings(ConfigurationProvider.Action)
            .Cache(x => x.UseSecondLevelCache().ProviderClass<SysCacheProvider>())
            .ExposeConfiguration(
                c => c.SetProperty(NHibernate.Cfg.Environment.CurrentSessionContextClass, "web"))
            .BuildSessionFactory();
    }
```
```
其次，在BaseService中暴露一个静态的EndSession()方法，在Request结束时将数据的变化同步到持久层（数据库）。所以当UI层调用时，不需要实例化一个BaseService，只需要BaseService直接调用即可：
` EndSession`
```
ASP
			
			
```
public class BaseService
{
    public static void EndSession()
    {
    }
}
```
```
然后，我们回头看看前面的说法：“一旦HTTP request到达，就生成一个session；”，所以理论上需要一个InitSession()的方法，生成/提供一个session。但我突然有了点小聪明：有些页面可能是不需要数据库操作的，比如帮助、表单呈现，或者其他我们暂时想不到的页面。那我们无论如何总是生成一个session，是不是浪费了点？
越想越觉得是这么一回事，所以左思右想，弄出了一个方案：**按需生成session**。大致的流程是：
- 尝试获取session；
- 如果“当前环境”中已有一个session，就直接使用该session；
- 否则就生成一个session，使用该session，并将其存入当前环境中。
看来NHibernate支持这种思路，所以提供了现成的接口，可以很方便的实现上述思路：
` 按需获取session`
```
ASP
			
			
```
protected ISession session
        {
            get
            {
                ISession _session;
                if (!CurrentSessionContext.HasBind(sessionFactory))
                {
                    _session = sessionFactory.OpenSession();
                    CurrentSessionContext.Bind(_session);
                }
                else
                {
                    _session = sessionFactory.GetCurrentSession();
                }
                return _session;
            }
        }
```
```
其中CurrentSessionContext就是上文所谓的“当前环境”，在我们的系统中国就是一个HttpContext；我们使用GetCurrentSession()就总是能够保证取出的session是当前HttpContext中已有的session。所有的Service都继承自BaseService，直接调用BaseService中的session，这样就可以有效的保证了Session Per Request的实现。
同学们，这下知道了吧？其实我骨子里还是一个很“抠”性能的人。但这样做究竟值不值？我也不太确定，毕竟这样做一定程度上增加了代码的复杂性，而所获得的性能提升其实有限。
### 总是使用显性事务
如果同学们查看源代码，就会发现，我们的session总是启用了事务。
` 总是使用事务`
```
ASP
			
			
```
protected ISession session
        {
            get
            {
                //......
                if (!_session.Transaction.IsActive)
                {
                    _session.BeginTransaction();
                }
                return _session;
            }
        }
        public static void EndSession()
        {
            if (CurrentSessionContext.HasBind(sessionFactory))
            {
                    //.......
                    using (sessionFromContext.Transaction)
                    {
                        try
                        {
                            sessionFromContext.Transaction.Commit();
                        }
                        catch (Exception)
                        {
                            sessionFromContext.Transaction.Rollback();
                            throw;
                        }
                    }
            }
        }
```
```
在我们传统的观念中，使用“transaction”，会增加数据库的开销，降低性能。但实际上并不是这样的，至少我可以保证在NHibernate和Mysql中不是这样的。
大致的原因有几点：
- 即使不显式的声明事务，数据库也会显式的生成一个事务；
- NHibernate的二级缓存需要事务做保证
详细的介绍请参考：[Use of implicit transactions is discouraged](https://www.hibernatingrhinos.com/products/nhprof/learn/alert/DoNotUseImplicitTransactions)
其实，既然使用了Session Per Request模式，我们即使从业务逻辑上考虑，也应该总是使用“事务”：很多时候一次表单提交要执行多个数据库操作，一些步骤执行了一些报了异常，数据不完整咋办？
### 没有Session.Save()和Update()
前面已经反复说过，在Service中，没有数据库的Update操作。我们是通过：Load()数据 -> 改变其属性 -> 然后在Save()到数据库来实现的。
但同学们查看我们的源代码的时候会发现：“咦？怎么没有Session.Save()这样一个过程？”
首先，大家应该了解NHibernat中的Update()不是我们大多数同学想象的那样，对应着sql里的update语句。它实际上用于多个session交互时的场景，我们目前的系统是永远不会使用的。
然后，NHibernate也不是使用session.Save()来同步session中的数据到数据库的。我们系统中只是偶尔使用session.Save()来暂时的获得entity的Id。
最后，NHibernate中实际上是使用session.Flush()来最终“同步”内存（session）中的数据到数据库的。而我们代码中使用的是session.Transaction.Commit()，这会自动的调用session.Flush()。
因为Session Per Request模式，我们在UI层中，总是会在request结束时调用EndSession()，所以在Service的代码中，看起来就没有了“存储”数据的过程。
### UI层的调用
那么，在UI层的哪里调用EndSession()呢？（因为按需生成session，已经不需要BeginSession()了）
大致来说，有两种方案，一种是使用HttpModule，另一种是利用ASP.NET MVC的filter机制。
我们采用了后者，一则是这样更简单，另一方面是因为：当引入ChildAction之后，从逻辑上讲，Session Per Action更自洽一些。比如一个Request可能包含多个Child Action，将多个Child Action放在一个session里，可能出现难以预料的意外情况。
当然，这样做的不利的一面就是会消耗更多的session，但好在session的开销很小，而且我们使用的“按需生成session”可以降低一些session生成情景。
代码非常简单，如下：
` 调用EndSession()`
```
ASP
			
			
```
public class SessionPerRequest : ActionFilterAttribute
    {
        public override void OnResultExecuted(ResultExecutedContext filterContext)
        {
            #if PROD
            FFLTask.SRV.ProdService.BaseService.EndSession();
            #endif
            base.OnResultExecuted(filterContext);
        }
    }
```
```
#if PROD的使用是为了前后端分离（后文详述）：只有当调用ProdService时才使用以上代码，UI开发人员使用UIDevService时不需要改项操作。
同时，为了避免反复的声明，我们提取出BaseController，由所有Controller继承，并在BaseController上声明SessionPerRequest即可：
` SessionPerRequest声明`
```
ASP
			
			
```
[SessionPerRequest]
    public class BaseController : Controller
    {
    }
```
```
### 其他
由于我们在Action呈现后实现数据的同步（session.Transaction.Commit()），所以我们所有的Ajax调用，没有使用Web API，而是继承自ActionResult的JsonResult。否则，不会触发OnResultExecuted事件，也无法同步数据库。
` AJAX返回JsonResult`
```
ASP
			
			
```
public JsonResult GetTask(int taskId)
        {
            string title = _taskService.GetTitle(taskId);
            return Json(new { Title = title });
        }
```
```
综上，我们实际上是借鉴了SessionPerRequest的思路，实际上采用了按需生成Session、且一个Action使用一个session的实现。可以描述成：SessionPerActionIfRequire，呵呵。
通过SessionPerRequest，我们可以发现架构的一个重要作用：将系统中“技术复杂”的部分封装起来，让开发人员可以脱离复杂琐碎的技术，而专注于具体业务的实现。事实上，采用我们的系统，即使一个不怎么懂NHibernate的普通开发人员，经过简单的介绍/培训，也可以迅速的开始业务领域代码的编写工作。
