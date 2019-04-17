# AI说：你的书法有咖喱味丨看字识国别 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

在电影《大侦探福尔摩斯》中，福尔摩斯能够根据两个英文字母的笔迹，判断书写者的才能与性格特点，识破对手的伪装。
![](https://pic1.zhimg.com/v2-141336a076db0d7bf9ea1f3934e183b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='1080'></svg>)
如此神奇的笔迹理论听起来像是玄学，恐怕只能在小说电影中出现。现实社会中，利用笔迹来破案倒不是一件稀罕事。

但是，你听说过AI也能判断笔迹了么？
![](https://pic2.zhimg.com/v2-0886e94e2b828eca79c7d806c749e0a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='688'></svg>)
一个跨国研究团队通过机器学习的算法，让机器从英文手写的字迹中判断书写者来自中国、印度、马来西亚、孟加拉国和伊朗中的哪个国家。

这个团队包含中国、印度和马来西亚研究人员，其中两名中国研究者分别来自河海大学和南京大学。

## **亚洲五国英文书法**

这项研究的基础是来自中国、印度、马来西亚、孟加拉国、伊朗五个国家的数据集。

研究者们找了每个国家100份英文书写文本，总共500份数据。
![](https://pic1.zhimg.com/v2-6effc44b20b25bd53174bbfe9903de48_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1068' height='530'></svg>)
正如上图的例子所示，每个国家的人写英文风格都有所不同。

比如，中国人写英文就像写汉字，横平竖直，尤其是在“f”、“l”、“d”这些字幕上，竖着的笔画顶天立地一般。

而印度人和孟加拉人写的英文则完全不同，就像绣花一样，笔画圆润、上下起伏明显。

于是，他们根据笔画的特征，设计了下面这样一套方法：
![](https://pic2.zhimg.com/v2-c954f264b8a75da2d97d611d634ca359_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='842' height='846'></svg>)
降噪后提取笔画线条，识别字体数据的四种特征：距离因素、主成分分析（PCA Direction）、通过Cloud of Line Distribution软件进行的线分布特征、描边特征，通过分类器判断属于五个国家的哪一种。

Cloud of Line Distribution（COLD）是一个分析笔画的软件，可以分解单个字母，测量笔画的垂直度和弯曲度，进而判断写字者的国别。
![](https://pic4.zhimg.com/v2-8e1a870f95f7608a0fa75e6131d3f363_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='783' height='425'></svg>)
五国人书写的英文描边特征有巨大的区别。
![](https://pic3.zhimg.com/v2-af4da2d693cb8a4ca669eb9cba7809fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='803' height='414'></svg>)
而通过COLD分析笔迹关键点，可以变为极坐标系的展示形式，而五国笔迹的极坐标系图有巨大差别：
![](https://pic1.zhimg.com/v2-5b12b0bc7244dffb6b158528cecdbe30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='793' height='484'></svg>)
综合所有特征，五国笔迹就有了他们特有的矢量特征：
![](https://pic4.zhimg.com/v2-de51870bdc5a3cc839846192c3d4aedf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='465' height='908'></svg>)
## **准确率比前人算法翻倍**

在最后的测试中，这套方法的确识别出了不少各种各样的字体：
![](https://pic1.zhimg.com/v2-829fec7877fa705ffbf79d8c39aaaa34_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='773' height='701'></svg>)
研究者们统计了一下这个方法的准确率
![](https://pic2.zhimg.com/v2-e631c2c52b94b198d26c1ec970376cc9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='755' height='662'></svg>)
上图中，下面的表格是前人研究的方法测试结果，可以看出，针对数据集中的每个国家，笔迹识别准确率都在40%左右，没有过半；

上方的表格则是这次研究者们的测试结果，每个国家的笔迹识别准确率都在60%以上，平均达到了75%。

## **识别笔迹的国家到底有什么用？**

75%的准确率并不是一个多么好看的数字，何况这是仅仅建立在5个国家的基础上的。

毕竟，人们对这项技术的应用还有疑虑。或许它可以用在刑侦领域，以字迹结合人脸识别、生物识别等技术来判断犯罪，但字迹的识别只能作为参考，有误判的可能性。

不过，字迹作为个人的数据也可以被用来做营销。想想去银行填单子的时候留下了多少字迹，或许他们就会用字迹判断国家，进而给你推送理财产品呢。

## **论文传送门**

A New COLD Feature based Handwriting Analysis for Ethnicity/Nationality Identification
作者：Sauradip Nag, Palaiahnakote Shivakumara, Wu Yirui, Umapada Pal, Tong Lu
Comments: Accepted in ICFHR18
arXiv: [https://arxiv.org/abs/1806.07072](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1806.07072)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


