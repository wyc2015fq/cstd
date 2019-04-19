# JSON参数 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月06日 00:33:40[左直拳](https://me.csdn.net/leftfist)阅读数：3862
JSON(JavaScript Object Notation，JavaScript 对象表示法），多么简单，不就是键值对嘛。
可是每次在前后端之间通过json作为参数传递，我都心烦意乱，甚至吓到面无人色。
何故？因为没搞懂咯。
现在也是一知半解。是时候做一个总结了。
1、前端传送给后端
```java
$.ajax({
    url: "/api/customview/SetTags?projectId=0&account=leftfist",
    data: JSON.stringify(jsonObj),//这个JSON.stringify函数是js自带的，作用是将JSON对象序列化为字符串
    dataType: "json",
    type: "POST",
    contentType: "application/json; charset=utf-8",
    success: function (data) {//成功后回调函数
    	alert(data);
    },
    error: function (XMLHttpRequest, textStatus, errorThrown) {
        alert(textStatus + ": " + errorThrown);
    }
});
```
上述例子中，URL含有参数，但json对象不在这些参数中。而是以提交的方式，放在消息体里面传送。http协议，前端到后端，是由请求行 + 请求报头 + 消息正文组成的，后端到前端，是由响应行 + 相应报头 + 消息正文组成。我估计，json对象，即放在消息正文中。
2、后端接收前端
后端，我这里以asp.net web api为例。
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42406489#)[copy](http://blog.csdn.net/leftfist/article/details/42406489#)
- [System.Web.Http.HttpPost]  
- publicbool SetTags(int projectId, string account, SetViewTag svt)  
- {//此为对应前端的请求API:url: "/api/customview/SetTags?projectId=0&account=leftfist"
-     List<ViewTag> lisVt = new List<ViewTag>();  
- 
-     assemblyViewTagList(ref lisVt,svt.Updated,ViewTag.EChangeType.Update);  
-     assemblyViewTagList(ref lisVt, svt.Deleted, ViewTag.EChangeType.Remove);  
- 
- return customViewService.SetTags(projectId,account,lisVt);  
- }  
- void assemblyViewTagList(ref List<ViewTag> lisVt,string strJson,ViewTag.EChangeType ctype)  
- {  
- if (strJson.Length == 0) return;  
- 
- //JsonConvert.DeserializeObject<> 是Newtonsoft.Json 的方法，作用是将JSON字符串反序列化，转换回JSON对象
- //注意里面的泛型
-     List<ViewTag> items = JsonConvert.DeserializeObject<List<ViewTag>>(strJson);  
- foreach (ViewTag vt in items)  
-     {  
-         vt.ChangeType = (byte)ctype;  
-         lisVt.Add(vt);  
-     }  
- }  
- 
- publicclass SetViewTag  
- {  
- publicstring Updated { get; set; }  
- publicstring Deleted { get; set; }  
- }  
- publicclass ViewTag  
- {  
- publicint ViewId { get; set; }  
- publicstring Name { get; set; }  
- publicbool IsValid { get; set; }  
- publicint Seq { get; set; }  
- publicbyte ChangeType { get; set; }  
- 
- publicenum EChangeType : byte { NoChanged = 0,Update,Remove}  
- }  
3、前端接收后端
假设后端有WCF的接口：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42407131#)[copy](http://blog.csdn.net/leftfist/article/details/42407131#)
- <span style="font-size:10px;">        staticreadonly DateTime dtZone = new DateTime(1970, 1, 1, 0, 0, 0);  
- public Stream GetUTC()  
-         {  
-             DateTime utc = DateTime.Now.ToUniversalTime();  
- return GetStream(String.Format(@"{{""data"":""{0}""}}", (long)utc.Subtract(dtZone).TotalMilliseconds));  
-         }</span>  
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42407131#)[copy](http://blog.csdn.net/leftfist/article/details/42407131#)
- <span style="font-size:10px;">        /// <summary>
- /// 辅助方法，用于输出流
- /// </summary>
- /// <param name="str"></param>
- /// <returns></returns>
- private Stream GetStream(string str)  
-         {  
-             MemoryStream ms = new MemoryStream();  
-             StreamWriter sw = new StreamWriter(ms);  
-             sw.AutoFlush = true;  
-             sw.Write(str);  
-             ms.Position = 0;  
-             WebOperationContext.Current.OutgoingResponse.ContentType = "text/plain";  
- return ms;  
-         }</span>  
前端则有
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/42407131#)[copy](http://blog.csdn.net/leftfist/article/details/42407131#)
- function getLocationTime(){  
- var url = _webUrl + "/Attendance.svc/GetUTC";  
- var xhr;  
-     xhr = new XMLHttpRequest();  
- if (xhr) {  
-         xhr.onerror = function () { alert("erro"); };  
-         xhr.ontimeout = function () { alert("Time out"); };  
-         xhr.onload = function () {  
- var data = $.parseJSON(xhr.responseText);  
-             serverDateTime = new Date();  
-             serverDateTime.setTime(data.data * 1);  
- 
-             timeId = window.setInterval(getCalTimes, 1000);  
-         };  
-         xhr.open("get", url, true);  
- //xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
-         xhr.send(null);  
-     }  
- else {  
-         alert("Failed to create");  
-     }                 
- }  
前端这里使用了 XMLHttpRequest()，主要是照顾IE。如果用纯ajax，可以：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/42407131#)[copy](http://blog.csdn.net/leftfist/article/details/42407131#)
- $.ajax({    
-     url: _webUrl + "/Attendance.svc/GetUTC";,    
-     type: "GET",    
-     success: function (data) {//成功后回调函数  
- //假如这个data是json字符串，则 obj 得到json对象
- var obj = eval("(" + data + ")");  
-     },    
-     error: function (XMLHttpRequest, textStatus, errorThrown) {    
-         alert(textStatus + ": " + errorThrown);    
-     }    
- });    
3、前端接收后端-2
asp.net web api 默认的结果传送方式是XML。但可以设置为JSON。方法为在WebApiConfig里加入一句：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42434763#)[copy](http://blog.csdn.net/leftfist/article/details/42434763#)
- publicstaticclass WebApiConfig  
- {  
- publicstaticvoid Register(HttpConfiguration config)  
-     {  
- // Web API 路由
-         config.MapHttpAttributeRoutes();  
- 
-         config.Routes.MapHttpRoute(  
-             name: "DefaultApi",  
-             routeTemplate: "api/{controller}/{action}/{id}",  
-             defaults: new { id = RouteParameter.Optional }  
-         );  
-         config.Formatters.JsonFormatter.SupportedMediaTypes.Add(new MediaTypeHeaderValue("text/html"));//改为JSON方式
-     }  
- }  
如此，在服务端那些List<>之类只有服务端才能明白的东西，在前端接收到时一律是JSON。
