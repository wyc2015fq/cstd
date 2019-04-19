# 服务器端将复合json对象传回前端 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月26日 19:51:16[左直拳](https://me.csdn.net/leftfist)阅读数：2588
前端接收后端传过来的JSON对象，对前端来说，传过来的确实就是JSON对象；但后端，类型则灵活得多，可以是IList<>等类型，当然也可以是newtonsoft的JObject类型。反正在后端，传达的参数类型很多，到达前端后，都变成了JSON对象。
见拙作：[http://blog.csdn.net/leftfist/article/details/42404989](http://blog.csdn.net/leftfist/article/details/42404989) ([JSON参数](http://blog.csdn.net/leftfist/article/details/42404989) )
其中原因，固然是估计后端（asp.net）做了处理，将多种数据自动转换：
config.Formatters.JsonFormatter.SupportedMediaTypes.Add(new MediaTypeHeaderValue("text/html"));//改为JSON方式
另一方面，也是因为JSON类型确实强，跟XML一样，可以描述各种各样的数据格式。
这里又对后端将JSON作为参数传递做一些记录：
比如说，我前端用easyUI的datagrid，要分页，那么传递给前端的数据格式，应该是这样的：
｛"total":106,"rows":[{},{},{}.....]｝
那么后端该怎么生成这种格式呢？
有代码有J8：
```
using Newtonsoft.Json;
[System.Web.Http.HttpGet]
public JObject GetTags(string account)
{
    JObject jobj = new JObject();
	//ViewTag是自定义类
    IList<ViewTag> list = customViewService.GetTags(ProjectId,account);
    jobj.Add("total", (JToken)list.Count());
    jobj.Add("rows", JArray.FromObject(list));
    return jobj;
}
```
