# 升采样(Upsampling)技术 - 逍遥剑客 - CSDN博客
2012年09月25日 23:23:29[xoyojank](https://me.csdn.net/xoyojank)阅读数：13551
最近老是想着学习下Upsampling, 因为在做优化时这是一项非常实用的技术
参考AMD的PPT: [Mixed Resolution Rendering](http://www.google.com.hk/url?sa=t&rct=j&q=Mixed+Resolution+Rendering&source=web&cd=1&cad=rja&ved=0CC0QFjAA&url=http%3A%2F%2Fdeveloper.amd.com%2Fgpu_assets%2FShopfMixedResolutionRendering.pdf&ei=m8JhUP6UFIORigeG6oD4DA&usg=AFQjCNH7jl1KzfuaALLNrjg9aLO13izJNA), 有这么几种情况可以应用:
- 把半透明元素(如粒子)单独画到一个小的BlendBuffer上, 再跟原场景混合, 减小填充率的负担
- SSAO, 一般不会做FullSize的处理, 所以Upsampling基本上是必须的一步
- 屏幕空间的软阴影, 同上
- 间接光照, 实时计算量比较大, 所以也是低精度进行
或许这个关键词可以概括这项技术: Bilateral Upsampling
通常应用的前提条件或者说场景如下:
- 延迟渲染, 有个DownSample过的DepthBuffer和原大小的DepthBuffer
- 一些比较低频或者粒度要求没有那么高的效果, 画到1/4(或更小)大小的FrameBuffer上, 使用DownSample过的DepthBuffer做DepthTest
- 低分辨率的效果叠加到原分辨率的场景上, 需要根据两个DepthBuffer的进行插值, 避免边缘出现锯齿状的走样
性能提升是显而易见的: 节省填充率, 节省采样带宽消耗, 节省显存等
缺点嘛是有一点点效果上的瑕疵, 不过一般用于低频的效果, 人眼很难注意到
这里有篇法文的文章不错, 连蒙带猜还是看得差不多能懂的:
[http://bbs.demoscene.fr/code/upsampling-technique/msg11403/#msg11403](http://bbs.demoscene.fr/code/upsampling-technique/msg11403/#msg11403)
![](https://img-my.csdn.net/uploads/201209/25/1348586804_8101.png)
```cpp
float SceneZ = ReadDepth( _In.UV ); // On lit le vrai Z full size
float2 UV = _In.UV - _dUV;  // Avec Unity, je dois mettre un offset sur les UV d'entrée... Don't ask !
float4 SkyZ;
SkyZ.x = tex2D( _TexDownsampledZBuffer, UV ).x; UV.xy += _dUV.xz;
SkyZ.y = tex2D( _TexDownsampledZBuffer, UV ).x; UV.xy += _dUV.zy;
SkyZ.z = tex2D( _TexDownsampledZBuffer, UV ).x; UV.xy -= _dUV.xz;
SkyZ.w = tex2D( _TexDownsampledZBuffer, UV ).x;UV.xy -= _dUV.zy;
float4 V[4];
V[0] = SampleValue( UV ); UV.xy += _dUV.xz;
V[1] = SampleValue( UV ); UV.xy += _dUV.zy;
V[3] = SampleValue( UV ); UV.xy -= _dUV.xz;
V[2] = SampleValue( UV ); UV.xy -= _dUV.zy;
// Compute bias weights toward each sample based on Z discrepancies
float WeightFactor = 2.0;
float ZInfluenceFactor = 0.001;
float4 DeltaZ = ZInfluenceFactor * abs(SceneZ - SkyZ);
float4 Weights = saturate( WeightFactor / (1.0 + DeltaZ) );
// Default UV interpolants for a normal bilinear interpolation
float2 uv = frac( UV.xy / _dUV.xy );
// This vector gives the bias toward one of the UV corners. It lies in [-1,+1]
// For equal weights, the bias sums to 0 so the UVs won't be influenced and normal bilinear filtering is applied
// Otherwise, the UVs will tend more or less strongly toward one of the corners of the low-resolution pixel where values were sampled
//
// Explicit code would be :
// float2 uv_bias  = Weights.x * float2( -1.0, -1.0 ) // Bias toward top-left
//   + Weights.y * float2( +1.0, -1.0 ) // Bias toward top-right
//   + Weights.z * float2( +1.0, +1.0 ) // Bias toward bottom-right
//   + Weights.w * float2( -1.0, +1.0 ); // Bias toward bottom-left
float2 uv_bias  = float2( Weights.y + Weights.z - Weights.x - Weights.w, Weights.z + Weights.w - Weights.x - Weights.y );
// Now, we need to apply the actual UV bias.
//
// Explicit code would be :
//  uv.x = uv_bias.x < 0.0 ? lerp( uv.x, 0.0, -uv_bias.x ) : lerp( uv.x, 1.0, uv_bias.x );
//  uv.y = uv_bias.y < 0.0 ? lerp( uv.y, 0.0, -uv_bias.y ) : lerp( uv.y, 1.0, uv_bias.y );
//
// Unfortunately, using branching 1) is bad and 2) yields some infinite values for some obscure reason !
// So we need to remove the branching.
// The idea here is to perform biasing toward top-left & bottom-right independently then choose which bias direction
// is actually needed, based on the sign of the uv_bias vector
//
float2 uv_topleft = lerp( uv, 0.0, saturate(-uv_bias) ); // Bias toward top-left corner (works if uv_bias is negative)
float2 uv_bottomright = lerp( uv, 1.0, saturate(uv_bias) ); // Bias toward bottom-right corner (works if uv_bias is positive)
float2 ChooseDirection = saturate( 10000.0 * uv_bias ); // Isolate the sign of the uv_bias vector so negative gives 0 and positive gives 1
uv = lerp( uv_topleft, uv_bottomright, ChooseDirection ); // Final bias will choose the appropriate direction based on the sign of the bias
// Perform normal bilinear filtering with biased UV interpolants
float4 FinalValue = lerp(
lerp( V[0], V[1], uv.x ),
lerp( V[2], V[3], uv.x ),
uv.y );
```
