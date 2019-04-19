# 前端CDN公共库整理 - =朝晖= - 博客园
# [前端CDN公共库整理](https://www.cnblogs.com/dhcn/p/7116148.html)
转自: 灰狼博客, 地址: http://itlobo.com/articles/2016.html
现在web应用都在使用[js](http://lib.csdn.net/base/javascript)类库，这些类库小的几十K，大的几百K，而国内网络访问速度大家都知道不是那么惬意，所以给各位开发者推荐常用JS类库的CDN缓存，这样不管客户在哪里访问你的页面，调用公共类库的速度都会为你的页面节省很多时间。
下面是常用JavaScript公共库CDN服务商列表：
Google的js类库CDN发布页
http://code.google.com/intl/zh-CN/apis/libraries/
目前Google访问不稳定，不建议使用
微软的js库CDN发布页
http://www.asp.net/ajaxlibrary/cdn.ashx
又拍云为您托管常用的JavaScript库，您可以在自己的网页上直接通过script标记引用这些资源。这样做不仅可以为您节省流量，还能通过我们的CDN加速，获得更快的访问速度。
http://jscdn.upai.com/
百度开放平台CDN公共库
http://developer.baidu.com/wiki/index.php?title=docs/cplat/libs
百度公共库目前还不支持 HTTPS 加密访问
阿里CDN公共库
http://a.tbcdn.cn/libs/
新浪CDN公共库
http://lib.sinaapp.com/
更新慢
360 CDN公共库
http://libs.useso.com/
目前也不支持 HTTPS 加密访问
国外CDNJS(cloudflare)
http://cdnjs.com/
支持http/https/spdy协议，但由于在国外，在国内访问速度不稳定
速度和稳定的原因，有的人觉得国内站点会快一些，不过也有很多人用google的。
那么会不会有一天使用这种方式加载jquery库文件也会失败呢？可用以下方法：
```
<script type="text/javascript" src="http://upcdn.b0.upaiyun.com/libs/jquery/jquery-2.0.0.min.js"></script>  
<script type="text/javascript">  
!window.jQuery && document.write('<script type="text/javascript" src="jquery-2.0.0.min.js"></script>');  
</script>
```
将上面的代码放到你的html代码的头部。意义是这样的：首先去加载又怕的jquery库文件，如果没有加载成功的话，那么就加载自己服务器上的jquery库文件。

