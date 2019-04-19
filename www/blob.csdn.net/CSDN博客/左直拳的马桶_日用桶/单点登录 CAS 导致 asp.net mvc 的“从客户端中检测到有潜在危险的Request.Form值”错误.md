# 单点登录 CAS 导致 asp.net mvc  的“从客户端中检测到有潜在危险的Request.Form值”错误 - 左直拳的马桶_日用桶 - CSDN博客
2018年09月29日 15:57:41[左直拳](https://me.csdn.net/leftfist)阅读数：223
我强调一下：[在asp.net](http://xn--asp-lp6e.net) mvc中，如果遇到表单提交时，报“从客户端中检测到有潜在危险的Request.Form值”这种错误，
**方法一：**
就在action头部加上 `[ValidateInput(false)]` 特性，这就足够了，而且绝对可行。不要信网上什么修改配置文件，还要将运行时改为2.0，都快5.0了，你还2.0，扯。
但这不是最好的办法，因为同一表单，往往有多个字段。我认为比较好的方式是，
**方法二：**
提交的时候，用实体类参数代替FormCollection参数，实体类里需要用到html标记的属性，加上`[AllowHtml]`特性。
```
public class MailVM
    {
        public string Title { get; set; }
        public string Time { get; set; }
        [AllowHtml]
        public string Mess { get; set; }
    }
```
参考文章：
[ASP.NET MVC 页面使用富文本控件的XSS漏洞问题](https://blog.csdn.net/leftfist/article/details/69629394)
如果上面2种方法都不行，怎么办？
这是不可能的。如果真出现了这种情况，就检查下你是否用到了一些额外的Handler或module，比如我今天就发现，因为项目使用了CAS作为单点登录，客户端 DotNetCasClient.dll会率先对所有的请求进行拦截，我们定义在action上的特性之类根本还没机会运行到就报错了。
```
<modules>
      <remove name="DotNetCasClient"/>
      <add name="DotNetCasClient" type="DotNetCasClient.CasAuthenticationModule,DotNetCasClient"/>
    </modules>
```
这时就要对官网给的DotNetCasClient稍作修改。没关系，官网有源代码，直接下载下来修改。
原代码为
```
internal static bool GetRequestIsCasSingleSignOut()
        {
            CasAuthentication.Initialize();
            HttpContext context = HttpContext.Current;
            HttpRequest request = context.Request;
            bool requestIsFormPost = (request.RequestType == "POST");
            //请注意这一行
            bool haveLogoutRequest = !string.IsNullOrEmpty(request.Params["logoutRequest"]);
            bool result =
            (
                requestIsFormPost &&
                haveLogoutRequest
            );
            return result;
        }
```
CAS客户端并没有刻意去拦截什么危险性内容，但`request.Params["logoutRequest"]`这个读取动作会自然触发异常。知道原因就好改了。
可以改为
```
internal static bool GetRequestIsCasSingleSignOut()
        {
            CasAuthentication.Initialize();
            HttpContext context = HttpContext.Current;
            HttpRequest request = context.Request;
            bool requestIsFormPost = (request.RequestType == "POST");
            //by chenqu
            bool haveLogoutRequest = false;
            if(requestIsFormPost)
            {
                try
                {
                    haveLogoutRequest = !string.IsNullOrEmpty(request.Params["logoutRequest"]);
                }
                catch(System.Web.HttpRequestValidationException ex)
                {
                    haveLogoutRequest = false;
                }
                catch(Exception ex)
                {
                    throw ex;
                }
            }
            bool result =
            (
                requestIsFormPost &&
                haveLogoutRequest
            );
            return result;
        }
```
改完，编译成新的DotNetCasClient.dll，替换项目中的就OK了。尼玛再也不用担心你提交了啥危险内容了。其实，所谓XSS跨域攻击，提交的内容到了服务器，这时还不会有啥问题的，只有你将提交的内容又展示在页面上才会发生攻击，因为只不过是些HTML标记罢了，只有在浏览器才能被解释运行。
