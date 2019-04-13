
# ASP.NET MVC模板 - 追求卓越,做到专业 - CSDN博客


2016年04月22日 16:33:57[Waldenz](https://me.csdn.net/enter89)阅读数：1991


本文为大家演示下MVC3中怎样使用模板页，在传统的webform设计模式中，我们使用masterpage作为模板页，在MVC3 razor视图设计中，我们使用另一种方式作为模板页。
新建一个MVC3项目，在解决方案资源管理中，我们可以看到VIEWS文件夹下面有一个Shared文件夹。在Shared文件夹里面有一个_Layout.cshtml页面。这个就是项目中默认的模板页面。如下图所示 ：
![](http://files.jb51.net/file_images/article/201508/2015811144756889.jpg?201571114485)[       ](http://files.jb51.net/file_images/article/201508/2015811144756889.jpg?201571114485)
**1.新建内容页**
内容页在MVC3中也叫做视图布局页，可以右键选中视图文件夹，通过添加视图来添加内容页，然后选择相应的模板。下面就是我们创建的内容页ViewPageOne.cshtml
![](http://files.jb51.net/file_images/article/201508/2015811144703550.jpg?2015711144715)
使用模板页的页面，会自动在页面中生成代码。来指定模板页，使用Layout来指定具体的模板页。如果内容页中没有Layout属性，则使用的是默认的模板页
[?](http://www.jb51.net/article/70934.htm#)
|1
|2
|3
|4
|5
|6
|@{
|ViewBag.Title
 = "ViewPageOne";
|Layout
 = "~/Views/Shared/_Layout.cshtml";
|}
|<|h2|>ViewPageOne</|h2|>
|

如果Layout指定为null,则表示不使用模板页
[?](http://www.jb51.net/article/70934.htm#)
|1
|2
|3
|4
|@{
|ViewBag.Title
 = "ViewPageOne";
|Layout
 = null;
|}
|

**2.新建模板页**
右键选择Shared文件，然后在菜单中选择添加 新建项，弹出添加新项对话框，然后选择MVC3布局页，这样就添加完成了布局页，下面的我们添加的布局页_LayoutPageOne.cshtml
![](http://files.jb51.net/file_images/article/201508/2015811144840435.jpg?2015711144851)
[
](http://files.jb51.net/file_images/article/201508/2015811144840435.jpg?2015711144851)小猪CMS(PigCms)微信营销源码
还在做微信代理?不如自己搭建平台!
 全国发售源码 国内外均有使用 PigCms





[
](http://files.jb51.net/file_images/article/201508/2015811144840435.jpg?2015711144851)
**3.为某个文件单独指定模板页**
**假如某个页面不想使用默认的模板页，可以单独为其指定模板页，下面的代码把内容页的模板页指定为刚创建的模板页_LayoutPageOne.cshtml**
[?](http://www.jb51.net/article/70934.htm#)
|1
|2
|3
|4
|5
|@{
|ViewBag.Title
 = "ViewPageOne";
|Layout="~/Views/Shared/_LayoutPageOne.cshtml";
|}
|<|h2|>ViewPageOne</|h2|>
|

**4.为某个视图文件夹指定模板页**
**假如想让一个Controller下面的所有视图使用同一个模板文件，可以在该Controller对应的试图文件夹下面创建一个_ViewStart.cshtml文件，然后在_ViewStart.cshtml里面指定所使用的模板页**
![](http://files.jb51.net/file_images/article/201508/2015811144941793.jpg?2015711144954)
**[?](http://www.jb51.net/article/70934.htm#)**
**1**
**2**
**3**
**@{**
**Layout
 = "~/Views/Shared/_LayoutPageOne.cshtml";**
**}**
|

**这样就可以把一个controller下面的视图指定为使用一个模板文件**
**5.页面不使用模板页**
**如果一个内容页面不想使用模板页，可以把_Layout设置为null**
**也可以在添加视图页面的时候不选择模板。**
**[6.@RenderBody](mailto:6.@RenderBody)****()**
**@RenderBody()在模板页中使用表示内容页在模板中的位置。当创建具有模板页的内容页的时候，内容页就呈现在模板页中@RenderBody()所在的位置，一个模板页中只能有一个@RenderBody().**
**[7.@RenderSection](mailto:7.@RenderSection)**
**@RenderSection用于在模板布局中定义一个区域，在内容页可以定义一些内容来填充这个区域，例如内容页中引用的JS文件，可以填充到模板页的section位置。每个内容页的单独一些信息，可以在模板页中这个区域显示。**
**@RenderSection有两个参数，第一个参数用于定义section的名称，第2个参数是布尔类型，如果为TRUE，表示内容页必须定义这个section,如果为false，则表示内容页可定义section，也可以不定义。**
**模板页：**
**[?](http://www.jb51.net/article/70934.htm#)**
**1**
**2**
**3**
**4**
**5**
**6**
**7**
**8**
**9**
**10**
**11**
**12**
**13**
**14**
**15**
**16**
**17**
**<!DOCTYPE
 html>**
**<****html****>**
**<****head****>**
**<****title****>欢迎
 @ViewBag.Title</****title****>**
**</****head****>**
**<****body****>**
**<****p****>第2个模板页</****p****>**
**<****div****>**
**@RenderBody()**
**</****div****>**
**<****div****>**
**备注:**
**@RenderSection("Remark",
 false);**
**</****div****>**
**</****body****>**
**</****html****>**
|

**内容页：**
**[?](http://www.jb51.net/article/70934.htm#)**
**1**
**2**
**3**
**4**
**5**
**6**
**7**
**8**
**9**
**10**
**11**
**12**
**13**
**@{**
**ViewBag.Title
 = "主页";**
**}**
**<****h2****>@ViewBag.Message</****h2****>**
**<****p****>**
**若要了解有关
 ASP.NET MVC 的更多信息，请访问 <****a****href****=****"****[http://asp.net/mvc](http://asp.net/mvc)****"****title****=****"ASP.NET
 MVC 网站"****>****[http://asp.net/mvc](http://asp.net/mvc)****</****a****>。**
**</****p****>**
**@section
 Remark**
**{**
**我是主页**
**}**
**<****p****>欢迎</****p****>**
|

**[8.@RenderPage](mailto:8.@RenderPage)**
**@RenderPage用于表示在一个页面中呈现另一个页面的内容.参数指定要呈现的网页的位置。**
**我们在Shared文件夹下面新建一个footer.cshtml文件，然后里面写上版权信息，然后在模板页中开辟一块位置用来呈现footer页面。**

**[?](http://www.jb51.net/article/70934.htm#)**
**1**
**2**
**3**
**4**
**5**
**6**
**7**
**8**
**9**
**10**
**11**
**12**
**13**
**14**
**15**
**16**
**17**
**18**
**19**
**20**
**21**
**<!DOCTYPE
 html>**
**<****html****>**
**<****head****>**
**<****title****>欢迎
 @ViewBag.Title</****title****>**
**</****head****>**
**<****body****>**
**<****p****>第2个模板页</****p****>**
**<****div****>**
**@RenderBody()**
**</****div****>**
**<****div****>**
**备注:**
**@RenderSection("Remark",
 false);**
**</****div****>**
**<****footer****>**
**@RenderPage("~/Views/Shared/footer.cshtml")**
**</****footer****>**
**</****body****>**
**</****html****>**
|


**[9.@Html.Partial](mailto:9.@Html.Partial)****()与Html.RenderPartial()**
**[.@Html.Partial](mailto:.@Html.Partial)****()与Html.RenderPartial()这两个方法可以用来输出一个分部页，右键添加新建项，选择MVC3分部页，就可以添加一个分部页，相当于一个用户控件，**
**这时候就可以使用者两个方法来输出分部页的内容，当然，这两个方法也可以直接输出视图页和内容页。**
**@Html.Partial()的参数为用户控件名称，也就是分部页名称，返回值为string类型，可以直接输出。**
**@Html.RenderPartial()的参数也为用户控件名称，返回值为VOID,调用的时候直接把内容输出到Response中。**
**用法如下：**
**[?](http://www.jb51.net/article/70934.htm#)**
**1**
**2**
**3**
**4**
**5**
**6**
**7**
**<****div****id****=****"logindisplay"****>**
**@Html.Partial("_PartialLogin")**
**@{**
**Html.RenderPartial("_PartialLogin");**
**}**
**</****div****>**
|

**以上就是ASP.NET MVC3模板页的使用方法**

