# 软件测试知识点 fiddler抓包教程 - zhusongziye的博客 - CSDN博客





2018年10月30日 21:16:00[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：513








**Fiddler抓包教程**



Fiddler简介

简单地讲就是一个抓包工具，或者是叫做网络嗅探器，把网络传输的数据抓取下来进行分析、Debug。

它可以对常用的浏览器：IE、Chrome、Firefox、Safari 等进行抓包，支持 HTTP、HTTPS。

支持代理，可以通过它，在同一个局域网中抓取 APP 的网络请求，然后进行分析。

**壹**

配置

具体安装配置请参考 文档

https://www.yuque.com/imhelloworld/dxc2yg/wn7wp1

**贰**

简单抓包说明

![](https://img-blog.csdnimg.cn/20181030211023310.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**叁**

Fiddler抓包配置

打开【Tools】--【Options】选项

![](https://img-blog.csdnimg.cn/20181030211035491.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**肆**

配置抓取https请求

【Options】--【HTTPS】

![](https://img-blog.csdnimg.cn/20181030211046194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**伍**

修改默认监听端口

【Options】--【Connections】 默认端口号为8888

![](https://img-blog.csdnimg.cn/20181030211056682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**陆**

抓取数据

【Inspectors】--【WebForms】 

【Inspectors】--【Raw】

![](https://img-blog.csdnimg.cn/20181030211109350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**柒**

请求session过滤

【Filters】  勾选 【use filters】

![](https://img-blog.csdnimg.cn/20181030211123887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**捌**

如何拦截请求

【Rules】--【Atuomatic BreakPoints】--【Befor Requets】 

设置断点

![](https://img-blog.csdnimg.cn/20181030211139192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**玖**

篡改服务器端响应数据

【Rules】--【Atuomatic BreakPoints】-- 【After Responses】

![](https://img-blog.csdnimg.cn/20181030211153439.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**拾**

抓包并设置自定义响应数据

【AutoResponer】--【Enable Rules】--【Add Rule】

![](https://img-blog.csdnimg.cn/20181030211205428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**壹**

使用fiddler模拟发送get请求接口

【Composer】面板设置

![](https://img-blog.csdnimg.cn/20181030211443543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**贰**

使用fiddler模拟发送post请求接口

![](https://img-blog.csdnimg.cn/20181030211524957.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**叁**

修改HOST值

![](https://img-blog.csdnimg.cn/20181030211302932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

图文来源网络，如有侵权联系删除



