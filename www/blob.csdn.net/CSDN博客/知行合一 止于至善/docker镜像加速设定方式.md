
# docker镜像加速设定方式 - 知行合一 止于至善 - CSDN博客

2019年02月18日 20:39:27[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：586标签：[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[镜像加速																](https://so.csdn.net/so/search/s.do?q=镜像加速&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=docker&t=blog)个人分类：[Docker																](https://blog.csdn.net/liumiaocn/article/category/6328276)



在docker中通过设定registry-mirror参数即可设定国内镜像加速。这里进行简单memo。
# 为什么要进加速
因为速度慢，还老容易断，连不上
# 可以使用什么进行加速
如下源可以考虑：
|类别|地址|
|---|---|
|Docker官方中国区设定|[https://registry.docker-cn.com](https://registry.docker-cn.com)|
|ustc|[https://docker.mirrors.ustc.edu.cn](https://docker.mirrors.ustc.edu.cn)|
|网易|[http://hub-mirror.c.163.com](http://hub-mirror.c.163.com)|
|阿里云|需要开发者账号：阿里云开发平台->Docker镜像仓库->加速器（开发者专属，一般格式为https://xxxxx.mirror.aliyuncs.com）|
# 如何设定
Mac
可以直接使用图形进行设定，选择Preferences-> Daemon，比如如下方式即可设定网易加速方式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218112011979.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
其他Linux
直接在dockerd的option中添加–registry-mirror=“[http://hub-mirror.c.163.com](http://hub-mirror.c.163.com)”，重启docker服务即可
当然也可以在如下设定文件中进行设定，重启docker即可
/etc/docker/daemon.json
内容示例
{
“registry-mirrors”: [“[http://hub-mirror.c.163.com](http://hub-mirror.c.163.com)”]
}

