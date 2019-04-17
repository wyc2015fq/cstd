# Nginx配置限流限连接示例及相关知识汇总 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月01日 17:52:10[boonya](https://me.csdn.net/boonya)阅读数：129标签：[Nginx																[限流量																[限连接](https://so.csdn.net/so/search/s.do?q=限连接&t=blog)
个人分类：[Nginx](https://blog.csdn.net/boonya/article/category/2126311)





**目录**

[*Nginx与Tomcat配置](#*Nginx%E4%B8%8ETomcat%E9%85%8D%E7%BD%AE)

[Nginx初始化限流20MB](#Nginx%E5%88%9D%E5%A7%8B%E5%8C%96%E9%99%90%E6%B5%8120MB)

[模板一](#%E6%A8%A1%E6%9D%BF%E4%B8%80)

[模板二](#%E6%A8%A1%E6%9D%BF%E4%BA%8C)

[模板三](#%E6%A8%A1%E6%9D%BF%E4%B8%89)

[*Nginx文件上传设置](#*Nginx%E6%96%87%E4%BB%B6%E4%B8%8A%E4%BC%A0%E8%AE%BE%E7%BD%AE)

[5MB](#5MB)

[10MB](#10MB)

[15MB](#15MB)

[18MB](#18MB)

[*Nginx高频接口配置[登录、实时位置、通知公告]](#*Nginx%E9%AB%98%E9%A2%91%E6%8E%A5%E5%8F%A3%E9%85%8D%E7%BD%AE%5B%E7%99%BB%E5%BD%95%E3%80%81%E5%AE%9E%E6%97%B6%E4%BD%8D%E7%BD%AE%E3%80%81%E9%80%9A%E7%9F%A5%E5%85%AC%E5%91%8A%5D)

[50r/s](#50r%2Fs)

[100r/s](#100r%2Fs)

[150r/s](#150r%2Fs)

[200r/s](#200r%2Fs)

[参考资料](#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)

[网文参考](#%E7%BD%91%E6%96%87%E5%8F%82%E8%80%83)

[Nginx官文](#Nginx%E5%AE%98%E6%96%87)

[限连：客户端连接限速](#%E9%99%90%E8%BF%9E%EF%BC%9A%E5%AE%A2%E6%88%B7%E7%AB%AF%E8%BF%9E%E6%8E%A5%E9%99%90%E9%80%9F)

[限流：客户端请求的处理速率](#%E9%99%90%E6%B5%81%EF%BC%9A%E5%AE%A2%E6%88%B7%E7%AB%AF%E8%AF%B7%E6%B1%82%E7%9A%84%E5%A4%84%E7%90%86%E9%80%9F%E7%8E%87)

# *******Nginx与Tomcat配置******
|****属性****|****Nginx****|****Tomcat****|
|----|----|----|
|版本|1.14.2|Tomcat7|
|主机|192.168.1.XXX|192.168.1.XXX|
|端口|18080|8080|
|项目名称|xht|xht|



# ******Nginx初始化限流20MB******

## ******模板一******



http{

#限流20MB

#limit_conn_zone $binary_remote_addr zone=one:20m;



    #限流20MB，每秒允许处理1000个请求

limit_req_zone $binary_remote_addr zone=perip:20m rate=1000r/s;

server {

location /xht {

root html;

index index.html index.htm;

limit_req zone=perip;

proxy_pass http://192.168.1.111:8080/xht;

}

}



}

## ******模板二******

http {

    # 总限流20MB 1000次/秒

limit_req_zone $binary_remote_addr zone=totalLimit:20m rate=1000r/s;

# 设置5MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:5m;

server {

location /xht {

root html;

index index.html index.htm;

#总流量限制和突发限制

limit_req zone=totalLimit burst=1000 nodelay;

proxy_pass http://192.168.1.111:8080/xht;

}

# 附件上传

location /uploadAttach.do{

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  1024k;

}

}

}

## ******模板三******

http {

    # 总限流20MB 1000次/秒

limit_req_zone $binary_remote_addr zone=totalLimit:20m rate=1000r/s;

# 设置5MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:5m;

# 设置10MB用于高频接口，每秒请求频率

limit_req_zone $binary_remote_addr zone=feqLimit:10m rate=100r/s;

server {

location /xht {

root html;

index index.html index.htm;

#总流量限制和突发限制

limit_req zone=totalLimit burst=1000 nodelay;

proxy_pass http://192.168.1.111:8080/xht;

}

# 附件上传

location /uploadAttach.do{

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  512k;

}



# 登录

location /userLogin.do {

limit_req zone=feqLimit burst=100;

}

# 实时位置

location /sendXhyPos.do {

limit_req zone=feqLimit burst=100;

}



# 通知公告

location /getMessage.do {

limit_req zone=feqLimit burst=100;

}



}

}



# *******Nginx文件上传设置******

## ******5MB******

http {

# 设置5MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:5m;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

location /uploadAttach.do {

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  512k;

}

}

}





## ******10MB******

http {

# 设置10MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:10m;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

location /uploadAttach.do {

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  512k;

}

}

}



## ******15MB******

http {

# 设置15MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:15m;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

location /uploadAttach.do {

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  512k;

}

}

}



## ******18MB******

http {

# 设置18MB用于图片上传，限制连接个数

limit_conn_zone $binary_remote_addr zone=uploadLimit:18m;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

location /uploadAttach.do {

    # 限制每个IP只允许1个并发连接

    limit_conn uploadLimit 1;

    # 每个连接上传流量限制

limit_rate  512k;

}

}

}



# *******Nginx高频接口配置[登录、实时位置、通知公告]******

## ******50r/s******

http {

limit_req_zone $binary_remote_addr zone=feqLimit:20m rate=50r/s;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

# 后台登录

location /login_gis/loginVerify.do {

limit_req zone=feqLimit burst=50;

}

# 移动端登录

location /userLogin.do {

limit_req zone=feqLimit burst=50;

}



# 实时位置

location /sendXhyPos.do {

limit_req zone=feqLimit burst=50;

}



# 通知公告

location /getMessage.do {

limit_req zone=feqLimit burst=50;

}



}

}

## ******100r/s******

http {

limit_req_zone $binary_remote_addr zone=feqLimit:20m rate=100r/s;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

# 后台登录

location /login_gis/loginVerify.do {

limit_req zone=feqLimit burst=100;

}

# 移动端登录

location /userLogin.do {

limit_req zone=feqLimit burst=100;

}

# 实时位置

location /sendXhyPos.do {

limit_req zone=feqLimit burst=100;

}



# 通知公告

location /getMessage.do {

limit_req zone=feqLimit burst=100;

}



}

}



## ******150r/s******

http {

limit_req_zone $binary_remote_addr zone=feqLimit:20m rate=150r/s;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

# 后台登录

location /login_gis/loginVerify.do {

limit_req zone=feqLimit burst=150;

}

# 移动端登录

location /userLogin.do {

limit_req zone=feqLimit burst=150;

}

# 实时位置

location /sendXhyPos.do {

limit_req zone=feqLimit burst=150;

}



# 通知公告

location /getMessage.do {

limit_req zone=feqLimit burst=150;

}



}

}



## ******200r/s******

http{

limit_req_zone $binary_remote_addr zone=feqLimit:20m rate=200r/s;

server {

location /xht {

root html;

index index.html index.htm;

proxy_pass http://192.168.1.111:8080/xht;

}

# 后台登录

location /login_gis/loginVerify.do {

limit_req zone=feqLimit burst=200;

}

# 移动端登录

location /userLogin.do {

limit_req zone=feqLimit burst=200;

}

# 实时位置

location /sendXhyPos.do {

limit_req zone=feqLimit burst=200;

}



# 通知公告

location /getMessage.do {

limit_req zone=feqLimit burst=200;

}



}



}

# ******参考资料******

## ******网文参考******

[Nginx之限流](https://promagic.cn/nginx-zhixianliu.shtml)



[服务接口API限流 Rate Limit](https://www.cnblogs.com/exceptioneye/p/4783904.html)



[使用nginx实现分布式限流的方法](http://www.iis7.com/a/nr/10573.html)



[nginx + lua + redis 防刷和限流](https://blog.csdn.net/fenglvming/article/details/51996406)



[NGINX上的限流（译）](https://www.jianshu.com/p/2cf3d9609af3)



## ******Nginx官文******

[ngx_http_limit_req_module](http://nginx.org/en/docs/http/ngx_http_limit_req_module.html)


- [Limit_req](#limit_req)


- [limit_req_zone](#limit_req_zone)


- [Limit_status](#limit_req_status)



[变量名称](http://nginx.org/en/docs/varindex.html)

## ******限连：客户端连接限速******

[http://nginx.org/en/docs/http/ngx_http_limit_conn_module.html](http://nginx.org/en/docs/http/ngx_http_limit_conn_module.html)

参考：

[http://www.cnblogs.com/MacoLee/p/6023201.html](http://www.cnblogs.com/MacoLee/p/6023201.html)



http { 

#容器共使用10M的来对于IP传输开销

limit_conn_zone $binary_remote_addr zone=one:10m;

server { 

    location /download/ { 

limit_conn   one  1;#限制每个IP只能发起一个并发连接

limit_rate 300k;#对每个连接限速300k。

} 

  } 

} 

#注意，这里是对连接限速，而不是对IP限速。

#如果一个IP允许两个并发连接，那么这个IP就是限速limit_rate×2。



## ******限流：客户端********请求的处理速率******

[http://nginx.org/en/docs/http/ngx_http_limit_req_module.html](http://nginx.org/en/docs/http/ngx_http_limit_req_module.html)



http {

limit_req_zone $ binary_remote_addr zone = one:10m rate = 1r/s;

server{

        location /upload/{

            limit_req zone= one burst = 5;

     }

}](https://so.csdn.net/so/search/s.do?q=限流量&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)




