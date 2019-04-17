# 百度语音识别API使用小结 - xmdxcsj的专栏 - CSDN博客





2014年08月08日 10:43:01[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2962








http://developer.baidu.com/wiki/index.php?title=docs/cplat/media/voice/sdk

1.      首先到百度开发者平台注册（http://developer.baidu.com/），创建新的应用，应用创建需要审核一段时间，然后可以得到APIKEY和SECRETKEY，

2.      申请开启语音识别服务。

3.      获得tokens，按照如下格式在浏览器输入，即可以获得返回的tokens值：

[https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=******&client_secret=******](https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=******&client_secret=******)

          tokens值是有有效期的，有效期为30天，30天以后重新输入上面的命令即可以获取更新以后的tokens。

4.      上传语音数据采用显示发送的格式，linux命令如下：

wget --tries=3 --timeout=20 -O "输出结果"--user-agent="chromium" --post-file=”输入语音”--header="Content-Type: audio/pcm; rate=16000"
[http://vop.baidu.com/server_api?lan=zh&cuid=123&token=************](http://vop.baidu.com/server_api?lan=zh&cuid=123&token=************)

5.      最后返回的结果为utf8编码的json格式结果，可以使用php脚本解析获得最后的识别结果：

<?php

       while($str= fread(STDIN,1000)){

                $result=json_decode($str);

                echo$result->{'result'}[0]."\n";

       }

?>

6.      可以使用后续的脚本统计准确率。



