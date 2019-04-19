# Cheat—— 给Linux初学者和管理员一个终极命令行"备忘单" - 文章 - 伯乐在线
原文出处： [Avishek Kumar](http://www.tecmint.com/cheat-command-line-cheat-sheet-for-linux-users/)   译文出处：[su-kaiyao](http://linux.cn/article-3760-1.html)
当你不确定你所运行的命令，尤其是那些使用了许多选项的复杂命令时，你会怎么做？在这种情况下，我们使用man pages来获取帮助。还有一些其它的选择可能包括像‘**help**’，‘**whereis**’和‘**whatis**’这样的命令。但是所有的这些既有优点，也有缺点。
![](http://jbcdn2.b0.upaiyun.com/2016/01/08f524826aed3a997aefab94d084eb0f.png)
当我们浏览**man pages**来查看选项和帮助的时候，里面的描述实在太冗长了，我们无法在短的时间里理解它的意思。
![Linux Man Pages](http://jbcdn2.b0.upaiyun.com/2016/01/d5d2619af25cf69718fd36d784e0e56b.jpeg)Linux Man Pages
同样，‘**help**’命令可能也不会给你期待的答案。
![Help Command](https://camo.githubusercontent.com/96250682158db1bb3c9889531d4f8589e109c1fd/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f68656c702d636f6d6d616e642e6a706567)Help Command
‘**whereis**’命令几乎不给你任何信息，除了安装二进制文件的位置（有些时候可能是重要的）
![Whereis Command](https://camo.githubusercontent.com/f94b1fe991d7b95fd99a505c53baebe955ea71ad/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f776865726569732d636f6d6d616e642e6a706567)Whereis Command
‘**whatis**’命令给出一套很严格的答案，它除了说出所查询命令的作用，并没有什么太大帮助。而且，它从来不说明可用的选项。
![Whatis Command](https://camo.githubusercontent.com/d1f2b2d8a830108b1d8c38650649b6d2fbc4b1c0/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f7768617469732d636f6d6d616e642e6a706567)Whatis Command
直到在困难中解决问题前，我们已经使用了以上全部选项，但是现在来了一个交互式的备忘录应用程序‘**cheat**’，它将在其余的命令中脱颖而出。
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)什么是cheat?
**cheat**是在GNU通用公共许可证下，为Linux命令行用户发行的交互式备忘单应用程序。它提供显示Linux命令使用案例，包括该命令所有的选项和简短但尚可理解的功能。
![Cheat: Provides Easy Command Options](https://camo.githubusercontent.com/7b4f156caaa898c7c9c9655143773f7fbd747a61/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f63686561742d73686565742e6a706567)
Cheat：提供简单命令选项
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)在Linux系统中安装‘Cheat’
‘**Cheat**’有两个主要的依赖——‘**python**’ 和 ‘**pip**’，在安装‘**cheat**’之前，确保你的系统安装了python和pip。
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)Install Python

Shell
```
# apt-get install Python (基于Debian的系统)
# yum install python (基于小红帽的系统)
```
Install Pip

Shell
```
# apt-get install python-pip (基于Debian的系统)
# yum install python-pip (基于小红帽的系统)
```
**Note**：pip是一个简单的安装替代，是一个有极大改进的Python第三方包安装器。
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)下载并安装Cheat
我们将通过Git下载‘Cheat’，确保你安装了‘git’包，如果没有最好安装一下。

Shell
```
# apt-get install git (基于Debian的系统)
# yum install git (基于小红帽的系统)
```
接下来，通过运行下面的命令来安装所需要的python依赖包。

Shell
```
# pip install docopt pygments
```
现在，复制cheat的[Git库](http://www.tecmint.com/install-git-to-create-and-share-your-own-projects-on-github-repository/)

Shell
```
# git clone https://github.com/chrisallenlane/cheat.git
```
进入cheat目录，运行‘**setup.py**’（一个python脚本）。

Shell
```
# cd cheat
# python setup.py install
```
如果安装很顺利，你就能够看到安装在系统上的cheat版本了。

Shell
```
# cheat -v
cheat 2.0.9
```
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)cheat的一些必要配置
你必须在‘~/.bashrc’文件里设置‘**EDITOR**’环境变量，打开用户‘**.bashrc**’文件，然后把下面这行加入到里面：

Shell
```
export EDITOR = /usr/bin/nano
```
你可以使用你喜欢的编辑器来替代‘**nano**’。保存文件然后退出，再次登录以确保修改生效。
接下来，添加cheat的自动补全特性，来确保不同解释器下命令行的自动补全。为了到达该功能要求，简单地将‘**cheat.bash**’脚本clone下来，然后复制到你系统正确的路径下。

Shell
```
# wget https://github.com/chrisallenlane/cheat/raw/master/cheat/autocompletion/cheat.bash 
# mv cheat.bash /etc/bash_completion.d/
```
**NOTE**：团队已经在Git上上传了其他解释器的自动补全脚本，这些都可以被克隆，然后用在不同的解释器上。使用下面的链接，以找到其余解释器的自动补全脚本：
- [Auto Completion Script for Various Shells](https://github.com/chrisallenlane/cheat/tree/master/cheat/autocompletion)
不仅如此，如果需要的话，你也可以使用语法高亮。要想做到这一点，在你的‘**.bashrc**’文件中添加‘**CHEATCOLORS**’环境变量。

Shell
```
export CHEATCOLOR=true
```
Cheat应用默认只提供最基本和最常用的命令。cheat备忘单的内容保存在**~/.cheat/.**目录里，我们可以手动在这个目录添加备忘单里面的内容，这样，我们的cheat应用将更强大。

Shell
```
# cheat -xyz
```
这将打开xyz备忘单，如果没有的话就会创建一个。备忘单将使用**.bashrc**里设置的默认**编辑器**打开，默认编辑器就是在上面**.bashrc**所设置的。
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)一些Cheat命令的使用
一个包可以是 *.gz , *.bz2 , *.zip , *.xz，可是什么情况下用哪些选项呢？
![tar command options](https://camo.githubusercontent.com/7952f8f5083052f6b4a776c0d361d0b8af56d629/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f63686561742d7461722e6a706567)tar命令选项
除非在不同的地方咨询和核对后，我才使用**dd**命令，在此之前，无论我对这个命令多么的肯定，我从不使用它。现在，事情就变得简单多了。
![dd command options](https://camo.githubusercontent.com/21cfa4c8f30ce6d2b53bf641979b101c7824ac5e/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f63686561742d64642e6a706567)dd命令选项
‘**uname**’命令帮助
![uname command options](https://camo.githubusercontent.com/6e18d6f64123dca16aea36c090d0766057082720/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f756e616d652d636f6d6d616e642e6a706567)uname命令选择
一个简短的**ifconfig**命令操作教程
![ifconfig command options](https://camo.githubusercontent.com/74d764202dc1d59949d8f6f96716c0e02b665cb0/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f6966636f6e6669672d636f6d6d616e642e6a706567)ifconfig命令选项
‘top’命令，对管理员和普通用户来说，是最重要的命令之一。
![top command options](https://camo.githubusercontent.com/f4862dbe1db99c1adf5b2f2fb169706e8ba4edc5/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f746f702d636f6d6d616e642e6a706567)top命令选项
我们来骗骗cheat命令，如何（尽管别有意义）？得到一个可用命令的列表，其实就是安装在你系统里的cheat备忘录。
![List All Linux Commands](https://camo.githubusercontent.com/5a5ab67a3bd0afaacec1f7d26129a12629d9f376/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f6c696e75782d636f6d6d616e64732e6a706567)列出所有Linux命令
使用关键字搜索备忘单。
![Search Cheat Sheet](https://camo.githubusercontent.com/45360ea72bcf8ccea7d3db000f003dfe6b8a5d4c/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f7365617263682d63686561742d73686565742e6a706567)搜索备忘单
来看看包含所有命令的内置备忘单。

Shell
```
$ cheat -d
/home/avi/.cheat
/usr/local/lib/python2.7/dist-packages/cheat/cheatsheets
```
复制内置的备忘单到你的本地目录。

Shell
```
# cp /usr/local/lib/python2.7/dist-packages/cheat/cheatsheets/* /home/avi/.cheat/
```
### [](https://github.com/LCTT/TranslateProject/blob/master/translated/tech/20140804%20Cheat--An%20Ultimate%20Command%20Line%20)结论
这个超棒的工具在许多情况下担任“救世主”的角色，它给予你需要的信息，信息不冗余，不模糊，相反地，却击中要点。这是每个人都会需要的工具，很简单就能创建、安装，使用和理解，这个工具前途无量。
这个Git仓库已经添加了一个精彩的**gag**，这里我不打算去解释它，而留给各位来解释。
![Linux Gag](https://camo.githubusercontent.com/5e30737c90821b1c4c4fb9d7e2a57d40e4f6570a/687474703a2f2f7777772e7465636d696e742e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f30372f6c696e75782d6761672e6a706567) Linux Gag
好了，文章就要结束了，我会带着下一篇你们喜欢的有趣文章回到这里的，在此之前，要时刻关注我们。别忘了在下面的评论部分给我们留下您宝贵的反馈信息。
