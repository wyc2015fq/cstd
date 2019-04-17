# Skype SILK vs. iLBC vs. Speex - DoubleLi - 博客园






对比一下这三种VOIP语音[算法](http://lib.csdn.net/base/datastructure)的特点:



1 参数与特征



![](http://hi.csdn.net/attachment/201004/28/0_12724708406VXX.gif)



2 SILK性能



![](http://hi.csdn.net/attachment/201004/28/0_1272469009ldFP.gif)



![](http://hi.csdn.net/attachment/201004/28/0_1272469047QpEs.gif)



   关于iLBC和Speex的性能可以参考以前写的文章。



3 关于VOIP一些观点（仅代表个人观点）



  1)  Skype 辛苦三年开发的SILK为什么要开源？

       第一，技术上，其实SILK用到了很多Royalty Free的算法部分，遵循开源的法则，取之于民，用之于民；

       第二，商业上，Skype 根本不靠Codec赚钱，他本身最大的优势是网络条件做的比较好，这种环境下无论是SILK、iLBC还是G.729都可以达成比较不错的音质。

       第三，口碑上，提高知名度，都已经把SILK定点好了，将来一定会有很多第三方去推广，广告、市场和口碑双赢；



   2）iLBC一定就比Speex质量好吗？

       根据目前的评测，当二者的编码速率差不多时，iLBC的质量要好于Speex，但是也有一些牛人认为iLBC这种每帧独立编码的思想其实是低效率的，通过增加编码冗余也可以达到相同的音质，意思是将CELP编码器增加一些前后冗余信息适当提高码率，也可以提高丢包情况下的抗干扰性，这个暂时还没有定论。



   3）Speex 总感觉用的人少？

    到底是Speex的推广差、维护差，还是音质差？虽然Speex的开发者出来解释了很多原因，可是目前好像用的第三方不是很多，同时我也感觉它的音质好像是有一点差。



   4）有了SILK，还有人用iLBC吗？

    SILK和iLBC来自两个不同公司skype and GIPS，个人感觉会同时存在。



好了，欢迎大家对以上观点进行补充，谢谢！









