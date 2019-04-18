# Shader山下（二十）编译指令（Compilation Directives） - 凯奥斯的注释 - CSDN博客





2016年10月21日 11:01:18[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2458
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









Shader中，编译指令分为两种，一种是常规的编译指令，也就是顶点片元着色器（Vetex & Fragment Shader）使用的编译指令，另一种就是表面着色器（Surface Shader）使用的编译指令。二者都使用#pragma语句来编写，并且都需要写在CGPROGRAM和ENDCG之间。区别在于，VF编译指令写在Pass里面，而表面着色器编译指令写在SubShader里面，表面着色器会自行编译到多通道里去，并且需要使用#pragma
 surface …指令来标识这是一个表面着色器。




VF编译指令：


|#pragma vertex *name*|编译name函数为顶点着色器|
|----|----|
|#pragma fragment *name*|编译name函数为片元着色器|
|#pragma geometry *name*|编译name函数为DX10的几何着色器注：会自动开启#pragma target 4.0|
|#pragma hull *name*|编译name函数为DX10的壳着色器注：会自动开启#pragma target 5.0|
|#pragma domain *name*|编译name函数为DX10的域着色器注：会自动开启#pragma target 5.0|
|#pragma target *name*|表明编译目标参考[着色器编译目标等级](https://docs.unity3d.com/Manual/SL-ShaderCompileTargets.html)|
|#pragma only_renderers *space_separated_names*|只为指定的渲染平台渲染着色器包括下列值：d3d9:Direct3D 9d3d11:Direct3D 11/12glcore:OpenGL 3.x/4.xgles:OpenGL ES 2.0gles:OpenGL ES 3.xmetal:IOS&Mac Metald3d11_9x:Direct3D 11 9.x特性等级一般用于WSA平台xbox360:Xbox 360xboxone:Xbox Oneps4:PlayStation 4psp2:PlayStation Vitan3ds:Nintendo 3DSwiiu:Nintendo Wii U|
|#pragma exclude_renderers *space_separated_names*|排除指定的渲染平台参数同上|
|#pragma multi_compile...|参考[Shader山下（二十一）多重变体（Multiple Variants）](http://blog.csdn.net/ecidevilin/article/details/52882400)或者Unity官方文档：[多重着色器变体](https://docs.unity3d.com/Manual/SL-MultipleProgramVariants.html)|
|#pragma enable_d3d11_debug_symbols|生成d3d11的调试信息可以在VS2012（或以上）使用图形调试器调试shader|
|#pragma hardware_tier_variants *renderer_name*|针对所选渲染器的每个硬件层级生成每个已编译的Shader的多重Shader硬件变体参考[Shader山下（二十一）多重变体（Multiple Variants）](http://blog.csdn.net/ecidevilin/article/details/52882400)或者Unity官方文档：[多重着色器变体](https://docs.unity3d.com/Manual/SL-MultipleProgramVariants.html)[](https://docs.unity3d.com/Manual/SL-MultipleProgramVariants.html)|





表面着色器编译指令，只有#pragma surface一个，写法：



```cpp
#pragma surface surfFunc lightingModel [optional params]
```




但是可以为这条指令配置不同的选项：


|*surfaceFunction*（必选）|表面着色器函数|
|----|----|
|*lightModel*（必选）|光照模型函数，内置模型：Standard：基于物理的漫反射模型StandardSpecular：基于物理的高光模型Lambert：不基于物理的漫反射模型BlinnPhong：不基于物理的高光模型也可以自己写，命名规则：Lighting......为在编译指令里填写的名称例如#pragma surface surf Custom光照模型函数名就要写成：LightingCustom具体参考[表面着色器中的自定义光照模型](https://docs.unity3d.com/Manual/SL-SurfaceShaderLighting.html)|



|alpha或者alpha:auto|透明度混合对于简单的光照模型（例如Lambert和BlinnPhong）使用alpha:fade对于基于物理的光照模型使用alpha:premul[](https://docs.unity3d.com/Manual/SL-SurfaceShaderLighting.html)|
|----|----|
|alpha:blend|透明度混合|
|alpha:fade|传统透明度混合（参考[Shader山下（十八）混合（Blend）命令](http://blog.csdn.net/ecidevilin/article/details/52864349)）|
|alpha:premul|预乘透明度混合|
|alphatest:*variable_name*|透明度测试，并使用variable_name作为裁切阈值|
|keepalpha|对于默认的不透明Shader，会无视光照模型返回的透明度值，直接把1.0写入Alpha通道。使用keepalpha选项，允许在不透明Shader里保留光照模型返回的透明度值。|
|decal:add|附加的贴花shader，这意味着对象在其他表面的上面并使用添加方法进行混合。|
|decal:blend|半透明贴花shader，这意味着对象在其他表面的上面并使用透明度方法进行混合。|
|vertex:*vertex_function*|自定义顶点函数|
|finalcolor:*color_function*|自定义的最终颜色修改函数|
|finalgbuffer:*gbuffer_function*|自定义的改变GBuffer内容的延迟路径|
|finalprepass:*prepass_function*|自定义的预通道基础路径|
|addshadow|生成一个阴影投射通道一般用于自定义顶点函数，这样的话，就可以对阴影投射使用程序化的顶点动画一般情况下，shader并不需要任何特殊的阴影处理，因为它们可以使用Fallback里的阴影投射通道|
|fullforwardshadows|支持前向渲染路径里的所有光照阴影默认情况下只支持一个平行光的阴影如果需要点光源（point）或者聚光灯（spot）的阴影，那么就要使用这个选项|
|tessellate:*tessellate_function*|使用DX11的GPU镶嵌，tessellate_function计算镶嵌参数参考[表面着色器镶嵌](https://docs.unity3d.com/Manual/SL-SurfaceShaderTessellation.html)|
|exclude_path:*path*|不生成指定渲染路径的通道可选项：deferredforwadprepass|
|noshadow|禁用阴影|
|noambient|禁用环境光或者光探头|
|novertexlights|禁用前向渲染中的光探头或者每顶点光照|
|nolightmap|禁用所有的光照贴图|
|nodynlightmap|禁用动态光照贴图|
|nodirlightmap|禁用平行光照贴图|
|nofog|禁用内置雾效|
|nometa|不生成元通道光照贴图和动态全局光照使用元通道来提取表面信息|
|noforwardadd|禁用前置渲染的附加通道这样就让shader支持一个完全平行光，而其他的光使用每顶点或者SH（球谐函数）计算同样让shader变得更轻|
|softvegetation|在Quality Setting里的Soft Vegetation被开启的时候，才会被渲染|
|interpolateview|在顶点着色器中计算视图方向并插入它（默认在像素着色器中计算）这样使得Shader变得更快，不过需要多使用一个纹理插值。|
|halfasview|传递半角向量给光照模型（默认是视图向量）会在每个顶点计算并归一化半角向量这样更快，但是并不完全正确。|
|approxview|5.0中被interpolateview取代|
|dualforward|在前向渲染路径中使用双光照贴图|










