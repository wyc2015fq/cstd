# TensorFlow推出模型优化工具包，可将模型压缩75% - 知乎
# 



选自Medium,机器之心编译,参与：张倩、王淑婷.

> 近日，TensorFlow 推出了一款新的模型优化工具包。利用该该工具包中的技术可以将模型压缩 75%，最大可将速度提升到原来的 3 倍。

新手和熟练的开发者都能利用这套工具包中的技术来优化待部署和执行的机器学习模型。预计该工具包中的技术将有助于优化所有 TensorFlow 模型以进行部署，但它对于那些为内存紧张、功率和存储受限的设备提供模型的 TensorFlow Lite 开发人员来说帮助最大。

TensorFlow Lite 相关信息链接：[https://www.tensorflow.org/mobile/tflite/](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/mobile/tflite/)。
![](https://pic4.zhimg.com/v2-7394b41f9237c0853869982c911e0f5f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='393'></svg>)优化模型以减少尺寸、延迟和功耗，使准确率损失不明显
添加的第一项支持技术是对 TensorFlow Lite 转换工具的训练后量化（post-training quantization）。对于相关的机器学习模型，利用该技术可以将模型压缩到原来的 1/4，并将速度提升到原来的 3 倍。

开发者可以通过量化模型减少能耗。这点对于在边缘设备中部署（不局限于手机）非常有用。




**实现训练后量化**

训练后量化技术是 TensorFlow Lite 转换工具的一部分。上手非常简单：创建 TensorFlow 模型之后，开发者可以简单地实现 TensorFlow Lite 转换工具中的「post_training_quantize」标记。假设这一保存的模型存储在 saved_model_dir 中，则可以生成量化的 tflite flatbuffer：


```
converter=tf.contrib.lite.TocoConverter.from_saved_model(saved_model_dir)
converter.post_training_quantize=True
tflite_quantized_model=converter.convert()
open(“quantized_model.tflite”, “wb”).write(tflite_quantized_model)
```


TensorFlow 提供了相关教程，深入解析如何做到这一点。未来的目标是将这种技术纳入通用的 TensorFlow 工具中，以便将其部署在目前没有 TensorFlow Lite 支持的平台上。

教程链接：[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/tutorials/post_training_quant.ipynb](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/tutorials/post_training_quant.ipynb)




**训练后量化的好处**
- 模型缩小到原来的 1/4
- 主要由卷积层组成的模型执行速度提高了 10-50%。
- 基于 RNN 的模型得到了 3 倍的加速
- 由于减少了内存和计算需求，预计大多数模型将降低功耗。

下图显示一些模型尺寸减小，执行速度提高（使用单核的 Android Pixel 2 手机上进行的测量）。
![](https://pic4.zhimg.com/v2-f84b5c3efa95da2712301a5e13941693_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='878' height='545'></svg>)图 1：模型大小比较：优化后的模型差不多缩小到原来的 1/4![](https://pic4.zhimg.com/v2-39042b345071cefcef1b41052df4ddcb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='876' height='529'></svg>)图 2：延时比较：优化后的模型速度是原来的 1.2—1.4 倍
加速和模型尺寸减小对准确率的影响很小。一般对于手头任务来说，已经较小的模型（如用于图像分类的 mobilenet v1）可能会损失更多的准确率。对于这些模型，TensorFlow 为其中的大部分提供预训练的完全量化模型。
![](https://pic2.zhimg.com/v2-823d4ac619d80630905995ee2e48f9f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='876' height='537'></svg>)图 3：准确率比较：除了 mobilenets，优化后的模型准确率下降不明显
TensorFlow 期望在未来继续改进结果，所以请参见模型优化指南，了解最新的测量结果。

地址：[https://www.tensorflow.org/performance/model_optimization](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/performance/model_optimization)




**训练后量化如何起作用**

TensorFlow 通过将参数（即神经网络权重）的精度从训练时的 32 位浮点表示降至小得多且高效的 8 位浮点数表示，来进行优化（也称为量化）。详细信息请参见训练后量化指南。地址：[https://www.tensorflow.org/performance/post_training_quantization](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/performance/post_training_quantization)

这些优化将确保结果模型中精度降低的操作定义与使用固定和浮点数学混合的内核实现配对。这将在较低精度下快速执行最重的计算，但在较高精度下执行最敏感的计算，因此通常会导致任务很少甚至没有最终准确率损失，但与纯浮点执行相比，速度显著加快。对于没有匹配的「混合」内核的操作，或者工具包认为有必要的操作，它会将参数重新转换为更高的浮点精度来执行。有关支持混合操作的列表，请参见训练后量化页面。




**未来展望**

TensorFlow 将继续改进训练后量化及简化模型优化过程的其它技术上的工作。这些将整合到 TensorFlow 相关工作流程中，使其更加容易使用。

训练后量化是 TensorFlow 正在开发的优化工具包下的第一个产品，该团队希望得到来自开发者的相关反馈。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://medium.com/tensorflow/introducing-the-model-optimization-toolkit-for-tensorflow-254aca1ba0a3](https://link.zhihu.com/?target=https%3A//medium.com/tensorflow/introducing-the-model-optimization-toolkit-for-tensorflow-254aca1ba0a3)


