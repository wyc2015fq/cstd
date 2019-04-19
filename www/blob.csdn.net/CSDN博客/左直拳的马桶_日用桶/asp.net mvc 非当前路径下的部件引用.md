# asp.net mvc 非当前路径下的部件引用 - 左直拳的马桶_日用桶 - CSDN博客
2018年08月22日 14:38:11[左直拳](https://me.csdn.net/leftfist)阅读数：141
个人分类：[Web																[.NET](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/674421)
asp.net mvc的视图里面，引用一个部件通常是这样写的：
```java
//最简单
@Html.Partial("_MiniArgs")
```
```
//带参数
@Html.Partial("_MiniArgs", Model, new ViewDataDictionary()
{
    new KeyValuePair<string, object>("cols",7)
})
```
这样写的话，这个部件应该位于视图同一目录，或者所在区域（Area）的Shared下面：`Areas/本区域/Views/Shared/`，或者是根目录下的Shared: `~/Views/Shared/`
但如果这个部件位于别的区域下呢？那应该这样写
`@Html.Partial("~/Areas/本区域/Views/Shared/_MiniArgs.cshtml")`
或者
```
@Html.Partial("~/Areas/本区域/Views/Shared/_MiniArgs.cshtml", Model, new ViewDataDictionary()
{
    new KeyValuePair<string, object>("cols",7)
})
```
引用部件，跟引用布局页差不多的。
