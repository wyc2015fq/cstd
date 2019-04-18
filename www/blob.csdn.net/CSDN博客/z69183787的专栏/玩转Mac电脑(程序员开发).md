# 玩转Mac电脑(程序员开发) - z69183787的专栏 - CSDN博客
2019年02月11日 18:03:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：105
[https://my.oschina.net/genghz/blog/2993181](https://my.oschina.net/genghz/blog/2993181)
## 一. 初识Mac开发
        我第一次接触Mac时，在刚刚工作的时候，当时并不觉得Mac多好用，还是习惯Win，但是当你熟练了，用过了一段时间，你会发现用Mac开发的才是真正的利器，和服务相似的终端、各种便捷操作，真是用起来再抛弃不掉。
## 二. Mac怎么能没有iTerm2和Homebrew
###     1. iTerm2安装与配置
        iTerm2并不是说一定需要用的，但是iTerm2会让你应用终端更加炫酷、舒服
        (1). 安装iTerm2
            直接下载，安装到Application中 [下载链接](https://www.iterm2.com/downloads.html)
        (2). 配置iTerm2 炫酷起来
            运行，command + , 调出配置窗口 (一次配置 透明、Full-Width、快捷键 如图配置)
![](https://oscimg.oschina.net/oscnet/caeb1dd1cd7b654ea67b40295ed44768c5c.jpg)
![](https://oscimg.oschina.net/oscnet/90f0d3d5ea486565b35444613f814c33363.jpg)
![](https://oscimg.oschina.net/oscnet/5e7d09d1875853bb410a9e199a8584a9167.jpg)
            command + q 退出iTerm2，再重新打开，弹出iTerm窗口直接commad + w关闭，然后用热键 option + space，调出、隐藏
        (3). iTerm2 Dock中图标隐藏
            既然都可以用快捷键，调出、隐藏，那么我要iTerm2在Dock何用，很是碍眼，现在就让它消失掉             
            方法一：
**/usr/libexec/PlistBuddy -c "Add :LSUIElement bool true" /Applications/iTerm.app/Contents/Info.plist**
            方法二：
**                defaults write /Applications/iTerm.app/Contents/Info.plist LSUIElement 1                sudo codesign -f -s - /Applications/iTerm.app**
方法二 更通用一些，试用于微信、QQ、酷狗音乐等，你想在Dock中隐藏的，都可以去尝试，有问题大不了重装 (需要先打开一次app，再清除Dock)
            执行完发现，重启ITerm2，就会小时的无影无踪 (>_<)
        (4). iTerm2 rz/sz配置 (不要问我是什么，找度娘)
            写入配置文件 [github配置文件地址](https://github.com/mmastrac/iterm2-zmodem)
```
cd ／usr/loal/bin
vim iterm2-recv-zmodem.sh
vim iterm2-send-zmodem.sh
把对应文件的内容写入进去，内容如下
```
**iterm2-recv-zmodem.sh**
```
#!/bin/bash
# Author: Matt Mastracci (matthew@mastracci.com)
# AppleScript from http://stackoverflow.com/questions/4309087/cancel-button-on-osascript-in-a-bash-script
# licensed under cc-wiki with attribution required 
# Remainder of script public domain
osascript -e 'tell application "iTerm2" to version' > /dev/null 2>&1 && NAME=iTerm2 || NAME=iTerm
if [[ $NAME = "iTerm" ]]; then
	FILE=`osascript -e 'tell application "iTerm" to activate' -e 'tell application "iTerm" to set thefile to choose folder with prompt "Choose a folder to place received files in"' -e "do shell script (\"echo \"&(quoted form of POSIX path of thefile as Unicode text)&\"\")"`
else
	FILE=`osascript -e 'tell application "iTerm2" to activate' -e 'tell application "iTerm2" to set thefile to choose folder with prompt "Choose a folder to place received files in"' -e "do shell script (\"echo \"&(quoted form of POSIX path of thefile as Unicode text)&\"\")"`
fi
if [[ $FILE = "" ]]; then
	echo Cancelled.
	# Send ZModem cancel
	echo -e \\x18\\x18\\x18\\x18\\x18
	sleep 1
	echo
	echo \# Cancelled transfer
else
	cd "$FILE"
	/usr/local/bin/rz -E -e -b
	sleep 1
	echo
	echo
	echo \# Sent \-\> $FILE
fi
```
**            iterm2-send-zmodem.sh**
```
#!/bin/bash
# Author: Matt Mastracci (matthew@mastracci.com)
# AppleScript from http://stackoverflow.com/questions/4309087/cancel-button-on-osascript-in-a-bash-script
# licensed under cc-wiki with attribution required 
# Remainder of script public domain
osascript -e 'tell application "iTerm2" to version' > /dev/null 2>&1 && NAME=iTerm2 || NAME=iTerm
if [[ $NAME = "iTerm" ]]; then
	FILE=`osascript -e 'tell application "iTerm" to activate' -e 'tell application "iTerm" to set thefile to choose file with prompt "Choose a file to send"' -e "do shell script (\"echo \"&(quoted form of POSIX path of thefile as Unicode text)&\"\")"`
else
	FILE=`osascript -e 'tell application "iTerm2" to activate' -e 'tell application "iTerm2" to set thefile to choose file with prompt "Choose a file to send"' -e "do shell script (\"echo \"&(quoted form of POSIX path of thefile as Unicode text)&\"\")"`
fi
if [[ $FILE = "" ]]; then
	echo Cancelled.
	# Send ZModem cancel
	echo -e \\x18\\x18\\x18\\x18\\x18
	sleep 1
	echo
	echo \# Cancelled transfer
else
	/usr/local/bin/sz "$FILE" -e -b
	sleep 1
	echo
	echo \# Received $FILE
fi
```
            配置iTerm2，cmmand + ,  调出配置窗口
![](https://oscimg.oschina.net/oscnet/27d6804d4456661e42fad1591f546a28d14.jpg)
![](https://oscimg.oschina.net/oscnet/81c92f1c846a40feb19e57de6659dc35606.jpg)
```
Regular expression: \*\*B0100
Action: Run Silent Coprocess
Parameters: /usr/local/bin/iterm2-send-zmodem.sh
Regular expression: \*\*B00000000000000
Action: Run Silent Coprocess
Parameters: /usr/local/bin/iterm2-recv-zmodem.sh
```
            至此ITerm2配置完毕，基于ssh的 rz/sz已经可以应用了
###         (5). 常用快捷键介绍
            cmmand + t          再窗口中新建tab窗口
            cmmand + n         新建窗口(基本用不到)
            cmmand + d         分栏
            cmmand + k         清屏
            cmmand + w        关闭当前窗口/分栏
            cmmand + enter   全屏 
            cmmand + ,         调出配置窗口
            cmmand + q        退出
##     2. Homebrew安装
        Homebrew是什么？它是Mac上的包管理器，类似CentOs的yum等，让你下载、管理一条命令搞定，不需要每次去网上搜索、下载，具体可以看官网介绍 [Homebrew官网](https://brew.sh/index_zh-cn)
###         (1). 安装
            ITerm2或者终端，如下:
`/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
检测是否安装成功：
`brew -v`
出现以下字样证明安装完毕：
```
Homebrew 1.8.4
Homebrew/homebrew-core (git revision 78fa0; last commit 2018-12-05)
Homebrew/homebrew-cask (git revision d496a; last commit 2018-12-06)
```
###         (2). 配置国内镜像源
            Homebrew原生的当然是国外源，平时安装下载时会很慢，推荐两个镜像源，[清华镜像源](http://mirror.tuna.tsinghua.edu.cn/help/homebrew/)和[科大镜像源](https://lug.ustc.edu.cn/wiki/mirrors/help/homebrew-bottles)
```
替换镜像:
echo 'export HOMEBREW_BOTTLE_DOMAIN=https://mirrors.tuna.tsinghua.edu.cn/homebrew-bottles' >> ~/.bash_profile
source ~/.bash_profile
终端粘贴 替换镜像索引:
cd "$(brew --repo)"
git remote set-url origin https://mirrors.tuna.tsinghua.edu.cn/git/homebrew/brew.git
cd "$(brew --repo)/Library/Taps/homebrew/homebrew-core"
git remote set-url origin https://mirrors.tuna.tsinghua.edu.cn/git/homebrew/homebrew-core.git
brew update
这样的homebrew就可以飞快下载了（>_<）
```
###         (3). Homebrew应用
            Homebrew 命令主要有两类:
```
brew <cmd> [FORMULA]
brew cask <cmd> [FORMULA]
```
            command都是一样的， 主要的两个区别是：
                brew 主要用来下载一些不带界面的命令行下的工具和第三方库，mysql，maven，git等
                brew cask 主要用来下载一些带界面的应用软件或者已经编译好直接安装的软件，下载好后会自动安装，并能在mac中直接运行使用，jdk，chrome等
            下面看一下brew命令：
`brew help`
出现如下提示:
```
Example usage:
  brew search [TEXT|/REGEX/]
  brew info [FORMULA...]
  brew install FORMULA...
  brew update
  brew upgrade [FORMULA...]
  brew uninstall FORMULA...
  brew list [FORMULA...]
Troubleshooting:
  brew config
  brew doctor
  brew install --verbose --debug FORMULA
Contributing:
  brew create [URL [--no-fetch]]
  brew edit [FORMULA...]
Further help:
  brew commands
  brew help [COMMAND]
  man brew
  https://docs.brew.sh
```
命令还是很简单的
###         (3). 安装路径
             进入这个目录，以我的为例
```
cd /usr/local
ls
```
            如下:
`Caskroom	Cellar		Frameworks	Homebrew	bin		etc		include		lib		opt		sbin		share		var`
            Caskroom     brew cask命令所下载的安装包都在这里，安全可以cd进去，双击安装
            Cellar           brew 命令安装后的服务、依赖等都在这里
            etc              brew 命令安装后各个服务的配置文件的 link 文件
            bin               这个目录是被添加到path的, 存放的各服务终端命令的 link 文件
            opt              和bin类似，但是这个目录没有添加到path中
            sbin             则是一些服务的启动脚本的 link 文件
            var              brew 命令安装后的服务的数据，比如日志，mysql的数据文件等
###         (4). Homebrew安装mysql
            比如我想装一个mysql
```
brew search mysql
出现如下：
==> Formulae
automysqlbackup              mysql++                      mysql-cluster                mysql-connector-c++          mysql-search-replace         mysql@5.5                    mysql@5.7
mysql                        mysql-client                 mysql-connector-c            mysql-sandbox                mysql-utilities              mysql@5.6                    mysqltuner
证明有这么多mysql相关，如果你对mysql没有版本要求(可能装的近期新版本)：
brew install mysql 
这里我们安装5.6版本，直接复制粘贴mysql@5.6
brew install mysql@5.6
提示权限问题 请 sudo brew install mysql@5.6)
出现如下，则安装完毕(无关日志略):
......
MySQL is configured to only allow connections from localhost by default
To connect:
    mysql -uroot
mysql@5.6 is keg-only, which means it was not symlinked into /usr/local,
because this is an alternate version of another formula.
If you need to have mysql@5.6 first in your PATH run:
  echo 'export PATH="/usr/local/opt/mysql@5.6/bin:$PATH"' >> ~/.bash_profile
For compilers to find mysql@5.6 you may need to set:
  export LDFLAGS="-L/usr/local/opt/mysql@5.6/lib"
  export CPPFLAGS="-I/usr/local/opt/mysql@5.6/include"
To have launchd start mysql@5.6 now and restart at login:
  brew services start mysql@5.6
Or, if you don't want/need a background service you can just run:
  /usr/local/opt/mysql@5.6/bin/mysql.server start
==> Summary
🍺  /usr/local/Cellar/mysql@5.6/5.6.42: 341 files, 154.2MB
关键信息一:
If you need to have mysql@5.6 first in your PATH run:
  echo 'export PATH="/usr/local/opt/mysql@5.6/bin:$PATH"' >> ~/.bash_profile
意思要使用mysql命令需要自己写入path:
echo 'export PATH="/usr/local/opt/mysql@5.6/bin:$PATH"' >> ~/.bash_profile
source ~/.bash_profile
再另一个终端窗口 检测:
mysql -v
出现如下证明path已经配置成功，可以找到mysql命令，但是无法连接服务（因为我没启动啊!）
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/tmp/mysql.sock' (2)
关键信息二:
To have launchd start mysql@5.6 now and restart at login:
  brew services start mysql@5.6
Or, if you don't want/need a background service you can just run:
  /usr/local/opt/mysql@5.6/bin/mysql.server start
意思是启动mysql有两种方式:
brew services start mysql@5.6 
Or
/usr/local/opt/mysql@5.6/bin/mysql.server start
我当然推荐第一种，统一化管理, 直接执行启动
到这里mysql就安装完毕了
```
            brew services 命令介绍  
```
brew services -help
brew services command:
    Integrates Homebrew formulae with macOS' launchctl manager.
    [sudo] brew services list:
    List all running services for the current user (or root).
    [sudo] brew services run (formula|--all):
    Run the service formula without registering to launch at login (or boot).
    [sudo] brew services start (formula|--all):
    Start the service formula immediately and register it to launch at login (or boot).
    [sudo] brew services stop (formula|--all):
    Stop the service formula immediately and unregister it from launching at login (or boot).
    [sudo] brew services restart (formula|--all):
    Stop (if necessary) and start the service formula immediately and register it to launch at login (or boot).
    [sudo] brew services cleanup:
    Remove all unused services.
    If sudo is passed, operate on /Library/LaunchDaemons (started at boot).
    Otherwise, operate on ~/Library/LaunchAgents (started at login).
例如刚刚安装的 mysql
查看被 brew services 管理的服务
brew services list
Name      Status  User  Plist
mongodb   stopped
mysql@5.6 started hardy /Users/hardy/Library/LaunchAgents/homebrew.mxcl.mysql@5.6.plist
rabbitmq  started hardy /Users/hardy/Library/LaunchAgents/homebrew.mxcl.rabbitmq.plist
redis     stopped
zookeeper stopped
复制Name
停止服务：
brew services stop mysql@5.6 
启动服务：
brew services start mysql@5.6 
重启服务：
brew services restart mysql@5.6 
对就这么简单（>_<）
```
###         (5) Homebrew安装常用开发软
```
#安装jdk8,其实和手动下载再安装效果一样,但homebrew一条命令搞定
brew tap caskroom/versions
brew cask install java8
#检测
java -version
#安装
brew install maven
#检测
mvn -v
#安装
brew install git
#检测
git -v
#安装
brew install telnet
#安装redis
brew install redis
#启动
brew services start redis
#检测
redis-cli -v
#安装rabbitMq
brew install rabbitmq
#启动
brew services start rabbitmq
#检测
http://127.0.0.1:15672
......
```
## 三. 其他小东东
###     1. ssh key的生成
        输入如下，没有特殊要求的话，一路回车，就可以github找到ssh，把生成的公钥配置进去
`ssh-keygen`
###     2. ssh 会话克隆
        会话克隆，也就是你在链接同一个服务器时，再次链接根本不用输密码，比如公司需要跳板机才能链接服务，简直是利器啊！(>_-)
```
cd ~/.ssh
vim config
```
        粘贴如下:
```
Host *
ServerAliveInterval 120
ServerAliveCountMax 16
ControlMaster auto
ControlPath ~/.ssh/master-%r@%h:%p
```
###     3. google插件推荐
        （1). WEB前端助手(FeHelper)
![](https://oscimg.oschina.net/oscnet/f2a803ed6bce57bcad10d60e226fc56c098.jpg)
            日常用的json、sql、二维码等等，常用功能，真实谁用谁知道啊
        (2). Utime
![](https://oscimg.oschina.net/oscnet/41c05b900afb0b9c4e23a85dee99dd627d3.jpg)
            直接看效果图
            转换时间戳，好用不、服气不 (>0<)
![](https://oscimg.oschina.net/oscnet/d6ee68ddd1ca83b5d3e6291bb72d0b5f660.jpg)![](https://oscimg.oschina.net/oscnet/241275b7f9bcaa02dcfb9780620b9c590f9.jpg)![](https://oscimg.oschina.net/oscnet/593483298d396db07f85ccee673d2b21b0e.jpg)
            时间戳格式化 怎么样
![](https://oscimg.oschina.net/oscnet/fdc06bb20c307c1be407f381733c55d02da.jpg)
        (3). One Click Timestamp
            当地想或去当前时间戳怎么办，对就是它，仅仅需要做的是 点一下，没听错，都不用你复制，直接拿去粘贴 (0_0)
![](https://oscimg.oschina.net/oscnet/5c9604097ef86083f600abdaba425df41d3.jpg)
        (4). User-Agent Switcher for Chrome
            这个货可以让你自定义User-Agent，作为程序员不要问我UA干吗的。
![](https://oscimg.oschina.net/oscnet/bf160f7cfd78b2993ce9b56360962e79170.jpg)
###     4. 编辑器推荐
        sublime text3  老生常谈了，基本大家都会装吧！虽然你可能不是大家的一份子！！！
        package control安装：
            ctrl* + *` 或者 View *>* Show Console  然后粘贴如下:
`import urllib.request,os,hashlib; h = '6f4c264a24d933ce70df5dedcf1dcaee' + 'ebe013ee18cced0ef93d5f746d80ef60'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); print('Error validating download (got %s instead of %s), please try manual install' % (dh, h)) if dh != h else open(os.path.join( ipp, pf), 'wb' ).write(by)`
        回车等待安装完成，然后重启
        shift + command + p
![](https://oscimg.oschina.net/oscnet/a1540871e310ec671da929e860aa93dae1d.jpg)
        选择完回车，输入：ChineseLocalizations, 找到了回车，等待安装完成重启。好的这回是中文了吧。
        再推荐一个，如果你写python，作为脚本使用，快轻，选 vs code吧！值拥有，当然写别的也是可以的，插件推荐这两个
![](https://oscimg.oschina.net/oscnet/b9fe07946059e55d063e780b3fd8f399712.jpg)
        python 配置
![](https://oscimg.oschina.net/oscnet/a5cf9617ed4630d40873cde4b9511917f82.jpg)
![](https://oscimg.oschina.net/oscnet/bcba229d2eb6a4036e7c42b1c35a094c42c.jpg)
![](https://oscimg.oschina.net/oscnet/c943c360569a17438580f24c891800ba519.jpg)
        如下:        
```
{
    "editor.formatOnSave": true,
    "editor.rulers": [
        250
    ],
    "workbench.colorTheme": "Visual Studio Dark",
    "python.pythonPath": "/usr/local/bin/python",
    "python.linting.maxNumberOfProblems": 500,
    "python.formatting.autopep8Args": [
        "--max-line-length=250"
    ],
    "editor.minimap.enabled": false,
    "window.zoomLevel": 0,
}
```
        重启 KO!
