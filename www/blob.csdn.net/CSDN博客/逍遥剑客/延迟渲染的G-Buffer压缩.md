# 延迟渲染的G-Buffer压缩 - 逍遥剑客 - CSDN博客
2012年03月18日 00:53:13[xoyojank](https://me.csdn.net/xoyojank)阅读数：7623
最近渲染器终于稳定下来了, 效果也做得差不多了, 于是做一下总结
当初为了追求进度和效果, 直接就是采用最暴力的A16R16G16B16F x 3的G-Buffer:
|COLOR0|Normal.x|Normal.y|Normal.z|Depth|
|----|----|----|----|----|
|COLOR1|Diffuse.r|Diffuse.g|Diffuse.b||
|COLOR2|Specular.r|Specular.g|Specularb|Specular  Glossy|
可以说没有什么特殊的, 效果是出来了, 性能很是问题
所以想投入实际使用还是要优化一下性能, 对G-Buffer减肥
最苦逼的是渲染风格改了, 材质信息又增加了Emissive(4分量)和Half-Lambert(3个参数)
RTT的格式当然还是选择最快的A8R8G8B8, 但是8bit的通道只能保存[0, 1]范围的值, 精度只有256级
Normal可以只保存两个分量, 参考[http://aras-p.info/texts/CompactNormalStorage.html](http://aras-p.info/texts/CompactNormalStorage.html), 用的method#1
虽然理论上有偏差, 但效果可以接受, 我要的是效率
Depth 8bit肯定不行, 之前想过用两个8bit通道保存, 实败了, 解出的值不是连续的, 所以把其中一个RTT的格式改成了R16G16F(又少了一个通道的说...)
MRT并不强制格式一样, 只要位数一样就好了(当然还有Hardware的INTZ什么, 考虑到兼容性还没试[http://aras-p.info/texts/D3D9GPUHacks.html#depth](http://aras-p.info/texts/D3D9GPUHacks.html#depth))
其中用了一个技巧做position的重建:[http://blog.csdn.net/xoyojank/article/details/5294575](http://blog.csdn.net/xoyojank/article/details/5294575)
为这个把光照计算全部转移到ViewSpace去计算, 节省了几条GPU指令
Diffuse没法省
Specular牺牲掉了颜色, 只保留强度和范围, 需要两个参数, 缩放到[0,1]的范围写入两个8bit通道
Half-Lambert光照的结果就是一个寻址1D纹理的UV.x(float), 是不是有这个效果需要一个bool(写入符号, 如+代表有, -代表没有)
Emissive牺牲掉颜色, 采用Diffuse的颜色, 强度也压缩一下写一个8bit通道
Rimlight计算在写入G-Buffer之前并入Diffuse, 这样理论上既有错误也有损失, 但是效果也是可以接受的, 毕竟是个辅助效果
最后G-Buffer被我搞成了这个样子:
|COLOR0|Depth|[+/-][Lambert.u]| | |
|----|----|----|----|----|
|COLOR1|Diffuse.r|Diffuse.g|Diffuse.b|Emissive|
|COLOR2|Normal.x|Normal.z|Specular|Specular Glossy|
嗯, 要是再加材质信息, 那我就哭了, 实在没地方存了.....
另外, 最后合成的输出范围最好是HDR(A16R16G16B16F)的, 要不然PostProcess效果出不来
或者使用RGBM(A8R8G8B8)进行压缩, 然后半透明的元素只能单独再用一个BlendBuffer(1/4大小)画完了进行合成, 这样又可以省掉很多带宽和填充率
再就是折射可以直接从G-Buffer里取Diffuse, 不需要再做一个折射用的Pass, 根据Depth进行剪裁
Fog/ShadowMap/SSAO/SoftParticle也可以直接使用G-Buffer中的Depth信息重建position进行计算
题外话: 延迟渲染的材质灵活性还是差啊, 但是对于程序来说还是挺省事的
