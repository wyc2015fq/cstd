# TS+M3U8+directshow流媒体播放器 简介 - 建建的博客 - CSDN博客
2017年10月16日 20:09:59[纪建](https://me.csdn.net/u013898698)阅读数：222
  工作两个多月一直围绕着网络播放。现在基本解决了主要问题，小结一下。其中可能有很多的问题，希望各位客官见谅。
      我使用的上海一家公司的流媒体服务器。所以我涉及到的开发内容，M3U8解析工作和TS流播放器，其他JSON解析主要是针对影片内容展示上使用的。
      播放器开发描述：通过解析M3U8文件获取ts流片段地址，使用libcurl进行数据下载，利用文件映射或环形buffer进行数据缓存，通过directshow实现播放。sourcefilter使用push和pull两种模式实现。
       第一篇    m3u8解析
       第二篇    libcurl基本使用
       第三篇    环形buffer
       第四篇    实现pull模式sourcefilter
       第五篇    创建图（pull）
       第六篇    实现控制（暂停、播放、快进、快退）
       第七篇    实现push模式sourcefilter
       第八篇    连接MPEG-2 Demultiplexer
       第九篇    创建图（push）
     网上的相关资料太少了，间断总结一二望能对各位有所帮助。
