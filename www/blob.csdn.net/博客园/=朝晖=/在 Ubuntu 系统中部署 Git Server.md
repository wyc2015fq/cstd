# 在 Ubuntu 系统中部署 Git Server - =朝晖= - 博客园
# [在 Ubuntu 系统中部署 Git Server](https://www.cnblogs.com/dhcn/p/7120910.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/poisonchry/article/details/11849781
虽然有很多开源的[Git](http://lib.csdn.net/base/git)仓库，不过并非所有都尽人意，譬如Github，Gitlab等，不是服务器不稳定，就是强制开源，又或者有文件大小，项目数量等方面的限制，我们认为[git](http://lib.csdn.net/base/git)服务提供商设置的这些限制是合理的，是无可指责的，但从实际出发，这些免费仓库并不一定都符合所有开发者的需求。或许对于个人开发者或者小团队开发组来说，这些开源仓库还是非常有用的，至少在减少成本方面有不可取代的地位，但也正如我之前所说，并非每一个开发者都认同这些，幸好作为一个优秀的CVS（control version system），除了具备分布式开发的特点外，还兼具有传统CVS的功能。
也就是说，我们可以在本地创建Git Server，用于保存和维护我们自己的代码。在您继续阅读以下内容前，需要说明的是，这些内容是基于[Linux](http://lib.csdn.net/base/linux)发行版中最流行之一的Ubuntu，当然也可以用于Debian[操作系统](http://lib.csdn.net/base/operatingsystem)，不过除此之外的其他[linux](http://lib.csdn.net/base/linux)系统，甚至是Unix系统或者是Windows系统，作为使用者的您可能需要参考相关文献及文档，有些命令可能不一样，不过总体思路应该是一样的。
## 准备工作
首先，你需要更新你的包文件，包括更新你的系统，Git或者某些组件可能需要一些新的特性以支持，不过在Ubuntu系统中，这一步相当简单，你只需要在Terminal中输入
```
sudo apt-get update
```
完成这一步后，我们就可以安装 git core 组件，通常情况下，我们安装在系统中的git 只是一个简单的客户端，包括一些最基本的命令以及特性，而作为保管代码的仓库，则还需要其他特性支持，所以我们需要安装 git core，如果你之前已经安装了 git，那么仍然需要安装 git core
```
sudo apt-get install git-core
```
完成这两步之后，我们就可以开始配置 Git Server 了。
## 增加 developers 组以及创建保存项目的仓库
我们需要创建一个新的用户组，并且将创建的仓库都放置在名为 git 的用户下，当然git 要隶属于 developers 组，这样也是为了方便将来的管理需要。
```
sudo groupadd developers
cd /home/
sudo mkdir git
sudo useradd git -d /home/git
```
以上四个命令分别为我们创建 developers 用户组， 并在/home/ 文件夹下为名为 git 的用户创建工作目录，创建名为 git 的用户账户并将它的工作目录指向/home/git。
之后，我们需要将增加的用户添加到 developers 用户组中，这一步一般通过修改/etc/group 文件达成，当然在正式修改 /etc/group 文件前或许需要先做一个备份。找到你创建的用户组，譬如这里的例子中创建的 developers。你或许能在文件中看到类似的一行：
```
developers:x:1003:
```
将刚才创建的用户 git，添加到developers中，当然你也可以添加其他的“开发者”，就像这样
```
developers:x:1003:git,tom,francklin,james
```
创建代码仓库，并修改权限。现在，让我们回到创建好的工作目录 **/home/git/**，你可能需要通过命令cd 来完成将工作路径由 /etc/ 切换为 /home/git/，然后让我们在 git 下创建一个新的仓库
```
sudo mkdir yourproject.git
sudo chgrp developers yourproject.git
sudo chmod g+rws yourproject.git
```
我们需要将创建的文件夹的所有人由 root 修改为 developers，这样当我们在本地上传代码至仓库时就不会遇到权限不够的问题。其次我们需要将仓库的所有权修改，你也可以用“777”修改仓库的权限，不过不建议你这样做。
```
sudo git init —bare —shared yourproject.git
```
在正式上传代码至仓库前，还需要对仓库进行初始化，请注意这里和平常使用方式的不同，—bare 指定创建的仓库为公共仓库，—shared 将使其他人都有权限将他们的代码提交至这个仓库中。
## SSH
题外话：有朋友@我说这部分说的不够详细，有些云里雾里。因此我决定修改本章的内容，力求各位能尽可能的看得懂。
首先，我们使用GIT的原因，是希望能够分布式的管理代码，即便因为主仓库受损，也不希望影响到本地的代码，或者说张三因为电脑中毒了，导致代码丢失，依然能够通过其他人，或者代码仓库恢复代码。通常，提交代码至代码仓库有四种方法：
- 本地提交
- 通过HTTP提交代码
- 通过SSH提交代码
- 通过FTP提交代码
本地提交的意思是，你的代码仓库就存放在本地硬盘或某个文件夹，你可以不借助任何其他工具而直接访问你的仓库。因此基于这样一种情况，你可以完全无视本节内容，你只需要在本地硬盘上开辟一块空间，创建对应的代码仓库即可。
但是当你的代码需要联网才能提交到仓库的时候，你就需要考虑 HTTP、FTP、SSH 中的其中一种方式提交你的代码。这里，我推荐使用SSH，其一、提交代码的时候不需要输入密码，其二、适用于绝大多数情况，其三、使用SSH提交代码的方式也最为容易实现。
使用SSH进行提交代码，你需要在GIT Server上先安装SSH服务器，如果你使用的系统是Ubuntu/Debian的话，可以使用 apt-get 安装 SSH 服务器。
```
sudo apt-get install openssh-server
```
如果是CentOS系统，那么也可以使用 yum 安装 SSH 服务器，不过Linux企业版系统，一般默认有安装 SSH 服务器，所以你可以无视这一步。在服务器上安装好 ssh 之后，可以使用 ssh localhost 试着登录一下。在你没做其他配置的情况下，一般会要求输入用户名和密码，之后会提示登录成功。你可以在登录成功后，使用 **ls -l** 或者 **ll** 命令查看当前工作路径下有哪些文件和目录，之后，你需要使用 **exit** 退出 ssh 登录。
> 
在本地客户端上，你可能需要创建隶属于 developers 组的用户，当然这一步并非必须。
```
$ sudo groupadd developers  # 创建developers用户组
$ sudo mkdir -p /home/git   # 创建主目录
$ sudo useradd git -d /home/git -g developers # 创建git用户，并制定其默认的工作路径和隶属的用户组
$ sudo passwd git  # 为git设置密码
```
完成这一步后，我们需要为客户端设置 ssh 免登录，所谓 ssh 免登录指的是从客户端登录服务器端，不需要输入用户名和密码，如果你觉得这一步很麻烦的话，那么大可以无视这一节的内容。首先，将当前的用户从管理员用户切换为 git，并使用如下命令：
```
ssh-keygen -t rsa # 使用RSA协议，生成一对公私钥，过程中出现的提示，一路回车即可
ssh-copy-id -i ~/.ssh/id_rsa.pub git@192.168.1.100 # 将生成的公钥发送至位于192.168.1.100的设备上（GIT 服务器）
```
这个过程可能会要求你输入密码，如果发现发送失败了，那么请检查用于登录192.168.1.100的用户名和密码是否正确。另外需要知道的是，如果创建了用户而没有指定密码，那么当你试图登录设备的时候一般会失败。如果是后者的原因，那你需要用管理员的账户，登录到系统，并使用 passwd username 为用户指定一个密码，比如(以Ubuntu系统为例)：
```
$ sudo passwd git # 为用户git指定密码为1234
```
之后你再次尝试一次，那么就会成功的把公钥发送给 GIT Server 了。你可以使用 ssh git@192.168.1.100 试一试，看看能不能免密码登录至 GIT 服务器上。​
## 代码仓库
完成这些后，就可以上传自己的代码、或者从 Git server 上拉取代码了。
```
git remote add origin git@x.x.x.x:/home/git/yourproject.git
git push origin master
```
## 可能出现的 Error
如果出现了类似的错误
> 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ WARNING: POSSIBLE DNS SPOOFING DETECTED! @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
The RSA host key for **.net has changed,and the key for the according IP address xx.xx.xxx.xxxis unknown. This could either mean thatDNS SPOOFING is happening or the IP address for the hostand its host key have changed at the same time.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED! @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!Someone could be eavesdropping on you right now (man-in-the-middle attack)!It is also possible that the RSA host key has just been changed.The fingerprint for the RSA key sent by the remote host isxx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx.Please contact your system administrator.Add correct host key in /Users/user/.ssh/known_hosts to get rid of this message.Offending key in /Users/user/.ssh/known_hosts:5RSA host key for **.net has changed and you have requested strict
checking.
Host key verification failed.
处理方法是，删除客户端的 .ssh/known_hosts 文件。

