# OC和cocos2d-x内存管理机制的简单比喻 - 凯奥斯的注释 - CSDN博客





2014年02月27日 13:55:18[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1522








                就好像你家里有一台电视机
你爸爸看的时候把它打开，相当于alloc并init（对应cocos2d-x里是create+retain）
你妈妈看的时候，retain一下
你要看的时候retain一下
现在referenceCount = 3
有人不想看了，就release一下。
比如说，你爸不想看了，就release一下，但是不能把它关掉（dealloc），否则你妈就看不到了，就跟你爸打起来了（程序崩溃）
只有所有人都不想看了（referenceCount = 0），最后一个人才会把它关掉            


