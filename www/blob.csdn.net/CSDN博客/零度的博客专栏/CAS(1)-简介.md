# CAS(1)-简介 - 零度的博客专栏 - CSDN博客
2016年05月25日 11:18:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：755
       Cas的全称是Centeral Authentication Service，是对单点登录SSO（Single Sign On）的一种实现。其由Cas Server和Cas Client两部分组成，Cas Server是核心，而Cas Client通常就对应于我们的应用。一个Cas Server可以对应于多个Cas Client。它允许我们在一个Client进行登录以后无需再让用户输入用户名和密码进行认证即可访问其它Client应用。
       Cas Server的主要作用是通过发行和验证Ticket（票）来对用户进行认证和授权访问Client应用，用于认证的凭证信息都是由Cas Server管理的。而Cas Client就对应于我们真正的应用，当然其中会使用到Cas相关的类，用于与Cas Server进行交互。官网有两张图最能体现Cas的[架构](http://lib.csdn.net/base/16)和原理。
![](http://dl2.iteye.com/upload/attachment/0101/8882/0f1ca02c-5b4a-3872-a025-be68192f7928.png)
![](http://dl2.iteye.com/upload/attachment/0101/8884/d8ad28a4-ff0f-3681-a2a0-06cc3a14ed1d.png)
       如你所见，在第一次访问应用app1时，由于没有登录会直接跳转到Cas Server去进行登录认证，此时将附带查询参数service在Cas Server的登录地址上，表示登录成功后将要跳转的地址。此时Cas Server检查到没有之前成功登录后生成的SSO Session信息，那么就会引导用户到登录页面进行登录。用户输入信息提交登录请求，Cas Server认证成功后将生成对应的SSO Session，以及名为CASTGC的cookie，该cookie包含用来确定用户SSO
 Session的Ticket Granting Ticket（TGT）。之后会生成一个Service Ticket（ST），并将以ticket作为查询参数名，以该ST作为查询参数值跳转到登录时service对应的URL。如：
       http(s)://domain/app1?ticket=ST-2-59fS6KxvmykibRXyoPJE
       之后的操作对用户来说都是透明的，即不可见的。app1之后将以service和ticket作为查询参数请求Cas Server对service进行验证，验证通过后Cas Server将返回当前用户的用户名等信息。app1就会给当前用户生成其自身的Session，以后该用户以该Session都可以成功的访问app1，而不需要再去请求Cas Server进行认证。当该用户再去访问app2的时候，由于其在app2上没有对应的Session信息，将会跳转到Cas
 Server的登录地址，Cas Server此时发现其包含名为CASTGC的cookie，将获取其中包含的TGT来获取对应的SSO Session，然后会将用户重定向到app2对应的地址，以Service Ticket作为查询参数。之后app2会向Cas Server发送请求校验该Service Ticket，校验成功后app2将建立该用户对应的Session信息，以后该用户以该Session就可以自由的访问app2了。
       综上所述，我们知道，各系统之间的单点登录是通过Cas Server生成的SSO Session来交流的，而用户与实际的应用系统进行交互的时候，各应用系统将建立单独的Session，以满足用户与该系统的交互需求。
      原文地址：[http://haohaoxuexi.iteye.com/blog/2128728](http://haohaoxuexi.iteye.com/blog/2128728)
