# timeout in asp.net - weixin_33985507的博客 - CSDN博客
2018年05月21日 17:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
[http://justgeeks.blogspot.my/2008/07/aspnet-session-timeouts.html](http://justgeeks.blogspot.my/2008/07/aspnet-session-timeouts.html)
[http://www.dofactory.com/topic/1747/session-timeout-vs-forms-authentication-timeout.aspx](http://www.dofactory.com/topic/1747/session-timeout-vs-forms-authentication-timeout.aspx)
让这2个timeout保持一致
Session timeout vs Forms Authentication timeout
另外，还有一个role manager
[https://msdn.microsoft.com/en-us/library/ms164660(v=vs.100).aspx](https://msdn.microsoft.com/en-us/library/ms164660(v=vs.100).aspx)
[https://msdn.microsoft.com/en-us/library/system.web.security.roles.cookietimeout(v=vs.110).aspx](https://msdn.microsoft.com/en-us/library/system.web.security.roles.cookietimeout(v=vs.110).aspx)
```
<roleManager defaultProvider="SqlProvider" 
  enabled="true"
  cacheRolesInCookie="true"
  cookieName=".ASPROLES"
  cookieTimeout="30"
  cookiePath="/MyApplication"
  cookieRequireSSL="true"
  cookieSlidingExpiration="true"
  cookieProtection="Encrypted" >
  <providers>
  <add
    name="SqlProvider"
    type="System.Web.Security.SqlRoleProvider"
    connectionStringName="SqlServices"
    applicationName="MyApplication" />
  </providers>
</roleManager>
```
[https://support.microsoft.com/en-us/help/910439](https://support.microsoft.com/en-us/help/910439)
#### The forms authentication ticket times out
The other common cause for a user to be redirected is if the forms authentication ticket has expired. The forms authentication ticket can time out in two ways. The first scenario occurs if you use absolute expiration. With absolute expiration, the authentication ticket expires when the expiration time expires. For example, you set an expiration of 20 minutes, and a user visits the site at 2:00 PM. The user will be redirected to the login page if the user visits the site after 2:20 PM.
If you use sliding expiration, the scenario is a 
bit more complicated. The cookie and the resulting ticket are updated if
 the user visits the site after the expiration time is half-expired. For
 example, you set an expiration of 20 minutes by using sliding 
expiration. A user visits the site at 2:00 PM, and the user receives a  
cookie that is set to expire at 2:20 PM. The expiration is only updated 
if the user visits the site after 2:10 PM. If the user visits the site 
at 2:09 PM, the ticket is not updated because half of the expiration 
time has not passed. If the user then waits 12 minutes, visiting the 
site at 2:21 PM, the ticket will be expired. The user is redirected to 
the login page. 
[https://www.codeproject.com/Articles/534693/Authentication-vs-Session-timeout-Session-expired](https://www.codeproject.com/Articles/534693/Authentication-vs-Session-timeout-Session-expired)
[https://support.microsoft.com/en-us/help/910443/understanding-the-forms-authentication-ticket-and-cookie](https://support.microsoft.com/en-us/help/910443/understanding-the-forms-authentication-ticket-and-cookie)
