# ASP.NET MVC之Html.RenderAction - 左直拳的马桶_日用桶 - CSDN博客
2013年12月04日 11:59:01[左直拳](https://me.csdn.net/leftfist)阅读数：18192
WEB窗体模式开发惯了，切入MVC模式，好多东西都不懂，每一步都要查资料。
初步得来的一些知识点体会是：
_Layout.cshtml就相当于母版页
然后partical视图（部分视图）就是用户控件。
那么在_Layout.cshtml里要放一个用户控件，要怎么做呢？
假设我有一个“用户控件”：/Partial/LoginInfo，那么在_Layout.cshtml里可以这样使用：
```
<div id="ly_User">
                @{Html.RenderAction("LoginInfo", "Partial");}
            </div>
```
用Html.RenderAction。
```
​
            <div id="ly_User">
                <!-- 跨Area -->
                @{Html.RenderAction("LoginInfo", "Partial", new { Area = "Common" });}
            </div>
​
```
有关用户控件的文件结构如下：
![](https://img-blog.csdn.net/20131204115421078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
画红线的就是相应的“用户控件”
假如要传点参数给这个用户控件，该怎么写呢？
_Layout.cshtml：
```
<div id="ly_Navi">
                @{Html.RenderAction("Navi"
                      , "Partial"
                      , new { parentController = ViewContext.RouteData.Values["controller"].ToString() });}
            </div>
```
将当前页面的Controller信息传给用户控件，注意这个 
`parentController`
是我们自己在这里随意起的，并没有事先声明。
则在用户控件视图_Navi.cshtml 是这样接收：
```
@model NaviModels
<div>
    @{
        StringBuilder sb = new StringBuilder("");
        string controller = ViewContext.RouteData.Values["parentController"].ToString().ToLower();
        @Html.Raw(sb.ToString());
    }
</div>
```
