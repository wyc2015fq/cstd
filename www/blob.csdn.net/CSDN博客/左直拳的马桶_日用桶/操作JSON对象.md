# 操作JSON对象 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月26日 19:56:49[左直拳](https://me.csdn.net/leftfist)阅读数：3706
JSON类型对象，最简单了，就是键值对，key:value、key:value、一直不停地key:value下去，层层嵌套，理论上多少层都可以，只要你喜欢。
可是，每次应用JSON，我都心烦意乱，甚至吓到面无人色。何哉？不懂咯，没有真正理解咯，拿到一个JSON对象，不知道如何去使用它，如何去读取里面的数据。
只能够一点一点地记录一些心得，期待量变达成质变，化蛹成蝶，肩胛骨处长出一对翅膀来。
1、JS里面使用
1）用key直接访问对应的value
比如：
```java
var jsonobj =｛"total":106,"rows":[{},{},{}.....]｝
alert(jsonobj.total);
```
```java
```
```java
将字符串转化成json:
```
```java
```
var jsonobj = JSON.parse(str);
```java
```
注意！！！str里的json元素，务必是双引号分隔，而不能是单引号！！！！
2）遍历key
```java
var smsTypeDesc = {"4":"回访短信","3":"邮件短信","aa":"测试短信"};  
function EnumaKey(){  
    for(var key in smsTypeDesc){  
        alert(key);  
    }  
}  
function GetVal(){  
    var key = prompt("请输入要查询的key","4");  
                if("undefined"==typeof(smsTypeDesc)) return;  
    if("undefined"==typeof(smsTypeDesc[key])){  
        alert("输入的key:"+key+"， 在json对象中不存在！");  
        return;  
    }     
    alert("您输入的key是："+key + "，该key所对应的值是："+smsTypeDesc[key]);  
}  
function GetValByKey(){  
    alert(smsTypeDesc.aa);  
}
```
本例子摘自 [http://blog.csdn.net/love__coder/article/details/7532616](http://blog.csdn.net/love__coder/article/details/7532616)
3、提交或获取服务器端的JSON数据
```java
$.ajax({
    url: "/api/customview/SetTags?account=" + _index_account,
    data: JSON.stringify(effectRow),
    dataType: "json",
    type: "POST",
    contentType: "application/json; charset=utf-8",
    success: function (data) {
        _dg.datagrid('acceptChanges');
        if (data == true) {
            if (setTagsCallback != null) setTagsCallback();
            alert("OK");
        } else {
            alert("failed");
        }
    },
    error: function (XMLHttpRequest, textStatus, errorThrown) {
        alert(textStatus + ": " + errorThrown);
    }
});
```
2、.NET里使用
.NET中操作JSON，非newtonsoft.json莫属。
newtonsoft.json里面的类型，目前来看，有以下类型：
JObject
 用于操作JSON对象
JArray
 用语操作JSON数组
JValue
 表示数组中的值
JProperty
 表示对象中的属性,以"key/value"形式
JToken
 用于存放Linq to JSON查询后的结果
参考文章：[http://www.cnblogs.com/usharei/archive/2012/04/24/2467578.html](http://www.cnblogs.com/usharei/archive/2012/04/24/2467578.html)
有代码有J8：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/43155341#)[copy](http://blog.csdn.net/leftfist/article/details/43155341#)
- using Newtonsoft.Json;  
- 
- JObject json = ...  
- 
- JToken[] jps = json["records"][0].ToArray();  
- List<ItemInfo> cols = json["columns"].ToObject<List<ItemInfo>>();  
- 
- ……  
- 
- JArray array = new JArray();  
- array.Add(data);  
- Tuple<int, JObject> tuple = customViewService.UpdateDataByView(ProjectId, account, array, viewId);  
- JObject ret = new JObject();  
- ret.Add(new JProperty("rows", array));  
- ret.Add(new JProperty("result", tuple.Item1 > 0));  
遍历，在不清楚该JSON的结构情况下：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/43155341#)[copy](http://blog.csdn.net/leftfist/article/details/43155341#)
- JsonTextReader reader = new JsonTextReader(new StringReader(sjson));  
- StringBuilder sb = new StringBuilder("");  
- while (reader.Read())  
- {  
- if (reader.Value != null)  
-         sb.Append(String.Format("Token: {0}, Value: {1}\r\n", reader.TokenType, reader.Value));  
- else
-         sb.Append(String.Format("Token: {0}\r\n", reader.TokenType));  
- }  
