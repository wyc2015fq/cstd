# nginx+jwplayer配置flv/MP4点播系统, 视频拖动支持 - DoubleLi - 博客园






[http://www.cnblogs.com/lingdhox/p/4523885.html?utm_source=tuicool&utm_medium=referral](http://www.cnblogs.com/lingdhox/p/4523885.html?utm_source=tuicool&utm_medium=referral)






# 一 配置nginx

1. 下载 nginx 最新版 http://nginx.org/

2. 安装依赖库, 以ubuntu为例



```
`apt-``get``install libpcre3 libpcre3-dev libssl-dev openssl`
```



3. 编译nginx, 增加flv和MP4的支持



```
`/configure --with-http_flv_module --with-http_mp4_module  --with-http_ssl_module --with-debug`
```



   编译时可以指定安装目录 --prefix=/path/to/install

   然后make install

4. 测试是否支持seek(拖动, 快进)

    1) flv需要metadata数据才可以seek. 可以通过ffmpeg来查看flv视频是否有metadata数据.　



```
`> ffmpeg -i test.flv`



`Input #0, flv, ``from``'test.flv'``:`

`  ``Metadata:`

`    ``metadatacreator : Yet Another Metadata Injector ``for``FLV - Version 1.9`

`    ``hasKeyframes    : ``true`

`    ``hasVideo        : ``true`

`    ``hasAudio        : ``true`

`    ``hasMetadata     : ``true`

`    ``canSeekToEnd    : ``false`

`    ``datasize        : 23736525`

`    ``videosize       : 21784752`

`    ``audiosize       : 1923169`

`    ``lasttimestamp   : 157`

`    ``lastkeyframetimestamp: 153`

`    ``lastkeyframelocation: 23532906`
```



　　如果flv没有metadata数据. 可以通过yamdi来增加metadata数据, 这个工具windows和linux版本都有, 使用起来也很简单     



```
`yamdi -i sample.flv -o sample_with_metadata.flv `
```





     2) 把flv视频放在nginx 的html目录下. 然后通过curl或vlc测试拖动

     先修改下nginx的配置nginx.conf, 增加如下内容



```
`location ~ \.flv$ {`

`    ``flv;`

`    ``limit_rate  250k;`

`}`

`location ~ \.mp4$ {`

`    ``mp4;`

`    ``limit_rate  250k;`

`}`
```



　　limit_rate是限速的意思, 因为是本地测试. 缓冲速度非常快. 如果不限速的话, 视频会立刻下载完成. 





```
`curl -I http:``//ip/test.flv`

`Content-Length: 23739622`



`curl -I http:``//ip/test.flv?start=2373`

`Content-Length: 23727290`
```



　　两次的Content-Length不一样. 

　　也可以通过vlc来播放http流. 都是界面点一点. 

# 二 配置jwplayer

     网上的jwplayer的配置基本上是过时的. 误导我半天

　　早期jwplayer里配置的streamer; type; provider; 这些参数在新版里已经没有, 或者不生效了. 

     jwplayer的support中关于seek的文章 http://support.jwplayer.com/customer/portal/articles/1430518-pseudo-streaming-in-flash 可能要FQ才能看



```
`<div id=``"myElement"``>Loading the player...</div>`



`<script type=``"text/javascript"``>`

`    ``jwplayer(``"myElement"``).setup({`

`        ``file:``"[http://ip/test.flv](http://ip/test.flv)"``,`

`        ``image: ``"../image/webrtc.png"``,`

`        ``startparam: ``"start"`

`    ``});`

`</script>`
```



　　关键是 startparam: "start". 没有这句的话. jwplayer是无法拖动视频的. MP4可以不需要这个startparam就可以拖动. 

![](https://images0.cnblogs.com/blog2015/521870/201505/231118023587585.png)



    这个是jwplayer文档中关于startparam的参数说明. 










