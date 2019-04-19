# HDR效果 - 逍遥剑客 - CSDN博客
2008年04月28日 04:14:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4189
HDR, High Dynamic Range, 高动态范围
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Processes.JPG)
<?xml:namespace prefix = v /><?xml:namespace prefix = o />
三项处理:
1.Glare, 即Bloom/Glow, 一种发光的效果, 包括曝光处理
2.Streaks, 即Star-like glow, 星光效果
3.Ghosts, 即Lens Flares, 镜头光晕
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Flow.jpg)
通常情况下, 只做第一种处理, 这样的HDR效果上并没有差太多, 但是效率比原来有很大提高, 也不需要那么多的RenderTarget. 这就是应用最广的Fake HDR(伪HDR).
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Fake_Flow.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Fake_Passes.JPG)
Gaussian Filter(高斯模糊), 分u, v两个方向进行效果比较好
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Gaussain_Bluring.jpg)
最后的Tone Mapping的曝光度要控制得当, 不然画面会失真
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Tone_Mapping.jpg)
<?xml:namespace prefix = w />
最终效果
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/HDR_Bloom_Effect.jpg)
