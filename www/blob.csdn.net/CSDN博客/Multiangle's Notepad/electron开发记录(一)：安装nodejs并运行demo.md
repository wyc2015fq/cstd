
# electron开发记录(一)：安装nodejs并运行demo - Multiangle's Notepad - CSDN博客


2016年11月26日 11:14:59[multiangle](https://me.csdn.net/u014595019)阅读数：7550



---一些个人的唠叨(着急的人可以跳过)

---很久之前就想写图形化界面应用了，正巧最近又有一项任务需要做这个，正好实现之前的愿望。挑挑拣拣的，看了看wpf，好像通用性不够啊，还不能跨平台；再看看qt，太丑了；看来看去看中了electron, 本质上就是个能够与本地交互的网页。界面可以用别人的js库，自然不用担心不够酷炫，正适合写轻量化的应用。就是它了！再看了看网上关于electron的资料不是很多啊，那我就把我的开发经历写下来吧，希望能帮到人。

---正巧微软家的VS Code也刚发布了一段时间，也是用electron写的，而且能够在上面直接运行node程序，挺方便。所以这次就不用以前一直用的idea了

---，改用 VSCode+electron+node的组合

---。虽然这三样我只有node之前学过一些，其他的都是新玩意，不过嘛，多多尝试，多踩踩坑，总是没坏处的。当然了，初次接触，概念上理解有偏差也是难免的，希望各位看官能够指正。

---内容速览:

---1.nodejs的安装

---2.下载并运行demo

---nodejs安装

---electron是nodejs中的一个包，所以想用electron，nodejs是必须要安装的。

---去nodejs官网

---[https://nodejs.org/en/](https://nodejs.org/en/)

---下载nodejs的安装包。记住要去英文官网，不要去中文官网！中文官网的稳定版是4.4.4，最高版本也不过6.2.0 而英文官网的推荐版本都已经6.9.1了。我一开始去中文网下的，装的包各种不对，坑死我了。

---下好安装包以后，使用tar -xvf解压(如果你下的是tar文件的话)， 然后放到你想安装的位置。

---$ tar

----xvf

---node

----v6

---.9

---.1

----linux

----x64

---.

---tar

---.

---xz
$ cp

----r

---node

----v6

---.9

---.1

----linux

----x64

---~/software/nodejs

---这样你就把安装包放在了~/software/nodejs下。 接下来将node和npm的路径写入环境变量。npm是nodejs的包管理软件，类似于apt之于linux, pip之于python, maven之于java

---$

---gedit ~

---/.bashrc

---打开~/.bashrc文件，往最后写入

---export NODEJS

---=

---~/software/nodejs/node

----v6

---.9

---.1

----linux

----x64

---/bin
export PATH

---=

---$PATH

---:

---$NODEJS

---保存，关闭，在shell中输入

---$

---source

---~/.bashrc

---让环境变量立即生效。

---当然了，最好再设置下npm的源，默认的软件源在国外，速度忒慢，国内有淘宝家的，还不错～

---$

---gedit ~

---/.npmrc

---往其中写入

---registry=https://registry

---.npm

---.taobao

---.org

---即可

---下载并运行demo

---这边就使用electron官网的例子。这里就先不讲demo的结构了，以后会讲。这边只讲怎么运行起来。(其实只要照着官网来就行了哈哈)

---\# Clone the Quick

---Start

---repository
$ git clone https://github.com/electron/electron-quick-

---start

---\#

---Go

---into

---the repository
$ cd electron-quick-

---start

---\# Install the dependencies

---and

---run
$ npm install && npm

---start

---现在你已经能打开应用了

![选区_044.png-14kB](http://static.zybuluo.com/multiangle/99sx4t401lk34vd4xt8h85nt/%E9%80%89%E5%8C%BA_044.png)

