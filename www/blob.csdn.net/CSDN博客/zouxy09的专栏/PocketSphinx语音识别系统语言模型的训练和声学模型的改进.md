
# PocketSphinx语音识别系统语言模型的训练和声学模型的改进 - zouxy09的专栏 - CSDN博客


2012年09月06日 09:27:01[zouxy09](https://me.csdn.net/zouxy09)阅读数：28342


**PocketSphinx****语音识别系统语言模型的训练和声学模型的改进**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
关于语音识别的基础知识和sphinx的知识，具体可以参考我的另外两篇博文：
语音识别的基础知识与CMUsphinx介绍：
http://blog.csdn.net/zouxy09/article/details/7941585
PocketSphinx语音识别系统的编译、安装和使用：
http://blog.csdn.net/zouxy09/article/details/7942784
下面的工作都是基于上面两篇博文往下做的，所以最好先了解下前面的情况。
下面的过程参考CMU sphinx的wiki：
http://cmusphinx.sourceforge.net/wiki/
**一、训练语言模型**
词与词之间存在着合乎句法与否的约束，语言模型就是用来表示这些约束的，它可以提供字与字之间的上下文信息和语义信息。在pocketsphinx语音识别系统时，采用了N-gram模型，即对训练音频文件所对应的文本文件进行统计，提取不同字词之间先后发生的统计关系。
如果你的语言模型较小（例如小的语音指令集或者任务），而且是英文的，那就可以直接上CMU提供的网络服务器上面训练，如果较大的话，一般使用CMUclmtk语言模型工具来训练。下面分两种情况来介绍：
**1.1****、利用在线工具建立语言模型**
**（****1****）创建一个语料库：**
语料库实际上就是一些文本的集合，包含了你需要识别的语音的文字的一些集合，例如句子啊，词啊等等。
\#vi corpus.txt
输入如下内容：
stop
forward
backward
turn right
turn left
保存退出
**（****2****）利用在线工具****LMTool****建立语言模型：**
进入网址：http://www.speech.cs.cmu.edu/tools/lmtool.html
点击Browse按钮,选择之前创建的corpus.txt，最后点击COMPILE KNOWLEDGE BASE。这样就会生成字典文件* .dic和语言模型文件*.lm，例如：
生成TAR2916.tar.gz
tar xzf TAR2916.tar.gz
2916.corpus 2916.lm 2916.sent.arpabo 2916.vocab
2916.dic 2916.sent 2916.token
真正有用的是.dic、.lm的文件
**（****3****）测试结果：**
pocketsphinx_continuous解码器用-lm选项来指定要加载的语言模型，-dict来指定要加载的字典。
\#pocketsphinx_continuous -lm 2916.lm -dict 2916.dic
INFO: ngram_search_fwdflat.c(295): Utterance vocabulary contains 1 words
INFO: ngram_search_fwdflat.c(912): 97 words recognized (2/fr)
INFO: ngram_search_fwdflat.c(914): 2342 senones evaluated (38/fr)
INFO: ngram_search_fwdflat.c(916): 1011 channels searched (16/fr)
INFO: ngram_search_fwdflat.c(918): 167 words searched (2/fr)
INFO: ngram_search_fwdflat.c(920): 47 word transitions (0/fr)
WARNING: "ngram_search.c", line 1087: </s> not found in last frame, using <sil> instead
INFO: ngram_search.c(1137): lattice start node <s>.0 end node <sil>.56
INFO: ps_lattice.c(1228): Normalizer P(O) = alpha(<sil>:56:60) = -341653
INFO: ps_lattice.c(1266): Joint P(O,S) = -341653 P(S|O) = 0
000000000: STOP (-6531224)
READY....
Listening...
**1.2****、训练大文本数据的语言模型**
我们通过语言模型训练工具CMUCLMTK统计大量文本数据得到以单个词建立的N-Gram模型。
语言模型训练工具的说明见：
http://www.speech.cs.cmu.edu/SLM/toolkit_documentation.html
具体的步骤如下：（得先安装语言模型训练工具CMUCLMTK，安装过程在上一博文中）
**（****1****）准备训练的文本，也就是语料库：**
这里只是举一个例子，所以语料库也不大，而一般的语料库都是由大文本来组成的，里面就是有一些我们的日常用语或者报纸啊，书啊等等所出现过的句子。文本数据越大，训练得到的语言模型就越好。
\#vi weather.txt
在.txt中输入如下内容，记住结尾不可留“\n”(实验证明了这一点)。每个utterances由<s>和</s>来分隔
<s>天气</s>
<s>有雨</s>
<s>晴朗</s>
<s>多云</s>
<s>雷电</s>
wq保存退出
**（****2****）产生词汇表****vocabulary****文件：**
\# text2wfreq < weather.txt | wfreq2vocab > weather.tmp.vocab
**命令****text2wfreq****：**统计文本文件中每个词出现的次数，得到一个后缀为wfreq的文件，内容示例为：
二1334
九1334
</s> 3680
即表示词二、九和</s>在训练文本中出现的次数依次为1334、1334、3680。
**命令****wfreq2vocab****：**统计文本文件中含有多少个词，即有哪些词。如数字识别中包含10个数字和两个静音，故共有12个词，按照拼音顺序依次是：</s>、<s>、八、二、九、零、六、七、三、四、五、一。
**（****3****）生成****arpa****格式的语言模型：**
\# text2idngram -vocab weather.vocab -idngram weather.idngram < weather.closed.txt
\#idngram2lm -vocab_type 0 -idngram weather.idngram -vocab weather.vocab -arpa weather.arpa
**命令****text2idngram****：**列举文本中出现的每一个n元语法。产生一个二进制文件，含有一个n元数组的数值排序列表，对应于与词有关的的N-Gram。超出文本范围的词汇映射值为0。
**命令****idngram2lm****：**输入文件包括一个idngram文件，一个vocab文件和一个ccs文件，输出是一个后缀为binlm的语言模型文件。其中ccs文件指句首和句尾的静音<s>和</s>。
**命令****binlm2arpa****：**是将binlm文件转换为实验中需要的arpa格式语言模型文件。
**（****4****）转换为****CMU****的二进制格式****(DMP)****：**
如果你的语言模型比较大的话，最好就转换为CMU的二进制格式(DMP)，这样可以加快加载语言模型的速度，减少解码器初始化的时间。但对于小模型来说，就没有这个必要，因为sphinx3能处理这两种后缀名的语言模型文件。
\#sphinx_lm_convert -i weather.arpa -o weather.lm.DMP
最终生成了语言模型weather.lm.DMP，此文件为解码器端所需要的文件格式。
**二、改进现有的声学模型去适应我们的语音：**
（原文是Adapting the default acoustic model，但我不知道怎么用中文表达合适，呵呵，请各位前辈指导）
本文主要描述了如何通过对声学模型做一些简单工作去改进语音识别。但需要注意的是，他改进的只是你提供的适应数据和模型的匹配。例如：可以适应你的声音使得口语较好的识别，或者也可以使用特定的录音环境，适应你的或者用户的口音等等。
适应的过程是通过提供适应的数据（我不知道怎么翻译，也就是你的录音数据）去改进现有的模型。这样获得的识别效果往往比重新训练鲁棒性要强，尽管你的适应数据集比较小。例如，对于特定人来说，5分钟的语音就可以达到很明显的听写准确度了。
**1****、创建一个适应的语料库**
语料库包含了一些句子，这些句子是任意指定的，但最好可以覆盖你想要识别的句子里面包含的高频率单词或者音素。
CMU提供了一个例子CMU ARCTIC，里面包含了20个句子，但这个是英文的，所以我们得自己做一个中文的。CMU ARCTIC 见：http://festvox.org/cmu_arctic/
**1.1****、需要的文件：**
共需要四个文件：
arctic20.txt文本文件：里面是中文的句子
arctic20.fileids控制文件：记录我的语音文件（读arctic20.txt里面的句子的录音）的路径
arctic20.transcription脚本文件：中文句子和语音文件的对应
arctic20.dic字典文件：记录arctic20.txt里面的句子的因素组成
其实我也不知道具体应该如何建这些文件，我参考了CMU ARCTIC这个英文的，但中文有点不一样，我自己的这些文件的内容如下：（我新建了一个voice文件夹来管理这次的工作）
**arctic20.txt****因为我是要语音控制系统的，所以我就加入了很多这类的控制语言，共****12****个句子：**
你确定吗
打开我的电脑，确定还是取消
关闭计算机，退出浏览器
你好吗？可以退出程序吗
返回上一页还是返回主菜单
放大还是缩小页面
今天的天气不错，不下雨
你喜欢听音乐吗，需要打开音乐播放器吗，听第几首歌
需要浏览图片，上一张还是下一张
我想看视频，有什么电影
打开系统设置，基本设置
进入主菜单，请控制，向左，向右，还是，向上，向下
**arctic20.fileids****我把上面****12****个句子对应的录音文件（放在当前目录下的）命名为****arctic_*****，不需要后缀名的：**
arctic_0001
arctic_0002
arctic_0003
arctic_0004
arctic_0005
arctic_0006
arctic_0007
arctic_0008
arctic_0009
arctic_0010
arctic_0011
arctic_0012
**arctic20.transcription****前面****<s>****和****</s>****表示静音，（）括号内的就是对应的录音文件：**
<s>你确定吗</s> (arctic_0001)
<s>打开我的电脑，确定还是取消</s> (arctic_0002)
<s>关闭计算机，退出浏览器</s> (arctic_0003)
<s>你好吗？可以退出程序吗</s> (arctic_0004)
<s>返回上一页还是返回主菜单</s> (arctic_0005)
<s>放大还是缩小页面</s> (arctic_0006)
<s>今天的天气不错，不下雨</s> (arctic_0007)
<s>你喜欢听音乐吗，需要打开音乐播放器吗，听第几首歌</s> (arctic_0008)
<s>需要浏览图片，上一张还是下一张</s> (arctic_0009)
<s>我想看视频，有什么电影</s> (arctic_0010)
<s>打开系统设置，基本设置</s> (arctic_0011)
<s>进入主菜单，请控制，向左，向右，还是，向上，向下</s> (arctic_0012)
**arctic20.dic****字典文件：记录****arctic20.txt****里面的句子的因素组成，我建立这个文件的时候还是比较坎坷的。先是按词语为单位，到后面统计时，统计不过去，然后我改成以词为单位，还是统计不了，出现段错误，最后我改成以句子为单位，就是下面这样，才可以，注意下面标红的这几个，和我们平时的拼音有点不一样：**
你确定吗n iq uxed ing m a
打开我的电脑，确定还是取消d a k ai w o d e d ian n aoq uxed ing h aish ibq uxx i ao
关闭计算机，退出浏览器g uan b i j i s uan j i t ui ch u l iu l an q i
你好吗？可以退出程序吗n i h ao m a k e y i t ui ch u ch engx uxm a
返回上一页还是返回主菜单f an h ui sh ang y i y e h aish ibf an h ui zh u c ai d an
放大还是缩小页面f ang d a h ai sh ib s uo x iao y e m ian
今天的天气不错，不下雨j in t ian d e t ian q i b u c uo b u x ia y u
你喜欢听音乐吗，需要打开音乐播放器吗，听第几首歌n i x i h uan t ing y iny uxs uxem ax uxy ao d a k ai y in y uxe b o f ang q i m a t ing d i j i sh ou g e
需要浏览图片，上一张还是下一张x ux y ao l iu l an t u p ian sh ang y i zh ang h ai sh i x ia y i zh ang
我想看视频，有什么电影w o x iang k an sh i p in y ou sh en m e d ian y ing
打开系统设置，基本设置d a k ai x i t ong sh ezh ibj i b en sh ezh ib
进入主菜单，请控制，向左，向右，还是，向上，向下j in r u zh u c ai d an q ing k ong zh ib x iang z uo x iang y ou h ai sh i x iang sh ang x iang x ia
**1.2****、录制你的适应数据**
为适应语料库里面的每一个句子录制一个语音文件，录音文件的命名需要和arctic20.transcription与arctic20.fileids的一致
注意：此处音频文件采样率16KHz，16bit单声道录音，而且是wav格式。
我在Linux下写了一个处理脚本来实现上面这12个句子的录音：rec_wav.sh，内容如下：
for i in `seq 1 12`; do
fn=`printf arctic_%04d $i`;
read sent; echo $sent;
rec -r 16000 -e signed-integer -b 16 -c 1 $fn.wav 2>/dev/null;
done < arctic20.txt
在这里我们需要用到rec这个录音命令，这个命令是由Linux下一个很有名的音频转换程序sox携带的，所以我们需要先安装sox：
\#apt-get install sox
然后改变rec_wav.sh的运行属性：
\#chmod 777 rec_wav.sh
\#./rec_wav.sh
这个脚本会显示一句话，然后进入录音，我们把这个句子读出来，然后按ctrl+c显示下一句话，然后录音，如此循环到全部的句子的语音数据拿到。这样在当前文件夹下面就会显示如下文件：
arctic_0001.wav
arctic_0002.wav
.....
arctic_0012.wav
然后，我们需要测试下每个音频文件是否正常：
for i in *.wav; do play $i; done
他会依次的播放我们录下的这些音频文件；如果不对的话，就得重录，我的就没有什么问题。
**2****、适应声学模型**
首先我们需要先拷贝现有的默认的声学模型到当前目录下
\#cp -a /usr/local/share/pocketsphinx/model/hmm/zh/tdt_sc_8k/ .
这个是pocketsphinx自带的中文声学模型
**2.1****、生成声学特征文件**
我们需要对我们录制的wav语音文件提取MFCC特征，而且必须以和默认的模型相同的声学模型参数去提取这些特征，他们存储在声学模型目录的feat.params文件中。
\#sphinx_fe -argfile tdt_sc_8k/feat.params -samprate 16000 -c arctic20.fileids -di . -do . -ei wav -eo mfc -mswav yes
这样在当前文件夹下就会对每一个语音文件生成一个*.mfc后缀的特征文件：（我这里生成的是*.mfc，但我执行后面的工作，也就是统计数据时，发现统计程序需要的是*..mfc的，我也不知道哪里出现问题了，所以我只能将其修改为*..mfc）
arctic_0001.mfc
arctic_0001.wav
arctic_0002.mfc
arctic_0002.wav
……
**2.2****、转化****sendump****和****mdef****文件**
有一些模型没有足够的数据去做适应。这里有一个额外的文件我们需要下载的，为了节省空间，所以没有添加进PocketSphinx的发行版包了。我们可以从下面的链接下载：
http://cmusphinx.svn.sourceforge.net/viewvc/cmusphinx/trunk/pocketsphinx-extra/?view=tar
解压，拷贝pocketsphinx-extra/model/hmm/zh/mandarin_sc3_notone_3s_8k.cd_semi_5000文件夹里面的mixture_weights文件到你的声学模型目录下；
\#cp ../sourcecode/pocketsphinx-extra/model/hmm/zh/mandarin_sc3_notone_3s_8k.cd_semi_5000/mixture_weights tdt_sc_8k/
当然，如果你安装了SphinxTrain Python modules，那么你就可以使用sendump.py去把声学模型的sendump文件转换为mixture_weights文件。
然后我们需要把声学模型的mdef文件转换为SphinxTrain训练工具可以使用的plain text格式：
\#pocketsphinx_mdef_convert -text tdt_sc_8k/mdef tdt_sc_8k/mdef.txt
**2.3****、累加观察序列**
通过SphinxTrain训练工具带的bw程序去收集适应数据的统计数据。bw程序在sphinxtrain-1.0.7声学训练工具源码的bin.i686-pc-linux-gnu文件夹中。
把bw和map_adapt和mk_s2sendump和mllr_solve程序都拷贝到当前目录。
\#cp ../sourcecode/sphinxtrain-1.0.7/bin.i686-pc-linux-gnu/bw .
\#cp ../sourcecode/sphinxtrain-1.0.7/bin.i686-pc-linux-gnu/mk_s2sendump .
\#cp ../sourcecode/sphinxtrain-1.0.7/bin.i686-pc-linux-gnu/map_adapt .
\#cp ../sourcecode/sphinxtrain-1.0.7/bin.i686-pc-linux-gnu/mllr_solve .
然后开始统计：
./bw -hmmdir tdt_sc_8k -moddeffn tdt_sc_8k/mdef.txt -ts2cbfn .semi. -feat 1s_c_d_dd -svspec 0-12/13-25/26-38 -cmn current -agc none -dictfn arctic20.dic -ctlfn arctic20.fileids -lsnfn arctic20.transcription -accumdir .
确保bw的参数和声学模型目录的feat.params文件一致；
我刚才说过字典文件的建立的时候，以词语和单字为基本单元的时候，在这个步骤就会出现以下类似的错误：
……
utt>     0               arctic_0001stat_retry(arctic_0001..mfc) failed
ERROR: "corpus.c", line 1555: MFCC read of arctic_0001..mfc failed.  Retrying after sleep...
stat_retry(arctic_0001..mfc) failed
ERROR: "corpus.c", line 1555: MFCC read of arctic_0001..mfc failed.  Retrying after sleep...
stat_retry(arctic_0001..mfc) failed
……
utt>     0               arctic_0001  357INFO: cmn.c(175): CMN: 30.40 -1.07 -0.65 -0.24  1.70 -0.08 -0.39  0.05  0.59 -0.15  0.15  0.40  0.34
0WARNING: "mk_phone_list.c", line 173: Unable to lookup word '你确定吗' in the lexicon
WARNING: "next_utt_states.c", line 79: Segmentation fault (core dumped)
修改为句单元后，就没有问题了。
**2.4****、创建****MLLR****（最大似然线性回归算法）变换**
MLLR是一种有效的轻量级的自适应方法，尤其是当样本数据集较小时。在在线适应中使用MLLR是一种比较好的方法。MLLR在连续模型中表现得比较好，但是因为半连续模型很依赖混合权重，所以它的效果有限。如果想得到最好的准确度，最好将MLLR适应和MAP适应两种方法结合起来应用；
下面我们产生MLLR变换，并在运行时将其传入解码器去适应这个声学模型。
\#./mllr_solve -meanfn tdt_sc_8k/means -varfn tdt_sc_8k/variances -outmllrfn mllr_matrix -accumdir .
这个命令行会产生一个名叫mllr_matrix的适应数据文件。现在，如果我们需要使用这个适应模型，那就可以通过给pocketsphinx命令行参数添加-mllr mllr_matrix来使用；
**2.5****、通过****MAP****更新声学模型**
MAP是一个不同的适应方法。他不同于MLLR，他不会创建一个通用的变换，而是更新每个模型参数。
我们将原来的声学模型目录拷贝为一个新的模型目录：
\#cp tdt_sc_8k/ -rf tdt_sc_8kadapt
通过map_adapt程序来实现适应：
\#./map_adapt -meanfn tdt_sc_8k/means -varfn tdt_sc_8k/variances -mixwfn tdt_sc_8k/mixture_weights -tmatfn tdt_sc_8k/transition_matrices -accumdir . -mapmeanfn tdt_sc_8kadapt/means -mapvarfn tdt_sc_8kadapt/variances -mapmixwfn tdt_sc_8kadapt/mixture_weights -maptmatfn tdt_sc_8kadapt/transition_matrices
**2.6****、重新创建适应的****sendump****文件**
通过已经更新的mixture_weights文件来重新创建sendump文件，可以节省空间：
\#./mk_s2sendump -pocketsphinx yes -moddeffn tdt_sc_8kadapt/mdef.txt -mixwfn tdt_sc_8kadapt/mixture_weights -sendumpfn tdt_sc_8kadapt/sendump
好了，恭喜你，现在你已经拥有一个改进了的适应你的语音的声学模型了。
新的声学模型在hub4wsj_sc_8kadapt目录下，目录有以下文件：
mdef
feat.params
mixture_weights
means
noisedict
transition_matrices
variances
然后把他拷贝到我们的软件目录，
\#pocketsphinx_continuous -hmm <your_new_model_folder> -lm <your_lm> -dict <your_dict>
然后就可以使用了。其中的语言模型和字典文件用默认的即可。
我试验了以下，感觉提升也不是很大。识别率还是挺低的，呵呵，不知道是不是我这个过程有一些错误或者有一些没有考虑的地方。所以暂时也没办法实用。
我也试验了pocketsphinx所携带的英语的识别，感觉比中文的好点，但识别率还是不高，呵呵，也可能是我的发音不标准。对于中文的大词汇量语音识别来说，我试过微软的，IBM的，感觉效果也不算特别好，不过商业的一般都会比这些开源的好，因为别人有足够的样本（不同人不同录音环境等等）去训练一两个月。但CMU这个开源项目还是得到不少的资助的。另外，不知道国内牛逼的科大讯飞怎样，还没试过。
而对于小词汇量的语音识别，例如一些控制指令集啊，数字啊这些，好像准确度都做得挺好的了。所以难道我要妥协只能采用小词汇量的语音识别来加入我的人机交互系统吗？但这样就只是能控制机器而已，不能像iphone的siri这样和她聊天了？不知道各位前辈有什么建议吗？


