# ASP.NET MVC 多路由规则应用一例 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月01日 15:16:48[左直拳](https://me.csdn.net/leftfist)阅读数：1120
背景，ASP.NET MVC里，页面路径一般是 `~/Area/Controller/Action`。
同事为了菜单路径有所区别，写了一堆控制器。但是按照业务结构，这些控制器都应该从属于某个控制器之下，现在却变成与之并列了。
其实可以通过设置路由来解决这个问题。即取消这堆“子”控制器，将它们的代码移植到“父”控制器中，然后在路由规则里假装还有这些控制器。
路由规则：
```
public override void RegisterArea(AreaRegistrationContext context) 
{
    context.MapRoute(
        "OneMap_RedTide",
        "OneMap/RedTide/{action}/{id}",
        defaults: new { controller = "Pop", action = "Index", id = UrlParameter.Optional }
    );
    context.MapRoute(
        "OneMap_AAlgae",
        "OneMap/AAlgae/{action}/{id}",
        defaults: new { controller = "Pop", action = "Index", id = UrlParameter.Optional }
    );
    context.MapRoute(
        "OneMap_Cyanobacteria",
        "OneMap/Cyanobacteria/{action}/{id}",
        defaults: new { controller = "Pop", action = "Index", id = UrlParameter.Optional }
    );
    context.MapRoute(
        "OneMap_Protozoa",
        "OneMap/Protozoa/{action}/{id}",
        defaults: new { controller = "Pop", action = "Index", id = UrlParameter.Optional }
    );
    //以上4条路由规则，都实质指向控制器Pop（原理详见底部相关文章），但访问地址都可以各自写成假的控制器：
    /*
    ~/OneMap/RedTide/**
    ~/OneMap/AAlgae/**
    ~/OneMap/Cyanobacteria/**
    ~/OneMap/Protozoa/**
    等同于：
    ~/OneMap/Pop/**
    ~/OneMap/Pop/**
    ~/OneMap/Pop/**
    ~/OneMap/Pop/**
    */
    //系统默认路由
    context.MapRoute(
        "OneMap_default",
        "OneMap/{controller}/{action}/{id}",
        defaults: new { controller = "Home", action = "Index", id = UrlParameter.Optional }
    );
}
```
相关文章： 
[对ASP.NET MVC 的路由一点理解](http://blog.csdn.net/leftfist/article/details/53230069)
context.MapRoute( 
    “OneMap_Hlx”, 
    “OneMap/Hlx/Ais/{id}”, 
    defaults: new { controller = “Pop”, action = “Hlx_Ais”, id = UrlParameter.Optional } 
);
控制器、action都是另有所指。
```
config.Routes.MapHttpRoute(
    name: "BetweenApi",
    routeTemplate: "{controller}/{code}/{st}/{et}",
    defaults: new { code = RouteParameter.Optional,
        st = RouteParameter.Optional,
        et = RouteParameter.Optional,
    }
);
```
`http://localhost/ybjzcheck.api/hyz/BAO/2017-09-01/2017-09-03`
