# 实现@using{}代码块 - 左直拳的马桶_日用桶 - CSDN博客
2015年09月18日 18:57:28[左直拳](https://me.csdn.net/leftfist)阅读数：3845
前几天，我要写个插件，放在asp.net mvc的view上。这需要写一些扩展HtmlHelper功能的方法。这些方法的一个参数是一段javascript代码。
假如这个参数类型是字符型，当然很简单了。问题是，将一段js代码放在字符串里，写的时候会很不方便，没有VS编辑器的智能提示；同时，JS里面有字符串的话，还要避免与外层的单/双引号冲突。
能不能像razor那样，直接将html或js代码嵌在c#代码里呢？比如像这样：
```
@using(BeginForm()){
    <div>呵呵，我是html代码，我在using代码块里面。</div>
}
```
这段代码输出的浏览器以后，会变成：
```
<form>
<div>呵呵，我是html代码，我在using代码块里面。</div>
</form>
```
就是说，系统会自动在头尾加上”<form>”的开始和结束标记。它是怎么做到的？
其实，凡支持using代码块的对象，都要实现IDisposable接口。在using结尾处，也就是对象结束时，调用了Dispose方法。处理结束标记，就放在Dispose方法里面！
经过一番努力，并研读了asp.net mvc的源代码（听起来挺高大上），书写代码如下：
```
public class TableHeadSorterScript : IDisposable
    {
        private readonly TextWriter _writer;
        private readonly string _endscript;
        private bool _disposed;
        public TableHeadSorterScript(HtmlHelper html, string endscript = ");")
        {
            this._writer = html.ViewContext.Writer;
            this._endscript = endscript;
        }
        public void Dispose()
        {
            Dispose(true /* disposing */);
            GC.SuppressFinalize(this);
        }
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                _disposed = true;
                _writer.Write(_endscript);
            }
        }
        public void EndScript()
        {
            Dispose(true);
        }
    }
    public static class TableHeadSortHelper
    {
        static readonly string beginscript = @"
$(function () { 
    tableHeadSort_init(";
        static readonly string endscript = @"
    );
});";
        /*
            ---------------------------------------------------------
            输出自动初始化表头排序插件的JS脚本
            此脚本在页面装载完成后触发；注意用ajax动态加载页面情况下此脚本无效
            ---------------------------------------------------------
            e.g.
            @using (Html.BeginTableHeadSortAutoInit())
            {
                @:function(url) { location.href = url; }
            }
        */
        public static TableHeadSorterScript BeginTableHeadSortAutoInit(this HtmlHelper helper)
        {
            helper.ViewContext.Writer.Write(beginscript);
            TableHeadSorterScript script = new TableHeadSorterScript(helper,endscript);
            return script;
        }    
   }
```
放在view上
```java
@using (Html.BeginTableHeadSortAutoInit())
{
    @:function(url) { location.href = url; }
}
```
，输出到浏览器，会看到：
```java
$(function () {
    tableHeadSort_init(
        function (url) { location.href = url; }
    );
});
```
