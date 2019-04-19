# nginx-rtmp开启ssi功能页面无法加载css的解决方法 - xcyl的口袋 - CSDN博客
2016年10月25日 00:28:54[xcyl](https://me.csdn.net/cai6811376)阅读数：560
EasyDarwin的rtmp采用nginx-rtmp，项目地址在[https://github.com/EasyDarwin/nginx-rtmp](https://github.com/EasyDarwin/nginx-rtmp)
当我们使用nginx-rtmp时并希望同时使用http功能，这里我们使用nginx的配置文件，开启ssi使nginx支持静态网页时，我们遇到页面无法正常加载css的问题。
寻找原因我们发现配置文件中缺少
```
#文件扩展名与文件类型映射表
include       mime.types;
#默认文件类型
default_type  application/octet-stream;
```
```
......
http {
    include       mime.types;
    default_type  application/octet-stream;
    server {
        listen      80;
        location / {
            ssi on;
            ssi_silent_errors on;
            root www;
            index index.html index.htm;
        }
        ......
    }
    ......
}
......
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
