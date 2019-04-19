# CentOS搭建git服务器实测 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月12日 14:57:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：174
Git 可以使用四种主要的协议来传输数据：本地传输，SSH 协议，Git 协议和 HTTP 协议
1,安装:
CentOS/Fedora: yum install git
Ubuntu/Debian: apt-get install git
创建一个git用户，用来运行git服务
useradd git
passwd git
找一个目录，创建一个裸仓库
git init --bare your-project.git
chown -R git:git your-project.git
客户端连接：
ssh-keygen -t rsa -C "youremail@mailprovider.com"
根据提示输入密码。windows可用PuTTY Gen生成。
将你的id_rsa.pub上传到git服务器，然后用cat命令将内容拷贝到授权文件中。注意a single '>' will overwrite all the contents of the second file you specify. A double '>' will append it。
创建.ssh目录，然后创建authorized_keys文件,
mkdir /home/git/.ssh	
touch /home/git/.ssh/authorized_keys
cat id_ras.pub >> authorized_keys
If you want to add others to your access list, they simply need to give you their id_rsa.pub key and you append it to the authorized keys file.一行一个
Git服务器打开RSA认证
在Git服务器上首先需要将/etc/ssh/sshd_config中将RSA认证打开，即：
RSAAuthentication yes     
PubkeyAuthentication yes     
AuthorizedKeysFile  .ssh/authorized_keys
%h/.ssh: %h is replaced by the home directory of the user being authenticated
service sshd restart
authorized_keys文件的作用是什么？
一般懂Linux的朋友可能知道了，它是用来做SSH免密码登录配置的，在客户端生成一个RSA或者其他加密方式的公钥和私钥，
然后将公钥传至Linux服务端，保存到authorized_keys文件，这样，客户端在SSH远程登录的时候，就不需要输入密码验证了。
git clone git@192.168.8.34:/data/git/learngit.git
测试
vim test.c
git add test.c
git commit -m "test"
git push -u origin master
艺搜参考
[https://github.com/elesos/progit/blob/master/zh/04-git-server/01-chapter4.markdown](https://github.com/elesos/progit/blob/master/zh/04-git-server/01-chapter4.markdown)有GitWeb的介绍
[https://git-scm.com/book/zh/v2/%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E7%9A%84-Git-%E5%9C%A8%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E6%90%AD%E5%BB%BA-Git](https://git-scm.com/book/zh/v2/%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E7%9A%84-Git-%E5%9C%A8%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E6%90%AD%E5%BB%BA-Git)
