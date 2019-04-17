# ASP.NET发布Web API接口时请求方式版本差异问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月23日 13:28:14[boonya](https://me.csdn.net/boonya)阅读数：270








从Java到ASP.NET，过渡到接口发布上层，HTTP请求方式不外乎那么几种：GET、POST、PUT、DELETE等这些都是标准。在ASP.NET中因MVC版本差异而出现了不同的写法，下面我们就来看一看。



# AcceptVerbsAttribute 类

构造函数




显示:                    继承                            保护            

||名称|说明|
|----|----|----|
|![公共方法](https://i-msdn.sec.s-msft.com/areas/global/content/clear.gif)|[AcceptVerbsAttribute(String[])](https://msdn.microsoft.com/zh-cn/library/dd493037(v=vs.108).aspx)|使用操作方法将响应的 HTTP 谓词的列表来初始化 AcceptVerbsAttribute 类的新实例。|
|![公共方法](https://i-msdn.sec.s-msft.com/areas/global/content/clear.gif)|[AcceptVerbsAttribute(HttpVerbs)](https://msdn.microsoft.com/zh-cn/library/dd460496(v=vs.108).aspx)|使用操作方法将响应的 HTTP 谓词来初始化 AcceptVerbsAttribute 类的新实例。|

注：属性支持字符串的请求标记类型和HttpVerbs的枚举类型。



# HttpVerbs 枚举

成员


||成员名称|说明|
|----|----|----|
||Get|检索由请求的 URI 标识的信息或实体。|
||Post|发布新实体作为对 URI 的补充。|
||Put|替换由 URI 标识的实体。|
||Delete|请求删除指定的 URI。|
||Head|检索由请求的 URI 标识的信息或实体的消息头。|
||Patch|请求将请求实体中描述的一组更改应用于请求 URI 所标识的资源。|
||Options|表示由请求 URI 标识的请求/响应链上提供的通信选项的相关信息请求。|

MVC2.0+与之前API请求方式的区别

请参考：



# [System.Web.Http.AcceptVerbs(“GET”) vs System.Web.Http.HttpGet](https://stackoverflow.com/questions/24135209/system-web-http-acceptverbsget-vs-system-web-http-httpget)



# [What is the difference between [AcceptVerbs(HttpVerbs.Post)] and [HttpPost]?](https://stackoverflow.com/questions/3843875/what-is-the-difference-between-acceptverbshttpverbs-post-and-httppost)



# [asp.net mvc - [HttpPost/HttpGet] vs. [AcceptVerbs(HttpVerbs.Post/Get)]](https://stackoverflow.com/questions/4744990/asp-net-mvc-httppost-httpget-vs-acceptverbshttpverbs-post-get)

示例程序：

```
[AcceptVerbs("GET","POST")]
public IHttpActionResult Login([FromUriOrBody]LoginModel login)
{
            return Ok(login);
}
```

mvc2.0+以上版本:




```
[HttpGet]
[HttpPost] 
public string Login([FromUriOrBody]LoginModel login)
{
            return Ok(login);
}
```
所以，不必在此纠结写法的问题，他们只是版本不同造成的写法不一样而已。











