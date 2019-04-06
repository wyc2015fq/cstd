# Web 前端优化最佳实践之 Server 篇

 

转载：<http://www.dbanotes.net/web/best_practices_for_speeding_up_your_web_site_server.html>

Web 前端优化最佳实践第二部分面向 Server 。目前共计有 6 条实践规则。【注，这最多算技术笔记，查看最原始内容，还请访问：[Exceptional Performance : Best Practices for Speeding Up Your Web Site](http://developer.yahoo.com/performance/rules.html) 】

#### 1. 使用 CDN (Use a Content Delivery Network)

国内 CDN 的普及还不够。不过我们有**独特的电信、网通之间的问题**，如果针对这个作优化，基本上也算能收到 CDN 或类似的效果吧(假装如此)。【Tin 说国内 CDN 用的挺多，看看 CDN 厂商的市场就知道了，还没走入寻常百姓家】

#### 2. 添加 Expires 或 Cache-Control 信息头 (Add an Expires or a Cache-Control Header)

 各个浏览器都有针对的方案, Apache 例子【注意：下面的说明例子还不够精细，具体的环境上还要加一些调整】: 

```
ExpiresActive On
ExpiresByType image/gif "modification plus 1 weeks"
```

Lighttpd 启用 mod_expire 模块 后：

```
$HTTP["url"] =~ "/.(jpg|gif|png)___FCKpd___1quot; {
     expire.url = ( "" => "access 1 years" )
}
```

Nginx 例子参考:

```
location ~* /.(jpg|gif|png)$ {
  if (-f $request_filename) {
        expires      max;
    break; 
  }        
}
```

 

#### 3. 压缩内容 (Gzip Components)

 

对于绝大多数站点，这都是必要的一步，能有效减轻网络流量压力。或许有人担心对 CPU 压缩对于 CPU 的影响，放心大胆的整吧，没事儿。Nginx 例子：

```
gzip            on;
gzip_types      text/plain text/html text/css ext/javascript;
```

另外参见： 

IIS 如何启用 Gzip 压缩? 



#### 4. 设置 Etags (Configure ETags)

对于 [Etag](http://www.dbanotes.net/web/http_11_etag_lastmodified.html)，可能是多数网站维护者都会忽略的地方。在这一系列优化规则出现之前，可能互联网上绝大多数站点都对这个问题忽略了。当然，Etag 对多数站点性能的影响并不是很大。除非是面向 RSS 的网站。【看到有朋友批评说写的简略，并且说 IE 不支持 ETag。明确说一下：IE 支持 ETag，倒是使用 IIS 要注意相关 Etag Bug。】

补充：我的意思是"很多网站在不注意的情况下都是打开 Etag 的，而没有网站关心如何用，消耗资源而不知。并不是说 Etag 不好，合理利用 Etag ，绝对能取得很好的收益.

#### 5. 尽早刷新 Buffer (Flush the Buffer Early)

对这一条，琢磨了半天，貌似还是**异步**的思路。能更好的提升用户体验? 

#### 6. 对 AJAX 请求使用 GET 方法 (Use GET for AJAX Requests)

XMLHttpRequest POST 要两步，而 GET 只需要一步。但要注意的是在 IE 上 GET 最大能处理的 URL 长度是 2K。 