# CentOS6.5下用Git克隆代码（https方式） - 左直拳的马桶_日用桶 - CSDN博客
2014年10月20日 18:32:01[左直拳](https://me.csdn.net/leftfist)阅读数：5806
一、首先最好保证GIT是最新版
查看GIT命令
$ git --version
有关git的安装，应该有好多文章介绍。注意更新之后，要重启系统，否则显示的版本号，还是老版本。
二、如果工作环境存在网络代理，那么要设置代理：
git配置（非root）：
git config --global user.name "username"
git config --global user.email emailbox
git config --global http.proxy http://username:password@proxyserver:port
git config --global http.sslverify=false
然后
git clone .....
即可。代码服务器如果需要验证的话，会弹出对话框，让你输入账号和密码。如果见不到这个对话框，说明没有连上去。
三、我的经验教训：
失败！失败了好多次。
我已经按照上面的步骤，做足功夫，设置好git的各种配置。
后来发现，这是由于我下载代码的时候，采用了sudo命令。
原因是，我之前操作的时候，用root身份创建了一个目录~/task，后来回到当前账户，想将代码下载到这个目录：
$ cd ~/task
$ git clone https://code.teambition.com/project/core.git
提示没有权限创建一个名为 “core” 的工作目录
也没有多想，自然而然地采用了sudo 命令:
$ sudo git clone https://code.teambition.com/project/core.git
结果就是无法连接源代码服务器。
后来将旧文件夹删除，用当前账户身份创建了一个新的文件夹，运行
$ git clone https://code.teambition.com/project/core.git
终于显示代码服务器的账户、密码输入提示，正确输入后，克隆成功。
看来sudo是让当前账户以超级管理员的身份运行命令，而不仅仅是让当前账户获得超级管理员的权限，否则无法解释为何加上sudo之后，git config http.proxy就会失效。
但是，设置git的配置的时候，不是有—global的参数吗，为何对超级管理员无效呢。
