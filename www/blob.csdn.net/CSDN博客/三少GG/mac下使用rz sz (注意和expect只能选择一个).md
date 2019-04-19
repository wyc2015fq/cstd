# mac下使用rz/sz (注意和expect只能选择一个) - 三少GG - CSDN博客
2015年06月14日 14:34:42[三少GG](https://me.csdn.net/scut1135)阅读数：9085
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
习惯了 在windows下的securecrt和xshell的rz 和sz。
rz 可以很方便的从客户端传文件到服务器，sz也可以很方便的从服务器传文件到客户端，就算中间隔着跳板机也不影响。在mac下试了一下，mac的终端是不支 持的，需要下载item2。另外不能**在mac下用expect 自动登录服务器，执行rz或sz 否则终端会挂掉，改用scp/rsync吧。**
**​1.先安装item2，**item2 市类似mac风格的终端
     item2 下载地址，​​http://iterm2.com/downloads.html，下载后解压缩就能运行
2.使用brew 安装lrzsz
   yubing> sudo brew install lrzsz
   安装完成后检查 ls -alh /usr/local/bin/sz 是否存在
下载iterm2-zmodem
$ sudo wget https://raw.github.com/mmastrac/iterm2-zmodem/master/iterm2-send-zmodem.sh
$ sudo wget https://raw.github.com/mmastrac/iterm2-zmodem/master/iterm2-recv-zmodem.sh
$sudo chmod 777 /usr/local/bin/iterm2-*
给终端Iterm2 添加触发trigger
    打开iterm2 ------  同时按 command和,键 ------- Profiles ----------  Default ------- Advanced ------ Triggers的Edit按钮

转载：
## 1.包安装和管理
brew是mac下的包管理工具，类似yum，apt-get等工具。 比安装wget命令是：
brew install wget
卸载：
brew uninstall wget
具体参见brew的官网： [http://brew.sh/](http://brew.sh/)
## 2.ssh免密码登录服务器
mac上创建本地的密钥对：
ssh-keygen -t rsa
会在 ~/.ssh/下面生成id_rsa和id_rsa.pub，其中id_rsa.pub是需要部署到远程服务器上的，可以使用ssh-copy-id进行分发：
```
brew install ssh-copy-id
ssh-copy-id user@hostname
```
对应远程服务器的用户和主机名，输入密码后，id_rsa.pub就copy到远程的~/.ssh/的authorized_keys中 此后，再登录远程的时候就不需要输入密码了
`ssh user@hostname`
## 3.远程服务器文件下载和上传
以前在windowns下用sercueCRT的时候，从远端服务器下载文件和上传文件，可以简单的使用sz，rz命令，其实iterm2也能支持rzsz命令，具体步骤是：
安装lrzsz:
brew install lrzs
安装脚本到mac指定目录，地址在： [https://github.com/mmastrac/iterm2-zmodem](https://github.com/mmastrac/iterm2-zmodem)
Save the iterm2-send-zmodem.sh and iterm2-recv-zmodem.sh scripts in /usr/local/bin/
这一步需要注意的是：
a.脚本的可执行权限
b.脚本里面的sz/rz的路径设置正确
然后利用iterm2的triggers功能，当iterm2探测到指定的正则匹配后，自动执行后来的命令，在triggers里面配置两个规则：
Regular expression: \*\*B0100
Action: Run Silent Coprocess
Parameters: /usr/local/bin/iterm2-send-zmodem.sh
Regular expression: \*\*B00000000000000
Action: Run Silent Coprocess
Parameters: /usr/local/bin/iterm2-recv-zmodem.sh
使用方法：
### To send a file to a remote machine:
Type “rz” on the remote machine
Select the file(s) on the local machine to send
Wait for the coprocess indicator to disappear
### The receive a file from a remote machine
Type “sz filename1 filename2 … filenameN” on the remote machine
Select the folder to receive to on the local machine
Wait for the coprocess indicator to disappear
