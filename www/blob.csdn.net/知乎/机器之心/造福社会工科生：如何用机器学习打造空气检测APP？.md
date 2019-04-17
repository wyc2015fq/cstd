# 造福社会工科生：如何用机器学习打造空气检测APP？ - 知乎
# 



> 大城市的空气污染早已不稀奇，「立霾」都被调侃为一个节气。为提高大家对空气污染的意识，提高环境保护的觉悟，来自印度的几个小哥哥利用[机器学习](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757267%26idx%3D2%26sn%3D301bf3d0822bf7d1acde34b42cece51d%26chksm%3D871a9cedb06d15fb720c3f6bfc408a86ffaa191de65cf4cc705633a9e6c5c142810f8651e378%26token%3D1491778879%26lang%3Dzh_CN)设计了一款 APP 来检测空气质量。

选自medium，作者：Prerna Khanna、Tanmay Srivastava、Kanishk Jeet，机器之心编译，参与：高璇、淑婷。

像德里这样的大城市可能会遭受空气污染，尤其在冬季。如「清晨寒冷的德里，空气质量依然很糟糕」这样的标题常会出现在报纸头条。冬季空气质量差会导致雾霾产生，这会限制市民的户外活动，诱发健康问题。
![](https://pic2.zhimg.com/v2-cc346c2251638d0c56c6a27b74a7a1c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)使用手机评估 PM 2.5
作为工科生，我们努力用科技造福社会。解决空气污染问题的关键第一步是让市民能够自己监测空气质量。

这可以通过污染传感器实现，但是大规模部署该传感器成本高昂。我们的目标是设计一个可靠、价格公道的空气质量评估解决方案，让每个人可以直接用智能手机实现。

《Particle Pollution Estimation Based on Image Analysis》等研究表明，通过使用相机图像，可以有效地利用机器学习来评估空气质量，尽管以前的研究通常局限于静态相机的图像。

我们的目标是开发基于 Android 的移动应用程序，利用智能手机相机图像提供本地的实时空气质量评估。来自 Marconi Society 的 Celestini Project India 给了我们很大的启发，并为我们提供了在德里印度理工学院（IIT Delhi）实习的机会，以及开发所需的资源。
[空气质量估计_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/w083847chz4.html)
*应用程序运行 demo*

我们研究的重点是用「PM 2.5」（直径不大于 2.5 微米的颗粒）来预测空气质量。为了将结果可视化，我们预测 PM 2.5 值并将其映射到颜色渐变的空气质量指数（AQI）表中。这是每个国家政府制定的标准，然后根据 AQI 值预警。

**使用 TensorFlow Lite 预测空气质量**

我们开发的应用程序从手机相机收集图像，然后在设备上利用 Tensorflow Lite 处理图像，得到 AQI 估计。在开发应用程序之前，我们在云上训练了 AQI 评估模型。在 Android 应用程序中，使用 Firebase ML Kit 能自动下载该模型。

下面将详细描述该系统：
- 移动应用程序。用于获取图像和预测 AQI 值。应用程序可以在手机上处理图像。
- TensorFlow Lite 用低精度的数据类型进行计算（当带宽受限时，对下载速度有优势），用训练好的机器学习模型在手机上进行推理。
- Firebase。从图像中提取的[参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757267%26idx%3D2%26sn%3D301bf3d0822bf7d1acde34b42cece51d%26chksm%3D871a9cedb06d15fb720c3f6bfc408a86ffaa191de65cf4cc705633a9e6c5c142810f8651e378%26token%3D1491778879%26lang%3Dzh_CN)（如下图所示）将发送到 Firebase。每当新用户使用该 APP 时，都会为其创建一个唯一的 ID。这可以用于以后为不同地理位置的用户定制机器学习模型。
- Amazon EC2。我们使用这些参数和来自地理位置的 PM 值训练当前模型。
- ML Kit。训练好的模型被托管至 ML Kit 上，并自动加载到设备上，然后使用 TensorFlow Lite 运行。
![](https://pic4.zhimg.com/v2-6c540f24b963ff7c44a355bfa01a5c37_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='369'></svg>)
**两个模型**

下面将介绍关于如何分析图像以预测 AQI 的更多细节。我们训练了两个基于图像的机器学习模型来构建应用程序：第一个模型根据用户上传照片的特征预测 AQI，第二个模型过滤掉不包含天空区域的图像。

**AQI 模型**

我们利用以下特征根据用户照片预测 AQI。这些特征通过传统的图像处理技术提取，然后由线性模型结合。第二个模型（稍后讨论）直接处理图像，这在深度学习中很常见。

传输：描述场景衰减和经过空气粒子反射后进入手机摄像头的光量。公式如下：
![](https://pic4.zhimg.com/v2-b06eaae5cd25015a7cccd661633e68bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='744' height='86'></svg>)
其中 I 是观察到的模糊图像，t 是从场景到相机的传输，J 是场景亮度，A 是 airlight 颜色矢量。

利用暗通道的概念发现了单个模糊图像的传输，暗通道假设所有室外图像中至少有一个颜色通道存在为零或极低的像素。对于无雾图像 J，暗通道是：
![](https://pic2.zhimg.com/v2-73cd2c7516f5f9f1f0586de95047e3b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='100'></svg>)
其中 Jc 是 J 的颜色通道之一，Ω(x) 是集中在 x 附近的局部图像块。利用天空或最亮区域可以估计出 airlight，所以可以得到传输：
![](https://pic1.zhimg.com/v2-7f144db4afd69b792ffa23387f499b9c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='528' height='114'></svg>)
其中 Ic(y)/A 是由 airlight A 归一化的模糊图像，右边第二项是归一化模糊图像的暗通道。

天空蓝度：此特征有点类似于我们观察天空来判断污染。如果天空是灰色的，我们认为今天空气质量差。蓝度估计利用 RGB 分割来估算。

天空梯度：天空可能因云层覆盖而呈现灰色，因此考虑到这种可能性，我们加入该项特征。通过制作天空区域的掩模来计算梯度，然后计算该区域的拉普拉斯算子。

熵，RMS 对比度：这些特征告诉我们图像中包含的细节。如果有空气污染，图像会丢失细节。RMS 对比度被定义为图像像素强度的标准差。RMS 对比度的等式如下：
![](https://pic1.zhimg.com/v2-e0e4244df24fa1be0a5124d4557ec5c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='798' height='202'></svg>)
式中，Iij 是大小为 MxN 的图像像素 (i,j) 处的强度，avg(I) 是图像所有像素的平均强度。因此，对比度和 PM 2.5 成反比关系。可使用以下公式估算熵：
![](https://pic3.zhimg.com/v2-220596b7c08793d9ac5a50de211b01ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='570' height='146'></svg>)
其中 pi 是像素强度等于 i 的概率，M 是图像的最大强度。随着 PM 浓度增加，图像逐渐失去其细节，并且图像熵降低。因此，它与 PM 2.5 成反比关系。

湿度：通过研究可知，空气越潮湿，污染程度越高，因为 PM 2.5 会吸收水分并降低能见度。
![](https://pic3.zhimg.com/v2-f0687ae572a65695d5769bcdec201292_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='340' height='185'></svg>)
**Skyline 模型**

最开始发布应用程序时，人们好奇它是否能够用来预测室内外的 AQI。我们的模型能够预测图像是否包含至少 50％的天空区域，并且通过二元分类器接受超过 50% 的图像。

我们利用[迁移学习](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757267%26idx%3D2%26sn%3D301bf3d0822bf7d1acde34b42cece51d%26chksm%3D871a9cedb06d15fb720c3f6bfc408a86ffaa191de65cf4cc705633a9e6c5c142810f8651e378%26token%3D1491778879%26lang%3Dzh_CN)创建了这个分类器，并使用 TensorFlow Hub 在我们标记的数据集上重新训练了模型。数据集由两类组成：500 张天空区域为 50% 的图像，540 张不包含天空区域（或低于 50%）的图像。这些图像的场景包括房间、办公室、花园、室外等。我们使用 MobileNet 0.50 架构并在 100 个未见过的样本上进行测试，准确率达 95％。TF for Poets 有助于图像再训练。

再训练模型的混淆矩阵如下：
![](https://pic2.zhimg.com/v2-a1d0806be0c1c4134132df16e0f1fb71_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='169'></svg>)![](https://pic4.zhimg.com/v2-e466d84e7bd1e0fcddd3e087fd93b577_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='420'></svg>)左：天空区域大于 50% 的图像。右：天空区域小于 50% 的图像
**为每个用户自定义模型**

我们意识到每个用户都需要自定义的 ML 模型，因为每个智能手机的相机规格不同，为了训练这样的模型，我们收集了每个用户的图像。

我们决定结合两个模型的结果，其中一个是基于图像的模型，一个是使用气象参数的时间模型。在基于图像的机器学习模型进行训练时，使用气象参数的时间模型有助于实现更高的推理精度，为用户提供一定结果。而基于图像的机器学习模型帮助我们为特定用户自定义模型；从而通过减少估计误差，提高推理精度。

为每个用户创建一个小型训练数据集，需要从 7 张图像中提取特征并用于训练。图像必须是连续 7 天的，其中一半的图像包含天空，没有太阳或其它直接光源。从图像中提取特征后，利用特征训练回归模型。该模型是线性的，因为所有图像特征或多或少与 PM 2.5 值成线性比例。

在创建训练数据集和模型之后，再创建用于测试的第二组图像。一旦数据集具有 7 天的图像特征，测试就开始了。如果 7 天的训练 RMSE 小于 5，则模型将被冻结并发送到 ML Kit，ML Kit 可以从应用程序中下载。如果 RMSE 不小于 5，则会收集更多的训练数据。
![](https://pic1.zhimg.com/v2-4aee91592ea710a4d1c31a366c54d7dc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)图像特征与 PM 2.5 的关系
**气象参数**

此外，我们还利用气象数据，用时间模型基于最近位置的历史 AQI 来预测 AQI，时间模型补充了基于图像的模型，提高了推断的准确性。我们从 2015 年至 2017 年的政府网站收集了德里的气象数据集，利用 LASSO 优化进行岭回归，选择影响 PM 2.5 水平的关键参数。选择的关键参数是：前一小时的 PM 2.5 浓度，各种气体的浓度，如二氧化氮、二氧化硫、臭氧和露点。然后将数据分开进行训练和测试。我们使用 2015 年 1 月至 2017 年 1 月的数据进行训练。使用 2017 年 1 月至 2017 年 6 月的数据进行测试。我们在数据集上的准确率达到了 90％。
![](https://pic2.zhimg.com/v2-942db8edaceb7bac7c0287c52ff69871_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='560'></svg>)折线图表示 21 天内 3 个模型给出的 RMS 误差值
以下代码有助于我们在 Android 上使用 TFLite。下一个挑战是为每个用户托管基于自适应图像创建的模型。为了解决这个问题，我们通过 Firebase ML Kit 找到了一个有趣的解决方案。它允许自定义和自适应的 ML 模型托管在云端和设备上。

代码地址：[https://codelabs.developers.google.com/codelabs/tensorflow-for-poets-2-tflite/#0](https://link.zhihu.com/?target=https%3A//codelabs.developers.google.com/codelabs/tensorflow-for-poets-2-tflite/%230)

**展望未来**

我们打算在未来对此应用进行以下改进：
- 生成夜间拍摄照片的结果。
- 扩展到其它城市。
- 使模型在各种天气条件下都具有鲁棒性。
- 我们开展这个项目的目的是提高人们对污染的意识。我们希望随着时间的推移，大家都能积极采取措施，共同提高空气质量。
- Air Cognizer 应用程序可以从 Play 商店中搜索获得。 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*原文链接：*[https://medium.com/tensorflow/air-cognizer-predicting-air-quality-with-tensorflow-lite-942466b3d02e](https://link.zhihu.com/?target=https%3A//medium.com/tensorflow/air-cognizer-predicting-air-quality-with-tensorflow-lite-942466b3d02e)


