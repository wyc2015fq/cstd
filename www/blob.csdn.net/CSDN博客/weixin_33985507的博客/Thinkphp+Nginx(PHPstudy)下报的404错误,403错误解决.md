# Thinkphp+Nginx(PHPstudy)下报的404错误,403错误解决 - weixin_33985507的博客 - CSDN博客
2017年12月12日 11:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

最近一个TP5的项目说放到Nginx下测试看看，下载个 PHPstudy,放到WWW下，配置好域名，直接给个报个404；
解决方法：
1.先在phpstudy下配置好域名目录指向项目下的public下；
2.复制下面这段代码放进nginx/conf/vhost文件里：
 location / {
   index  index.html index.htm index.php;
        #autoindex  on;
       if (!-e $request_filename) {
       rewrite ^(.*)$ /index.php?s=/$1 last;
       break;
    }
		}
代码截图如下：
![](https://images2015.cnblogs.com/blog/1080006/201706/1080006-20170620210934366-962536754.jpg)
3.重新启动nginx即可，上面这段代码在TP5手册的部署里也有提到;
4.上面红色是的什么呢；
　　当访问该网站的时，nginx 会按照 index.html，index.htm ，index.php 的先后顺序在根目录中查找文件。如果这三个文件都不存在，那么nginx就会返回403 Forbidden。
　　所以，在vhost里没有这段内容直接输入域名访问就会报403的错误，除非你在域名后面加个 /index.php才可以正常访问;
