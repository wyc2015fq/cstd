# windows宿主机ping不通Docker容器的解决办法 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月03日 14:45:24[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1350
![](https://note.youdao.com/src/C11B9FF4419847FE8625DFA80209E0CE)
网卡上有
![](https://note.youdao.com/src/630A605EFE134BB2B991F497DA6AC09E)
![](https://note.youdao.com/src/1B5567AC8B344322B68013282AD4B2FC)
docker is not a virtual machine, and you don't get access to the docker host via IP address.
如果想要访问里面的服务，需要先-p暴露端口，然后通过localhost:端口进行访问
ping不通也是可以访问的！
不需要添加什么路由！
