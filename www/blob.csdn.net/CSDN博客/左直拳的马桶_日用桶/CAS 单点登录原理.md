# CAS 单点登录原理 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月18日 20:22:00[左直拳](https://me.csdn.net/leftfist)阅读数：498标签：[sso																[cas																[单点登录](https://so.csdn.net/so/search/s.do?q=单点登录&t=blog)](https://so.csdn.net/so/search/s.do?q=cas&t=blog)](https://so.csdn.net/so/search/s.do?q=sso&t=blog)
个人分类：[学习笔记																[系统架构](https://blog.csdn.net/leftfist/article/category/2567173)](https://blog.csdn.net/leftfist/article/category/1790155)
- 访问服务： 浏览器发送请求访问应用系统
- 定向认证： 应用系统重定向用户请求到 SSO 服务器。
- 用户认证：用户身份认证。
- 发放票据： 认证通过后，SSO 服务器会产生一个随机的 Service Ticket ，并缓存下来，同时重定向到应用系统，附带上该Ticket。
- 验证票据： 应用系统与SSO 服务器验证票据 Service Ticket 的合法性，验证通过后，允许客户端访问应用系统。
- 传输用户信息： SSO 服务器验证票据通过后，传输**账号信息**给应用系统。
![这里写图片描述](https://img-blog.csdn.net/20170418201936217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考资料： 
[http://www.coin163.com/java/cas/cas.html](http://www.coin163.com/java/cas/cas.html)
