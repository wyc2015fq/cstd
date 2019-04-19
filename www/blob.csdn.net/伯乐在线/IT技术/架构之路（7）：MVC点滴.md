# 架构之路（7）：MVC点滴 - 文章 - 伯乐在线
原文出处： [自由飞](http://www.cnblogs.com/freeflying/p/5014502.html)
本系列：
- [架构之路（1）：目标](http://blog.jobbole.com/90661/)
- [架构之路（2）：性能](http://blog.jobbole.com/90664/)
- [野生程序员：优先招聘](http://blog.jobbole.com/90749/)
- [架构之路（4）：测试驱动](http://blog.jobbole.com/92173/)
- [架构之路（5）：忘记数据库](http://blog.jobbole.com/92868/)
- [架构之路（6）：把框架拉出来](http://blog.jobbole.com/95524/)
*我们目前正在开发中的是[任务管理系统](http://task.zyfei.net/)，一个前端复杂的项目，所以我们先从MVC讲起吧。*
**WebForm**
随着ASP.NET MVC的兴起，WebForm已成昨日黄花，但我其实还很想为WebForm说几句。
没有经历过从ASP向ASP.NET转变的同学，是很难理解当WebForm出现时，程序猿世界的欢呼雀跃的。事实上，我也是在Razor出现之后，才勉勉强强的转向MVC，因为看见<% %>这个东西就怕。我曾经参加过一个升级ASP到ASP.NET的项目，ASP里面乱七八糟的代码看得我眼睛又酸又胀红通通的流泪，一辈子都记得！
WebForm最后生成的html可能会臃肿难看，但其代码页面（.aspx）是相当清爽漂亮的。
既然我们都已经决定采用MVC了，WebForm的不足就不用再多说了。但我们应该努力的学习和借鉴它优秀的地方，这些也是在MVC的开发中会用到的：
- 呈现和页面逻辑相分离。WebForm里由于它的框架本来就显式的区分了aspx和aspx.cs，所以大多数时候我们不会担心这个事情。但MVC里面，我们很容易就在view里面利用ViewModel数据进行运算，模糊Controller和View之间的逻辑界限。这个问题我们将在CurrentUser的时候详细讲解。
- 良好的页面封装和重用。当我们发现页面又反复出现的、大同小异的“部件”时，我们肯定就会想到重用。这就是考验我们功力的时候。我先提一点我想到的：有时候我们宁愿重复不愿重用！这是我得出来的血泪教训。应该是在创业家园项目的评论页面部分，我曾经试图重用所有评论的PartialView，结果惨不忍睹，最后放弃重用，反而海阔天空。其实有一个更好的例子就是WebForm中的GridView和Repeater，从实践上看，反而是简单封装的Repeater更受欢迎，“大而全”的GridView却少有人用。所以封装和重用有一个度的问题。
**RouteTest**
Route功能是MVC的一个重大突破，也是一个重要缺陷。由于没有良好的自动检查机制，在实际的开发过程中，非常容易出错！相信有过开发经验的同学都有体会，有时候老半天都报错：找不到View找不到Action，查来查去就一个拼写错误；有时候新增一条RouteConfig，一会儿其他同事叫起来了，“考！原来是你的设置把我的覆盖了。查了我一下午！”
把时间浪费在这些地方实在是可惜，所以我们解决这个问题的办法是使用单元测试，在PCTest的project中引入了RouteTest。每一次新增RouteConfig，跑一遍单元测试：自己的能过，也不影响别人的，就OK了。
这是单元测试在我们项目的UI层最成功的例子。照理说，MVC的最大的一个好处就是“可测试”，其他地方也应该广泛引入单元测试的，但本人偷懒，另外HttpContext的sealed限制也限制了单元测试的实施（MVC 5应该解决了这个问题），所以目前UI层的单元测试还没有展开。但估计这个工作迟早都得做，现在已经出现了一些手工测试繁琐费事易遗漏的问题了。
**URL/View层级**
MVC现目前的另一个问题是，View很难按多层级组织。比如，我可能需要的View是这样组织的：
![](http://images2015.cnblogs.com/blog/49387/201512/49387-20151202121120437-2088062673.png)
注意Controller也有层级关系设置。我始终觉得这样会更清晰整洁，但如果MVC的框架不能这样进行“层级对应”。如果一定要这样把View分层组织起来，在Action中就必须写出View的全部路径，比如：
```
ASP
			
			
```
public class LogController : Controller
    {
        //
        // GET: /Account/Log/On
        public ActionResult On()
        {
            return View("~/Views/Account/Log/On.cshtml");
        }
    }
```
```
还得专门配置RouteConfig，这也太麻烦了一点。所以，我们就还是尽量按MVC的框架，从URL的设计开始，就尽量是/{Controller}/{action}/{route-parameter}的样式，View也同样，放在Contoller对应的文件夹下即可。
**Partial/ChildAction/EditorTemplate**
当我们需要重用某些“页面片段”时，我们就面临了以上这几种选择。切入的点有很多，我们就只结合我们项目，抽取其最鲜明最容易辨认的特点，直接讲述他们的使用场景：
首先是EditorTemplate。它的特点最明显，是和Post相关的。也就是，当一个“页面片段”的数据，还需要再Post回服务器的时候，我们就必须使用EditorTemplate；如果不使用EditorTemplate，就ViewModel的数据就无法传回（参考：[任务管理系统](http://task.zyfei.net/)代码中/Views/Task/EditorTemplates）。为什么呢？和MVC的ViewModel绑定机制有关，EditorTemplate中的html控件呈现时，会在其name上加上所属父Model的前缀，以便于MVC自动解析post数据并绑定到ViewModel。
如果“页面片段”不需要POST，只负责呈现即可，又该如何选择呢？我们的原则是：
- 如果“页面片段”不需要和服务器端交互，所需要的数据都能从父Model中获得，使用Partial；
- 否则，如果“页面片段”说需要的数据还需要从服务器获得，那就只能使用ChildAction了。
**HtmlHelper**
除了上述几种页面片段的重用，还有通过创建HtmlHelper的扩展方法，自定义一种“页面片段”的呈现方式。这种方式一般是PartialView的一种替代方式，我们通常把“很小很小”（比如一个链接、一个下拉列表等），用处“很多很多”（甚至于跨项目）的可重用html片段用HtmlHelper封装起来。可参考：
- [任务管理系统](http://task.zyfei.net/)项目中的DocumentLink：封装一个总是使用doc.zyfei.net域名的html链接
- Global项目（*还未上传源代码*）中的EnumDropDownListFor：封装一个使用dropdownlist，该dropdownlist由enum填充，使用enum上的[Description]作为呈现文本
**AJAX**
观察我们的Action就可以发现，我们为Ajax提供的Action始终是返回的ActionResult，而不是使用“更先进”的WebApi机制（直接返回int等简单类型）。这主要是因为我们使用了SessionPerRequest机制（主要是为了提高性能），我们让一个Request请求只使用一个session（可先简单的理解为一个数据库连接），亦即：
- 当MVC获得一个Request，需要使用session时，Service生成一个session；
- 然后，在这个Request的整个请求过程中，使用的都将是这个已经生成的session（类似于“单例模式”）；
- 当Request结束后，释放这个session，将所有改动同步到数据库
好了，这里我们的关键点就是什么时候算“Request结束”？我们更进一步的定义它为View呈现完毕的时候，所以利用了Filter机制，在OnResultExecuted()时同步数据库，代码如下：
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
所以，即使Ajax调用，也必须经历一个“View呈现完毕”的过程，才能完成数据同步。
**UIDevService切换**
进行前台开发，不需要连接后台数据库的同学，只需要在MVC项目编译时，输入UIDEV即可（如果要真正的连接数据库，使用PROD），如下所示：
![](http://images2015.cnblogs.com/blog/49387/201512/49387-20151202214954033-920640327.png)
那么，这究竟是如何实现的呢？
总体上来说，我们借用了[autofac](http://autofac.org/)这个类库，实现了所谓的“依赖倒置”
![](http://images2015.cnblogs.com/blog/49387/201512/49387-20151202215514424-1202120171.png)
所以，在MVC的Controller中，我们只使用ServiceInterface而不管其具体实现，如下所示：
```
ASP
			
			
```
private IAuthroizationService _authService;
        public AuthController(IAuthroizationService authService)
        {
            _authService = authService;
        }
```
```
最后，在Global.asax.cs中我们通过条件编译符if…else来确定究竟使用哪一种Service实现：ProdServiceModule，或者UIDevServicemodule
```
ASP
			
			
```
void ResolveDependency()
        {
            var builder = new ContainerBuilder();
            builder.RegisterControllers(Assembly.GetExecutingAssembly());
            builder.RegisterFilterProvider();
#if PROD
            builder.RegisterModule(new ProdServiceModule());
#endif
#if UIDEV
            builder.RegisterModule(new UIDevServicemodule());   
#endif
            container = builder.Build();
            DependencyResolver.SetResolver(new AutofacDependencyResolver(container));
        }
```
```
最后，不要忘了，新引入一个Service时，在ProdServiceModule.cs或者UIDevServicemodule.cs中添加：
```
ASP
			
			
```
builder.RegisterType<RegisterService>().As<IRegisterService>();
```
```
这一章就差不多了吧。下一章我们再讲CurrentUser，并由此引出我们的原则：如何在View、Controller、Service和ViewModel之间划分逻辑（或者责任）。
