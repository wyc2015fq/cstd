# Nginx接收的host值会影响alias的规则匹配 - =朝晖= - 博客园
# [Nginx接收的host值会影响alias的规则匹配](https://www.cnblogs.com/dhcn/p/7121445.html)
一般内网接收的HTTP请求都是内网唯一的网关传过来的，nginx的alias匹配会直接使用网关穿过的host值，而不是从URL解析出来的，从而导致的问题是，容器的alias相关Server_name规则会失效，因为根据网关传过来的自定制host可能在nginx配置里面找不到匹配的server_name,然后最后的结果就是Nginx配置里面第一个server_name配置接收这个请求。
所以要求网关传过来的HOST必须与URL里面的原host一致。
解决办法：网关在定义upstream值是，最好就定义成相关的域名，因为这个upstream值会作为host传给二级nginx服务器。
再多说一句：alias项的配置，url和文件目录的最后是否有斜杠，必须一致。

