# 资源帖丨用PyTorch实现Mask R-CNN - 知乎
# 



> 伊瓢 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

FACEBOOK人工智能实验室何恺明博士领衔的Mask R-CNN喜提ICCV 2017最佳论文后，吸引了大量业内研究者的兴趣。

10个月后，FAIR官方公布了源代码，以及基于Caffe 2的开源物体检测平台Detectron。

但是，再好的“黑科技”也有人吐槽啊，比如，MIT的周博磊博士就曾经在知乎爆料，FAIR的实习生们对不常使用的Caffe 2一直叫苦不迭；

还有人翘首以盼官方PyTorch版本；

还有人想起TensorFlow成熟的可视化工具和文档，看着Caffe 2就开始思考人生……

“Caffe 2，从安装到放弃……”

还好最近，量子位发现有人做出了Mask R-CNN的PyTorch版本：
![](https://pic2.zhimg.com/v2-3a7f386f43c2b1a03add0dd4a3bc3451_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='840'></svg>)
开源代码的贡献者是印度理工学院的小哥哥Sai Himal Allu。
![](https://pic2.zhimg.com/v2-64225ed4d681c9df6c6ff28bb902ac09_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)△ 作者Sai Himal Allu
**Mask R-CNN**

不过说到这里，还是需要先介绍一下前面提到的目标检测大杀器Mask R-CNN。

Mask R-CNN可以说是从目标检测领域R-CNN系列的四代目了，FACEBOOK人工智能实验室（FAIR）团队以何恺明（Kaiming He）和Ross Girshick（RBG）为首的一众目标检测大佬不断更迭了许多个版本：
- R-CNN：对每个候选区域进行特征提取，能提升一定的效率；
- Fast R-CNN：一张图像上使用一次AlexNet，然后再得到不同区域的特征的新方法，比R-CNN快了200多倍；
- Faster R-CNN：Fast R-CNN的升级版，训练时间和测试时间缩短十倍；
- Mask R-CNN：像素级目标检测，不仅给出其边界框，并且对边界框内的各个像素是否属于该物体进行标记。 
![](https://pic1.zhimg.com/v2-afddfd523e6398a28a63f2c0b56ed584_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='795'></svg>)
Mask R-CNN在MS COCO测试集的实例分割、边界框对象检测、以及人体关键点检测三项任务上，都取得了顶尖的成绩。

它的单模型边界框（bounding box）检测成绩，比上一代——Fatser-RCNN与ResNet-101和FPN结合所取得的，还要高3.6个百分点。

于是，Mask R-CNN一经推出，广受好评。 
![](https://pic4.zhimg.com/v2-1e1e83d80359132cf1c678a48cf0a18f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='530'></svg>)
**传送门全家桶**

GitHub: A PyTorch implementation of the architecture of Mask RCNN, serves as an introduction to working with PyTorch
[https://github.com/wannabeOG/Mask-RCNN](https://link.zhihu.com/?target=https%3A//github.com/wannabeOG/Mask-RCNN)

Mask R-CNN:
[https://arxiv.org/abs/1703.06870](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.06870)

Detectron:

[https://github.com/facebookresearch/Detectron](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/Detectron)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


