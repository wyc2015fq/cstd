# nginx 屏蔽恶意请求 - =朝晖= - 博客园
# [nginx 屏蔽恶意请求](https://www.cnblogs.com/dhcn/p/9967918.html)
https://www.xlongwei.com/detail/nginx-%E5%B1%8F%E8%94%BD%E6%81%B6%E6%84%8F%E8%AF%B7%E6%B1%82
nginx可以很方便地做访问控制，特别是一些偶发性的大量恶意请求，需要屏蔽处理。
- 屏蔽ip地址
location /someapi/ {
allow ip;  #特定接口只开放给某个ip调用
deny all;
}
location /somepage/ {
deny ip;   #屏蔽某个ip访问（iptables可以拒绝某个ip连接）
allow all;
}
- 屏蔽user-agent
if ($http_user_agent = Mozilla/5.0 ) { return 403; }   #有些请求头很明显不是用户浏览器
分析nginx日志，找出恶意ip或user-agent
cat /var/log/nginx/access.log | awk -F\" '{A[$(NF-1)]++}END{for(k in A)print A[k],k}' | sort -n |tail
122 58.144.7.66
337 106.91.201.75
2270 122.200.77.170  #显然这个ip不正常，而且这不是nginx所知道的真实ip，而是$http_x_forwarded_for变量
- 屏蔽代理ip，有两种情形会需要屏蔽代理ip：一是代理ip访问，二是负载均衡（real-ip请求负载均衡服务器，再代理给后端server）
vi /etc/nginx/badproxy.rules
map $http_x_forwarded_for $badproxy {
        default 0;
        ~*122.200.77.170  1;  #建立映射
}
vi /etc/nginx/nginx.conf
http {
include /etc/nginx/badproxy.rules  #这个要在server配置之前
server {
    location /somepage/ {
        if ( $badproxy ) { return 403; }
    }
}
}
