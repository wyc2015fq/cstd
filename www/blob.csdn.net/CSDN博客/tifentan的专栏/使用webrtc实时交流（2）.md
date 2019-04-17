# 使用webrtc实时交流（2） - tifentan的专栏 - CSDN博客





2017年09月06日 14:32:23[露蛇](https://me.csdn.net/tifentan)阅读数：180








本系列文章翻译自webrtc官网链接。 
[https://www.html5rocks.com/en/tutorials/webrtc/infrastructure/](https://www.html5rocks.com/en/tutorials/webrtc/infrastructure/)

本人翻译水平有限，本着翻译过程就是学习过程的目的进行翻译。若有不当之处，欢迎指出。
# 概述

构建一个app实现摄像头视频获取，截图获取，并通过webrtc/p2p的方式分享出去。在实现途中，你会学习到webrtc APIS的使用，并通过Node建立一个信令服务器。

## 你会学到什么
- 从摄像头获取视频
- 利用RTCPeerConnection传输视频
- 利用RTCDataChannel传输数据
- 建立一个信令服务来交换信息
- 结合peer connection 和信令（signaling）
- 获取一个相片并通过data channel共享

## 你需要什么
- 47版本后的chrome
- [chrome http服务器](https://chrome.google.com/webstore/detail/web-server-for-chrome/ofhbbkphhbklhfoeikjpcbhemlocgigb)或者你自己的web服务器
- 样例代码




