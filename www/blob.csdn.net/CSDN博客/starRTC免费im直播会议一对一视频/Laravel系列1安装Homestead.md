# Laravel系列1安装Homestead - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 16:28:49[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：342

laravel requirements:
- PHP >= 5.5.9   (机器上yum安装的是5.3.3)
- OpenSSL PHP Extension
- PDO PHP Extension
- Mbstring PHP Extension
- Tokenizer PHP Extension

Laravel
 Homestead是一个官方的、预封装的Vagrant“箱子”，它提供给你一个奇妙的开发环境而不需要你在本机上安装PHP、HHVM、web服务器和其它的服务器软件。不用再担心搞乱你的操作系统！Vagrant箱子是完全可支配的。如果出现故障，你可以在几分种内完成销毁和重建箱子！
Homestead能运行在所有的Windows、Mac或Linux系统上，它包含了Nginx、PHP
 5.6、MySQL、Postgres、Redis、Memcached和你开发神奇的Laravel应用程序需要的所有其它软件。
后面的所有命令都是在 Git Bash 里面执行，并不是在 Windows 的命令提示符里面执行，这是新手很容易犯的一个错误，切记，一定要在 Git Bash 里面执行命令。
#### 安装Homestead Vagrant Box：
`vagrant box add laravel/homestead`
```
```
``
进入 Windows 的 home 目录
cd ~
执行
`git clone https://github.com/laravel/homestead.git Homestead`
执行完成后，会在 home 目录下生成一个 Homestead 目录，在 Git Bash 中进入 Homestead 目录，执行：
bash init.sh
此时，会在 home 目录下生成一个 .homestead 文件夹，里面会生成一些文件，其中 Homestead.yaml 文件很重要，有些地方需要你手动修改。

配置
1，共享文件夹
folders:
    - map: C:/Users/leijh/Desktop/ljh_share
      to: /home/vagrant/Code
Nginx站点配置：
sites:
    - map: homestead.app
      to: /home/vagrant/Code/Laravel/public
即站点homestead.app映射到了C:/Users/leijh/Desktop/ljh_share/Laravel/public下
在public目录里放一个html文件即可访问
在hosts文件里添加
`192.168.10.10  homestead.app`
上面IP在配置文件最前面有，然后
[http://homestead.app/](http://homestead.app/)
即可访问
【附：多站点配置：
只需要在sites选项中新增匹配即可：
sites:
    - map: homestead.app
      to: /home/vagrant/Code/Laravel/public
    - map: test.app
      to: /home/vagrant/Code/Laravel/test
然后也要在hosts中加上
192.168.10.10  test.app
然后vagrant reload --provision，注意上面的配置文件不能用TAB键
】
编辑完 Homestead.yaml 后，进入 Homestead 文件夹，在 Git Bash 中执行 vagrant up 命令。Vagrant 会将虚拟机开机，并且自动配置你的共享目录和 Nginx 站点。
虚拟机运行后，进入 Homestead 目录，在 Git Bash 中通过 vagrant ssh 命令就可以连接虚拟机了。
vagrant halt：关闭 Homestead 虚拟机。
vagrant reload --provision 
 如果修改了sites属性，需要运行更新nginx配置,配置文件在 /etc/nginx/sites-available/
destroy the machine, you may use the
```
vagrant
 destroy --force
```
command.
参考：
[http://blog.qiji.tech/archives/7787](http://blog.qiji.tech/archives/7787)
laravel requirements:
- PHP >= 5.5.9   (机器上yum安装的是5.3.3)
- OpenSSL PHP Extension
- PDO PHP Extension
- Mbstring PHP Extension
- Tokenizer PHP Extension

Laravel
 Homestead是一个官方的、预封装的Vagrant“箱子”，它提供给你一个奇妙的开发环境而不需要你在本机上安装PHP、HHVM、web服务器和其它的服务器软件。不用再担心搞乱你的操作系统！Vagrant箱子是完全可支配的。如果出现故障，你可以在几分种内完成销毁和重建箱子！
Homestead能运行在所有的Windows、Mac或Linux系统上，它包含了Nginx、PHP
 5.6、MySQL、Postgres、Redis、Memcached和你开发神奇的Laravel应用程序需要的所有其它软件。
后面的所有命令都是在 Git Bash 里面执行，并不是在 Windows 的命令提示符里面执行，这是新手很容易犯的一个错误，切记，一定要在 Git Bash 里面执行命令。
#### 安装Homestead Vagrant Box：
`vagrant box add laravel/homestead`
```
```
``
进入 Windows 的 home 目录
cd ~
执行
`git clone https://github.com/laravel/homestead.git Homestead`
执行完成后，会在 home 目录下生成一个 Homestead 目录，在 Git Bash 中进入 Homestead 目录，执行：
bash init.sh
此时，会在 home 目录下生成一个 .homestead 文件夹，里面会生成一些文件，其中 Homestead.yaml 文件很重要，有些地方需要你手动修改。

配置
1，共享文件夹
folders:
    - map: C:/Users/leijh/Desktop/ljh_share
      to: /home/vagrant/Code
Nginx站点配置：
sites:
    - map: homestead.app
      to: /home/vagrant/Code/Laravel/public
即站点homestead.app映射到了C:/Users/leijh/Desktop/ljh_share/Laravel/public下
在public目录里放一个html文件即可访问
在hosts文件里添加
`192.168.10.10  homestead.app`
上面IP在配置文件最前面有，然后
[http://homestead.app/](http://homestead.app/)
即可访问
【附：多站点配置：
只需要在sites选项中新增匹配即可：
sites:
    - map: homestead.app
      to: /home/vagrant/Code/Laravel/public
    - map: test.app
      to: /home/vagrant/Code/Laravel/test
然后也要在hosts中加上
192.168.10.10  test.app
然后vagrant reload --provision，注意上面的配置文件不能用TAB键
】
编辑完 Homestead.yaml 后，进入 Homestead 文件夹，在 Git Bash 中执行 vagrant up 命令。Vagrant 会将虚拟机开机，并且自动配置你的共享目录和 Nginx 站点。
虚拟机运行后，进入 Homestead 目录，在 Git Bash 中通过 vagrant ssh 命令就可以连接虚拟机了。
vagrant halt：关闭 Homestead 虚拟机。
vagrant reload --provision 
 如果修改了sites属性，需要运行更新nginx配置,配置文件在 /etc/nginx/sites-available/
destroy the machine, you may use the
```
vagrant
 destroy --force
```
command.
参考：
[http://blog.qiji.tech/archives/7787](http://blog.qiji.tech/archives/7787)

