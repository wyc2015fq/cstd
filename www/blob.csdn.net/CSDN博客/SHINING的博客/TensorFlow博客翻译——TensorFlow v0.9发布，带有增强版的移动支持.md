# TensorFlow博客翻译——TensorFlow v0.9发布，带有增强版的移动支持 - SHINING的博客 - CSDN博客
2016年07月13日 09:44:44[snsn1984](https://me.csdn.net/snsn1984)阅读数：4449标签：[TensorFlow																[ios																[mobile																[移动																[发布](https://so.csdn.net/so/search/s.do?q=发布&t=blog)](https://so.csdn.net/so/search/s.do?q=移动&t=blog)](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=ios&t=blog)](https://so.csdn.net/so/search/s.do?q=TensorFlow&t=blog)
个人分类：[TensorFlow](https://blog.csdn.net/snsn1984/article/category/6223558)
## [TensorFlow v0.9 now available with improved mobile support](https://developers.googleblog.com/2016/06/tensorflow-v09-now-available-with.html)
Monday, June 27, 2016
Posted by Pete Warden, Software Engineer
When we started building TensorFlow, supporting mobile devices was a top priority. We were already supporting many of Google’s mobile apps like Translate, Maps, and the Google app, which use neural networks running on devices. We knew that we had to make mobile a first-class part of open source TensorFlow.
当我们开始构建TensorFlow的时候，支持移动设备是一个首要选项。我们已经支持了很多Google移动Apps,比如：翻译，地图和使用神经网络并运行在设备上的Google应用。我们知道我们必须让移动支持成为开源的TensorFlow的第一流的部件。
TensorFlow has been available to developers on Android since launch, and today we're happy to add iOS in v0.9 of TensorFlow, along with Raspberry Pi support and new compilation options.
从一开始，TensorFlow就可以支持Android开发，现在我们非常高兴的把对iOS的支持添加到TensorFlow的v0.9版本中，与此同时，我们还支持了树莓派和一些新的编译选项。
To build TensorFlow on iOS we’ve created a set of scripts, including a makefile, to drive the cross-compilation process. The makefile can also help you build TensorFlow without using Bazel, which is not always available.
为了在iOS上构建TensorFlow，我们创建了一些列的脚本，包括一个驱动交叉编译进程的makefile。这个makefile也可以帮助你在不使用Bazel的情况下构建TensorFlow，因为Bazel经常不太好用。
![](https://1.bp.blogspot.com/-7PffdHtJYnM/V3F2DyuipuI/AAAAAAAACEU/9b9jVZUbzHcMIzQ0nz8x-JiWqvPIHl7nQCLcB/s640/image01.png)
All this is in the latest [TensorFlow](http://tensorflow.org/) distribution. You can read more by visiting our [Mobile TensorFlow guide](https://www.tensorflow.org/mobile.html) and the documentation in our [iOS samples](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/ios_examples) and [Android sample](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/android). The mobile samples allow you to classify images using the ImageNet Inception v1 classifier.
所有的这些特征都已经在最新的TensorFlow发布版里了。你可以通过访问我们的[Mobile TensorFlow guide](https://www.tensorflow.org/mobile.html)和我们[iOS samples](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/ios_examples) 和 [Android sample](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/android)的文档获取更多的信息。这些移动端的例子允许你使用ImageNet Inception v1 classifier去归类图片。
![](https://3.bp.blogspot.com/-9jt-eK34Qc8/V3F2JeEMHkI/AAAAAAAACEc/nnZyusgsblgfDvJCr2PSMOCUcU8e6DwBwCLcB/s640/image00.png)
These mobile samples are just the beginning---we'd love your help and your contributions. Tag social media posts with #tensorflow so we can hear about your projects!
这些移动端的例子只是开始，我们欢迎你的帮助和你的贡献。请在社交媒体的发布上打上#tensorflow的标记，这样我们可以接收到你的项目。
See the full TensorFlow 0.9.0 release notes [here](https://github.com/tensorflow/tensorflow/blob/r0.9/RELEASE.md).
查看TensorFlow 0.9.0的完成版发布通告请点[这里](https://github.com/tensorflow/tensorflow/blob/r0.9/RELEASE.md)。
本博客是原创翻译，如果转载请提前获得本人同意。
