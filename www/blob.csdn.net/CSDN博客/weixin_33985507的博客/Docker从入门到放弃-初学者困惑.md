# Docker从入门到放弃-初学者困惑 - weixin_33985507的博客 - CSDN博客
2018年10月26日 11:49:47[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 前言
这篇文章采用自问自答的方式记录自己的接触Docker的所想与困惑.
## 学习方式
菜鸟教程过一遍 -> 实战总结 -> Docker官网查找细节
## 基础知识
- Docker搜索镜像 docker search
- Docker查看本地镜像 docker images
- 运行镜像 docker run 镜像ID
- 停止镜像 docker stop 镜像ID
- 重启镜像 docker start 镜像ID
- 删除容器 docker rm name
- 删除镜像：需要注意删除镜像和容器的命令不一样。 docker rmi ID  ,其中 容器(rm)  和 镜像(rmi)
- 拖取镜像：docker pull name
- 提交容器：容器中做了一些安装和更改，可以使用docker commit 进行提交得到一个新的镜像，以后就不需要安装之前安装过的东西
- 主机拷贝文件到容器，容器拷贝到宿主主机分别是： docker cp [options] 容器ID:源路径 目的路径和docker cp [options] 源路径 容器ID: 目的路径。也可以用挂载 在docker run的时候用 -v命令指定，但是挂载需要和宿主主机关联，有点耦合的感觉
- docker save 保存一个或者多个镜像为压缩文件，使用docker load加载
- docker logs 可以查看一些密码的生存，比如Jenkins
- docker ps 和docker ps -a  前者查看运行的容器，后者查看包括不成功运行的所有容器
- docker 和 docker common --help 帮助获取命令更多细节
## 问题
- Docker容器和宿主主机的概念好迷糊
> 宿主主机就是当前安装了docker的系统，可以是window也可以是liunx. 容器就是镜像运行的一个环境，使用docker
run可以创建这个容器（环境），docker exec -it 容器ID（容器别名 --name）可以进入到这个容器中。进入发现运行镜像的容器有可能也是一个liunx系统。在这个容器中我们可以安装其他工具，比如vim，比如jdk,比如tomcat等，这样安装好以后直接使用docker -commit 变成一个新的镜像，以后直接通过docker run 新镜像就能获得之前安装好的vim jdk等这个新环境。每次使用docker run 都会创建一个容器，如果出现已经存在的提示，请使用docker stop 、docker rm去移除这个容器。
- Docker容器如何进入和运行
> Docker容器运行其实只要通过 docker run 就可以跑起响应的镜像，并能使用这个镜像，比如jenkins镜像，先通过docker search jenkins 找到仓库列表，然后docker pull拉去官网镜像， 直接docker run
镜像ID就可以启动Jenkins并且通过浏览器访问到这个Jenkins。使用docker run --privileged=true可以获取权限
Docker容器如何查看？ 准确说如何进入到Docker容器。 可以使用docker exec 这个命令，在liunx下可能有很多例子(docker exec -it -u root 容器别名 /bin/bash)，但是如果是window，那么分为两种模式，一种是Window容器（docker exec -it -u root 容器别名 powershell），一种是liunx 容器(docker exec -it -u root 容器别名 bash),
- Docker容器运行时的资源挂载
> liunx的挂载是将磁盘分区挂载指定目录，这个目录代表就是这个分区。那么这里资源挂载又是怎么回事呢？在使用docker run命令的时候可以使用-v来指定将宿主主机目录挂载在容器的目录下，这样做的好处是我不需要在容器中安装新的东西，只要将宿主主机已经存在的内容在docker run的时候用-v挂载一下就可以了，在容器下面会创建这个挂载的目录。比如说是宿主主机的jdk路径用-v /usr/jdk:/usr/container/jdk 挂载后在容器下面就会存在jdk路径，那么这个容器的jdk路径在什么时候使用呢？ 比如这个容器是Jenkins所在的容器，那么通过Web页配置Jenkins所需的jdk路径指的就是这个容器内的路径，我们不需要在容器里面在安装jdk了
具体挂载写法语法请看这篇文章 [这篇文章很全](https://blog.csdn.net/magerguo/article/details/72514813) ，容器指定的挂载路径会自动生成
- 容器运行的时候如何挂载新路径
> 在使用docker安装jenkins的时候，将容器运行起来然后在web页面进行大半天的插件下载，进入配置页的时候发现忘记挂载jdk了，这可怎么办？ 简单来说可以在容器下自己安装一个jdk，不一样要挂载。 但是如果非得挂载那么只能docker 删掉当前容器在run一次，这样做的结果是你之前在网页配置的jenkins已经不存在了需要从头再来
- 挂载后需要更改宿主主机中的tomcat版本，从7.9变成8.5，会直接影响容器吗
> 这个问题求助过别人，也查过资料，貌似只要更改宿主主机就可以了，具体还得等日后实践
- 容器权限不够
> 进入容器后通过 ls -l 发现 用户和用户组是root 只有用户才有wrx权限，用户组和其他人没有任何操作权限，我们知道x权限关系到你能不能cd到这个目录，连x都没有该如何是好？ 通过 w 和 who命令 或者命令提示符后面的$符可以看到当前用户不是root，可以在docker exec的时候指定 -u root参数来让进入的用户是个root用户提示符#是root
- /bin/bash的作用
> docker命令是不是就跟个/bin/bash，什么意思？ 这是表示载入容器后运行bash ,docker中必须要保持一个进程的运行，要不然整个容器就会退出。这个就表示启动容器后启动bash保证有个进行在运行。看懂了吗，反正在下很迷糊
- 容器没有vim ？ 不同使用yum安装？
> [请点击这里获取方法](https://www.cnblogs.com/river2005/p/8503238.html)
- window 安装docker
> 最悲催的是1G内容的云服务里玩不了docker，因此大多数还是在window下安装docker。安装需要注意几点内容，必须是window10企业版或者专业版，如果是win10家庭版的还是去某宝花几块钱买个序列号升级一下（去【开始——设置——更新——激活】点击更改产品密钥，然后把密钥复制粘贴进去）如何查看当前电脑操作系统版本：[点击这里](https://jingyan.baidu.com/article/f006222831bec9fbd2f0c861.html)，下图是官网使用Hyper-v虚拟机安装，
![clipboard.png](https://image-static.segmentfault.com/459/691/459691258-5bd31e6911a8a_articlex)
一路安装下去后出现了下面的错误Hardware assisted virtualization and data execution protection must be enabled in the BIOS. See [https://docs.docker.com/docke...](https://docs.docker.com/docker-for-windows/troubleshoot/#virtualization-must-be-enabled)。 虚拟化禁用了，进入bios，直接找到configuration（或advanced->CPU configuration）开启Intel Virtualization Technology, 解决好了没遇到其他问题，默认仓库太慢，修改daemon.json里面的内容（everything下个快速搜索）
- 容器中生成的jar包拷贝到主机window目录中
> docker cp myjenkins:/var/jenkins_home/workspace/xxx/target/xxx-0.0.1-SNAPSHOT.jar D:\
docker cp --help 显示的第一条命令是从容器拷贝主机，第二条命令是主机拷贝容器
- docker run Jenkins容器的时候不指定jenkins_home的挂载路径会如何？
> jenkins容器会自动生成/var/jenkins_home路径，这个路径存放第三方工具构建好的一些包war/jar，以后如有需要可以进入这个目录下获取包
proceed  &)_(&
