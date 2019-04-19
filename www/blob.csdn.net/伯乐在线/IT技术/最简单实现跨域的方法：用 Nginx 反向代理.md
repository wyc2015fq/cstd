# 最简单实现跨域的方法：用 Nginx 反向代理 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [良少](http://www.jobbole.com/members/shendl) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 什么是跨域
跨域，指的是浏览器不能执行其他网站的脚本。它是由浏览器的同源策略造成的，是浏览器对javascript施加的安全限制。
所谓同源是指，域名，协议，端口相同。浏览器执行javascript脚本时，会检查这个脚本属于那个页面，如果不是同源页面，就不会被执行。
同源策略的目的，是防止黑客做一些做奸犯科的勾当。比如说，如果一个银行的一个应用允许用户上传网页，如果没有同源策略，黑客可以编写一个登陆表单提交到自己的服务器上，得到一个看上去相当高大上的页面。黑客把这个页面通过邮件等发给用户，用户误认为这是某银行的主网页进行登陆，就会泄露自己的用户数据。而因为浏览器的同源策略，黑客无法收到表单数据。
现在随着RESTFUL的流行，很多应用提供http/https接口的API，通过xml/json格式对外提供服务，实现开放架构。如，微博、微信、天气预报、openstack等网站和应用都提供restful接口。
Web应用也在向单页面方向发展。越来越多的web应用现在是这样的架构：
> 
静态单个web页面
ajax调用
RESTFUL服务
我们本可以利用各个网站提供的API，做出很多精彩的Web应用。但浏览器执行javascript时的跨域限制，就成为了这类开放架构的拦路虎。
本文提出了一种简单有效的方式解决跨域问题。
### 常用的跨域方法
常用的跨域方法有这样一些：
1，使用iFrame访问另一个域。 然后再从另一个页面读取iFrame的内容。jquery等有一些封装。据说Firefox等可能不支持读取另一个iFrame的内容。
2，jsonp。需要服务器支持。使用script src动态得到一段java代码。是回调页面上的js函数，参数是一个json对象。jquery也有封装。
3，设置http头，Access-Control-Allow-Origin：*但据说IE有一些版本不识别这个http头。
4，服务器代理。如，服务器写一个url的处理action。其参数是一个url。这个服务器会用参数拼凑一个url,用httpclient库去执行url，然后把读取的内容再输出到http客户端。
### nginx反向代理实现跨域
上面提到的这些跨域方法，都有一些问题。有的不能支持所有浏览器，有的需要修改javascript代码，有的需要重写服务器端代码。有的在session等场景下会有问题。
其实，用nginx反向代理实现跨域，是最简单的跨域方式。只需要修改nginx的配置即可解决跨域问题，支持所有浏览器，支持session，不需要修改任何代码，并且不会影响服务器性能。
我们只需要配置nginx，在一个服务器上配置多个前缀来转发http/https请求到多个真实的服务器即可。这样，这个服务器上所有url都是相同的域名、协议和端口。因此，对于浏览器来说，这些url都是同源的，没有跨域限制。而实际上，这些url实际上由物理服务器提供服务。这些服务器内的javascript可以跨域调用所有这些服务器上的url。
下面，给出一个nginx支持跨域的例子，进行具体说明。
如，我们有两个pythonflask开发的项目：testFlask1和testFlask2。
testFlask2项目上的javascript脚本要通过ajax方式调用testFlask1的一个url，获取一些数据。
正常情况下部署，就会有跨域问题，浏览器拒绝执行如下这样的调用。


```
$("button").click(function(){
   $.get("127.0.0.1:8081/partners/json",
   function(result){
      $("div").html(result);
   });
});
```
下面把testFlask2项目的javascrip文件修改一下。这样访问同源的url，就不会有跨域问题。


```
$("button").click(function(){
$.get("partners/json",
function(result){
$("div").html(result);
});
});
```
但是，我们testFlask2项目实际上没有partners/json这样的url，那怎么处理呢？
我们这样编写nginx的配置文件：


```
server{
listen8000;
location/ {
includeuwsgi_params;
uwsgi_passunix:/tmp/testFlask2.sock;
}
location/partners {
rewrite^.+partners/?(.*)$ /$1 break;
includeuwsgi_params;
uwsgi_passunix:/tmp/testFlask1.sock;
}
}
```
我们把testFlask2项目部署在8080端口的根目录下。把提供web服务的testFlask1项目部署在/partners目录下。
但我们的testFlask1项目并不能处理/partners/json这样的url请求。那怎么办呢？
通过 rewrite^.+partners/?(.*)$ /$1 break; 这一条命令，nginx可以把收到的/partners/*请求全部转为/*请求后再转发给背后的真实web服务器。
这样，RESTFUL的ajax客户端程序，只需要给出特定前缀的url就可以调用任意服务器提供的RESTFUL接口了。
甚至，通过nginx的反向代理，我们还能调用其他公司开发的网站提供的RESTFUL接口。
如，


```
location/sohu {
rewrite^.+sohu/?(.*)$ /$1 break;
includeuwsgi_params;
proxy_passhttp://www.sohu.com/;
}
```
我们就把sohu网站整个搬到我们的8080:/sohu/目录下了，我们的javascript就可以尽情调用其RESTFUL服务了。
顺便说一下，rewrite^.+sohu/?(.*)$ /$1 break; 这句命令中，$1表示(.*)这个部分。第一对()内的参数是$1，第二对()内的参数就是$2，以此类推。
### 总结
本文介绍了利用nginx的反向代理的功能，实现跨域访问任意应用和网站的方法。
nginx是一个高性能的web服务器，常用作反向代理服务器。nginx作为反向代理服务器，就是把http请求转发到另一个或者一些服务器上。
通过把本地一个url前缀映射到要跨域访问的web服务器上，就可以实现跨域访问。
对于浏览器来说，访问的就是同源服务器上的一个url。而nginx通过检测url前缀，把http请求转发到后面真实的物理服务器。并通过rewrite命令把前缀再去掉。这样真实的服务器就可以正确处理请求，并且并不知道这个请求是来自代理服务器的。
简单说，nginx服务器欺骗了浏览器，让它认为这是同源调用，从而解决了浏览器的跨域问题。又通过重写url，欺骗了真实的服务器，让它以为这个http请求是直接来自与用户浏览器的。
这样，为了解决跨域问题，只需要动一下nginx配置文件即可。简单、强大、高效!
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/3a8dd024cd150c597044ab5cfab49f6c1.jpg)
