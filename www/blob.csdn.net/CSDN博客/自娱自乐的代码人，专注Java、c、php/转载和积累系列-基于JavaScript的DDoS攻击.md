# 转载和积累系列 - 基于JavaScript的DDoS攻击 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年06月30日 16:51:35[initphp](https://me.csdn.net/initphp)阅读数：1242








# 基于JavaScript的DDoS攻击

DDoS，即Distributed Denial of Service，中文译为分布式拒绝服务攻击，是一种常见的网络攻击方式。它的基本思想就是像一台服务器发送大量的请求，服务器大量的服务资源被占用，导致正常用户的请求无法得到回应。

近几年来，基于JavaScript的DDoS攻击事件常用发生。和其他DDoS攻击方式不同，基于JavaScript的DDoS攻击利用的工具是普通网民的伤亡设备，这也意味着只要装有浏览器的电脑，都能被用作为DDoS攻击者的工具。当被操纵的浏览器数量达到一定程度时，这种DDoS攻击方式将会带来巨大的破坏性。






## JavaScript DDoS攻击原理



出于安全的角度考虑，现代浏览器都遵守同源策略，即在浏览器中只能够加载相同域中的资源，比如在浏览器中发起AJAX请求时，只能向相同域下面的资源和数据发起请求(使用CORS除外)。然后，标签的src属性可以绕过同源策略的限制，加载任何域中的资源。这个属性被经常被用来进行跨域获取数据，比如著名的JSONP方法。另外，为了让各种JS库加载速度更快，让用户获得更好的体验，开发者经常会使用
```java
script
```
标签来加载CDN下的脚本，而一般情况下这些脚本都是跨域获取的。src跨域发起请求的特性给web开发带来了很多方便，但是这也使得它被用作DDoS发起攻击的工具。

下面是一个简单的例子：



```java
function imgflood() {  
  var TARGET = 'victim-website.com'
  var URI = '/index.php?'
  var pic = new Image()
  var rand = Math.floor(Math.random() * 1000)
  pic.src = 'http://'+TARGET+URI+rand+'=val'
}
setInterval(imgflood, 10)
```




在上面的imgflood函数中，通过
```java
Image
```
构造函数生成了一张图片，并将这张图片的src属性指向了攻击目标站点。在定义完函数之后，使用setInterval方法定时调用imgflood函数。此时，浏览器就会源源不断的向攻击目标站点发起请求，以达到攻击目的。

如果攻击者将这段恶意代码放在一个网站加载的JavaScript脚本中，可能会由于网站的点阅率过少而使DDoS攻击失效。由于很多站点选择从CDN加载JS资源，攻击者如果破解了CDN的后台权限，将恶意代码加入JS公共库中，只要网站加载了该CDN中的JS公共库，访问该网站的用户都会成为DDoS攻击的工具。






在文章任何区域双击击即可给文章添加【评注】！浮到评注点上可以查看详情。








# 基于JavaScript的DDoS攻击

DDoS，即Distributed Denial of Service，中文译为分布式拒绝服务攻击，是一种常见的网络攻击方式。它的基本思想就是像一台服务器发送大量的请求，服务器大量的服务资源被占用，导致正常用户的请求无法得到回应。

近几年来，基于JavaScript的DDoS攻击事件常用发生。和其他DDoS攻击方式不同，基于JavaScript的DDoS攻击利用的工具是普通网民的伤亡设备，这也意味着只要装有浏览器的电脑，都能被用作为DDoS攻击者的工具。当被操纵的浏览器数量达到一定程度时，这种DDoS攻击方式将会带来巨大的破坏性。

## JavaScript DDoS攻击原理

出于安全的角度考虑，现代浏览器都遵守同源策略，即在浏览器中只能够加载相同域中的资源，比如在浏览器中发起AJAX请求时，只能向相同域下面的资源和数据发起请求(使用CORS除外)。然后，标签的src属性可以绕过同源策略的限制，加载任何域中的资源。这个属性被经常被用来进行跨域获取数据，比如著名的JSONP方法。另外，为了让各种JS库加载速度更快，让用户获得更好的体验，开发者经常会使用
```java
script
```
标签来加载CDN下的脚本，而一般情况下这些脚本都是跨域获取的。src跨域发起请求的特性给web开发带来了很多方便，但是这也使得它被用作DDoS发起攻击的工具。

下面是一个简单的例子：



```java
function imgflood() {  
  var TARGET = 'victim-website.com'
  var URI = '/index.php?'
  var pic = new Image()
  var rand = Math.floor(Math.random() * 1000)
  pic.src = 'http://'+TARGET+URI+rand+'=val'
}
setInterval(imgflood, 10)
```


在上面的imgflood函数中，通过
```java
Image
```
构造函数生成了一张图片，并将这张图片的src属性指向了攻击目标站点。在定义完函数之后，使用setInterval方法定时调用imgflood函数。此时，浏览器就会源源不断的向攻击目标站点发起请求，以达到攻击目的。

如果攻击者将这段恶意代码放在一个网站加载的JavaScript脚本中，可能会由于网站的点阅率过少而使DDoS攻击失效。由于很多站点选择从CDN加载JS资源，攻击者如果破解了CDN的后台权限，将恶意代码加入JS公共库中，只要网站加载了该CDN中的JS公共库，访问该网站的用户都会成为DDoS攻击的工具。

## JavaScript DDoS攻击防御方法

为了解决由于引入第三方恶意脚本导致的DDoS攻击的问题，W3C提出了一个叫做“子资源完整性”（ Subresource Integrity）的新特性，简称SRI。这个新特性允许一个网站告诉浏览器只有在引入的脚本满足需求时才会进行解析并运行。

举个例子：




```java
<script src="http://xxx/jquery-1.10.2.min.js"></script>
```



上面的这个
```java
<script>
```
标签引入了最新稳定版的jQuery代码，如果其中被插入了恶意脚本，浏览器也会不加分辨的运行。而使用了SRI新特性之后，浏览器将会根据代码的加密哈希来对引入的代码进行判断。加密哈希就好比一个文件的指纹一般，不同的文件将会产出不同的加密哈希值。如果其中被添加了恶意脚本，则浏览器会因为加密哈希不匹配而拒绝执行引入的代码。如下所示：



```cpp
<script src="https://code.jquery.com/jquery-1.10.2.min.js"  
    integrity="sha256-C6CB9UYIS9UJeqinPHWTHVqh/E1uhG5Twh+Y5qFQmYg="
    crossorigin="anonymous">
```


在上面的
```java
script
```
标签上，多添加了两个属性，其中的integrity是加载的jquery版本的加密哈希值，用于判定判定加载进来的脚本是否符合我们的期望。如果你不了解你需要加载的JS脚本的加密哈希值，可以通过
```java
https://srihash.org/
```
这个网站进行计算。同时，crossorgin属性和http
 header中的CORS属性能够保证浏览器遵守同源属性并防止XSS攻击。











