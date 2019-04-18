# Ubuntu小工具 - xcw0754 - 博客园
# [Ubuntu小工具](https://www.cnblogs.com/xcw0754/p/6306903.html)
### 更好的工具
更多的界面风格： `https://github.com/anmoljagetia/Flatabulous`
更丰富的终端zsh： `https://github.com/robbyrussell/oh-my-zsh`
### 更快的源
源替换：
```
第一步：$sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak #备份
第二步：$sudo vim /etc/apt/sources.list  # 改一下里边的内容
第三步：$sudo apt-get update #更新列表
```
针对不同的ubuntu版本，需要的源是不一样的，一般国内高校，大厂都有源，自己上官网找找。
### ssh连接
`~/.ssh/config`文件可以设置快捷的ssh连接方式，下面框的配置的效果是`$ssh 801`就可以直接连上去了。
```
Host 801
     HostName 192.168.0.1
     Port 8087
     User xcw0754
     PubkeyAuthentication yes
     IdentityFile ~/.ssh/id_rsa
```
如果已经ssh打开了一个终端，想要再开一个连接，为了这样的效果：终端两个tab共享一条连接，避免再次连接时输入密码。可以在`~/.ssh/config`文件中加入：
```
ontrolMaster auto
ControlPath /tmp/ssh_mux_%h_%p_%r
```
### 终端
终端终端字体：Ubuntu Mono derivative Powerline| 13 (不使用系统等宽字体)
终端背景颜色：#000000
终端文本颜色：#CB96C5
### vim及zsh
vimrc和zsh主题`https://github.com/xcw0579/vimrc.git`
zsh插件 指令自动补全：`https://github.com/zsh-users/zsh-autosuggestions`
zsh插件 目录模糊切换：`https://github.com/wting/autojump`

