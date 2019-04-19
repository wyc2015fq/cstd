# 快速LightMap烘焙 - 逍遥剑客 - CSDN博客
2009年07月12日 21:30:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：22030
什么是烘焙? 简单地说, 就是把物体光照的明暗信息保存到纹理上, 实时绘制时不再进行光照计算, 而是采用预先生成的光照纹理(lightmap)来表示明暗效果. 那么, 这样有什么意义呢?
好处:
- 由于省去了光照计算, 可以提高绘制速度
- 对于一些过度复杂的光照(如光线追踪, 辐射度, AO等算法), 实时计算不太现实. 如果预先计算好保存到纹理上, 这样无疑可以大大提高模型的光影效果
- 保存下来的lightmap还可以进行二次处理, 如做一下模糊, 让阴影边缘更加柔和
当然, 缺点也是有的:
- 模型额外多了一层纹理, 这样相当于增加了资源的管理成本(异步装载, 版本控制, 文件体积等). 当然, 也可以选择把明暗信息写回原纹理, 但这样限制比较多, 如纹理坐标范围, 物体实例个数...
- 模型需要隔外一层可以展开到一张纹理平面的UV(范围只能是[0,1], 不能重合). 如果原模型本身就是这样, 可以结省掉. 但对于大多数模型来说, 可能会采用WRAP/MIRROR寻址, 这只能再做一层, 再说不能强制每个模型只用一张纹理吧? 所以, lightmap的UV需要美术多做一层, 程序展开算法这里不提及....
- 静态的光影效果与对动态的光影没法很好的结合. 如果光照方向改变了的话, 静态光影效果是无法进行变换的. 而且对于静态的阴影, 没法直接影响到动态的模型. 这一点, 反而影响了真实度
肯定不只这几点,但我暂时只想到这几点
那么怎么生成lightmap呢?
最直接的办法: 光线追踪....(原理想想很简单, 按照物体定律来就可以了)
但是光线追踪这东西......就算用来离线生成我都嫌慢-_-
下面说的这个是利用GPU进行计算的, 跟实时光照没什么两样:
原理:
想想实时渲染的顶点变换流程: pos * WVP之后, 顶点坐标就变换到屏幕空间了[-1, 1]
如果VertexShader里直接把纹理坐标做为变换结果输出(注意从[0,1]变换到[-1,1]), 那么相当于直接变换到了纹理坐标系, 这时在PixelShader里还是像原来那样计算光照, 输出的结果就可以拿来做lightmap了
示例:
这是一个典型的Phong光照模型下的球(这里不考虑阴影效果, 对它不需要进行特殊处理):
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090712/PhoneSphere.jpg)
这是VS:
VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output        = (VS_OUTPUT)0;
   Output.Position         = mul( Input.Position, matViewProjection );
   
   Output.Texcoord         = Input.Texcoord;
   
   float3 fvObjectPosition = mul( Input.Position, matView );
   
   Output.ViewDirection    = fvEyePosition - fvObjectPosition;
   Output.LightDirection   = fvLightPosition - fvObjectPosition;
   Output.Normal           = mul( Input.Normal, matView );
      
   return( Output );
}
把原来的WVP变换改成变换到纹理坐标系:
   //Output.Position         = mul( Input.Position, matViewProjection );
   // transform to projection space
   Output.Position.xy      = Input.Texcoord * float2(2, -2) + float2(-1, 1);
   Output.Position.w       = 1;
输出的结果就成这样了:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090712/PhoneLightedTexture.jpg)
保存下来可以直接使用. 这里我用的模型比较特殊, 本身的UV就满足前面提到的条件, 所以直接跟原纹理叠加就可以. 当然, 如果只保存明暗信息的话, 就不影响原纹理的复用, 因为通常lightmap不需要很高的精度:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090712/PhoneLightmap.jpg)
有了lightmap, 再次画的时候就简单了, 只需要贴纹理, 光照大可以关掉:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090712/TextureLight.jpg)
如果还想要一更好的效果, 可以加入一些实时的全局光照算法, 如Dynamic Ambient Occlusion之类...阴影同理...
