# Shader山下（二十四）相机深度纹理 - 凯奥斯的注释 - CSDN博客





2016年11月09日 21:48:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2671
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 相机可以生成深度和深度法线纹理，以及运动向量纹理（需要Unity5.4以上）。我们可以使用这些纹理来实现屏幕后处理效果。

当你需要某种纹理的时候，可以为Camera的depthTextureMode添加以下变量：

DepthTextureMode.Depth深度纹理

DepthTextureMode.DepthNormals一个包含深度和视图空间法线的纹理

例如：



```
using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Camera))]
public class TestDepth : MonoBehaviour {

	private Material _mat;

	// Use this for initialization
	void Start () {
		Camera cam = GetComponent<Camera> ();
		cam.depthTextureMode |= DepthTextureMode.Depth;
		Shader shader = Shader.Find ("Custom/TestDepth");
		if (null != shader) {
			_mat = new Material (shader);
		}
	}

	void OnRenderImage(RenderTexture source, RenderTexture destination)
	{
		if (null != _mat) {
			Graphics.Blit (source, destination, _mat);
		} else {
			Graphics.Blit (source, destination);
		}
	}
}
```






深度会被保存在_CameraDepthTexture中，可以通过uv坐标来获取贴图中的值，然后使用UNITY_SAMPLE_DEPTH宏来转换为深度。



```cpp
float2 uv = i.uv;
                float depth = UNITY_SAMPLE_DEPTH(tex2D(_CameraDepthTexture, uv));
```




需要注意的是，深度纹理使用了与阴影投射（shadow caster）相同的Shader pass，所以如果一个对象不支持阴影投射，那么它将不会出现在深度纹理当中，并且只有RenderQueue小于等于2500的对象才能被渲染到深度纹理当中去。




而使用DepthTextureMode.DepthNormals时，我们可以获取深度法线纹理：_CameraDepthNormalTexture。Unity将法线编码到R和G通道里，而深度编码到B和A通道里。

我们可以通过DecodeDepthNormal方法获取深度和法线：



```cpp
float2 uv = i.uv;
                half depth;
                half3 norm;
                DecodeDepthNormal(tex2D(_CameraDepthNormalTexture, uv), depth, norm);
```

使用深度法线纹理，我们可以实现边缘检测等图像效果（可能会在后续文章中介绍）。








