# Nginx日志配置及配置调试 - =朝晖= - 博客园
# [Nginx日志配置及配置调试](https://www.cnblogs.com/dhcn/p/7124696.html)
        防火墙内的内网服务器，因为网关传过来的remot_addr都一样，不得不对Nginx的日志格式做了配置
配置语法如下：
- log_format  myformat  '$http_x_forwarded_for - $remote_user [$time_local] "$request" '  
-                                         '"$status" $body_bytes_sent "$http_referer" '  
-                                         '"$http_user_agent" "$http_x_forwarded_for" '  
-                                         '"$gzip_ratio" $request_time $bytes_sent $request_length';  
上面这只是定义一个格式，要使用这个格式，很多人采用server里面加access_log,其实不用，直接http里面的access_log指令出改配置，可以改全局的日志格式：
- access_log /var/log/nginx/access.log myformat;  
如果你的nginx服务器内部有其他容器，可以直接把容器传参的params设置中的
#$remote_addr
$remote_addr也直接改掉，这样在容器里面少折腾一点。
       顺便谈谈Nginx的配置调试，你使用reload或者restart装载新配置，如果新配置格式有误，它不会报错，可以启动成功，所以改了配置以后，最好用stop和start指令来，这样，启动失败，就可以确定为配置格式问题，否则，新配置不会生效，你也不知道是配置格式问题。
 参考资料：
http://www.ttlsa.com/[Linux](http://lib.csdn.net/base/linux)/the-nginx-log-configuration/
http://www.nginx.cn/273.html

