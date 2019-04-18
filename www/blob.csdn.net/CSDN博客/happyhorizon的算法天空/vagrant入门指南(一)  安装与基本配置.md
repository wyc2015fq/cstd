# vagrant入门指南(一) : 安装与基本配置 - happyhorizon的算法天空 - CSDN博客
2018年09月17日 18:15:55[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：176
# vagrant介绍
vagrant是用来构建虚拟开发环境的工作,非常适合php/python/ruby/java开发web应用. 在生产环境中经常利用vagrant封装一个Linux开发环境,分发给团队成员, 每个成员可以在自己的桌面系统上开发,代码却可以统一在vagrant封装好的环境中运行.
## vagrant工作的基本原理
1)读取配置文件,得到用户设定的环境配置: 操作系统\网络\基础软件等;
2)调用虚拟化管理软件的API(VMWare, VirtualBox, AWS等)为用户创建基础环境;
3)调用用户定义的安装脚本(shell, puppet, chef)安装好相应的服务和软件包.
# Vagrant安装
vagrant需要运行在虚拟机上, 例如virtualbox或者VMWare. 这里选择免费的virtualbox.
## 1 安装Virtualbox.
到oracle的https://www.virtualbox.org/网站下载ubuntu平台的virtualbox软件，考虑到可能出现的兼容问题, 一般不建议下载最新版, 比较新的版本就可以了. 我下载的是virtualbox-5.1_5.1.38的deb包. deb包的安装很简单, 直接双击就可以.
## 2 安装Vagrant.
从官网下载,得到一个封装好的可执行程序. 直接解压就可以得到Vagrant的可执行程序. 将解压后的地址添加到系统环境变量.也就是在~/.bashrc中添加:
```
export PATH="/home/vagrant:$PATH"
```
## 3 配置环境
获取官网封装好的系统镜像, 地址: [http://www.vagrantbox.es/](http://www.vagrantbox.es/)
首先,用`vagrant box add {title} {url}`通过指定的链接下载系统镜像. 这里也可以在url栏中复制地址,通过迅雷下载. 下载完成后在终端里输入, 以centos7.0为例,
```
vagrant box add centos7 H:/vagrantbox/centos-7.0-x86_64.box
```
centos7是我们给这个 box 命的名字， H:/vagrantbox/centos-7.0-x86_64.box 是下载的 box 所在路径.
之后用`vagrant init {title}`初始化系统镜像. 初始化成功后,会生成一个Vagrantfile的配置文件, 修改配置文件可以进行个性化定制.
接着就可以启动这个镜像了`vagrant up`
## 4 初始化开发环境
### 4.1) 修改虚拟机的端口映射方式:
Vagrant 默认是使用端口映射方式将虚拟机的端口映射本地从而实现类似 [http://localhost:80](http://localhost:80) 这种访问方式，这种方式比较麻烦，新开和修改端口的时候都得编辑。相比较而言，host-only 模式显得方便多了。打开 Vagrantfile，将下面这行的注释去掉（移除 #）并保存：
```
config.vm.network :private_network, ip: "192.168.27.101"
```
重启虚拟机，这样我们就能用 192.168.27.101 访问这台机器了，你可以把 IP 改成其他地址，只要不产生冲突就行。
### 4.2) 配置provision脚本
在安装完成后,可以对虚拟系统做一些设置,例如自动安装vim和git.可以编辑Vagrantfile文件,在文件末尾的end语句之前,  `config.vm.box_url = "http://files.vagrantup.com/precise64.box"`之后添加:
```
# 添加下面的这行
  config.vm.provision "shell", path: "provision.sh"
```
[这一行就指定了provision的脚本provision.sh](http://xn--provisionprovision-kz12au0f124i5yc930d41uyv1gfsvctuzbmg6b.sh). 在Vagrantfile同目录下创建该文件, 打开添加自动下载安装vim和git的语句:
```
sudo apt-get install vim git -y
```
### 5 启动实例
运行启动命令: `vagrant up` ,vagrant就会启动该目录下的Vagrantfile指定的虚拟机实例. 首先vagrant会去本地查找box文件, 之后启动虚拟机, 进行网络配置并将虚拟机目录挂在到/vagrant下, 使得虚拟机与物理机直接共享.
最后vagrant会开始provision过程,为虚拟机配置基础的软件.(只在第一次启动时候触发)
### 6 ssh登录虚拟机
用命令`vagrant ssh`可以虚拟远程登录虚拟机. 输入后得到:
```
vagrant ssh 
vagrant@{title}:~$    #看到这个已经是进来了虚拟机
```
这里{ title}是刚才设置的虚拟机名称.
# vagrant的常用命令:
vagrant init  # 初始化
vagrant up  # 启动虚拟机
vagrant halt  # 关闭虚拟机
vagrant reload  # 重启虚拟机
vagrant ssh  # SSH 至虚拟机
vagrant status  # 查看虚拟机运行状态
vagrant destroy  # 销毁当前虚拟机
# 参考
vagrant 添加本地 box 安装 laravel homestead: [https://zhuanlan.zhihu.com/p/25338468](https://zhuanlan.zhihu.com/p/25338468)
ubuntu下用vagrant创建虚拟开发环境: [https://www.linuxidc.com/Linux/2016-12/138113.htm](https://www.linuxidc.com/Linux/2016-12/138113.htm)
## 更多阅读
如何使用vagrant在虚拟机安装Hadoop集群 [http://www.linuxidc.com/Linux/2013-04/82750.htm](http://www.linuxidc.com/Linux/2013-04/82750.htm)
Vagrant 中高效的 Puppet 模块管理 [http://www.linuxidc.com/Linux/2014-05/101873.htm](http://www.linuxidc.com/Linux/2014-05/101873.htm)
利用Vagrant搭建多平台环境  [http://www.linuxidc.com/Linux/2015-09/123518.htm](http://www.linuxidc.com/Linux/2015-09/123518.htm)
使用 Vagrant 和 Fabric 用于集成测试 [http://www.linuxidc.com/Linux/2014-07/104113.htm](http://www.linuxidc.com/Linux/2014-07/104113.htm)
使用 Vagrant 搭建开发环境 [http://www.linuxidc.com/Linux/2014-07/104116.htm](http://www.linuxidc.com/Linux/2014-07/104116.htm)
Windows 下配置 Vagrant 环境 [http://www.linuxidc.com/Linux/2014-07/104115.htm](http://www.linuxidc.com/Linux/2014-07/104115.htm)
使用 Vagrant 搭建开发环境 [http://www.linuxidc.com/Linux/2014-07/104116.htm](http://www.linuxidc.com/Linux/2014-07/104116.htm)
通过 Vagrant 搭建虚拟机环境 [http://www.linuxidc.com/Linux/2015-08/121985.htm](http://www.linuxidc.com/Linux/2015-08/121985.htm)
使用Vagrant和Virtualbox创建虚拟机  [http://www.linuxidc.com/Linux/2016-08/134435.htm](http://www.linuxidc.com/Linux/2016-08/134435.htm)
