# asp.net mvc布局页也有Model属性 - 左直拳的马桶_日用桶 - CSDN博客
2018年10月20日 12:55:49[左直拳](https://me.csdn.net/leftfist)阅读数：141
[asp.net](http://asp.net) mvc的布局页，也有Model属性，并不只是一个公用的静态内容。
```
@model Task
        <ul class="nav nav-tabs ml10">
            @if (Model != null)
            {
                <li class="active"><a href="#Main" data-toggle="tab" onclick='f_tabs.to2("divMain","@Url.Action("Main",new { id = Model.ID})")'>任务明细</a></li>
                <li><a href="#FilesAcc" data-toggle="tab" onclick='f_tabs.to("divFilesAcc","@Url.Action("Acc",new { id = Model.ID})")'>任务附件</a></li>
                <li><a href="#Feedback" data-toggle="tab" onclick='f_tabs.to("divFeedback","@Url.Action("Feedback",new { id = Model.ID})")'>执行过程</a></li>
                <li><a href="#Check" data-toggle="tab" onclick='f_tabs.to("divCheck","@Url.Action("Check",new { id = Model.ID})")'>审核意见</a></li>
            }
            else
            {
                <li class="active"><a href="#Main" data-toggle="tab" onclick='f_tabs.to("divMain","@Url.Content("~/Business/Task/BasicInfo")")'>任务明细</a></li>
            }
        </ul>
```
这个Model，由使用该母版页的视图提供。
