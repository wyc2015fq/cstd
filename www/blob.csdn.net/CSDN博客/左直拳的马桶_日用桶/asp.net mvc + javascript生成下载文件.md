# asp.net mvc + javascript生成下载文件 - 左直拳的马桶_日用桶 - CSDN博客
2015年11月04日 18:46:28[左直拳](https://me.csdn.net/leftfist)阅读数：3069
近期做的是对现有项目进行重构。WEB FROM改成MVC，其实也算是推倒重来了。
里面有一个导出功能，将数据输出成txt文件，供下载。原先的做法是有一个隐藏的iframe，在这个iframe的页面中设置一个表单form，将相关参数提交到服务器端；而在服务器端，是真的生成一个文件，然后再将文件内容往客户端推送。
好奇怪的做法啊。将内容导出，让用户下载，不必真的生成文件的，因为内容千变万化，没有一次生成，多次使用的可能和必要。完全可以将内容生成后，直接往客户端推送。
其次，我理解原先为何要用一个隐藏的iframe。因为提交页面会导致刷新，为了避免页面内容刷新，于是搞了个隐藏的iframe，让它来负责提交。好是好，就是要多搞一个页面。
在新项目里面，前端，form由JS动态生成；服务器端，直接生成内容并推送：
前端：
```java
<script type="text/javascript">
    var f_pointXY = function () {
        function exportData() {//动态添加表单
            var form = $("<form>");
            form.attr('style', 'display:none');
            form.attr('target', '');
            form.attr('method', 'post');
            form.attr('action', "@Url.StaticFile("~/Common/YongHai/ExportData/")" + $("#txt_SMID").val());
            var input1 = $('<input>');
            input1.attr('type', 'hidden');
            input1.attr('name', 'isExportInput');
            input1.attr('value', document.getElementById("chkInput").checked);
            var input2 = $('<input>');
            input2.attr('type', 'hidden');
            input2.attr('name', 'exportFormat');
            input2.attr('value', document.getElementById("Select1").value);
            $('body').append(form);
            form.append(input1);
            form.append(input2);
            try { form.submit(); } catch (ex) { alert(ex); }
            form.remove();//用完即弃
        }
        return {
            exportData: function () {
                exportData();
            }
        };
    }();
```
服务器端：
```
[HttpPost]
        public ActionResult ExportData(int id, FormCollection collection)
        {
            string content = ...;//生成内容
            Response.Clear();
            Response.Buffer = false;
            Response.ContentType = "application/octet-stream";
            Response.AppendHeader("content-disposition", "attachment;filename=" + DateTime.Now.ToString("yyyy-MM-dd") + ".txt;");
            Response.Write(content);
            Response.Flush();
            Response.End();
            return new EmptyResult();
        }
```
这样，点击按钮导出按钮
`<input type="button" value="导出" class="btn mini minilt" onclick="f_pointXY.exportData()" />`
之后，即可下载yyyy-MM-dd.txt矣。
猴赛雷。
