# node.js安装与入门使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月30日 09:57:32[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：114
一个基于 [Chrome
 V8](https://developers.google.com/v8/) 引擎的 JavaScript 运行环境。
Node.js 的包管理器 [npm](https://www.npmjs.com/)，是全球最大的开源库生态系统。
提供[事件驱动](https://zh.wikipedia.org/wiki/%E4%BA%8B%E4%BB%B6%E9%A9%85%E5%8B%95)和[非阻塞I/O](https://zh.wikipedia.org/w/index.php?title=%E9%9D%9E%E9%98%BB%E5%A1%9EI/O&action=edit&redlink=1)[API](https://zh.wikipedia.org/wiki/API)，可优化应用程序的吞吐量和规模。这些技术通常被用于[实时](https://zh.wikipedia.org/wiki/%E5%AE%9E%E6%97%B6)应用程序。
[https://nodejs.org/en/download/](https://nodejs.org/en/download/)
官方有docker镜像
先安装一个 nvm
wget -qO- https://raw.github.com/creationix/nvm/master/install.sh | sh
或
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.25.2/install.sh | bash
nvm 的全称是 Node Version Manager，之所以需要这个工具，是因为
 Node.js 的各种特性都没有稳定下来，所以我们经常由于老项目或尝新的原因，需要切换各种版本。
安装完成后，你的 shell 里面应该就有个 nvm 命令了，调用它试试
$ nvm
nvm install 4.4.7
或
打开一个新的终端运行：
$ nvm install stable
安装完成后，查看一下
$ nvm ls
这时候可以看到自己安装的所有 Node.js 版本
绿色小箭头的意思就是现在正在使用的版本
告诉 nvm 你要使用 0.12.x 版本
$ nvm use 0.12
在终端中输入
$ node
上述过程完成后，有时会出现，当开启一个新的 shell 窗口时，找不到 node 命令的情况。
检查 ~/.profile 或者 ~/.bash_profile 中有没有这样两句
export NVM_DIR="/root/.nvm"[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh" # This loads nvm
没有的话，加进去。
source ~/.bash_profile
调用
$ nvm ls
看看像不像上述图1中一样，有 default 的指向。
如果没有的话，执行
$ nvm alias default 4.4.7
再
$ nvm ls
看一下
node -v
