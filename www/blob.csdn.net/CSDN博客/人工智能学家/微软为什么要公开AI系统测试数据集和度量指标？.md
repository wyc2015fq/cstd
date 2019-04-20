# 微软为什么要公开AI系统测试数据集和度量指标？ - 人工智能学家 - CSDN博客
2017年11月21日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：346
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/HkPvwCuFwNN9pchxgliacPveu0uDcJN1vSLjuV3lvxbuwnwCYRibVvvicSXRTMO6GZ5QQqM4pzpd6yYmPIdhJj8cw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：微软研究院AI头条*
*概要：微软研究院Maluuba团队的研究员Samira Ebrahimi Kahou等人在研究如何利用人工智能理解柱线图和饼图中所包含的信息这一问题时遇到了一个难题：没有现成的数据集可以用来测试他们的假设。*
微软研究院Maluuba团队的研究员Samira Ebrahimi Kahou等人在研究如何利用人工智能理解柱线图和饼图中所包含的信息这一问题时遇到了一个难题：没有现成的数据集可以用来测试他们的假设。为了解决这一问题，他们自己创建了FigureQA数据集，并在今年秋天对外公布。
这个数据集只是近年来微软研究人员和工程师用来测试AI系统的众多数据集、度量指标和工具之一。目前，这些工具向全世界的研究人员开放，可以被用于测试各自的AI系统在例如实时语音翻译、输入词汇预测等任务中的表现。微软开发这些工具的团队认为，这些开放工具可以帮助学术研究人员和行业专家测试各自系统、比较研究成果以及相互学习。
微软必应搜索部门合作伙伴项目经理Rangan Majumder说：“这些公开数据集使我们的目标变得更加清晰。这样其他研究人员就很容易看清楚我们的技术发展方向。”Majumder带领的团队负责MS MARCO机器读阅读理解数据集的开发工作，他们即将在年底再次更新这个诞生刚刚一年的数据集。
MS MARCO 官网地址：
http://www.msmarco.org/
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/HkPvwCuFwNN9pchxgliacPveu0uDcJN1vW9nODT33lBmxiaTiaqZdWfS3yEV8UltnuJ0jyH9RlIpKFGPOKJqibia2Nw/640?wx_fmt=jpeg)
*微软必应搜索部门合作伙伴项目经理Rangan Majumder*
对于习惯传统科技行业行事风格的人来说，这种信息共享似乎无法理解。但是，在人工智能领域，这种开放性正在变得越来越普遍。位于蒙特利尔的微软研究院Maluuba团队项目经理Rahul Mehrotra 表示：“传统上，企业的研究工作是秘不示人的。但是现在，共享的风潮正席卷整个行业——几乎所有企业都在发表论文，试图引领科技前沿的发展。”去年，微软研究院Maluuba团队还发布了另外两个数据集：NewsQA和Frames。
NewsQA官网地址：
http://datasets.maluuba.com/NewsQA
Frames官网地址：
https://datasets.maluuba.com/Frames
 不少人工智能专家认为，协作精神对人工智能领域的长足发展至关重要，许多人工智能领域的早期突破都源于此。甚至身处有竞争关系的不同机构内的研究人员都可以共享彼此的研究成果。
微软翻译团队高级项目经理Christian Federmann表示：“我们不可能想到世界上所有的好点子，所以如果别人有个好点子并想尝试一下，我们可以给他们提供一个数据集来试一试。”
Federmann的团队开发了微软语音翻译语料库（Microsoft Speech Language Translation Corpus），并在最近在语料库中加入了更多语言。目前，所有人都可以利用这个语料库测试他们自己的类似于Microsoft Translator的实时翻译以及Skype Translator的语音翻译系统。Federmann表示，微软公司是少数几个拥有雄厚预算和资源的大公司之一，可以创建高质量的工具和数据集，让业界同行得以比较各自的研究成果。
Microsoft Speech Language Translation Corpus 下载地址：
https://www.microsoft.com/en-us/download/details.aspx?id=54689
研究人员需要创造一些标杆（benchmark）来展示自己的研究成果，而这些公开的数据集恰恰是上述标杆的关键。例如，语音识别研究领域新近的几项里程碑式的成就正是基于Switchboard语料库的训练结果。
**预付费模式**
许多正在开发数据集和度量指标的团队表示，从某种意义上说，他们是在“预先付费”，因为他们将来也会用到其他人所创建的数据集。
Mehrotra说，当Maluuba还是一个小型创业公司时，他们的研究非常依赖名为MCTest的微软数据集。现在，Maluuba已成为微软的一部分，他们很高兴看到自己所创建的数据集正在被业内其他人所使用。
获知MCTest 更多信息，请访问：
https://www.microsoft.com/en-us/research/publication/mctest-challenge-dataset-open-domain-machine-comprehension-text/
乔治亚理工学院助理教授、Facebook人工智能研究院的研究员Devi Parikh表示，Maluuba最近发布的FigureQA数据集很有帮助，在这个数据集的帮助下，像她这样的研究人员就可以研究需要多种人工智能技术才能解决的问题。例如，计算机要能准确阅读图形并回答有关问题，就需要同时运用计算机视觉和自然语言处理。她说：“从研究的角度来看，研究人工智能各分领域之间的交叉问题变得越来越有趣。”
尽管大家都看到了信息共享的价值，但AI领域的研究人员和工程师有时候也希望既能够比较他们的系统，也不必公开所有所用数据。
在微软去年收购的SwiftKey公司，高级软件工程主管Doug Orr表示，他所在的团队希望创建一种标准方法，用于衡量某个输入法在词汇预测（预测用户即将输入的词汇）方面的表现。词汇预测是SwiftKey输入法的一个关键组件，它能够根据个人的表达风格提供个性化的词汇预测。SwiftKey团队并没有共享某个具体的数据集，而是创建了一套可供研究人员应用于任何数据集的衡量标准——Language
 Model Challenge 。用户可通过GitHub下载到这套标准，它让研究人员有了一个标准化的尺度，用于衡量自身的改进并将结果与其他同行进行比较，而又无须共享专有数据。
Orr表示，这些衡量标准让公司的内部团队受益匪浅，因为他们能更了解自己的输入法，能够看到输入法改进的幅度，同时，该领域的每一个参与者也能更清晰地了解彼此的表现。
