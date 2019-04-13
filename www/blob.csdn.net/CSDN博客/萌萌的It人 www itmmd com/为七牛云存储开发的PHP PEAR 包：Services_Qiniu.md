
# 为七牛云存储开发的PHP PEAR 包：Services_Qiniu - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:04:33[Jlins](https://me.csdn.net/dyllove98)阅读数：1543


七牛云存储Qiniu Resource (Cloud) Storage：用于存储图片、apk等静态资源。
优点：
1、使用七牛带宽和CDN，速度快，不占用开发者服务器。
2、支持图片任意格式、任意分辨率自动生成，可以用来做图片服务器。
3、小流量免费：存储空间 10GB，每月下载流量 10GB，每月 PUT/DELETE 10万次请求，每月 GET 100万次请求。当项目发展大时，需要更大流量才需购买。适合互联网创业者。
4、支持webp，画质不变，图片体积大幅度减小，特别适合作移动互联网app的图片服务器。
比如：
原图 103KB：[http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703](http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703)
自动生成jpg 79KB：[http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703?imageMogr/v2/auto-orient/thumbnail/1366x768/quality/80/format/jpg](http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703?imageMogr/v2/auto-orient/thumbnail/1366x768/quality/80/format/jpg)
自动生成webp 23KB：[http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703?imageMogr/v2/auto-orient/thumbnail/1366x768/quality/80/format/webp](http://com-163-sinkcup.qiniudn.com/6ffe255fab20747c8872638870268703?imageMogr/v2/auto-orient/thumbnail/1366x768/quality/80/format/webp)

七牛官方PHP SDK：
[http://docs.qiniu.com/php-sdk/v6/index.html](http://docs.qiniu.com/php-sdk/v6/index.html)
对于这种第三方库，PHP官方称之为PEAR，需要按照PEAR标准开发（[标准URI](http://pear.php.net/manual/en/standards.php)）。
PEAR的优势：一键安装到php/lib/php目录，require即可使用，很方便。
由于七牛官方PHP SDK没按照PEAR标准开发，使用不方便，所以我开发了一个，安装步骤如下：
pear channel-discover sinkcup.github.io/pear
pear install sinkcup/Services_Qiniu
使用步骤：
1、到七牛（[http://www.qiniu.com/](http://www.qiniu.com/)）注册一个帐号，创建一个bucket空间。
2、demo（参考[https://github.com/sinkcup/php-sdk/blob/pear/tests/Services/Qiniu/RSTest.php](https://github.com/sinkcup/php-sdk/blob/pear/tests/Services/Qiniu/RSTest.php)）：
<?php$conf=array('accessKey' => 'foo',
    'secretKey' => 'bar',);require'Services/Qiniu/RS.php';$c=newServices_Qiniu_RS('com-example-img-agc',$conf);$headers=array('Content-Type' => 'image/jpeg',);$r=$c->uploadFile('/home/u1/2.jpg', '/2.jpg',$headers);var_dump($r);?>
输出：
array(1) {
  ["uri"]=>string(40) "http://com-163-sinkcup.qiniudn.com/2.jpg"}
七牛官方PHP SDK源代码：
[https://github.com/qiniu/php-sdk](https://github.com/qiniu/php-sdk)
fork以后，我的Services_Qiniu源代码：
[https://github.com/sinkcup/php-sdk/tree/pear](https://github.com/sinkcup/php-sdk/tree/pear)
PEAR频道：
[http://sinkcup.github.io/pear/](http://sinkcup.github.io/pear/)


