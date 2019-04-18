# ASP.NET Web API模型验证以及异常处理方式 - weixin_33985507的博客 - CSDN博客
2019年01月14日 08:44:39[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
ASP.NET Web API的模型验证与ASP.NET MVC一样，都使用System.ComponentModel.DataAnnotations。
具体来说，比如有：
[Required(ErrorMessage="")]
[Range(0, 999)]
[Bind(Exclude="")]
[DisplayName("")]
[StringLength(1024)]
...
验证扩展可以看这里：http://dataannotationsextensions.org/
在controller中如何验证模型呢？
通常是通过ModelState.IsValid.
```
public HttpResponseMessage Post(Product product)
{
    if(ModelState.IsValid){
        //
        return new HttpResponseMessage(HttpStatusCode.OK);
    }
    else
    {
        return Request.CreateErrorResponse(HttpStatusCode.BadRequest, ModelState);
    }
}
```
还可以通过"面向切面"的方式，自定义一个模型验证的特性ActionFilterAttibute特性。
```
public class ValidateModelAttribute:ActionFilterAttibute
{
    public override void OnActionExecuting(HttpActionContext actionContext)
    {
        if(actionContext.ModelState.IsValid == false)
        {
            actionContext.Response = actionContext.Request.CreateErrorResponse(HttpStatusCode.BadRequest, actionContext.ModelState);
        }
    }
}
```
然后需要把自定义的过滤器特性注册到默认的过滤器集合中去。
```
public static class WebApiConfig
{
    public static void Register(HttpConfiguraiotn config)
    {
        config.Filters.Add(new ValidateModelAttribute());
    }
}
```
如果不在全局注册过滤器，我们还可以把过滤器特性打到控制器或控制器方法上去。
```
[ValidateModel]
public HttpResponseMessage Post(Product product)
{
}
```
