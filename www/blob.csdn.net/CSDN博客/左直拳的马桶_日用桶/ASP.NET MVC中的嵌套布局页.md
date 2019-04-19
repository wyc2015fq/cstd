# ASP.NET MVC中的嵌套布局页 - 左直拳的马桶_日用桶 - CSDN博客
2013年12月25日 16:44:46[左直拳](https://me.csdn.net/leftfist)阅读数：21924
在WEB窗体模式中，用惯了母版页，并且常有母版页嵌套的情况。
而在MVC模式下，对应母版页的，称作为布局页。默认的布局页为 ~/Views/Shared/_Layout.cshtml。默认每个页面都会嵌于其中，因为在~/Views/_ViewStart.cshtml里已经写好：
```
@{
    Layout = "~/Views/Shared/_Layout.cshtml";
}
```
当然，这个应该可以改的，但一般没有必要。我的做法是，将_Layout.cshtml作为基本母版页，然后再在上面衍生出各种子母版页。以下分别是_Layout.cshtml和某一子布局页(_SingleContent_Layout.cshtml)的代码：
_Layout.cshtml
```
@using common = www.AppCode.Common;
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width" />
    <title>@common.GetTitle(ViewBag.IndependentTitle,ViewBag.Title)</title>
    @Styles.Render("~/Content/css")
    @Scripts.Render("~/bundles/modernizr")
	<!-- 子页插入到head的入口。相当于母版页里的ContentPlaceHolder -->
    @RenderSection("header", required: false)
</head>
<body>
    <div>
        <div id="ly_Top" class="ly_960">
            <div id="ly_User">
				<!-- 登录信息。相当于用户控件 -->
                @{Html.RenderAction("LoginInfo", "Partial");}
            </div>
            <div id="ly_Web">
            </div>
        </div>
        <div id="ly_NaviContainer">
            <div id="ly_Navi">
                <div class="ly_960">
                    <div id="ly_Logo" onclick="location.href='/'"></div>
                    <div>
						<!-- 导航条。相当于用户控件 -->
                        @{Html.RenderAction("Navi"
                              , "Partial"
                              , new { parentController = ViewContext.RouteData.Values["controller"].ToString() });}
                    </div>
                </div>
            </div>
        </div>
        <div id="ly_Main">
			<!-- 输出嵌套页 -->
			@RenderBody()
        </div>
   </div>
    @Scripts.Render("~/bundles/jquery182")
	<!-- 子页插入到页面底部的入口。相当于母版页里的ContentPlaceHolder -->
    @RenderSection("scripts", required: false)
</body>
</html>
```
子母版页_SingleContent_Layout.cshtml
```
@{
    Layout = "~/Views/Shared/_Layout.cshtml";
}
@section header{
    @Styles.Render("~/Content/SingleContent")
	@* 这是本子布局页的子页插入内容的入口，注意到没？它写在本子布局页插入母版页的入口处*@
    @RenderSection("header", required: false)
}
@section Scripts {
    @RenderSection("scripts", required: false)
}
<div class="content_box container">
    <div class="content_box_in">
        @RenderBody()
    </div>
</div>
```
内容页index.cshtml
```
@{
    ViewBag.Title = "Service";
    Layout = "~/Views/Shared/_SingleContent_Layout.cshtml";
}
<div class="service">
    <h2>服务条款</h2>
</div>
```
参考文章：
[http://www.cnblogs.com/haiyabtx/archive/2012/06/12/2545821.html](http://www.cnblogs.com/haiyabtx/archive/2012/06/12/2545821.html)
