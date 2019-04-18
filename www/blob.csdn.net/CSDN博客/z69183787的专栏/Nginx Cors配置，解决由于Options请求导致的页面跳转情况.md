# Nginx Cors配置，解决由于Options请求导致的页面跳转情况 - z69183787的专栏 - CSDN博客
2018年01月02日 18:02:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6489
首先，普及一下Cors中简单请求与非简单请求的区别
浏览器将CORS请求分成两类：简单请求（simple request）和非简单请求（not-so-simple request）。
只要同时满足以下两大条件，就属于简单请求。
（1) 请求方法是以下三种方法之一：
HEAD
GET
POST
（2）HTTP的头信息不超出以下几种字段：
Accept
Accept-Language
Content-Language
Last-Event-ID
Content-Type：只限于三个值application/x-www-form-urlencoded、multipart/form-data、text/plain
凡是不同时满足上面两个条件，就属于非简单请求。
浏览器对这两种请求的处理，是不一样的。
当在fetch或ajax中设置了自定义header 比如 Content-Type时，
fetch(
"https://xxx.com/brand/queryList",    {credentials: 'include',method:'get',headers:{"cache-control":"application/json"}}).then(function(Response){returnResponse.json();}).then(function(a){console.log(a)});
返回
Fetch API cannot load https://xxxx/brand/queryList. Response for preflight is invalid (redirect)
暗示由于是Cors的非简单请求，所以首先会发送一个Options预检请求，
浏览器先询问服务器，当前网页所在的域名是否在服务器的许可名单之中，以及可以使用哪些HTTP动词和头信息字段。只有得到肯定答复，浏览器才会发出正式的XMLHttpRequest请求，否则就报错。
由于此时用户没有登录态，页面跳转，导致预检失败。
思考之后发现，只要在登录态校验前，保证Options请求正常返回，且可使用的请求头与允许的一致即可。
一种方法是在增加一个过滤器针对requestMethod为Options的请求单独处理，另一种就是在前置Nginx中判断了。
几经试验后得出可以使用的结论：
if ($request_method = 'OPTIONS') {
    add_header 'Access-Control-Allow-Credentials' true;
    add_header 'Access-Control-Allow-Origin' "$http_origin";
    add_header 'Access-Control-Allow-Methods' 'GET, POST, OPTIONS';
    add_header 'Access-Control-Allow-Headers' 'DNT,X-CustomHeader,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Content-Range,Range';
    add_header 'Access-Control-Max-Age' 1728000;
    add_header 'Content-Type' 'text/plain; charset=utf-8';
    add_header 'Content-Length' 0; return 204;
}
浏览器对于非简单的请求，会分两次应答。第一次 预检请求（method：OPTIONS），主要验证来源是否合法，并返回允许的 Header 等。第二次才是真正的 HTTP 应答。所以服务器必须处理 OPTIONS 应答，OPTIONS 请求要做 204 处理。
以下为需在 nginx 中配置的几个参数：
Access-Control-Allow-Origin（必含）：允许的域名，只能填通配符或者单域名
Access-Control-Allow-Methods（必含）：允许跨域请求的 http 方法
Access-Control-Allow-Headers：返回支持的 http 请求头
Access-Control-Allow-Credentials（可选）：标志着当前请求是否包含 cookies 信息，布尔值。只有一个可选值：true，如果不包含 cookies，请略去该项，而不是填 false。这一项与XmlHttpRequest2 对象当中的 withCredentials 属性应保持一致，即 withCredentials 为true时该项也为 true；withCredentials为false时，省略该项不写。反之则导致请求失败。
Access-Control-Max-Age（可选）：以秒为单位的缓存时间，用于缓存预检请求。

