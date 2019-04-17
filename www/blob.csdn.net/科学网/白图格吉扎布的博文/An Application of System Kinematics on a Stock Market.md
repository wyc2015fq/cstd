# 科学网—An Application of System Kinematics on a Stock Market - 白图格吉扎布的博文




# An Application of System Kinematics on a Stock Market                           

已有 930 次阅读2018-1-15 08:42|个人分类:[CLUB](http://blog.sciencenet.cn/home.php?mod=space&uid=333331&do=blog&classid=171794&view=me)|系统分类:[论文交流](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=333331&catid=11)



An Application of System Kinematics on a Stock Market 




Definition of *30*-market: Thirty stocks were chosen from Fidelity, and these thirty stocks in a given order make up  the *30*-market.

Definition of *30*-vectors: The thirty prices of these thirty stocks expresses the state of the *30*-market.

***Y******=Y(i)****（**i=1,2,…30）,        *(1-1）

***Y****’=**Y**(i)/|**Y**|**（**i=1,2,…30）,        *(1-2）

Where the |Y|=√∑((* Y**(i)*)2), *（**i=1,2,…30）,*is the vector length, a scalar. 

The market transition vectors (trends) are defined as the present over the previous states: 

***T****(i,k)**=**Y’****(i,k)**/**Y’****(i,k-1)**（**i=1,2,…30）        *（2-1）

Where the second index indicates the time: *k*=present, *k-1*=previous. 

Projection of the next states are expressed by a vector product of the present states and the trends:

***P******(i,k+1)****=**Y******(i,k)*******T******(i,k)****（**i=1,2,…30）        *（2-2）

Where the ***P******(i,k+1)*** is the projection of the next states of the *30*-market based on the existing information of the *30*-market.

Bai-Jameson Filter connects the two sequential states: 

***Y******(i,k+1)****=[**Y******(i,k)*******T******(i,k)***** β +**D******(i,k+1)***** α]/**（α**+β）**（**i=1,2,…30）*（3-1）

Where the ***D******(i,k+1)*** are the *30*-observation vectors, the closing prices of the thirty stocks on time k+1. Thus, the expectation of the *30*-market on *k+1* are the weighted sum of the observations on k+1 and the projection from k. And, where *α* and *β*are the weights of *D* and *P*, respectively. Setting the two weights equal to *α* = *β* =0.5: 

***Y******(i,k+1)****=[**Y******(i,k)*******T******(i,k)****+**D******(i,k+1)****]/2 **（**i=1,2,…30）*（3-2）

The formula can be further extended into a time chain (3-3). This Bai-Jameson Chain uses history information to predict the future states in a manner of Fading Memory:

***P(i，k+1)****= **T(i，k)**(**D(i,k)**+0.5***T(i,k-1)**(**D(i,k-1)**+0.5***T(i,k-2)**(**D(i,k-2)**+...+0.5***T2**(**D2**+**D1**)))) *

*（**i=1,2,…30）                                                      *(3-3)               



**Main Reference: T. Jay Bai, Trend Analysis, The Ethnic Publishing House, Beijing, 2006. **

**Translated from**：[http://blog.sciencenet.cn/blog-333331-768774.html](http://blog.sciencenet.cn/blog-333331-768774.html)



“新‘系统论’初探”地址：[http://blog.sciencenet.cn/blog-333331-825601.html](http://blog.sciencenet.cn/blog-333331-825601.html)








转载本文请联系原作者获取授权，同时请注明本文来自白图格吉扎布科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-333331-1094889.html](http://blog.sciencenet.cn/blog-333331-1094889.html)

上一篇：[为什么用股票市场数据检验草原系统动态](blog-333331-1094798.html)
下一篇：[Table of 30-DIA Investment Reports (2018)](blog-333331-1094949.html)


