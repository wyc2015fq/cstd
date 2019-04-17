# nginx开发（二）配置mp4文件在线播放 - DoubleLi - 博客园






1: 第一步先开打nginx的文件夹遍历功能



vi /usr/local/nginx/conf/nginx.conf #编辑配置文件，在http {下面添加以下内容：

autoindex on; #开启nginx目录浏览功能

autoindex_exact_size off; #文件大小从KB开始显示

autoindex_localtime on; #显示文件修改时间为服务器本地时间

:wq! #保存，退出



2: nginx -s stop ＃停止nginx

    nginx     #重新启动ngnix



3:/usr/local/nginx/html/  此目录时ngnix的默认根目录，在这个目录下创建了multimedia/pd

拷贝一个test.mp4到此目录下.



5:最后，在浏览器目录下打开链接：   http://localhost/multimedia/pd/test.mp4，网页开始播放视频。



完成！！！









