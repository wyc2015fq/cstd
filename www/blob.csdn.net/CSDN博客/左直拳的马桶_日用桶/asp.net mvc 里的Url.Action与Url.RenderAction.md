# asp.net mvc 里的Url.Action与Url.RenderAction - 左直拳的马桶_日用桶 - CSDN博客
2018年10月20日 12:46:00[左直拳](https://me.csdn.net/leftfist)阅读数：228
其实，[asp.net](http://asp.net) mvc 里的Url.Action与Url.RenderAction，这根本就是两个不同类的东西，不能相提并论。唯一有联系的，就是都跟action有关。记录下来，主要是做个备忘录。
1、Url.Action()
其实就是输出一个链接。可以传参数
```
<a href="@Url.Action("Main",new { id = Model.ID})">任务明细</a>
```
输出到网页就是：
```
<a href="/YbjzHpm.Web/Business/Task/Main/13">任务明细</a>
```
对应的action代码:
```
public ActionResult Main(long id)
        {
            return View();
        }
```
2、Url.RenderAction()
输出该Action所对应的视图。也可以传参数
```
@model Task
<div>
@{
    Html.RenderAction("Main2", new { model = Model});
}
</div>
```
```
public ActionResult Main2(Task model)
{
	return View("Main",model);
}
```
这个子视图、分部控件，之前有所总结，不赘述。
[Html.RenderAction传递model](https://blog.csdn.net/leftfist/article/details/77097789)
[Html.Partial](https://blog.csdn.net/leftfist/article/details/60589658)
[老革命老问题之相对路径转绝对路径](https://blog.csdn.net/leftfist/article/details/52789468)
