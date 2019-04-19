# GDC2016: AMD LiquidVR - 逍遥剑客 - CSDN博客
2016年04月04日 14:40:37[xoyojank](https://me.csdn.net/xoyojank)阅读数：2523
跟NVIDIA的VRWorks一样, AMD也对VR进行了一些优化, 两家的各项技术基本上可以一一对应起来, 只是名字不同 
![这里写图片描述](https://img-blog.csdn.net/20160404133917234)
# Latest data latch
[https://www.youtube.com/watch?v=e_o22yJOgkg](https://www.youtube.com/watch?v=e_o22yJOgkg)
其实就是Timewarp 
![这里写图片描述](https://img-blog.csdn.net/20160404135643600)
# Affinity multi-GPU
对应NVIDIA的VR SLI.  
![这里写图片描述](https://img-blog.csdn.net/20160404140430525)
去年在UE4中整合过VR SLI, 性能提升的确挺明显的, 不过用了两块980Ti, 也挺烧包的. 暂时的意义可能只是开发者用来做次时代画面演示用, 对于消费用户来说并没有什么意义, 因为大多数的VR游戏并不支持这一特性. 或许等DX12普及了, 可以利用集成显卡做一部分的渲染工作提升渲染性能. 
# Asynchronous compute
用来实现[asynchronous timewarp](https://developer.oculus.com/blog/asynchronous-timewarp-on-oculus-rift/), NVIDIA暂时是通过high-priority context实现的, AMD这个还要先进一点, 不过NVIDIA马上就出新显卡了, 值得期待.  
![这里写图片描述](https://img-blog.csdn.net/20160404142340032)
开发者并不需要关注这个, 因为这个特性是由驱动和硬件配合实现的.
# Direct-to-display
对应NVIDIA的Direct Mode, 只对VR硬件厂商开放. 最早Oculus是可以当成Windows的显示器的, 从0.7后就不行了, 就是这个原因. 
# Future directions
![这里写图片描述](https://img-blog.csdn.net/20160404143104957)
sound integration值得期待下. 毕竟现在VR头戴显示器都是接显卡的HDMI的, 本身就包含了音频流, 如果能把空间化的声音处理集成到显卡上, 那CPU那边就少了很多的计算量, 就类似PSVR那个盒子的功能. 
总体看下来, 还少个类似NVIDIA Multi-Resolution Shading的特性, 虽说现阶段没游戏支持, 但是未来对性能的改善会比较明显, 至少在我看来, 这个比Multi-GPU有用多了.  
![这里写图片描述](https://img-blog.csdn.net/20160404143954772)
不得不感叹VR推动了API&操作系统&硬件的改进, 这也是为什么整个行业都需要它, 大家都需要个新的增长点. 
