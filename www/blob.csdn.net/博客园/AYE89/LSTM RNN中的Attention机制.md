
# LSTM-RNN中的Attention机制 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[LSTM/RNN中的Attention机制](https://www.cnblogs.com/eniac1946/p/8820321.html)
|Posted on|2018-04-13 14:50|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8820321)|[收藏](#)
|一、解决的问题
|采用传统编码器-解码器结构的LSTM/RNN模型存在一个问题，不论输入长短都将其编码成一个固定长度的向量表示，这使模型对于长输入序列的学习效果很差（解码效果很差）。
|注意下图中，a|x|和 a|xx|部分。
![](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180413145644147-1157652498.png)
|公式如下
![](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180413150135135-85775558.png)
|科普：
|[http://www.jeyzhang.com/understand-attention-in-rnn.html](http://www.jeyzhang.com/understand-attention-in-rnn.html)
|[一文读懂Attention:](一文读懂Attention)|https://mp.weixin.qq.com/s/0SWcAAiuN3BYtStDZXyAXg
|二、基于Keras代码：
|Attention Layer:|[https://gist.github.com/cbaziotis/6428df359af27d58078ca5ed9792bd6d](https://gist.github.com/cbaziotis/6428df359af27d58078ca5ed9792bd6d)
|Github讨论
|How to add Attention on top of a Recurrent Layer (Text Classification)|\#4962
|可视化Attention权重：|[https://github.com/philipperemy/keras-attention-mechanism](https://github.com/philipperemy/keras-attention-mechanism)
|三、应用
|[Text Classification,  - Hierarchical attention network](https://richliao.github.io/supervised/classification/2016/12/26/textclassifier-HATN/)
![](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180413145433665-1981651080.png)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
