# 【备忘】Windows的命令行下设置网络代理 - =朝晖= - 博客园
# [【备忘】Windows的命令行下设置网络代理](https://www.cnblogs.com/dhcn/p/7115334.html)
在公司访问网络的时候，需要设置代理，设置浏览器的代理是相当的方便了。
但有的时候要使用命令行，就需要自己设置了（貌似只要浏览器设置好了，可以直接使用命令行，但我们公司的借，需要有用户名和密码，如果没有使用浏览器先输入的话，在命令行下还是无法访问网络的），设置的方式也比较简单，只需要设置http_proxy,http_proxy_user和http_proxy_pass三个环境变量就可以了。
在命令行下，执行如下命令：
```
set http_proxy=http://proxy.com:port/  
set http_proxy_user=username  
set http_proxy_pass=password
```
之后就可以通过代理访问网络了。
如果不想每次都设置，可以将这些环境变量，设置到系统的环境变量中。右击我的电脑–>属性–>高级–>环境变量–>系统变量，用新建的方式去设置代理服务器。
http://puras.cn/win-cmd-http-proxy
