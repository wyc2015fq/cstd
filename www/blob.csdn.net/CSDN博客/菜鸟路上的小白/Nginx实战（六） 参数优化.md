# Nginx实战（六） 参数优化 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 13:40:24[somnus_小凯](https://me.csdn.net/u012486840)阅读数：490
个人分类：[nginx](https://blog.csdn.net/u012486840/article/category/6428325)









Nginx参数配置与优化

## 本机缓存设置

浏览器缓存是为了提高加载速度，因此我们可以通过Nginx对静态文件进行缓存。
- location ~^/(images|javascript|js|css|flash|media|static)/{
- #过期30天
-     expires 30d;
- }

## 定义错误提示页面
- error_page   500502503504/50x.html;
-         location =/50x.html {
-         root   html;
- }

## 自动显示目录
- location /{
-     autoindex on;
- }

此外，还可以添加两个参数
- autoindex_exact_size off; 默认为on，显示出文件的确切大小，单位是bytes。改为off后，显示出文件的大概大小，单位是kB或者MB或者GB
- autoindex_localtime on; 默认为off，显示的文件时间为GMT时间，改为on后，显示的文件时间为文件的服务器时间



