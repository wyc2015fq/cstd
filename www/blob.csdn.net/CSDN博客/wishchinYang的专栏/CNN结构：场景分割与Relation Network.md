# CNN结构：场景分割与Relation Network - wishchinYang的专栏 - CSDN博客
2018年03月23日 14:11:58[wishchin](https://me.csdn.net/wishchin)阅读数：1312
           参考第一个回答：如何[评价DeepMind最新提出的RelationNet](https://www.zhihu.com/question/60784169)Work 
           参考链接：[Relation Network](https://blog.csdn.net/u011418174/article/details/77254656)笔记  ，暂时还没有应用到场景中
           LiFeifei阿姨的课程：[CV与ML课程](https://zhuanlan.zhihu.com/p/29064921)在线
- 论文：[A simple neural network module for relational reasoning](https://arxiv.org/abs/1706.01427)
- github代码: [https://github.com/siddk/relation-network](https://github.com/siddk/relation-network)
![](https://img-blog.csdn.net/2018032313243655)
   摘抄一段：
Visual reasoning是个非常重要的问题，由于ResNet等大杀器出现，visual recognition任务本身快要被解决，所以计算机视觉的研究方向逐渐往认知过程的更上游走，即逻辑推理。
于是出现了去年比较火的Visual question answering（VQA)：给张图，你可以问任意问题，人工智能系统都需要给出正确答案。这是我之前做的一个简单的VQA demo ([Demo for Visual Question Answering](https://link.zhihu.com/?target=http%3A//visualqa.csail.mit.edu/))。VQA任务的典型数据库是COCO-VQA([Introducing the 2nd VQA Challenge!](https://link.zhihu.com/?target=http%3A//www.visualqa.org/))，今年出了第二代。大家做了半天，各种lstm, attention-based model, stacked LSTM等等，发现其实跟BOW+IMG的baseline差不了太多；VQA还是被当成个分类问题，离真正人类级别的reasoning还很远。这里大家逐渐意识到了两个问题，第一个是网络本身的问题，即现有的卷积网络并不能很好的表达因果推断；第二个问题是，直接在自然图片上进行问答系统的研究太难了，很难debug整个系统，于是有了下面两个解决方向：
**针对第一个问题，研究者开始在网络设计中explicitly加入reasoning or memory module.** 比如说，去年有篇比较有意思的CVPR'16论文，**Neural Module Networks**( [https://arxiv.org/pdf/1511.02799.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1511.02799.pdf)) ,　很好地提出了一个可以让网络进行compositional reasoning的模块，概念挺漂亮。可惜的是调参能力一般，performance离我那个iBOWIMG的baseline也差得不远（参见[https://arxiv.org/pdf/1512.02167.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1512.02167.pdf)）。
![](https://pic3.zhimg.com/50/v2-c6e660e0ffd0eabf78fed561c1f9aadf_hd.jpg)
![](https://img-blog.csdn.net/20180323140943873)
另外，我觉得这篇论文也有一稿多投的嫌疑，因为这几乎一模一样的模型在作者另外一篇**Learning to compose neural networks for question answering** （[https://arxiv.org/pdf/1601.01705.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1601.01705.pdf)）拿了NAACL'16的best paper 。作者Jacob我也认识，我就不多吐槽了，还好他不会看中文。。。
**针对第二个问题，研究者开始通过graphics合成图片的办法来建立绝对可控的VQA数据库，这样就可以更好的分析模型的行为**。Facebook AI Research几个研究者（Larry Zitnick和Ross Girshick)带着Feifei的学生Justin Johnson实习去年暑假搞了个合成的VQA数据库CLEVR（[CLEVR: A Diagnostic Dataset for Compositional Language and Elementary Visual Reasoning](https://link.zhihu.com/?target=http%3A//cs.stanford.edu/people/jcjohns/clevr/)）。CLEVR有100,000图片，~1,000,000个问题答案对。里面的每张图片都是通过Blender渲染生成，里面的物体基本属性是three object shapes (cube, sphere, cylinder), two absolute sizes (small and large), two materials (shiny, matte), four relationships (left, right, behind, in front)。然后问题也是通过从90个问题模板里面采样生成。 通过各种组合可以生成海量而且可控的问题答案，如下图所示。然后在这篇论文中，作者测试了一些VQA常用的baselines, BOW+IMG, LSTM啥的，表现都一般。那个暑假正好我也在FAIR实习，跟Larry和田总 
[@田渊栋](//www.zhihu.com/people/9819f6938be0d3bb133ad0151eefd188)
 一起做项目。Larry是非常喜欢这种Neural module network和programming sytax的路子。跟Justin和Ross以及组里其他成员去Lake Tahoe那边hiking的时候也聊起了这个数据库，他们说你那个simple baseline for visual question answering在这个数据库上不管用了啊=,=!。另外，八卦下，Justin是Stanford的跳水运动员，Ross是户外运动达人，所以整个hiking过程中我费了牛劲，也再没见过这两位，望其项背而不得=,=!![](https://pic1.zhimg.com/50/v2-7e278269f95ca5c09152294ab09017aa_hd.jpg)![](https://img-blog.csdn.net/20180323141003384)
后来，这篇CLEVR数据库的论文被CVPR'17收了。然后，同一堆作者再马不停蹄再交了篇ICCV'17 submission: **Inferring and executing programs for visual reasoning  ([https://arxiv.org/pdf/1705.03633.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1705.03633.pdf))。**这篇论文也搞了个compositional model for visual question answering的模型。模型有两个模块program generator, execution engine，第一个模块来预测问题里面的program, 第二个模块来执行这些预测出来的program, 然后进一步预测答案。这两个模块还是蛮新颖的，打破了以前做VQA就用CNN叠LSTM的简单粗暴套路。这模型受之前那个Neural Module Network也影响挺大，也更符合CLEVR本身数据的生成过程。结合CLEVR本身的生成program, 这两个模块其实是可以分开进行训练。
![](https://img-blog.csdn.net/20180323141036315)
![](https://pic4.zhimg.com/50/v2-49df92dd4fb035152057fc944c3a3659_hd.jpg)
在Table 1里面（如下图所示），作者测试了两种办法, Ours-strong是把CLEVR数据库本身的700K的生成program全部拿来训练（注：这篇论文的作者本来就是CLEVR的作者），然后可以得到牛逼的96.9%,　已经秒杀人类的92.6%准确度。然后Ours-semi方法，用更少量的18K和9K的program进行训练，也可以得到95.4%和88.6%的准确度。
![](https://pic4.zhimg.com/50/v2-f3a09a9d2154013a781c637307001f49_hd.jpg)![](https://pic4.zhimg.com/80/v2-f3a09a9d2154013a781c637307001f49_hd.jpg)
这个训练过程其实蛮tricky。训练过程给的除了question-answer pair, 还给了生成CLEVR数据库本身的program, 这跟其他方法比较就不是这么fair了。另外，我觉得这个训练过程，其实是在训练一个新的网络reverse-engineer之前CLEVR数据库本身的生成程序。并且，这个reverse-engineering的复杂度挺低，用9k program训练就可以达到88.6%就是个证明。换句话说，对于这种自动生成的数据库，用好了方法和模块结构，也许挺容易刷到高分。这就为接下来的Visual Relation Network埋下了伏笔。
说了这么大堆东西，终于轮到DeepMind的Visual Relation Network module ([https://arxiv.org/pdf/1706.01427.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1706.01427.pdf))登场了。模型如下图所示，
![](https://img-blog.csdn.net/2018032314105343)
![](https://pic2.zhimg.com/50/v2-5a560789932444516731eb89a8d03098_hd.jpg)
如标题所示，模型结构确实很简单：LSTM编码question, 然后跟两两配对的spatial cell的deep feature叠加，然后后面接一些FC layers最后softmax分类到某个答案词上面。总结起来就是这个非常简单的两两配对的learnable module：
![](https://pic2.zhimg.com/50/v2-6bb7236c99466e18fed2850976f43697_hd.jpg)![](https://img-blog.csdn.net/2018032314113314)
这样简单的模型，在CLEVR上达到了"惊人"的95.5%，比之前最好的CNN+LSTM+SA还要好差不多20%。这里作者并没有比较前面那篇ICCV submission里的96.9%，这是为什么了，作者解释说“ ...(ICCV method) uses additional supervisory signals on the functional programs used to generate the CLEVR questions" 。这种说法的确没任何问题，但这却能帮我们更好的理解模型。
这篇论文并没有给出任何解释模型为什么work, 以及内部到底学到了些什么东西（这是我自己很关注的一个问题）。我自己觉得这个relational module其实是implicitly学到了"the functional programs used to generate the CLEVR questions"，也就是说，训练过后这个module可以很完美地reverse engineer那个CLEVR数据本身的生成过程。如果我是这篇论文的reviewer, 我会要求作者对模型进行可视化分析，可能里面的一些hidden units就是在做visual relation detection。
Relational module种explicitly表达两两物体关系的做法，跟之前DeepMind的那篇Spatial Transformer ([https://arxiv.org/pdf/1506.02025.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1506.02025.pdf))也有共通之处：CNN网络本身并不能很好地表达某些变换或者关系，所以需要一些特定的learnable的module结构来帮助表达和学习。可以预见这个relational module可以在很多结构化数据，如graph learning, structure learning等结构化数据上派上大用场。论文中也把模型在其他两个数据库任务bAbI model for language understanding(这个数据库其实早就被刷到100%了)和dynamic physical system reasonsing进行了测试，都取得了不错的效果。但是，这个module是真正解决了relationship detection的问题，还是仅仅只是利用short-cut来overfit数据库，还得等到人们在其他场合，如visual relationship detection([Visual Relationship Detection with Language Priors](https://link.zhihu.com/?target=http%3A//cs.stanford.edu/people/ranjaykrishna/vrd/))等测试其有效性
作者：周博磊
链接：https://www.zhihu.com/question/60784169/answer/180518895
