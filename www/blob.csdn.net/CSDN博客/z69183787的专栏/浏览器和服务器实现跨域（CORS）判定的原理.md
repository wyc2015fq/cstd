# 浏览器和服务器实现跨域（CORS）判定的原理 - z69183787的专栏 - CSDN博客
2016年11月09日 17:55:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：890
前端对Cross-Origin Resource Sharing 问题(CORS，中文又称'跨域')应该很熟悉了。众所周知出于安全的考虑，浏览器有个`同源策略`，对于不同源的站点之间的相互请求会做限制（跨域限制是浏览器行为，不是服务器行为。）。不过下午想到了一个略无趣的问题：浏览器和服务器到底是如何判定有没有跨域呢？本文主要分两个部分，一是对这个问题的总结，二是nginx下如何配置服务器允许跨域。
<!-- more -->
## 同源策略
> 
同源指的是域名（或IP），协议，端口都相同，不同源的客户端脚本(javascript、ActionScript)在没明确授权的情况下，不能读写对方的资源。
[同源的判定：](https://en.wikipedia.org/wiki/Same-origin_policy)
以`http://www.example.com/dir/page.html`为例，以下表格指出了不同形式的链接是否与其同源：（原因里未申明不同的属性即说明其与例子里的原链接对应的属性相同）
|链接|结果|原因|
|----|----|----|
|http:// `www.example.com` /dir/page2.html|`是`|同协议同域名同端口|
|http:// `www.example.com` /dir2/other.html|`是`|同协议同域名同端口|
|http://user:pwd@ `www.example.com`/dir2/other.html|`是`|同协议同域名同端口|
|http://www.example.com: `81`/dir/other.html|否|端口不同|
|`https`://www.example.com/dir/other.html|否|协议不同端口不同|
|http:// `en.example.com`/dir/other.html|否|域名不同|
|http:// `example.com`/dir/other.html|否|域名不同（要求精确匹配）|
|http:// `v2.www.example.com`/dir/other.html|否|域名不同（要求精确匹配）|
|http://www.example.com: `80`/dir/other.html|`不确定`|取决于浏览器的实现方式|
## 是否允许跨域的判定
前文提到了同源策略的判定，然而同源策略在加强了安全的同时，对开发却是极大的不便利。因此开发者们又发明了很多办法来允许数据的跨域传输（常见的办法有`JSONP`、`CORS`)。当域名不同源的时候，由于跨域实现的存在，浏览器不能直接根据域名来判定跨域限制。那么浏览器具体又是如何实现判定的呢？看下面的例子。
### 环境说明
- 
参与实验的前端域名三个有：`http://www.zhihu.com`、`http://segmentfault.com`、
`http://localhost`。
- 
请求的服务器端地址为`http://localhost/city.json`,服务器解析引擎使用的`nginx`,且服务器只配置了允许来自`http://segmentfault.com`的跨域请求
- 
检测方法：在各个域名下利用chrome浏览器的`console`界面模拟发送ajax请求，代码如下：
```java
var xhr = new XMLHttpRequest();
xhr.open('GET', 'http://localhost/city.json',true);
xhr.send();
```
### 实验过程
- 
在`http://localhost`域名下,请求成功。
`服务器回应的http文件头如下：`
```
HTTP/1.1 200 OK
Server: nginx/1.6.2
Date: Sun, 05 Jul 2015 17:44:06 GMT
Content-Type: application/octet-stream
Content-Length: 2084
Last-Modified: Sat, 18 Apr 2015 06:20:12 GMT
Connection: keep-alive
ETag: "5531f79c-824"
Access-Control-Allow-Credentials: true
Access-Control-Allow-Methods: GET, POST, OPTIONS
Accept-Ranges: bytes
```
- 
在`http://segmentfault.com`域名下，请求成功
`服务器回应的http文件头如下：`
```java
```
HTTP/1.1 200 OK
Server: nginx/1.6.2
Date: Sun, 05 Jul 2015 18:17:27 GMT
Content-Type: application/octet-stream
Content-Length: 2084
Last-Modified: Sat, 18 Apr 2015 06:20:12 GMT
Connection: keep-alive
ETag: "5531f79c-824"
**Access-Control-Allow-origin: http://segmentfault.com**
Access-Control-Allow-Credentials: true
Access-Control-Allow-Methods: GET, POST, OPTIONS
Accept-Ranges: bytes
```
```
- 
在`http://www.zhihu.com`下，请求失败
虽然都是失败，但是返回的HTTP文件头内容会视服务器是否有配置跨域请求而发生变化
#### 服务器允许跨域请求
(仅允许来自`http://segmentfault.com`的跨域请求)
console.log窗口提示：
> 
XMLHttpRequest cannot load [http://localhost/city.json.](http://localhost/city.json.) The
 'Access-Control-Allow-Origin' header has a value '[http://segmentfault.com](http://segmentfault.com/)' that is not equal to the supplied origin.
 Origin '[http://www.zhihu.com](http://www.zhihu.com/)' is therefore notallowed access.
服务器回应的http文件头如下：
```java
```
HTTP/1.1 200 OK
    Server: nginx/1.6.2
    Date: Sun, 05 Jul 2015 17:59:25 GMT
    Content-Type: application/octet-stream
    Content-Length: 2084
    Last-Modified: Sat, 18 Apr 2015 06:20:12 GMT
    Connection: keep-alive
    ETag: "5531f79c-824"
    Access-Control-Allow-origin: http://segmentfault.com
    Access-Control-Allow-Credentials: true
    Access-Control-Allow-Methods: GET, POST, OPTIONS
    Accept-Ranges: bytes
```
```
#### 服务器不允许任何跨域请求
console.log窗口提示:
> 
XMLHttpRequest cannot load [http://localhost/city.json.](http://localhost/city.json.) No
 'Access-Control-Allow-Origin' header is present on the requested resource. Origin '[http://www.zhihu.com](http://www.zhihu.com/)'
 is therefore not allowed access.
服务器回应的http文件头如下：
```
HTTP/1.1 200 OK
Server: nginx/1.6.2
Date: Sun, 05 Jul 2015 17:51:29 GMT
Content-Type: application/octet-stream
Content-Length: 2084
Last-Modified: Sat, 18 Apr 2015 06:20:12 GMT
Connection: keep-alive
ETag: "5531f79c-824"
Access-Control-Allow-Credentials: true
Access-Control-Allow-Methods: GET, POST, OPTIONS
Accept-Ranges: bytes
```
### 跨域的判定流程
从`zhihu`页面的两次浏览器报错以及`segmentfault`的成功返回值来看，可以很容易得出浏览器和服务器的合作判定步骤如下：
> - 
浏览器先根据同源策略对前端页面和后台交互地址做匹配，若同源，则直接发送数据请求；若不同源，则发送跨域请求。
- 
服务器解析程序收到浏览器跨域请求后，根据自身配置返回对应文件头。若未配置过任何允许跨域，则文件头里不包含`Access-Control-Allow-origin`字段，若配置过域名，则返回`Access-Control-Allow-origin`+ `对应配置规则里的域名的方式`。
- 
浏览器根据接受到的http文件头里的`Access-Control-Allow-origin`字段做匹配，若无该字段，说明不允许跨域；若有该字段，则对字段内容和当前域名做比对，如果同源，则说明可以跨域，浏览器发送该请求；若不同源，则说明该域名不可跨域，不发送请求
（但是不能仅仅根据服务器返回的文件头里是否包含`Access-Control-Allow-origin`来判断其是否允许跨域，因为服务器端配置多域名跨域的时候，也会出现不能跨域的域名返回包里没有`Access-Control-Allow-origin`字段的情况。下文配置说明里会讲。)
## 配置服务器实现跨域传输
前面讲到了同源策略的基本判定，以及浏览器实现跨域判断的方式，那么，如何在服务器端做配置来允许跨域传输呢？下文将以Nginx为例，讲一下三种情况下的配置。
### 配置项解析
CORS常用的配置项有以下几个：
> - 
Access-Control-Allow-Origin（必含） – 允许的域名，只能填通配符或者单域名
- 
Access-Control-Allow-Methods（必含） – 这允许跨域请求的http方法（常见有`POST`、`GET`、`OPTIONS`）
- 
Access-Control-Allow-Headers（当预请求中包含Access-Control-Request-Headers时必须包含） – 这是对预请求当中Access-Control-Request-Headers的回复，和上面一样是以逗号分隔的列表，可以返回所有支持的头部。
- 
Access-Control-Allow-Credentials（可选） – 该项标志着请求当中是否包含cookies信息，只有一个可选值：true（必为小写）。如果不包含cookies，请略去该项，而不是填写false。这一项与XmlHttpRequest2对象当中的withCredentials属性应保持一致，即withCredentials为true时该项也为true；withCredentials为false时，省略该项不写。反之则导致请求失败。
- 
Access-Control-Max-Age（可选） – 以秒为单位的缓存时间。预请求的的发送并非免费午餐，允许时应当尽可能缓存。
### 具体配置举例
#### 全域名或者单域名允许跨域
这个最省心
打开Nginx的配置文件（默认为`nginx.conf`）。找到对应域名设置的`local`配置部分。
添加以下内容：
```
add_header 'Access-Control-Allow-origin' 'http://www.example.com';
add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
```
> 
添加的域名必须带`http://`协议头（否则服务器无法区分是http还是https），如果接受所有域名的跨域请求，则可以用`*`(安全性有问题，不推荐)
#### 添加多域名跨域配置
如果允许跨域的域名有多个但出于安全问题又不想配置全域名通配的时候，就可以用到nginx里的`if`判断了。
添加如下内容：
```
if ($http_origin = 'http://segmentfault.com' ) {  
 add_header 'Access-Control-Allow-Origin' "$http_origin";
 }
if ($http_origin = 'http://localhost:4000' ) {  
 add_header 'Access-Control-Allow-Origin' "$http_origin";
 }
 add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
```
> - 
如果对正则比较熟悉的，可以直接用正则来匹配条件判断，不需要用if这么麻烦的方式。
- 
'Access-Control-Allow-Methods' 允许多参数，'Access-Control-Allow-origin'不允许多参数，所以只能是条件语句判断要不要加这个。这也是我前面提到的为什么即使HTTP文件头返回值里没有'Access-Control-Allow-origin',也不能说明它就是不允许跨域的。
- 
nginx配置文件的`http`配置部分不能用`if`条件语句，所以多域名的时候必须加在`local`部分内。另外加在`local`内的只对对应的服务器域名做跨域请求的配置，加在`http`里会让跑在该nginx下的所有网站都统一采取这种配置。
- 
`Access-Control-Allow-Origin`也可以改成全小写的形式，不影响结果.(`access-control-allow-origin`也可以)
PS：这篇文章写的可能有点绕，另外由于运维方面和WEB安全方面不是特别熟悉，所以后面配置那里未必是最优解，恳请看到的各位指点。
## 参考文献
- 
[Same-origin policy](https://en.wikipedia.org/wiki/Same-origin_policy)
- 
[HTTP access control
 (CORS)](https://developer.mozilla.org/en-US/docs/Web/HTTP/Access_control_CORS)
- 
[Using CORS](http://www.html5rocks.com/en/tutorials/cors/)
- 
[利用CORS实现跨域请求](http://newhtml.net/using-cors/) (是参考文献2的中译版，配置参数解析那里有所参考)
- 
[enable cross-origin resource sharing](http://enable-cors.org/)
