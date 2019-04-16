# 服务器数据库系列 - Nginx 负载均衡配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月02日 14:32:31[initphp](https://me.csdn.net/initphp)阅读数：933








```
http
{
  upstream www.test2.com {
        	server 10.1.165.36:80  weight = 3;        
	server 10.249.198.235;
         server 10.1.168.1  down; 
         server 10.1.168.2  backup;
        #ip_hash;
   }
  server
  {
        location / {
              proxy_pass        http://www.test2.com;
              proxy_set_header   Host             $host;
              proxy_set_header   X-Real-IP        $remote_addr;
              proxy_set_header  X-Forwarded-For  $proxy_add_x_forwarded_for;
              }
}
```
- Nginx负载均衡需要添加 upstream  和在Server中添加proxy_pass
- server 10.1.165.36:80  weight = 3;  代表这个服务器会有相同概率的3倍被访问
- server 10.1.168.1  down;  暂时不使用
- server 10.1.168.2  backup; 压力大的时候才会启动
- ip_hash;通过IP地址进行hash匹配。但是会有一些弊端，比如Nginx不在最外层的话就无法获取IP等
- proxy_pass [http://www.test2.com](http://www.test2.com)  代表访问的网址

参考：Nginx官网



