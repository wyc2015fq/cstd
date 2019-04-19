# MVC框架使用简介 - Big Smile - CSDN博客
2017年01月01日 15:13:02[王啸tr1912](https://me.csdn.net/tr1912)阅读数：494
        MVC框架，相信大家都不会很陌生，尤其是经历过项目的人，它的大名有耳闻可肯定是由来已久了的。但是MVC的使用上和一般的Asp.net的不同还是了解的很少，下面来对比一下他们的区别。
# 一、直接的区别
        最直接的区别就是Asp.net的页面都是动态的，可以直接拖.net平台带的控件来构造页面的每个形状和布局，完全不用在代码中调试，而且控件的事件触发和属性的改变都可以在后台的cs文件中直接控制，没有任何的困扰。而MVC的页面（Razor）是静态的cshtml文件，他的页面和html是一样的，为静态的页面，没有直接和后台交互的事件，如果想和后面交互的话需要通过Ajax来传值或者直接用post和get来实现调用。
        而且Asp.net的页面是一个直接可以显示的页面，不用特殊的解析途径，而MVC的页面（也就是视图）需要通过Razor引擎来解析实现加载。在构建和显示页面的方面也有所不同，ASP的页面直接显示，而MVC的视图需要通过controller的返回方法来实现页面的显示，有的同志刚刚接触MVC的页面的时候肯定会很不熟悉，因为页面的显示实在是和以前不一样。
我们先建立一个MVC的项目：
![](https://img-blog.csdn.net/20161231204723884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20161231204731956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后添加一个controller，并建立index的视图：
![](https://img-blog.csdn.net/20161231205022854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20161231205057245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在属性里面设定需要启动的controller，然后指定特定的页面
![](https://img-blog.csdn.net/20161231205436887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20161231205510868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：这里如果方法名和视图的名字如果相同的话，直接写return view(); 就可以了。
# 二、可嵌入式页面
        在MVC的页面中用@Html.这个标签可以实现很多的功能，包括局部页面的返回，控件赋值，获取model的值等等的功能，这也是MVC中的一个特色。下面是各种返回的实例：
返回页面：
`@{Html.RenderPartial("../../Views/frmManageUser/addUser");}`
返回数据包：
`<form id="myForm"> @Html.Raw(ViewBag.Message)</form>    把bag中的信息添加到表单中`
加载控件和信息：
`<td>@Html.TextBoxFor(e => e.graduateSchool, new { @tabIndex = 5 })</td>`
