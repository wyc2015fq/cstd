# AOP面向切面编程简介 - Big Smile - CSDN博客
2016年12月17日 16:30:25[王啸tr1912](https://me.csdn.net/tr1912)阅读数：4639标签：[aop																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=aop&t=blog)
个人分类：[项目经验																[B/S](https://blog.csdn.net/tr1912/article/category/6184179)](https://blog.csdn.net/tr1912/article/category/6211594)
       AOP这个词相信大家都没有接触太多过，但是实际上你们已经有所接触了，就在设计模式中。AOP所用的思想其实和设计模式是一样的，即在不修改原代码的情况下统一增加或者修改功能。还有，AOP大多用在spring里面，但是本文所写的只是在MVC中的应用，要注意。
# 一、简介
        所谓AOP（Aspect Oriented Programming的缩写）意为
![](https://img-blog.csdn.net/20161217154442804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
面向切面的编程，通过预编译方式和运行期动态代理实现程序功能的统一维护的一种技术。AOP是OOP的延续，是软件开发中的一个热点，也是Spring框架中的一个重要内容，是函数式编程的一种衍生范型。利用AOP可以对业务逻辑的各个部分进行隔离，从而使得业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。
        以上是百度百科的官方解释，但是在我看来，其实AOP更多的是一种思想，一种可以动一发而通全身的思想，AOP其实面对的更多是一种同意的功能或者调用过程来进行编写的一种程序体，从它的第一个词Aspect（切面）也就是指的一个方面，也可以了解到这种方式也就是针对一个方面的实现，这一点其实和mvc中的global文件很像，也是Spring框架中的一个重要内容，是函数式编程的一种衍生范型。利用AOP可以对业务逻辑的各个部分进行隔离，从而使得业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。
        其次就是它的预编译性质可以很好的处理一些错误或者是预先判断一些情况，这也导致了它在设计的时候在判断权限，统一输出某个东西方面变得比较常见。“可以通过预编译方式和运行期动态代理实现在不修改源代码的情况下给程序动态统一添加功能的一种技术。AOP实际是GoF设计模式的延续，设计模式孜孜不倦追求的是调用者和被调用者之间的解耦,提高代码的灵活性和可扩展性，AOP可以说也是这种目标的一种实现。”上面这句话也是对于AOP的很好的诠释。
# 二、在MVC中的实现
        说了这么多了，让我们来实战一下吧，首先我们需要在vs新建一个mvc的项目，选择MVC3或者是4都OK，然后建立一个controller，命名随意，然后建立他的视图，在视图上写上最简单的HelloWorld。
```
<!DOCTYPE html>
<html>
<head>
    <title>Index</title>
</head>
<body>
    <div>
        <a>Hello! World</a>
    </div>
</body>
</html>
```
![](https://img-blog.csdn.net/20161217155037291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        右图是我创建好的MVC解决方案和添加好的controller还有view，在view中写入上面的代码以显示Hello！World。
         运行之后（不会运行的自行百度）是不是就显示出了一个HelloWorld的字样在网页上呢?
         好，接下来我们开始新建AOP的一个文件然后去使用，首先在本项目中新添加一个类，名字叫FilterPublic，在引用的地方添加using  System.Web.Mvc这个引用，然后让这个类继承于ActionFilterAttribute，大家要注意Action这个词，说明这个东西是基于动作的。
        然后我们写入以下代码：
```
public string Message { get; set; }
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            base.OnActionExecuting(filterContext);
            filterContext.HttpContext.Response.Write("Action执行之前" + Message + "<br />");
        }
        public override void OnActionExecuted(ActionExecutedContext filterContext)
        {
            base.OnActionExecuted(filterContext);
            filterContext.HttpContext.Response.Write("Action执行之后" + Message + "<br />");
        }
        public override void OnResultExecuting(ResultExecutingContext filterContext)
        {
            base.OnResultExecuting(filterContext);
            filterContext.HttpContext.Response.Write("返回Result之前" + Message + "<br />");
        }
        public override void OnResultExecuted(ResultExecutedContext filterContext)
        {
            base.OnResultExecuted(filterContext);
            filterContext.HttpContext.Response.Write("返回Result之后" + Message + "<br />");
        }
```
        这个里面就是Filter触发的各种动作，然后我们在controller里面的默认方法做以下修改：
```
[FilterPublic(Message = "Action")]
        public ActionResult Index()
        {
            HttpContext.Response.Write("Action正在执行···<br />");
            return Content("正在返回Result···<br />");
        }
```
然后在controller的class类上面也添加一句[FilterPublic(Message = "Controller")]，然后运行，会出现什么结果呢？
```
Action执行之前Action
Action正在执行···
Action执行之后Action
返回Result之前Action
正在返回Result···
返回Result之后Action
```
        可以看到controller里面的方法在执行之前会去先执行以下我们设置的FilterPublic里面的代码，而且随着里面动作发生时间不同也可以看出了都是哪个方法被触发了。
        但是在controller上面写的基于controller的方法却没有触发，这是为什么呢?
         其实这个问题很简单，就是我们在设置我们的AOP程序的时候没有进行参数的设置，没有让filter进行叠加的运行，这个时候我们只要在写的FilterPublic类的上面加上：[AttributeUsage(AttributeTargets.All, AllowMultiple = true)]，就可以让你设置的各种filter或者叠加的filter进行触发了，这个时候我们再运行一下试试：
```
Action执行之前Controller
Action执行之前Action
Action正在执行···
Action执行之后Action
Action执行之后Controller
返回Result之前Controller
返回Result之前Action
正在返回Result···
返回Result之后Action
返回Result之后Controller
```
       这样一来，在controller上的Filter也被触发了，所以我们使用AOP的方法只需要在我们自己的写的方法或者页面默认的加载方法上面写上一个filter类的标签就可以了。
       那么，如果我们有个功能需要所有的页面都触发，这样岂不是也很麻烦么？别担心，这个时候就轮到我们的global文件发挥作用了。在global.asax文件下面的RegisterGlobalFilters方法下面注册自己写的Filter就可以了：
```
public static void RegisterGlobalFilters(GlobalFilterCollection filters)
{
    filters.Add(new HandleErrorAttribute());
    //把自己的filter定义成全局的filter
    filters.Add(new FilterPublic() { Message = "全局<br/>" });
}
```
然后运行，看结果：
```
Action执行之前全局
Action执行之前Controller
Action执行之前Action
Action正在执行···
Action执行之后Action
Action执行之后Controller
Action执行之后全局
返回Result之前全局
返回Result之前Controller
返回Result之前Action
正在返回Result···
返回Result之后Action
返回Result之后Controller
返回Result之后全局
```
这样全局的触发也就做好了（filter中的message只是用于标识层级的，正式用的时候可以不用定义。）
        这样一看，如果想要快速的添加一个全局都用的方法，只需要新建一个Filter，然后再改一下global文件就可以办到了，是不是很方便呢。
    以上均为本人对于AOP的一些浅浅的研究，如有错误的地方还请读者指正。
