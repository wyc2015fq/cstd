# Ubuntu 下 SSH 的应用 - 文章 - 伯乐在线
原文出处： [yueming](http://blog.chinaunix.net/uid-429659-id-5755175.html)
1 ssh远程登录服务器


```
ssh username@remote_ip #将username换成自己的用户名，将remote_ip换成远程服务器的ip地址
```
2 将文件/文件夹从远程服务器拷至本地(scp)


```
scp -r username@remote_ip:/home/username/remotefile.txt ./
```
3 将文件/文件夹从本地拷至远程服务器(scp)


```
scp -r localfile.txt username@remote_ip:/home/username/
```
4 将文件/文件夹从远程服务器拷至本地(rsync)


```
rsync -v -u -a --delete --rsh=ssh –stats username@remote_ip:/home/username/remotefile.txt .
```
5 将文件/文件夹从本地拷至远程服务器(rsync)


```
rsync -v -u -a --delete --rsh=ssh --stats localfile.txt username@remote_ip:/home/username/
```
6 连接远程ssh非22端口的服务器(ssh端口为12345)


```
ssh -p 12345 username@remote_ip
```
7 远程拷贝ssh非22端口的服务器文件(ssh端口为12345)


```
scp -P 12345 local_file username@remote_ip:remote_dir
scp -P 12345 username@remote_ip:remote_file local_dir
scp -o port=12345 username@remote_ip:remote_file local_dir
scp -P 12345 -r local_dir/.* username@remote_ip:remote_dir
```
拷贝目录，-r是将目录下的目录递归拷贝。”.*”是将隐藏文件也拷贝过去。需要先在远端创建好相应的目录。
sftp用法


```
sftp -o port=12345 username@remote_ip:remote_dir
```
1. 首先在服务器上安装ssh的服务器端。


```
$ sudo aptitude install openssh-server
```
2. 启动ssh-server。


```
$ /etc/init.d/ssh restart
```
3. 确认ssh-server已经正常工作。


```
$ netstat -tlp
tcp6 0 0 *:ssh *:* LISTEN -
```
看到上面这一行输出说明ssh-server已经在运行了。
4. 在[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)客户端通过ssh登录服务器。假设服务器的IP地址是192.168.0.103，登录的用户名是hyx。


```
$ ssh -l hyx 192.168.0.103
```
接下来会提示**输入密码**，然后就能成功登录到服务器上了。
上传文件：
用ssh自带的scp或者sftp.
scp 本地文件 user@host:远程路径
