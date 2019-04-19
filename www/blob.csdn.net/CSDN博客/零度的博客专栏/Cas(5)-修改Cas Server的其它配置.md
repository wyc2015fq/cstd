# Cas(5)-修改Cas Server的其它配置 - 零度的博客专栏 - CSDN博客
2016年05月25日 13:58:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：3583
1.1     修改host.name
       host.name是定义在cas.properties文件中的一个属性。该属性将被定义在uniqueIdGenerators.xml文件中的各种UniqueTicketIdGenerator用来生成TGT、ST等ticket。默认在生成这些ticket时会将host.name作为对应ticket的后缀。host.name的设置在集群环境下将非常有用，其值对于每个节点来说都必须是唯一的，这样在整个集群环境下生成出的各种ticket也必定是唯一的。当然，如果只是在单机环境下使用，我们也可以不修改它。
## 1.2     修改SSO Session的超时策略
       SSO Session的超时策略是由TicketGrantingTicketExpirationPolicy来描述的，其定义在ticketExpirationPolicies.xml文件中的。以下是其定义：
<!-- Provides both idle and hard timeouts, for instance 2 hour sliding window with an 8 hour max lifetime -->
<beanid=*"grantingTicketExpirationPolicy"*class=*"org.jasig.cas.ticket.support.TicketGrantingTicketExpirationPolicy"*
p:maxTimeToLiveInSeconds=*"${tgt.maxTimeToLiveInSeconds:28800}"*
p:timeToKillInSeconds=*"${tgt.timeToKillInSeconds:7200}"*/>
       我们可以看到它需要指定两个参数，maxTimeToLiveInSeconds和timeToKillInSeconds，它们的单位都是秒。其中timeToKillInSeconds表示用户在多久不进行操作的情况下将超时，maxTimeToLiveInSeconds表示SSO Session的最大有效时间，从生成到指定时间后就将超时。timeToKillInSeconds默认是7200秒，即2小时；maxTimeToLiveInSeconds默认是28800秒，即8小时。用户如果需要修改的话可以之前介绍的Maven
 War覆盖的方式在ticketExpirationPolicies.xml文件中修改，也可以在cas.properties文件中通过对应的属性进行修改。
## 1.3     修改允许管理service的角色
       有的时候我们需要查看或管理Cas Server端的service注册情况。这就需要我们在cas.properties文件中指定用户访问service管理页面需要的角色，这是通过cas.securityContext.serviceProperties.adminRoles来指定的，默认是ROLE_ADMIN。之后我们就可以通过访问“/services/manage.html”来对Cas Server上注册的service进行管理了。在访问该URL时可能会出现国际化时某信息在zh_CN中找不到的问题，解决方法可以将对应的信息在messages_zh_CN.properties中加上，或者将messages_en.properties文件copy一份重命名为messages.properties，英文版本的messages相对而言应该是比较全的。Cas
 Server的权限控制也是基于[spring](http://lib.csdn.net/base/17) Security的，其主要配置文件是WEB-INF/spring目录下的securityContext.xml文件，关于权限相关的内容可以在这里查找。
## 1.4     修改logout后的重定向
       默认情况下，Cas客户端应用通过Cas Server的logout登出后是会展示Cas Server的默认登出结果页面的。如果用户希望在Cas Server登出后能够跳转到自己的应用来，则可以在访问Cas Server的logout时通过参数service传递登出后需要跳转的地址。有一点需要注意的是指定的service需要是在Cas Server注册过的，我们可以通过访问Cas Server的“/services/manage.html”对service进行管理，包括增、删、改、查等。另外我们还需要启用Cas
 Server在logout后的重定向功能，该功能默认是不启用的。通过去掉cas.properties文件中属性cas.logout.followServiceRedirects=false前面的注释，并修改其值为true可以启用logout后的重定向了。
## 1.5     禁用logout后的回调
       默认情况下在通过访问Cas Server的logout进行登出后Cas Server将依次回调其中注册的各个Cas Client应用，以进行单点登出。如果用户不希望Cas Server在logout后回调Cas Client应用，则可以通过设置cas.properties文件中的slo.callbacks.disabled=true来禁用其回调。
## 1.6     修改service ticket的超时时间
       service ticket的默认有效时间是10秒。通常来说这已经足够了，因为从Cas Server生成service ticket返回给Cas client应用，到Cas client应用发送service ticket到Cas Server进行验证这个过程10秒已经足够。这也是为什么如果我们使用debug追踪Cas应用的认证过程时经常会失败的原因，因为追踪的时候service ticket已经过了10秒的有效期了。如果用户需要修改它可以通过cas
 .properties文件中的st.timeToKillInSeconds属性进行修改。
（注：本文是基于Cas3.5.2所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2137767](http://haohaoxuexi.iteye.com/blog/2137767)）
