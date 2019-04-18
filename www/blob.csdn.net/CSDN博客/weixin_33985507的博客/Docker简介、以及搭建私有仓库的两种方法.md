# Docker简介、以及搭建私有仓库的两种方法 - weixin_33985507的博客 - CSDN博客
2017年02月21日 16:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
Docker是一个可为任何应用创建轻量级的、可移植的、自给自足的容器。开发者本地编译测试通过的容器即可批量的在生产环境中部署。
Docker能处理的事情：
1）隔离应用依赖
2）创建应用镜像并进行复制
3）创建容易分发的即启即用的应用
4）允许实例简单、快速地扩展
5）测试应用并随后销毁它们
Docker的目的是创建**软件程序可移植的轻量容器**，让其可以在任何安装了Docker的机器上运行。
**镜像**：类似于虚拟机的快照。
镜像是容器运行时的只读模板，每个镜像由一系列的层（layers）组成，如果修改一个Docker镜像，比如说升级某个程序，一个新的层就会被创建。
创建一个镜像之前，可以先拿一个镜像，对它进行修改来创建它的子镜像。方式有两种：
1）在一个文件中指定一个基础镜像及需要完成的修改；
2）通过“运行”一个镜像，对其进行修改并且提交。
**仓库**：用于保存镜像，可以理解为代码控制中的控制仓库。
**容器**：从镜像中创建，等同于从快照中创建虚拟机，应用都是由容器运行的。可以将容器看做一个简易版的linux环境（包括root用户权限、进程空间、用户空间和网络空间等）和运行在其中的应用环境。
**搭建自己的私人docker registry**（两个功能：镜像管理、认证）：镜像管理主要由docker-registry来实现，通过http服务上传下载；认证可以由现成的方案（如nginx）实现http请求管理；
![4574557-868b6589181374b6.png](https://upload-images.jianshu.io/upload_images/4574557-868b6589181374b6.png)
docker-registry-deploy.png
1）安装docker-registry
```
docker run -d -p 5000:5000 -v /opt/registry:/tmp/registry registry
```
指定本地目录/opt/registry为容器存放地址，默认为/tmp/registry
访问[http://ip:5000/v2](https://link.jianshu.com?t=http://ip:5000/v2)得到返回“{}”即为正确安装
2）将本地镜像pull到私有仓库中
```
//修改镜像tag
docker tag name ip:5000/name
//上传打了tag的镜像到私服
docker push ip:5000/name
```
访问[http://ip:5000/v2/_catalog](https://link.jianshu.com?t=http://ip:5000/v2/_catalog)会返回{"repositories":["name"]}即为上传成功
3）下载镜像
```
docker pull ip:5000/name
```
由于新版的docker-registry需要https协议，所以有两种方法建立私有库
1）修改docker默认配置（debian中位置在/etc/default/docker）
添加
Docker是一个可为任何应用创建轻量级的、可移植的、自给自足的容器。开发者本地编译测试通过的容器即可批量的在生产环境中部署。
Docker能处理的事情：
1）隔离应用依赖
2）创建应用镜像并进行复制
3）创建容易分发的即启即用的应用
4）允许实例简单、快速地扩展
5）测试应用并随后销毁它们
Docker的目的是创建**软件程序可移植的轻量容器**，让其可以在任何安装了Docker的机器上运行。
**镜像**：类似于虚拟机的快照。
镜像是容器运行时的只读模板，每个镜像由一系列的层（layers）组成，如果修改一个Docker镜像，比如说升级某个程序，一个新的层就会被创建。
创建一个镜像之前，可以先拿一个镜像，对它进行修改来创建它的子镜像。方式有两种：
1）在一个文件中指定一个基础镜像及需要完成的修改；
2）通过“运行”一个镜像，对其进行修改并且提交。
**仓库**：用于保存镜像，可以理解为代码控制中的控制仓库。
**容器**：从镜像中创建，等同于从快照中创建虚拟机，应用都是由容器运行的。可以将容器看做一个简易版的linux环境（包括root用户权限、进程空间、用户空间和网络空间等）和运行在其中的应用环境。
**搭建自己的私人docker registry**（两个功能：镜像管理、认证）：镜像管理主要由docker-registry来实现，通过http服务上传下载；认证可以由现成的方案（如nginx）实现http请求管理；
![4574557-868b6589181374b6.png](https://upload-images.jianshu.io/upload_images/4574557-868b6589181374b6.png)
docker-registry-deploy.png
1）安装docker-registry
```
docker run -d -p 5000:5000 -v /opt/registry:/tmp/registry registry
```
指定本地目录/opt/registry为容器存放地址，默认为/tmp/registry
访问[http://ip:5000/v2](https://link.jianshu.com?t=http://ip:5000/v2)得到返回“{}”即为正确安装
2）将本地镜像pull到私有仓库中
```
//修改镜像tag
docker tag name ip:5000/name
//上传打了tag的镜像到私服
docker push ip:5000/name
```
访问[http://ip:5000/v2/_catalog](https://link.jianshu.com?t=http://ip:5000/v2/_catalog)会返回{"repositories":["name"]}即为上传成功
3）下载镜像
```
docker pull ip:5000/name
```
由于新版的docker-registry需要https协议，所以有两种方法建立私有库
1）修改docker默认配置（debian中位置在/etc/default/docker）
添加
![4574557-b1b40639b20dc50f.png](https://upload-images.jianshu.io/upload_images/4574557-b1b40639b20dc50f.png)
registry配置.png
如果需要添加多个则以空格隔开：
```
DOCKER_OPTS="--insecure-registry 121.201.116.112:5000 --insecure-registry 121.201.116.112:5000"
```
同时在各个服务器上也做一样的修改
2）使用https请求私有库
1、新建registry 2.X的容器
```
mkdir -p /opt/data/registry/conf
```
![4574557-ad97827397f5cfe8.png](https://upload-images.jianshu.io/upload_images/4574557-ad97827397f5cfe8.png)
conf内文件.png
.crt文件即为证书，.htpasswd为创建私有仓库用户和密码生成的文件
```
docker run -d --name docker-registry -v /opt/data/registry:/tmp/registry-dev registry:2.4.1
```
将该容器命名为docker-registry，目录为/opt/data/registry
2、生成自签名证书
```
openssl req -x509 -days 3650 -nodes -newkey rsa:2048 -keyout /opt/data/registry/conf/docker-registry.key -out /opt/data/registry/conf/docker-registry.crt
```
**注意**：生成签名是设置your name的时候需要跟域名一致
3、生成私有仓库的账号密码
先要安装apache2-utils，然后利用其附带的htpasswd生成（debian系统）
```
apt-get install apache2-utils
htpasswd -c /opt/data/registry/conf/docker-registry.htpasswd infinite
```
创建一个名为infinite的用户
4、创建一个nginx容器
```
docker run -d \
-p 443:443 \
--name docker-registry-proxy \
-e REGISTRY_HOST="docker-registry" \
-e REGISTRY_PORT="5000" \
-e SERVER_NAME="your domain name" \
--link docker-registry:docker-registry \
-v /opt/data/registry/conf/docker-registry.htpasswd:/etc/nginx/.htpasswd:ro \
-v /opt/data/registry/conf:/etc/nginx/ssl:ro \
containersol/docker-registry-proxy
```
SERVER_NAME填写的为hosts中配置的域名
5、将crt文件放入默认位置
```
mkdir -p /etc/docker/certs.d/your domain name/
cp /opt/data/registry/conf/docker-registry.crt /etc/docker/certs.d/your domain name/ca.crt
```
**注意**：自定义证书可能存在不被信任的问题，所以需要以下步骤
```
cp /opt/data/registry/conf/docker-registry.crt /usr/local/share/ca-certificates
update-ca-certificates
service docker restart
```
**注意**：docker重启后，容器是不会重启的，所以要将先前的两个容器手动重启
6、测试登录
```
docker login your domain name
```
7、客户端下载镜像
在服务器上传了镜像之后，客户端设置hosts，将证书复制过来（自签名正式也可能不被信任），登录之后即可上传下载。
