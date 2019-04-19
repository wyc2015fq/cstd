# Html.RenderAction传递model - 左直拳的马桶_日用桶 - CSDN博客
2017年08月11日 15:07:17[左直拳](https://me.csdn.net/leftfist)阅读数：2483
asp.net mvc里面，Html.RenderAction怎么传递参数？
这样：
`public static void RenderAction(this HtmlHelper htmlHelper, string actionName, RouteValueDictionary routeValues);`
视图view:
```
<div id="szjc" name="szjc" >
    @{Html.RenderAction("SzJcCs", "Detail",new { lisArgs = Model }); }
</div>
```
后台代码DetailController.cs:
```
public ActionResult SzJcCs(IList<StationArgs> lisArgs)
{
    return View();
}
```
观察代码可知，视图传递参数时，要指明参数名称，且要与action一致。不能这样传：
```
//public static void RenderAction(this HtmlHelper htmlHelper, string actionName, string controllerName, object routeValues);
<div id="szjc" name="szjc" >
    @{Html.RenderAction("SzJcCs", "Detail",Model); }
</div>
```
相关文章： 
[http://blog.csdn.net/leftfist/article/details/60589658](http://blog.csdn.net/leftfist/article/details/60589658)
