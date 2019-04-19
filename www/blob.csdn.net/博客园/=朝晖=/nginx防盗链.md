# nginx防盗链 - =朝晖= - 博客园
# [nginx防盗链](https://www.cnblogs.com/dhcn/p/7116222.html)
盗链是指一个网站的资源(图片或附件)未经允许在其它网站提供浏览和下载。
尤其热门资源的盗链，对网站带宽的消耗非常大，本文通过nginx的配置指令location来实现简单的图片和其它类型文件的防盗链。
Nginx 的配置文件 :
```
location ~ \.(jpe?g|png|gif)$ {  
     valid_referers none blocked mysite.com *.mysite.com;  
     if ($invalid_referer) {  
        return   403;  
    }  
}
```
用 (“|”) 来分隔你想保护的文件的扩展名。
valid_referers指令包含允许访问资源的网站列表,不在列表中请求的返回403。下面是valid_referers指令参数的解释 :
none - 匹配没有Referer的HTTP请求(Matches the requests with no Referer header).
blocked - 请求有Referer ，但是被防火墙或者代理服务器修改，去掉了https://或http:// (Matches the requests with blocked Referrer header).
*.mydomain.com - 匹配mysite.com的所有二级域名(Matches all the sub domains of mydomain.com. Since v0.5.33, * wildcards can be used in the server names).
除了使用location对文件访问进行限制，也可以对特定目录进行限制，下面的配置会禁止访问images目录下所有文件
```
location /images/ {  
     valid_referers none blocked mysite.com *.mysite.com;  
     if ($invalid_referer) {  
        return   403;  
    }  
}
```
以上配置都是简单通过验证请求头来实现防盗链，如果盗链的网站通过伪造来路的http请求时不能屏蔽。
参考：
http://www.nginx.cn/657.html
http://blogread.cn/it/article/1536

