
# ubutun16.04LTS获取root账户并登录 - lpty的博客 - CSDN博客

2017年01月13日 22:29:57[lpty](https://me.csdn.net/sinat_33741547)阅读数：1331


一、设置密码
1、为root账户设置密码
sudo passwd root
2、切换root账户
su root
二、修改配置文件
1、打开配置文件50-unity-greeter.conf
sudo gedit  /usr/share/lightdm/lightdm.conf.d/50-unity-greeter.conf
2、添加内容，保存
user-session=ubuntu
greeter-show-manual-login=true
all-guest=false3、打开配置文件.profile
sudo gedit /root/.profile
4、修改为 mesg n || true 为 tty -s && mesg n || true
5、保存
三、重启，登陆root账户

