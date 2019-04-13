
# Linux常用工具的安装 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:49:19[尹成](https://me.csdn.net/yincheng01)阅读数：26


**@安装PyCharm**
官网下载压缩包
解压缩
进入压缩目录，[运行pycharm.sh](http://xn--pycharm-cb1ux83c.sh)
**@安装MySQL**
`sudo apt-get install mysql-server		无法通过apt安装，应该是兼容性问题
sudo apt install mysql-client			安装client端
sudo apt install libmysqlclient-dev 	安装依赖	
netstat -tap | grep mysql				查看是否运行（参数：-t=tcp，-a=all，-p=显示程序名称）
sudo gedit /etc/mysql/mysql.conf.d/mysqld.cnf	修改配置文件，注释掉bind 127.0.0.1
grant all on *.* to root@'%' identified by '123456' with grant option;	mysql命令：允许所有IP访问
flush privileges;		mysql命令：刷新权限；
service mysql restart;	重启mysql
#接下来在另一台主机上测试连接即可；`**修改root的原始密码**
第一次使用root进行无密码登录
对系统数据库mysql的user表做如下修改
`use mysql;
//修改root用户的密码和插件类型
update user set plugin="mysql_native_password",authentication_string=password("123456") where user="root";
//刷新权限
flush privileges;`退出并使用新密码登录
**@安装Redis**
`sudo wget http://download.redis.io/releases/redis-4.0.9.tar.gz	下载压缩包，默认~/Downloads
tar -zxvf redis-4.0.9.tar.gz	解压
sudo apt-get install gcc	安装c语言编译器
cd redis-4.0.9/				进入目录
sudo make clean				清扫编译垃圾
sudo make					编译
sudo make install			安装
cd src						进入源码目录，那里有编好的可执行文件
redis-server ../redis.conf	运行服务端，使用配置文件
ps -ef | grep redis			查看redis是否运行
redis-cli					启动客户端进行测试`**@安装vim**
`sudo apt-get remove --purge vim	完全卸载vim
unzip vim-master.zip	解压
cd vim-master/			进入目录
./configure				执行配置，会搜索和安装需要的类库
apt search ncurses		搜索缺失的类库
sudo apt install libncurses5-dev	安装依赖库
./configure			再次执行配置，通过
make clean			保险起见，清除原来编译的垃圾
make				编译
sudo make install	安装
vim					启动vim`**@安装搜狗拼音**
在Ubuntu应用市场中搜索安装fcitx系列工具
安装输入法切换工具：
`sudo apt install im-switch`安装搜狗输入法（本次会失败，提示依赖有问题）
`dpkg -i sogouxxx.deb`自动修复依赖关系（缺失和兼容性）
`sudo apt install -f`安装搜狗输入法（本次会成功）
`dpkg -i sogouxxx.deb`在【设置-地区和语言】中，点击【管理已安装的语言】，在对话框中将输入法系统设置为fcitx
![在这里插入图片描述](https://img-blog.csdn.net/2018091914302172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
重新启动Ubuntu
reboot点开桌面右上角的输入法设置-当前设置：
![在这里插入图片描述](https://img-blog.csdn.net/20180919142137569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击加号，取消勾选Show Only Current Language，搜索sogou，追加搜狗输入法，BINGO搞定!
![在这里插入图片描述](https://img-blog.csdn.net/20180919142420712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

