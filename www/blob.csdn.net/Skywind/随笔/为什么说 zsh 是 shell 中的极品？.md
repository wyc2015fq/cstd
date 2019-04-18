# Skywind Inside » 为什么说 zsh 是 shell 中的极品？
## 为什么说 zsh 是 shell 中的极品？
March 7th, 2018[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
（这是之前我在知乎上回答的一个可能对大家有点用处的答案，关于 zsh 的一些演示）
### 色彩高亮
并不是传统基于正则表达式的色彩高亮，而是真的会判断你输入的是啥的色彩高亮：
![](https://skywind3000.github.io/word/images/linux/zsh-1.png)
白色代表普通命令或者程序，红色代表错误命令，这个很管用，你再一个个字母的敲命令，前面都是红色的，如果敲对了最后一个字母的话，你会看到整条命令连着前面的都变成了白色，代表你敲对了。以前无高亮的时候敲错了都不知道，还要往上翻着左右检查。下面青色的代表内建命令或者 alias （echo 和 ls ），这些都不是正则判断出来的，是真的去检查的。
细心的人会发现非零的错误码，也会高亮显示在最右边（上一条 data命令错误，返回127）。
### 命令提示
注意，命令提示和补全是两个完全不同的系统，很多时候提示比补全更有用：
![](https://skywind3000.github.io/word/images/linux/zsh-2.png)
你才输入完 “tar”命令，后面就用灰色给你提示 tar 命令的参数，而且是随着你动态输入完每一个字母不断修正变化：
![](https://skywind3000.github.io/word/images/linux/zsh-3.png)
比如你输入到 – 后，没有跟着它上面的提示，而是输入了一个c字母，它马上明白你是要压缩，不是解压，然后随即给出你压缩对应的命令提示。
这个命令提示是基于你的历史命令数据库进行分析的，随着你输入的命令越来越多，提示将会越来越准确和顺手，某些不常输入的命令特别管用，比如偶尔查看下网卡配置：
![](https://skywind3000.github.io/word/images/linux/zsh-4.png)
刚输入完：cat /etc/n 它后面已经猜出你可能要查看网卡配置了，然后马上给出你提示，用不着你 tab 补全半天，你才敲 gc ，它就猜测出你可能想运行 gcc，然后马上给出完整建议：
![](https://skywind3000.github.io/word/images/linux/zsh-5.png)
如果你觉得它提示的正确，你可以 CTRL+F 表示采纳，后面就会自动帮你一次性全部输入完了，不用一个字一个字的照着敲。前面的高亮就不说了，用惯这套提示系统，你就再也难以回到光秃秃的 bash 时代了。
### 智能补全
传统 shell 的补全在 zsh 面前基本都可以下班了：
![](https://skywind3000.github.io/word/images/linux/c-1.png)
即便可以在终端下舒适工作的人，面对有些任务也会觉得烦躁，比如频繁的切换路径，这种缩写路径补全是我用 zsh 的一大痛点之一，特别是路径比较长的时候，比如 OS X 下工具链层层套的那种路径，比如某 java 代码树，有了这种缩写补全，能让你切换路径流畅不少。
当补全内容较多时，不用像 bash 一样持续提示你需要继续输入，也不会像 cmd 永无止境的循环下去，连续敲击两次 TAB 键 zsh 给你一个补全目录，让你上下左右选择：
![](https://skywind3000.github.io/word/images/linux/c-2.png)
这叫选择模式，由两次连续 TAB 进入，进入后，除了 tab/shift+tab 可以前后切换外，你还可以使用光标键上下左右移动，或者使用 emacs 键位：ctrl + f/b/p/n (左右上下：forward, backward, previous, next) 。如果你觉得光标键太远难按，CTRL+f/b/p/n 太伤小拇指，可以跟我一样新定义出一套：ALT+hjkl （左下上右）来选择，十分顺手。回车表示确认选择，用 CTRL+G 表示退出。
命令参数补全更不在话下，输入 svn 后面按 TAB：
![](https://skywind3000.github.io/word/images/linux/c-4.png)
就出现了 svn 的参数，这种一级参数补全基本只会对很少用的命令才有效果，svn/git 这种一级参数基本都不需要补全的，我们一般会需要到二级参数补全，比如已经输入了 svn commit，但是有一个 commit 的参数我忘记了，我只记得两个减号开头的，于是：
![](https://skywind3000.github.io/word/images/linux/c-3.png)
这时候两次 TAB 进入选择模式就比较管用了，svn 的二级参数往往很长，选择模式比如信任 server 那个，选择完回车确认，或者 CTRL+G 退出选择模式。
zsh的补全真的太强了，我这里只说了十分之一不到，没法一一展开了，但就上面几个已经让我有充分的理由切换到 zsh 了。
### 快速跳转
前面也说过命令行工作中，不同的路径间切来切去是个头疼的问题，除了上面提到的缩写补全外，有无更快的办法让我马上切换到我最近跳转过的某个路径？当然有“cd -”命令：
![](https://skywind3000.github.io/word/images/linux/cd-1.png)
输入 cd 后面加一个减号后，按一次 tab 马上就列出本次登陆后去过的最近几次路径，接着根据下面的提示输入数字按回车就过去了，比如输入：
```text
$ cd -5 <回车>
```
就跳转到 ~/software/libclang-python3 路径下了。当然你还可以不输入数字，而是再按一次 tab 进入选择模式，上下键或者 ctrl+n/p 来选择，回车确认，ctrl+g 返回。
### 自动跳转
有了前面的路径缩写展开，和这里的最近访问路径切换，你已经没法再回到过去那种按部就班输入路径外加点弱智补全的方式了，但是可能你还会问，能否更进一步，不限于本次登陆或者最近去过的几级路径，有没有办法让我快速进入自我开始用 zsh 之后进入过的某个路径呢？当然可以，我们用 z 命令，查看历史上进入过的目录：
![](https://skywind3000.github.io/word/images/linux/z-1.png)
敲入 z 命令，列出了自从我开始用zsh进入过的目录和他们的权重，进入次数越多，权重越大，便于演示，我删除了我的历史，随便 cd 了一下，保持列表的简洁。z 后面加一个关键词就能跳转到所有匹配的历史路径中权重最高的那个了：
![](https://skywind3000.github.io/word/images/linux/z-3.png)
比如所有历史路径都包含 o ，那么 z o 就会跳转到权重最高的 ~/software 目录中。使用：“z -l foo” 可以列出包含 foo 的所有历史路径：
![](https://skywind3000.github.io/word/images/linux/z-4.png)
比如我们查询包含关键字为 c 的所有历史路径和他们的权重，有时你搞不清楚权重，可能会跳转错了，比如有两个路径：
```text
project1/src
project2/src
```
那么你 z src 的时候可能并不能如你愿跳转到你想要去的路径，那怎么办呢？第一个办法是实际 cd project1/src 过去，增加它的权重，权重超过 project2/src 那么下次 z src 的时候就会跳转过去，你可以实时用 z -l src 查看包含 src 的所有路径权重。
更加可靠的方法是，增加一个关键字，比如 z 1 src ，空格分隔多个关键字，z会先匹配出第一个来，比如1 ，然后再匹配第二个 src ，马上锁定 project1/src 了。大家实际使用起来，一般是 z + 最后一级目录名，比如：
```bash
$ z vim     # -> /home/skywind/software/vim
$ z tmp     # -> /home/skywind/tmp
$ z local   # -> /home/skywind/.local
```
99%的时候这样做就足够了，当没有按照你要求跳转的时候，你可以再补充一下再上一级目录的一些信息，比如 z vim/src 或者 z v src 都可以，弄不明白会跳转到哪里，可以随时用：
```bash
$ z -l key1 [key2 ... ]
```
查看权重。不过常使用你根本必担心这个问题，基本上常去的地方，z 都是指哪打哪。如果说前面的路径缩写展开和最近访问快速切换是火箭的话，z 就是加速燃料了。
熟练的掌握上面几点内容，可让你体验到在终端里溜冰的感觉，再也没有泥里走路的抓狂。
### 热键绑定
zsh 里面使用 bindkey 命令可以设置一系列热键，用来运行某一个 zsh 内部命令或者某个 shell 命令，谁规定终端只能敲字母呢？我们还可以按热键，比如从网上下载了一个 tar 包解开后要稍微浏览一下里面的内容，用的最多的两条命令是啥呢？第一条是 ls 命令，每到一个子目录都要先按一下，还有就是 cd .. 对吧，经过配置：
```bash
bindkey -s '\eo'   'cd ..\n'    # 按下ALT+O 就执行 cd .. 命令
bindkey -s '\e;'   'ls -l\n'    # 按下 ALT+; 就执行 ls -l 命令
你还可以设置一键打开编辑器，或者一键帮你输入某常用命令的一部分。除了这些命令外，日常命令编写也可以加强一下：
bindkey '\e[1;3D' backward-word       # ALT+左键：向后跳一个单词
bindkey '\e[1;3C' forward-word        # ALT+右键：前跳一个单词
bindkey '\e[1;3A' beginning-of-line   # ALT+上键：跳到行首
bindkey '\e[1;3B' end-of-line         # ALT+下键：调到行尾
```
敲命令时经常需要对已有命令进行修改，默认一个字符一个字符的跳太慢了，这样设置以后基于单词的跳转快速很多，配合其他一些快捷键，修改命令事半功倍。
终端下从  v220t 到 xterm 规范里，按下 alt+x 会先发送一个8位 ASCII 码 27，即 ESC键的扫描吗，然后跟着 x 这个字符，也等价于快速（比如100毫秒内）前后按下 ESC 和 x。
还不会再自己的终端软件里设置允许 alt 键的同学们可以搜索下相关文章。
### 如何配置？
zsh 有多强呢？上面说的这些和我平时用的功能可能只发挥了 zsh 10% 不到的能力，我也并不是什么 zsh 专家或者脚本高手，上面所讲的五点内容对于 zsh 的全部功能来讲可能都只用到了 zsh 的九牛一毛，但是以上五点只要有一条，就已经够我放弃其他 shell 来尝试一下了。
那最后上面这些功能怎么配置的？眼熟的人应该发现这不是默认的 oh-my-zsh 框架，这只是我写的一个 100 多行的 .zshrc 小脚本，如果你想体验一下的话，可以先 apt-get 安装一下 zsh，然后打开：
https://github.com/skywind3000/vim/blob/master/etc/zshrc.zsh 
把上面这个配置的内容复制粘贴到你的 ~/.zshrc 文件里，保存，运行 zsh 即可。头一次运行会安装一些依赖包，稍等两分钟，以后再进入就瞬间进入了。
PS：想自己配置到话，推荐使 zsh 的包管理器：antigen 来管理所有功能，用它配置起来比原始 oh-my-zsh 自动化多了。
看很多人都比较迷恋 zsh 的 git prompt ，我从来不用这华而不实玩意儿，让我的终端不流畅，每次没内容按下回车都要调用一大堆命令，建议大家关闭。
相关阅读：
[韦易笑：有哪些命令行的软件堪称神器？](https://www.zhihu.com/question/59227720/answer/286665684)
[终端调试哪家强？](http://www.skywind.me/blog/archives/2036)
[Bash 中文速查表/Cheatsheet （全网最全）](http://www.skywind.me/blog/archives/2071)
