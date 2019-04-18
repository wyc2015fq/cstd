# Shader山下（二十九）基于渲染路径的优化 - 凯奥斯的注释 - CSDN博客





2017年12月12日 18:30:58[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：364标签：[unity																[优化																[shader																[光照																[light](https://so.csdn.net/so/search/s.do?q=light&t=blog)
个人分类：[Shader](https://blog.csdn.net/ecidevilin/article/category/6448925)





以下摘录自[《Unity 3D ShaderLab开发实战详解》](https://book.douban.com/subject/25892263/)，第33章。 

# VertexLit 渲染路径下的优化

## VertexLit 渲染路径的特点

虽然读者一般不会使用 VertexLit 渲染路径，但是，如果你觉得这是一个适合你项目的渲染路径，那么需要知道：在 VertextLit 渲染路径下，所有的光源数据都存放于 unity_LightPosition[] 和 unity_LightColor[] 数组中， ShaderLab 定义的其他光源数据并不是为 VertexLit 渲染路径准备的。

## 合适的光照计算

虽然这些数据是 Unity 为基于顶点照明而准备的，但是很显然，也可以通过 fragment 函数计算 unity_LightPosition 中的光源，也就是逐像素计算。如果你是用的是 Unity 默认的材质，那么不用担心冗余 Shader 代码的问题， Unity 会为 VertexLit 渲染路径准备合适的 SubShader 。 

如果读者使用的是自己写的 Shader ，可以考虑去掉所有 LightMode 不是 Always 或者Vertex 的 Pass ，因为他们都无法在 VertexLit 渲染路径下被渲染，只会增加最终的 Shader 代码的大小。 

另外，只需要计算 unity_LightPosition[] 和 unity_LightColor[] 数组组合，其他光源的数据不要在 VertexLit 渲染路径下考虑。
# Forward 渲染路径下的优化

## Forward 渲染路径的特点

首先，对于移动平台， Forward 渲染路径是一个不错的选择，尤其是对于尝试写 Shader 的读者来说。 Forward 渲染路径有 ForwardBase 和 ForwardAdd 两种 Pass 之分，对于 ForwardBase 来说，除了 _WorldSpaceLightPos0 和 _LightColor0 之外， unity_4LightPosXYZ 数组中也包含光源数据。

## 合理的光照计算

首先， ForwardBase 必须要有，但是可以舍弃 ForwardAdd，这是仍然可以通过 unity_4LightPosXYZ 数组来得到足够的光源数据。 

其次，物体每被一个重要光源或者说逐像素光源所影响，就会导致 ForwardAdd 被执行一次，也就是说会导致一次 Draw Call 。当然，前提是你的 SubShader 中包含了一个 ForwardAdd 的 Pass 。 

最后，一定不要在 ForwardAdd 中计算除了 _WordSpaceLightPos0 之外的任何光源，因为这是非常耗时的事情。
# Deferred 渲染路径下的优化

## Deferred 渲染路径的特点

如果读者是从头看本书的，想必已经知道 Deferred 渲染路径其实最终是基于 G-Buffer 来渲染的一个图像，因此， Deferred 渲染必然是逐像素进行的。

## 合理的灯光布局

首先，应当使灯光的影响范围尽量小，因为一个灯光的计算量只和它的照明范围正相关。 

其次，虽然 Deferred 渲染路径下 Unity 默认支持点光源的阴影，但是，阴影仍然不是免费的，每一个阴影都会导致 Unity 的额外 Draw Call ，因此不要滥用。 

最后，读者一般会用 Surface Shader 来写 Deferred 渲染路径的 Shader ，因此，一旦你确定使用 Deferred 渲染路径，要善于利用 exclude_path:forward 、 noambient 、 nolightmap 等参数去掉冗余代码，精简 Shader 。如果你不知道这些参数什么意思，请查看书本的前面内容。](https://so.csdn.net/so/search/s.do?q=光照&t=blog)](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)




