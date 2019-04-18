# HTTP 请求头中的 X-Forwarded-For，X-Real-IP - z69183787的专栏 - CSDN博客
2019年04月12日 18:40:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17
个人分类：[Nginx](https://blog.csdn.net/z69183787/article/category/5774417)
[https://www.cnblogs.com/diaosir/p/6890825.html](https://www.cnblogs.com/diaosir/p/6890825.html)
**X-Forwarded-For**
在使用nginx做反向代理时，我们为了**记录整个的代理过程**，我们往往会在配置文件中做如下配置:
```bash
location / {
       省略...
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_pass http://1xx.xxx.xxx.xxx;
    
    }
```
**红色部分就是为了记录代理过程做的配置，在http header中添加代理的信息,我们可以把X-Forwarded-For当成http扩展头,其格式一般为:**
****X****-Forwarded-****For**:192.168.247.1, 192.168.247.131, 192.168.247.132**
注意，引用X-Forwarded-For时要这样$http_x_forwarded_for
**做一下测试**
1.我们测试一下请求经过三层代理的情况，测试设备分配:
- win10 一台
- 运行在win10上的虚拟机centos6-0，ip:192.168.247.131，一级代理
- 运行在win10上的虚拟机centos6-1, ip:192.168.247.132 ,二级代理
- 运行在win10上的虚拟机centos6-2, ip:192.168.247.133 ,三级代理
- 云服务器,应用服务器
![](https://images2015.cnblogs.com/blog/1086077/201705/1086077-20170522183840413-1959294813.png)
2.测试环境配置:
- win10 在/etc/hosts文件中添加192.168.247.131 http://test.proxy.com
- centos6-0，ip:192.168.247.131，安装nginx，把所有请求转发到192.168.247.132
- centos6-1, ip:192.168.247.132，安装nginx，把所有请求转发到192.168.247.133
- centos6-2, ip:192.168.247.133，安装nginx，把所有请求转发到云服务器
- 在云服务器上的日志中打印http header中的X-Forwarded-For信息
- 防火墙可以关闭掉，防止win10请求无法进入代理链
3.nginx配置文件
```bash
#centos6-0，ip:192.168.247.131 ,nginx.conf
location / {
            root   html;
            index  index.html index.htm index.php;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_pass http://192.168.247.132;
        }
#centos6-1，ip:192.168.247.132 ,nginx.conf
    location / {
      root   html;
      index  index.html index.htm index.php;
      #proxy_set_header X-Real-IP $remote_addr;
      proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
      proxy_pass http://192.168.247.133;
    } 
#centos6-2，ip:192.168.247.133 ,nginx.conf
    location / {
        root   html;
        index  index.html index.htm index.php;　　
        #proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_pass http://123.206.96.111;
    }
#云服务器方便起见在日志中设置打印$http_x_forwarded_for，进行观察
 log_format  main  '$http_x_forwarded_for|$http_x_real_ip|$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';
```
4.基于上面的配置在win10浏览器输入:"http://test.proxy.com" 查看云服务器日志打印结果如下:
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.1|101.254.182.6 - - 
[22/May/2017:18:20:27 +0800] "GET /admin/login/?next=%2Fadmin%2F HTTP/1.0" 200 623 
"http://test.proxy.com/admin/login/?next=%2Fadmin%2F" "Mozilla/5.0 (Windows NT 10.0; WOW64) 
AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 
192.168.247.131, 192.168.247.132"
```
192.168.247.1, 192.168.247.131, 192.168.247.132 为**$http_x_forwarded_for内容，显然记录了代理过程，其中192.168.247.1是客户端ip**
192.168.247.1 为基于上述设置的真实IP(不一定准确）
101.254.182.6 公网IP
继续。。。
我们要仔细测试一下在不同代理服务器设置X-FORWARDED-FOR在应用服务器拿到的**$http_x_forwarded_for**有何不同
1.只在proxy01设置X-FORWARDED-FOR, 在proxy02,proxy03配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.1|192.168.247.1|101.254.182.6 - - [22/May/2017:18:52:49 +0800] "GET /admin/login/?
next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?next=%2Fadmin%2F" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1"
```
2.只在proxy02设置X-FORWARDED-FOR, 在proxy01,proxy03配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.131|192.168.247.1|101.254.182.6 - - [22/May/2017:18:59:59 +0800] "GET /admin/login/?
next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?next=%2Fadmin%2F" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.131"
```
3.只在proxy03设置X-FORWARDED-FOR, 在proxy01,proxy02配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.132|192.168.247.1|101.254.182.6 - - [22/May/2017:19:01:27 +0800] "GET /admin/login/?
next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?next=%2Fadmin%2F" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.132"
```
4.只在proxy01，proxy03设置X-FORWARDED-FOR, 在proxy02配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.1, 192.168.247.132|192.168.247.1|101.254.182.6 - - [22/May/2017:19:05:49 +0800] "GET 
/admin/login/?next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?
next=%2Fadmin%2F" "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.132"
```
5.只在proxy02，proxy03设置X-FORWARDED-FOR, 在proxy01配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.131, 192.168.247.132|192.168.247.1|101.254.182.6 - - [22/May/2017:19:08:39 +0800] 
"GET /admin/login/?next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?
next=%2Fadmin%2F" "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.131, 192.168.247.132"
```
6.只在proxy01，proxy02设置X-FORWARDED-FOR, 在proxy03配置文件中注释掉proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
```bash
192.168.247.1, 192.168.247.131|192.168.247.1|101.254.182.6 - - [22/May/2017:19:10:40 +0800] "GET
 /admin/login/?next=%2Fadmin%2F HTTP/1.0" 200 623 "http://test.proxy.com/admin/login/?
next=%2Fadmin%2F" "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131"
```
** 小结：**
**1.通过以上几种情况我们可以了解到设置X-Forwarded-For是一个可叠加的过程，后面的代理会把前面代理的IP加入X-Forwarded-For，类似于python的列表append的作用.**
**2.我们看到在三层代理情况下无论如何设置，应用服务器不可能从$http_x_forwarded_for拿到与它直连的这台服务器的ip（proxy03 ip）,此时我们可以使用$remote_addr（远程ip,表示直连的那台代理）.一句话，当前服务器无法通过**$http_x_forwarded_for**获得上级代理或者客户端的ip,应该使用**$remote_addr**.**
 3.在代理过程中至少有一个代理设置了proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;否则后面代理或者应用服务器无法获得相关信息.
4.注意，应用服务器可以通过$proxy_add_x_forwarded_for客户端IP（只要至少proxy01代理设置了proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;我们取第一IP就好了），但是我们要考虑客户端伪造头部的情况,如下示例：
假设我们在所有代理都加上了proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;然后我们在proxy01机器上本机curl代替win10模拟一个客户端请求，
在proxy01上执行: ***curl localhost/admin -H 'X-Forwarded-For: 1.1.1.1' -H 'X-Real-IP: 2.2.2.2'***
```bash
1.1.1.1, 127.0.0.1, 192.168.247.131, 192.168.247.132|127.0.0.1|101.254.182.6 - - 
[23/May/2017:11:02:09 +0800] "GET /admin HTTP/1.0" 301 263 "-" "curl/7.15.5 (i386-redhat-linux-
gnu) libcurl/7.15.5 OpenSSL/0.9.8b zlib/1.2.3 libidn/0.6.5" "1.1.1.1, 127.0.0.1, 
192.168.247.131, 192.168.247.132"
```
可以看到，1.1.1.1放到了最前面，所以我们不能够想当然的去取第一个ip作为客户端的这是IP.这里127.0.0.1是真实IP.
5.虽然***X-Forwarded-For***可以伪造，但是对我们依然有用，比如我们就从proxy01代理往后截取就行了，这样就能做到直接忽视伪造得IP.
****X-Real-IP****
********下面我们看一下有多级代理存在时如何获取客户端真实IP.********
首先要明确在header里面的 X-Real-IP只是一个变量，后面的设置会覆盖前面的设置（跟*X-Forwarded-For的追加特性区别明显*）,所以我们一般只在第一个代理设置proxy_set_header X-Real-IP $remote_addr;就好了，然后再应用端直接引用$http_x_real_ip就行.
1.假如我们只在proxy01设置了 X-Real-IP
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.1|101.254.182.6 - - 
[23/May/2017:11:23:00 +0800] "GET /test/ HTTP/1.0" 200 9 "http://test.proxy.com/test/" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131, 192.168.247.132"
```
2.假如我们只在proxy02设置了X-Real-IP
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.131|101.254.182.6 - - 
[23/May/2017:11:26:22 +0800] "GET /test/ HTTP/1.0" 200 9 "http://test.proxy.com/test/" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131, 192.168.247.132"
```
3.假如我们只在proxy03设置了X-Real-IP
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.132|101.254.182.6 - - 
[23/May/2017:11:27:21 +0800] "GET /test/ HTTP/1.0" 200 9 "http://test.proxy.com/test/" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131, 192.168.247.132"
```
4.所有代理都设置X-Real-IP
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.132|101.254.182.6 - - 
[23/May/2017:11:29:09 +0800] "GET /test/ HTTP/1.0" 200 9 "http://test.proxy.com/test/" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131, 192.168.247.132"
```
5.强迫症来了，再试一个只设置proxy01,proxy02的看看
```bash
192.168.247.1, 192.168.247.131, 192.168.247.132|192.168.247.131|101.254.182.6 - - 
[23/May/2017:11:30:36 +0800] "GET /test/ HTTP/1.0" 200 9 "http://test.proxy.com/test/" 
"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 
Chrome/57.0.2987.110 Safari/537.36" "192.168.247.1, 192.168.247.131, 192.168.247.132"
```
假如有人假冒***X-Real-IP***呢？
6. 在proxy01上执行: ***curl localhost/admin -H 'X-Forwarded-For: 1.1.1.1' -H 'X-Real-IP: xx.xx.xx.xx'***
```bash
1.1.1.1, 127.0.0.1, 192.168.247.131, 192.168.247.132|192.168.247.131|101.254.182.6 - -
 [23/May/2017:11:36:02 +0800] "GET /admin HTTP/1.0" 301 263 "-" "curl/7.15.5 (i386-redhat-linux-
gnu) libcurl/7.15.5 OpenSSL/0.9.8b zlib/1.2.3 libidn/0.6.5" "1.1.1.1, 127.0.0.1, 
192.168.247.131, 192.168.247.132"
```
并没有影响.
