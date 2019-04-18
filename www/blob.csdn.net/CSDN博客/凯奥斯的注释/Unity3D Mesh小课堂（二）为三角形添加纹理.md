# Unity3D Mesh小课堂（二）为三角形添加纹理 - 凯奥斯的注释 - CSDN博客





2016年09月05日 20:44:50[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4428
所属专栏：[Unity3D Mesh小课堂](https://blog.csdn.net/column/details/12888.html)









[上一篇中](http://blog.csdn.net/ecidevilin/article/details/52431530)，我们讲解了如何绘制一个等腰三角形的Mesh，这一篇，我们就为这个三角形添加纹理。

为了方便阅读，先把创建Mesh的源码片段贴出来出来。



```
private Mesh Create(float sideLength, float angleDegree)
		{
			Mesh mesh = new Mesh();
			Vector3[] vertices = new Vector3[3];

			float angle = Mathf.Deg2Rad * angleDegree;
			float halfAngle = angle / 2;
			vertices [0] = Vector3.zero;
			float cosA = Mathf.Cos (halfAngle);
			float sinA = Mathf.Sin (halfAngle);
			vertices [1] = new Vector3 (cosA * sideLength, 0, sinA * sideLength);
			vertices [2] = new Vector3 (cosA * sideLength, 0, -sinA * sideLength);

			int[] triangles = new int[3];
			triangles [0] = 0;
			triangles [1] = 1;
			triangles [2] = 2;

			mesh.vertices = vertices;
			mesh.triangles = triangles;

			Vector2[] uvs = new Vector2[vertices.Length];
			for (int i = 0; i < uvs.Length; i++)
			{
				uvs[i] = Vector2.zero;
			}
			mesh.uv = uvs;

			return mesh;
		}
```




我们看到最后一段将所有的uv（纹理坐标）赋值为Vector2.zero。那么今天我们就要通过修改uv值来完成为三角形添加纹理。

那么uv是什么呢？可以参考百度百科[UV坐标](http://baike.baidu.com/link?url=Ud19scY012C7WZnd-LDdi7etKQ8CbX9fAWGW6D4xOYle6lGyU5dgEzYPO_K5vyAOXeW22-EC68wd6JtCS6fzDa)，简单的来讲就是某个点对应于贴图中的坐标。

光说没有用，我们先做起来。首先新建一个material（材质），shader（着色器）选择Legacy Shaders/Transparent/Diffuse。（这里shader就不展开讲了，要展开的话实在太多了，以后我们有机会单独再讲。）

然后我找到了一张三角形的图片，并为它做了标记。

![](https://img-blog.csdn.net/20160905202527954)


导入Unity3D，把材质的贴图设为这张图，并找到挂载[TestTriangle](http://download.csdn.net/download/ecidevilin/9621503)脚本的GameObject，把Mesh Renderer这个组件上的Material设为我们新建的材质。

修改一下uv部分的代码：



```
Vector2[] uvs = new Vector2[vertices.Length];
			uvs [0] = new Vector2(0, 0.5f);
			uvs [1] = Vector2.one;
			uvs [2] = Vector2.right;
			mesh.uv = uvs;
```
点击运行，我们就可以看到一个拥有贴图的三角形了。


通过上面的例子，我们知道：

三处的三角形顶点对应的uv坐标是(0,0.5f),(1,1)和(1,0)。

这样我们就可以知道，uv坐标系是从0到1，从左到右，自下而上增加的坐标系。




当然我们也可以调整uv的值：



```
uvs [0] = Vector2.one;
			uvs [1] = Vector2.right;
			uvs [2] = new Vector2(0, 0.5f);
```

或者




```
uvs [0] = new Vector2(0, 0.5f);
			uvs [1] = Vector2.one;
			uvs [2] = new Vector2(0.5f, 0.5f);
```

都是可以的，会显示出不同的效果。


这样我们就完成了最简单的等腰三角形Mesh的绘制和添加纹理，后面的Mesh小课堂，我们会绘制一些较为复杂的Mesh，而且uv坐标也会有不同的应用。



