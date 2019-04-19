# Nginx的 ip_hash 负载均衡配置临时解决Session共享问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月18日 14:56:31[boonya](https://me.csdn.net/boonya)阅读数：4标签：[Nginx																[ip_hash](https://so.csdn.net/so/search/s.do?q=ip_hash&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)
个人分类：[Nginx](https://blog.csdn.net/boonya/article/category/2126311)
固定IP hash配置（动态IP不行--未实施Nginx负载均衡设计遇到session共享问题），此方案可作为临时解决方案。
文章来源：[http://www.cnblogs.com/oshine/p/3953259.html](https://link.jianshu.com?t=http://www.cnblogs.com/oshine/p/3953259.html)
![](https://upload-images.jianshu.io/upload_images/3061645-2524421f9df49aae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/491/format/webp)
# ip_hash:
一台服务器就可以完成实验，你得监听端口。。
```
http {
    #在http字段添加
    upstream servers.mydomain.com {
        server 192.168.2.3:8080; 
        server 192.168.2.4:8081; 
        server 192.168.2.5:8082; 
        ip_hash;  #不加这句就不能保持会话，当然有其他方式实现
    }
    server{ 
        listen 80; 
        server_name www.mydomain.com; 
        location / { 
            proxy_pass http://servers.mydomain.com; 
            proxy_set_header Host $host; 
            proxy_set_header X-Real-IP $remote_addr; 
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for; 
        } 
    } 
}
```
下面两个服务端口的内容及监听自己加上，访问servers.mydomain.com就可以通过 ip_hash均衡
192.168.2.3:8080;
192.168.2.4:8081;
192.168.2.5:8082;
