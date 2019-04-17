# 行人重识别(Re-ID)与跟踪(tracking)区别 - 别说话写代码的博客 - CSDN博客





2018年07月06日 10:56:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：3240








总结自：https://www.zhihu.com/question/68584669/answer/265070848

作者：陈狄




链接：https://www.zhihu.com/question/68584669/answer/326110383
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



从任务的角度来看，两者最主要的区别如下：
- **行人再识别**：给定一张切好块的行人图像 (probe image, 即图像大部分内容只包含这个人), 从一大堆切好块的图像 (gallery images) 中找到跟probe image中同一身份的人的图像。这些图像通常是由不同摄像头拍摄的不连续帧。
- **行人跟踪**：给定一张切好块的行人图像 (probe image), 从一段全景视频 (panorama track, 视野中只有一小部分是这个行人) 中找到 probe 所在的位置。这段全景视频是由单个摄像头拍摄的连续帧。
&lt;img src="https://pic4.zhimg.com/v2-f0d33f0901e6fcdda2c91c42891af7d7_b.jpg" data-size="normal" data-rawwidth="718" data-rawheight="388" class="origin_image zh-lightbox-thumb" width="718" data-original="https://pic4.zhimg.com/v2-f0d33f0901e6fcdda2c91c42891af7d7_r.jpg"&gt;![](https://pic4.zhimg.com/80/v2-f0d33f0901e6fcdda2c91c42891af7d7_hd.jpg)行人再识别（左）与行人跟踪（右）
在视频监控领域，我们的最终目标是要做到**多目标跨摄像头跟踪 **(Multi-target Multi-camera Tracking, 简称MTMC Tracking). 而行人再识别和行人跟踪都是为了达到这个最终目标的子任务。
&lt;img src="https://pic1.zhimg.com/v2-bc7f0942b1393224d2e19221a85b1fe0_b.jpg" data-size="normal" data-rawwidth="617" data-rawheight="456" class="origin_image zh-lightbox-thumb" width="617" data-original="https://pic1.zhimg.com/v2-bc7f0942b1393224d2e19221a85b1fe0_r.jpg"&gt;![](https://pic1.zhimg.com/80/v2-bc7f0942b1393224d2e19221a85b1fe0_hd.jpg)从行人再识别 (Re-ID) 到跨时段跨摄像头跟踪 (MTMC Tracking)
简单画了个图~ 如上，Re-ID 在图中的第三象限，处理的是**静态图像**，并且是已经**切好块**的patch. 

然而在实际应用中，摄像头拍摄到的都是全景图像，于是就需要加入**行人检测 **(Pedestrian Detection) 技术，从全景图像中找到行人的位置，再将包含行人的图像块切出来。此时就形成了位于第二象限的新任务：**行人搜索 **(Person Search). 

行人搜索处理的对象依然是静态图像，在实际应用中摄像头拍摄到的通常是动态的视频。如果能将时序信息 (Temporal Information) 利用起来，加上**行人跟踪** (Tracking) 技术，特别是 Tracking by Detection 技术，就能大致实现位于第一象限的最终目标 MTMC Tracking.

另一方面，在 Re-ID 的基础上，如果不考虑行人检测，直接将时序信息利用起来，就形成了位于第四象限的任务：**基于视频的行人再识别** (Video-based Re-ID), 有时也被称作 Multi-shot Re-ID. 同样地，将这个任务扩展到全景视频上也能够达到最终目标。

目前大量的工作都集中在第三象限的 Re-ID 上，相比之下 Person Search 和 Video-based Re-ID 的工作就少了很多。直接解决 MTMC Tracking 的工作更是少之又少。各位同僚们一起努力吧~







