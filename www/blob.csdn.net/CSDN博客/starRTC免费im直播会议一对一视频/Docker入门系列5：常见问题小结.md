# Docker入门系列5：常见问题小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:56:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：335
## 重启容器
再次运行容器：
docker start container_id
然后
docker attach container_id
就可以继续下命令了。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%975%EF%BC%9A%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98%E5%B0%8F%E7%BB%93&action=edit&section=2)]命名
--name
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%975%EF%BC%9A%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98%E5%B0%8F%E7%BB%93&action=edit&section=3)]端口映射
docker run的时候可通过-p选项指定要映射的端口到host(hostPort:containerPort),如
docker run -i -t -p 8080:80 1f4fd79edf6d /bin/bash
docker run -d -p 80:80 -p 22:22 //分别映射80 和22 到host
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%975%EF%BC%9A%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98%E5%B0%8F%E7%BB%93&action=edit&section=4)]从docker中复制文件出来
docker cp ff35dcd5168e:/home/2/cuc_ieschool.mp3 .  //其中的.表示当前目录
