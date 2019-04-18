# vagrant入门指南(二): 创建vagrant项目 - happyhorizon的算法天空 - CSDN博客
2018年09月19日 20:11:18[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：139
创建vagrant项目的第一步应该是新建Vagrantfile文件. 在Vagrantfile中应该明确两个问题:
1 明确项目的root文件夹位置. vagrant的很多配置选项都是根据root文件夹的位置设置的.
2 描述项目需要的机器和资源, 包括要预先安装的软件以及如何访问.
## Vagrantfile文件
具体来说, 应该首先应该创建一个保存vagrant项目文件的文件夹, 并运行vagrant init进行初始化, 就会在该文件夹下自动生成一个Vagrantfile文件.
```
mkdir vagrant_getting_started
cd vagrant_getting_started
vagrant init
```
拿xnat-vagrant文件夹中的Vagrantfile为例:
```
# -*- mode: ruby -*-
# vi: set ft=ruby :
# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.
  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://vagrantcloud.com/search.
  config.vm.box = "base"
  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false
  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # NOTE: This will enable public access to the opened port
  # config.vm.network "forwarded_port", guest: 80, host: 8080
  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine and only allow access
  # via 127.0.0.1 to disable public access
  # config.vm.network "forwarded_port", guest: 80, host: 8080, host_ip: "127.0.0.1"
  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"
  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"
  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"
  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  # config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
  #   # Customize the amount of memory on the VM:
  #   vb.memory = "1024"
  # end
  #
  # View the documentation for the provider you are using for more
  # information on available options.
  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  # config.vm.provision "shell", inline: <<-SHELL
  #   apt-get update
  #   apt-get install -y apache2
  # SHELL
end
```
文件中虽然大多数都被comment掉了, 实际上内部只是定义了`config.vm.box = "base"`. 这里虚拟机的box可以理解为vagrant创建虚拟机的一些系统镜像. 这些comment也不是没有用,给出了很多Vagrantfile的设置例子.
## boxes
从零开始打造一个vagrant虚拟机是很冗长无聊的工作, vagrant支持用base镜像来快速clone部署虚拟机系统环境. Vagrantfile文件中, 首先就是要指定这个box. Box可以用`vagrant box add`命令添加到vagrant中.
```
vagrant box add hashicorp/precise64
```
这个命令会从 HashiCorp’s Vagrant Cloud box catalog([https://app.vagrantup.com/boxes/search](https://app.vagrantup.com/boxes/search))自动下载 “hashicorp/precise64”
添加了box之后,就可以在Vagrantfile中设置box的配置, 包括版本或者直接指定下载的链接, 例如:
```
Vagrant.configure("2") do |config|
  config.vm.box = "hashicorp/precise64"
  config.vm.box_version = "1.1.0"
  # config.vm.box_url = "http://files.vagrantup.com/precise64.box"
end
```
# vagrant虚拟机基本操作
## 启动, 登陆和离开操作
配置好vagrant虚拟开发环境后, 就可以启动和登录了.
启动: 在vagrant虚拟机文件所在的路径下输入`vagrant up`
”远程”登录: 在vagrant虚拟机启动后, `vagrant ssh` 模拟远程登录.
登陆后可以对虚拟机进行正常的操作. vagrant虚拟机与宿主机的共享文件是Vagrantfile同目录的vagrant文件夹, 在虚拟机的路径中, 该文件夹在根目录的/vagrant. 注意一定不要执行`rm -rf /`操作, 这样会删除所有的文件.
离开虚拟机: `logout`命令或者CTRL+d
## Provisioning
vagrant启动时候加载的是一个全新的基本linux环境, 例如ubuntu或者centOS. 如果我们希望虚拟机作为一个web服务器使用. 我们需要用`vagrant ssh`登陆后安装web服务器需要的软件和库.vagrant提供了一个自动进行初始配置的工具provisioning, 这样在执行`vagrant up`时候就直接安装配置好, provisioning的功能只要执行一次, 后续在启动虚拟机后环境都会自带这些安装过的软件. 以安装网络服务器apache为例.
我们在Vagrantfile所在的路径下创建一个新的脚本`touch bootstrap.sh`, 添加如下语句:
```
#!/usr/bin/env bash
apt-get update
apt-get install -y apache2
if ! [ -L /var/www ]; then
  rm -rf /var/www
  ln -fs /vagrant /var/www
fi
```
之后修改Vagrantfile, 在其中添加`config.vm.provision :shell, path: "bootstrap.sh"`告诉vagrant在setup的时候运行bootstrap.sh脚本, 配置apache服务器:
```
Vagrant.configure("2") do |config|
  config.vm.box = "hashicorp/precise64"
  config.vm.provision :shell, path: "bootstrap.sh"
end
```
之后运行`vagrant up`, vagrant会自动执行provision, 如果虚拟机正在运行, 可以通过`vagrant reload --provision`重启vagrant并执行provision更新配置. 为了验证provisioning的效果, 运行:
`vagrant ssh`, 远程登录后输入`wget -qO- 127.0.0.1`, 这条命令将设置Apache默认的 DocumentRoot指向/vagrant文件夹, 也就是Vagrant默认的与宿主机进行文件交互的文件夹.
