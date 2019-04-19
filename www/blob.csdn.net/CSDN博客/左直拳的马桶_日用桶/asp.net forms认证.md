# asp.net forms认证 - 左直拳的马桶_日用桶 - CSDN博客
2015年03月25日 23:00:08[左直拳](https://me.csdn.net/leftfist)阅读数：2023
工作中遇到的asp.net项目使用forms认证。以前虽然用过，但其原理并不了解，现在甚至对什么是form认证也完全不知道了。对一样东西如果不清楚其原理，不知其所以然，那么死记硬背是无济于事的。
asp.net提供了三种认证方式：windows、forms、passport（现在不清楚是否还是这三种）。看上去，forms应该用得最多。
**cookie**
forms认证，其实依赖的就是cookie。这些cookie由asp.net控制、加密。
比如说：
```
<authentication mode="Forms" xdt:Transform="Replace">
      <forms name="GJ.Web" loginUrl="http://192.168.0.22:8080/cas/login" timeout="2880" defaultUrl="~/Home/Index" cookieless="UseCookies" slidingExpiration="true" enableCrossAppRedirects="true" path="/" />
    </authentication>
```
其中`<forms name="GJ.Web"`的 “GJ.Web” 就是Oookie的容器名称。
在Forms身份认证中，由于登录状态是保存在Cookie中，而Cookie又会保存到客户端，因此，为了保证登录状态不被恶意用户伪造， ASP.NET采用了加密的方式保存登录状态。 为了实现安全性，ASP.NET采用【Forms身份验证凭据】（即FormsAuthenticationTicket对象）来表示一个Forms登录用户， 加密与解密由FormsAuthentication的Encrypt与Decrypt的方法来实现。
用户登录的过程大致是这样的：
1. 检查用户提交的登录名和密码是否正确。
2. 根据登录名创建一个FormsAuthenticationTicket对象。
3. 调用FormsAuthentication.Encrypt()加密。
4. 根据加密结果创建登录Cookie，并写入Response。
在登录验证结束后，一般会产生重定向操作， 那么后面的每次请求将带上前面产生的加密Cookie，供服务器来验证每次请求的登录状态。（cookie附着在请求中，这是http协议的内容，并不是asp.net所独有）
每次请求时的（认证）处理过程如下：
1. FormsAuthenticationModule尝试读取登录Cookie。
2. 从Cookie中解析出FormsAuthenticationTicket对象。过期的对象将被忽略。
3. 根据FormsAuthenticationTicket对象构造FormsIdentity对象并设置HttpContext.User
4. UrlAuthorizationModule执行授权检查。
在登录与认证的实现中，FormsAuthenticationTicket和FormsAuthentication是二个核心的类型， 前者可以认为是一个数据结构，后者可认为是处理前者的工具类。
UrlAuthorizationModule是一个授权检查模块，其实它与登录认证的关系较为独立， 因此，如果我们不使用这种基于用户名与用户组的授权检查，也可以禁用这个模块。
由于Cookie本身有过期的特点，然而为了安全，FormsAuthenticationTicket也支持过期策略， 不过，ASP.NET的默认设置支持FormsAuthenticationTicket的可调过期行为，即：slidingExpiration=true 。 这二者任何一个过期时，都将导致登录状态无效。
**machineKey**
在多台服务器之间使用Forms身份认证
machineKey是密钥，而不是什么类似MAC地址似的机器码。
默认情况下，ASP.NET 生成随机密钥并将其存储在本地安全机构 (LSA) 中， 因此，当需要在多台机器之间使用Forms身份认证时，就不能再使用随机生成密钥的方式， 需要我们手工指定，保证每台机器的密钥是一致的。
用于Forms身份认证的密钥可以在web.config的machineKey配置节中指定，我们还可以指定加密解密算法：
<machineKey 
  decryption="Auto" [Auto | DES | 3DES | AES]
  decryptionKey="AutoGenerate,IsolateApps" [String]
/>
昨天我遇到这么一种现象：
IIS6中，同一个网站下部署两个各自独立，但登录模块相同、数据库共享的应用程序。它们都采用forms认证，其中一个登录，必然会导致另一个应用程序登出。真奇怪，它们的应用程序池是各自分开的啊，怎么会这样呢？
比较各自的配置文件，发现这个 machineKey 不同，修改成一致，就起到单点登录的效果了：其中一个登录，另外一个非但不会登出，反而本来未登录的话，也不必再登录了。
今天又发现：假如它们的数据库分开，用户信息不再相同，那么互斥的现象再次出现。
思考了一下，认为原因如下：
machineKey不一致的情况下，应用A登录后，如果请求应用B的页面，那么应用A生成的cookie附在请求中，在应用B中进行检测，解密方法不同，必然无法通过，所以导致应用B登出或强迫再次登录；
在machineKey一致，但两个应用数据库不同，用户信息不同的时候，应用A的cookie虽然在应用B成功解密，但这个账户在应用B的数据库中不存在，所以可能应用B做了防御，因此也强迫登出。
参考文章：
http://www.cnblogs.com/fish-li/archive/2012/04/15/2450571.html
