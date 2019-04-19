# Nginx 教程（3）：SSL 设置 - 文章 - 伯乐在线
原文出处： [netguru](https://www.netguru.co/codestories/nginx-tutorial-ssl-setup)   译文出处：[开源中国翻译](https://www.oschina.net/translate/nginx-tutorial-ssl-setup)
大家好！分享即关怀，我们很乐意与你分享其他的一些知识。我们准备了一个 Nginx 指南，分为三个系列。如果你已经知道一些 Nginx 知识或者想扩展你的经验和认知，这个再合适不过了。
我们将告诉你 Nginx 的运作模式、蕴含的概念，怎样通过调优 Nginx 来提高应用性能，或是如何设置它的启动和运行。
## 这个教程有三个部分：
- [基本概念](https://www.oschina.net/translate/nginx-tutorial-basics-concepts) —— 这部分需要去了解 Nginx 的一些指令和使用场景，继承模型，以及 Nginx 如何选择 server 块，location 的顺序。
- [性能](https://www.oschina.net/translate/nginx-tutorial-performance) —— 介绍改善 Nginx 速度的方法和技巧，我们会在这里谈及 gzip 压缩，缓存，buffer 和超时。
- [SSL 安装](https://www.oschina.net/translate/nginx-tutorial-ssl-setup) —— 如何配置服务器使用 HTTPS
创建这个系列，我们希望，一是作为参考书，可以通过快速查找到相关问题（比如 gzip 压缩，SSL 等）的解决方式，也可以直接通读全文。为了获得更好的学习效果，我们建议你在本机安装 Nginx 并且尝试进行实践。
## SSL 和 TLS
SSL（Socket Secure Layer 缩写）是一种通过 HTTP 提供安全连接的协议。
SSL 1.0 由 Netscape 开发，但由于严重的安全漏洞从未公开发布过。SSL 2.0 于 1995 年发布，它存在一些问题，导致了最终的 SSL 3.0 在 1996 年发布。
TLS（Transport Layer Security 缩写）的第一个版本是作为 SSL 3.0 的升级版而编写的。之后 TLS 1.1 和 1.2 出来了。现在，就在不久之后，TLS 1.3 即将推出（这确实值得期待），并且已经被一些[浏览器](https://caniuse.com/#feat=tls1-3)所支持。
从技术上讲，SSL 和 TLS 是不同的（因为每个协议都描述了协议的不同版本），但其中使用的许多名称是可以互换的。
## 基本 SSL/TLS 配置
为了处理 HTTPS 流量，你需要具有 SSL/TLS 证书。你可以通过使用 Let’s encrypt 以生成免费的证书。
当你拥有证书之后，你可以通过以下的方式轻易切换至 HTTPS：
- 开始监听端口 443（当你输入 https://sample.co 时浏览器将使用的默认端口）
- 提供证书及其密钥


```
server {
  listen 443 ssl default_server;
  listen [::]:443 ssl default_server;
  ssl_certificate /etc/nginx/ssl/netguru.crt;
  ssl_certificate_key /etc/nginx/ssl/netguru.key;
}
```
我们也想通过调整配置实现：
- 仅使用 TLS 协议。由于众所周知的漏洞，所有的 SSL 版本都将不再使用
- 使用预定义的安全的服务器密码（类似于协议的情况 – 那些日子只有少数密码被认为是安全的）
请牢记，上述设置总是在变化的。时不时重新更新是个好主意。


```
ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
ssl_ciphers EECDH+CHACHA20:EECDH+AES128:RSA+AES128:EECDH+AES256:RSA+AES256:!MD5;
ssl_prefer_server_ciphers on;
server {
  listen 443 ssl default_server;
  listen [::]:443 ssl default_server;
  ssl_certificate /etc/nginx/ssl/netguru.crt;
  ssl_certificate_key /etc/nginx/ssl/netguru.key;
}
```
## TLS 会话恢复
使用 HTTPS，在 TCP 之上需要增加 TLS 握手。这大大增加了此前实际数据传输的时间。假设你从华沙请求 /image.jpg，并接入到柏林最近的服务器：


```
Open connection
TCP Handshake:
Warsaw  ->------------------ synchronize packet (SYN) ----------------->- Berlin
Warsaw  -<--------- synchronise-acknowledgement packet (SYN-ACK) ------<- Berlin
Warsaw  ->------------------- acknowledgement (ACK) ------------------->- Berlin
TLS Handshake:
Warsaw  ->------------------------ Client Hello  ---------------------->- Berlin
Warsaw  -<------------------ Server Hello + Certificate ---------------<- Berlin
Warsaw  ->---------------------- Change Ciper Spec -------------------->- Berlin
Warsaw  -<---------------------- Change Ciper Spec --------------------<- Berlin
Data transfer:
Warsaw  ->---------------------- /image.jpg --------------------------->- Berlin
Warsaw  -<--------------------- (image data) --------------------------<- Berlin
Close connection
```
为了在 TLS 握手期间节省一个 roundtrip 时间，以及生成新密钥的计算开销，我们可以重用在第一个请求期间生成的会话参数。客户端和服务器可以将会话参数存储在会话 ID 密钥的后面。在接下来的 TLS 握手过程中，客户端可以发送会话 ID，如果服务器在缓存中仍然有正确的条目，那么会重用前一个会话所生成的参数。


```
server {
    ssl_session_cache shared:SSL:10m;
    ssl_session_timeout 1h;
}
```
## OCSP Stapling
SSL 证书可以随时撤销。浏览器为了知道给定的证书是否不再有效，需要通过在线证书状态协议 (Online Certificate Status Protocol ，OCSP) 执行额外的查询。无需用户执行指定的 OCSP 查询，我们可以在服务器上执行此操作，缓存其结果，并在 TLS 握手期间为客户端提供 OCSP 响应。它被称为OCSP stapling。


```
server {
  ssl_stapling on;
  ssl_stapling_verify on;                               # verify OCSP response
  ssl_trusted_certificate /etc/nginx/ssl/lemonfrog.pem; # tell nginx location of all intermediate certificates
  resolver 8.8.8.8 8.8.4.4 valid=86400s;                # resolution of the OCSP responder hostname
  resolver_timeout 5s;
}
```
## Security headers
有一些标头确实值得调整以提供更高的安全性。有关更多关于标头及其详细信息，你绝对应该查看[OWASP 项目之安全标头](https://www.owasp.org/index.php/OWASP_Secure_Headers_Project)。
### HTTP Strict-Transport-Security
或简称 HSTS，强制用户代理在向源发送请求时使用 HTTPS。


```
add_header Strict-Transport-Security "max-age=31536000; includeSubdomains; preload";
```
### X-Frame-Options
表示浏览器是否需要在一帧、一个 iframe 或一个对象标签中渲染页面。


```
add_header X-Frame-Options DENY;
```
### X-Content-Type-Options
此选项将阻止浏览器在判断文件类型时[嗅探](https://en.wikipedia.org/wiki/Content_sniffing)文件。文件将会按照 Content-Type 头中声明的格式转译。


```
add_header X-Content-Type-Options nosniff;
```
## Server tokens
另一个很好的做法是在 HTTP 响应头字段中隐藏有关 Web 服务器的信息：


```
Server : nginx/1.13.2
```
实现此功能可以通过禁用 server_tokens 指令：


```
server_tokens off;
```
## 附录 :: Let’s Encrypt
### 安装
最新的安装包可以在[这里](https://certbot.eff.org/#ubuntuother-nginx)找到。
为了测试使用[暂存环境](https://letsencrypt.org/docs/staging-environment/)，不排除[速率限制](https://letsencrypt.org/docs/rate-limits/)。
### 生成新证书


```
certbot certonly --webroot --webroot-path /var/www/netguru/current/public/  \
          -d foo.netguru.co \
          -d bar.netguru.co
```
确保能够正确更新。


```
certbot renew --dry-run
```
确保你在 crontab 添加了自动更新。运行 crontab -e，同时添加下边一行代码


```
3 * * * /usr/bin/certbot renew --quiet --renew-hook "/usr/sbin/nginx -s reload"
```
检查 SSL 是否能够通过 [ssllabs](https://www.ssllabs.com/ssltest/) 正常运行。
## 总结
感谢您的阅读。如果没有大量的资源，这个系列是不可能完成的。在这一系列的写作中，我们发现了一些特别有用的网站：
- [nginx 文档](https://nginx.org/en/docs/)
- [nginx 博客](https://www.nginx.com/blog/)
- [udemy（在线教育网站 ）的 nginx 原理](https://www.udemy.com/nginx-fundamentals/)
- [Ilya Grigorik 的博客](https://www.igvita.com/)，和他的令人惊奇的书：[《高性能浏览器网络》](https://hpbn.co/)
- [Martin Fjordvald 的博客](https://blog.martinfjordvald.com/)
