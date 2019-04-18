# ASP.NET 2.0 新特性 - weixin_33985507的博客 - CSDN博客
2006年04月29日 14:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
第一部分：UI design——1.0和1.1对界面设计的支持不是很方便，现在提供了很多类似dreamweaver的设计功能，可以方便实现界面设计，如模板、样式设计等等。
Theme—— (customize all same controls in all pages)
SkinID (differ themes)
StyleSheetTheme(allow page-define mode)
Master Page——(define the same content in different pages, contains a ContentPlaceHolder to set other controls)
第二部分：介绍几个经典的高级控件
Login——封装好的登录控件，以前的六个控件合为一个。
CreateUserWizard——创建用户的向导，可以实现自定义的分步创建用户，比如可以设置用户条例等等，按自动生成各个步骤来实现用户的注册。通过这个控件的使用，可以自动生成aspnetdb的数据库，默认存储在SQL Sever 2005 Expresss 版本中，当然可以通过aspnet_regsql.exe配置使用哪个版本的数据库。
第三部分：Caching 
VS中提供了非常方便的缓存机制，如输出缓存、硬盘缓存、回调缓存、sql缓存等等。
1、Output cache: cache whole page——<% @ output cache Duration=“60” VarByPara=“none” %>
2、Disk output cache——(DiskCacheable = “true”)
3、Callback Cache ——<asp:Substitution ID= “” MethodName=“”>
4、Use API to define cache：
Response.Cache.SetExpires(Now.AddSeconds(60);
Response.Cache.SetCacheability(HttpCacheability.Public);
5、 Page Fragment Caching
 <asp: SqlDataSource>
 <asp:SqlParameters>
  <asp:QueryStringParameter Name = “State” QueryStringField=“State” DefaultValue=“CN”
 </asp:SqlParameters>
<asp: SqlDataSource>
6、SQL Caching
Config a Connection Pool, when server data is changed, fresh cache
aspnet_regsql.exe –S “.\SqlExpress” –E –d “Pubs” –ed
aspnet_regsql.exe –S “.\SqlExpress” –E –d “Pubs” –et –t “authors”
<%@ OutputCache Duration= “999999” SqlDependency=“Pubs:authors” VaryByPara=“none” %>
7、Cache Configuration
web.config:
<caching>
   <outputCacheSettings>
  <outputCacheProfiles>
   <add name="CacheFor60Seconds"  duration="60" />
  </outputCacheProfiles>
    </outputCacheSettings>
</caching>
.aspx:
 <%@ OutputCache CacheProfile="CacheFor60Seconds" VaryByParam="name" %>
第四部分：Membership Management——asp.net 2.0中提供了封装的成员管理模型，可以不用编写代码实现身份认证和角色管理。下面是 Forms Authentication的配置，没有通过认证的用户将自动跳转到登录的页面。 
<configuration>
 <system.web>
   <authentication mode="Forms">
   <forms name=".ASPXUSERDEMO"       loginUrl="Formslogin.aspx" 
   protection="All" timeout="60" />
  </authentication>
 </system.web>
</configuration>
同时，提供了方便的几个静态类实现用户和角色编程模型,如MemberShip类、MemberShipUser类和Role类等等，下面的代码就实现了用户的创建。
Create a new user
try {
    Membership.CreateUser ("Jeff", "imbatman", "[[email]jeff@microsoft.com[/email]](mailto:jeff@microsoft.com)");
}
catch (MembershipCreateUserException e) {
    // Find out why CreateUser failed
    switch (e.StatusCode) {
    case MembershipCreateStatus.DuplicateUsername:
      ...
    case MembershipCreateStatus.DuplicateEmail:
      ...
    case MembershipCreateStatus.InvalidPassword:
      ...
    default:
      ...
    }
}
同时，权限的配置也可以十分方便地实现，比如允许哪些用户访问那些目录资源等等，下面是web.config的配置内容，很容易理解：
 <location path="secured">
  <system.web>
   <authorization>
    <deny users="?"/>
    <allow users="*"/>
   </authorization>
  </system.web>
 </location>
上面的代码只允许注册用户访问secured目录下的内容，匿名用户将被拒绝。下面的配置说明只有角色为Administrators的用户才可以访问administrators_role目录下的内容，其他角色或者非注册用户均不可访问。
 <location path="administrators_role">
  <system.web>
   <authorization>
    <allow roles="Administrators" />
    <deny users="*"/>
   </authorization>
  </system.web>
 </location>
其他：在2.0中，实现了更方便快捷的数据绑定，各种datasource等等，对原有的DataGrid也进行了升级包装——GridView，功能更加强大可控。总体给人的感觉就是，非常人性化！
