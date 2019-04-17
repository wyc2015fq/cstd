# 【OCR技术】大批量生成文字训练集 - 知乎
# 



如果是想训练一个手写体识别的模型，用一些前人收集好的手写文字集就好了，比如中科院的这些数据集。

[http://www.nlpr.ia.ac.cn/databases/handwriting/Offline_database.html](https://link.zhihu.com/?target=http%3A//www.nlpr.ia.ac.cn/databases/handwriting/Offline_database.html)

但是如果我们只是想要训练一个专门用于识别印刷汉字的模型，那么我们就需要各种印刷字体的训练集，那怎么获取呢？借助强大的图像库，自己生成就行了！

**GitHub地址：[https://github.com/AstarLight/CPS-OCR-Engine/blob/master/ocr/gen_printed_char.py](https://link.zhihu.com/?target=https%3A//github.com/AstarLight/CPS-OCR-Engine/blob/master/ocr/gen_printed_char.py)**

先捋一捋思路，生成文字集需要什么步骤：
- 确定你要生成多少字体，生成一个记录着汉字与label的对应表。
- 确定和收集需要用到的字体文件。
- 生成字体图像，存储在规定的目录下。
- 适当的数据增强。

第三步的生成字体图像最为重要，如果仅仅是生成很正规的文字，那么用这个正规文字集去训练模型，第一图像数目有点少，第二模型泛化能力比较差，所以我们需要对字体图像做大量的图像处理工作，以增大我们的印刷体文字数据集。

我总结了一下，我们可以做的一些图像增强工作有这些：
- 文字扭曲
- 背景噪声（椒盐）
- 文字位置（设置文字的中心点）
- 笔画粘连（膨胀来模拟）
- 笔画断裂（腐蚀来模拟）
- 文字倾斜（文字旋转）
- 多种字体

做完以上增强后，我们得到的数据集已经非常庞大了。

现在开始一步一步生成我们的3755个汉字的印刷体文字数据集。

## **一、生成汉字与label的对应表**

这里的汉字、label映射表的生成我使用了pickel模块，借助它生成一个id:汉字的映射文件存储下来。
这里举个小例子说明怎么生成这个“汉字：id”映射表。

首先在一个txt文件里写入你想要的汉字，如果对汉字对应的ID没有要求的话，我们不妨使用该汉字的排位作为其ID，比如“一二三四五”中，五的ID就是00005。如此类推，把汉字读入内存，建立一个字典，把这个关系记录下来，再使用pickle.dump存入文件保存。

## **二、收集字体文件**

字体文件上网收集就好了，但是值得注意的是，不是每一种字体都支持汉字，所以我们需要筛选出真正适合汉字生成的字体文件才可以。我一共使用了十三种汉字字体作为我们接下来汉字数据集用到的字体，具体如下

图：
![](https://pic3.zhimg.com/v2-15a61be40e43b7f94898b4910328ea6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='708' height='267'></svg>)
当然，如果需要进一步扩大数据集来增强训练得到的模型的泛化能力，可以花更多的时间去收集各类汉字字体，那么模型在面对各种字体时也能从容应对，给出准确的预测。

## **三、文字图像生成**

首先是定义好输入参数，其中包括输出目录、字体目录、测试集大小、图像尺寸、图像旋转幅度等等。



![](https://pic3.zhimg.com/v2-afb0ce4eb4e739c71ec25ef7073e1f26_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='894'></svg>)



接下来需要将我们第一步得到的对应表读入内存，因为这个表示ID到汉字的映射，我们在做一下转换，改成汉字到ID的映射，用于后面的字体生成。
![](https://pic1.zhimg.com/v2-a9439bc9c0f70bbed57c692c993d1f90_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='684' height='244'></svg>)



我们对旋转的角度存储到列表中，旋转角度的范围是[-rotate,rotate].



![](https://pic1.zhimg.com/v2-796e560b4fb2d8381c5aea6b6d432138_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='685' height='189'></svg>)



现在说一下字体图像是怎么生成的，首先我们使用的工具是PIL。PIL里面有很好用的汉字生成函数，我们用这个函数再结合我们提供的字体文件，就可以生成我们想要的数字化的汉字了。我们先设定好我们生成的字体颜色为黑底白色，字体尺寸由输入参数来动态设定。



![](https://pic4.zhimg.com/v2-7dc9df7d010bde209869b71c9b65bbc7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='705' height='133'></svg>)


![](https://pic3.zhimg.com/v2-881ce7bcb4f0ddc950bbe5683f523262_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='685' height='799'></svg>)
我们写两个循环，外层循环是汉字列表，内层循环是字体列表，对于每个汉字会得到一个image_list列表，里面存储着这个汉字的所有图像。



![](https://pic4.zhimg.com/v2-d3fa596ecc044dd3f8ab50346706746f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='682' height='225'></svg>)
我们将image_list中图像按照比例分为训练集和测试集存储。
![](https://pic2.zhimg.com/v2-77214e24e3200162a5d1a0d29dbb23dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='681' height='314'></svg>)
写好代码后，我们执行如下指令，开始生成印刷体文字汉字集。
![](https://pic2.zhimg.com/v2-2e91db727faa147d0eb5144df1063ebd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='678' height='44'></svg>)
解析一下上述指令的附属参数：
- --out_dir 表示生成的汉字图像的存储目录
- --font_dir 表示放置汉字字体文件的路径
- --width --height 表示生成图像的高度和宽度
- --margin 表示字体与边缘的间隔
- --rotate 表示字体旋转的范围，[-rotate,rotate]
- --rotate_step 表示每次旋转的间隔

生成这么一个3755个汉字的数据集的所需的时间还是很久的，估计接近一个小时。其实这个生成过程可以用多线程、多进程并行加速，但是考虑到这种文字数据集只需生成一次就好，所以就没做这方面的优化了。数据集生成完我们可以发现，在dataset文件夹下得到train和test两个文件夹，train和test文件夹下都有3755个子文件夹，分别存储着生成的3755个汉字对应的图像，每个子文件的名字就是该汉字对应的id。随便选择一个train文件夹下的一个子文件夹打开，可以看到所获得的汉字图像，一共634个。

dataset下自动生成测试集和训练集



![](https://pic4.zhimg.com/v2-cfa2d9ba4778f62acdbc08923dc7bb7b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='208' height='29'></svg>)



测试集和训练集下都有3755个子文件夹，用于存储每个汉字的图像。
![](https://pic3.zhimg.com/v2-0fe42c8303d6b47126d5ddd2b04aeb5e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1068' height='598'></svg>)
生成出来的汉字图像
![](https://pic2.zhimg.com/v2-9ad855f61b1b03677dc1179f1bbbf9f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='563'></svg>)
## **额外的图像增强**

第三步生成的汉字图像是最基本的数据集，它所做的图像处理仅有旋转这么一项，如果我们想在数据增强上再做多点东西，想必我们最终训练出来的OCR模型的性能会更加优秀。我们使用opencv来完成我们定制的汉字图像增强任务。

因为生成的图像比较小，仅仅是30*30，如果对这么小的图像加噪声或者形态学处理，得到的字体图像会很糟糕，所以我们在做数据增强时，把图片尺寸适当增加，比如设置为100×100，再进行相应的数据增强，效果会更好。

## **噪点增加**
![](https://pic1.zhimg.com/v2-1fb58eac29dc85b26b09e64028c36b6c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='886' height='822'></svg>)



输入指令
![](https://pic3.zhimg.com/v2-69fdb535037f884b7586ba18b3e79c62_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='71'></svg>)
使用这种生成的图像如下图所示，第一数据集扩大了两倍，第二图像的丰富性进一步提高，效果还是明显的。当然，如果要获得最好的效果，还需要调一下里面的参数，这里就不再详细说明了。
![](https://pic3.zhimg.com/v2-63b047697e548e79fd4951a4b664755a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='497'></svg>)
至此，我们所需的印刷体汉字数据集已经成功生成完毕，下一步要做的就是利用这些数据集设计一个卷积神经网络做文字识别了！完整的代码可以在GitHub上获取。

再次送上传送门：

[https://github.com/AstarLight/CPS-OCR-Engine/blob/master/ocr/gen_printed_char.py](https://link.zhihu.com/?target=https%3A//github.com/AstarLight/CPS-OCR-Engine/blob/master/ocr/gen_printed_char.py)


> 来源 | 博客园 作者 | Madcola

关注V信公众号：**七月在线实验室**，查看更多BAT面试题，干货资料等

![](https://pic3.zhimg.com/v2-24ae2ebcd1e0c3e40f8b726dd31aaa6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='258' height='258'></svg>)扫描二维码 获取更多信息
**今日学习推荐：**
为了让小伙伴们更好的学习，我们为你准备了**机器学习、OCR文字识别实战、CV就业、推荐系统就业、AI就业班**相关方面知识，可免费试听喔，详情点击下方学习~
[机器学习集训营 第八期「线上线下结合，线下在北上深广杭沈济郑成武西长十二城」- 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-0d5b82db757beb2dcc9eb1c23938bf99_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/train8%3Fv%3Dm1)[OCR文字识别实战 [国内首套全面公开OCR技术的实战课程] - 七月在线​www.julyedu.com![图标](https://pic3.zhimg.com/v2-a5f964d8f54e9a2a46dafeec421c6d12_ipico.jpg)](https://link.zhihu.com/?target=https%3A//www.julyedu.com/course/getDetail/138)[CV就业班 第一期「BAT大咖小班教学，保证就业」- 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-4e6d94ebbf70294325a2e6e6a9f27d61_ipico.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/Employment/gjob)[推荐系统就业班 第二期「BAT大咖小班教学，保证就业」- 七月在线​www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com/employment/rs2)[AI就业班 四期「BAT大咖一对一高端私人定制」 - 七月在线​www.julyedu.com](https://link.zhihu.com/?target=https%3A//www.julyedu.com/weekend/aijiuye4)

