# 一段处理json的C#代码 - 左直拳的马桶_日用桶 - CSDN博客
2017年07月27日 20:24:23[左直拳](https://me.csdn.net/leftfist)阅读数：349
服务器端：
```
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
public ActionResult GetGatherData()
{
	IList<M_Gather> list = gatherpolygonService.GetAll();
	JArray jarr = new JArray();
	foreach (var g in list)
	{
		jarr.Add(new JObject()
		{
			new JProperty("ID",g.GATHERID),
			new JProperty("POINTNAME",g.POINTNAME),
			new JProperty("DEVICEID",g.DEVICEID),
		});
	}
	JObject jobj = new JObject()
	{
		new JProperty("totalCount",getall.Count()),
		new JProperty("topics",jarr)
	};
	return Content(JsonConvert.SerializeObject(jobj), "application/json");
}
```
得到的json如下：
```
{
totalCount:10,
topics:[
{ID:"","POINTNAME":"","DEVICEID":""},...
]
}
```
相关文章：
[http://blog.csdn.net/leftfist/article/details/43155295](http://blog.csdn.net/leftfist/article/details/43155295)
