# 移植了OVRLipSync到UE4 - 逍遥剑客 - CSDN博客
2016年09月23日 12:17:46[xoyojank](https://me.csdn.net/xoyojank)阅读数：5140
[OVRLipSync](https://developer3.oculus.com/downloads/audio/1.0.1-beta/Oculus_OVRLipSync_for_Unity_5/) 是Oculus为Unity开发的一个口型同步插件, 能够实时处理语音数据转换成相应的口型, 按照它的规范, 一共有15个口型: 
![这里写图片描述](https://img-blog.csdn.net/20160923114729698)
![这里写图片描述](https://img-blog.csdn.net/20160923114741214)
![这里写图片描述](https://img-blog.csdn.net/20160923114751535)
上面的截图是基于MorphTarget的, 这个插件也提供了基于贴图的表现形式: 
![这里写图片描述](https://img-blog.csdn.net/20160923114930895)
![这里写图片描述](https://img-blog.csdn.net/20160923115012020)
通过分析插件可以知道, 核心算法封装在OVRLipSync.dll里, 这个dll经过我测试是可以独立运行的: 
![这里写图片描述](https://img-blog.csdn.net/20160923115443085)
之前也有个人尝试把它整合进UE4: [https://github.com/ChairGraveyard/ovrlipsync-example](https://github.com/ChairGraveyard/ovrlipsync-example)
不过这个人没有把效果调对, 因为OVRLipSync.dll是个完全没有任何文档说明的东西, 一些参数的意思只能靠猜. 好在可以与Oculus的人进行邮件上的沟通, 搞懂了所有函数的参数的意义后终于把效果调试正确了. 在此基础上, 我也参考Unity插件的功能, 重写了一些代码, 支持多个实例对象, 并且把处理音频的接口暴露出来, 以便于使用自定义的音源来驱动口型, 如使用一段录音. 
这是在UE4中运行的效果: 
![这里写图片描述](https://img-blog.csdn.net/20160923115946135)
![这里写图片描述](https://img-blog.csdn.net/20160923115957154)
![这里写图片描述](https://img-blog.csdn.net/20160923120007057)
有几个需要注意的地方:
- 由于UE4的麦克风录音是写死的16000的采样率, 所以我在OVRLipSyncInitialize时就设置成了固定的16000的采样率, 至于BufferSize是参考Unity的默认设置1024, 为了延迟也可以自行改小一些
- OVRLipSyncProcessFrame目前的版本(1.0.1)是有Bug的, 被坑了很久, 跟Oculus的人确认现在的版本只有OVRLipSyncProcessFrameInterleaved才能正常工作. 不过使用者是不需要关心这个的, 只需要保证传入UOVRLipSyncContextComponent::ProcessFrame的是16000采样率的单声道音频数据, 并保证BufferSize不小于VISEME_BUF_SIZE. 另外, 音频处理是比较消耗时间的, 所以这个函数设计成线程安全的, 对性能有要求的人可以把ProcessFrame放到另一个线程去调用
- ULipSyncTextureFlipComponent使用前需要设置15张口型贴图 
![这里写图片描述](https://img-blog.csdn.net/20160923120646133)
并在BP里设置嘴的Material和TextureName 
![这里写图片描述](https://img-blog.csdn.net/20160923120824259)
- ULipSyncMorphTargetComponent使用前需要设置MorphTarget的动画名字, 另外SkeletonMesh上也要打上Tag, 默认是”Mouse” 
![这里写图片描述](https://img-blog.csdn.net/20160923121227217)
功能没有多复杂, 使用起来也很方便. 但是UE4这边因为没有像Unity那样OnAudioFilterRead的回调和DSP Buffer, 所以只能设置一个固定的采样率, 这样就对我们的音频数据有了很多限制. 不管怎么说, 能够在运行时进行LipSync计算的免费插件, 目前来说应该就这么一个选择. 好了, 有问题就提到github吧: 
[https://github.com/xoyojank/ovrlipsync-example](https://github.com/xoyojank/ovrlipsync-example)
