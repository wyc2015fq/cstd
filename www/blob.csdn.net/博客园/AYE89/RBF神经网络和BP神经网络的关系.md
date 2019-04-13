
# RBF神经网络和BP神经网络的关系 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[RBF神经网络和BP神经网络的关系](https://www.cnblogs.com/eniac1946/p/7371805.html)
|Posted on|2017-08-16 10:02|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7371805)|[收藏](#)
|作者：李瞬生
|链接：https://www.zhihu.com/question/44328472/answer/128973724
|来源：知乎
|著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
|BP Neural Network|- 使用 Automatic Differentiation (Backpropagation) 进行导数计算的层级图模型 (layer-by-layer graphical model)
|只要模型是一层一层的，并使用AD/BP算法，就能称作 BP Network。RBF Network 是其中|一个特例|。
|关于 AD 与 BP 算法的介绍
|1.|[请问自动微分法（Automatic differentiation）是如何用C++实现的？ - 李瞬生的回答](https://www.zhihu.com/question/48356514/answer/123290631?from=profile_answer_card)
|2.|[Backwards differentiation in AD and neural nets: Past links and new opportunities](https://link.zhihu.com/?target=http%3A//link.springer.com/chapter/10.1007/3-540-28438-9_2)
|接下来简单介绍 RBF Network 相比一般 Neural Network 具有的特点。
|机器学习的模型通常存在各种解读。相对正统的是统计上的解读。但这里只谈直观上的解读，以求易读。
|RBF Network 通常只有三层。输入层、中间层计算输入 x 矢量与样本矢量 c 欧式距离的 Radial Basis Function (RBF) 的值，输出层算它们的线性组合。
|&amp;amp;amp;amp;amp;amp;lt;img src="https://pic2.zhimg.com/v2-6ca339af711c3e1f4bc0faec67183765_b.png" data-rawwidth="652" data-rawheight="413" class="origin_image zh-lightbox-thumb" width="652" data-original="https://pic2.zhimg.com/v2-6ca339af711c3e1f4bc0faec67183765_r.png"&amp;amp;amp;amp;amp;amp;gt;![](https://pic2.zhimg.com/v2-6ca339af711c3e1f4bc0faec67183765_b.png)|图一
|如此可见，和普通的三层神经网络，RBF 神经网络的区别在中间层。中间层采用 RBF Kernel 对输入作非线性变换，以便输出层训练线性分类器。
|那么RBF Kernel 有什么特点呢？
|&amp;amp;amp;amp;amp;amp;lt;img src="https://pic4.zhimg.com/v2-eee3757340da292aa37e7041e431a8d3_b.png" data-rawwidth="410" data-rawheight="69" class="content_image" width="410"&amp;amp;amp;amp;amp;amp;gt;![](https://pic4.zhimg.com/v2-eee3757340da292aa37e7041e431a8d3_b.png)
|图二 Radial Basis Function
|&amp;amp;amp;amp;amp;amp;lt;img src="https://pic4.zhimg.com/v2-247e21354a2a27841557f0ae63133fbb_b.png" data-rawwidth="432" data-rawheight="277" class="origin_image zh-lightbox-thumb" width="432" data-original="https://pic4.zhimg.com/v2-247e21354a2a27841557f0ae63133fbb_r.png"&amp;amp;amp;amp;amp;amp;gt;![](https://pic4.zhimg.com/v2-247e21354a2a27841557f0ae63133fbb_b.png)
|图三 Plot of Radial Basis Function with 2D input
|RBF 拥有较小的支集。针对选定的样本点，它只对样本附近的输入有反应，如下图。
|&amp;amp;amp;amp;amp;amp;lt;img src="https://pic2.zhimg.com/v2-414076018e191d3ed7cf50bc1f49b0c1_b.png" data-rawwidth="466" data-rawheight="347" class="origin_image zh-lightbox-thumb" width="466" data-original="https://pic2.zhimg.com/v2-414076018e191d3ed7cf50bc1f49b0c1_r.png"&amp;amp;amp;amp;amp;amp;gt;![](https://pic2.zhimg.com/v2-414076018e191d3ed7cf50bc1f49b0c1_b.png)|图四 RBF 使样本点只被附近（圈内）的输入激活
|T. Poggio 将 RBF 比作记忆点。与记忆样本越近，该记忆就越被激活。
|有的同学看到这，也许会说：这不就是 SVM with RBF Kernel 么。
|这些同学对了一半。光看模型，RBF Network 确实与 SVM with RBF kernel 无异。区别在于训练方式。
|其实在深度学习出现之前，|RBF神经网络就已经提出了 2-stage training|。
|第一阶段为非监督学习，从数据中选取记忆样本（图四中的紫色中心）。例如聚类算法可在该阶段使用。
|第二阶段为监督学习，训练记忆样本与样本输出的联系。该阶段根据需要可使用 AD/BP。
|小结：
|1. RBF神经网络的训练过程可以使用BP，因此应纳入BP神经网络的范畴。
|2. RBF神经网络的训练分两阶段，这一点类似深度学习，且使RBF神经网络不同于SVM with RBF kernel。

|\#简单来说|，1）BP_NN是使用BP算法的神经网络，RBF_NN是激活函数是RBF的神经网络；2）层数、结构不同；
|3）NN一般使用BP算法，RBF_NN使用改进的BP算法（2-stage training）
|4）BP_NN是全局逼近，有局部极小值；RBF_NN是“局部感知域”，不存在局部极小，对非线性连续函数有一致逼近性
|更理论化的解释：
|作者：Zach
|链接：https://www.zhihu.com/question/44328472/answer/155506678
|来源：知乎
|著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
|RBF神经网络与BP神经网络都是非线性多层前向网络，它们都是通用逼近器。对于任一个BP神经网络，总存在一个RBF神经网络可以代替它，反之亦然。但是这两个网络也存在着很多不同点，这里从网络结构、训练算法、网络资源的利用及逼近性能等方面对RBF神经网络和BP神经网络进行比较研究。
|从网络结构上看。 BP神经网络实行权连接，而RBF神经网络输入层到隐层单元之间为直接连接，隐层到输出层实行权连接。BP神经网络隐层单元的转移函数一般选择非线性函数（如反正切函数），RBF神经网络隐层单元的转移函数是关于中心对称的RBF（如高斯函数）。BP神经网络是三层或三层以上的静态前馈神经网络，其隐层和隐层节点数不容易确定，没有普遍适用的规律可循，一旦网络的结构确定下来，在训练阶段网络结构将不再变化；RBF神经网络是三层静态前馈神经网络，隐层单元数也就是网络的结构可以根据研究的具体问题，在训练阶段自适应地调整，这样网络的适用性就更好了。
|从训练算法上看。 BP神经网络需要确定的参数是连接权值和阈值，主要的训练算法为BP算法和改进的BP算法。但BP算法存在许多不足之处，主要表现为易限于局部极小值，学习过程收敛速度慢，隐层和隐层节点数难以确定；更为重要的是，一个新的BP神经网络能否经过训练达到收敛还与训练样本的容量、选择的算法及事先确定的网络结构（输入节点、隐层节点、输出节点及输出节点的传递函数）、期望误差和训练步数有很大的关系。RBF神经网络的训练算法在前面已做了论述，目前，很多RBF神经网络的训练算法支持在线和离线训练，可以动态确定网络结构和隐层单元的数据中心和扩展常数，学习速度快，比BP算法表现出更好的性能。
|从网络资源的利用上看。 RBF神经网络原理、结构和学习算法的特殊性决定了其隐层单元的分配可以根据训练样本的容量、类别和分布来决定。如采用最近邻聚类方式训练网络，网络隐层单元的分配就仅与训练样本的分布及隐层单元的宽度有关，与执行的任务无关。在隐层单元分配的基础上，输入与输出之间的映射关系，通过调整隐层单元和输出单元之间的权值来实现，这样，不同的任务之间的影响就比较小，网络的资源就可以得到充分的利用。这一点和BP神经网络完全不同，BP神经网络权值和阈值的确定由每个任务（输出节点）均方差的总和直接决定，这样，训练的网络只能是不同任务的折中，对于某个任务来说，就无法达到最佳的效果。而RBF神经网络则可以使每个任务之间的影响降到较低的水平，从而每个任务都能达到较好的效果，这种并行的多任务系统会使RBF神经网络的应用越来越广泛。
|局部逼近与全局逼近
|BP神经网络的隐节点采用输入模式与权向量的内积作为激活函数的自变量，而激活函数采用Sigmoid函数。各隐节点对BP网络的输出具有|同等地位|的影响，因此BP神经网络是对|非线性映射的全局逼近|。
|RBF神经网络的隐节点采用输入模式与中心向量的距离（如欧式距离）作为函数的自变量，并使用径向基函数（如Gaussian函数）作为激活函数。神经元的输入离径向基函数中心越远，神经元的激活程度就越低。RBF网络的输出与数据中心离输入模式较劲的“局部”隐节点关系较大，RBF神经网络因此具有|“局部映射”|特性。

|总之，RBF神经网络可以根据具体问题确定相应的网络拓扑结构，具有自学习、自组织、自适应功能，它对非线性连续函数具有一致逼近性，学习速度快，可以进行大范围的数据融合，可以并行高速地处理数据。RBF神经网络的优良特性使得其显示出比BP神经网络更强的生命力，正在越来越多的领域内替代BP神经网络。目前，RBF神经网络已经成功地用于非线性函数逼近、时间序列分析、数据分类、模式识别、信息处理、图像处理、系统建模、控制和故障诊断等。
|[1] MATLAB中文论坛. MATLAB神经网络30个案例分析[M]. 北京航空航天大学出版社, 2010.
|[2] letsgoaway. 神经网络及BP与RBF比较[EB/OL].|https://|wenku.baidu.com/view/fd|cb2ba7195f312b3069a54c.html|.








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
