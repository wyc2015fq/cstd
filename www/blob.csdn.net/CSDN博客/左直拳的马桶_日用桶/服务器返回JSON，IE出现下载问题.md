# 服务器返回JSON，IE出现下载问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年03月14日 17:50:40[左直拳](https://me.csdn.net/leftfist)阅读数：1400
我向来的观点，IE就是个奇葩。
服务器返回json，chrome处理得好地地，但IE却奇葩地向你请求是否要保存这个JSON文件？
之所以出现这种弱智现象，是因为IE无法识别一个所谓的响应头部：`application/json`
那如何处理？这样子： 
服务器端：
```
public ContentResult OperateResult(bool ok = true,string msg = "保存成功")
        {
            return new ContentResult
            {
                ContentEncoding = Encoding.GetEncoding("utf-8"),
                //ContentType = "application/json",//IE说它不认识
                ContentType = "text/plain;charset=UTF-8",
                Content = $@"{{""ok"" : ""{ok.ToString().ToLower()}"",""message"" : ""{msg}""}}"
            };
        }
```
前端：
```java
$('#mainForm').ajaxSubmit(
                    url: "@url",
                    dataType: 'json',//<---------------
                    type: 'post',
                    success: function (data) {
                        if (data.ok == "true") {
                            toastr.success(data.message);
                        } else {
                            toastr.error(data.message);
                        }
                    },
                    error: function (e) {
                        toastr.error(e);
                    }
                });
```
其他什么在服务器`ContentType = "text/html"`，然后客户端 `$.parseJSON(xhr.responseText);`  都是扯蛋。
===================================== 
事实上，在服务器端（C#）这样写，存在同样的问题：
```
public ActionResult OperateResult(bool ok = true,string msg = "保存成功")
        {
            return Json(new {ok = ok,message = msg });
        }
```
====================================== 
在 asp.net搞的WebApi里面，通常以json格式返回给前端。那么在服务器端索性将消息头部类型设为JSON方式:
`config.Formatters.JsonFormatter.SupportedMediaTypes.Add(new MediaTypeHeaderValue("text/html"));//改为JSON方式`
因为 IE可以识别 “text/html” ，所以没有上面这个问题。但由于更改了返回格式，所以想返回页面已经不可能。不过，既然是WebAPI，谁要页面呢？
