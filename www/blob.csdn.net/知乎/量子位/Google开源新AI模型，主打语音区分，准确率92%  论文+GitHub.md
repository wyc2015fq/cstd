# Google开源新AI模型，主打语音区分，准确率92% | 论文+GitHub - 知乎
# 



> 雷刚 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

一个AI音箱，认得每一个说话的对象。

所谓“千人千面”，此之谓也。小孩说话，它就依照小孩的喜好和模式，老人票友，它的智能推荐也相应变化……

总之一个音箱，对每一个跟他说话的人，都能分辨清楚——对人类再正常不过的能力，对AI却并不容易。

幸好，Google今天把这种AI正在追求的能力，又往前推进了一步。

而且论文放出，核心算法开源，就等你也来试试了。

## **RNN模型**

Google的新方法，是通过递归神经网络（RNN）模拟说话者的输入，只要开始发言，每个说话者都会建立一个属于他的RNN模型，然后在过程中，不断更新相应的RNN状态。



![](https://pic3.zhimg.com/v2-5e680c41b3617ce48ee02f1da159e1fe_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='321'></svg>)



加入我们正在进行一场多人会议，音频输入是线性的，但在Google这个模型中，会按照不同音色进行不同标注。


黄色代表一个发言者，红色又是另一个，然后黄色嘉宾发言者又接着发言了。

之前在进行类似的研究时，通常基于聚类算法，都是无监督学习的方法，因此给每个发言者打标签不容易。

另外，发言者混合交织发言，让无监督学习的模型最终效果亦打折扣。

但RNN不同，在这次研究者中，Google同学专门在中餐厅进行了训练（其中几位作者都是中国人）。



![](https://pic4.zhimg.com/v2-260d42f52f6c208909a6f4d2f5f8d3c3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='530' height='310'></svg>)



在中餐厅中，发言人未知且人数不确定。然后在开始发言后，他们一旦被识别，就会有对应的RNN模型，每个人都被建了一个，然后相应发言者有更新，就会在他的RNN模型上更新。


## **效果**

论文中称，他们把核心算法放在数据集NIST SRE 2000 CALLHOME上验证，最后得出的在线分类错误率（DER）低至7.6%。

更早之前，他们旧方法的错误率还是8.8%.

那92.4%的准确率意味着什么呢？

对实时应用程序而言，已足够高了。
![](https://pic2.zhimg.com/v2-1e15c9d41edb38b476400b7c56f86d2d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='426'></svg>)



## **意义**

或许你会问了，有啥用嘞？

智能音箱自不必说了，如果你家音箱能认得你家每一个人，自然个性化的AI体验会好很多很多。

另外在很多会议速记、影视作品配字幕配音的过程中，也能应用。

你可能也会用过一些所谓的“AI速记”产品，但一到多人会议就惨不忍睹了。

所以Google这次开源，可能会帮到他们。

至于更多实现或应用，感兴趣的盆友，欢迎自己上手~

## **传送门**



![](https://pic4.zhimg.com/v2-a6879b14a8676119f4c8ab6e96a161c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='193'></svg>)



论文地址：


[https://arxiv.org/abs/1810.04719](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.04719)

开源地址：

[https://github.com/google/uis-rnn](https://link.zhihu.com/?target=https%3A//github.com/google/uis-rnn)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




