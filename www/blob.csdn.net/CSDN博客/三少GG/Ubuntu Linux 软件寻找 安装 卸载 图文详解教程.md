# Ubuntu Linux 软件寻找/安装/卸载 图文详解教程 - 三少GG - CSDN博客
2010年04月28日 15:51:00[三少GG](https://me.csdn.net/scut1135)阅读数：1359标签：[ubuntu																[linux																[deb																[终端																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=deb&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
原文地址：[http://hi.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/blog/item/8fe434134b663b0b5baf532b.html](http://hi.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/blog/item/8fe434134b663b0b5baf532b.html)
作为一个桌面操作系统，Ubuntu（以后我简称为UB）自然也和Windows一样，拥有众多的实用软件，让我们平常来使用。很多新手，在安装了Ubuntu系统后，面对着与众不同的桌面，不知道该如何安装软件，这次的教程，我将详细的讲解下，Ubuntu系统下，如何寻找，安装，卸载，使用各种软件。
和Windows有很大的不同，Ubuntu系统下，有很多很多方法来寻找，安装软件。最常见的方法，我先总结一下，后面逐个解释：
**1. 用“添加删除程序”来寻找／安装软件**
**2. 直接从“终端”，来安装／卸载软件**
**3. 从“辅助软件”里面，来选择安装／卸载软件**
**4. 从网站，论坛来下载，安装软件**
简单的总结，就这4个方面，后来我将分别讲解，四种途径如何来获取，安装软件。到这里，大家或许能发现，Ubuntu和Windows，仅仅在安装软件这个方面，就存在着很大的区别了吧？想想我们在windows下怎么安装软件的，都是迅雷下载，然后双击。。。。。下一步下一步下一步吧？嘿嘿！Ubuntu下可就没这么麻烦了，即便是（和 windows相同的）第四种方法，下载后，来安装，也是：双击－点击install，就能开始自动安装了，大部分软件，根本没有什么下一步下一步。。。。。。。。。。
对于一个全新安装好的Ubuntu Linux系统，我们在寻找，安装各种软件之前，都必须先解决3个问题。分别是：
1. 配置你自己的网络，让Ubuntu Linux可以上网。这个不用多说吧？我这里根据的情况，只有ADSL上网拨号的教程，需要的可以看下：
[http://nkevin.blog.163.com/blog/static/44819481200911294210422/](http://nkevin.blog.163.com/blog/static/44819481200911294210422/)
2. 配置系统的“更新源”。先知道下，我后面会着重详细解释“源”这个概念的。
3. 更新源列表。后面会给出详细说明
一. 源
1.源的定义
先解释下，这个Linux系统下，超级特殊，重要的关键词。
源，是什么？说白了，就是“（系统更新+软件下载）服务器”，是linux系统特有的东西，虽然Windows系统也有更新服务器，但它和linux系统根本不是一个层次，根本不是一个等级，根本不是一个概念，根本不是。。。。。
众所周知，Linux系统是“开源系统”，因此，它的大部分东西都是免费提供的，包括：系统更新（补丁），软件，程序，甚至以前我们解释过的，操作系统的“内核”。因此，我们可以完全免费，自由，任意的联网更新它们，获取我们需要的东西。用来更新系统，获取软件的服务器，就称作“源”。
2.源的分类
全世界有很多企业，个人，组织来提供更新服务器，就是源了。简单的说，源可以分为：官方源，第三方源（私人的更新服务器）。UB系统安装好以后，是默认自带官方源的，全世界很多国家，都有自己的官方源。在中国自然也有，国内很多大学都有自己的源。从个人角度讲，我推荐国内用户使用“冰岛官方源”，因为它不但速度快，适合国内各种上网方法，什么联通，网通，长城宽带，铁通之类的。而且，它很稳定，不会出毛病。虽然国内很多大学，如清华，北大等都有自己的源，但有时候容易出毛病，而且速度不稳定。
3.源的设置
在UB下，我们可以完全自由的设置，定制我们的“源”，UB下，设置源的具体方法有如下2种：
（1）文本形式来设置源
源的地址，是保存为一个文本文档里面，具体位置在：/etc/apt 文件夹，名称：sources.list，我们可以通过编辑这个文件，来进行详细设置。编辑方法：从“应用程序（Application）”－“附件（Accessories），里面，打开你的“终端（Terminal）”，然后输入：
sudo gedit /etc/apt/sources.list
意思是：以管理员身份，编辑/etc/apt/sources.list文件。打开后我们就能看到了。
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/ee1089810efb39f2bd3e1eaf.jpg)
稍微解释下，首先，面对着满屏幕的英文，大家怎么看，方法就是，凡是每行的开头出现：“＃”字母的，这一行都能删除！它在文件里面不起任何作用，就是解释下，这行是干什么用的！其次，我们来看看，一个“源地址的组成：
**“deb http://tw.archive.ubuntu.com/ubuntu intrepid main restricted universe multiverse”**
**deb，是ubuntu 系统特有的，软件类型**，我后面详细解释，这里先大概知道就行，后面：“http://tw.archive.ubuntu.com/ubuntu”，是真正的，源地址（或者叫更新网址），再后面，我们需要特别注意：intrepid，如果你是很细心的人，应该能知道，这个英文，是UB系统版本的代号！
**Intrepid**，是8.10版本的代号。最新版本的 9.04，这里的英文应该是：**Jaunty**
UB的源，每个版本，根据代号不同，它对应的源地址也不同，比如我上面的是8.10的地址，那么，以前旧的8.04版本，它的源地址应该是这种：“deb http://Ubuntu.uestc.edu.cn/ubuntu/ hardy main restricted universe multiverse”
比较一下，应该能发现，最大的区别就是：hardy（main前面那个英文“，hardy，是UB 8.04的开发代号。和8.10不同哦！所以，选择适合自己的源地址，要先弄清楚，你自己安装的到底是什么版本，是8.04还是8.10，还是其他的。
下面，我给出 Ubuntu Linux 8.10的源的地址，在这个网页里面，大家不用都复制进去，自己决定用哪个吧~：
[http://nkevin.blog.163.com/blog/static/448194812008101505221925/](http://nkevin.blog.163.com/blog/static/448194812008101505221925/)
(2) 图形化设置方法。这种方法相对上面那个，是很简单的了。一般推荐菜鸟使用。但是我个人的建议，还是觉得新手们，应该至少知道，上面的方法。后面我会给出原因的。
用图形化的方法来设置源，大家可以参考这个页面，里面的”**三. 桌面配置**“，详细的图文介绍了使用方法，我这里就不再重复的贴一次了：
[http://nkevin.blog.163.com/blog/static/44819481200932474453592/](http://nkevin.blog.163.com/blog/static/44819481200932474453592/)
4.我们来说下，关于“第三方源”，除了UB官方，有很多企业，个人，组织，出于推广UB系统的目的，来提供自己的源服务器的，它们一般是在源里面，包括了自己出品的软件。在UB下，我们可以添加它们提供的源地址，就能更加方便的用“2.从终端来安装软件“来安装这些私人软件了。
比如，很多人都知道，UB下，安装显卡驱动后，我们想开启3D桌面特效，就要安装这个软件：Compiz Configure Settings Manager，这个特效管理软件，但官方源里面，没有这个软件，所以我们可以添加这个第三方源地址：
deb http://ppa.launchpad.net/compiz/ubuntu intrepid main，这样，你就能更方便的安装了。
其他的第三方源，也是这样添加的，就像我上面的图，能看到吧？里面就有compiz的源。
二.更新“源文件列表“
这个是什么意思？意思就是，和Windows存在着本质的区别，从源服务器上面，我们不但能免费获取到系统的更新补丁，每个源上面，都存在着超级大量的软件，来提供我们安装，使用，免费下载！因此，我们在设置好了源以后，就得来更新一下，UB会把源上面的信息，包括系统补丁，软件，内核等等，都作一个列表，这样我们就能方便的更新／安装它们了。命令如下，终端下输入：sudo apt-get update，做好列表后，就会自动停止终端的工作的，完全不需要我们干预，看下面的图吧，最后在终端出现：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/8b436f136dd442faf6039ea9.jpg)
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/791a2cea2e3a16f9d439c9aa.jpg)
三. 与众不同的“添加/删除程序“
行了，前面说了N多废话，现在我们开始正经的，寻找，安装软件了。
Windows下的添加删除程序，大家都熟悉了，只能用来卸载已经安装的软件，而UB不同，我们来看看UB的“添加删除程序”界面，就明白了，如图：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/94afdff37a72c97a342accab.jpg)
看到了吧？爽不？这里，列出了源里面大部分的软件，各式各样的，什么都有，我自己是没详细数过，至少得上百种吧？（不信自己数数，嘿嘿！）
1. 安装软件
超级简单！找到你需要的软件，勾上软件名字前面的“勾“，然后点击右下角的“**Apply Changes“（应用更改）**，UB就会替你”全自动“安装软件，根本不需要你任何干预！（好像就需要输入个你自己的密码……），软件安装后，你就能在面板的”**Applications“（应用程序）**里面，找到它们了。
2. 卸载软件
更简单，从“添加删除程序”里面安装的软件，卸载方法，就是再**找到你这个软件，去掉它前面的“勾”，然后“Apply Changes“（应用更改）**，Ubuntu就会替你”全自动“卸载软件，你输入个密码就行了。
**注意：在添加删除里面，我们可以一次性的安装/卸载，多个软件，随你安装，卸载多少。都选择好，最后点击：Apply Changes，就是了！**
3. 对于英文不好的，我给出添加删除程序里面，左边“列”对应的英文解释，其实，它就是把软件分类了：
All：全部软件
Accessories：附件。一些小功能的软件
Education：教育类的软件
Games：地球人都知道！
Graphics：图形，图像处理软件
Internet：网络软件
Office：办公软件
Other：其他软件
Programming：程序设计软件
Sound&Video：音乐，视频处理软件
System Tools：系统软件，工具
Universal Access：辅助软件
二. 从终端来安装软件
从终端来安装软件，需要事先满足两个条件：
**1. 你要安装的软件，必须在“源”里面，否则，终端就会提示：E: Couldn't find package**
**2. 你需要直接知道，你要安装的软件的名字！比如：UB下的“分区魔术师”—— Gparted**
**3. 安装，用这个命令：**
**sudo apt-get install 软件名**
比如：**sudo apt-get install gparted**
回车，输入密码后，也是全自动安装的，如果一个软件太大，那么终端可能就会问你，是否继续安装 [y/n］，这时候，输入个 y，回车就是了。然后就能全自动安装好！
4. 卸载。从终端安装的软件，这样卸载：
**sudo apt-get remove 软件名**，比如：**sudo apt-get remove gparted**
5. 一次性安装多个软件
在终端里面，你也可以一次性的安装N个软件，这样：
**sudo apt-get install 软件名1 软件名2 软件名3 软件名N**
我就不举例子了，你自己试试看，安装个电驴吧~ 软件名是：amule和amule-utils
看我的图，安装电驴的过程，跟你的是不是一样的：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/af1e2e13caa22909dc5401b4.jpg)
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/28d4e7087d0d2afb63d986b7.jpg)
三.从网站手动下载软件安装包来安装
大家都知道，windows下，常见的安装程序，是exe，在UB下，它有自己的安装程序类型，是：**deb**，比如，在UB下，一个软件的名字是这样：**xdict_1.0.3-2_i386.deb**
来解释下，xdict，是软件的名字， 1.0.3.2是软件的版本，i386是软件适用的平台，最后deb是安装包的类型。
在UB下，是提供两种系统平台的，I386和AMD64，在两种平台上面，适用的软件也不同，注意下载的时候看清楚，别下错了。如果你安装的软件，对应的平台不对，安装的时候就会出现错误：Wrong Architure（错误的系统架构）
1. 下载
下载东西我不想多说了，和windows一样的，UB自带的firefox，火狐网页浏览器，就能下载东西的。
2. 安装
双击一个下载好的deb 格式的软件安装包，就会出现类似这个界面：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/9ca68e63dcde4b4a0d33fab0.jpg)
注意看！在“status“处，会出现三种情况
**① All dependencies are satisfied，就代表，这个软件可以全自动安装了！我们只需要点击“install package“（安装软件包），就会开始安装了（需要输入你的密码）**
**② Requires the installation of .....packages**
**这种情况，ub会先自动下载些软件所需要的额外的文件，然后自动开始安装，我们只需要和上面一样，点击“install package“（安装软件包），就会开始安装了（需要输入你的密码）**
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/2e787423d926816eac34deb1.jpg)
**③ failed of dependencies.........................**
**错误，UB自己无法解决“依赖关系”（我后面会详细解释），这里先知道就行。这样就不能安装了。但这种情况还算少见的。**
安装完成后，就会出现提示的，而且右上角，我们点击的那个“install package“（安装软件包）”，就会变成：“Reinstall package“（重新安装软件包）”，就证明我们安装完了。如图：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/afbfe4e966fd0115b90e2db2.jpg)
3.卸载
我这里先不解释如何卸载了，后面，我会单独分一大类，详细介绍。
四.从网站，直接安装软件
这个，可是UB的特例了，嘿嘿，网址是：[http://appnr.com/](http://appnr.com/)，大家进去看看就知道了。
1.安装方法，点击相应软件右边的“**install**“，就能全自动安装了！超级简单！！！
2.卸载方法，这种方法安装的软件，和前面介绍的下载安装，卸载方法完全一样，我这里先不说，后面单独分类，详细介绍！
五.从“辅助软件”里面，来选择安装／卸载软件
首先，让我们来隆重的请出，UB系统下的“优化大师”： Ubuntu Tweak，这个软件，
下载地址：
[http://ubuntu-tweak.googlecode.com/files/ubuntu-tweak_0.4.7.1-1%7Ejaunty1_all.deb](http://ubuntu-tweak.googlecode.com/files/ubuntu-tweak_0.4.7.1-1~jaunty1_all.deb)
下载后安装，安装方法，我就不说了，deb包安装前面教过了。
安装好以后，从“**Applications"（应用程序）-"system tools"（系统工具）-"Ubuntu Tweak“**运行它，看图：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/2194beb5f31735e437d3cab3.jpg)
Ubuntu Tweak，是用来对UB系统进行些简单的设置的，它里面，带了一个“软件管理”功能，包括功能如下：
**1. 添加，删除许多额外推荐的软件。如我经常推荐的：QQ聊天软件eva，BT下载软件Deluge还有永中office！**
**2. 管理设置“源”。在这里可以直接编辑源的文件的。嘿嘿！**
**3. 软件的安装和卸载。**
从左边选择“**Applications（应用程序）**“，就能在右边看到很多软件了。在这里安装软件，和“添加删除程序”，安装／卸载的方法一样的，就是找到软件名字，然后“打勾”，“去掉勾”，点击：**Apply（应用）**，就能安装/卸载了。够方便的吧？
六. 新立得 软件包管理器 （**Synaptic Package Manager**）
这个是UB系统特有的好东西，正如名称所说的，它是用来“管理系统安装的软件包”的。你系统里面安装的全部东西（系统补丁，运行程序需要的库文件，软件，等等）在这里都能找到，并且进行添加，删除，卸载。
**注意：我前面提到的，自己下载deb包安装的，从网站自动安装的软件，都是在这里，找到并且进行卸载删除操作的。**
从面板上的：**System（系统）——Administration（系统管理）——Synaptic Package Manager（新立得软件包管理器。**运行它。界面如下：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/94afdff37a79c97a342accbc.jpg)
使用方法：
1.首先，你需要找到你安装的软件，（就是需要卸载的，或者安装的），可以用它里面的“**搜索（quick search）**”，或者自己慢慢找。很好找的！
2.找到你的软件后，在上面点击右键，会弹出几个选项，选择：**标记以完全删除（mark for complete removal）**
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/0a97fa3017db98b3a8018ebe.jpg)
3.你可以一次性删除多个软件包，最后，点击里面的“**Apply（应用）**“，就能自动删除你选择的软件（包）了。看我的图，就是在删除QQ程序：eva！自己照猫画虎，应该会吧？
4. 这里有一个实例，讲的是如何用“新立德软件包管理器”，搜索安装软件，大家可以看看。
[http://nkevin.blog.163.com/blog/static/44819481200956103527478/9481200956103527478/](http://nkevin.blog.163.com/blog/static/44819481200956103527478/9481200956103527478/)
五. RPM 类型 软件包的安装
在linux的世界，软件安装包，常见的就3种：**RPM**（广泛应用于redhat,fedora,open suse 系统上），**DEB**（用于debian 和 ubuntu系统上），**源代码形式**（就是程序的代码，要你自己来“编译安装”，适用于全部linux系统）
前面，我们说过了deb包的安装，在UB系统下，deb包的安装是最简单，方便，快捷的。所以，我们在找软件的时候，尽量找该软件的deb包，这样安装简单。
但是，对于一部分软件，它根本就没有deb，而只有RPM的包，怎么办呢？照样安装！方法如下
1.首先，我们先要安装一个辅助软件：alien，源里面就有，直接在终端里安装就行（**sudo apt-get install alien**）
2.安装好以后，不用管它，我们继续。下载你的RPM软件安装包，放桌面上（其他地方也行，只要你自己知道），然后打开终端，用cd命令，进入你那个安装包，所在的目录，比如，我放桌面上，就是：
**cd /home/kevin/Desktop** （kevin是我的用户名，自己对照着你的改！）
3.用alien，转换你的RPM包，为 DEB形式
方法：终端里面输入“**alien -d 软件包的名字**”，比如：** sudo alien -d abc.rpm**
4. 转换完成后，会在你放RPM包的位置，得到个同名的DEB包，就能安装了，deb包安装不用再说一次了吧？
5.注意！不是所有的RPM包，都能顺利转换的，有些转换后，deb无法安装。这是正常现象。碰上这种情况，就只能放弃了，继续寻找deb包，或者，索性直接去找“该软件的源代码安装包“了。
六. 源代码安装
这是最麻烦的安装方法了，但是，如果你想得到一个软件，最新的版本，就只能用这个方法。因为作者还没来得及制作deb或者rpm形式的安装包呢！源代码安装包，一般都是已“ tar.gz”这种文件格式存在的。
每个软件的源代码，安装方法都稍有不同，但基本上是“三步走”，在终端内，进入该软件的源代码文件夹：
**1. ./configure （进行设置）**
**2. make （产生安装所需的文件）**
**3. make install**
根据自己系统的不同（准确的说，是你系统所附带的包不同），在这个编译－安装过程中，会出现很多错误。这时候，你可以在终端里面，复制出这些错误，比如： error 1,failed to...之类的，然后直接用googgle去搜索，一般网上都能找到解决方法。
此外，凡是提供源代码的，它里面都附带了 readme.txt或者install.txt，就是安装说明，你可以先阅读，然后按照里面的详细说明，进行编译安装！
这里有一个实例，打开可以参考的，用源代码软件包安装声卡驱动：
[http://nkevin.blog.163.com/blog/static/44819481200942913635491](http://nkevin.blog.163.com/blog/static/44819481200942913635491)
七.其他特殊形式（类型）的软件的安装
除了上面提到的三种，还有一些，少部分其他的，比如： **xxxxx.run, xxxx.install**等等，这些其实都是有一个通性的，就是在”运行安装文件“之前，他们的操作其实都是一样的。
在Linux的世界里，常见的特殊形式的软件包有：**sh.bin.bundle.run**，这4种。比如官方的ATI显卡驱动安装文件，名字就是：ati-driver-installer-9-3-x86.x86_64.run，而Linux系统下的Vmware虚拟机软件，它的安装包名字则是：**VMware-Workstation-6.5.2-156735.i386.bundle**
其实它们的安装都是有共性的。安装过程中，用到的命令，其实都是相同的。按数字顺序进行逐步的操作
**1. 打开终端**
**2. 输入： cd 桌面**
**3. 输入： ls**
回车后，你应该能看到，屏幕上显示出了你要安装的软件的名称，如： abc.bin
4. 输入： **sudo chmod +x abc.bin**
5. 输入： **sudo ./abc.bin**
这样安装过程就开始了。看图，就是这样的：
![](http://hiphotos.baidu.com/%CE%E5%CC%EC%CB%C4%BF%C9%D2%D4/pic/item/610667262dfc3f3d8a82a1bf.jpg)
八. 其他常见问题说明
1. 安装的程序，从哪里运行？
两种方法：
①从**面板—应用程序**，相应的类别里面找，比如，你安装了eva，那就应该在**网络软件**里面，安装了永中office，就应该在办公里面。
②直接在终端里面，运行“软件名字”，比如直接打开终端，输入：**eva**，就能运行它了～（这种方法，有个问题，你运行后，终端窗口不能关闭！否则软件也跟着一起关闭了！）
2.程序都装在哪里了？
安装好的程序文件夹一般都在** /usr/share**，这个文件夹下面，软件的快捷方式则是在：**/usr/bin**，或者**/usr/share/bin**，这2个下面。（和Windows不同，不会在桌面上创建快捷方式的，其实，你可以去”应用程序“里面找，然后把它托拽到桌面上就行了）
3.在终端里面 apt-get install 。。。。。的软件，它会先下载软件安装包，然后自动安装，下载的包，在：**/var/cache/apt/archives**文件夹里面，自己去看看吧！如果你刚才，按我说的，装了电驴，那现在，你应该在这里看到：**amule_2.2.2-1ubuntu1_i386.deb**，这个东西，就是电驴的安装文件哦！
4. 在新立得里面，除了可以卸载软件包，还能右键：“mark for install“，来安装一个软件。
5. 对于一个下载好的deb包，我们除了可以双击——install，也可以在终端里面，通过dpkg命令来安装或者卸载，安装的命令是： sudo dpkg -i 软件包的名称。卸载命令是：sudo dpkg -r 软件包名称。比如
**安装软件：sudo dpkg -i amule_2.2.2-1ubuntu1_i386.deb**
**卸载软件：sudo dpkg -r amule_2.2.2-1ubuntu1_i386.deb**
**解压缩deb软件包到桌面上：sudo dpkg -x amule_2.2.2-1ubuntu1_i386.deb ~/Desktop**
6. 绿色软件
嘿嘿，在ub下，也有绿色软件哦！从网上下载的是压缩包，解压缩以后，你可以试试看，去找”软件名“那个文件，双击它，看看能不能运行。据我所知，linux版本的Xnview（看图用的），就是绿色的～双击就能用，都不用安装的！
7.软件依赖库
对于新手，这个是新词了，意思就是：一个软件，它依赖很多其他的附属的程序，文件。我必须具备这些程序，文件，才能开始顺利的安装我需要的软件。
在UBUNTU系统中，“依赖”的问题，得到了很好的改善了。但是，如果你是从网上下载的deb包来安装，有可能，会出现：** failed of dependencies of.......................（依赖关系不满足。。。。）**，这时候，你就需要先解决 of...................后面提到的文件，或者程序了。。。。
所以，建议新手们，还是尽量选择从“添加删除程序”，“终端”，ubuntu tweak里面，来安装软件。真的真的够用了！
九. Ubuntu Linux 常用软件推荐，图文说明，加下载地址。
哈哈，保证爽死你，下面这些软件都是我在用的，而且我提供的软件包，都是deb的，双击就能安装！
[http://nkevin.blog.163.com/blog/static/4481948120092167051465/](http://nkevin.blog.163.com/blog/static/4481948120092167051465/)
十.可以下载linux软件的常见网站
1.Ubuntu Linux 官方中文论坛，里面有个”软件推荐“专区，有很多好软件的。
[http://forum.ubuntu.org.cn/index.php?sid=6cfe2ff73d68ad30635def407b71f46e](http://forum.ubuntu.org.cn/index.php?sid=6cfe2ff73d68ad30635def407b71f46e)
2. 华军软件园 Linux 专区：[http://linux.newhua.com/](http://linux.newhua.com/)
3. Linux 自由软件库：[http://download.chinaunix.net/](http://download.chinaunix.net/)
4. Linux 伊甸园 软件专区：[http://download.linuxeden.com/](http://download.linuxeden.com/)
5. Firefox 火狐网页浏览器，插件下载网页（在线直接安装的喔！）
[https://addons.mozilla.org/zh-CN/firefox/browse/type:1/cat:5](https://addons.mozilla.org/zh-CN/firefox/browse/type:1/cat:5)
6. Linux 人 软件库
[http://www.linux-ren.org/modules/softstorage/index.php?seq=latest&mainid=24&subid=0](http://www.linux-ren.org/modules/softstorage/index.php?seq=latest&mainid=24&subid=0)
7.（下面开始是国外的了，要稍微懂点英文）Getdeb 软件下载，这里提供的都是deb安装包喔！
[http://www.getdeb.net/](http://www.getdeb.net/)
8. 下面这个最NB，要求的英文水平要高一点，这个软件是linux世界里面很知名的了，一天24小时，不停的在更新各式各样的linux软件~保证你用不完！
[http://linux.softpedia.com/get/Desktop-Environment/Tools/](http://linux.softpedia.com/get/Desktop-Environment/Tools/)
9. gnome files 软件下载站：[http://www.gnomefiles.org/](http://www.gnomefiles.org/)
10.最后，推进给大家一个专门搜索 DEB 类型软件安装包的搜索引擎：SourceForge 软件搜索下载站，虽然是英文的，但是确实很好用的。建议大家试试看，里面的英文也不难的！
[http://sourceforge.net/](http://sourceforge.net/)
输入软件名字，就能搜索，一般软件都能在这里找到的！
