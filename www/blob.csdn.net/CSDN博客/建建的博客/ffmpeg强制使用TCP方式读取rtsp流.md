# ffmpeg强制使用TCP方式读取rtsp流 - 建建的博客 - CSDN博客
2017年03月11日 22:27:52[纪建](https://me.csdn.net/u013898698)阅读数：1732
ffmpeg强制使用TCP方式处理rtsp流，参考网上资料，得知可以使用如下命令：
“ffmpeg -rtsp_transport tcp -i rtsp://admin.......”
可以是使用抓包工具进行验证一下是否强制使用了TCP方式。
参考：
[https://freshventure.wordpress.com/?s=ffmpeg](https://freshventure.wordpress.com/?s=ffmpeg)
-----------------------------------------------------------
[https://freshventure.wordpress.com/2011/12/21/ffmpegffserver%e5%b0%86ip-camera%e7%9a%84rtsp%e6%b5%81%e8%bd%ac%e6%8d%a2%e4%b8%baflv%e6%b5%81/](https://freshventure.wordpress.com/2011/12/21/ffmpegffserver%E5%B0%86ip-camera%E7%9A%84rtsp%E6%B5%81%E8%BD%AC%E6%8D%A2%E4%B8%BAflv%E6%B5%81/)
-----------------------------------------------------------
[https://freshventure.wordpress.com/2011/12/20/ffmpeg%e5%bc%ba%e5%88%b6%e4%bd%bf%e7%94%a8tcp%e6%96%b9%e5%bc%8f%e8%af%bb%e5%8f%96rtsp%e8%a7%86%e9%a2%91/](https://freshventure.wordpress.com/2011/12/20/ffmpeg%E5%BC%BA%E5%88%B6%E4%BD%BF%E7%94%A8tcp%E6%96%B9%E5%BC%8F%E8%AF%BB%E5%8F%96rtsp%E8%A7%86%E9%A2%91/)
