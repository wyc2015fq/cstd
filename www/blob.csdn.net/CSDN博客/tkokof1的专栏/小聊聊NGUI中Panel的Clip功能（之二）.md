# 小聊聊NGUI中Panel的Clip功能（之二） - tkokof1的专栏 - CSDN博客

2016年08月03日 21:29:39[tkokof1](https://me.csdn.net/tkokof1)阅读数：3649


  上篇简单聊了一下NGUI中Panel裁剪的实现原理，总结来看其实比较简单，就是通过Shader计算fragment关于Panel裁剪区域的相对位置，然后通过调整alpha值来实现裁剪效果~

  那么依样画瓢，如果我们想要实现非NGUI元素的裁剪，也可以考虑使用Shader的方式，在此以ParticleSystem为例，看看我们如何将其挂接到UI之上，并且实现裁剪效果~

  回忆一下之前的frag着色器：


```
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

  其中的IN.worldPos是经过变换过的顶点坐标，如果我们找到方法对粒子的顶点进行同样的变换，那么就可以实现相同的裁剪功能~

  那么如何执行变换呢？考虑之前vert着色器中进行转换的方法：

  o.worldPos = v.vertex.xy * _ClipRange0.zw + _ClipRange0.xy;

  我们是不是可以直接照搬，将其运用于粒子顶点之上呢？答案是否定的，原因在于粒子的顶点数据并不和Panel在同一坐标系下（而关于为何NGUI元素的顶点数据和Panel是在同一个坐标系的问题，有兴趣的朋友可以细看看UIDrawCall.cs），而不同坐标系下的数据进行相互操作，一般都不会得到正确结果，多是没有什么意义的~

  那怎么办呢？其实也简单，统一坐标系即可，即将粒子的顶点坐标系和Panel的顶点坐标系进行统一，而至于选择哪个坐标系则并不重要，在Unity中有不少选择，我们在此选择[Viewport坐标系](https://docs.unity3d.com/ScriptReference/Camera.WorldToViewportPoint.html)~

  至此，方法已经很明确了，为了实现粒子在NGUI Panel中的裁剪，我们仅需要以Viewport坐标系为桥梁，同样利用Shader来判断粒子顶点是否在Panel的裁剪范围之内，并仍然通过调整alpha值来实现真正的裁剪效果~

  先来看第一步，将Panel坐标转至Viewport坐标，在此我们通过MonoBehaviour来完成：


```
using UnityEngine;

[ExecuteInEditMode]
public class ModelClip : MonoBehaviour
{
    UIPanel m_panel;
    Material m_material;

    int m_panelSizeXProperty;
    int m_panelSizeYProperty;
    int m_panelCenterAndSharpnessProperty;

    void Start()
    {
        m_panel = UIPanel.Find(transform);

        var renderer = gameObject.GetComponent<Renderer>();
        if (renderer)
        {
            if (!Application.isPlaying)
            {
                m_material = renderer.sharedMaterial;
            }
            else
            {
                m_material = renderer.material;
            }
        }

        m_panelSizeXProperty = Shader.PropertyToID("_PanelSizeX");
        m_panelSizeYProperty = Shader.PropertyToID("_PanelSizeY");
        m_panelCenterAndSharpnessProperty = Shader.PropertyToID("_PanelCenterAndSharpness");

        if (m_panel)
        {
            UpdateClip(m_panel);
            m_panel.onClipMove += UpdateClip;
        }
    }

    void UpdateClip(UIPanel panel)
    {
        if (panel && panel.hasClipping)
        {
            var soft = panel.clipSoftness;
            var sharpness = new Vector2(1000.0f, 1000.0f);
            
            if (soft.x > 0f)
            {
                sharpness.x = panel.baseClipRegion.z / soft.x;
            }
            if (soft.y > 0f)
            {
                sharpness.y = panel.baseClipRegion.w / soft.y;
            }

            Vector4 panelCenterAndSharpness;

            var uiCamera = NGUIUtil.GetCamera();
            Debug.Assert(uiCamera != null);
            var panelWorldCorners = m_panel.worldCorners;
            var leftBottom = uiCamera.WorldToViewportPoint(panelWorldCorners[0]);
            var topRight = uiCamera.WorldToViewportPoint(panelWorldCorners[2]);
            var center = Vector3.Lerp(leftBottom, topRight, 0.5f);

            panelCenterAndSharpness.x = center.x;
            panelCenterAndSharpness.y = center.y;
            panelCenterAndSharpness.z = sharpness.x;
            panelCenterAndSharpness.w = sharpness.y;

            // Set shader properties
            m_material.SetFloat(m_panelSizeXProperty, topRight.x - leftBottom.x);
            m_material.SetFloat(m_panelSizeYProperty, topRight.y - leftBottom.y);
            m_material.SetVector(m_panelCenterAndSharpnessProperty, panelCenterAndSharpness);
        }
    }

}
```

  代码并不复杂，核心自然如前面所说，就是将Panel坐标转至Viewport坐标系下，所以留心一下那两句WorldToViewportPoint即可~

  接着，便是使用Shader来判断粒子顶点是否在Panel的裁剪范围之下了，由于Panel的裁剪范围已经变换至Viewport坐标系，所以粒子的顶点也需要做相同的转换~


```
v2f vert (appdata v)
{
	v2f o;

	o.vertex = mul(UNITY_MATRIX_MVP, v.vertex);
	o.color = v.color;
	o.uv = TRANSFORM_TEX(v.uv, _MainTex);

	float2 clipSpace = o.vertex.xy / o.vertex.w;

	// Normalize clip space
	o.posInPanel = (clipSpace.xy + 1) * 0.5;

	// Adjust for panel offset
	o.posInPanel.x -= _PanelCenterAndSharpness.x;
	o.posInPanel.y -= _PanelCenterAndSharpness.y;

	// Adjust for panel size
	o.posInPanel.x *= (2 / _PanelSizeX);
	o.posInPanel.y *= (2 / _PanelSizeY);

	return o;
}
```

  上面代码最重要的其实是这两句：

```
float2 clipSpace = o.vertex.xy / o.vertex.w;
// Normalize clip space
o.posInPanel = (clipSpace.xy + 1) * 0.5;
```

  首先使用[ModelViewProjection](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)变换后的xy分量除以w分量，可以将顶点变换至[NormalizedDeviceCoordinates(NDC)空间](http://stackoverflow.com/questions/21841598/clip-space-and-screen-space)，该空间下，xy的取值范围为[-1, 1]，据此我们重新将其映射至[0, 1]范围中（即Viewport空间中），于是便有了上面两行代码~

  之后的操作与NGUI原生的裁剪Shader如出一辙，不清楚的朋友可以参考[前篇](http://blog.csdn.net/tkokof1/article/details/52089289)~

  至此我们便完成了粒子在NGUI Panel下的裁剪操作，有图有真相~

![](https://img-blog.csdn.net/20160803212820601)

![](https://img-blog.csdn.net/20160803212826288)


  OK，废话不多说了，下次再见吧~

