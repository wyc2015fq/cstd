# UE4学习笔记(六): 次世代的移动平台渲染技术 - 逍遥剑客 - CSDN博客
2014年10月19日 23:30:11[xoyojank](https://me.csdn.net/xoyojank)阅读数：8778
在2014年, UE4一下子拿出了3个移动平台的TechDemo, 分别是
- Soul: [http://www.youtube.com/watch?v=jzGRbGb-fog](http://www.youtube.com/watch?v=jzGRbGb-fog)
- Zen Garden: [http://www.youtube.com/watch?v=OK_gtNJ0Olk](http://www.youtube.com/watch?v=OK_gtNJ0Olk)
- Rivalry: [http://www.youtube.com/watch?v=JaSgf9k7eYQ](http://www.youtube.com/watch?v=JaSgf9k7eYQ)(PC版本移植)
![](https://img-blog.csdn.net/20141019222450278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20141019222331109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从2012到2014的三年时间里, 移动GPU的运算能力提升了十几倍之多
从硬件GFLOPS来看: [http://kyokojap.myweb.hinet.net/gpu_gflops/](http://kyokojap.myweb.hinet.net/gpu_gflops/)
目前移动设备的高端GPU的运算能力直逼Xbox360和PS3, 虽然可能现阶段内存带宽仍然受限, 但通过一定的优化, 达到AAA级的画面已经变成现实
也就是说, 未来一到两年内, 移动设备上完全可以出现媲美Xbox360和PS3画面的游戏, 这也意味着移动游戏即将进入一个精品时代, 相应的制作门槛也会变高
扯远了, 回到技术上
硬件的进化不仅仅是性能, 从API来说, MRT和FP16/FP32纹理的支持意味着渲染管线不再只能是简单的Forward Rendering, 而是可以实现Deferred Rendering(现阶段带宽还是不行, 未来可以考虑), 更多的Post Effect也会随之而来
不过, 现阶段UE4的Mobile Renderer还是加入了大量预计算, 这样可以在性能和画面上都取得不错的效果表现:
- Physically Based Shading Model- 能在移动设备上实现与PC版本共用材质, 这比UE3是一个巨大的进步(制作时不需要再调两套参数)
- [https://www.unrealengine.com/blog/physically-based-shading-on-mobile](https://www.unrealengine.com/blog/physically-based-shading-on-mobile)
- HDR & Linear Color Space- FP16的frame buffer, 不支持该格式的使用RGBA8模拟
- [http://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting](http://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting)
- HDR Directional Lightmaps- 也叫Radiosity Normal Mapping(RNM), UE3中同HalfLife2的技术, 可以把NormalMap的光影一起烘焙了
- [http://www2.ati.com/developer/gdc/d3dtutorial10_half-life2_shading.pdf](http://www2.ati.com/developer/gdc/d3dtutorial10_half-life2_shading.pdf)
- Signed Distance Field Shadows- 同样是预计算, 可以实现高质量的软阴影
- [http://www.tomlooman.com/distance-fields-unreal-engine/](http://www.tomlooman.com/distance-fields-unreal-engine/)
- Image Based Lighting- 使用RGBM编码的Cubemap, 但是一个物体同时只能使用一张, 所以可能会在相邻反射上出现点小瑕疵
- Post Processing Pipeline- Depth of Field
- Vignette
- Bloom
- Light Shafts
- Film Tonemapping
- Grain
- Anti-Aliasing(非MSAA)
- Terrain- Continuous LOD
![](https://img-blog.csdn.net/20141019232341547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141019232352343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以Soul Demo为例, 可以看到已经不输PC游戏的技术水准:
- 490MB的安装包
- 400次绘制/帧
- 40万的三角形
# 参考资料
https://de45xmedrsdbp.cloudfront.net/Resources/files/GDC2014_Next_Generation_Mobile_Rendering-2033767592.pdf
