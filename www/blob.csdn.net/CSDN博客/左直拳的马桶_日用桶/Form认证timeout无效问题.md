# Form认证timeout无效问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年08月09日 20:50:39[左直拳](https://me.csdn.net/leftfist)阅读数：2350
web.config有如下设置：
```xml
<authentication mode="Forms">
      <forms name=".GZFBCWeb" loginUrl="~/Admin/Auth/Login" timeout="1" defaultUrl="~/" cookieless="UseCookies" slidingExpiration="true" enableCrossAppRedirects="true" path="/"/>
    </authentication>
```
其中 
timeout的单位是分钟，意即认证多少分钟后过期； 
slidingExpiration=”true”，如果用户保持活跃，则重置timeout计时器，理论上，只要你永远活跃，则认证永不过期。
问题是，我登录之后，就此不动，过了1分钟后再去访问，仍旧没有退出，认证依然有效。
用浏览器调试，发觉cookie=.GZFBCWeb的过期时间为“Session”，意思是，这是一个会话cookie，只有浏览器关闭才失效。
为啥呢，timeout不起作用？
后来翻看代码，才知道是我代码的原因。登录成功后，没有采用默认的处理方式，而是自己生成了一个cookie,由于没有指定cookie的过期属性，造成该COOKIE是默认的会话生命周期。我原先代码是这样写的：
```
void Signin(LoginInfo sUT)
{//此函数在登录成功后调用
    System.Web.Security.FormsAuthenticationTicket tk = new FormsAuthenticationTicket(1,
            sUT.LoginName,
            DateTime.Now,
            DateTime.Now.AddMonths(1),
            true,
            "",
           FormsAuthentication.FormsCookiePath
            );
    string key = FormsAuthentication.Encrypt(tk); //得到加密后的身份验证票字串 
    HttpCookie ck = new HttpCookie(FormsAuthentication.FormsCookieName, key);
    Response.Cookies.Add(ck);
}
```
又是票据又是加密，不明觉厉。 
重点是最后两句，自己输出了一个cookie，使得web.config的设置完全失去效用。
后来改为：
```
void Signin(LoginInfo sUT)
{
    //注意最后一个参数一定要设置为true
    FormsAuthentication.SetAuthCookie(sUT.LoginName, true);
}
```
问题解决。简简单单，何必另起炉灶，故弄玄虚。
============================== 
由上面的设置，我这个FORM认证依赖于COOKIE“.GZFBCWeb”,只要将该COOKIE删除，系统立马就退出了。 
![这里写图片描述](https://img-blog.csdn.net/20170810165235411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
