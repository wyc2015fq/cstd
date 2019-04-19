# CAS(8)-单点登出 - 零度的博客专栏 - CSDN博客
2016年05月25日 15:03:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：2550
1.1     Cas Client端配置单点登出
       单点登出功能跟单点登录功能是相对应的，旨在通过Cas Server的登出使所有的Cas Client都登出。Cas Server的登出是通过请求“/logout”发生的，即如果你的Cas Server部署的访问路径为“https://localhost:8443/cas”时，通过访问“https://localhost:8443/cas/logout”可以触发Cas Server的登出操作，进而触发Cas Client的登出。在请求Cas Server的logout时，Cas Server会将客户端携带的TGT删除，同时回调该TGT对应的所有service，即所有的Cas
 Client。Cas Server中对应的TGT失效时默认也会触发同样的操作。Cas Client如果需要响应该回调，进而在Cas Client端进行登出操作的话就需要有对应的支持。具体来说，需要在Cas Client应用的web.xml文件中添加如下Filter和Listener。
<listener>
<listener-class>org.jasig.cas.client.session.SingleSignOutHttpSessionListener</listener-class>
</listener>
<filter>
<filter-name>casSingleSignOutFilter</filter-name>
<filter-class>org.jasig.cas.client.session.SingleSignOutFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casSingleSignOutFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
       SingleSignOutFilter需要配置在所有Filter之前，当Cas Client通过Cas Server登录成功，Cas Server会携带生成的Service Ticket回调Cas Client，此时SingleSignOutFilter会将Service Ticket与当前的Session绑定在一起。当Cas Server在进行logout后回调Cas Client应用时也会携带该Service Ticket，此时Cas Client配置的SingleSignOutFilter将会使对应的Session失效，进而达到登出的目的。
       SingleSignOutHttpSessionListener用于在Cas Client应用中的Session过期时将其从对应的映射关系中移除。
## 1.2     Cas Server端禁用单点登出
       单点登出功能默认在Cas Server端是启用的。可能因为某种原因用户想禁用它。在Cas Server的WEB-INF/[spring](http://lib.csdn.net/base/17)-configuration目录下有一个argumentExtractorsConfiguration.xml文件，其中定义了两个ArgumentExtractor类型的bean，一个是针对于Cas协议的，一个是针对于SAMP1.1协议的，但是它们都定义了一个共同的属性disableSingleSignOut，如下所示，该值对应的是一个属性替换变量，默认为false。
<bean
id=*"casArgumentExtractor"*
class=*"org.jasig.cas.web.support.CasArgumentExtractor"*
p:httpClient-ref=*"noRedirectHttpClient"*
p:disableSingleSignOut=*"${slo.callbacks.disabled:false}"*/>
<beanid=*"samlArgumentExtractor"*class=*"org.jasig.cas.web.support.SamlArgumentExtractor"*
p:httpClient-ref=*"noRedirectHttpClient"*
p:disableSingleSignOut=*"${slo.callbacks.disabled:false}"*/>
       在前面的内容中已经介绍过，Cas Server的所有属性替换变量都是定义在WEB-INF/cas.properties文件中的。所以如果需要禁用Cas Server的logout功能，我们可以在cas.properties文件中指定“slo.callbacks.disabled=true”。
（注：本文是基于Cas Server3.5.2和Cas Client3.1.11所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2144265](http://haohaoxuexi.iteye.com/blog/2144265)）
