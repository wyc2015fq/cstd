# vagrant 介绍,安装与使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月29日 15:26:23[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：308
可以帮你统一团队成员的开发环境。如果你或者你的伙伴创建了一个[Vagrantfile](http://docs.vagrantup.com/v2/vagrantfile/)，那么你只需要执行vagrant
 up就行了，所有的软件都会安装并且配置好。团队成员可以通过相同的Vagrantfile来创建他们的开发环境，无论他们是在Linux, Mac OS X, 或者Windows下，这样就可以保证你团队成员的代码是跑在相同的环境中。
安装非常简单，直接下载对应操作系统的版本就可以了
[https://www.vagrantup.com/downloads.html](https://www.vagrantup.com/downloads.html)
Vagrant 安装后还不能直接使用，因为它只是一个虚拟机管理和配置工具，虚拟机系统的安装和运行还得靠专门的虚拟化软件。Vagrant
 默认已经内置了 VirtualBox Provider 用来跟 VirtualBox 交互，所以再去 [VirtualBox 官网](http://www.virtualbox.org/)下载并安装
 VirtualBox 就可以正式开始使用了。

程序员搞最多的就是码代码了，可能做很多个项目，公司里搞java的，php的，自己回家再玩个python什么的，想体验下新版本的mongodb或者nginx，Emacs或者vim的配置啦，保不准哪个项目前个版本的数据库是mysql，下个版本用了postgresql, 这么多东西全都搞在一起，装在一个电脑上，肯定会被这各种配置环境搞的晕晕的，烦不胜烦。
怎么办呢，最好是每个项目都有一个干净的开发环境，只为这个项目，可是我们不可能为每一个项目配一个电脑吧，有了，虚拟机，给每一个项目配一个虚拟机，开发A的时候就启A的虚拟机，这样各个开发环境互相独立，干干净净。还有一个问题，我们的项目有多个开发人员，如何保障大家的开发环境都一样呢，总不能每个人都一个个点击鼠标，填写配置参数，建好后上支一个个软件安装吧，这太麻烦了，太不geek了。我们想要的是，环境只配置一遍，然后可以把这个环境打包deliver给别人，别人拿到后，直接启起来就可以用。那有没有这样的东西呢，肯定是有的，Vagrant，它就是用来干这个的。
Create a single file for your
project to describe the type of machine you want, the software
 that needs to be installed, and the way you want to access the machine. Store
 this file with your project code.
Run a single command — "vagrant
 up"
SSH into this machine withvagrant ssh
The first step in configuring any Vagrant project is to create a[Vagrantfile](https://www.vagrantup.com/docs/vagrantfile/):
- Mark the root directory of your project. Many of the configuration options in Vagrant are relative to this root directory.
- Describe the kind of machine and resources you need to run your project, as well as what software to install and how you want to access it.

$ mkdir vagrant_getting_started$ cd vagrant_getting_started$ vagrant init
This will place a Vagrantfile in your current directory. 
specifying the box to use for your Vagrant environment is always the
first step
after creating a new Vagrantfile.
vagrant box 
add hashicorp/precise64
the username is "hashicorp", and the box is "precise64". 

run vagrant destroy back on your host machine, and Vagrant will terminate the use of any
 resources by the virtual machine.
The vagrant destroy command
 does not actually remove the downloaded
 box file. To *completely*remove
 the box file, you can use the vagrant box remove command.
By default, Vagrant shares your project directory (remember, that is the one with the Vagrantfile) to the /vagrant directory
 in your guest machine.
Note that when you vagrant ssh into your machine, you're in/home/vagrant. /home/vagrant is
 a different directory from the synced/vagrant directory.
logout退出！
installing packages, users, and more with [provisioning](https://www.vagrantup.com/docs/getting-started/provisioning.html):
Let us now serve those files using a webserver.
We could just SSH in and install a webserver and be on our way,
but then every person who used Vagrant would have to do the same thing. Instead, Vagrant has built-in support for *automated provisioning*. Using this feature, Vagrant will
automatically install software when youvagrant up so
 that the guest machine can be repeatably created and ready-to-use.
Vagrant.configure("2") do |config| config.vm.box = "hashicorp/precise64" config.vm.provision :shell, path: "bootstrap.sh"end
The "provision" line is new, and tells Vagrant to use the shellprovisioner
 to setup the machine, with the bootstrap.sh file. The file path is relative to the
 location of the project root (where the Vagrantfile is).
If the guest machine is already running from a previous step, runvagrant
 reload --provision

port forwarding
allows you to specify ports on the guest machine to share via a port on the
host machine外面的宿主机. This allows you to access a port on your own machine, but actually have all
 the network traffic forwarded to a specific port on the guest machine.
Vagrant.configure("2") do |config| config.vm.box = "hashicorp/precise64" config.vm.provision :shell, path: "bootstrap.sh" config.vm.network :forwarded_port,
 guest: 80, host: 4567end
 load http://127.0.0.1:4567 in your browser. 
分享
Vagrant Share lets you share your Vagrant environment to anyone around
 the world
with an Internet connection. It will give you a URL that will route directly to your Vagrant environment from any device in the world that is connected to the Internet.
[https://www.vagrantup.com/docs/getting-started/share.html](https://www.vagrantup.com/docs/getting-started/share.html)
Before being able to share your Vagrant environment, you will need an account on [HashiCorp's
 Atlas](https://www.vagrantup.com/docs/other/atlas.html). Do not worry, it is free.
Once you have an account, log in using vagrant
 login:
$ vagrant loginUsername or Email: mitchellhPassword (will be hidden):You are now logged in!
SHARE
Once you are logged in, run vagrant
 share:
$ vagrant share...==> default: Your Vagrant Share is running!==> default: URL: http://frosty-weasel-0857.vagrantshare.com...
Your URL will be different, so do not try the URL above. Instead, copy the URL that vagrant share outputted
 for you and visit that in a web browser. It should load the Apache page we setup earlier.
To end the sharing session, hit Ctrl+C in your terminal.
-----------------------------------
停止：
**Suspending** the virtual machine by calling vagrant
 suspend will save the current running state
of the machine and stop it. When you are ready to begin working again, just run vagrant up,
 and it will be resumed from where you left off. The main benefit of this method is that it is super
fast, usually taking only 5 to 10 seconds to stop and start your work. The downside is that the virtual machine still eats up
 your disk space, and requires even more disk space to store all the state of the virtual machine RAM on disk.
**Halting** the virtual machine by calling vagrant
 haltwill gracefully
shut
down the guest operating system and power down the guest machine. You can use vagrant
 up when you are ready to boot it again. The benefit of this method is that it will
cleanly shut down your machine, preserving the contents of disk, and allowing it to be cleanly started again. The downside is that
 it'll take some extra time to start from a cold boot, and the guest machine still consumes disk space.
**Destroying** the virtual machine by calling vagrant destroy will
 remove all traces **痕迹**of the guest machine from your system. It'll stop the guest machine,
 power it down, and remove all of the guest hard disks.
Again, when you are ready to work again, just issue a vagrant up. The benefit of this is that *no cruft* is left on your machine. The disk space and RAM consumed by the guest machine is reclaimed
**回收的**and your host machine is left clean. The downside is that vagrant up to
 get working again will take some extra time since it has to reimport the machine and
re-provision it.
