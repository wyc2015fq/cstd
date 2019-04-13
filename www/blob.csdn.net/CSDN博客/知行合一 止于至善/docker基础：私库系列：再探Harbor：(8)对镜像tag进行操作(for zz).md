
# docker基础：私库系列：再探Harbor：(8)对镜像tag进行操作(for zz) - 知行合一 止于至善 - CSDN博客

2018年12月09日 20:28:50[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：378所属专栏：[DevOps系列之：镜像私库](https://blog.csdn.net/column/details/26478.html)



harbor使用了docker的registry，docker的registry所提供的api使用起来并不方便，harbor提供了使用/repositories/{repo_name}/tags可以对镜像的tag进行重新设定，非常方便，当然也可以通过docker cli方式使用docker tag命令进行修改。但是由于retag的功能是harbor在1.7.0才开始引入的功能，而且目前harbor的1.7.0只是在rc1版本，更多使用之前稳定版本的项目稍微需要走一下曲线救国的路线。
# 前提准备
harbor环境安装，详细参看：
[https://blog.csdn.net/liumiaocn/article/details/81805063](https://blog.csdn.net/liumiaocn/article/details/81805063)
## 创建project
`[root@liumiaocn ~]# cat testproject1.json 
{
  "project_name": "testproject1",
  "metadata": {
    "public": "false"
  }
}
[root@liumiaocn ~]# curl -X POST "http://localhost:32031/api/projects" -H "accept: application/json" -H "Content-Type: application/json" -d@testproject1.json -uadmin:liumiao123
[root@liumiaocn ~]#`![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209200625503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 推送镜像
`[root@liumiaocn ~]# docker pull busybox
Using default tag: latest
latest: Pulling from library/busybox
90e01955edcd: Pull complete 
Digest: sha256:2a03a6059f21e150ae84b0973863609494aad70f0a80eaeb64bddd8d92465812
Status: Downloaded newer image for busybox:latest
[root@liumiaocn ~]# 
[root@liumiaocn ~]# docker tag busybox:latest 192.168.163.118:32031/testproject1/busybox:v1
[root@liumiaocn ~]#
[root@liumiaocn ~]# docker push 192.168.163.118:32031/testproject1/busybox:v1
The push refers to a repository [192.168.163.118:32031/testproject1/busybox]
8a788232037e: Pushed 
v1: digest: sha256:e2d9acbe92a6def141a9f9f2584468206735308df6a696430e25947882385fb2 size: 527
[root@liumiaocn ~]#`确认推送结果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209201305241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
确认tag
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209201338261.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
修改tag
比如此处需要将v1修改为v2，首先要做如下的事情
[root@liumiaocn ~]\# docker tag 192.168.163.118:32031/testproject1/busybox:v1 192.168.163.118:32031/testproject1/busybox:v2
[root@liumiaocn ~]\# docker push 192.168.163.118:32031/testproject1/busybox:v2
The push refers to a repository [192.168.163.118:32031/testproject1/busybox]
8a788232037e: Layer already exists 
v2: digest: sha256:e2d9acbe92a6def141a9f9f2584468206735308df6a696430e25947882385fb2 size: 527
[root@liumiaocn ~]\#结果确认
可以看到v1和v2的tag同时存在，如果这个结果已经能够满足要求，即之前的tag存在也不介意，registry和harbor可以使用同样的方式解决，至此就可以结束了，但是如果希望原有的v1的tag删除，在1.5.2的harbor的版本就会出现一个小问题。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209201540316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
一个小问题
Q：选中v1，然后点击 Delete按钮，结果会是什么？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209201916204.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
结果就是v1和v2都会删除
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209202005604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
作为一个普通用户来说，这个操作的结果需要给予解释才能够理解的，当然，使用restapi方式进行删除在这个版本也有同样问题。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209202222243.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
所以，最简单的暂定对应方式就是先删除在tag+push。当然时候harbor后续的版本自然是更好的选择之一，但是请注意在retag时，源和目的如果在不同的project，而且相关的权限又不同的时候，即使是当前的1.7.0的版本也正在改进，毕竟是rc1的版本，但是如果在项目中使用则需多加留意。
参考文章
[https://docs.docker.com/registry/spec/api/](https://docs.docker.com/registry/spec/api/)
[http://editor.swagger.io/](http://editor.swagger.io/)
[https://github.com/goharbor/harbor/blob/master/docs/swagger.yaml](https://github.com/goharbor/harbor/blob/master/docs/swagger.yaml)

[
  ](https://img-blog.csdnimg.cn/20181209202005604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)