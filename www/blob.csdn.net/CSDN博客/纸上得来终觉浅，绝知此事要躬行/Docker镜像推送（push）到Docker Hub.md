# Docker镜像推送（push）到Docker Hub - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月09日 23:26:06[boonya](https://me.csdn.net/boonya)阅读数：32853








镜像构建成功后，只要有docker环境就可以使用，但必须将镜像推送到Docker Hub上去。我们之前创建的镜像不符合Docker Hub的tag要求，因为 在Docker Hub注册的用户名是boonyadocker而不是boonya，所以需要docker tag进行修改，最后利用docker push命令推送镜像到公共仓库。

## Docker hub注册用户

到官网注册账号：[https://hub.docker.com/](https://hub.docker.com/)

在本地Linux登录docker：


` docker login`
输入用户名密码进行登录：




```
docker@default:~$ docker login
Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
Username: boonyadocker
Password: 
Login Succeeded
docker@default:~$
```




## tag修改镜像名称

推送镜像的规范是：
`docker push 注册用户名/镜像名`



tag命令修改为规范的镜像：



`docker tag boonya/tomcat-allow-remote boonyadocker/tomcat-allow-remote`
查看修改后的规范镜像：




```
docker@default:~$ docker images
REPOSITORY                         TAG                 IMAGE ID            CREATED             SIZE
boonyadocker/tomcat-allow-remote   latest              6137f64875dd        7 hours ago         571MB
boonyadocker/tomcat-web            latest              09677d05b579        2 days ago          571MB
<none>                             <none>              e1471ab1845a        3 days ago          188MB
<none>                             <none>              cf240e31edfb        3 days ago          188MB
<none>                             <none>              717961796ae2        3 days ago          334MB
apptomcat                          1.0                 249dc3f5dfb0        3 days ago          334MB
tomcat                             8                   a2fbbcebd67e        8 days ago          334MB
ubuntu                             14.04               4a2820e686c4        2 weeks ago         188MB
wurstmeister/kafka                 latest              12453f4efa7b        6 weeks ago         265MB
docker@default:~$
```




## 推送镜像到Docker Hub

通过push命令推送镜像：




```
docker push boonyadocker/tomcat-allow-remote:latest
```
![](https://img-blog.csdn.net/20170709232347308)

注：推送Docker Hub速度很慢，耐心等待，很有可能失败，失败会尝试多次重传，之后断开推送（但已推送上去的会保留，保留时间不知道是多久）。

下面是上传完毕的输出（多次重传）：




```
docker@default:~$ docker push boonyadocker/tomcat-allow-remote:latest
The push refers to a repository [docker.io/boonyadocker/tomcat-allow-remote]
464a44ea0195: Layer already exists 
29b57e33a4da: Pushed 
d649a240e453: Layer already exists 
d0757a6730d0: Layer already exists 
768dcfe5d05f: Layer already exists 
f5cfc06b640d: Layer already exists 
9669d6b73383: Layer already exists 
latest: digest: sha256:1e7562a15ef1728f213922d9633be67f3025447d0a641e333a8ec5107749c386 size: 11802
docker@default:~$
```

还是需要耐心，因为大文件比较耗时。



## 访问Docker Hub发布镜像

上传完成后访问：[https://hub.docker.com/r/boonyadocker/tomcat-allow-remote/](https://hub.docker.com/r/boonyadocker/tomcat-allow-remote/)，如下图所示，大家都可以使用我发布的镜像了：

![](https://img-blog.csdn.net/20170710084410293)


在Docker Hub上进行搜索，也是可以搜出来的：

![](https://img-blog.csdn.net/20170710084816002)


至此我们已达到将自己的镜像发布到Docker Hub仓库。

## Docker 使用发布的镜像

搜索已发布的镜像：



```
docker@default:~$ docker search boonyadocker/tomcat-allow-remote
NAME                               DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
boonyadocker/tomcat-allow-remote   In this Tomcat image server you can use ma...   0                    
docker@default:~$
```

因为我们已经发布自己的镜像，以后都可以直接使用docker pull命令拉取使用镜像：

`docker pull boonyadocker/tomcat-allow-remote`
![](https://img-blog.csdn.net/20170710085354978)



注：镜像是基于Tomcat8的远程管理角色权限，管理账号和密码是：tomcat/password。









