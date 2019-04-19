# 对ASP.NET MVC 的路由一点理解 - 左直拳的马桶_日用桶 - CSDN博客
2016年11月19日 15:14:55[左直拳](https://me.csdn.net/leftfist)阅读数：1617
这个东西，真搞不懂。看了网上的教程和文章，也不懂（也不清楚写那些文章的人自己是否真的懂）。只好靠自己一顿乱摸索。
好比说，下面这个路由：
```
//路由1
config.Routes.MapHttpRoute(
    name: "SysApi",
    routeTemplate: "api/SysManager/{action}/{id}",
    defaults: new { controller = "SysManager", id = RouteParameter.Optional }
);
```
routeTemplate里面有个“SysManager”，从URI结构来看，可以猜出这大概是个Controller； 
然后下面的defaults，开宗明义地写着：controller = “SysManager”。
按照我过往想当然的理解，defaults里的controller = “SysManager”，就是个默认值，当没有指明controller的时候，那么这个默认值可以顶上去。可有可无之物。就这么简单，没深想。
然而，真是可有可无吗？仔细咀嚼：**没有指明controller的时候，这个默认值可以顶上去**，也就是说，[http://localhost/api/SysManager/GetUser/108](http://localhost/api/SysManager/GetUser/108) 这个地址，其实也可以简化写成 [http://localhost/api/GetUser/108](http://localhost/api/GetUser/108)，系统会自动帮我们解析成前面那个地址。事实证明，还真是这样。
然后再看这个路由，有点怪，为啥routeTemplate里面已经有个“SysManager”，defaults里还要来一个。通常好像应该这么写：
```
//路由2
config.Routes.MapHttpRoute(
    name: "SysApi",
    routeTemplate: "api/{controller}/{action}/{id}",
    defaults: new { controller = "SysManager", id = RouteParameter.Optional }
);
```
是的，默认路由都是2这种写法。而路由1是路由2的一个补充，多路由。路由1的含义其实是这样的：
对于 /api/SysManager/GetUser/108 这样的URI，系统会将其路由到控制器SysManager中去。为什么呢？在routeTemplate中的”SysManager”，只是一个普通的字符串而已，它并不是控制器（或者说，系统并不知道它代表着控制器）！由于这个URI没有指明controller，所以系统将其解析到默认控制器SysManager。所以说，routeTemplate中的”SysManager”跟defaults中的其实是两回事。
`api/SysManager/{action}/{id} 并不等于 api/{controller}/{action}/{id} `
这一点，是我以前从未理解的。
只有写成{controller}这种占位符的形式，才是控制器。像{controller}、{action}是系统默认符号，它真的就代表了控制器和action。
另外，像{id}中的id，代表参数id。而且你真的要在方法中将参数写成id，比如：
```
public class TestController : ApiController
    {
        // 可以这样访问： api/Test/5
        public string Get(int id)
        {
            return "value";
        }
        //这个方法只能这样访问：api/Test/?name=leftfist
        public string Get(string name)
        {
            return "value";
        }
    }
```
之所以有以上一些感悟，是因为这两天在集成两个asp.net webapi项目。遇到了一个问题，就是项目A遵循RESTFul原则，地址都是没有action的，像这样：api/User/108；而项目B则有action，地址像这样：api/User/GetUser/108。现在将B集成到A后就有问题了，访问api/User/GetUser/108的时候，会提示”multiple get actions”之类的错误。
这就涉及到路由的问题。我是这样解决的：
```
//增强路由，满足多action
config.Routes.MapHttpRoute(
    name: "SysApi",
    routeTemplate: "api/SysManager/{action}/{id}",
    defaults: new { controller = "SysManager", id = RouteParameter.Optional }
);
//默认路由，满足RESTful原则
config.Routes.MapHttpRoute(
    name: "DefaultApi",
    routeTemplate: "api/{controller}/{id}",
    defaults: new { id = RouteParameter.Optional }
);
```
