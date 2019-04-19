# 真正从零开始，TensorFlow详细安装入门图文教程 - 零度的博客专栏 - CSDN博客
2017年09月27日 10:06:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：5143
AI这个概念好像突然就火起来了，大比分战胜李世石的AlphaGo成功的吸引了大量的关注，但其实看看你的手机上的语音助手，相机上的人脸识别，今日头条上帮你自动筛选出来的新闻，还有各大音乐软件的歌曲“每日推荐”……形形色色的AI早已进入我们生活的方方面面。深刻的影响了着我们，可以说，这是一个AI的时代。
其实早在去年年底，谷歌就开源了其用来制作AlphaGo的深度学习系统Tensorflow，相信有不少同学曾经对着这款强大的机器学习系统蠢蠢欲动，但虽然有关Tensorflow的教程其实不少，**甚至谷歌官方就在Tensorflow的官网给出了一份详细的教程，也有网站已经将其翻译成了**[**中文**](http://wiki.jikexueyuan.com/project/tensorflow-zh/)**（点击查看）****，但它仍然是有门槛的，尤其是对于对计算机及编程语言了解不深的同学，可能看着页面上凭空给出的代码陷入懵逼。**
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bf8fe82dca.png?imageMogr2/format/jpg/quality/90)
而今天我要做的，就是带着所有这些几乎没有编程基础却很想学习Tensorflow的同学跨过这道坎。告诉你们大家如何准备好使用Tensorflow进行编程所需的一切，以及如何看懂教程上的那些代码所代表的含义，那么废话不多说，我们马上开始。
# 在Linux操作系统下的安装
### 安装环境
Tensorflow的支持列表里是没有Windows的。虽然可以用Docker来实现在Windows上运行，但小问题很多，它支持的最好的还是基于unix内核的系统，如Linux，因此我们最好还是安装一个Linux的环境来运行它。Linux是一款免费的开源操作系统，应用非常广泛，如著名的Android就是基于Linux改进的一款针对手机的操作系统。而对于我们来说，最易于理解的版本就是著名的Ubuntu，点击[链接](http://cn.ubuntu.com/?_ga=1.127623629.1589445120.1465646782)即可去官网下载，正如前面所说，下载和使用都是免费的。
如果你使用的是Mac，那就方便很多了，因为Mac os本身就是一个基于Unix的操作系统，已经搭载了全部安装Tensorflow所需要的组件。所以安装的很多步骤都可以省去，直接进行Tensorflow的安装，不过命令同Ubuntu有所不同。但你也可以看看接下来的教程，对理解那些命令也会有帮助。呃……你说你在Mac上装了Windows？
下载之后，你会得到一个装着Ubuntu系统的.iso文件。有两个办法来处理它，一是直接安装，同电脑现在在用的系统组成双系统（当然你也可以换掉现在的系统，不过我想应该没有人会愿意这么做的吧哈哈）。如果你不想那么大动干戈，也可以通过虚拟机来虚拟一个系统。不过，安装虚拟机对配置有一定要求，毕竟它相当于在你的电脑上同时打开两个系统了——**CPU最好不要低于4代i3（尤其是笔记本，CPU型号的i3/5/7-xxxx的四位数里第一个x就表示它的代数）。内存——注意，内存一定不能低于4g，因为你给虚拟机分配的内存在虚拟机启动之后会1:1的从你的物理内存中划走。**再加上物理机系统的消耗。当然，这个配置表是只有下限没有上限的，你的电脑配置越好，体验就越好。而装双系统的话，对配置的需求就小很多了。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bf9580f83b.png?imageMogr2/format/jpg/quality/90)
虚拟机安装中
网上相关的教程都比较多，在这里就不重复了，注意搜索安装双系统教程的时候要加上你现用的系统和你下载的Ubuntu系统的版本号。这里只讲几个需要注意的地方。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bf9729f9da.png?imageMogr2/format/jpg/quality/90)
因为Linux对安全性非常重视，因此不仅权限管理十分严格，在创建操作系统的时候也会要求你一定要输入一个密码，这个密码将用来登陆和获得root权限。就比如上面这个界面，如果你不把所有空格都填好是没法点下一步的。
### Python
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bf9c86c5f8.png?imageMogr2/format/jpg/quality/90)
Ubuntu桌面的样子，（悄悄说一句笔者觉得基于Unix的系统普遍比Windows好看太多，主要是字体看着真舒服）
安装好之后，我们看到了一些熟悉的软件和一些不熟悉的软件，我们先不去管它，介绍一下Python。Python是一种高级语言，它的特点是程序很简洁，但是因为简洁，所以在将其翻译成CPU可以理解的指令——也就是执行代码的时候速度会比那些低级一些的语言，比如C语言慢一些。不过在现在电脑的性能面前，这种速度的差别大多数时候根本不足挂齿。
### 解释器
很多人可能会觉得Python很抽象，因为提到Python，大家不会像C语言那样马上想起它有一个专用的编译器。Python可以执行的地方千奇百怪。甚至可以很方便的在系统的Terminal（一个类似于Windows里的命令行的工具）里直接执行。Ubuntu自带了Python，因此一般不用烦心安装事宜。
**Tensorflow支持多种前端语言，但对Python的支持是最好的，因此我们的教程也基于Python来进行**，首先我们打开Terminal。作为Ubuntu中非常重要的一个组成部分，有很多种方法可以打开它。比如在桌面上点击右键
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfa5d02baa.png?imageMogr2/format/jpg/quality/90)
也可以点击左上角的那个Ubuntu图标搜索。打开后可以看到Terminal窗口如下所示。标题栏里@符号前面是你的用户名。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfa6aad116.png?imageMogr2/format/jpg/quality/90)
如果你在你的计算机使用史中从来没有离开过图形用户界面（就是有一个鼠标和很多图标让你点的那种）。看到这个简单得吓人的窗口不要慌，只要输入正确的命令就好了。
在正式开始之前我想说：**一定要注意窗口给出的提示（英文）。在实际操作中可能会碰到各种各样的问题，但常见的问题其实都可以根据它的报错信息找到原因，只要上网搜一搜相应的信息就能解决，甚至它自己就会给出解决的建议。**如果你发现你的步骤一切正确，但就是无法成功，多试几次就好了，毕竟这些服务都是谷歌提供的，而谷歌……你懂的。
### 必不可少的python-pip和python-dev
#### 在这个窗口中输入命令：
#### $ sudo apt-get install python-pip python-dev
注意，窗口中其实已经有一个$符号了，因此你在复制或者输入命令时其实不用将这个$符号也包括进去，但是网上的教程给出命令的时候一般是会把这个符号包括进去的，作为一个标志。注意一下就好。
命令最开始的sudo意味着这条指令将以root权限执行，所以需要输入你最开始设置的那个密码，注意输入过程在屏幕上是不会有任何体现的。输完直接按回车就行。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfafd8a8d7.png?imageMogr2/format/jpg/quality/90)
输入后会出现一串代码，然后问是否继续，输入y回车
然后可以静待安装完成，**解释一下这条命令：**apt-get是从软件仓库中获取软件的一条命令，而软件仓库是Linux各大发行版的共有特征：它是一系列存放软件的服务器或网站，包含了软件包和索引文件，用户可以很轻松的使用命令自动定位并安装其中的软件，而省去了到处搜索的麻烦。install则是安装指令，而python-pip和python-dev则是两个需要安装的软件包的名字。pip可以理解成一个比较高级的软件安装器，安装Tensorflow要用到，而dev则是一个额外的类库，也是Tensorflow的安装和运行需要的。如果一切顺利，在大串英文划过后，我们会看到如下的界面：
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfb120c0f4.png?imageMogr2/format/jpg/quality/90)
### 此时就可以安装Tensorflow了，指令如下：
$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
就像前面所说的，你可能会碰到这样那样的问题，比如下图这样的
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfb244bd52.png?imageMogr2/format/jpg/quality/90)
在这些时刻，不要慌张，仔细阅读错误信息，越往后的通常越重要，因为结论都是在最后给出的，像这个的最后：
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfb3b6be5d.png?imageMogr2/format/jpg/quality/90)
你看，它不仅告诉了你应该试着做什么，连怎么做都告诉你了。按它说的做就好
不过其实当时笔者用的并不是8.1.1版本，使用升级命令后也只是升级到了8.1.1版本，再次输入相同命令，它会告诉你你的版本已经是最新的了。但问题确实解决了，这条提示在之后没有再出现。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfb86c021c.png?imageMogr2/format/jpg/quality/90)
更新程序将版本更新到了8.1.1，然后这个提示就不会再烦你了
再次输入安装Tensorflow的命令，如果不行，就多试几次，
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfbc3ccfba.png?imageMogr2/format/jpg/quality/90)
仔细看这些黄字的信息，会发现Network is unreachable这样的字样，如果你有什么传说中的利器的话，现在是时候拿出来用了。但是不用也没关系，多试几次，还是可以成功的。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfbd10dffa.png?imageMogr2/format/jpg/quality/90)
出现上图这样的界面的时候，你就可以长舒一口气，因为安装已经完成，现在你已经可以使用Tensorflow了，让我们跟着教程向世界说声hi吧。
**从Terminal进入Python环境的方法也很简单，直接输入命令：Python。如果本机已经安装了Python，Terminal会显示Python的版本号等信息，同时下一个指令的前缀会由用户名等一串字符变成这样的东西：“>>>”，这说明进入完成了。**
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfc121f898.png?imageMogr2/format/jpg/quality/90)
按照下面的命令一行一行的敲入代码，语法我在这就不解释了~
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfc1f5cf0c.png?imageMogr2/format/jpg/quality/90)
为了到达这一步，你可能付出了很多艰辛，可能中途出了很多错，但相信当你看到程序真的按照你的指令来运行了一遍，你会觉得这一切都是值得的。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfbfba1d0c.png?imageMogr2/format/jpg/quality/90)
**#彩蛋时间#**
1.程序员们在接触一门新语言的时候，通常做的第一件事就是编写一个Hello World程序，这一惯例源自最初一批大神们对计算机程序的希冀，希望它们就像一个新生儿一样，能友善的对这个世界宣告它的到来。
2.数字42的含义非常深刻。据著名一本道科幻小说“银河系漫游指南”所说，42是一个关于“生命、宇宙以及一切”的问题的答案（哈？你问我问题是什么？）。谷歌的创始人很可能也是这本小说的狂热粉丝，因为谷歌把这个彩蛋也嵌入了它们的搜索引擎之中，而且谷歌的总部外面就有一个“42”的模型。
## 大型程序开发必备：IDE（集成开发环境）
虽然说在Terminal里就可以直接编程了，但我们总还是需要一个工具的，要不然要编一个大型程序几乎是不可能完成的事。这就是IDE出场的时候了。大家就会惊喜的发现，自己之前在C语言看过的熟悉的编程界面又回来了（当然它们很多都是收费的）。其实由于Python是开源的，理论上任何人足够厉害的人都能开发出针对它的IDE，而确实有很多人这样做了。牛人太多，他们做出的工具孰优孰劣大家并没有统一的意见，所以大家干脆就不介绍该用哪个了，这才造成了很多人对Python“抽象”的感觉。
好用的IDE有很多，本文介绍的是Komodo IDE的免费版Komodo Edit。在Linux下打开它的官网（点击链接http://komodoide.com/download/edit-linux64/#），下载得到的是一个压缩文件。点击右键解压。
Linux中安装有些第三方软件的方法和Windows不太一样，比如这款，在它的文件夹里有一个install.sh。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfc818ab88.png?imageMogr2/format/jpg/quality/90)
这实际上就是安装文件，但直接打开它是不行的，要安装它还得在Terminal里进行。进入Terminal，确保自己在普通而不是Python环境下，然后通过输入cd+文件夹名称，逐层进入这个安装文件所在的文件夹，如进入Download文件夹，就输入：cd Download（**Python对大小写敏感，因此最好养成在输入时都分清大小写的习惯。**），因为**在输入一个安装命令的时候，程序只会在你当前所在的那个文件夹（不包括子文件夹）里搜索安装文件的名称。**习惯图形界面的人可能会有点头晕，但是只要牢记以下这点就能找到回家的路：打开Terminal时默认你所在的文件夹对应的文件管理器界面就是下图这个。在初期你可以一边开着图形界面打开文件夹，一边在Terminal里敲出对应的路径，这样就不会迷路了。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfc959cb33.png?imageMogr2/format/jpg/quality/90)
当然，有些软件有更简单的安装方式，这里不讲了。
到达安装文件所在的界面之后，输入./+文件名称，如本例中就是输入./install.sh，效果如下：
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfcacc5aa7.png?imageMogr2/format/jpg/quality/90)
按照信息中给出的路径就可以找到启动这个程序的文件了，本例的文件在bin里。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfcb77bba7.png?imageMogr2/format/jpg/quality/90)
接下来的设置就和在Windows中差不多了。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfcbf992d4.png?imageMogr2/format/jpg/quality/90)
打开后，我们通过它试验一下官方教程中“介绍”部分给出的一个相对简单的算法。
点击New File
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfcd3cdf11.png?imageMogr2/format/jpg/quality/90)
点击右上角的语言选择，选择Python，这样就可以在输入过程中检查出一些语法错误。
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfce4caaa7.png?imageMogr2/format/jpg/quality/90)
（如果直接复制的话，会把网页里带有的一些奇怪的代码也带过来，会导致程序运行出错，如下面示例中出现过的那样，因此需要将中文注释删掉。）将输入的代码保存为*.py，这是Python文件的后缀名。可以通过Terminal调用。按照之前的方法在Terminal中进入到存放文件的文件夹里，本文中的路径是这样的：
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfde052a03.jpg?imageMogr2/format/jpg/quality/90)
然后输入Python+文件名，本例中是Python Text-3.py，由于这个算法很简单，很快就能得到结果。满满的成就感有没有？
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfd275fb93.png?imageMogr2/format/jpg/quality/90)
![真正从零开始，TensorFlow详细安装入门图文教程！](https://static.leiphone.com/uploads/new/article/740_740/201606/575bfd31af585.png?imageMogr2/format/jpg/quality/90)
## 总结
其实本教程只是开了一个很小很小的头，讲解了关于Tensorflow的一些最基础的知识，包括其他环境中的安装、GPU支持等稍微复杂一点的东西都没有讲到。但实际上这从0到1的过程却是难住很多人的最大因素。在了解到教程中的这些东西之后，相信理解Tensorflow的官方教程已经对你没有难度了，如果你跟着官方教程走下去，会发现其实这些东西并不算难，相信以各位的智慧一定能学会的。那么本次教程就到这里了，各位加油吧，说不定创造出下一个AlphaGo的就是你们呢。
