# curl  28:Connection timed out after 1000milliseconds - weixin_33985507的博客 - CSDN博客
2016年09月08日 23:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：208
![1935245-ce5d1998fe9c5670.png](https://upload-images.jianshu.io/upload_images/1935245-ce5d1998fe9c5670.png)
ThinkPhp 5 如题出现这个错误，请先确认代码设置是否有问题,如果代码没问题，就是php环境和 apache服务器问题。
***一  、***
***首先打开php的配置文件，配置文件在php根目录下的**php.ini**，没有这个文件的自行百度。***
打开curl拓展（将图片所示的一行前面的分号删掉）
![1935245-47ae0307e1c47dd1.png](https://upload-images.jianshu.io/upload_images/1935245-47ae0307e1c47dd1.png)
***二 、然后检查是否配置了**extension_dir*****
![1935245-794ecdd97d817c59.png](https://upload-images.jianshu.io/upload_images/1935245-794ecdd97d817c59.png)
***三 、***
OK，这个时候重启Apache发现提示“系统错误 1067”，网上有人说说32位系统配置curl没有问题，笔者也么做测试，接着往下进行。
打开Apache的配置文件，配置文件在Apache根目录下的**conf/httpd.conf**，在文档最后添加如下的四行代码，自行将图中所示的目录填写成你的PHP目录的绝对路径。
LoadFile  绝对路径+php7ts.dll (去php安装目录下看安装的版本是几就会是版本几)                   LoadFile  D:/website/php/php7ts.dll 
LoadFile D:/website/php/libeay32.dll
LoadFile D:/website/php/ssleay32.dll
LoadFile D:/website/php/libssh2.dll
五、 重启apache  ok
