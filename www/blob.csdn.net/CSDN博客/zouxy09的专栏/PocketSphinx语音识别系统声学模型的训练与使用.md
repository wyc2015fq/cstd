
# PocketSphinx语音识别系统声学模型的训练与使用 - zouxy09的专栏 - CSDN博客


2012年09月10日 10:37:54[zouxy09](https://me.csdn.net/zouxy09)阅读数：22534


**PocketSphinx****语音识别系统声学模型的训练与使用**
zouxy09@qq.com
http://blog.csdn.net/zouxy09

声学模型主要用于计算语音特征和每个发音模板之间的似然度。目的是为每个声学单元建立一套模型参数（通过不断地学习和改进得到最优的，也就是概率最大的一组HMM模型参数）。
**需要训练的情况：**
1）需要创建一个新的语音或者方言的模型；
2）为你的小词汇的应用创建一个特定的模型；
3）你有足够的大量的语音数据：单的人的话（控制指令）需要1小时的录音，200个人的话需要50小时的录音。
4）你需要有语言的音素结构的知识；
5）你有足够的时间去训练和优化（大概需要一个月）。
如果不具备以上的情况或者条件，那么用模型增强（适应）比训练一个新的模型更有效。
另外，你的语音数据库应该能够比较好地代表你所要识别的语音，而且最好是多个人的录音数据，而且包含多种录音情况，还有所有可能的语言学句子。语音数据库包含两个部分：训练集和测试集，一般来说，测试集占全部数据库的1/10，但最好不要超过4个小时的录音时长。
获得一个语音数据库的一个比较好的方式是：
1）对现有的你需要识别的语音的录音材料进行切割，例如新闻或者电台播音等；
2）把你身边的人，例如家人，朋友或者同事的语音录下来；
3）在voxforge上面收集；
关于语音识别的基础知识和sphinx的知识，具体可以参考我的另外的博文：
语音识别的基础知识与CMUsphinx介绍：
http://blog.csdn.net/zouxy09/article/details/7941585
PocketSphinx语音识别系统的编译、安装和使用：
http://blog.csdn.net/zouxy09/article/details/7942784
PocketSphinx语音识别系统语言模型的训练和声学模型的改进:
http://blog.csdn.net/zouxy09/article/details/7949126

SphinxTrain是CMU大学开发的开源声学模型训练器。此工具可以训练适应于PocketSphinx的半连续HMM声学模型，也可以训练应用于Sphinx3的连续HMM声学模型。
**（****1****）需要准备的文件：**
在进行训练之前，先通过SphinxTrain中的wave2feat文件对音频文件进行特征提取，得到的MFCC特征参数文件，并把它们作为声学模型训练的输入。在进行训练之前，需要建立如下准备文件：
分别创建两个目录etc和wav，然后在两个目录下面制作一些数据文件，方便后面训练时使用。待会需要的目录及文件如下所示：
**etc****：**
     your_db.dic - Phonetic dictionary
     your_db.phone – Phone set file
     your_db.lm.DMP - Language model
     your_db.filler - List of fillers
     your_db_train.fileids - List of files for training
     your_db_train.transcription - Transcription for training
     your_db_test.fileids - List of files for testing
     your_db_test.transcription - Transcription for testing
**wav****：**
     speaker_1
         file_1.wav - Recording of speech utterance
     speaker_2
          file_2.wav
下面对以上的文件作用做些说明：
**your_db.dic - Phonetic dictionary**
主字典文件：该文件中包含要训练的所有词语以及与它们相对应的音素集（记录的是字到语音单元的映射）。如果某个词有两个以上的发音要分别列出。在训练之前，要指明如何将每一个数字的拼音拆解成声学模型。这里我们将数字的每一个音节看成一个声学模型，存放在字典文件中。文件的加载内容为：例如：
三SAN
四SI
五WU
一YAO
一(2) YI
如果是英文的话，例如：
HELLO HH AH L OW
WORLD W AO R L D
另外，对于特殊字符，我们需要对其进行修改，此处不说了。
**your_db.phone – Phone set file**
音素文件：表示声学模型训练单元，由于数字识别系统只包含10个词的小词汇量识别系统，所以我们的训练单元以这是个词的音节为训练基本单元即可。音素排列以26个字母顺序排列。在数字识别中主要选择音节作为识别单元，故音素文件中包括十二个单元：BA、ER、JIU、LING、LIU、QI、SAN、SI、SIL、WU、YAO、YI。其中SIL表示句子的停顿或静音，或者是录音环境的背景噪声。YAO和YI代表了数字1的两个发音。
例如，如果声学单元是音素的话，就是每个音素一行，最后加上SIL这个静音单元：
AH
AX
DH
**your_db.lm.DMP - Language model**
语言模型文件：你在训练得到的语言模型。
**your_db.filler - List of fillers**
补充字典文件：该文件主要列举了非语音单词，它可以包括静音，背景噪声，或者mm，um或者呼吸，笑等等。多音字的情况必须全部列出，除此文件外我们还要建立一个补充字典来说明语音中的停顿，当然这种静音（或停顿）可以理解为背景噪声。每个补充字典文件至少包含以下三项：
<s> SIL
</s> SIL
<sil> SIL
文件中的<s>代表句首的停顿，</s>代表句末的停顿，<sil>代表句中词间的停顿。这些静音词汇共同对应音素SIL。
**your_db_train.fileids - List of files for training**
控制文件：指明了包含所有训练语音文件的路径。表示如下
wav/train/name1/1.WAV
wav/train/name2/1.WAV
**your_db_train.transcription - Transcription for training**
脚本文件:存放每个控制文件（用于训练的）路径的内容（告诉训练器你需要训练哪些语音单元）。文件内容是与控制文件的路径一一对应的语句内容。包括句中的停顿也要用文本列举出来。例如：1.WAV录下的就是hello world这个句子的音，那文件的内容就是：
<s> hello world </s> (1)
<s> foo bar </s> (2)
主要注意的是，每个行都需要以<s>开始和以</s>结束，而且()括号内的语音文件只包含文件名，不包含文件路径。
**your_db_test.fileids - List of files for testing**
控制文件：指明了包含所有测试语音文件的路径。
**your_db_test.transcription - Transcription for testing**
脚本文件:存放每个控制文件（用于测试的）路径的内容。
**在准备这些文件的时候必须注意以下几点：**
1）主字典和补充字典必须与音素文件相对应，即字典文件中不允许有音素文件中不存在的音素；
2）确保字典文件中没有重复的单词，不包括多音字的列举；
3）确保脚本文件和控制文件行数相同，并且一一对应；
4）确保脚本文件中的词都存在与字典文件中；
5）确保脚本文件中所有音素都存在于音素文件中，并且音素在训练的脚本文件中出现一次。
以上五点，如有一点不满足条件，训练将自动终止。
**（****2****）准备****etc****下的数据文件**
\# mkdir train
\# cd train
\# mkdir etc wav
\# cd etc
1、声音文件我是直接放在wav文件夹下的，命名如下（因为这样方便我用脚本去控制录音）：
\# vi my_db_train.fileids
voice_0001
voice_0002
voice_0003
voice_0004
voice_0005
voice_0006
voice_0007
voice_0008
voice_0009
voice_0010
voice_0011
voice_0012
voice_0013
voice_0014
voice_0015
voice_0016
voice_0017
2、测试数据和语音数据一样，因为我并没有测试这个步骤，所以把简单的copy了一下：
\# cp my_db_train.fileids my_db_test.fileids
3、我的库主要是包含了17个控制的指令，如下：
\#vi my_db_train.transcription
<s>确定</s> (voice_0001)
<s>取消</s> (voice_0002)
<s>关闭</s> (voice_0003)
<s>退出</s> (voice_0004)
<s>返回</s> (voice_0005)
<s>放大</s> (voice_0006)
<s>缩小</s> (voice_0007)
<s>音乐</s> (voice_0008)
<s>图片</s> (voice_0009)
<s>视频</s> (voice_0010)
<s>天气</s> (voice_0011)
<s>设置</s> (voice_0012)
<s>菜单</s> (voice_0013)
<s>向左</s> (voice_0014)
<s>向右</s> (voice_0015)
<s>向上</s> (voice_0016)
<s>向下</s> (voice_0017)
4、\#cp my_db_train.transcription my_db_test.transcription
5、\# vi my_db.dic
确定Q UE D ING
取消Q U X IAO
关闭G UAN B I
退出T UI CH U
返回F AN H UI
放大F ANG D A
缩小S UO X IAO
音乐Y IN Y UE
图片T U P IAN
视频SH I P IN
天气T IAN Q I
设置SH E ZH IB
菜单C AI D AN
向左X IANG Z UO
向右X IANG Y OU
向上X IANG SH ANG
向下X IANG X IA
6、\#vi my_db.filler
<s> SIL
</s> SIL
<sil> SIL
7、生成my_db.phone
\#cp ../../sourcecode/sphinxtrain-1.0.7/scripts_pl/make_phoneset.pl .
\#./make_phoneset.pl my_db.dic my_db.filler > my_db.phone
\# vi my_db.phone  //看下有没有多余的
A
AI
AN
ANG
B
C
CH
D
E
F
G
H
I
IA
IAN
IANG
IAO
IB
IN
ING
OU
P
Q
S
SH
SIL
T
U
UAN
UI
UO
UX
UXE
X
Y
Z
ZH
以上若有多余字母，可手工删除。
8、把之前的my_db.lm.DMP也拷贝到etc下。
至此，etc下的数据文件已准备完毕。
注意：以上所有的文件在输入结尾不可留下“/n”。
**（****3****）准备****wav****下的音频文件**
为每一个控制词录制一个语音文件，录音文件的命名需要和my_db_train.transcription与my_db_train.fileids的一致。
注意：此处音频文件采样率16KHz，16bit单声道录音，而且是wav格式。
我在Linux下写了一个处理脚本来实现上面这17个词的录音：rec_wav.sh，内容如下：
\#cd ../wav
\#vi rec_wav.sh
for i in `seq 1 17`; do
fn=`printf voice_%04d $i`;
read sent; echo $sent;
rec -r 16000 -e signed-integer -b 16 -c 1 $fn.wav 2>/dev/null;
done < ../etc/my_db.dic
在这里我们需要用到rec这个录音命令，这个命令是由Linux下一个很有名的音频转换程序sox携带的，所以我们需要先安装sox：
\#apt-get install sox
然后改变rec_wav.sh的运行属性：
\#chmod 777 rec_wav.sh
\#./rec_wav.sh
这个脚本会显示一个词，然后进入录音，我们把这个词读出来，然后按ctrl+c显示下一个词，然后录音，如此循环到全部的词的语音数据拿到。这样在当前文件夹下面就会显示如下文件：
voice_0001.wav
voice_0002.wav
.....
voice_0017.wav
然后，我们需要测试下每个音频文件是否正常：
for i in *.wav; do play $i; done
他会依次的播放我们录下的这些音频文件；如果不对的话，就得重录，我的就没有什么问题。
**（****4****）****an4****与****sphinxtrain****搭建语音训练系统**
an4是一个CMU在1991年录的音频数据库。具体见：
http://www.speech.cs.cmu.edu/databases/an4/
需要的包有：sphinxbase、SphinxTrain、pocketsphinx、perl和python，这两个是额外的需要安装的包，如果在windows下的话，建议使用ActivePerl。
配置训练脚本：
注意下面的sphinxtrain-1.0.7和pocketsphinx-0.7是我们当时下载解压的相应的源码目录。
\#mkdir /host/xtrain
\#cd /host/xtrain
\# ../sourcecode/sphinxtrain-1.0.7/scripts_pl/setup_SphinxTrain.pl -task an4
\# ../sourcecode/pocketsphinx-0.7/scripts/setup_sphinx.pl -task an4
在当前目录下面会产生如下目录：
bin
bwaccumdir
etc
feat
logdir
model_parameters
model_architecture
scripts_pl
wav
把（2）、（3）中etc、wav下面的文件相应的拷贝到刚生成的wav与etc中。这时候xtrain目录的文件如下：
.
├──bin
│├──agg_seg
│├──batch.c
……
│└──tiestate
├──bwaccumdir
├──etc
│├──feat.params
│├──make_phoneset.pl
│├──my_db.dic
│├──my_db.filler
│├──my_db.phone
│├──my_db_test.fileids
│├──my_db_test.transcription
│├──my_db.tmp.vocab
│├──my_db_train.fileids
│├──my_db_train.transcription
│├──sphinx_decode.cfg
│└──sphinx_train.cfg
├──feat
├──logdir
├──model_architecture
├──model_parameters
├──scripts_pl
│├──00.verify
││└──verify_all.pl
│├──01.lda_train
││├──baum_welch.pl
││├──lda_train.pl
││├──norm_and_launchbw.pl
││├──norm.pl
││└──slave_lda.pl
……
│├──90.deleted_interpolation
││└──deleted_interpolation.pl
│├──bin
│├──bwaccumdir
│├──copy_setup.pl
│├──decode
││├──psdecode.pl
││├──setup_sphinx.pl
││├──setup_tutorial.pl
││├──slave.pl
││├──verify_dec.pl
││└──word_align.pl
│├──etc
│├──feat
│├──lib
││├──Queue
│││├──Job.pm
│││├──PBS.pm
│││└──POSIX.pm
││├──Queue.pm
││├──SimpleConfig.pm
││├──SphinxTrain
│││├──Config.pm
│││└──Util.pm
││└──test_SimpleConfig.pl
│├──logdir
│├──make_feats.pl
│├──make_phoneset.pl
│├──maketopology.pl
│├──model_architecture
│├──model_parameters
│├──new_experiment.pl
│├──RunAll.pl
│├──setup_SphinxTrain.pl
│├──setup_tutorial.pl
│├──texFormat.pl
│├──tune_senones.pl
│└──wav
├──setup_SphinxTrain.pl
└──wav
├──rec_wav.sh
├──test_wav.sh
├──voice_0001.wav
├──voice_0002.wav
……
├──voice_0016.wav
└──voice_0017.wav
然后我们需要修改etc文件夹下面的配置文件，还要修改很多变量；
**（****5****）修改****etc/sphinx_train.cfg****配置**
**1****）配置那些文件的路径**
\# These are filled in at configuration time
$CFG_DB_NAME = "an4";
改为
$CFG_DB_NAME = "my_db";
**2****）修改语音数据库的音频文件的格式**
我们录制的是MSWav格式，所以修改sph为wav，修改nist为mswav。
\# Audio waveform and feature file information
$CFG_WAVFILES_DIR = "$CFG_BASE_DIR/wav";
$CFG_WAVFILE_EXTENSION = 'sph';
$CFG_WAVFILE_TYPE = 'nist'; \# one of nist, mswav, raw
改为
\# Audio waveform and feature file information
$CFG_WAVFILES_DIR = "$CFG_BASE_DIR/wav";
$CFG_WAVFILE_EXTENSION = 'wav';
$CFG_WAVFILE_TYPE = 'mswav'; \# one of nist, mswav, raw
**3****）文件命名**
\#*******variables used in main training of models*******
$CFG_DICTIONARY     = "$CFG_LIST_DIR/$CFG_DB_NAME.dic";
$CFG_RAWPHONEFILE   = "$CFG_LIST_DIR/$CFG_DB_NAME.phone";
$CFG_FILLERDICT     = "$CFG_LIST_DIR/$CFG_DB_NAME.filler";
$CFG_LISTOFFILES    = "$CFG_LIST_DIR/${CFG_DB_NAME}_train.fileids";
$CFG_TRANSCRIPTFILE = "$CFG_LIST_DIR/${CFG_DB_NAME}_train.transcription";
$CFG_FEATPARAMS     = "$CFG_LIST_DIR/feat.params";
确定etc下面的文件是以这种方式命名的。
**4****）配置模型类型与模型参数**
\#*******variables used in characterizing models*******
$CFG_HMM_TYPE = '.cont.'; \# Sphinx 4, Pocketsphinx
\#$CFG_HMM_TYPE  = '.semi.'; \# PocketSphinx
\#$CFG_HMM_TYPE  = '.ptm.'; \# PocketSphinx (larger data sets)
cont表示的是连续模型，semi表示半连续模型，是什么类型，就把前面的注销符号\#去掉就行了，pocketsphinx和Sphinx III一样，都是.cont。
**5****）配置声音特征参数**
这里的操作为采样率为8KHz的情况，若采样率为16KHz，则可以直接略过此步操作。
vi feat.params
添加修改如下：
-samprate 8000.0
-nfilt 31
-lowerf 200.00
-upperf 3500.00
-dither yes
**（****6****）配置解码参数****etc/sphinx_decode.cfg**
**1****）路径**
\# These are filled in at configuration time
$DEC_CFG_DB_NAME = 'an4';
改为
$DEC_CFG_DB_NAME = 'my_db';
**2****）语言模型名称**
\# This variables, used by the decoder, have to be user defined, and
\# may affect the decoder output
$DEC_CFG_LANGUAGEMODEL_DIR = "$DEC_CFG_BASE_DIR/etc";
$DEC_CFG_LANGUAGEMODEL  = "$DEC_CFG_LANGUAGEMODEL_DIR/an4.lm.DMP";
改为
\# This variables, used by the decoder, have to be user defined, and
\# may affect the decoder output
$DEC_CFG_LANGUAGEMODEL_DIR = "$DEC_CFG_BASE_DIR/etc";
$DEC_CFG_LANGUAGEMODEL  = "$DEC_CFG_LANGUAGEMODEL_DIR/my_db.lm.DMP";
**（****6****）开始训练**
\#cd /host/xtrain
\#./scripts_pl/make_feats.pl -ctl etc/my_db_train.fileids
\#./scripts_pl/make_feats.pl -ctl etc/my_db_test.fileids
\#./scripts_pl/RunAll.pl
这需要几分钟的训练时间，但如果数据库很大的话，可能需要一个月的训练时间。整一个训练过程最重要的阶段是第一阶段，主要是检查所有的配置是否正确，还有输入的数据是够具有一致性。
新的声学模型在目录model_parameters中生成：
root@ubuntu:/host/xtrain/model_parameters\# ls
my_db.cd_cont_1000    my_db.cd_cont_1000_2  my_db.cd_cont_1000_8   my_db.cd_cont_untied  my_db.ci_cont_flatinitial
my_db.cd_cont_1000_1  my_db.cd_cont_1000_4  my_db.cd_cont_initial  my_db.ci_cont
命名规则是：
model_parameters/<your_db_name>.cd_cont_<number_of senones>
或者
model_parameters/<your_db_name>.cd_semi_<number_of senones>
每个模型目录的文件如下：
mdef
feat.params
mixture_weights
means
noisedict
transition_matrices
variances
**三、测试使用**
\#cd /host/xtrain
\# pocketsphinx_continuous -hmm model_parameters/my_db.cd_cont_1000 -lm etc/my_db.lm.DMP -dict etc/my_db.dic
**Reference**
CMU sphinx的wiki：http://cmusphinx.sourceforge.net/wiki/
王韵，基于Sphinx的汉语连续语音识别，太原理工大学，硕士学位论文


