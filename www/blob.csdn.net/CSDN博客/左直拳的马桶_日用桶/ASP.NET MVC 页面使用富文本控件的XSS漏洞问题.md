# ASP.NET MVC 页面使用富文本控件的XSS漏洞问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月07日 20:08:15[左直拳](https://me.csdn.net/leftfist)阅读数：1303
目前在做的项目存在XSS安全漏洞！
原因是有一些页面使用了富文本编辑框，为了使得其内容可以提交，为相关action设置了`[ValidateInput(false)]` 特性：
```
[HttpPost]
        [ValidateInput(false)]
        public ActionResult MailPreview(FormCollection collection)
        {
            return View();
        }
```
但问题来了，同一页面还有其他字段内容，现在HTML、js脚本都能提交，不再受限，要么提交的时候全部都检查，否则就有XSS漏洞。我在某个文本框里输入`<script>alert(0);</script>`，提交成功后，页面展示提交内容，居然执行了！
所以说，千万小心不能轻易开启这个`[ValidateInput(false)]` 特性。我觉得根本就不应该开启。可以酱紫：
```
public class MailVM
    {
        public string Title { get; set; }
        public string Time { get; set; }
        [AllowHtml]
        public string Mess { get; set; }
    }
```
针对这个实体类的单个字段设置 [AllowHtml] ，这样提交的时候，系统就会放过该字段。
问题是，action不能使用 FormCollection 来接收提交数据，而应该使用实体类。否则[AllowHtml]特性不起作用。究其原理，这种检查应该是系统的过滤器在拦截，你一个FormCollection，过滤器怎么着力？所以，上面的代码应改为：
```
[HttpPost]
        public ActionResult MailPreview(MailVM model)
        {
            return View();
        }
```
用实体类当然没有FormCollection灵活，不过对于这种特定场合，还是必须的。特别提一下，实体类可以只在提交的时候使用，页面输出时并不要求绑定实体类，页面的文本框等控件，也不一定要写成 @Html.TextBoxFor 这种形式。只需要
```
[HttpPost]
        public ActionResult MailPreview(MailVM model)
        {
            ……
        }
```
系统会自动按照model来装配提交数据。
