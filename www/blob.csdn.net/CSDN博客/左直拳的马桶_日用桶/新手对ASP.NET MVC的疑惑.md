# 新手对ASP.NET MVC的疑惑 - 左直拳的马桶_日用桶 - CSDN博客
2014年01月25日 16:46:27[左直拳](https://me.csdn.net/leftfist)阅读数：1934标签：[MVC](https://so.csdn.net/so/search/s.do?q=MVC&t=blog)
个人分类：[ASP.NET MVC																[学习笔记																[.NET](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2783751)
习惯了多年的WEB FORM开发方式，突然转向MVC，一下子懵了，晕头转向，好多不习惯，好多不明白，直到现在也没弄明白，只好先记下来，在应用中一一求解。
主要集中在视图（View）这里。
1、@Html
视图这里到处是@Html.***，那么这个Html从哪里冒出来的？也不见有声明。
实际上Html是ViewPage的一个HtmlHelper类型的属性，因此它无需声明。
2、也跟这个HtmlHelper有关。
扩展一下这个原始类型HtmlHelper，比如有以下代码
```
namespace System.Web.Mvc.Html
{
    public static class HtmlExtensions
    {
        public static string Span(this HtmlHelper helper, string strId, string strContent)
        {
            return string.Format("<span id=\"{0}\">{1}</span>", strId, strContent);
        }
    }
}
```
在视图页面，调用如下
`@Html.Span("span1","这是一个span")`
看到没，定义的时候明明是3个参数，怎么调用时候只有2个？
说明我长时间闭塞，已经落后太多了。
实际上，第一个参数有this修饰符，代表该方法用于第一个参数这种类型的对象。比如在上面的代码中，Span这个方法，就是用于HtmlHelper这种类型的对象。
3、Model
在视图里面，Model也是一个类似Html一样的东西，无须定义就可以使用。实际上，Model是ViewData的属性。但不知道为什么，像ViewPage、ViewData这些东西都不用显示出来的。
4、lambda表达式
比如 @Html.TextBoxFor(m => m.Name)
嗨！这个m又是从哪里冒出来的？总不至于，它又是谁谁谁的属性吧。其实，将m换成其他名字一样成立，比如 @Html.TextBoxFor(y => y.Name)，一点问题没有。
lambda表达式其实就是匿名函数，如 x => x*x，x是参数，x*x是函数体。并且，系统会根据函数体来猜测这个x的类型，比如在这里，x估计就是数值型。
同样，在视图里面，我估计是有个视图引擎之类的东西会根据lambda表达式来猜测出我们想用的是那个Model对象，并且做了相应的处理。
这并没有什么好奇怪的。就好比做网页，我们写了一些HTML代码，再嵌上一点CSS、javascript什么的，结果就出现了绚丽多彩的网页。这几行代码就有如此魔力？不是的，关键在于浏览器，正因为浏览器根据代码做了相应的处理和呈现，才有多姿多彩的网页，核心在于浏览器。如果用记事本打开，将只是几行代码而已。
