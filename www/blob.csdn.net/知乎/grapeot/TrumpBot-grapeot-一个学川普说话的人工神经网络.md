# TrumpBot - 一个学川普说话的人工神经网络 - 知乎
# 

之前看到一些关于RNN的教程（当时看的是这个：[知乎专栏](https://zhuanlan.zhihu.com/p/22930328)），觉得很有意思。所以爬了川普发的26000+条推特训练了一个LSTM学他说话。感觉语言还不是特别通，但还是比较有川普的风格。。搞笑的是这个网络还会@奥巴马，发一些（不存在的）链接，加hag，以及引用和转发其他人的（并不存在的）tweet。。一些这个神经网络编出来的推特：
- After setbacks are simply putting all thanked for the situation. Too many statements against China--good luck.
- Does a budget blogging @BarackObama won't approve the opening of the luck course today- [http://t.co/uu002y4t](https://link.zhihu.com/?target=http%3A//t.co/uu002y4t)
- Looking forward to meeting a #TrumpTuesday on @AScheifer [http://bit.ly/bMvxVu](https://link.zhihu.com/?target=http%3A//bit.ly/bMvxVu)
- "@TexasArmance2016: @realDonaldTrump @mmunhannitorial Donald Trump for president!"

但大多数语句还是不通的。更多的例子参见这个网页：[Trump bot](https://link.zhihu.com/?target=http%3A//zhihu.grapeot.me/trumpbot.html)

一些技术细节：
- Twitter的数据是在这里拿的：[Trump Twitter Archive](https://link.zhihu.com/?target=http%3A//trumptwitterarchive.com/)。这网站也挺搞笑的。。
- LSTM用的是开源的code：[jcjohnson/torch-rnn](https://link.zhihu.com/?target=https%3A//github.com/jcjohnson/torch-rnn)。
- 训练用的是AWS上的K80。

