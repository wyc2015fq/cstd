# F8大动作：PyTorch 1.0现身（Logo也换了），围棋AI开源 - 知乎
# 



> 夏乙 若朴 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-5b5f6e017cd130131f2153442660be8d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='648'></svg>)
F8开发者大会第二天，Facebook亮出了一系列AI大动作。

Caffe2和PyTorch合体事件有了清晰的发展方向，同样服务于深度学习开发者的ONNX也宣布支持更多的框架。

另外，Facebook开源了视频理解、自然语言处理的模型，开源了围棋AI ELF OpenGo，还展示了一个打星际的AI。

## **PyTorch**

v0.4.0正式版发布没几天，Facebook在F8开发者大会第二天宣布将发布PyTorch 1.0，还提前展示了这款新框架的特性。

这个框架，还换了LOGO：
![](https://pic2.zhimg.com/v2-90c09b535a928f3beaba59afac083dd9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='600'></svg>)
深度学习框架Caffe2的作者 [@贾扬清](https://www.zhihu.com/people/7b05ff0c9118c26df2b5b133d6dfbc46)[，在知乎上将这一版本的发布总结为Caffe2 + PyTorch = PyTorch 1.0。](https://zhuanlan.zhihu.com/p/36353529)

也就是将原本两款框架面向研究的和面向生产的特性结合了起来。
![](https://pic3.zhimg.com/v2-f6bc35c40e3a473fe1343b28f350e69a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
Facebook介绍说，PyTorch 1.0结合了Caffe2和ONNX模块化、面向生产的特性，和PyTorch自身灵活、面向研究的特性结合起来，为广泛的AI项目提供了一个从科研原型到生产部署的快速、无缝途径，让用户可以快速实验，通过一个能在强制执行模式和声明执行模式之间无缝切花的混合前端优化性能。

除了将研究和生产特性结合起来，PyTorch 1.0还将ONNX（开放神经网络交换）包含进来。ONNX是Facebook去年联合多家软硬件公司发布的神经网络模型转换协议，现在，它新增了对苹果的Core ML、百度PaddlePaddle、高通SNPE的支持，再加上原本支持的MXNet、Caffe2、PyTorch、TensorFlow、CNTK等框架，实现了神经网络模型在各种主流框架之间的转换。

PyTorch 1.0 beta版将在今年夏天和用户见面。

不过，Facebook内部已经用上了。官方称，Facebook多款产品和服务都在大规模应用这个新框架，它每天要处理60亿次文本翻译任务。

PyTorch最初亮相于1年多以前，Facebook的另一款深度学习框架Caffe2，则在去年的F8大会上正式发布。

不过今年4月，Caffe2已经宣布全部代码并入PyTorch。接下来的几个月里，两款框架原本的组件将深度结合，成为一个单独的软件包。

就在上周，PyTorch发布了v0.4.0版本，将Tensors（张量）和Variables（变量）合并，新增了零维张量，还开始了对Windows系统的官方支持。

展示PyTorch 1.0的同时，Facebook还开源了一部分研究成果。比如用于视频理解的ResNext3D模型将于6月发布，视频行为识别模型Res 2+1今天就已经开源，PyTorch中的自然语言理解库Translate也将开源。
![](https://pic2.zhimg.com/v2-f1894be2b10a83bea75640856158ded1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='575'></svg>)
发布了这么多资源和工具，去哪找呢？Facebook还为旗下所有的AI资源推出了一个网站：

[https://facebook.ai/developers](https://link.zhihu.com/?target=https%3A//facebook.ai/developers)

## **围棋AI开源下载**

在F8大会上，还开源了一个围棋AI：ELF OpenGo。

这个AI是Facebook团队对DeepMind技术的一个重现，最近他们选择与四名排名世界前30的人类高手对战，取得了14-0的胜利。

和AlphaGo一样，这个AI的重点也并不只是下围棋，而是想要更好的解决问题。现在ELF OpenGo已经可以开源下载。
![](https://pic1.zhimg.com/v2-58678448c8ad1aea33b47dfbacec8888_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='809'></svg>)
对此， [@田渊栋](https://www.zhihu.com/people/9819f6938be0d3bb133ad0151eefd188)[在知乎上有更详细的解答](https://zhuanlan.zhihu.com/p/36353764)：

> 我们最近改进了ELF框架，并且在上面实现了DeepMind的AlphaGoZero及AlphaZero的算法。用两千块GPU训练约两到三周后得到的围棋AI，基本上超过了强职业的水平。我们和韩国棋院合作进行了一次测试，给这个AI单卡每步50秒搜索时间（每步搜索8万个局面），给人类棋手任意长时间思考，结果AI以14比0完胜。参与测试的棋手包括金志锡，申真谞，朴永训及崔哲瀚，在这里我们非常感谢他们的合作，大家都很尽力，一些棋局下了三四个小时极其精彩。应棋手们的要求，这14局棋谱中的12局不久将公开。
另外我们也和现在著名的LeelaZero比较了下。我们采用了LeelaZero除ponder外的缺省配置（约一分钟一步），及4月25日的公开权重(192x15, 158603eb)，结果我们的AI以200比0获胜。在此我们非常感谢Leela团队的工作，对于他们的开源精神，我们表示由衷的敬意。
这次我们将训练代码，测试代码及训练出来的模型（224x20）全部公开，首要目的是贯彻我们一直以来坚持的开源方针，让AI为全世界服务。其次是对于AlphaGoZero及AlphaZero这样非常优秀的算法，我们想要提供一个可重复的参考实现，让全球的研究者们能在这上面继续改进，充分发挥自己的创造力。最后是借此机会推广一下我们的ELF平台和PyTorch深度学习框架，希望更多的人能使用和完善它。
感谢大家的支持！
田渊栋，龚渠成&马子嫯（Jerry Ma）, Shubho Sengupta, 陈卓远，Larry Zitnick

ELF OpenGo代码及模型的地址：

[https://github.com/pytorch/ELF](https://link.zhihu.com/?target=https%3A//github.com/pytorch/ELF)

## **其他**

在F8大会上，还展示了一个可以**打《星际争霸》的AI**，Facebook也计划随后开源这一项目。星际争霸和围棋一直也都是Facebook团队研究的方向。

还有一项突破研究。基于35亿张用户已打标签（17000个）的公开图像，Facebook成功训练了一个图像识别系统，这比之前只能用手动打标签的5000万张图片训练相比，提高了系统的识别能力，在ImageNet上获得了创纪录的高分（准确率85.4%）。

更多信息，可以参考这个页面：

[https://code.facebook.com/posts/1700437286678763/](https://link.zhihu.com/?target=https%3A//code.facebook.com/posts/1700437286678763/)

此外，F8大会上还展示了AR和VR方面的进步。

Facebook已经创建了一个原型系统，可以生成效果惊人的三围重建画面。下面这个视频，展示了正常的视频与3D重建画面的比较，几乎难以分辨左右哪个画面为真。（友情提示：左边露出操作员脚部的是真实世界）
![](https://pic1.zhimg.com/80/v2-bee6cd0cb075735ecd3a53c1b8281688_b.jpg)https://www.zhihu.com/video/975292735460225024
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


