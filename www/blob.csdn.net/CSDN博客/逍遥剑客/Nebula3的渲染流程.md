# Nebula3的渲染流程 - 逍遥剑客 - CSDN博客
2009年02月02日 23:05:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2467
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
在说流程之前, 有必要先看一下N3的Frame子系统:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090202/N3-Frame.JPG)
每帧的渲染过程都写进了配置文件, 默认使用的是"frame/win32/dx9default.xml", 阴影还有另外一套.
<?xml version="1.0" encoding="UTF-8"?>
<Nebula3 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../../../schemas/frame.xsd">
<!-- 
    Test frame shader for the Win32 platform.
    Renders a dedicated depth pass first, then renders the solid and alpha geometry to a (possibly
    multisampled) color buffer, and finally resolves the result through a post-effect into the backbuffer.
--> 
<FrameShader name="DX9Default">
    
    <!-- declare render targets -->
    <DeclareRenderTarget name="ColorBuffer" format="A8R8G8B8" depth="true" relWidth="1.0" relHeight="1.0" msaa="true"/>
    
    <!-- render the world to the BackBuffer render target -->
    <Pass name="Color" renderTarget="ColorBuffer" shader="p_color" clearColor="0.2,0.2,0.2,1.0" clearDepth="1.0" clearStencil="0">
        <Batch shader="b_depth" type="Depth" shdFeatures="Depth" nodeFilter="Solid" sorting="FrontToBack" lighting="None"/>
        <Batch shader="b_solid" type="Solid" shdFeatures="Solid" nodeFilter="Solid" sorting="None" lighting="SinglePass"/>
        <Batch shader="b_alpha" type="Alpha" shdFeatures="Alpha" nodeFilter="Alpha" sorting="None" lighting="None"/>
    </Pass>
    <!-- apply a post effect with color correction, renders directly to the presentation buffer -->
    <PostEffect name="Compose" shader="pe_compose">
        <ApplyShaderVariable sem="ColorBuffer" value="ColorBuffer"/>
        
        <!-- render debug stuff on top of complete frame -->
        <Batch shader="b_shape" type="Shapes"/>
        <Batch shader="b_empty" type="Text"/>
    </PostEffect>
    
</FrameShader>
</Nebula3>
在FrameShader的配置文件里, 声明了一些RenderTarget, 还有FramePass和FramePostEffect. 
FramePass包装了所有要渲染到RenderTarget的所有3D渲染相关的东西, 由FrameBatch组成. 
FrameBatch代表一个批次的ModelNodeInstance, 比如深度批次, 非透明批次和透明批次等等(相当于OGRE中的RenderGroup), 而ModelNodeInstance就做为最小的渲染单位了.
FramePostEffect就是后处理, 各种效果如HDR, Bloom, Depth of Field, Motion Blur等等都是在这里完成. Shadow Mapping有降采样也可以放到这里, 因为那也是图像空间的操作.
具体的流程就由流程图来说明吧:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090202/N3-RenderSequence.JPG)
InternalGraphicsServer代表就是图形世界, 在图形库外部对应前面提过的Stage/View.
一个Graphics View的作用就是把一个Stage通过CameraEntity渲染到RenderTarget上. 一个Stage可以有多个View, 而一个View也可以依赖另一个. 
