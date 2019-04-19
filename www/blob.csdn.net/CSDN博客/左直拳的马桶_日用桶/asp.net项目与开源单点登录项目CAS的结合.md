# asp.net项目与开源单点登录项目CAS的结合 - 左直拳的马桶_日用桶 - CSDN博客
2016年07月13日 11:47:55[左直拳](https://me.csdn.net/leftfist)阅读数：7242
这段时间搞的一个asp.net mvc项目，采用了单点登录。
这个单点登录就是CAS，一个开源的JAVA项目。当然，这并不影响ASP.NET项目结合它来进行登录。因为各自分工不同：单点登录（管它是不是单点）只负责账户校验，而账户的登录信息读取，以及做何种进一步的处理，那是应用项目自己的事。
就好比，单点登录是写字楼的安保。他在核实来访者的来访目的，进行来访者身份登记，通过安检之后，就放来访者进这个大厦了。至于位于写字楼里的某家公司（应用项目）要如何接待这个来访者，那是这家公司的事情。
总结如下：
1、采用 单点登录 + asp.net form认证 
单点登录网站负责校验；本项目负责存储登录认证信息。asp.net form认证为系统自带
2、访问本项目时，form认证首先检查是否已认证（asp.net form认证本身机制），否的话跳到单点登录；是的话，由本项目进一步校验。
3、本项目校验，如果发现缓存的登录信息丢失，则根据form认证记录的登录信息（简单），重新检索登录信息（详细）并缓存
4、无论单点登录、form认证还是本地缓存，都依赖cookie。
5、form认证主要在web.config里配置，代码里也有应用。
6、本项目触发登录信息校验的事件，在控制器的基类：AdminControllerBase.OnActionExecuting
![这里写图片描述](https://img-blog.csdn.net/20160713115032445)
## web.config:
```xml
<!--Form认证设置Start-->
    <authentication mode="Forms">
      <forms name=".ASPWeb" loginUrl="http://192.168.0.22:8080/cas/login" 
             timeout="2880" defaultUrl="~/Home/Index" 
    cookieless="UseCookies" slidingExpiration="true" enableCrossAppRedirects="true" path="/" />
    </authentication>
    <authorization>
      <deny users="?" />
      <allow users="*" />
    </authorization>
```
## AdminControllerBase.OnActionExecuting：
```
protected override void OnActionExecuting(ActionExecutingContext filterContext)
{
...
        if (this.LoginInfo == null)
        {
            LoginHelper.Instance.Login();
            return;
        }
...
}
```
## LoginHelper.cs
```
public void Login()
{
    if (HttpContext.Current != null)
    {
        //是否已经form验证？
        string username = HttpContext.Current.User.Identity.IsAuthenticated ? HttpContext.Current.User.Identity.Name : "";
        if (!string.IsNullOrEmpty(username))
        {//如果当前用户已登录单点登录系统
            var loginInfo = 。。。
            if (loginInfo != null)
            {
                //缓存loginInfo...                    
            }
        }
        else
        {
            Logout();
        }
    }
}
```
相关文章 
[asp.net forms认证](http://blog.csdn.net/leftfist/article/details/44629035)
