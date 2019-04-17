# htk网络和解码源码（三、语言模型） - xmdxcsj的专栏 - CSDN博客





2015年05月24日 17:09:53[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1274








### (一)         FSLM

1.      结构体

![](https://img-blog.csdn.net/20150524170926481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

l  对于所有的1gram

Arpa文件中的每一行的每一个单词依次编号，lablist[lmId]和wordlist[lmId]存放标号lmId对应的labId和word。

unigrams 存放一个单词（LMId）对应的所有的发音（PronID多个）的一元概率。

l  对于高阶gram

word存放所有的word（比如“北京 天气”这个2gram，word[0]=“北京”,word[1]=”天气”）。

l  pronid和lmid的对应关系

Pronid 对应于词典里的一条发音

LMId 对应于语言模型里的一条word

对应的Pronid <=>pronId2LMId<=>对应的LMId=> lablist[lmId]和wordlist[lmId]

2.      函数

l  LogFloat LMTransProb_ngram (FSLM *lm, LMState src, PronId pronid,LMState *dest)

目的：从src（NEntry）起始，到下一个pronid对应的dest的语言得分。

流程：

一开始src为0xffffffff，对应的dest为<s>；

如果src为空，说明是一元，直接返回一元得分；

如果src（比如对应于“北京”）不为空，说明是多元：pronid（比如对应于“天气”）对应的se如果在src中，说明存在src+pronid的词条，直接返回src中se对应的得分（“北京天气”这个2gram的得分），返回的dest对应于src+pronid（“北京天气”）；反之（比如pronid对应于“吃饭”），说明不存在，需要使用回退（p(吃饭|北京)=p(吃饭)+bow(北京)），返回的dest对应于pronid不回退的最多的gram（比如返回“吃饭”）。

l  LogFloat LMLookAhead_2gram (FSLM *lm, LMState src, PronId minPron,PronId maxPron)

目的：从src（NEntry）起始，对应于下一个pronId（范围为minPron至maxPron）的最高得分。

流程：

遍历minPron到maxPron，如果在src的se，使用se对应的得分，否则使用回退，找出范围内最高的得分。

### (二)         Lmcache

1.      结构体

![](https://img-blog.csdn.net/20150524170932315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



