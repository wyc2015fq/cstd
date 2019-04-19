# 一些VR渲染优化方法 - 逍遥剑客 - CSDN博客
2015年09月25日 11:32:07[xoyojank](https://me.csdn.net/xoyojank)阅读数：8930

VR渲染需要左右眼两幅不同的画面，现在的大部分引擎都是暴力的直接渲染两遍，这样做想想性能也很难达到75FPS（或90FPS）
以Oculus DK2为例，1920x1080@75FPS，加上Super Sampling就变成了（UE4默认135%）2592x1458@75FPS
如果是Oculus的消费者版本和HTC Vive，分辨率和刷新率更是提升到了2160x1200@90FPS， 推荐配置是GTX980
以140%的Super Sampling为标准的话，只是Color Buffer每秒的数据量就有2160x1200x1.4x1.4x90x4 byte ≈ 1.7GB
这还没算Post Processing里的N多张Render Target和Deferred Rendering的GBuffer、Light Buffer
性能永远是VR渲染的最具有挑战的部分，本着能省一点是一点的思路，VR的渲染优化我总结了这么一些：
虽然VR渲染需要左右两幅画面， 但是有很多效果是不需要画两次的：
- Shadow Map
- 部分的Reflection
- Occlusion Query
- 大多数Post Processing
API层面的优化， 有这么几个思路：
- 如果实现了多线程的渲染，一般会有一个Command Buffer，直接分别以不同的View提交两次
- 针对每个物体分别提交两次， 相比上面这个State切换开销会节省一些
- 使用Geometry Shader直接把Mesh分成左右眼的，drawcall不会翻倍了。但是坑爹的GS性能不咋地
- 使用Instancing一次drawcall绘制两个Viewport, 跟GS类似，但性能大约是GS的3倍
这只是减少一些API调用、State切换还有Vertex处理的消耗，那瓶颈最大的Pixel处理的消耗怎么减少呢？
Valve使用一个Stencil Mesh, 剔除了17%的像素
![](https://img-blog.csdn.net/20150925112705106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20150925112500620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20150925112700437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
NVIDIA的GameWorks也提供了一种方法， 叫Multi-Resolution Shading，大概的思路就是边缘的像素经过变形后会损失一些，另外人眼对视线中心的像素更敏感，所以周围一圈可以降低分辨率来渲染。通过这种方式可以节省25%到50的像素
![](https://img-blog.csdn.net/20150925112806415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20150925112810675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20150925112719405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
硬件方面，NVIDIA和AMD都推出了双GPU渲染的支持，即每块GPU渲染一只眼睛的画面。嗯，这一定是个阴谋，他们肯定在偷着乐：这下显卡不愁卖了
![](https://img-blog.csdn.net/20150925112654362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
SONY的PS VR在PS4的机能下实现了120FPS。听起来不可思议，实际是60FPS通过reproject插值出中间帧，跟Killzone的[Temporal Reprojection](http://www.redgamingtech.com/killzone-shadow-fall-resolution-temporal-reprojection-shadow-fall-1080p/) 和Oculus的Timewrap差不多原理
参考资料：
[Fast Stereo Rendering for VR - Google Slides - Google Docs](https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0CBoQFjAAahUKEwj2p_Sq5o_IAhWTGY4KHQidCb0&url=https%3A%2F%2Fdocs.google.com%2Fpresentation%2Fd%2F19x9XDjUvkW_9gsfsMQzt3hZbRNziVsoCEHOn4AercAc%2Fedit&usg=AFQjCNEIr5HG92jH_ti3pQ5sED8k_fYrgA)
['Advanced VR Rendering' by Alex Vlachos (Valve) - Steam](http://media.steampowered.com/apps/valve/2015/Alex_Vlachos_Advanced_VR_Rendering_GDC2015.pdf)
[GameWorks VR Presentation - NVIDIA Developer](https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&cad=rja&uact=8&ved=0CCwQFjACahUKEwiJzrXk-4_IAhWUSo4KHVclAL8&url=https%3A%2F%2Fdeveloper.nvidia.com%2Fsites%2Fdefault%2Ffiles%2Fakamai%2Fgameworks%2Fvr%2FGameWorks_VR_2015_Final_handouts.pdf&usg=AFQjCNEGJCVdW7DsAo5oCMa9vkp0gFzV-A)
[Asynchronous Timewarp Examined](https://developer.oculus.com/blog/asynchronous-timewarp-examined/)
