# asp.net mvc 的 视图（view ）的模块化开发 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月28日 23:39:22[左直拳](https://me.csdn.net/leftfist)阅读数：2622
目前所在项目有一个视图，几个模块都涉及到，对应不同的开发人员，虽然有SVN在管理，但代码冲突时有发生。并且大家的代码挤于其中，逻辑比较容易混乱。
将不同部件独立出去，实有必要。
分离方式，我知道的有 3 种：
**1、利用JS动态加载子视图**
将分离内容放置在一个视图里面，视图还可以对应有控制器。主视图加载过程中，或者运行过程中，再用JS去请求、加载这个子视图：
```java
var div = ...
var url = "@Url.Content("~/controller/GetDataByView?id=")" + id;
getViewRequest(url, {}, function(data){
        div.empty().html(data);
    }
);
function getViewRequest(url, params, onsuccess, onerror) {
    $.ajax(
    {
        type: 'get',
        url: url,
        data: params,
        contentType: "text/html; charset=utf-8",
        success: function (data) {
            if (onsuccess != undefined && onsuccess != null) {
                onsuccess(data);
            }
        },
        error: function (data) {
            if (onerror != undefined && onerror != null) {
                onerror(data);
            }
        }
    });
}
```
**2、Html.RenderAction**
这个与1的区别主要是加载方式不同。1是主视图出来后，在前端用JS动态加载，会比较迟才出来；2是与主视图一同输出到前端。
2与3的区别在于，2的数据来源于控制器，3的数据来源于主视图。3没有控制器。
有关Html.RenderAction，可见拙作：
[http://blog.csdn.net/leftfist/article/details/17115757](http://blog.csdn.net/leftfist/article/details/17115757)
**3、Html.RenderPartial**
相当于shtml里面的inclue，<!--#include virtual="/template/css.htm"-->。只是接入一个视图，该视图并没有控制器。数据可由主视图传送给它。
下面是具体的例子：
主视图控制器：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/43240987#)[copy](http://blog.csdn.net/leftfist/article/details/43240987#)
- public ActionResult Index()  
- {  
-     ……  
- //ViewTag是自定义类
-     ViewBag.TagArray = new IList<ViewTag>[2]{list,mylist};  
- 
- return View();  
- }  
主视图：
**[html]**[view plain](http://blog.csdn.net/leftfist/article/details/43240987#)[copy](http://blog.csdn.net/leftfist/article/details/43240987#)
- <divclass="bottomTabs"style="bottom: 36px; left: 240px;">
-     @{Html.RenderPartial("BottomTags", (Object)ViewBag.TagArray);}    
- </div>
其中，BottomTags是子视图名称（BottomTags.cshtml），ViewBag.TagArray是传给它的Model。一定要强制转换，不能直接用ViewBag。
子视图BottomTags
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/43240987#)[copy](http://blog.csdn.net/leftfist/article/details/43240987#)
- @using System.Text;  
- 
- @{  
- //接收Model。在视图里面，Model也是一个类似Html一样的东西，无须定义就可以使用。实际上，Model是ViewData的属性。
-     IList<ViewTag>[] array = (IList<ViewTag>[])Model;  
-     IList<ViewTag> list = array[0];  
-     IList<ViewTag> mylist = array[1];  
- }  
- <ul class="navList clx" id="J_GisSiteNav">……</ul>  
- <script type="text/javascript">……</script>  
如此，子视图可无缝接合于主视图矣。
