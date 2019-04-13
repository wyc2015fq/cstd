
# 创意视觉应用︱基于深度学习的CVaaS计算机视觉即服务案例（Computer Vision as a Service） - 素质云笔记-Recorder... - CSDN博客

2017年05月13日 15:03:20[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2564



> 技术与技法日进千里，快速迭代过程中，真正能够留下的是应用场景的重构与对新商业范式的思考。

### [CVaaS 计算机视觉即服务的理念介绍](https://mp.weixin.qq.com/s?__biz=MzI5MDUyMDIxNA==&mid=2247484289&idx=1&sn=68c6d6a30fd6320c7f45fec51d55a3ea&chksm=ec1fee78db68676e4aa766486929b08d9e29383a1b622dfc7d9673fba54a6a8ea547c3aac4c7&mpshare=1&scene=1&srcid=05153WusTfFiNLk11xAbsQYz#rd)
> 观点来源于：极视角科技联合创始人  罗韵
CVaaS 就是 Computer Vision as a Service, 我们把 CV 的部分标准化成为了一种服务，而每一个行业可以在这里找到自己行业需要的和图像处理、视频处理、计算机视觉相关的算法服务，然后他们可以整合这些算法服务成为他们需要的应用。
![这里写图片描述](https://img-blog.csdn.net/20170613103851304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170613103851304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而 CV 算法更接近于一种平台运行的服务，提供运算性能的横向拓展，提供运算的底层开发环境，甚至乎直接提供可开发测试的 sandbox, 所以，CVaaS 也是 PaaS 的一种。
CVaaS 的目的，就是让各行各业可以以最快的形式和方式完成这要的一些工作的转变。
例如，在零售行业，我们选择可以选择人脸识别做 VIP 识别，选择行人识别做客流统计，选择性别、年龄识别做顾客分类或者顾客肖像。
在安防行业，我们选择动作 (打架) 识别、行人跟踪、姿态识别等做安全的防范和预警。
再例如，在房地产领域做场景图片的分类 (例如哪些图片是卧室，客厅，厨房)，优质 (封面) 图片的挑选；印刷行业根据图片的内容做自动裁剪；等等。
![这里写图片描述](https://img-blog.csdn.net/20170613104011677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.
# 一、深度学习在汽车行业的应用
### ——如何提高分类的精确度或者准确率
> [作者：山同气,来源：知乎](https://www.zhihu.com/question/48979623/answer/114191798)

> 深度学习遵从大数定律，数据越多，规律性越能掌控。

> 1、Fine-Grained Classification细粒度分类

> 比如我去框定，汽车的车灯，汽车的前脸，汽车轮毂等，然后用 cnn或 deep cnn 或what ever其他的分类器做这些的分类，对于分类器来说输入是汽车的车灯+汽车的前脸+汽车轮毂，而不是整张图片。分类器再从车灯前脸等提取高级特征，从而得到一个分类模型。

> 在fine-tuning过程会遇到一个问题，拿来对未知信息图片进行判别，最好是框定过之后的。

> 对于局部图片，我们是这样提取的：
![这里写图片描述](https://img-blog.csdn.net/20170513144228050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 2、级联式做法

> 1、粗粒度——图像清洗。整体识别0/1，是否汽车，数据筛选，清洗垃圾图层，力求98%以上精度，甚至更高，每个分类拿2w张左右；

> 2、中粒度——品牌分类器。不细分到品牌下面的子品牌，譬如康师傅牛肉面，不细分到康师傅辣味、咸味等等诸多口味，太细了你的数据量不够。你即使花了很长时间标注很仔细，但是得不偿失。

> 3、细粒度——车型分类器。每个品牌的不同车型分类器进行区别，这样就简单、精度高很多。也可以训练更精细的，内饰、做工、配色等。

> 这样你会得到一个0.98*0.96*0.94~0.88的识别率，但是这样精确度会变成可调节式，粗粒度时候认定哪些要否定，提高概率的标准，是可以提高整体识别率。

> 3、数据增强

> opencv先做一些图片的变换，比如对光线的处理，去噪等等，这样会提高复杂环境的图片的识别精度。

> 而且，可以改变光线强弱来进行训练

> .

> 二、
> [深度学习在美团点评的应用](https://zhuanlan.zhihu.com/p/25691627)

> 1、图像质量评估

> 在美团点评，商家的首图是由商家或运营人工指定的，如何选择首图才能更好地吸引用户呢？图像质量排序算法目标就是做到自动选择更优质的首图，以吸引用户点击。

> 传统的图像质量排序方法主要从美学角度进行质量评价，通过颜色统计、主体分布、构图等来分析图片的美感。但在实际业务场景中，用户对图片质量优劣的判断主观性很强，难以形成统一的评价标准。比如:

> 1.有的用户对清晰度或分辨率更敏感；

> 2.有的用户对色彩或构图更敏感；

> 3.有的用户偏爱有视觉冲击力的内容而非平淡无奇的环境图。

> 因此我们使用深度学习方法，去挖掘图片的哪些属性会影响用户的判断，以及如何有效融合这些属性对图片进行评价。

> 我们使用AlexNet去提取图片的高层语义描述，学习美感、可记忆度、吸引度、品类等High Level特征，并补充人工设计的Low Level特征（比如色彩、锐度、对比度、角点）。在获得这些特征后，训练一个浅层神经网络对图像整体打分。该框架（如图2所示）的一个特点是联合了深度学习特征与传统特征，既引入高层语义又保留了低层通用描述，既包括全局特征又有局部特征。
![这里写图片描述](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 对于每个维度图片属性的学习，都需要大量的标签数据来支撑，但完全通过人工标记代价极大，因此我们借鉴了美团点评的图片来源和POI标签体系。关于吸引度属性的学习，我们选取了美团Deal相册中点击率高的图片（多数是摄影师通过单反相机拍摄）作为正例，而选取UGC相册中点击率低的图片（多数是低端手机拍摄）作为负例。关于品类属性的学习，我们将美团一级品类和常见二级品类作为图片标签。基于上述质量排序模型，我们为广告POI挑选最合适的优质首图进行展示，起到吸引用户点击，提高业务指标的目的。图3给出了基于质量排序的首图优选结果。

> [

](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 2、OCR技术

> [
](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> OCR在美团点评业务中主要起着两方面作用。一方面是辅助录入，比如在移动支付环节通过对银行卡卡号的拍照识别，以实现自动绑卡，又如辅助BD录入菜单中菜品信息。另一方面是审核校验，比如在商家资质审核环节对商家上传的身份证、营业执照和餐饮许可证等证件照片进行信息提取和核验以确保该商家的合法性，比如机器过滤商家上单和用户评价环节产生的包含违禁词的图片。相比于传统OCR场景（印刷体、扫描文档），美团的OCR场景主要是针对手机拍摄的照片进行文字信息提取和识别，考虑到线下用户的多样性，因此主要面临以下挑战：

> [
](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 成像复杂：噪声、模糊、光线变化、形变；

> 文字复杂：字体、字号、色彩、磨损、笔画宽度不固定、方向任意；

> 背景复杂：版面缺失，背景干扰。

> [
](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 对于上述挑战，传统的OCR解决方案存在着以下不足：

> [
](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 1.通过版面分析（二值化，连通域分析）来生成文本行，要求版面结构有较强的规则性且前背景可分性强（例如文档图像、车牌），无法处理前背景复杂的随意文字（例如场景文字、菜单、广告文字等）。

> 2.通过人工设计边缘方向特征（例如HOG）来训练字符识别模型，此类单一的特征在字体变化，模糊或背景干扰时泛化能力迅速下降。

> 3.过度依赖字符切分的结果，在字符扭曲、粘连、噪声干扰的情况下，切分的错误传播尤其突出。

> [
](https://img-blog.csdn.net/20170513144308128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 针对传统OCR解决方案的不足，我们尝试基于深度学习的OCR。

> 首先，我们根据是否有先验信息将版面划分为受控场景（例如身份证、营业执照、银行卡）和非受控场景（例如菜单、门头图）。

> 对于受控场景，我们将文字定位转换为对特定关键字目标的检测问题。主要利用Faster R-CNN进行检测，如下图所示。为了保证回归框的定位精度同时提升运算速度，我们对原有框架和训练方式进行了微调:

> 考虑到关键字目标的类内变化有限，我们裁剪了ZF模型的网络结构，将5层卷积减少到3层。

> 训练过程中提高正样本的重叠率阈值，并根据业务需求来适配RPN层Anchor的宽高比。
![这里写图片描述](https://img-blog.csdn.net/20170513144337363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170513144337363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 对于非受控场景，由于文字方向和笔画宽度任意变化，目标检测中回归框的定位粒度不够，我们利用语义分割中常用的全卷积网络（FCN）来进行像素级别的文字/背景标注，如下图所示。为了同时保证定位的精度和语义的清晰，我们不仅在最后一层进行反卷积，而且融合了深层Layer和浅层Layer的反卷积结果
![这里写图片描述](https://img-blog.csdn.net/20170513144353144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170513144353144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 为了有效控制字符切分和识别后处理的错误传播效应，实现端到端文字识别的可训练性，我们采用如下图所示的序列学习框架。框架整体分为三层：卷积层，递归层和翻译层。其中卷积层提特征，递归层既学习特征序列中字符特征的先后关系，又学习字符的先后关系，翻译层实现对时间序列分类结果的解码。

> [
](https://img-blog.csdn.net/20170513144353144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 由于序列学习框架对训练样本的数量和分布要求较高，我们采用了真实样本+合成样本的方式。真实样本以美团点评业务来源（例如菜单、身份证、营业执照）为主，合成样本则考虑了字体、形变、模糊、噪声、背景等因素。基于上述序列学习框架和训练数据，在多种场景的文字识别上都有较大幅度的性能提升，如下图所示。

> .

> 三、
> [机器学习在携程酒店图像数据中的应用](http://mp.weixin.qq.com/s?__biz=MjM5MDI3MjA5MQ==&mid=2697266056&idx=1&sn=b5575236afb3e156dae76a3fa90f08e7&chksm=8376fcbcb40175aad9b14411b32da7feadc60d7f1e1bff7fc275ce792c519de809ed4ff969ff&mpshare=1&scene=1&srcid=0506krQxycRHjkYT1wXDv2mw#rd)

> 1、图像快速去重

> 携程酒店图像数据的来源众多，从而导致同一家酒店出现相同/相似图像的几率往往较大。图像重复展示会影响用户的体验，不利于用户获取酒店的完整信息。酒店图像之间的相同/相似主要表现为1）尺寸形变；2）裁剪残缺；3）色彩变化；4）旋转变化；5）拍摄视角移动等多种情况，如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20170513144710427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170513144710427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 为了解决酒店图像之间相同/相似的问题，需要对酒店的图像数据进行去重。然而，由于图像数量巨大，人工去重耗时耗力。因此，通过图像去重技术自动对相同/相似的图像进行判定和去除势在必行。

> [
](https://img-blog.csdn.net/20170513144710427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 图像去重一般分为

> 1）图像特征表达的提取和2）图像之间相似度计算两个主要步骤。

> 对于图像特征表达的提取，常见的手工设计特征有颜色、纹理、HOG、SIFT和SURF等；此外基于深度学习的深层特征表达也经常被使用。对于图像之间相似度计算，常见的无监督距离度量方法有欧式距离、曼哈顿距离和余弦距离等；常见的有监督距离度量方法有LMNN、KISSME、LFDA和MFA等。然而这些方法基于浮点特征计算相似度，计算速度普遍较慢，因此通过哈希学习方法将图像特征转换为二元编码，再利用汉明距离进行相似度的快速计算更加符合工业界对图像数据处理速度的要求。

> 对于酒店中的相同/相似图像，大部分全局特征（比如颜色、纹理和HOG）不能很好地解决图像裁剪残缺和旋转变化等问题；一些局部特征（比如SIFT和SURF）与基于深度学习的特征虽然表达效果较好，但是由于特征提取复杂，计算速度过于缓慢。

> 针对以上特征提取方法存在的缺陷，我们最终采用ORB特征作为图像的特征表达，并使用汉明距离进行相似度的计算。

> ORB特征具有以下优点：

> 1）特征提取速度快；

> 2）在大多数情况下，去重效果能够与SIFT/SURF持平；

> 3）提取的特征直接是二元编码形式，无需使用哈希学习方法就可以直接利用汉明距离快速计算相似度。

> ORB特征在实际的图像去重过程中仍然存在一些不足，比如在处理图像尺寸差异过大、形变和模糊等问题时，去重效果一般。

> 为此我们在提取ORB特征前，首先将图像按照初始的宽高比例统一缩放到一个固定的标准尺寸之内，避免了图像之间的尺寸差异，较好地弥补了ORB特征不具有尺度不变性的缺陷。

> 同时，我们在面对图像形变和模糊问题时，在ORB特征的基础上，进一步融合了颜色直方图和LBP特征等全局特征来进行重复图像判定，令局部和全局的图像信息之间优势互补，降低了形变和模糊等因素对ORB特征的影响，保证了图像去重的准确率。

> .

> 2、水印图像检测

> 携程酒店图像数据的来源众多，同时也导致出现了另一个严峻的问题：带有其他公司水印信息的图像数量增多。人工进行水印图像的检测会花费大量的人力成本，所以我们希望能够利用计算机自动检测出图像中是否含有水印信息，从而避免出现误用和侵权的行为。

> 水印信息在图像中的视觉显著性很低，具有面积小，颜色浅，透明度高等特点，带水印图像与未带水印图像之间的差异往往很小，区分度较低。一些带水印的酒店图像示例如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20170513144722967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 一般的目标检测问题可以看作是一个图像部分区域的分类问题，在深度学习兴起前，可变形部件模型（DPM）一直是流行的目标检测方法，随着深度学习技术的发展，以R-CNN、Fast R-CNN、Faster R-CNN和YOLO等为代表的一系列基于卷积神经网络的目标检测方法成为了主流。然而水印检测和一般的目标检测的区别在于，水印在一幅图像中的位置基本是固定的，因此水印检测可以看作是一个简化的目标检测问题，而其中的关键就是训练水印分类器。

> 在训练水印分类器的过程中，我们遇到的最大问题是没有足够的标注了水印类别的图像数据用于训练。为了解决这个问题，我们选择自主地去生成训练数据。具体地，我们在大量无水印图像中随机截取若干个矩形区域图像，以这些矩形区域图像作为无水印的训练图像数据；同时，我们将要检测的水印信息图形随机缩放后嵌入这些不包含水印信息的矩形图像内，从而形成带水印的训练图像数据。通过这种方式，我们方便快捷地获取了大量的图像训练数据。
![这里写图片描述](https://img-blog.csdn.net/20170513144738764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们在自主生成大量训练数据后，训练了一个专门针对水印分类任务的AlexNet。对于一张待检测图像，我们以一个可变框随机在水印常见的位置（图像的左右下角和中间位置）截取一系列矩形区域图像，然后依次输入分类网络中进行分类，最后融合所有矩形区域图像的分类结果即可获取水印图像的检测结果。完整的水印图像检测流程如上图所示。

> 我们使用计算机自动进行图像快速去重和水印图像检测，两者都达到了99%+的准确率，在实际图像处理中有效地减少了大量人力成本的投入。

> 3、房型图像分类

> 酒店图像根据内容可以分为很多类别，比如外观、内景和房型等。其中，房型图像可以直观地展示出房型的信息，对于用户选择入住房型尤为关键。我们希望优先展示吸引用户的房型内容，以提升用户的体验和下单率。然而在实际中，房型图片往往包含很多内容，人工并没有一张张的进行甄别，所以导致房型首图不合适的情况经常出现。解决房型首图不合适问题的关键是需要将房型图像进行分类，从而能够优先展示吸引用户的内容。具体地，我们把房型图像按照优先级高低分为包含床的图像、不包含床的图像和卫生间的图像三类。

> 随着深度学习技术的出现，尤其是卷积神经网络的兴起，一个理想的房型图像分类方法一般分为两步：1）利用大量已经标注类别的房型图像数据直接训练一个深度卷积神经网络，比如AlexNet、VGGNet、ResNet或者基于Inception的一系列网络等；2）对于一张类别未知的房型图像，将其输入深度卷积神经网络中，网络最后一层直接输出其属于每个类别的概率。

> 和水印图像检测一样，在真正实践的过程中，我们遇到的最大问题依然是缺少标注了类别的房型图像数据用于训练。大量的训练图像如果通过人工标注代价极大，这显然是不现实的。而没有训练图像，却要得到一个分类模型则是更加不现实的。所以我们还是花了一些时间对少量房型图像的类别进行了标注。在这个很小规模的房型图像数据集的基础上，一般有两种分类思路：1）因为房型图像属于场景图像，所以可以提取房型图像的HOG、SIFT和GIST特征，这些手工设计的特征已经被证明在场景分类中比较有效，然后再训练传统分类器实现分类；2）利用深度卷积神经网络强大的特征迁移学习能力，先使用海量图像数据训练一个深度卷积神经网络，然后将该网络作为一个特征提取模型并结合传统分类器实现分类。当然，如果有更多的人力和时间，在标注的房型图像数据较多的情况下，直接对该网络进行微调则是更佳的选择。

> 在实际应用中，我们选择了第二种思路。我们没有借助应用最为广泛的ImageNet数据集，因为该数据集图像的内容和房型图像差异过大，特征迁移达不到最优的效果。为了尽可能的提升网络的特征迁移能力，我们借助了与房型图像最为接近的场景图像数据集，训练VGGNet作为房型图像特征提取器。最后，我们利用自己标注的小规模房型图像数据集，训练支持向量机模型来实现分类。具体的分类流程如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20170513144751421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们的房型图像分类上线后，达到了98%的准确率。下图展示了在房型图像分类上线前后，一家酒店中多个房型的首图变化的例子（红色框为上线前，绿色框为上线后）。
![这里写图片描述](https://img-blog.csdn.net/20170513144803155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 4、图像质量评价

> 在上一节中，我们介绍了通过房型图像分类使含床的房型图像得以优先展示。但是如果一个房型有多张含床的图像，该选哪一张图像作为该房型的首图呢？所以我们希望能够对房型图像的质量进行评价，这样在图像的类别相同时，能够按照质量高低进行排序。更广泛地，我们还希望能对所有的酒店图像都进行质量评价，这样酒店首图的选择、酒店图像的优选展示等都能够以图像的质量分数作为依据。

> 起初我们选择清晰度这一客观指标作为图像质量评价的标准，我们认为清晰图像变模糊丢失的信息要多于模糊图像变模糊丢失的信息。根据这一思想，对于一张图像，我们先将其灰度化，然后获取对应的模糊图像。接下来，我们分别对原图像和对应的模糊图像提取图像边缘信息，具体我们采用拉普拉斯卷积模板进行滤波。最后我们通过比较滤波后的两张图像的方差变化率即可对图像进行清晰度的量化。

> 完整的计算流程如下图所示。图像的清晰度分数的区间在[0,1]之间，如果分数越大则表示图像越清晰。我们对图像的清晰度分数进行分段校验，图像清晰度的评价精度达到了91%。
![这里写图片描述](https://img-blog.csdn.net/20170513144855344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 然而在实际应用中，我们发现仅以清晰度作为图像质量评价的标准还是存在一些不足，因为清晰度高但内容不好看的图像为数不少。这些图像因为清晰度高而被优先展示，但其不好看的内容却影响了用户的感受，所以我们希望能够进一步从美学角度来对图像质量进行评价。

> 图像的美感度是一个非常主观的概念，很难有一个统一的标准去量化，为了能够尽可能准确地计算图像的美感度，我们选择深度卷积神经网络模型来实现美感度评价。在实际应用中，我们又再次遇到了同样的问题：缺少大量标注了好看/不好看标签的训练图像。由于在房型图像分类中，我们利用卷积神经网络强大的迁移学习能力进行特征迁移取得了成功，所以我们决定继续沿用这种方法。

> 因为酒店图像的美感度受到内容、色彩和构图等多方面的影响，所以我们不再像在房型图像分类中那样只使用内容单一的场景图像数据集，而是将包罗万象的ImageNet数据集和场景图像数据集混合进行训练，力求让尽可能多的图像参与深度卷积神经网络的学习，令网络能够记住更多图像的内容，从而进一步提高网络的特征迁移能力。同时，为了保证深度卷积神经网络的特征表达能力，我们采用比AlexNet和VGGNet的层数更深的ResNet作为特征提取器。最后我们为少量图像标注好看/不好看的标签，并训练随机森林实现了图像好看/不好看的二分类模型。

> 我们将图像被模型判为好看的概率作为图像的美感度分数，美感度分数的区间在[0,1]之间，如果分数越大则表示图像越好看。由于美感度评价模型并没有考虑清晰度因素，所以最终我们融合图像的清晰度和美感度来计算图像质量分数。完整的图像质量评价流程如下图所示。通过图像质量评价，我们使得清晰而又好看的图像能够被优先展示，此举对酒店/房型首图的选择、酒店图片的排序等方面都有较好的指导意义。
![这里写图片描述](https://img-blog.csdn.net/20170513144816624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 写在最后

> 以上我们介绍了携程四个真实的图像数据处理需求，但机器学习对于携程酒店图像数据处理的价值远远不限于此。接下来我们将继续在多个图像应用场景进行深入挖掘，比如图像的个性化展示、利用超分辨和解模糊技术提升图像质量等，力求为携程酒店图像数据的智能化贡献一份力量。

> .

> 四、
> [深瞐科技如何以“车脸识别”布局AI+安防](https://mp.weixin.qq.com/s?__biz=MzIzNzUzMzQ3OQ==&mid=2247484584&idx=1&sn=7a35a59e3e24a5495867ca7ad61753ec&chksm=e8c664e0dfb1edf698c98fc266744d63fb2addd2c013f25245d01cf6a000e2fe8dc2b46b943b&mpshare=1&scene=1&srcid=0512ujjeDiNCUz7Nf1dZSDTM#rd)

> 陈瑞军告诉雷锋网：

> 最开始考虑的是做人脸识别，但发现动态人脸识别的精准度还不够，直到现在也是个需要解决的问题。对于深瞐来讲，作为一个初创的新公司，这个方向可能暂时没办法落地，所以就想做一些别人没做过的，结合用户的需求，所以就定了“车脸识别”这个方向。
![这里写图片描述](https://img-blog.csdn.net/20170513145105538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170513145105538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 此后，从最基础的图片识别开始，继而到视频识别，再聚焦到视频中的车辆分析，直到今天形成了以“车辆识别”为核心的软硬件产品布局。总体来看，深瞐科技的产品体系主要包括以下几个方面：

> 视频基因谱引擎

> 视频基因谱引擎是深瞐的核心产品，更通俗的名字叫做“视频结构化”。据深瞐介绍，“视频结构化”这一概念该由公安部第三研究所胡所长提出，指的是把视频数据中的非结构信息转化为结构信息。深瞐的视频结构化产品可以高精度地自动识别不同视角、不同光照条件、不同监控场景、不同天气状况中的人物特征和车辆特征，便于公安机关达到后期快速检索和布控的目的。

> 图像处理引擎

> 深瞐科技的图像处理引擎，主要应用于将复杂情况下的模糊车辆、车牌图片进行清晰化处理，能够做到的有去除噪点、去除重影模糊、图像光照增强、去除雨天模糊、去除镜面反光等。

> 人脸识别引擎

> 基于深瞐在深度学习和模式识别的研究和应用成果，采用人脸检测、跟踪和结构化对比算法模型，被应用于公安机关的人脸比对、检索、身份识别、商业人流大数据等方面。

> .

> 五、图普科技从 “鉴黄” 到视频＆直播个性化推荐

> 来源于
> [雷锋网](https://mp.weixin.qq.com/s?__biz=MTM2ODM0ODYyMQ==&mid=2651424006&idx=2&sn=46e5deef8ae58e5bc73dbf6e92e9627c&chksm=624d85a4553a0cb281563156340bb2c9e56bc2725bcd5078c13914bef2e596ffc7e6218f705f&mpshare=1&scene=1&srcid=0510R74rZWvt3hk9FG3KXhgY#rd)

> 1、帮企业省更多的钱：从 “鉴黄” 到内容审核

> 企业对图普科技的认知更多的也是 “鉴黄”，映客、美拍、小米直播、迅雷、酷狗、唱吧等视频和直播的头部平台，都是图普的 “鉴黄服务” 的客户。

> 图普科技 CEO 李明强告诉雷锋网新智造，其实从一开始，图普提供的就不只是鉴黄，政治敏感信息、暴力恐怖信息和广告的审核都是图普的业务，这些统称内容审核。

> 从去年开始，原来接入了鉴黄服务的客户，开始接受图普的其他内容审核服务。例如，知名的原创糗事笑话 UGC 内容社区糗事百科，就面临着流量巨大，违规广告的人工审核成本极高的问题。

> 和鉴黄的过程类似，清除违规小广告同样基于大量图片进行学习训练。基于糗百的违规图片集中为带有违规文字的图片以及含有推广二维码的小广告图片，图普基于图像识别技术及糗百的广告图片的特征，批量制作具有针对性的广告图进行优化训练，极大的提高了广告图片识别的精确度和准确率，降低了模型迭代的周期。

> 糗百接入图普定制化的广告识别模型后，机器自动识别糗百社区的图片，将图片识别判断为正常、二维码还是带文字图片，再利用 OCR 技术，检测出带文字图片中的文字位置并识别，将识别出来的含有敏感、违规文字的图片自动过滤。大大降低了内容审核成本。

> 现在，已经有越来越多的企业全面接入图普的内容审核平台。最近，图普还与阿里云达成了合作，在阿里云上发布色情图片和暴恐图片识别服务，阿里云的客户可以直接调用。

> 此前，图普曾透露其日处理图片数量已经上涨到 9 亿张左右，其中每万张图片的处理费用为 25 元。李明强告诉雷锋网新智造，去年，图普的营收实现了十倍的增长。

> .

> 2、帮人赚钱：视频大数据标签和个性化推荐

> 从一开始，图普就没有将自己局限在 “鉴黄” 或内容审核内，去年接受雷锋网采访时，李明强给图普的定位是给视频时代建立连接。文字时代，有了计算机对文字的理解就有了内容间的连接，同样，图像和视频被计算机理解后，也能形成内容间的连接。

> 图普已经开始着手这方面的工作。在内容审核之外，图普开始发力视频和直播的大数据标签和个性化推荐服务。

> 大数据标签主要用于短视频和直播平台，系统会通过对海量的标注过的数据的学习，根据主播的行为、场景、人物风格、年龄、性别等，自动为当前直播建立标签。例如，一个喜欢做嘟嘴表情的年轻女孩，会有很大可能被打上 “萌妹子” 的标签。而通过对人工标注的系统的学习，图普甚至可以判断主播的颜值。当然，李明强也解释，和有硬性指标的鉴黄不同，“萌妹子”、“颜值” 这些标签很多时候带有很大的主观因素。
![这里写图片描述](https://img-blog.csdn.net/20170513145423727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170513145433493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170513145440696?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 但是，对于做视频推荐来说，这些基本符合主流审美标准的标签已经足够了：新用户注册后，直播平台可以根据该用户选择关注的标签，在他的首页呈现相应的直播；直播平台还可以根据颜值和热门等综合因素，在首页推荐直播内容；另外，当用户关注的主播不在线时，系统还可以推荐一些和他关注的直播类型相似的主播。

> 李明强还告诉雷锋网新智造，目前还有一个产品在研发中，即在搜索的标签栏，用户可以直接根据标签去搜索。例如用户喜欢跳舞的、场景在 KTV 里的直播，都可以根据相应标签搜索。

> 图普表示，仅这个推荐功能，应用表较好的平台，可以使新用户的留存增加一倍以上；对老用户，也能使其停留时长至少增加 30%-40%。对短视频和直播平台来说，留存增加意味着用户体验的提升，更重要的是，广告、打赏等收入会得到显著提升。

> 如果说之前的内容审核是节约人力成本，为企业省钱的话，视频推荐则是在实打实地帮企业赚钱。

> .

> 六、利用目标检测制作-电子相册

> ——印刷快照行业的改革

> 来源文章：
> [CVaaS计算机视觉即服务 ——从算法,应用到服务的技术演变](https://mp.weixin.qq.com/s?__biz=MzI5MDUyMDIxNA==&mid=2247484289&idx=1&sn=68c6d6a30fd6320c7f45fec51d55a3ea&chksm=ec1fee78db68676e4aa766486929b08d9e29383a1b622dfc7d9673fba54a6a8ea547c3aac4c7&mpshare=1&scene=1&srcid=05153WusTfFiNLk11xAbsQYz#rd)

> 在印刷行业或者快照行业，会陆续推出一项产品——电子相册。

> 而电子相册从技术层面主要是要解决两个问题，1. 照片裁剪，2. 相框的匹配。
![这里写图片描述](https://img-blog.csdn.net/20170613103328911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170613103328911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 而当前，这些工作都是人工去完成，随着日益增长的电子图片的需求量，制作电子相册的人力成本越来越大，而这个时候，利用之前所述的内容识别算法，我们可以帮助电脑自动实现图片的裁剪，因为自动裁剪最大的担忧可能是担心把照片内的人裁剪掉了。

> [
](https://img-blog.csdn.net/20170613103328911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 另一方面，我们进而可以结合图片场景分类和人脸识别等算法技术，使用标签匹配方法去自动匹配与照片本身更搭配的相框。

> 算法本身我们可以做出很多技术，例如使用物体检测我们可以实现内容识别、除此之外我们还实现场景分类、人脸的识别、颜色的分类、人物表情等等。

> 而技术项目的组合，可以帮助我们是去实现更多行业内的目前人工完成的工作，例如实现自动裁剪、通过根据图片的内容、场景的分类、人脸信息等，匹配出合适的相框作为推荐，根据不同颜色的印刷材料做不同的印刷批次排序等等。

> 于是，一个简单的印刷快照行业的升级，我们可以归纳为如图：
![这里写图片描述](https://img-blog.csdn.net/20170613103523296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170613103523296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 而由图中，我们可以看到，技术和应用本质上是完全可以分开做横向拓展的，于是我们可以看到同样的技术可以用在不同的行业，也可以有很多不同行业特定的算法技术，如图：
![这里写图片描述](https://img-blog.csdn.net/20170613103533578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170613103533578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> .

> [
](https://img-blog.csdn.net/20170613103533578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 七、
> [“搜你所想”之用户搜索意图识别——NLP](https://mp.weixin.qq.com/s?__biz=MzA5NzkxMzg1Nw==&mid=2653162439&idx=1&sn=c50c340fb134d303f1abe4303d16c516&chksm=8b4936a9bc3ebfbf0c8ac75a6e9c50b749127540ea66b62f301c99c88594041bfea8441dd5f7&mpshare=1&scene=1&srcid=0515s5yZXUsGoWln19ZDZoyQ#rd)

> .

> 1、用户搜索意图的理解及其难点解析

> 分析一下理解用户搜索词背后的真实意图识别存在哪些难点：

> 用户输入不规范，输入方式多样化，使用自然语言查询，甚至非标准的自然语言。比如上面提到的“附近的特价酒店”

> 、“上海到扬州高速怎么走”都是自然语言查询的例子，又如 “披星 ( ) 月”、“吾尝终日而思矣, 下面“

> 用户的查询词表现出多意图，比如用户搜索“变形金刚”，是指变形金刚的电影还是游戏？ 搜索“仙剑奇侠传”是指游戏还是游戏软件？ 电影？ 小说？

> 电商网站搜索“水”是指矿泉水？还是女生用的护肤水？

> 意图强度，表现为不同用户对相同的查询有不同的需求强度。比如：宫保鸡丁。宫保鸡丁菜，菜谱需求占 90%。宫保鸡丁歌曲，歌曲下载需求占

> 10%。又比如：荷塘月色。荷塘月色歌曲，歌曲下载需求占 70%。荷塘月色小区，房产需求占 20%。荷塘月色菜，菜谱需求占 10%。

> 意图存在时效性变化，就是随着时间的推移一些查询词的意图会发生变化。比如：华为 P10 国行版 3 月 24 日上市。3 月 21

> 日的查询意图：新闻 90%，百科 10%3 月 24 日的查询意图：新闻 70%，购买 25%，百科 5%5 月 1 日的查询意图：购买

> 50%，资讯 40%，其他 10%5 年以后的查询意图：百科 100% 数据冷启动的问题，用户行为数据较少时，很难准确获取用户的搜索意图。

> 没有固定的评估的标准，CTR、MAP、MRR、nDCG

> 这些可以量化的指标主要是针对搜索引擎的整体效果的，具体到用户意图的预测上并没有标准的指标。

> .

> 2、如何识别用户搜索意图

> 一般把搜索意图归类为 3 种类型：导航类、信息类和事务类雅虎的研究人员在此基础上做了细化，将用户搜索意图划分如下类别：

> 导航类：用户明确的要去某个站点，但又不想自己输入 URL，比如用户搜索“新浪网“

> 信息类：又可以细分为如下几种子类型,

> 直接型：用户想知道关于一个话题某个方面明确的信息，比如“地球为什么是圆的”、“哪些水果维生素含量高”。间接型：用户想了解关于某个话题的任意方面的信息，比如粉丝搜索“黄晓明”。建议型：用户希望能够搜索到一些建议、意见或者某方面的指导，比如“如何选股票”。定位型：用户希望了解在现实生活中哪里可以找到某些产品或服务，比如“汽车维修”。列表型：用户希望找到一批能够满足需求的信息，比如“陆家嘴附近的酒店”。

> 资源类：这种类型的搜索目的是希望能够从网上获取某种资源，又可以细分为以下几种子类型,

> 下载型：希望从网络某个地方下载想要的产品或者服务，比如“USB

> 驱动下载”。娱乐型：用户出于消遣的目的希望获得一些有关信息，比如“益智小游戏”。交互型：用户希望使用某个软件或服务提供的结果，用户希望找到一个网站，这个网站上可以直接计算房贷利息。获取型：用户希望获取一种资源，这种资源的使用场合不限于电脑，比如“麦当劳优惠券”，用户希望搜到某个产品的折扣券，打印后在现实生活中使用。

> .

> 3、达观搜索意图识别引擎

> 达观通过 RESTAPI 接口的方式向客户提供基于公有云和私有云的搜索服务。其中语义分析模块包含了对用户 query 意图的离线挖掘和在线预测。
![这里写图片描述](https://img-blog.csdn.net/20170613104631636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170613104631636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 达观文本语义挖掘算法平台是一个融合了多种算法的集成学习平台，既包括经典的 SVM、LR、RF、LDA 等算法，也包括 CNN、RNN、LSTM、BILSTM 等深度学习算法。比如在实践中，我们尝试将线性统计模型 CRF 和神经网络结构 LSTM 相融合的方法，在 LSTM 的输出端将 softmax 与 CRF 结合起来，使用 LSTM 解决提取序列的特征问题，使用 CRF 有效利用了句子级别的标记信息，取得了不错的效果。

> [

](https://img-blog.csdn.net/20170613104631636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 八、
> [算法专题|纺织业智能升级 高精度图像检索—服装检索算法](https://mp.weixin.qq.com/s?__biz=MzI5MDUyMDIxNA==&mid=2247484367&idx=1&sn=f578bfb9beb5d3251d8e0d84123ed928&chksm=ec1fee36db686720c58200dd7ce618764be43336152fcf29db515088aefb46b1e77043b3bbe2&mpshare=1&scene=1&srcid=06091HWhM02TD5EaMFYhbuHs#rd)

> [
](https://img-blog.csdn.net/20170613104631636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 文章来源于极市平台

> 高精度检索算法也已经完成了搜衣和搜布两个产业级应用，后续会扩展更多的实际应用，促进纺织业和服装业更好得智能升级。
![这里写图片描述](https://img-blog.csdn.net/20170613104852393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 高精度检索算法实现了业界领先的图像检索系统，融合了清晰度，尺度，角度，遮挡，光照等多种因素，利用此系统上传服装图片可搜到同款和相似度很高的服装或者布料。
![这里写图片描述](https://img-blog.csdn.net/20170613104918940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 1、算法应用——时尚搭配推荐

> 根据上传的图片智能推荐相关的时尚搭配。
![这里写图片描述](https://img-blog.csdn.net/20170613104948176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 2、纺织行业的搜衣和搜布

> 根据上传的图片检索出同款和在颜色，款式等相似度非常高的服装或者布料，已经应用于产业。
![这里写图片描述](https://img-blog.csdn.net/20170613105021560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 九、人工智能 LOGO 设计

> 来源于文章：
> [真 · 人工智能 LOGO 设计，这次设计师危险了…](http://app.myzaker.com/news/article.php?pk=593aea021bc8e0083f00000f)

> 首先输入你想要制作 logo 的字母

> 选择你觉得不错的 5 个图形

> 选择你喜欢的色系

> 写一句 Slogan，介绍一下你自己

> 选择你喜欢的 ICON

> logo 就这么生成了！
![这里写图片描述](https://img-blog.csdn.net/20170613105240342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 十、美工终结者「鲁班智能设计平台」是如何工作的？

> 来源文章
> [《UCAN2017回顾!美工终结者「鲁班智能设计平台」是如何工作的？》](http://www.uisdc.com/alibaba-ai-how-it-works)

> 我们要让机器学习设计，首先必须要让机器理解感知设计是什么。以这样一张非常常见的广告为例，在机器的眼里是有一堆像素点组成的。如果今天以像素为单位让机器去理解设计，对设计的可控性非常弱，所以在前期技术方案选择中没有走像素级生产，而是走向了元素级生产。

> 1、设计框架组成

> 四个组成部分：设计框架、元素中心、行动器、评估网络

> 组成一，设计框架。还是以这个广告为例，首先通过人工标注的方式，让机器理解这张设计有哪些元素组成，比如它的商品主体，花的背景，蒙版。往上一层，我们通过设计的经验知识，定义一些设计的手法和风格。手法指的是这些元素为什么可以这么构成，这个知识在设计脑子里机器是不知道的，所以会有手法这一层做输入。最上面这一层是风格，当这些元素构成之后，它从美学或者视觉角度看是一个什么感受。让机器知道它是用什么组成的，为什么可以这么组成，以及它组成后的效果。这个过程将一个设计问题转化成数据问题，这就是设计数据化。

> 下一步是准备设计的原始文件，比如一系列花朵和设计方法，输入到深度学习系列网络。这个网络有一个很大特点：具备一定记忆功能。因为设计是个步骤很复杂的过程，经常有好几十步才能完成一个设计。

> 经过这层神经网络学习之后，我们会得到一个设计框架。从技术上理解，它是一堆空间特征和视觉特征构成的模型。设计师的视角来理解的话，它相当于设计师脑子里面在做一组设计之前大概的框架印象，比如今天你接到一个任务要做一个花朵风格，思考这个设计大概会怎么做，然后从一堆文件里提取出了特征模型出来。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-5.jpg)
> 组成二，元素中心。因为我们做的是元素级生成，所以必须准备一个元素的库。我们会通过收集一些版权图库，以及自己造设计元素的方式，输入到一个元素的分类器。这个分类器会把这些元素分布到各个类型里，比如背景、主体、修饰，也会完成图片库的提取。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-6.jpg)
> 组成三，行动器。接下来，就是设计的具体过程。比如今天我们接到一个设计任务，要为这样一件衣服设计一个花朵风格的广告。这时候会有一个行动器，负责把前面准备好的底料放到设计框架里。这个过程和下围棋很像，左边是棋盘，右边是下围棋的棋子。行动器就是把元素放到棋盘里，这是整个行动器的生成原理。

> 它很像设计师实际在做设计的过程，如设计师要做一个花朵的时候，也在软件里面会不断去调每个位置、每个像素、每个角度。同时，整个过程也是一个强化学习的过程，行动器会在不断试错中更智能。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-7.jpg)
> 组成四，评估网络。设计成品出来之后，我们要告诉机器人，从设计的角度是好还是不好。我们有一个设计评估网络，最终实现的效果就是给它输入任何一个设计成品，它能打个分。技术原理是，我们通过人工输入大量历史上投放过的一些设计图评分，它从这里训练出一个打分的模型出来。同时，专家也会人工干预打分，完成双向反馈。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-8.jpg)
> [ ](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-8.jpg)

> .

> [

](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-8.jpg)
> 2、视觉设计的四个层次

> [
](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-8.jpg)
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-10.jpg)
> 第二层，场景表达。今天你给它一个东西，它能理解，能表达对。比如今天你根据情人节，这些品牌能够找到一种合适的设计手段，去表达出情人节的温馨，这种手绘的方式会稍微难一点，也就是我们前面讲到的语义这一层。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-12.jpg)
> 第三层，创意洞见。它能够有一些启发性的东西出来。天猫品牌里面经常有把猫头和品牌创意做联合的事情，这是机器不可能做到的事情，或者在我有生之年没有指望它能做到的。
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-13.jpg)
> [ ](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-13.jpg)

> 最后一层，创造趋势。这一般是设计大师做的事情。它能定义明年、未来几年的设计趋势走向，这是更高的设计能力。比如今年“三八”女王节，天猫用了一种全新的设计手段，用这种很轻的质感、很细腻的方式来表达商品。它能够代表一个新的趋势和未来，代表一个新的手段，这件事情一定是人来做的。

> [
](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-13.jpg)
![这里写图片描述](http://image.uisdc.com/wp-content/uploads/2017/05/uisdc-ucan-20170511-14.jpg)
> .

> 十一、修正老电影或电视剧的画面

> 来源AI100文章
> [《他在Google Brain实习了一年，总结出这么些心得》](https://mp.weixin.qq.com/s?__biz=MzI0ODcxODk5OA==&mid=2247486076&idx=1&sn=ec3ae5c79b9950724433561087c6e08b&chksm=e99d3985deeab0939758aa30f48d77a091328213db8d7e24d62cb4162bee68dbb79aa0bb5b7d&mpshare=1&scene=1&srcid=06090F7VdTdaMp2sPstqja34#rd)

> 1、超分辨率的像素递归

> 放大照片图像时把相关像素所构成的合理图形呈现出来，这还是有可能做到的。

> PixelCNN，它也比较有戏。（等我们启动之后，用GAN解决来超分辨率问题的SRGAN就发布了，它生成的结果非常好。）

> PixelCNN是一种奇怪的反直觉模型。它将图像生成问题重写成每次选择一个像素序列。像LSTM（长短时记忆网络）这样的门控制递归网络在序列生成方面是非常成功的，它通常会用在单词或字符上。PixelCNN巧妙地构建出一个卷积神经网络（CNN），它能基于先前的像素的概率分布来精确生成像素。这是RNN和CNN的混合功能。
![这里写图片描述](https://img-blog.csdn.net/20170613112214904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 意外的是，PixelCNN所生成的图像看起来非常自然。不像艰难平衡两种目标的对抗网络，该模型的目标只有一个，因而面对超参数的变化，它有更好的稳健性。也就是说，它更易于优化。

> 如何定量评估结果，则是另外一个难题。如何才能证明我们的图像比基准模型好？衡量超分辨率质量的典型方法，是对比强化图像与原始图像的对应像素点之间的距离（峰值信噪比，PSNR）。虽说本模型输出的脸部图像在质量上明显更好，但在像素对比上，平均看来它们还不如基准模型所输出的模糊图像。我们尝试用PixelCNN本身的相似度测量来证明我们的样本比基准版本有着更高的概率值，但同样失败了。最后，我们把这项任务众包给人类评估员——询问他们哪些图像看上去更真实。这倒奏效了。

> .

> 2、PixColor: 关于着色的尝试

> Slim的创造者Sergio Guadarrama一直在尝试给图像着色。他跟我说过一个试验：用分量接口（该接口中图像的灰度、颜色相互分离）获取一张224×224×3的图像，将其颜色通道降至28×28×2的超低分辨率，然后用双线性插值法再把颜色通道放大，所得图像与颜色分辨率很高的原始图像相比几乎没有差别。
![这里写图片描述](https://img-blog.csdn.net/20170613112421082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 把问题变成仅预测低分辨率颜色，我们就可以简化着色问题。我原本已准备好彻底放弃PixelCNN了，因为它显然无法放大小图像，但用来生成28×28×2的图像还是很可行的。通过将颜色数值简化为32个数字而非256，我们进一步简化了着色问题。

> 十二：微博：TensorFlow在微博的大规模应用与实践

> 来源：
> [机器学习与微博：TensorFlow在微博的大规模应用与实践](https://mp.weixin.qq.com/s?__biz=MzA5NzkxMzg1Nw==&mid=2653162781&idx=1&sn=88608b239f07eae89b491a42179ebe41&chksm=8b493073bc3eb965da04bc48cee5fbccfe98a1d151d5097e20dc2002d76135664ca07c9ce60c&mpshare=1&scene=1&srcid=0808dUC4NPDA4iFieYnG4DRy#rd)

> 图像质量也是用户体验的基本要求。

> 用户可以容忍不感兴趣的图片，但很难容杂乱的图像。

> 例如左边的美女图，看起来赏心悦目，手机上刷过，即使不停下细看，也不会反感。

> 右边的图片，里面也是美女，但加上文字之后，立刻变得杂乱，版式与酒店里的小卡片相仿。很可能被认定为骗子。

> 明星脸识别是微博的特色，有海量的明星图片，也有巨大的识别需求。

> 明星脸识别有特别的困难：常用人脸识别研究所用的照片表情、造型较少，不同人之间的差别较大。而明星表情丰富，造型多变，无论男女都化妆！不少人妆容近似，有些整容脸连人脑都傻傻分不清，计算机就更难分清了。
![这里写图片描述](https://img-blog.csdn.net/20170903134107258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170903134221550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 其他NLP相关的文章：

> 1、
> [阿里自然语言处理部总监分享：NLP技术的应用及思考](http://www.toutiao.com/i6416480031240897026/?tt_from=weixin_moments&utm_campaign=client_share&from=timeline&app=news_article&utm_source=weixin_moments&isappinstalled=0&iid=9974398589&utm_medium=toutiao_android&wxshare_count=10&pbid=35949334502)

> 2、
> [电商客服自动问答系统的商品意图识别](http://xueshu.baidu.com/s?wd=%E7%94%B5%E5%95%86%E5%AE%A2%E6%9C%8D%E8%87%AA%E5%8A%A8%E9%97%AE%E7%AD%94%E7%B3%BB%E7%BB%9F%E7%9A%84%E5%95%86%E5%93%81%E6%84%8F%E5%9B%BE%E8%AF%86%E5%88%AB&tn=SE_baiduxueshu_c1gjeupa&cl=3&ie=utf-8&bs=paperuri%3A&f=8&rsv_bp=1&rsv_sug2=0&sc_f_para=sc_tasktype%3D%7BfirstSimpleSearch%7D&rsv_n=2)

> 3、
> [专访玻森数据：这款基于NLP技术的企业情报系统，商业化现状是这样的](http://mp.weixin.qq.com/s?__biz=MzIxMzY0OTQyNQ==&mid=2247484089&idx=1&sn=0b1391a982a5033bfff71e4b55aa7ea4&chksm=97b2d3c7a0c55ad1dc3419cf3b0003369d4f0e281a776e82791a6796c5e3d3137bd4a135f03a&mpshare=1&scene=1&srcid=0506YZguzT62ScVHYO7mnORR#rd)


