# Shader山下（十五）标准库函数 - 凯奥斯的注释 - CSDN博客





2016年10月15日 19:37:28[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1475
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# Cg提供了一组内置函数来简化Shader编程，这些内置函数会映射到一个专门的GPU指令，因此使用标准库一般会比自己编写的函数更快并更准确。

数学函数：



|abs(x)|绝对值|
|----|----|
|acos(x)|反余弦[0,π]，x∈[-1,+1]|
|all(x)|x的所有成员都不等于0返回true，否则false|
|any(x)|x的任一成员不等于0返回true，否则false|
|asin(x)|反正弦[-π/2,π/2]，x∈[-1,1]|
|atan(x)|反正切[-π/2,π/2]|
|atna2(y,x)|y/x的反正切，[-π,π]|
|ceil(x)|进位求整|
|clamp(x,a,b)|限制x在[a,b]内（return x < a ? a : (x > b ? b : x)）|
|cos(x)|余弦|
|cosh(x)|双曲余弦|
|coss(a,b)|外积，a和b必须是三元向量(Xa,Ya,Za)×(Xb,Yb,Zb)=(YaZb-YbZa,ZaXb-ZbXa,XaYb-XbYa）|
|degrees(x)|弧度转角度|
|determinant(M)|矩阵M的行列式值|
|dot(a,b)|点积(Xa,Ya,Za)·(Xb,Yb,Zb)=XaXb+YaYb+ZaZb|
|exp(x)|自然常数e的x次幂|
|exp2(x)|2的x次幂|
|floor(x)|去尾求整|
|fmod(x,y)|x/y的余数，与x符号相同。|
|frac(x)|x的小数部分|
|frexp(x,out exp)|把x分解成一个[1/2,1)之间的标准化分数并返回，并把2的幂保存在exp中|
|isfinite(x)|判断x是有限值|
|isinf(x)|判断x是无限制|
|isnan(x)|判断x不是数字|
|ldexp(x,n)|x乘以2的n次幂|
|lerp(a,b,f)|线性插值(1-f)*a + b*f|
|lit(NdotL, NdotH, m)|为环境光、漫反射光和镜面反射光成分计算光照系数。NdotL包含法线向量乘以光照向量。NdotH包含法线向量乘以半角向量。返回结果为一个四元向量：x包含环境光系数，通常为1.0y包含漫反射系数，y = NdotL＜0 ？0 : NdotLz包含镜面反射系数，z = NdotL < 0 || NdotH < 0 ? 0 : pow(NdotH, m)w=1.0|
|log(x)|自然对数，x>0|
|log2(x)|以2为底的对数，x>0|
|log10(x)|以10为底的对数，x>0|
|max(a,b)|a > b ? a : b|
|min(a,b)|a < b ? a : b|
|modf(x, out ip)|把x分解成整数和小数两部分，与x符号相同，整数部分被保存在ip中，小数部分返回|
|mul(M,N)|矩阵相乘，或矩阵乘以列向量，或行向量乘以矩阵|
|noise(x)|噪声函数（一元、二元或三元），返回值在0到1之间，通常与给定的输入值一样|
|pow(x,y)|x的y次幂|
|radians(x)|角度转弧度|
|round(x)|四舍五入取整|
|rsqrt(x)|x的平方根的倒数，x>0|
|saturate(x)|限制x在[0,1]之间，即clamp(x,0,1)或min(max(x,0),1)|
|sign(x)|x>0 ? 1 : 0|
|sin(x)|正弦|
|sincos(x, out s, out c)|s为x的正弦，c为x的余弦|
|sinh(x)|双曲正弦|
|smoothstep(min,max,x)|如果x 比min 小，返回 0;如果x 比max 大，返回 1；否则，如果x处于范围 [min，max]中，则是 0 和 1 之间的值，非线性插值-2*pow((x-min)/(max-min),3)+3*pow((x-min)/(max-min),2)|
|step(a,x)|x<a ? 0 : 1|
|sqrt(x)|平方根，x>=0|
|tan(x)|正切|
|tanh(x)|双曲正切|
|transpose(M)|转置矩阵|




几何函数：


|distance(pt1,pt2)|求两点之间欧式距离，即length(pt1-pt2)|
|----|----|
|faceforward(N,I,Ng)|dot(Ng,I)<0 ? N : -N|
|length(v)|向量的欧式长度，即sqrt(v.x*v.x+v.y*v.y+v.z*v.z)|
|normalize(v)|返回单位向量，即v/length(v)|
|reflect(I,N)|根据入射光方向I和表面法向量N计算反射向量，仅支持三元向量|
|refract(I,N,eta)|根据入射光方向I和表面法向量N和折射相对系数eta，计算折射向量。如果给定的eta，I和N之间的角度太大，返回(0,0,0)。仅支持三元向量|

纹理贴图函数：

|tex1D(sampler1D tex, float s)|1D非投影纹理查询|
|----|----|
|tex1D(sampler1D tex, float s, float dsdx, float dsdy)|1D非投影使用导数的纹理查询|
|tex1D(sampler1D tex, float2 sz)|1D非投影深度比较纹理查询|
|tex1D(sampler1D tex, float2 sz, float dsdx, float dsdy)|1D非投影深度比较并使用导数的纹理查询|
|tex1Dproj(sampler1D tex, float2 sq)|1D投影纹理查询|
|tex1Dproj(sampler1D tex, float3 szq)|1D投影深度比较纹理查询|
|tex2D(sampler2D tex, float2 s)|2D非投影纹理查询|
|tex2D(sampler2D tex, float2 s, float2 dsdx, float2 dsdy)|2D非投影使用导数的纹理查询|
|tex2D(sampler2D tex, float3 sz)|2D非投影深度比较纹理查询|
|tex2D(sampler2D tex, float3 sz, float2 dsdx, float2 dsdy)|2D非投影深度比较并使用导数的纹理查询|
|tex2Dproj(sampler2D tex, float3 sq)|2D投影纹理查询|
|tex2Dproj(sampler2D tex, float4 szq)|2D投影深度比较纹理查询|
|texRECT(samplerRECT tex, float2 s)|矩形2D非投影纹理查询（OpenGL独有）|
|texRECT(samplerRECT tex, float2 s, float2 dsdx, float2 dsdy)|矩形2D非投影使用导数的纹理查询（OpenGL独有）|
|texRECT(samplerRECT tex, float3 sz)|矩形2D非投影深度比较纹理查询（OpenGL独有）|
|texRECT(samplerRECT tex, float3 sz, float2 dsdx, float2 dsdy)|矩形2D非投影深度比较并使用导数的纹理查询（OpenGL独有）|
|texRECTproj(samplerRECT tex, float3 sq)|矩形2D投影纹理查询（OpenGL独有）|
|texRECTproj(samplerRECT tex, float4 szq)|矩形2D投影深度比较纹理查询（OpenGL独有）|
|tex3D(sampler3D tex, float3 s)|3D非投影纹理查询|
|tex3D(sampler3D tex, float3 s, float3 dsdx, float3 dsdy)|3D非投影使用导数的纹理查询|
|tex3Dproj(sampler3D tex, float4 sq)|3D投影纹理查询|
|texCUBE(samplerCUBE tex, float3 s)|立方贴图非投影纹理查询|
|texCUBE(samplerCUBE tex, float3 s, float3 dsdx, float3 dsdy)|立方贴图非投影使用导数的纹理查询|
|texCUBEproj(samplerCUBE tex, float4 sq)|立方贴图投影纹理查询（忽略q）|


注意：所有函数返回一个float4的值。其中s表示一元、二元或三元的纹理坐标；z表示这是一个用来进行阴影贴图查找的深度比较值；q表示这是一个透视值，在进行纹理查找之前，它被用来除以纹理坐标（s）。

此外，当你使用的纹理函数允许你指定一个深度比较值的时候，与之相关联的纹理单元必须被设置成深度比较纹理。否则，深度比较实际上不会被执行。




导数函数：


|ddx(a)|近似a关于屏幕空间x轴的偏导数|
|----|----|
|ddy(a)|近似a关于屏幕空间y轴的偏导数|


调试函数


|debug(float4 x)|如果一个编译器的DEBUG选项被选择了，调用这个函数将使得x的值被拷贝到程序的COLOR输出，并且正在执行的程序会终止。如果编译器的DEBUG选项没被选中，这个函数什么也不做|
|----|----|











