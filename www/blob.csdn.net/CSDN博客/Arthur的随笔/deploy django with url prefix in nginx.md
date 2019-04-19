# deploy django with url prefix in nginx - Arthur的随笔 - CSDN博客
2013年03月28日 21:53:20[largetalk](https://me.csdn.net/largetalk)阅读数：1754
个人分类：[linux																[django](https://blog.csdn.net/largetalk/article/category/716448)](https://blog.csdn.net/largetalk/article/category/727912)
我以前一般部署网站都是部署在根目录下， 比如www.example.com 直接转发到 后端服务器监听的端口，这种很简单，只要这样即可：
```
location / {
              proxy_pass http://127.0.0.1:8888 ;
	    }
```
但如果我们想将网站部署在某个子目录下， 比如www.example.com/admin/ ， 则不是那么简单的事
幸好django框架还是比较完善的，很容易就可以完成这个任务
[https://docs.djangoproject.com/en/dev/ref/settings/#force-script-name](https://docs.djangoproject.com/en/dev/ref/settings/#force-script-name)
通过在settings.py里设置FORCE_SCRIPT_NAME, 可以很容易的给url前面加个前缀
如 FORCE_SCRIPT_NAME='/admin' （注意， 最后是没有斜杠 / 的）
另外需要注意的是django template里的url必须用{% url url_name %}生成， 后端的redirect也必须用reverse
然后在nignx里需要如下配置：
```
location ~ ^/admin(/?)(.*) {
              proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
              proxy_set_header Host $http_host;
              proxy_pass http://127.0.0.1:8888/$2 ;
              proxy_redirect http://www.example.com/ http://www.example.com/admin/;
	    }
```
ref: [http://nginx.org/cn/docs/http/ngx_http_proxy_module.html](http://nginx.org/cn/docs/http/ngx_http_proxy_module.html)
稍微介绍下：
proxy_pass 格式是 proxy_pass http://host:port/uri/ 匹配location的url会被转成uri， 所以 http://127.0.0.:8888会转发完整的url， 而http://127.0.0.1:8888/ 则会将localtion匹配的去掉
请求URI按下面规则传送给后端服务器：
- 如果`proxy_pass`使用了URI，当传送请求到后端服务器时，[规范化](http://nginx.org/cn/docs/http/ngx_http_core_module.html#location)以后的请求路径与配置中的路径的匹配部分将被替换为指令中定义的URI：
> location /name/ {
    proxy_pass http://127.0.0.1/remote/;
}
- 如果`proxy_pass`没有使用URI，传送到后端服务器的请求URI一般客户端发起的原始URI，如果nginx改变了请求URI，则传送的URI是nginx改变以后完整的规范化URI：
> location /some/path/ {
    proxy_pass http://127.0.0.1;
}
proxy_redirct 作用是对返回的调整url做一个替换
|语法:|`**proxy_redirect** `default`;``**proxy_redirect** `off`;``**proxy_redirect** `*redirect*``*replacement*`;`|
|----|----|
|默认值:|proxy_redirect default;|
|上下文:|`http`, `server`, `location`|
设置后端服务器“Location”响应头和“Refresh”响应头的替换文本。 假设后端服务器返回的响应头是 “`Location: http://localhost:8000/two/some/uri/`”，那么指令
> proxy_redirect http://localhost:8000/two/ http://frontend/one/;
将把字符串改写为 “`Location: http://frontend/one/some/uri/`”。
`*replacement*`字符串可以省略服务器名：
> proxy_redirect http://localhost:8000/two/ /;
ps. nginx的配置真的有很多很多，想要用好用熟一门工具或语言真的是不容易
##############################################################
补充：上面的配置在GET请求的时候会将带的参数弄掉，改成这样就好了：
```
location ^~ /admin/ {
              proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
              proxy_set_header Host $http_host;
              proxy_pass http://127.0.0.1:8888/ ;
              proxy_redirect http://www.example.com/ http://www.example.com/admin/;
	    }
```
因为这个，我仔细看了下nginx关于location的文档，以前从来没有看过，都是拷来拷去加上自以为是的理解
[http://wiki.nginx.org/HttpCoreModule#location](http://wiki.nginx.org/HttpCoreModule#location)
location语法：
|**Syntax:**|**location** [ `=` | `~` | `~*` | `^~` ] uri { ... }|
首先location的url匹配分为两大类： 正则匹配和普通（字面）匹配， 有～（大小写敏感）或～*（大小写不敏感）的就是正则匹配，其他的（=前缀， ^~前缀和无前缀）都是普通匹配
匹配的顺序是：
1. 匹配精确匹配的（=前缀），如果有，停止匹配
2. 匹配普通匹配， 如果是^~前缀， 停止匹配， 否则会进行第三步的正则匹配
3. 匹配正则匹配， 按正则定义顺序匹配，一旦匹配上停止匹配
4. 如没有正则匹配上，返回步骤2得到的匹配（最大前缀匹配， 如同时匹配上 / 和 /doc/ 则会返回 /doc/ 的匹配）
虽然这节内容没有让我明白正则匹配为何会对GET方法参数有影响，但让我更好的理解了nginx的location的匹配规则

