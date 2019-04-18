# 小聊聊NGUI中Panel的Clip功能（之一） - tkokof1的专栏 - CSDN博客

2016年08月01日 21:31:32[tkokof1](https://me.csdn.net/tkokof1)阅读数：5217


  NGUI中的Panel具有裁剪的功能，操作上便是设置一个Panel类型即可~

![](https://img-blog.csdn.net/20160801212612559)

  原理上，该裁剪功能是基于Shader来实现的，简单列一下相关的着色器代码：

```
v2f vert (appdata_t v)
{
	o.vertex = mul(UNITY_MATRIX_MVP, v.vertex);
	o.color = v.color;
	o.texcoord = v.texcoord;
	o.worldPos = v.vertex.xy * _ClipRange0.zw + _ClipRange0.xy;
	return o;
}

half4 frag (v2f IN) : COLOR
{
	// Softness factor
	float2 factor = (float2(1.0, 1.0) - abs(IN.worldPos)) * _ClipArgs0;

	// Sample the texture
	half4 col = tex2D(_MainTex, IN.texcoord) * IN.color;
	col.a *= clamp( min(factor.x, factor.y), 0.0, 1.0);
	return col;
}
```

  乍一看似乎有不少陌生的东西，不急，慢慢来~

  首先来看顶点着色器（即vert），前几行都比较传统：使用[ModelViewProjection](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)来变换顶点，然后设置顶点颜色和纹理坐标，只是最后一步有些奇怪：

  o.worldPos = v.vertex.xy * _ClipRange0.zw + _ClipRange0.xy;

  这里的ClipRange0比较奇怪，来看看他的定义(UIDrawCall.cs)：

```
/// <summary>
/// Set the shader clipping parameters.
/// </summary>

void SetClipping (int index, Vector4 cr, Vector2 soft, float angle)
{
	angle *= -Mathf.Deg2Rad;

	Vector2 sharpness = new Vector2(1000.0f, 1000.0f);
	if (soft.x > 0f) sharpness.x = cr.z / soft.x;
	if (soft.y > 0f) sharpness.y = cr.w / soft.y;

	if (index < ClipRange.Length)
	{
		mDynamicMat.SetVector(ClipRange[index], new Vector4(-cr.x / cr.z, -cr.y / cr.w, 1f / cr.z, 1f / cr.w));
		mDynamicMat.SetVector(ClipArgs[index], new Vector4(sharpness.x, sharpness.y, Mathf.Sin(angle), Mathf.Cos(angle)));
	}
}
```

  暂时不管其中的ClipArgs（也就是原Shader中的_ClipArgs0），同样我们也暂时不管其中的index索引，其是NGUI用于实现多层Panel嵌套裁剪用的，此篇文章中我们不予讨论，但就ClipRange来看，其会被设置成：

  Vector4(-cr.x / cr.z, -cr.y / cr.w, 1f / cr.z, 1f / cr.w)

  至于cr到底是哪里传来的参数，我们同样可以跟踪看看，但是此处我们暂且略过（有兴趣的朋友可以细看看UIPanel.UpdateDrawCalls()），在此我们直接给出结论：cr代表的是Panel的（裁剪）区域大小，其中xy分量代表区域中心，zw分量代表区域大小的一半（注意，是区域大小的一半！）

  对于裁剪，我们的目标其实很明确，就是判断某个点是否在Panel的区域中，如果不在则将其直接“裁剪掉”，在则保留，相关的方法有不少，比较直接的一种就是直接比较点坐标与Panel的Corner坐标，不过这里我们使用一种更为通用的方法，就是将点坐标转换到Panel的局部坐标系下，并执行一个“归一化”操作，操作上就是首先执行一个位移操作，然后依据Panel的大小执行一个缩放，即：

  假设顶点坐标为 vx, vy, Panel的中心位置为px, py, Panel的区域大小为pw, ph,并设pw’ = 0.5 * pw, ph’ = 0.5 * ph, 则相关操作可表示为：

  vx’ = (vx - px) / pw’

  vy’ = (vy - py) / ph’

  考虑之前提到的ClipRange，其如果使用上述分量来表示的话，即为：

  ClipRange.x = -px / pw’

  ClipRange.y = -py / ph’

  ClipRange.z = 1 / pw’

  ClipRange.w = 1 / ph’

  综上，则有：

  vx’ = vx * ClipRange.z + ClipRange.x

  vy’ = vy * ClipRange.w + ClipRange.y

  上述公式即是vertex着色器中最后那行代码所要表达的流程~

  OK，看完vertex，接着瞅瞅片段着色器（即frag），没想第一句就有些奇怪：

// Softness factor

float2 factor = (float2(1.0, 1.0) - abs(IN.worldPos)) * _ClipArgs0;

  暂时不管那个_ClipArgs0，先来看看float2(1.0, 1.0) - abs(IN.worldPos)这句是什么意思~

  IN.worldPos是vertex着色器中最后那行代码输出的结果，根据前面所述，该值被变换到了Panel的局部坐标系下，并依据Panel大小进行了缩放操作，所以如果IN.worldPos.x（即前面的vx’）的绝对值小于等于1，则可以认定IN.worldPos.x在Panel的大小范围之内，如果其值大于1，则可认定IN.worldPos.x不在Panel的大小范围之内（IN.worldPos.y（即前面的vy’）亦是如此~）

  所以，float2(1.0, 1.0) - abs(IN.worldPos)这句话即是用来判断顶点是否在Panel的范围之内，所得结果的任一分量如果小于0则点在Panel范围之外，反之结果分量如果都大于0，则点在Panel范围之内~

  但是后面乘以_ClipArgs0又是个什么鬼？

  不急，首先我们还是先来看看_ClipArgs0的含义，答案同样可以从之前所列的SetClipping函数中找到：_ClipArgs0的xy分别是cr.z / soft.x和cr.w / soft.y，cr.z和cr.w即是Panel区域大小的一半（即之前的pw’和ph’），而所谓的soft，则是Panel所支持的soft clip功能，效果上就是裁剪控件的边缘可以根据soft大小渐隐显示，而不是显示成硬边~

设置Soft的Panel裁剪效果：

![](https://img-blog.csdn.net/20160801212619997)

  未设置Soft的Panel裁剪效果：

![](https://img-blog.csdn.net/20160801212627005)

假设顶点（以Panel坐标系下定义，此时Panel的中心位置即为（0,0））的坐标为(vx, vy)，Panel的区域大小为(pw, ph)，soft的设置大小为(sx, sy)，同样设pw’ = 0.5 * pw, ph’ = 0.5 * p，则有：


  顶点距离区域边缘的大小

  d = (pw’ - abs(vx), ph’ - abs(vy)) = (dx, dy)

  计算d与soft的比值

  f = (dx / sx, dy / sy) = (fx, fy)

  对比值f做一个clamp01操作 

  f’ = (clamp01(fx), clamp01(fy))

  此时f’分量的较小值即代表当前顶点的透明度大小~

使用代码来表达一下上述流程，就有：

float2 factor = (float2(pw’, ph’) - abs(vx, vy)) * (1 / sx, 1 / sy)

  // assume fragment color is col

  col.a *= clamp(min(factor.x, factor.y), 0.0, 1.0);

 factor继续处理下，提取一个（pw’, ph’），则有：

float2 factor = 

  (float2(1, 1) - abs(vx / pw’, vy / ph’)) * (pw’ / sx, ph’ / sy) = 

  (float2(1, 1) - abs(vx’, vy’)) * (pw’ / sx, ph’ / sy) 

  感觉是不是很熟悉，考虑到_ClipArgs0的xy分别是cr.z / soft.x和cr.w / soft.y，则上面的表述其实就是vert的第一行代码 ：）

// Softness factor

float2 factor = (float2(1.0, 1.0) - abs(IN.worldPos)) * _ClipArgs0;

而vert后面的代码便相对普通了，基本就是根据factor来调整alpha值~

OK，至此，NGUI用以实现clip的实现原理我们大概清晰了，不过目前该实现方式仅能支持NGUI的原生控件，对于其他的可视元素便无能为力了，如果我们在UI上挂上一个粒子（ParticleSystem），很快便能发现裁剪不起效果了，但是实际项目中，在UI上挂接粒子（或者其他可视元素）其实是个比较普遍的需求，如何来较好的解决这种情况下的Panel裁剪问题呢？我们下篇再来小聊聊吧~

