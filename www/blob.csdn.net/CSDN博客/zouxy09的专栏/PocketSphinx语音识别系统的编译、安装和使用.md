
# PocketSphinx语音识别系统的编译、安装和使用 - zouxy09的专栏 - CSDN博客


2012年09月04日 16:36:29[zouxy09](https://me.csdn.net/zouxy09)阅读数：53125标签：[语言																](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[transition																](https://so.csdn.net/so/search/s.do?q=transition&t=blog)[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[嵌入式																](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=工具&t=blog)个人分类：[嵌入式																](https://blog.csdn.net/zouxy09/article/category/1218763)[语音识别与TTS																](https://blog.csdn.net/zouxy09/article/category/1218766)[Linux																](https://blog.csdn.net/zouxy09/article/category/1218761)[
							](https://blog.csdn.net/zouxy09/article/category/1218766)
[
																								](https://blog.csdn.net/zouxy09/article/category/1218763)

[
		](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=transition&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
**PocketSphinx****语音识别系统的编译、安装和使用**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
Sphinx是由美国卡内基梅隆大学开发的大词汇量、非特定人、连续英语语音识别系统。Sphinx从开发之初就得到了CMU、DARPA等多个部门的资助和支持，后来逐步发展为开源项目。目前CMU Sphinx小组开发的下列译码器：
Sphinx-2采用半连续隐含马尔可夫模型（SCHMM）建模，采用的技术相对落后，使得识别精度要低于其它的译码器。
PocketSphinx是一个计算量和体积都很小的嵌入式语音识别引擎。在Sphinx-2的基础上针对嵌入式系统的需求修改、优化而来，是第一个开源面向嵌入式的中等词汇量连续语音识别项目。识别精度和Sphinx-2差不多。
Sphinx-3是CMU高水平的大词汇量语音识别系统，采用连续隐含马尔可夫模型CHMM建模。支持多种模式操作，高精度模式扁平译码器，由Sphinx3的最初版本优化而来；快速搜索模式树译码器。目前将这两种译码器融合在一起使用。
Sphinx-4是由JAVA语言编写的大词汇量语音识别系统，采用连续的隐含马尔可夫模型建模，和以前的版本相比，它在模块化、灵活性和算法方面做了改进，采用新的搜索策略，支持各种不同的语法和语言模型、听觉模型和特征流，创新的算法允许多种信息源合并成一种更符合实际语义的优雅的知识规则。由于完全采用JAVA语言开发，具有高度的可移植性，允许多线程技术和高度灵活的多线程接口。
针对不同的应用和平台选用不同版本的引擎。因为我是需要把语音识别融合到我的人机交互系统上面的，而且未来还会移植到嵌入式平台上面，所以我选择了PocketSphinx语音识别引擎来提供语音识别功能。
为了可以更好的理解下面我安装的是什么工具，这些工具有什么作用，我们最好可以先了解下语音识别的基础知识和sphinx的知识，具体可以参考我的另一篇博文：
语音识别的基础知识与CMUsphinx介绍：
http://blog.csdn.net/zouxy09/article/details/7941585
**一、使用平台**
Ubuntu12.04
还需要安装的包：
\# sudo apt-get install libasound2-dev bison
**二、****CMUSphinx****语音识别工具包下载**
Pocketsphinx—用C语言编写的轻量级识别库，主要是进行识别的。
Sphinxbase—Pocketsphinx所需要的支持库，主要完成的是语音信号的特征提取；
Sphinx3—为语音识别研究用C语言编写的解码器
Sphinx4—为语音识别研究用JAVA语言编写的解码器
CMUclmtk—语言模型训练工具
Sphinxtrain—声学模型训练工具
下载网址：http://sourceforge.net/projects/cmusphinx/files/
我们只需要下载以下四个最新版本的工具包即可：
sphinxbase-0.7、pocketsphinx-0.7、sphinxtrain-1.0.7、cmuclmtk-0.7；
三、**安装工具包**
由于pocketsphinx依赖于另外一个库Sphinxbase。所以先需要安装Sphinxbase。
**（****1****）安装****Sphinxbase**
\#tar -xzf sphinxbase-0.7.tar.gz
\#cd sphinxbase-0.7
\#./configure
\#make
\# make install
注意：./configure将会检查你的系统是否具有其编译所需要的所有包，如果没有的话，它会提示缺少什么包，例如缺少“bison”，那么我们需要对它进行安装：
//搜索包含“bison”关键字的包，因为有些包可能不是这样命名的，后面可能会加个-dev表示开发板，前面也可能加个lib表示库等等。但这里搜索了，包的名字就是bison
\#apt-cache search bison
//然后就下载相应的包安装即可
\#apt-get install bison
安装完缺少的包后，继续执行./configure，以后遇到类似情况同样处理。
包默认安装在/usr/local/bin下面，ls可查看。可以在配置时用–prefix指定自定义的安装路径，但最好使用默认的安装路径，否则的话得自己添加这些库的路径进环境变量中。
**（****2****）安装****pocketsphinx**
得先把Sphinxbase的库的路径进环境变量中。通过：
export LD_LIBRARY_PATH=/usr/local/lib  //这个一般系统都已经加进去了
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig  //这个PKG_CONFIG_PATH主要指明Sphinxbase的.pc文件的所在路径，这样pkg-config工具就可以根据.pc文件的内容动态生成编译和连接选项，比如cflags（编译用的头文件）和libs（连接用的库），至于说pkg-config的具体细节？百度下就知道了，了解这个工具非常有用。
另外，环境变量的设置只对当前的终端窗口有效，新建一个新的终端或者重启系统之后，需要重新设置PKG_CONFIG_PATH。所以我们可以通过编辑系统的链接文件/etc/ld.so.conf去使其长期有效，如下：
\#vi /etc/ld.so.conf
打开后，在新的一行添加（每一个路径一行）：
/usr/local/lib
/usr/local/lib/pkgconfig
然后，执行：
\#ldconfig
使其生效（ldconfig将/etc/ld.so.conf列出的路径下的库文件缓存到/etc/ld.so.cache以供使用，系统是通过读取这个库列表文件来加快搜索链接路径的速度的）。
**然后开始编译安装：**
\#tar -xzf pocketsphinx-0.7.tar.gz
\#cd pocketsphinx-0.7
\#./configure
\#make
\#sudo make install
默认安装在/usr/local/bin下。
**测试下安装结果：**
\#pocketsphinx_continuous
若出现如下信息，说明安装成功。
INFO: cmd_ln.c(512): Parsing command line:
pocketsphinx_continuous
Current configuration:
[NAME] [DEFLT] [VALUE]
-adcdev
………………………………….
…………………………………
………………………………….
Warning: Could not find Mic element
INFO: continuous.c(261): pocketsphinx_continuous COMPILED ON: Feb 21 2011, AT: 22:31:47
READY....
**（****3****）安装语言模型训练工具****CMUCLMTK**
顺便把语言模型训练工具也安装上，因为后面需要用到（当然，如果你采用的是现成的模型的话，那就不需要安装这个工具），安装过程其实都是一样的。
\#tar -xzf cmuclmtk-0.7.tar.gz
\#cd pocketsphinx-0.7
\#./configure
\#make
\#sudo make install
默认安装在/usr/local/bin下面，可以看到下面这些生成文件
idngram2lm   idngram2stats      mergeidngram     text2idngram
text2wfreq    text2wngram       wfreq2vocab
**（****4****）安装声学模型训练工具**sphinxtrain
顺便把声学模型训练工具也安装上，因为后面需要用到（当然，如果你采用的是现成的模型的话，那就不需要安装这个工具），安装过程其实都是一样的。
\#tar -xzf sphinxtrain-1.0.7.tar.gz
\#cd pocketsphinx-0.7
\#./configure
\#make
这里没有make install的安装过程，因为sphinxtrain只是包含了用来训练声学模型的Perl和Python脚本，还有一些工具命令。
所以我们还需要下载Perl和Python工具来使用这些脚本：
他们的网址：http://www.perl.org和http://www.python.org/download/
ubuntu就是好啊，apt-get就行：
\# apt-get install perl
\# apt-get install python
**四、获得语言模型和声学模型**
语言模型和声学模型的来源有以下三个方面：我们这里主要讲中文的。
**（****1****）源代码自带的：**
pocketsphinx-0.7/model/hmm/zh/tdt_sc_8k目录下是中文的声学模型
pocketsphinx-0.7/model/lm/zh_CN目录下是中文的语言模型
这些模型已经被安装（拷贝）到了/usr/local/share/pocketsphinx/model目录下：
（hmm表示隐马尔可夫声学模型，lm表示language model语言模型）
.
├──hmm
│├──en
││└──tidigits
││├──feat.params
││├──mdef
││├──means
││├──sendump
││├──transition_matrices
││└──variances
│├──en_US
││└──hub4wsj_sc_8k
││├──feat.params
││├──mdef
││├──means
││├──noisedict
││├──sendump
││├──transition_matrices
││└──variances
│└──zh
│└──tdt_sc_8k
│├──feat.params
│├──mdef
│├──means
│├──noisedict
│├──sendump
│├──transition_matrices
│└──variances
└──lm
├──en
│├──tidigits.dic
│├──tidigits.DMP
│├──tidigits.fsg
│├──turtle.dic
│└──turtle.DMP
├──en_US
│├──cmu07a.dic
│├──hub4.5000.DMP
│└──wsj0vp.5000.DMP
├──zh_CN
│├──gigatdt.5000.DMP
│└──mandarin_notone.dic
└──zh_TW
├──gigatdt.5000.DMP
└──mandarin_notone.dic
我们可以通过\#pkg-config --variable=modeldir pocketsphinx这个命令来获得模型的安装目录。也就是/usr/local/share/pocketsphinx/model
**（****2****）网上下载：**
CMU sphinx的官网提供了各种语言的声学模型和语言模型的下载，具体见：
http://sourceforge.net/projects/cmusphinx/files/Acoustic%20and%20Language%20Models/
其中也有中文的：
声学模型：zh_broadcastnews_16k_ptm256_8000.tar.bz2
语言模型：zh_broadcastnews_64000_utf8.DMP
字典文件：zh_broadcastnews_utf8.dic
**（****3****）自行训练得到：**
因为每个人的声音不一样，另外应用也不一样（所需词汇类别和不同），所以别人的模型可能在自己的语音下识别准确率不高，这样就需要训练自己的声学模型和语言模型（CMU也提供一个改进现有声学模型的方法）。因为训练需要的准备和步骤挺多的，所以这里就不写了，具体的训练方法会在下一个博文中说明。
**五、使用语言模型和声学模型**
运行pocketsphinx进行语音识别需要指定三个文件：声学模型、语言模型和字典文件。我们以第二个网上下载回来的这三个文件为例子说明如何使用他们进行语音识别。
声学模型：zh_broadcastnews_16k_ptm256_8000.tar.bz2
语言模型：zh_broadcastnews_64000_utf8.DMP
字典文件：zh_broadcastnews_utf8.dic
先把以上三个文件放在同一个目录下，再把声学模型解压：
\#tar -xjf zh_broadcastnews_16k_ptm256_8000.tar.bz2
解压后会在当前文件夹生成一个包含以下八个文件的文件夹。
zh_broadcastnews_ptm256_8000/
├──feat.params   //HMM模型的特征参数
├──mdef   //模型定义文件（为每个即将进行训练的HMM的每一状态定义一个独特的数字标识）
├──means  //混合高斯模型的均值
├──mixture_weights   //混合权重
├──noisedict    //噪声也就是非语音字典
├──sendump  //用来从声学模型中获取mixture_weights文件的？？
├──transition_matrices  //HMM模型的状态转移矩阵
└──variances  //混合高斯模型的方差
然后我们执行：
\#pocketsphinx_continuous -hmm zh_broadcastnews_ptm256_8000/ -lm zh_broadcastnews_64000_utf8.DMP -dict zh_broadcastnews_utf8.dic
-hmm选项指定声学模型文件夹–lm选项指定语言模型–dict选项指定字典文件；
pocketsphinx会分析你传入的文件取得模型和识别的参数，然后显示出来，再有很多信息，等到显示：
Warning: Could not find Mic element
READY....
到这里你就可以对她喊话让它识别了。对着麦克风讲话结束后在屏幕上以文字形式显示识别结果。在麦克风输入时，两者的最长语音时间为5秒，前者是通过说话人的正常语句停顿作为断句的分割点，并将停顿之前的语音信号作为一个处理段。
下一步我会进行pocketsphinx语音识别系统的编程，但会放在训练自己的声学和语言模型后面，具体见博客更新。

[
](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
