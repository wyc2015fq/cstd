# WebDNN：浏览器上运行的最快DNN执行框架（Macbook也行） - 知乎
# 



> 唐旭 编译整理
量子位出品 | 公众号 QbitAI

深度神经网络（DNN）已经被证明在图像识别、视频识别、自然语言处理、游戏人工智能等诸多不同领域都具有非常大的潜力，当前，DNN也已经被应用到多种产品之中。

然而，DNN计算所需的成本非常高，并且在通常情况下，DNN的执行需要基于硬件加速的大量执行环境设置进程。因此，要在像笔记本电脑、智能手机这样的终端用户设备上执行DNN并不十分现实。

过去解决这些问题的方法是云计算。而现在，东京大学机器智能实验室(MIL)开发的WebDNN又提出了一条新的途径。

# WebDNN是什么？

WebDNN是一种开源软件框架，用于在浏览器上执行DNN的预训练模型。
![](https://pic2.zhimg.com/v2-2440e5ba37457627b452c0f5c7c26a91_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1500' height='229'></svg>)
WebDNN包含两种模型：图形转译器，用于将训练过的模型转译并优化成能够在浏览器上执行的格式；以及描述符运行器，用于在浏览器上执行转换过的模型。

图形转译器是一种脱机模型，通过python（3.6版本）实现。它会输出包含JavaScript和二进制权数据的“图形转译器”文件。只有应用开发者需要运行这一模型。

描述符运行器则是一种在线模型，用于在终端用户的浏览器上运行图像转译器，通过JavaScript实现。应用开发者需要使用程序库所提供的API来对模型进行输入并显示输出结果。

# 特性

WebDNN主要有以下三种关键特性：

## 免安装

WebDNN在浏览器上执行DNN模型。通常情况下，每个终端用户的设备上都应该已经安装好了浏览器，同时用户会对它非常熟悉。因此，DNN应用的产出会变得更为容易，不会像原生应用安装那么麻烦。

目前市面上存在着几种主流的浏览器，并且它们都有着能够用于加速的不同特性。在每一种浏览器上，WebDNN都有几种能使DNN模型执行进程速度最大化的策略。

## 非开销接口

JavaScript是一种在浏览器上运行的标准编程语言，它通过解释器来执行。因此，它需要一定的计算开销，也无法完全发挥CPU的性能；在GPU上，也有同样的问题。

现在的浏览器都支持WebGL，一种能够实现GPU使用的JavaScript API。然而，这个API是为图形处理而设计，并不适合进行通用计算；除此之外，用WebGL进行通用计算还会增加计算开销。

WebDNN使用了新一代的JavaScript API——包括用于GPU执行的WebGPU，以及用于CPU执行的WebAssembly。这些API能够帮助充分发挥GPU和CPU的能力。

## 针对推理阶段的专门优化

为了实现更快的执行过程，对DNN模型的计算图进行优化非常重要。DNN的执行包括两个阶段：训练阶段和推理阶段，并且它们需要不同的优化策略。WebDNN只着眼于在终端用户设备上执行的推理阶段并支持激进优化。这种优化管道能够被应用于不同DNN框架所训练的模型，不需要编辑训练代码。

# 兼容浏览器平台![](https://pic1.zhimg.com/v2-1967d80fec3cfe07838a6af250327e50_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1184' height='516'></svg>)![](https://pic4.zhimg.com/v2-d85fafd51e476de0cb1fe4a965ad6f43_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='506'></svg>)

**                                                                                                      △** 下方数字为最低版本要求。

通过WebGPU、WebAssembly和Fallback后端，WebDNN能够在所有主流浏览器上运行。

其中，WebGPU后端是三种途径中最快的，但目前该API只支持Safari技术预览版；WebAssembly后端速度次之，能够兼容大部分主流浏览器；Fallback后端则只能进行向下兼容，且速度并不那么快。

在Safari技术预览版中，默认禁用WebGPU API。启用方法是在菜单中点击：“Develop” > “Experimental Features” > “WebGPU”

# 效果

处理时间——与Keras.js对比

（量子位注：Keras.js是GitHub上的一个开源项目，在GPU的支持下，能够用于在浏览器上运行Keras模型。）

测试环境：
- 
Mac Book Pro ，2015年初版本

- 
macOS 10.12.4 Sierra

- 
Intel Core i5 2.7 GHz CPU

- 
16GB内存

- 
Intel Iris Graphics 6100 GPU

- 
Safari技术预览，版本30


模型：VGG16, Inception-v3, ResNet50

输入形状：Inception-v3——(1, 299, 299, 3)，其它——(1, 224, 224, 3)![](https://pic2.zhimg.com/v2-9c5eb1b0a7310f228beaff34654cc579_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1648' height='792'></svg>)

**                                                                                △** 纵轴为每张图像消耗的时间，单位毫秒。

结果显示，搭载了后端WebGPU的WebDNN速度明显快于Keras.sj,而搭载了后端WebAssembly的WebDNN,速度则稍快于搭载了后端GPU的Keras.sj。

在每一个DNN模型及后端，WebDNN在速度上都比Keras.sj更快。当对图形转译器进行优化后，WebDNN的速度将得到进一步提升。

# Demo

WebDNN还有两个官方的Demo。

Demo 1：图像风格迁移模型![](https://pic4.zhimg.com/v2-a3e23ca203b96466f8b1e1ec61ff944b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1073' height='707'></svg>)

**                                                                                                                △** 浮世绘风格的猎豹。

Demo 2：ResNet50图像识别模型![](https://pic4.zhimg.com/v2-15bb35e3a43d6ba37cd707124278e767_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1089' height='684'></svg>)

**                                                                         △** 可能训练得不太好，海豚识别成了大白鲨……

以上两个示例，其预训练模型的执行都是通过WebDNN在浏览器上完成的。

量子位尝试了一下风格迁移的Demo，特别邀请阿凡提老师作为出镜测试嘉宾，结果有点尴尬?，如下图所示：![](https://pic1.zhimg.com/v2-456c30dd29a78ff0fe8e01b1002a7788_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1442' height='1344'></svg>)

我们用的也是Macbook，想测试的同学，可以直接访问这个网址：
[https://mil-tokyo.github.io/webdnn/neural_style_transfer.html?run=1](https://link.zhihu.com/?target=https%3A//mil-tokyo.github.io/webdnn/neural_style_transfer.html%3Frun%3D1)
# 相关地址

GitHub地址：
[https://github.com/mil-tokyo/webdnn](https://link.zhihu.com/?target=https%3A//github.com/mil-tokyo/webdnn)
document：
[https://mil-tokyo.github.io/webdnn/docs/index.html](https://link.zhihu.com/?target=https%3A//mil-tokyo.github.io/webdnn/docs/index.html)
官网（有demo）：
[https://mil-tokyo.github.io/webdnn/#about](https://link.zhihu.com/?target=https%3A//mil-tokyo.github.io/webdnn/%23about)
还有一个插曲：量子位曾经试图寻找该实验室的官网，发现一个类似的页面，不过这个页面嘛……我们截图如下：![](https://pic4.zhimg.com/v2-6fa5045bd51e6adbb8ed140a990ba477_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='211'></svg>)

【完】

> 
### 招聘

量子位正在招募编辑记者、运营、产品等岗位，工作地点在北京中关村。相关细节，请在公众号对话界面，回复：“招聘”。

### One More Thing…

今天AI界还有哪些事值得关注？在量子位（QbitAI）公众号对话界面回复“今天”，看我们全网搜罗的AI行业和研究动态。笔芯~

另外，欢迎加量子位小助手的微信：qbitbot，如果你研究或者从事AI领域，小助手会把你带入量子位的交流群里。



