# OpenSSH实现Windows与Linux之间文件的安全传输 - Orisun - 博客园







# [OpenSSH实现Windows与Linux之间文件的安全传输](https://www.cnblogs.com/zhangchaoyang/articles/1875191.html)





SSH实现Windows和Linux系统间的文件实现安全传输
本例中使用Windows(ip:192.168.1.25)来做客户端，Linux(ip:192.168.1.26)来做服务器。在Linux中安装ssh服务，这里就不讲了。
1.下载OpenSSH ： http://sshWindows.sourceforge.net/ 并安装。默认安装目录为C:\Program Files\OpenSSH2.修改/home（根目录），在注册表中修改
[HKEY_LOCAL_MACHINE\SOFTWARE\Cygnus Solutions\Cygwin\mounts v2\/home]
“native”=”C:\Program Files\OpenSSH\\home”
3.在C:\Program Files\OpenSSH 下建立目录 home 和username
cd C:\Program Files\OpenSSH
mkdir home
mkdir home\Administrator
mkdir home\Administrator\.ssh
4.生成密钥
cd C:\Program Files\OpenSSH\bin
ssh-keygen.exe -d
在C:\Program Files\OpenSSH\home\Administrator\.ssh目录生成两个文件
id_dsa 和 id_dsa.pub ，后缀.pub的就是公钥
5.拷贝id-dsa.pub到Linux服务器上，可以通过winscp来拷贝，是一个很好的Windows客户端工具。
6.在Linux服务器上执行命令
cat ~/.ssh/id_dsa.pub >> authorized_keys
7.修改sshd配置文件
vi /etc/ssh/sshd_config
将
#PubkeyAuthentication yes
#AuthorizedKeysFile .ssh/authorized_keys
的注释去掉，采用authorized_keys文件的验证用户身份
8.重启sshd服务
#service sshd restart
9.到Windows下采用scp 和ssh就可以登陆并拷贝文件了，不需要在输入密码，实现了文件的安全传输。
>ssh.exe root@192.168.1.26 #登陆到Linux服务器了
>scp.exe somefile root@192.168.1.26:/root/ #拷贝文件。
虽然使用公钥验证是一个提高SSH安全数据传输的很好的方法。传输SSH身份密钥也会是一件痛苦的事情。首先，你要创建你的密钥对。然后，你把公钥复制到所有你希望登录机器的正确位置。密钥必须是一种特殊的格式，你必须到一个有正确权限的特定目录。
幸运的是 ssh-copy-id，一个OpenSSH里面的小工具，让这一切变得简单。
SSH 是一个安全的远程管理工具，提供许多处理的捷径及能力。 比如，你可以登录然后，执行一个远程命令，而这一切只需要一个命令。而不用首先登录，再输入命令：carla@host1:~$ ssh terry@host2 ls ~
这是一个有意思的例子并且有一个小玄机- ls ~ 会列出 Carla的 home 目录，而不是 Terry的。如果你想看 Terry的 home 目录你必须使用 ls /home/terry。做这些你只需要输入一要命令。比如启动一个备份脚本，查看进程或者打印一份文档。
$ ssh-copy-id -i id_rsa.pub terry@host2
ssh-copy-id 用正确的格式复制身份密钥。要确保文件权限及属主是正确的并且要确定，私钥不会被意外上传。
使用基于密钥的认证而非密码认证，表示你不用泄露你的系统密码。为了让管理多台远程机器更加简单，你可以使用你所想要的任何名字创建密钥，就像下面这样：
$ ssh-keygen -t rsa -f id_apacheserver
要连接的时候，你可以容易使用正确的定密钥：
$ ssh -i id_apacheserver carla@host.alrac.net
最后，不要忘记强大的 sshfs 命令，用它来加载一个远程文件系统。这比Samba及NFS服务器速度都要快。首先创建 一个本地目录作为挂载点，然后加载远程文件系统：
$ sshfs hostname:/remotedir localdir/
现在你就可以，像操作本地文件一样操作这个远程文件系统了。












