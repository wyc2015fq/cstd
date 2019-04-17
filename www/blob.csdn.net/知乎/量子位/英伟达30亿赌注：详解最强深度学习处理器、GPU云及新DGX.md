# 英伟达30亿赌注：详解最强深度学习处理器、GPU云及新DGX - 知乎
# 


![](https://pic3.zhimg.com/v2-f9d86b612737514ddc90d07c7e5fac12_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2024' height='958'></svg>)
陈桦 李林 发自 凹非寺

量子位 报道 | 公众号 QbitAI

昨晚堪称进行了一次AI“爆炸”，好几件大事同时发生。

首先，英伟达GTC大会行至高潮，新一代GPU正式发布，以及多项配套新技术，英伟达股价一夜上涨17%。

其次，[微软Build大会也掀起高潮，软件巨头推出智能音箱等新产品，并且在边缘计算、人工智能等方面投下重注。](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247485649%26idx%3D2%26sn%3D52e6acdad00a69d6fd39334623b73944%26chksm%3De8d3bfa3dfa436b5d1c2d5f010819f7c56613ad73502e3fcfa143cbd9dd84feaa6ae8da486d4%26mpshare%3D1%26scene%3D1%26srcid%3D05110fbBtGJUWEyi7UcWhqfa%23rd)

量子位先讲讲英伟达发布的新一代最强深度学习处理器。

英伟达CEO黄仁勋昨晚在英伟达GPU技术大会上发布了新的GPU架构Volta。Volta相对于前代产品性能提升了5倍。共有超过7000名科学家、工程师、创业者和媒体参加了此次大会。
![](https://pic2.zhimg.com/v2-2503cce240bc2111128ff1c0db8338bd_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='600'></svg>)
在两小时的发布会上，黄仁勋发布了多款基于Volta的全新人工智能超级计算机，包括新款英伟达DGX-1深度学习计算机；展示了Isaac机器人训练模拟器；推出了英伟达GPU云平台，帮助开发者获得最新经过优化的深度学习框架；以及宣布了与丰田关于新一代自动驾驶汽车的合作。

# AI推动了对GPU计算能力的需求

目前，全球有数亿用户依靠基于人工智能的搜索、翻译，以及语音识别服务。黄仁勋指出，去年，对人工智能创业公司的投资总额达到50亿美元。在两年时间里，Udacity在线教育平台上参与人工智能课程的学生增加了100倍，达到2万人。

这推动了对更庞大人工智能计算能力的需求。两年前，领先的图像识别系统需要每秒7000千万亿次浮点运算的计算能力。而目前，处理实时语言翻译的研究者需要超过每秒100000千万亿次浮点运算的能力。作为对比，全球排名前500的超级计算机峰值性能总和还不到每秒1000千万亿次浮点运算。

# 摩尔定律的失效
![](https://pic2.zhimg.com/v2-681a1e3cc722a8c251154c1aabfc4875_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)
计算需求的迅速增长恰逢摩尔定律的失效。

目前，传统CPU单线程性能的每年提升只有1.1倍。而作为对比，GPU性能的提升仍能达到每年1.5倍。这些性能提升来自于芯片技术和软件技术的发展。

“有人认为，这代表了摩尔定律的失效。”黄仁勋表示，“这正是我们存在的原因。我们意识到，在摩尔定律终结之后，必须找到前进的道路。”

# 新架构：Volta

其实早在2013年，英伟达就公布了Volta架构名称。不过，后来这家公司的架构演进，变成Maxwell、Pascal和Volta。

Volta架构的第一款GPU是GV100，这是Pascal GP100的后继者，这也是英伟达的新一代GPU旗舰，用以驱动下一代Tesla产品。
![](https://pic2.zhimg.com/v2-f6597567ba3c84b7ea9f56fdab9d325d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1302' height='1244'></svg>)
Volta几乎是一个全新的架构，并不是12nm制程Pascal架构+新的Tensor Cores。在线程执行、调度、核心布局、内存控制、ISA等方面都有显著不同。

Tensor Cores是Volta的一种新核心，专门为Tensor深度学习运算设计。这些内核本质上是将用于执行4×4矩阵操作的ALU大量集合在一起，特别是融合了乘法加法(A×B+C)，将两个4×4 FP16矩阵相乘，然后加上一个FP16或者FP32的4×4矩阵，最后生成一个4×4 FP32矩阵。
![](https://pic3.zhimg.com/v2-964cc31247bb37716f23ea1fb2a9b16e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='678' height='168'></svg>)
这些核心的意义在于，通过在一个单元中执行巨大的矩阵矩阵乘法运算，NVIDIA可以为该操作实现更高数量的FLOPS。

单个Tensor Core每时钟执行64个FMA操作（总共128 FLOPS），每个SM具有8个这样的内核，每个SM每个时钟1024个FLOPS。相比之下，即使采用纯FP16操作，SM中的标准CUDA内核只能在每个时钟产生256个FLOPS。

因此，在可以使用这些内核的情况下，可以实现4倍于Pascal架构的性能。

Volta还使用了最新NVLink，可以提供更大的带宽。GV100比GP100度两个NVlinks，一共有6个。![](https://pic3.zhimg.com/v2-f7ed73e13ceba752917a5f9625957fae_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1366' height='642'></svg>)

另外，Volta使用的SIMT（单指令多线程）也有重大改变。32线程内的单个CUDA内核现在具有有限的自主权。线程现在可以在一个细粒度的水平上进行同步，这意味着更高的整体效率。个别线程现在可以产生，然后重新安排在一起。

# Tesla V100 GPU

![](https://pic3.zhimg.com/v2-61d6e6b0271d5cc7d432791a16cb3ad6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1574' height='882'></svg>)使用GV100 GPU的第一个产品，是Tesla V100。这也是一款专注深度学习的最新加速处理器。

这款加速处理器搭载了210亿个晶体管，使用了台积电12纳米FinFET工艺制造。Tesla V100包含5120个CUDA核心，在64位计算精度下能实现每秒7.5万亿次浮点运算，在32位计算精度下能实现每秒15万亿次浮点运算。

英伟达CEO黄仁勋表示，V100搭载了新的Tensor核心，其中包含4x4的主处理阵列，能并行完成矩阵乘法，以某些精度来看吞吐量达到前一代Pascal架构的12倍。

他同时表示，相对于Pascal架构，V100的通用每秒浮点运算次数为1.5倍，对深度学习训练的提升达到12倍，而深度学习推理的性能达到6倍。

黄仁勋指出：“在Titax X上需要花几分钟完成的任务目前只需要几秒钟。”

英伟达将于今年第四季度开始销售V100。

去年9月，英伟达推出了专用于神经网络的Pascal GPU。Pascal架构在去年5月的大会上发布。![](https://pic1.zhimg.com/v2-8921dccba231387bac935bb9dece2b00_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1298' height='1396'></svg>)

# 30亿美元的赌注

为了Tesla V100，英伟达投入了超过30亿美元，而处理器的制造已达到了“光刻工艺的极限”。

Volta将支持新发布的深度学习框架Caffe 2、微软Cognitive Toolkit、MXNet，以及谷歌TensorFlow。用户可以很方便地发挥Volta的全部性能。

微软Azure企业副总裁Jason Zander表示：“我们的云计算平台正在部署第二代GPU。我们刚刚宣布了P40和P100系列产品，但我们也很喜欢Volta。我的工作是吸引人们使用Azure云计算平台。人们喜欢使用立即可以上手，不需要等待的产品。我们希望让数据科学家和开发者专注于自己的模型，而不是底层设施。”

亚马逊AWS深度学习和人工智能总经理Matt Wood也在发布会上登台。他表示：“我们感到非常兴奋。在训练和推理两方面，我们都看到了性能的优化。我们很高兴成为产品发布的合作伙伴。”

# DGX

新款DGX-1计算机搭载了8颗Volta GPU，售价为14.9万美元。此外，黄仁勋还发布了新的静音工作站英伟达DGX Station。这款工作站搭载了4颗V100 GPU，带来了每秒480万亿次浮点运算能力，售价6.9万美元。

# Isaac机器人训练模拟器

黄仁勋此次还展示新的Isaac机器人训练模拟器。通过该模拟器，机器人可以在虚拟世界中接受训练，随后部署至真实世界。在演示中，机器人被训练打高尔夫球和曲棍球。黄仁勋还表示，这一技术将使制造、医疗和建筑等行业的机器人训练更安全、更快、成本更低。

黄仁勋指出：“我们需要创造一个替代世界。”这一世界要遵守所有的物理定律，需要看起来真实，需要支持在其中的学习。而与真实世界最主要的不同在于，需要加速运转，使速度更快。

# GPU云

英伟达还推出了GPU云平台(NGC)，可以让开发者跨平台接入最新的深度学习框架，以及利用最新的GPU计算资源。

在深度学习中有两个挑战，一个是需要把所需软件，包括优化后的深度学习框架、库、操作系统和驱动程序等装入一个堆栈之中，二是利用最新的GPU计算资源来训练神经网络。

为了解决这个问题，英伟达推出了NGC。
![](https://pic1.zhimg.com/v2-a32963e60b800ece7a7c947dec6d0cbc_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1742' height='800'></svg>)
NGC堆栈可以提供众多的软件，包括：Caffe,、Caffe2、CNTK、MXNet、TensorFlow、Theano、Torch等框架，以及NVIDIA DIGITS GPU训练系统，NVIDIA深度学习SDK，英伟达docker，GPU驱动和CUDA等。![](https://pic3.zhimg.com/v2-6477d57bd91b510240ef31a615f74d06_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)

只需要一个英伟达账号，无论在PC、DGX或者NGC哪一种平台，用户都能简单的开展深度学习工作流程。一句话，英伟达表示使用NGC以后，深度学习的模型从原型到部署都更为容易，并且灵活使用计算资源。

NGC今年第三季度将开始公开测试，定价也将在稍后公布。

# 与丰田合作

黄仁勋宣布，丰田将在自动驾驶汽车中使用英伟达即将推出的Xavier SOC。这其中包含512核的Volta GPU，能带来每秒30万亿次浮点运算的深度学习计算能力，功耗只有30瓦。

“从许多方面来看，丰田是一家传奇性的公司，发明了许多当代管理系统。”黄仁勋表示，“我们双方的工程团队将合作开发丰田自动驾驶汽车，并在未来几年内上路行驶。”

# 与SAP合作

黄仁勋还谈到了英伟达与SAP的合作。这项合作已帮助SAP开发了多款应用，更好地追踪品牌曝光，自动从企业应付账款系统中提取分类数据，以及分析客户投诉中的非结构化信息，从而更快地找到公司内能解决问题的人。

# Project Holodeck

黄仁勋还介绍了一款新工具Project Holodeck，用于在高度仿真的虚拟现实环境中展开协作。在展示Project Holodeck的过程中，他与科尼塞克创始人Christian von Koenigsegg进行了合作，后者位于瑞典。他们共同检查了价值190万美元Regera超级跑车的内饰。

【完】

P.S. 开头已经提到了，英伟达股价昨晚一路狂飙。放个图感受一下：
![](https://pic1.zhimg.com/v2-57f13eb9d29e3b50c61843a5d98101d8_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1374' height='948'></svg>)
招聘

量子位正在招募编辑记者、运营、产品等岗位，工作地点在北京中关村。相关细节，请在公众号对话界面，回复：“招聘”。


