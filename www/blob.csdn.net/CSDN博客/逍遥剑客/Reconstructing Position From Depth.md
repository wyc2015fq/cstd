# Reconstructing Position From Depth - 逍遥剑客 - CSDN博客
2010年02月06日 16:33:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：5993
需求: 根据当前像素的Depth计算出其View空间的Position
先说一种惯性思维的方法: 既然知道depth是怎么算出来的, 那么进行逆运算回去不就得到position了?
先说说depth是怎么出来的:
Vertex shader:
output.position = mul(input.postion, matWorldViewProject);
output.depth.xy = output.position.zw;
Pixel shader(输出z/w):
return input.depth.x / input.depth.y;
那么, 逆运算回去就很直接了(input.uv是全屏矩形的纹理坐标):
float z = tex2D(DepthSampler, input.uv);
// transform to projection space
float x = input.uv.x * 2 - 1;
float y = (1 - input.uv.y) * 2 - 1;
float4 vProjectedPos = float4(x, y, z, 1);
// transform to view space 
float4 vPosition = mul(vProjectedPos , matInvProject);
return (vPosition.xyz / vPosition.w);
那么这样做有什么缺点呢?
l z/w是非线性分布的, 经过RTT后再变换回去会有精度上的损失
l 计算量有点大, 要知道PS里的每个指令都是很宝贵的.
下面说说另一种非常快的算法, 而且也可以解决精度问题. 先看看摄像机视锥体的抽象形式:
![](http://images.cnblogs.com/cnblogs_com/xoyojank/vf2.gif)
从摄像机位置到远裁剪面发射一条射线, 那么, 对于可见的任意一点, 有这么个关系:
vPositionView = vViewRayDir * fLinearDepth;
其中, fLinearDepth代表规格化的Z, 它是线性分布的, 即:
fLinearDepth = vPositionView.z / fFarClipDist;
剩下的, 就是这个屏幕射线vViewRayDir从哪来的问题了.
我们知道, 在View空间, 摄像机位置是(0, 0, 0). 那么, 对于每条射线的方向, 等价于射线与远裁剪面的交点坐标. 即:
vViewRayDir = float3(fFarClipX, fFarClipY, fFarClipDist);
远裁剪面上的4个顶点坐标我们是可以算出来的, 就是Frustum中的四个顶点. 如果我们把这四个顶点坐标写入全屏矩形的顶点坐标中, 然后在VS中输出, 那么在PS中得到的就是已经插值好的射线方向了!
整理一下整个思路:
1. 把vPositionView.z / fFarClipDist输出到RTT, 这里因为是线性分布的, 在精度允许的前提下可以进行压缩
2. 从RTT里得到fLinearDepth, 从VS_OUTPUT出的寄存器里得到已经插值好的vViewRayDir.xy, vViewRayDir.z就是fFarClipDist, Position的重建只需要一句计算就可以得到:
vPositionView = vViewRayDir * fLinearDepth;
Reference(要翻墙): http://mynameismjp.wordpress.com/2009/03/10/reconstructing-position-from-depth/
