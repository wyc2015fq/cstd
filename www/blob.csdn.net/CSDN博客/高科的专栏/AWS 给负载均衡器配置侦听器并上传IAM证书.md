
# AWS 给负载均衡器配置侦听器并上传IAM证书 - 高科的专栏 - CSDN博客

2018年06月08日 15:25:22[高科](https://me.csdn.net/pbymw8iwm)阅读数：259


﻿﻿
1.打开EC2的负载均衡器：

添加侦听器，选择https，ssl证书需要有一个CA证书，可以去阿里云申请，也可以从亚马逊获取：

注意，这里的ssl证书要这样写

其中私有密钥不是那种xxxxxx.pem 而是xxxx.key， 公有密钥的应该是public.pem，证书链的是chain.pem，最开始一直以为私钥应该是xxxx。pem最后尝试了很多次才知道，不确定这个是不是AWS的bug







