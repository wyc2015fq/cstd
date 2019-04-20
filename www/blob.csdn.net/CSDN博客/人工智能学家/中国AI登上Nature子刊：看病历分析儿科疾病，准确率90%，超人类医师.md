# 中国AI登上Nature子刊：看病历分析儿科疾病，准确率90%，超人类医师 - 人工智能学家 - CSDN博客
2019年02月13日 17:33:20[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：23
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVwDXd77om50WIyIHTmDJZ7AAlOGTXxRDyVrvWwdQLtDVh9TEmEBMnicFSncLk5msNStMOqqNpLiaicg/640?wx_fmt=jpeg)
##### 铜灵 发自 凹非寺量子位 出品 | 公众号 QbitAI
AI大夫的能力又精进了。
以前的AI要想要辅助人类诊断，得先学会输入大量带标注的医学影像训练模型。现在不用了，AI只需读一读电子文字简历，就能具备病情分析能力。
有产品有真相。
最近，AI诊断平台“辅诊熊”引起了计算机圈的关注。它通过自动学习56.7万名儿童136万份电子文本病历中的诊断逻辑，就能准确诊断多种儿科常见疾病，研究显示已经达到了儿科主治医生的水准。
昨天，这项研究被刊发在Nature Medicine上，参与研发的依图科技表示，这是NLP技术至今处理过的最大规模临床数据。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVwDXd77om50WIyIHTmDJZ7S84tZxlVQr8gg6qqDsfYkYzq01QHRSXB5bfPVnzpMy4xKkdXDC4bjQ/640?wx_fmt=png)
# **论文介绍**
在论文Evaluation and accurate diagnoses of pediatric diseases using artificial intelligence中，依图科技和广州市妇女儿童医疗中心的71位研究人员介绍了“辅诊熊”的设计原理。
和人类中医“望闻问切”、西医“视触叩听”的诊断方法类似，AI也需要医生多方信息最终做出综合的病情诊断。
这一系列的诊断过程，对AI来说其实主要为两个部分：
一是用深度学习的方法读取NLP模型中的临床数据，包括患者病历中的主诉情况、症状、个人史、体格检查、实验室检验、影像学检查、用药信息等多方信息，输入到完全结构化的数据库中。
二是基于提取的临床特征，建立诊断系统，将疾病进行分类，输出诊断结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVwDXd77om50WIyIHTmDJZ7IX7ucuW5w9sCzbyJfjmgFOibibXV62s0VKmfFhpYwRcXFFghwbFR65BA/640?wx_fmt=png)
△ 诊断流程图
重头戏来了。针对这两个诊断阶段，论文中提出了一个专门对电子医学病例进行数据挖掘的系统框架，将医学知识和数据驱动模型结合在一起。
该模型先通过自然语言处理技术对电子病例进行标注，再利用逻辑回归来建立层次诊断。这是一种基于器官的方法，先将疾病划分为广泛的器官系统，之后逐层深入划分成器官子系统或更具体的诊断组。
诊断框架如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVwDXd77om50WIyIHTmDJZ7HvxO7tLJ2TOGNk7G7zjicIox8PoTW5FkIDmv4dSmoibTP1eIxPvks7FA/640?wx_fmt=png)
###### △ 诊断框架的层次结构
论文中显示，目前这只AI可以达到儿科主治医生的专业水准。
研究人员用F1 Score评估人类与AI的诊断水平。在这场较量中，研究人员共设置了2个初级医师组（组1、组2）、3个高级医师组（组3~组5）。
结果显示，这个AI模型在常见儿童疾病方面的综合诊断中准确率高于两个初级医师组，超过了8年临床经验的相对低年资儿科医生的诊断水平。在流感和手足口病等常见疾病到脑膜炎等危及生命的疾病，准确率达到90％至97％。
不过，这个AI目前还比不上“姜还是老的辣”的高级医师。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVwDXd77om50WIyIHTmDJZ7icibczicdv0bG0bQ8PEJ6ibGbdrdTvwzN34ciboibvbbHG9bgKewvkL2ZIeQ/640?wx_fmt=png)
# **外媒关注**
这篇文章的作者团队非常庞大，共71人，其中一作共6名，分别为：
广州妇女儿童医疗中心医生梁会营、Guangjian Liu, 加州大学圣地亚哥分校基因组医学研究所的Brian Tsui和Sally L. Baxter，依图科技CEO倪浩，广州医科大学第一附属医院Carolina C. S. Valentim。
论文一出，立即引发了外媒关注。
《纽约时报》点评说，这项研究前前后后访问过儿科医院18个月中18万名中国就医儿童的数据，能有这么庞大的数据量用于研究，也是中国在全球人工智能和竞赛中的优势。
外媒Techspot也感慨中国AI发展的巨大机会报道中表示，从中国收集医疗保健数据，比美国容易太多。
此外，外媒MIT科技报道中称，AI系统在医疗诊断领域已经显示出了巨大的希望，目前这个系统可以用于紧急护理中的分类任务，要达到取代医生的地步还有很长的路要走。
# **传送门**
想了解更多，可以前往Nature论文介绍页：
https://www.nature.com/articles/s41591-018-0335-9
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
