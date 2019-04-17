# html5播放mp4视频代码 - DoubleLi - 博客园






1.nginx支持flv和mp4格式播放

默认yum安装nginx

centos7安装nginx时候应该是默认安装nginx_mod_h264_streaming模块的

# nginx -V查看是否安装nginx_mod_h264_streaming模块

nginx在新版本中已经支持了--with-http_mp4_module --with-http_flv_module这2个模块即可

# vi /etc/nginx/nginx.conf

server {

        listen       80 default_server;

        listen       [::]:80 default_server;

        server_name  _;

        root         /usr/share/nginx/html;

        #guowang add

        limit_rate_after 5m;   #在flv视频文件下载了5M以后开始限速

        limit_rate 512k;      #速度限制为512K



        # Load configuration files for the default server block.

        include /etc/nginx/default.d/*.conf;



        location / {

        }



        #guowang add

        location ~ \.flv {

            flv;

        }

        location ~ \.mp4$ {

            mp4;

        }



        error_page 404 /404.html;

            location = /40x.html {

        }



        error_page 500 502 503 504 /50x.html;

            location = /50x.html {

        }

}



2.html5播放mp4视频代码

<!DOCTYPE html>

<html>

<head>

    <meta charset="utf-8">

    <title>公司宣传片</title>

</head>

<body>

    <h2>公司宣传片</h2>

    <video width="320" height="240" controls autoplay>

        <source src="gs.mp4" type="video/mp4">

        <source src="gs.ogg" type="video/ogg">

        <source src="gs.webm" type="video/webm">

        <object data="gs.mp4" width="320" height="240">

          <embed src="gs.swf" width="320" height="240"><p>如果你播放不了该视频，

那是你的设备不支持该文件格式</p>

        </object>

    </video>

</body>

</html>

3.参考：

[http://www.runoob.com/html/html-videos.html](http://www.runoob.com/html/html-videos.html)

[http://www.runoob.com/try/try.php?filename=tryhtml_video_html5_4](http://www.runoob.com/try/try.php?filename=tryhtml_video_html5_4)









