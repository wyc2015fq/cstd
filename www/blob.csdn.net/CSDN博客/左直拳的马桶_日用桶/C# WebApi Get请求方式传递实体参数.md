# C# WebApi Get请求方式传递实体参数 - 左直拳的马桶_日用桶 - CSDN博客
2019年04月10日 19:00:50[左直拳](https://me.csdn.net/leftfist)阅读数：31
我又搞回笃NET啦！java之路真是命运多舛，好事多磨。不过，也许我已经进入无招胜有招，博取众家之长、融会贯通的地步了。
对于WebApi，今天又有了一些新的了解。
话说，Get请求方式，参数会附在Url后面，称为QueryString，传递给服务器；而POST方式，则将参数放在消息体内。采用QueryString的话，简单，方便，但只适合参数比较少的情况；但有的时候，需要传递比较多、比较复杂的参数，比如，组合条件查询。
组合条件查询，条件会很多，通常会用一个实体类来封装，传递给服务器。用post方式肯定是可以的，驾轻就熟。问题是，RESTful原则，是将一切抽象成资源，对资源的不同请求方式，代表了对资源的不同操作。按道理，post难道不是代表插入吗？为啥查询也要用到post？
GET方式下，也可以将参数提交给服务器？但查来查去，ajax好像可以；但服务器端，使用WebClient就没有找到现成的例子，不知道该咋整。只好用最原始的方法，将实体类转化成QueryString，附在地址后面了。有两个问题：
> 
1、实体类如何转换成QueryString这种键值对格式？
2、服务器端如何提取？
**一、实体类如何转换成QueryString这种键值对格式？**
叫键值对可能不够专业，叫`NameValueCollection`？
很遗憾，也没找到啥现成的例子。
最后祭出反射，来拼装QueryString
**二、服务器端如何提取QueryString里参数，自动变成一个实体类？**
参数前面标注[FromUri]特性
上代码。
实体类：
```
namespace BaseLT.Core.Contract
{
    public class Request
    {
        public Request();
        public int Top { set; }
        public int PageSize { get; set; }
        public int PageIndex { get; set; }
        public string OrderBy { get; set; }
        public int SortState { get; set; }
        public bool CompareObject<T>(T obj1, T obj2);
        public void ExtjsInit();
    }
}
```
WebApi服务器端：
```
public class TankController : ApiController
{
	[HttpGet]
	[Route("api/tank/matters/public/{id=0}")]
	public IEnumerable<Matter> Get(int id,[FromUri]Request req)
	{
		return do sth;
	}
}
```
客户端：
```
[TestMethod]
public void TestTankApi()
{
	string url = "http://localhost/ybjzuser.api/api/tank/matters/public/";
	url += getQueryString(new Request()
	{
		PageIndex = 1,
		PageSize = 100
	});
	
	string re;
	using (WebClient webClient = new WebClient())
	{
		webClient.Encoding = Encoding.GetEncoding("utf-8");
		re = webClient.DownloadString(url);
	}
	Assert.AreNotEqual(null, re);
	Console.WriteLine(re);
}
static string getQueryString(Request req)
{
	StringBuilder query = new StringBuilder("?");
	PropertyInfo[] propertys = req.GetType().GetProperties();
	foreach (PropertyInfo pi in propertys)
	{
		if (pi.CanRead)
		{
			query.Append($@"{pi.Name}={pi.GetValue(req)}&");
		}
	}
	return query.ToString();
}
```
相关拙作：
[ASP.NET for WebApi](https://leftfist.blog.csdn.net/article/details/51519144)
[通过接口提交数据，Content-Type应该是application/x-www-form-urlencoded还是application/json?](https://blog.csdn.net/leftfist/article/details/79053389)
