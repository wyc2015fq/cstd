# Shader山下（十九）标记Tag - 凯奥斯的注释 - CSDN博客





2016年10月20日 10:58:33[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1452
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# Unity Shader里的Tag分为两种，一种是子着色器（SubShader）Tags，一种是通道（Pass）Tags。

我们使用Tag告诉渲染引擎我们期望什么时候渲染这些子着色器或者通道，以及怎么样渲染（既然是期望，那么渲染引擎也可能不答应，那么就靠Fallback了）。

SubShader Tags包括：


|Queue|渲染顺序。内建值包括：Background=1000Geometry=2000（默认）AlphaTest=2450Transparent=3000Overlay=4000可以自定义值例如"Queue"="3100"也支持这样的写法"Queue"="Transparent+100"|
|----|----|
|RenderType|渲染类型。内建值包括：Opaque：不透明（法线、自发光、反射、地形Shader）Transparent：半透明（透明、粒子、字体、地形添加通道Shader）TransparentCutout：遮罩透明（透明裁切、双通道植物Shader）Background：天空盒ShaderOverlay：GUI纹理、光晕、闪光ShaderTreeOpaque：地形引擎——树皮TreeTransparentCutout：地形引擎——树叶TreeBillboard：地形引擎——公告牌（始终面向摄像机）式树木Grass：地形引擎——草GrassBillboard：地形引擎——公告牌（始终面向摄像机）式草|
|DisableBatching|是否禁用Batch（打包、合并），可用值：True：禁用False：不禁用（默认）LODFading：当LOD fade开启的时候禁用，一般用在树木上面|
|ForceNoShadowCasting|是否强制不投射阴影，当这个值为True的时候，使用这个Shader的对象便不会投射阴影。一般用于透明对象，或者不想继承其他shader阴影的时候。|
|IgnoreProjector|无视投影器，当这个值为True的时候，对象便不受投射器影响。一般用于半透明对象，因为让投射器影响它们不是什么好事情。|
|CanUseSpriteAtlas|可使用精灵集，当这个值为False的时候，不能使用精灵集。|
|PreviewType|材质的预览形式，默认显示为球体，可以使用Plane（2D平面）或Skybox（天空盒）|





Pass Tags包括：


|LightMode|光照模式：Always：总是渲染，不使用光照ForwardBase：用于前向渲染，使用环境光、主平行光、顶点/SH（[球谐函数](http://baike.baidu.com/link?url=1JENpL-hYxXjc486HmfExWP1ayeATMjI8x7oqFi0RLlgQov2SEGefQvnTgl1DLqCBb7GvgeYSAMfPTVq7QdjRrRYz4bGr1a8CjH3gngnEr7KsYKxdJOsjbvk9ghi6vhd)）光照以及光照贴图ForwardAdd：用于前向渲染，额外使用每像素光，每个光照一个通道Deferred：用于延迟着色，渲染[G-Buffer](http://baike.baidu.com/link?url=LJi0JFgpaETp8LslhjLXT_sxqqk0u-H69Dydxmu0c5itU2yBImCeKjZVoUBTOqCnEipSzqj1KBiCBsVcv0XyR2s6WHvfYMyTXWDqcULymTi)ShadowCaster：渲染对象的深度到阴影贴图或者深度纹理PrepassBase：用于（旧版）延迟光照，渲染法线和高光指数PrepassFinal：用于（旧版）延迟光照，合并贴图、光照和自发光来渲染最终色彩Vertex：当对象不受光照贴图影响的时候，用来渲染（旧版）顶点发光。使用所有的顶点光照VertexLMRGBM：当对象接受光照贴图影响的时候，用来渲染（旧版）顶点发光。适用于使用RGBM编码光照贴图的平台（PC&主机）VertexLM：当对象接受光照贴图影响的时候，用来渲染（旧版）顶点发光。适用于使用double-LDR编码光照贴图的平台（移动平台）|
|----|----|
|PassFlags|标志渲染管线如何传递数据给通道OnlyDirectional：只有主平行光、环境光和光照探测器的数据会传递给通道仅用于LightMode为ForwardBase|
|RequireOptions|标志通道至于在某些外部条件满足时才会被渲染SoftVegetation：当Quality Setting中的Soft Vegetation选项被开启时，才会渲染通道|




