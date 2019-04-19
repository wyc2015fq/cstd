# ASP.NET MVC 提供与访问 Web Api - 左直拳的马桶_日用桶 - CSDN博客
2016年01月08日 19:17:10[左直拳](https://me.csdn.net/leftfist)阅读数：1862
ASP.NET MVC 提供与访问 Web Api
一、提供一个 Web Api 
新建一个项目，类型就选 “Web Api”。我用的是MVC5，结果生成的项目一大堆东西，还编译不过，真操蛋。用nuget装了好一阵才跑通。我估计MVC Web项目也可以的，甚至Web Form应该都行。
下面是一个Action。主要是想返回json数据。
```
public ContentResult GetUser()
        {
            return new ContentResult {
                ContentEncoding = Encoding.GetEncoding("utf-8")
                ,ContentType = "application/json"
                ,Content = @"{
  ""code"" : ""0",
  ""desc"" : ""请求数据成功！",
  ""dataobj"" : [ {
    ""action"" : ""I",
    ""userCode"" : ""001",
    ""userName"" : ""张三""
  }, {
    ""action"" : ""U",
    ""userCode"" : ""002",
    ""userName"" : ""李四""
  } ]}" };
        }
```
```
返回类型用JsonResult也可以，但无法应用预先拼好的json字符串。
```
二、访问Web Api
```
//url = "http://192.168.0.179/WebApi/Data/GetUser"
        protected static string getJson(string url)
        {
            string json;
            using (WebClient webClient = new WebClient())
            {
                webClient.Encoding = Encoding.GetEncoding("utf-8");
                json = webClient.DownloadString(url);
            }
            return json;
        }
```
