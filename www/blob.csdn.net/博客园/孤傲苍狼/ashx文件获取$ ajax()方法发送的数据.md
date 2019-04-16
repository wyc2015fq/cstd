# ashx文件获取$.ajax()方法发送的数据 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [ashx文件获取$.ajax()方法发送的数据](https://www.cnblogs.com/xdp-gacl/p/3462349.html)



今天在使用Jquery的ajax方法发送请求时，发现在后台中使用ashx文件无法接收到ajax方法中传递的参数，上网查了一下原因后发现了问题所在，原来是我**在$.ajax方法中指明了"contentType: 'application/json; charset=utf8'"，所以才导致了在ashx文件中处理请求时无法获取传递到服务器端的参数**，

正确的写法如下：

```
1 $.ajax({
 2             url: '/Handler1.ashx?operFlag=test1',
 3             type: 'POST',
 4             /*
 5             请求ashx文件的时候 要把contentType去掉，还有就是
 6             data 格式为 {”key”,”value”};切记 不要再 大括号外面加双引号，
 7             这样就会在ashx页面取不到数据而失败
 8             */
 9             //contentType: 'application/json; charset=utf8',
10             data: {
11                 "key1": "xdp",
12                 "key2":"孤傲苍狼" 
13             },
14             cache: false,
15             dataType: 'text',
16             success: function (data) {
17                 alert(data);
18             },
19             error: function (xhr) {
20                 alert("出现错误，请稍后再试:" + xhr.responseText);
21             }
22         });
```

这样在ashx文件中就可以使用如下的代码来获取$.ajax方法传递的参数了，代码如下：

```
1  string key1 = context.Request["key1"];
2  string key2 = context.Request["key2"];
```

以前一直都是用$.post方法来处理ajax，所以没有注意到这个问题，而这次由于是项目需要，所以就使用了$.ajax，没想到就遇到了上述的问题，好在找出了问题所在并且及时解决了问题。

另外，最近还遇到了一个奇怪的问题，"**用ajax提交数据到ashx后，用JSON.stringify格式化参数后在服务器端取不到值**？"，代码如下：

```
1  $.ajax({
 2             url: '/Handler1.ashx?operFlag=test2',
 3             type: 'POST',
 4             //JSON.stringify格式化参数
 5             data: **JSON.stringify(**{
 6                 "key3": "xdp-gacl",
 7                 "key4": "白虎神皇"
 8             }**)**,
 9             contentType: 'application/json; charset=utf8',
10             cache: false,
11             dataType: 'json',
12             success: function (data) {
13                 alert(data.key3 + "|" + data.key4);
14             },
15             error: function (xhr) {
16                 alert("出现错误，请稍后再试:" + xhr.responseText);
17             }
18         });
```

结果在ashx中使用**context.Request["key3"]**这种常规的方式是获取不到参数的，如下图所示：
![](https://images0.cnblogs.com/blog/289233/201312/07165146-65883bbbe035458797380f16d641b546.png)

郁闷了好久，怎么也想不明白为什么会这样，一开始以为是多了**contentType: 'application/json; charset=utf8'**这句代码造成的，于是把这句代码注释掉：

```
1 $.ajax({
 2             url: '/Handler1.ashx?operFlag=test2',
 3             type: 'POST',
 4             //JSON.stringify格式化参数
 5             data: JSON.stringify({
 6                 "key3": "xdp-gacl",
 7                 "key4": "白虎神皇"
 8             }),
 9             //contentType: 'application/json; charset=utf8',
10             cache: false,
11             dataType: 'json',
12             success: function (data) {
13                 alert(data.key3 + "|" + data.key4);
14             },
15             error: function (xhr) {
16                 alert("出现错误，请稍后再试:" + xhr.responseText);
17             }
18         });
```

可是结果还是一样的，使用**context.Request["key3"]**还是获取不到参数，没办法，既然常规的方式获取不到，那就另寻他法吧，百度了一下，找到了解决办法，在ashx中使用如下的方式就可以获取到了，首先写一个通用的获取参数的方法，代码如下：

```
1         /// <summary>
 2         /// 获取参数
 3         /// </summary>
 4         /// <param name="context"></param>
 5         /// <returns></returns>
 6         private Dictionary<String, Object> GetParameter(HttpContext context)
 7         {
 8             StreamReader reader = new StreamReader(context.Request.InputStream);
 9             //得到json字符串：strJson={"key3":"xdp-gacl","key4":"白虎神皇"}
10             String strJson = HttpUtility.UrlDecode(reader.ReadToEnd());
11             JavaScriptSerializer jss = new JavaScriptSerializer();
12             //将json字符串反序列化成一个Dictionary对象
13             Dictionary<String, Object> dicParameter = jss.Deserialize<Dictionary<String, Object>>(strJson);
14             return dicParameter;
15         }
```

GetParameter方法返回一个dicParameter对象，dicParameter就存放了从$.ajax方法中提交到ashx中的参数，如下图所示：
![](https://images0.cnblogs.com/blog/289233/201312/07172044-61097d6647964d3fb78271f6580917b7.png)

这样就可以从dicParameter中取出传递过来的参数作处理了，完整代码如下：

```
1  public void ProcessRequest(HttpContext context)
 2         {
 3             context.Response.ContentType = "text/plain";
 4             string operFlag = context.Request["operFlag"];
 5             if (operFlag == "test1")
 6             {
 7                 string key1 = context.Request["key1"];
 8                 string key2 = context.Request["key2"];
 9                 string resStr = key1 + "|" + key2;
10                 context.Response.Write(resStr);
11             }
12             else if (operFlag == "test2")
13             {
14                 **Dictionary<String, Object> dicParameter = GetParameter(context);**15                 **string key3 = dicParameter["key3"].ToString();**16                 **string key4 = dicParameter["key4"].ToString();**17                 string resStr = "{\"key3\":\"" + key3 + "\", \"key4\":\"" + key4 + "\"}";
18                 context.Response.Write(resStr);
19             }
20 
21         }
```











