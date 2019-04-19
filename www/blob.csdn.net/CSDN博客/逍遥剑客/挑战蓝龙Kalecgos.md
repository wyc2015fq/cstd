# 挑战蓝龙Kalecgos - 逍遥剑客 - CSDN博客
2009年03月12日 00:03:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2364标签：[string																[animation																[blend																[shader](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=blend&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[山口山寨](https://blog.csdn.net/xoyojank/article/category/488683)
上次画的刀因为是装备, 所以有多种颜色的纹理
而多种颜色的纹理都要去查表...
虽说DBC文件很简单, 但现在懒得看
找个单一纹理的弄吧, 纹理名全存在M2文件里的
随便看了看, 觉得这头蓝龙挺帅的, 就它了
这次除了Mesh正好可以研究下Animation
目标:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090312/Kalecgos.JPG)
- 2009/3/15: 模型+纹理绘制
没有进行渲染状态的管理, 比如alpha test/blend切换, envMap切换, cull切换等等. 这样是不是要写一堆shader来设置State?
牙和爪子部分第二层贴图把原来的颜色覆盖了, 可能是因为应该用envMap的原因吧
其它部分现在统一用的alpha blend, 这样会出现遮挡问题, 需要进行排序. 想想应该能用alpha test代替.
(因为没走N3的渲染流程, 发现它默认是不创建DepthBuffer的, 囧......害我查了半天为啥z test失败了)
解析文件时有5张纹理, 而texture unit竟然有12个, 也就是说有些纹理在多次DP中用到了
冒似纹理的寻址方式也要设置, 暴雪真是的, 有用的技术全给用上了-_-
看来在解析骨骼动画之前还有一大堆的绘制问题要解决
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090315/Kalecgos-Textured.JPG)
- 2009/3/22: 为了切换渲染状态写了一堆ShaderVariation(technique):
technique Static < string Mask = "Static"; >
{
    pass p0
    {
			VertexShader = compile vs_3_0 VertexShaderFunc();
			PixelShader = compile ps_3_0 PixelShaderFunc();
    }
}
//------------------------------------------------------------------------------
technique SingleSided < string Mask = "SingleSided"; >
{
    pass p0
    {
			CullMode = CCW;
    }
}
//------------------------------------------------------------------------------
technique TwoSided < string Mask = "TwoSided"; >
{
    pass p0
    {
			CullMode = None;
    }
}
//------------------------------------------------------------------------------
technique Opaque < string Mask = "Opaque"; >
{
    pass p0
    {
			AlphaTestEnable = False;
			AlphaBlendEnable = False;
			ZWriteEnable = True;
    }
}
//------------------------------------------------------------------------------
technique Transparent < string Mask = "Transparent"; >
{
    pass p0
    {
			AlphaTestEnable = True;
			AlphaRef = 1;
			AlphaFunc = GreaterEqual;
    }
}
//------------------------------------------------------------------------------
technique Alpha < string Mask = "Alpha"; >
{
    pass p0
    {
			AlphaBlendEnable = True;
			SrcBlend = SrcAlpha;
			DestBlend = InvSrcAlpha;
			ZWriteEnable = False;
    }
}
//------------------------------------------------------------------------------
technique AddColor < string Mask = "AddColor"; >
{
    pass p0
    {
			AlphaBlendEnable = True;
			SrcBlend = SrcColor;
			DestBlend = One;
			ZWriteEnable = False;
    }
}
//------------------------------------------------------------------------------
technique AddAlpha < string Mask = "AddAlpha"; >
{
    pass p0
    {
			AlphaBlendEnable = True;
			SrcBlend = SrcAlpha;
			DestBlend = One;
			ZWriteEnable = False;
    }
}
//------------------------------------------------------------------------------
technique Modulate < string Mask = "Modulate"; >
{
    pass p0
    {
			AlphaBlendEnable = True;
			SrcBlend = DestColor;
			DestBlend = SrcColor;
			ZWriteEnable = False;
    }
}
//------------------------------------------------------------------------------
technique ModulateX2 < string Mask = "ModulateX2"; >
{
    pass p0
    {
			AlphaBlendEnable = True;
			SrcBlend = DestColor;
			DestBlend = SrcColor;
			ZWriteEnable = False;
    }
}
纹理寻址还没去管它-_-. 用的时候可以用这些pass来设置渲染状态啦
- 2009/3/29: 解决了渲染状态的切换, 按pass进行了排序. 这样就修正了透明/遮挡/剔除错误.
还差个球形环境映射, mesh基本上就搞定了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090401/M2-Model-RenderState.JPG)
