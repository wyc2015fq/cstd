
# ubuntu下面的git服务器搭建 - 嵌入式Linux - CSDN博客

2016年07月15日 16:41:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：958


**１、安装相应的软件和依赖**


```python
ubuntu:~$ sudo apt-get install git-core openssh-server openssh-client
git-core是git版本控制核心软件
安装openssh-server和openssh-client是由于git需要通过ssh协议来在服务器与客户端之间传输文件
然后中间有个确认操作，输入Y后等待系统自动从镜像服务器中下载软件安装，安装完后会回到用户当前目录。如果
安装提示失败，可能是因为系统软件库的索引文件太旧了，先更新一下就可以了，更新命令如下：
ubuntu:~$ sudo apt-get update
更新完软件库索引后继续执行上面的安装命令即可。
```
**２、配置服务器**
在服务器端
查看ssh有没有启动

```python
root@weiqifa-Inspiron-3847:/home/git# ps -e|grep ssh
 1686 ?        00:00:00 sshd
root@weiqifa-Inspiron-3847:/home/git#
```
创建git用户，用来管理代码的用户

```python
root@weiqifa-Inspiron-3847:/home/git# sudo useradd git
useradd: user 'git' already exists
root@weiqifa-Inspiron-3847:/home/git#
```
**秘钥文件**
在gitServer上我们首先查看/home/git/.ssh目录下是否存在authorized_kesys文件，
如果没有，可以通过touch authorized_keys创建此文件。
![](https://img-blog.csdn.net/20161014141207217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在客户机端
在客户端用ssh-keygen -t rsa 生成秘钥
然后用命令scp把秘钥复制到服务器

```python
scp id_rsa.pub weiqifa@192.168.0.88:/home/git/.ssh/
```
然后回到服务器

```python
root@weiqifa-Inspiron-3847:/home/git/.ssh# ls
authorized_keys  id_rsa.pub
root@weiqifa-Inspiron-3847:/home/git/.ssh# cat id_rsa.pub >> authorized_keys 
root@weiqifa-Inspiron-3847:/home/git/.ssh# cat authorized_keys 
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDkIt2Hh532FoWoj0IUztIYF+jz4F1KvyOAmrhpxUMnnuj+YEOhyw6AGL/NAjmE5r8SzeJLTtAkVk9s143R9tYouPOgHPuwTTFvDy/skjD14yMne1aHzre2rM0jGiv5gtZDmi523FvQob7oTM5xOKKk3lsMBQNPdWrPPWqs8kS941sdWYymNQqFLBH4DpzDUAS+Itk5U6aNvVcQbldmGR+F3IxIKxH76HwpTmh+Os5fqraQLOMpWZzu8h0KMMMnNdideV0lHfq40Swb0ZO6ZGy/tdXBTc7oO8bn11oRVQ+Uf+USYMBzB6PCe6gODI3Fp0cBFzzQNque6MIAh6ELClUn Administrator@JU674J315CAOGPV
root@weiqifa-Inspiron-3847:/home/git/.ssh#
```

**３、建立仓库并克隆**
建立个文件夹用来管理仓库

```python
root@weiqifa-Inspiron-3847:/home/git# mkdir repo
root@weiqifa-Inspiron-3847:/home/git# chown -R git:git repo/
root@weiqifa-Inspiron-3847:/home/git# chmod 700 repo/
root@weiqifa-Inspiron-3847:/home/git#
```
在服务器测试clone一下

```python
root@weiqifa-Inspiron-3847:/home/git# cd repo/
root@weiqifa-Inspiron-3847:/home/git/repo# git init --bare sscom.git
Initialized empty Git repository in /home/git/repo/sscom.git/
root@weiqifa-Inspiron-3847:/home/git/repo# cd ../
root@weiqifa-Inspiron-3847:/home/git# ls
bin  examples.desktop  gitosis  id_rsa.pub  repo
root@weiqifa-Inspiron-3847:/home/git# cd ../
root@weiqifa-Inspiron-3847:/home# git clone --bare git
git/           gitrepository/ 
root@weiqifa-Inspiron-3847:/home# git clone --bare git/repo/sscom.git/
Cloning into bare repository 'sscom.git'...
warning: You appear to have cloned an empty repository.
done.
root@weiqifa-Inspiron-3847:/home# ls
git  gitrepository  lost+found  sscom.git  weiqifa
root@weiqifa-Inspiron-3847:/home#
```

在客户端clone一下

```python
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh
$ git clone git@192.168.0.88:/home/git/repo/sscom.git
Cloning into 'sscom'...
git@192.168.0.88's password:
warning: You appear to have cloned an empty repository.
Checking connectivity... done.
```
**在客户端push 一下**

```python
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh
$ git clone git@192.168.0.88:/home/git/repo/sscom.git
Cloning into 'sscom'...
git@192.168.0.88's password:
warning: You appear to have cloned an empty repository.
Checking connectivity... done.
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh
$ ls
id_rsa  id_rsa.pub  known_hosts  sample/  sscom/
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh
$ cd sscom/
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ ^C
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ ls
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ pwd
/c/Users/Administrator/.ssh/sscom
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ git branch
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ echo "this beginnings of project1" > any.file
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ git add .
warning: LF will be replaced by CRLF in any.file.
The file will have its original line endings in your working directory.
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ git commit -m "init"
[master (root-commit) e9565b2] init
warning: LF will be replaced by CRLF in any.file.
The file will have its original line endings in your working directory.
 1 file changed, 1 insertion(+)
 create mode 100644 any.file
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ git push origin master
git@192.168.0.88's password:
Counting objects: 3, done.
Writing objects: 100% (3/3), 229 bytes | 0 bytes/s, done.
Total 3 (delta 0), reused 0 (delta 0)
remote: error: insufficient permission for adding an object to repository database ./objects
remote: fatal: failed to write object
error: unpack failed: unpack-objects abnormal exit
To git@192.168.0.88:/home/git/repo/sscom.git
 ! [remote rejected] master -> master (unpacker error)
error: failed to push some refs to 'git@192.168.0.88:/home/git/repo/sscom.git'
```
*这个错误查看了一下这个链接*
*http://blog.csdn.net/yujunf/article/details/7595231*
在服务器然后再修改一下

```python
root@weiqifa-Inspiron-3847:/home/git/repo# ls
sscom.git
root@weiqifa-Inspiron-3847:/home/git/repo# chown -R git:git sscom.git/
root@weiqifa-Inspiron-3847:/home/git/repo#
```
回到客户端再push一下

```python
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
$ git push
git@192.168.0.88's password:
Counting objects: 3, done.
Writing objects: 100% (3/3), 229 bytes | 0 bytes/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To git@192.168.0.88:/home/git/repo/sscom.git
 * [new branch]      master -> master
Administrator@JU674J315CAOGPV MINGW64 ~/.ssh/sscom (master)
```

**４、总结**
１、原理问题查清楚，哪步在哪里执行，要搞清楚
２、认真仔细，ubuntu对用户权限要求比较高，要特别注意一下
３、学会在网查资料

**参考资料：**
*http://www.linuxdiyf.com/linux/12685.html*
*https://hacpai.com/article/1445337139234*
*http://www.linuxdiyf.com/linux/12159.html*
*http://www.linuxdiyf.com/linux/10730.html*
*http://www.gitguys.com/topics/creating-a-shared-repository-users-sharing-the-repository/*


