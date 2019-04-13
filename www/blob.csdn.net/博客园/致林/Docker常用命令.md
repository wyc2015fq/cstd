
# Docker常用命令 - 致林 - 博客园






# [Docker常用命令](https://www.cnblogs.com/bincoding/p/7900534.html)
查看容器
\# 查看容器
dockerps-a
\# 启动容器
docker start 69f58f921497
\# 启动容器，后台运行
docker start x
\# 进入容器1
\# -t 分配一个伪终端
\# -i 即使没有附件也保持Stdin打开
\# -d 分离模式，后台运行
docker exec -i -t 69f58f921497 bash
\# 进入容器2，只能登陆一个bash
docker attach 69f58f921497
查看docker镜像
docker imagels
搜索docker镜像
docker search sentos
下载docker镜像
\# 从docker官网下载
docker pull centos:[buildid]
\# 从私人创库下载
docker pull registry.cn-shenzhen.aliyuncs.com/haibincoder/centos:[版本号]
启动docker镜像，新建docker容器
\# 带输入输出的方式新建容器
docker run-i -t centos bash
登陆阿里云docker
sudodockerlogin--username=11503004@qq.com registry.cn-shenzhen.aliyuncs.com
从registry中拉取镜像
docker pull registry.cn-shenzhen.aliyuncs.com/haibincoder/centos:[镜像版本号]
将镜像推送到registry
将镜像推送到registry：
$sudodockerlogin--username=11503004@qq.com registry.cn-shenzhen.aliyuncs.com
$sudodocker tag [ImageId] registry.cn-shenzhen.aliyuncs.com/haibincoder/centos:[镜像版本号]
$sudodocker push registry.cn-shenzhen.aliyuncs.com/haibincoder/centos:[镜像版本号]
sample:
使用docker tag重命名镜像，并将它通过私网ip推送至registry：
$sudodocker images
REPOSITORY                                                         TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
registry.aliyuncs.com/acs/agent0.7-dfb6816         37bb9c63c8b27days ago37.89MB
$sudodocker tag 37bb9c63c8b2 registry..aliyuncs.com/acs/agent:0.7-dfb6816
通过docker images 找到您的imageId 并对于改imageId重命名镜像domain到registry内网地址。
$sudodocker push registry..aliyuncs.com/acs/agent
查看docker信息
dockerinfodocker version
Docker从Container建立Iamge
[http://blog.csdn.net/wxqee/article/details/52081866](http://blog.csdn.net/wxqee/article/details/52081866)
Docker 提交一个运行中的程序为镜像
docker commit [container_id] [new_name]
运行镜像并添加端口
docker run -d -p8000:80[image_name] /bin/bash





