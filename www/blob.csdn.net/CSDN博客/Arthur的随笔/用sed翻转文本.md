# 用sed翻转文本 - Arthur的随笔 - CSDN博客
2011年09月14日 18:43:52[largetalk](https://me.csdn.net/largetalk)阅读数：698
今天学到一个命令，用sed将文件中内容按行翻转一下。比如：
1
2
3
翻转之后变成
3
2
1
命令是
`sed '1!G;h;$!d' filename`如果想将改变直接保存到文本，那命令是`sed -i '1!G;h;$!d' filename`
