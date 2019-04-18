# 常见CDN免费服务 - 深之JohnChen的专栏 - CSDN博客

2017年02月23日 09:06:59[byxdaz](https://me.csdn.net/byxdaz)阅读数：2722


**CDN -内容分发网络**

您总是希望网页可以尽可能地快。您希望页面的容量尽可能地小，同时您希望浏览器尽可能多地进行缓存。如果许多不同的网站使用相同的 JavaScript 框架，那么把框架库存放在一个通用的位置供每个网页分享就变得很有意义了。CDN公共库是指将常用的JS库存放在CDN节点，以方便广大开发者直接调用。与将JS库存放在服务器单机上相比，CDN公共库更加稳定、高速。

CDN (Content Delivery Network) 解决了这个问题。CDN 是包含可分享代码库的服务器网络。

Google 为一系列 JavaScript 库提供了免费的 CDN，包括：
- jQuery
- Prototype
- MooTools
- Dojo
- Yahoo! YUI

但是由于 Google 在中国经常被GFW（防火长城，英文名称Great Firewall of China，简写为Great Firewall，缩写GFW）屏蔽，造成访问不稳定，所以建议使用百度静态资源公共库。

国内的Jquery CDN免费服务

**jquery CDN 引用地址**

j百度query引用地址
- http://apps.bdimg.com/libs/jquery/1.6.4/jquery.js
- http://apps.bdimg.com/libs/jquery/1.6.4/jquery.min.js
- http://apps.bdimg.com/libs/jquery/2.1.1/jquery.js
- http://apps.bdimg.com/libs/jquery/2.1.1/jquery.min.js

新良jquery引用地址

http://lib.sinaapp.com/js/jquery/1.9.1/jquery-1.9.1.min.js

其它版本也是直接修改版本号就可以了

360 jquery引用地址

http://libs.useso.com/js/jquery/1.9.1/jquery.min.js

其它版本也是直接修改版本号就可以了

如果担心CDN地址加载失败，可以这样载入本地jquery

```java
<script type="text/javascript">
  !window.jQuery && document.write('<script src="js/jquery.min.js"><\/script>');
</script>
```

**百度CDN公共库**

百度公共CDN为站长的应用程序提供稳定、可靠、高速的服务，包含全球所有最流行的开源JavaScript库。

网址：[http://cdn.code.baidu.com/](http://cdn.code.baidu.com/)

注：百度的速度目前来说应该是和新浪差不多的。不过jQuery的版本比SAE少几个，其他类库应该都差不多。

百度CDN公共库 是稳定，快速，全面，开源的国内CDN加速服务。
- 稳定，快速由百度遍布全国各地100+个CDN节点提供加速服务。让开源库享受与百度首页静态资源同等待遇。
- 全面，开源收录超过180+开源库，并且这个数字正在不断增加。百度静态资源公共库服务不仅在Github开源库上接受任何人的提交请求，同时实时同步国外如CDNJS上优秀的开源库。

百度在之前推出了CDN公共库，同时最近google的公共库经常性的出现无法访问的问题，所以对自己站点的公共库文件进行了更新了。百度CDN最明显的优势就是速度快。

**新浪云计算CDN公共库**

新浪云计算是新浪研发中心下属的部门，主要负责新浪在云计算领域的战略规划，技术研发和平台运营工作。主要产品包括 应用云平台Sina App Engine（简称SAE）。

SAE的CDN节点覆盖全国各大城市的多路（电信、联通、移动、教育）骨干网络，使开发者能够方便的使用高质量的CDN服务。

网址： [http://lib.sinaapp.com/](http://www.boyhost.cn/go.php?url=http://lib.sinaapp.com/)

**七牛云存储 开放静态文件CDN**

像 Google Ajax Library，Microsoft ASP.net CDN，SAE，Baidu，Upyun 等 CDN 上都免费提供的 JS 库的存储，但使用起来却都有些局限，因为他们只提供了部分 JS 库。但七牛云存储提供一个尽可能全面收录优秀开源库的仓库，并免费提供 CDN 加速服务。

网址： [http://www.staticfile.org/](http://www.boyhost.cn/go.php?url=http://www.staticfile.org/)

**360网站卫士CDN前端公共库**

托管在360众多的全国CDN节点上，覆盖电信、联通、移动等主流运营商线路，您可以在自己的网页上直接通过script标记引用这些资源，让网站访问速度瞬间提速！

只需替换一个域名就可以继续使用Google提供的前端公共库和免费字体库,让网站访问速度瞬间提速。

网址： [http://libs.useso.com/](http://www.boyhost.cn/go.php?url=http://libs.useso.com/)

**国外CDNJS**

CDNJS提供非常完整的 JavaScript 程式库，无论是热门或是冷门的一应俱全。若你觉得它们缺少哪些好用的函式库，也可以自行提交到网站里，通过审核后就 CDNJS 就会为你分流 js文件！这项服务是结合 CloudFlare、Pingdom 与 S3Stat的，稳定性与速度自然不在话下。CDNJS提供的 JavaScript Libraries 全部列在网站首页，使用者可以直接搜索。这些程式库都有标示版本编号、标签以及原维护网站链结。

网址： [http://www.cdnjs.com/](http://www.boyhost.cn/go.php?url=http://www.cdnjs.com/)

注：CDNJS应该算是最完整的的JS库了。存储了大部分主流的 JS 库，甚至 CSS、image 和 swf，不过很多国内优秀开源库是没有的。很多国外前卫的Js库在CDNJS大都能找到。国内的速度虽然比不上其他的几个国内的CDN服务，但是相对来说其实还可以。

当然你也可以使用国人提供的CDNJS国内镜像网站的又拍云路径来引用相关JS和CSS文件。

国内镜像： [http://www.cdnjs.cn/](http://www.boyhost.cn/go.php?url=http://www.cdnjs.cn/)

**国外Google Hosted Libraries**

Google出品，必属精品了。虽然最近Google全线产品被墙了，连基本的Google搜索服务都无法使用了。但是谷歌的公共CDN公共库应该是最强大的了，像其中的前卫的各种代码类库和Google Web Font 字体库，国内几大公共CDN服务几乎都不提供支持。

网址： [https://developers.google.com/speed/libraries/](http://www.boyhost.cn/go.php?url=https://developers.google.com/speed/libraries/)

