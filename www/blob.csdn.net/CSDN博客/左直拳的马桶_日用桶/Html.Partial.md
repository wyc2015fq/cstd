# Html.Partial - 左直拳的马桶_日用桶 - CSDN博客
2017年03月06日 19:38:11[左直拳](https://me.csdn.net/leftfist)阅读数：6785
老革命永远都在遇上各种似是而非的老问题。
这次，是这个Html.Partial，分部页。
Html.Partial与Html.Action有啥区别呢？区别就是，Html.Partial只有一个视图，而Html.Action除了视图，还真的有个Action跟它对应，所以，Html.Action功能比Html.Partial要强。
Html.Partial如何使用呢？
注意看参数如何传递和接收。
代码一，Html.Partial
```
@using BaseLT.Web
@{
    var url = ViewData["Url"].ToString();
}
<button type="button" id="btnSave" class="btn ltblue"><i class="icon-ok"></i> 保存</button>
<script type="text/javascript">
        $(function(){
            $("#btnSave").bind("click",function(){
                $.ajax({
                    url: "@url",
                    data: $('#mainForm').serialize(),
                    type: 'post',
                    success: function (data) {
                        ...
                    },
                    error: function (e) {
                        ...
                    }
                });
            });
        });
</script>
```
代码二、如何调用这个Html.Partial
```
@Html.Partial("_Save", Model, new ViewDataDictionary()
    {
        new KeyValuePair<string, object>("Url", Url.StaticFile("~/Admin/Role/EditSave/") + Model.ROLEID)
    })
```
值得一提的是，分部页与调用页不在同一个目录下，相反，相隔很远。那么如何指明它的位置才能引用呢？要不要这样写：
```
@Html.Partial("~/Views/Shared/_Save"）
```
答案是无须指明，以上写法是错误的。只需将分部页放在根目录下的views/shared/即可，我猜系统会先从调用页所在目录、所在层的shared 查找；然后再从根目录下的views/shared查找。如果
老革命以前遇到的这个问题：
[ASP.NET MVC之Html.RenderAction](http://blog.csdn.net/leftfist/article/details/17115757)
以上不全面，分部页也可以有类似action的，如
```
public PartialViewResult hehe()
        {
            return PartialView();
        }
```
那么分部页与action究竟差别在哪里呢？
我的理解是
1、视图对应完整的页面，分部页对应一段HTML
所以视图如果不设置Layout=null的话，系统会配上默认布局页，但分部页没有这个问题。
2、分部页既可以有action，也可以没有，不强制；但视图一定要有action。如果没有，那跟分部页没有啥区别
3、就算没有action，也照样可以将服务器端变量传给分部页
