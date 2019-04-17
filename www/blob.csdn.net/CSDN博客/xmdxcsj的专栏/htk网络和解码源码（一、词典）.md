# htk网络和解码源码（一、词典） - xmdxcsj的专栏 - CSDN博客





2015年05月24日 16:58:57[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1283








以空格隔开的单词和发音全部放入到hashtab中，hash表存放的元素是NameCell。

Vocab中的wtab存放单词word的hash表，其中每一个hash值对应的都是一个链表。

Pron表示一条发音

Word表示一个单词，可能包含多个发音

LabId存放发音或者单词的字符串

![](https://img-blog.csdn.net/20150524165603912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




