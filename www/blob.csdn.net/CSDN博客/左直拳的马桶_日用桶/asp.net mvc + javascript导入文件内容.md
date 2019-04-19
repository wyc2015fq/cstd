# asp.net mvc + javascript导入文件内容 - 左直拳的马桶_日用桶 - CSDN博客
2015年11月04日 19:08:03[左直拳](https://me.csdn.net/leftfist)阅读数：2771
。近期做的是对现有项目进行重构。WEB FROM改成MVC，其实也算是推倒重来了。
里面有一个导入功能，将文件上传。原先的做法是有一个隐藏的iframe，在这个iframe的页面中设置一个表单form，form中有一个文件上传控件。而在服务器端，接收到上传文件后，真的先将文件保存，然后读取之。
好奇怪的做法啊。将用户文件内容读入，然后利用之，不必真的将文件先保存下来的，因为这是临时文件，没有保存的价值。完全可以将上传的文件内容直接读出，用完即弃。
其次，我理解原先为何要用一个隐藏的iframe。因为上传文件需要提交，而提交页面会导致刷新，为了避免页面内容刷新，于是搞了个隐藏的iframe，让它来负责提交。好是好，就是要多搞一个页面。
在新项目里面，前端，文件由ajax负责提交；服务器端，接收文件后直接处理，不保存下来，用完即弃：
前端：
```xml
<!-- 隐藏的上传控件，onchange时就触发提交 -->
<input type="file" id="fileImportData" name="fileImportData" onchange="f_pointXY.importData()" style="display:none;" />  
<!-- 导入按钮，点击后就触发上传控件的点击事件，弹出文件选择框 -->
<input type="button" value="导入" class="btn mini minilt" onclick='javascript:document.getElementById("fileImportData").click();' />
```
```java
<script type="text/javascript">
    var f_pointXY = function () {
        function importData() {
            $("#formImport").ajaxSubmit({
                url: "@Url.StaticFile("~/Common/YongHai/importData/")" + $("#txt_SMID").val(),
                type: 'post',
                data: {},
                success: function (data) {
                    if (data.substr(0, 2) == "OK") {
                        alert("导入成功");
                    } else {
                        alert(data);
                    }
                },
                error: function (e) {
                    alert(e);
                }
            });
        }
        return {
            importData: function () {
                importData();
            }
        };
    }();
</script>
```
服务器端：
```
[HttpPost]
        public ActionResult importData(int id)
        {
            if (Request.Files.Count == 0)
            {
                return Content("接收上传数据失败", "text/html");
            }
            string msg = "OK";
            HttpPostedFileBase fb = Request.Files[0];
            string[] Acs_FilePath = fb.FileName.Split('\\');
            string Acs_FileName = Acs_FilePath[Acs_FilePath.Length - 1];
            string Acs_FileType = Acs_FileName.Split('.')[1].ToLower();
            string xyType = Request["xyType"] ?? "";
            using (Stream fs = fb.InputStream)
            {
                    int fileLen = fb.ContentLength;
                    byte[] input = new byte[fileLen];
                    fs.Read(input, 0, fileLen);
                    string str = System.Text.Encoding.UTF8.GetString(input);
                    msg += str;
            }
            END:
            return Content(msg, "text/html");
        }
```
这样，点击导入按钮，即可上传并导入矣。
猴赛雷。
