# CAS 5.1.8 登录后重定向次数太多问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年03月02日 20:15:30[左直拳](https://me.csdn.net/leftfist)阅读数：5488
终于，使用CAS 5.1.8作为单点登录服务器，应用程序可以输入账号密码了。但是，登录成功后，却看不到返回应用程序的页面，报错了：
![这里写图片描述](https://img-blog.csdn.net/20180302195451222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
该网页无法正常运作
localhost 将您重定向的次数过多。
尝试清除 Cookie.
ERR_TOO_MANY_REDIRECTS
```
坑真多啊。
调试应用程序，根本还没运行到应用程序内部。CAS 在认证时，会与应用程序有一番你来我往的交互过程，应该就是在这个过程中死掉的。为什么会提示重定向次数太多呢？估计是有一个环节没有认证成功，重试了好多遍。
查看CAS的运行日志（C:\etc\cas\logs\），最后都是死于这样的：
```
2018-03-02 14:00:36,258 INFO [org.apereo.inspektr.audit.support.Slf4jLoggingAuditTrailManager] - Audit trail record BEGIN
=============================================================
WHO: http://localhost/NTZHHY.Web/OneMap?proxyResponse=true
WHAT: Supplied credentials: [http://localhost/NTZHHY.Web/OneMap?proxyResponse=true]
ACTION: AUTHENTICATION_FAILED
APPLICATION: CAS
=============================================================
```
看提示，怎么有个 `proxyResponse=true` 这样的参数？代理，没用什么代理啊。
这一次是修改应用程序的配置文件解决问题。我们的应用程序是一个.NET项目，配置文件里有关CAS的配置有这么一句：
`<casClientConfig casServerLoginUrl="http://192.168.0.231:8080/cas/login" casServerUrlPrefix="http://192.168.0.231:8080/cas/" serverName="http://localhost/" notAuthorizedUrl="~/NotAuthorized.aspx" cookiesRequiredUrl="~/CookiesRequired.aspx" redirectAfterValidation="true" gateway="false" renew="false" singleSignOut="true" ticketTimeTolerance="5000" ticketValidatorName="Cas20" proxyTicketManager="CacheProxyTicketManager"  serviceTicketManager="CacheServiceTicketManager" gatewayStatusCookieName="CasGatewayStatus"/>`
里面有这么一个属性：`proxyTicketManager="CacheProxyTicketManager"`  ，这是干啥的？问题似乎跟它有关，去掉，搞定了！
原来，我们之前一直用CAS 3.*作为单点登录，.NET里的配置语句，是从别的地方拷贝过来的，我一直都不清楚里面的含义，因为可以用，所以一直都不管它。现在换了高版本，不行了，问题才暴露出来。
[官方说明文档](https://github.com/apereo/dotnet-cas-client)里，默认的配置也没有这个属性：
```
<casClientConfig
  casServerLoginUrl="https://server.example.com/cas/login"
  casServerUrlPrefix="https://server.example.com/cas/"
  serverName="https://client.example.com:8443"
  notAuthorizedUrl="~/NotAuthorized.aspx"
  cookiesRequiredUrl="~/CookiesRequired.aspx"
  redirectAfterValidation="true"
  renew="false"
  singleSignOut="true"
  ticketValidatorName="Cas20"
  serviceTicketManager="CacheServiceTicketManager" />
```
