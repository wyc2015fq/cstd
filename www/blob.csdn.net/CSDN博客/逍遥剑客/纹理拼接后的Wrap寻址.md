# 纹理拼接后的Wrap寻址 - 逍遥剑客 - CSDN博客
2008年11月03日 21:17:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3175
拼接后的纹理:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081103/AtlasTexture.JPG)
正常的草地(不进行WRAP寻址):
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081103/Atlas_WrapNum1.JPG)
WRAP = 5时的情况:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081103/Atlas_WrapNum5.JPG)
MinFilter = Linear时的情况:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081103/Atlas_WrapNum5_LINEAR.JPG)
shader实现:
sampler2D atlasTexture;
float4 texRect;   //(left, top, width, height) of uv
float2 invSize;   //(1/width, 1/height)
float4 uvRange;   //(left, top, right, bottom) of uv
float4 ps_main( float2 tex : TEXCOORD0 ) : COLOR
{
   // convert original uv to atlas uv's unit
   tex *= texRect.zw;
   
   float2 uv = tex - texRect.xy;
   // bring coordinates into normalized local texture coord [0..1]
   uv *= invSize;
   // if texture repeats then coords are > 1, use frc to bring 
   // these coords back into [0, 1) interval.
   uv = frac(uv);
   // transform coords back to texture atlas coords
   uv = uv * texRect.zw + texRect.xy;
   // clamp to inside texture (to avoid bi-linear filter pulling in foreign texels)
   uv = clamp(uv, uvRange.xy, uvRange.zw);
   // use the original coords for mip-map calculation
   return tex2Dgrad(atlasTexture, uv, ddx(tex), ddy(tex));
}
**Reference**:
ShaderX3 : Improved Batching via Texture Atlases
