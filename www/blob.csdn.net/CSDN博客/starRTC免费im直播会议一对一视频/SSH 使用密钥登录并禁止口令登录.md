# SSH 使用密钥登录并禁止口令登录 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年01月09日 22:57:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：617
小结：修改下sshd配置文件，把公钥传上去就好了
先生成公钥和私钥，默认在/root/.ssh/目录，可以先看一下有没有这个目录。
生成公钥后，以后其它服务器也都可以复用这个公钥
最好生成时输入密码！！
2，**复制密钥对**
用ssh-copy-id将公钥复制到被管理机器上面，观察authorized_keys有无变化（之前服务器上没有这个文件，拷贝后会出现这个文件，而且文件内容就是公钥的内容，最好在另一台服务器上用ssh-copy-id命令拷贝到被管理的机器上，这样生成的authorized_keys权限被自动设置好了，不用自己设置）
拷贝你的公钥到被管理的服务器上
ssh-copy-id
-i ~**/**.ssh**/**id_rsa.pub
172.29.0.* 此处输入的密码是机器的密码，不是你设置的私钥密码
ssh-copy-id
-i ~**/**.ssh**/**id_rsa.pub
"root@192.168.160.44 -p 60022"
或直接cat id_rsa.pub >> authorized_keys
追加符 >> 而不是 >
**3，修改SSH配置文件**
vim /etc/ssh/sshd_config
关闭root用户使用密码登陆
#禁用密码验证
PasswordAuthentication no //修改为no
133行（最后一行）
打开下面3个的注释。43行
#启用密钥验证
RSAAuthentication yes
PubkeyAuthentication yes
#指定公钥数据库文件
AuthorizedKeysFile .ssh/authorized_keys

可以用下面命令可以修改（最好用vim手动）：
sed -i "s/^PasswordAuthentication.*/PasswordAuthentication no/g" /etc/ssh/sshd_config
sed -i "s/^#RSAAuthentication.*/RSAAuthentication yes/g" /etc/ssh/sshd_config
sed -i "s/^#PubkeyAuthentication.*/PubkeyAuthentication yes/g" /etc/ssh/sshd_config
sed -i "s/^#AuthorizedKeysFile.*/AuthorizedKeysFile .ssh\/authorized_keys/g" /etc/ssh/sshd_config
重启SSH服务前建议多保留一个会话以防不测
service sshd restart
客户端如何连接：
xshell: 点击浏览，选择文件
不用管红色部分
法二;
Next：
如何authorized_keys里区分各个用户的内容？authorized_keys 注释
参考：
[https://wsgzao.github.io/post/ssh/](https://wsgzao.github.io/post/ssh/)
[http://blog.csdn.net/bravezhe/article/details/7302800](http://blog.csdn.net/bravezhe/article/details/7302800)
[http://7056824.blog.51cto.com/69854/403669/](http://7056824.blog.51cto.com/69854/403669/)
