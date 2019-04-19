# 景深效果(Depth of Field) - 逍遥剑客 - CSDN博客
2007年11月14日 04:07:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：9567标签：[float																[shader																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)
个人分类：[GPU编程](https://blog.csdn.net/xoyojank/article/category/337623)
**什么是景深效果?**
景深效果,简称DOF,在人眼跟光学摄像设备上很常见.如下图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_1.JPG)
简单地来说,就是近处跟远处的景物模糊,而焦点附近的物体则很清晰.至于为什么会产生这样的效果,我就懒得说了:p
**那么怎么来实现这种效果呢?**
看图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_2.JPG)
从摄像机开始,按距离分成三部分:
近距离模糊,焦点范围(清晰),远距离模糊
渲染的时候按深度(即距离)进行判断,在焦点范围内则是清晰的,否则就进行模糊处理.
|![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_3.JPG)|整个过程分三个Pass:1.将场景渲染到一个RenderTarget,做为清晰版.2.将上一步得到的RenderTarget进行模糊处理,得到BluredRT(模糊版).3.合成.跟据距离来判断是否应该模糊,如果不在焦点范围内则绘制BluredRT,否则就绘制RenderTarget.那么,还有一个问题,深度值怎么计算呢?公式: WDepth = Depth / Far_Z_Clip.在这里我是把深度值存在Alpha通道中一块渲染到RenderTarget,这样在最后一步直接取color.a就可以进行深度判断了![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_4.JPG)(右图为Alpha通道,存储了深度值)|
**Shader****与最终效果**
sampler RenderTarget;
sampler BluredRT;
//焦点范围
float fNearDis;
float fFarDis;
float4 ps_main( float2 TexCoord : TEXCOORD0 ) : COLOR0
{
float4 color = tex2D( RenderTarget, TexCoord );
if( color.a > fNearDis && color.a < fFarDis )
return color;
else
returntex2D( BluredRT, TexCoord );
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_5.JPG)
是不是看起来很不自然?
为什么呢?因为这样实现的DOF在清晰与模糊的交界处过渡太生硬了,所以就像分成了三部分一样-_-!
**那我们增加两个过渡不就成了?**
就像这样:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_6.JPG)
sampler RenderTarget;
sampler BluredRT;
//焦点范围
float fNearDis;
float fFarDis;
float fNearRange;
float fFarRange;
float4 ps_main( float2 TexCoord : TEXCOORD0 ) : COLOR0
{
float4 sharp = tex2D( RenderTarget, TexCoord );
float4 blur = tex2D( BluredRT, TexCoord );
float percent = max(saturate(1 - (sharp.a-fNearDis)/fNearRange),
saturate((sharp.a-(fFarDis-fFarRange))/fFarRange));
returnlerp( sharp, blur, percent );
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DOF_7.JPG)
