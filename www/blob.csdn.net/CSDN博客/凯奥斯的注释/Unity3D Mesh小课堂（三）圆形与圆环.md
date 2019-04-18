# Unity3D Mesh小课堂（三）圆形与圆环 - 凯奥斯的注释 - CSDN博客





2016年09月07日 08:22:17[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：9052标签：[Mesh																[Unity3D																[圆形																[圆环																[扇形](https://so.csdn.net/so/search/s.do?q=扇形&t=blog)
个人分类：[Unity3D](https://blog.csdn.net/ecidevilin/article/category/6456233)

所属专栏：[Unity3D Mesh小课堂](https://blog.csdn.net/column/details/12888.html)](https://so.csdn.net/so/search/s.do?q=圆环&t=blog)




# 之前我们介绍了如何绘制一个[三角形的Mesh](http://blog.csdn.net/ecidevilin/article/details/52431530)。今天讲解一下如何绘制一个圆形的Mesh。

Unity3D中Mesh的基本单位是三角形，而圆形就是由许许多多的三角形组成的。那么我们就知道了绘制圆形的Mesh需要两个变量



```
public float Radius = 5;	//半径
	public int Segments = 50;	//分割数
```


先加上基本的代码





```
[RequireComponent(typeof(MeshRenderer), typeof(MeshFilter))]
public class TestCircle : MonoBehaviour {

	public float Radius = 5;	//半径
	public int Segments = 50;	//分割数

	private MeshFilter meshFilter;

	void Awake()
	{

		meshFilter = GetComponent<MeshFilter>();
		meshFilter.mesh = CreateMesh(Radius, Segments);
	}

	Mesh CreateMesh(float radius, int segments)
	{
		Mesh mesh = new Mesh ();
		return mesh;
	}
}
```

那么CreateMesh这个方法要怎么实现呢？


首先是vertices（顶点）



```
int vlen = 1 + segments;
		Vector3[] vertices = new Vector3[vlen];
		vertices[0] = Vector3.zero;

		float angleDegree = 360;
		float angle = Mathf.Deg2Rad * angleDegree;
		float currAngle = angle / 2;
		float deltaAngle = angle / segments;
		for (int i = 1; i < vlen; i++)
		{
			float cosA = Mathf.Cos(currAngle);
			float sinA = Mathf.Sin(currAngle);
			vertices[i] = new Vector3 (cosA * radius, 0, sinA * radius);
			currAngle -= deltaAngle;
		}
```




vertices[0]保存圆心坐标，后面的segments个Vector3保存圆（边）上的点。

接着是triangles（三角形），里面实际保存的是vertices的下标



```
int tlen = segments * 3;
		int[] triangles = new int[tlen];
		for (int i = 0, vi = 1; i < tlen - 3; i += 3, vi++)
		{
			triangles[i] = 0;
			triangles[i + 1] = vi;
			triangles[i + 2] = vi + 1;
		}
		triangles [tlen - 3] = 0;
		triangles [tlen - 2] = vlen - 1;
		triangles [tlen - 1] = 1;
```

为了完成闭环，将最后一个三角形单独拎出来。


需要注意的是：三角形必须是顺时针取顶点。

最后是uv（纹理坐标），不太理解是什么的，可以先看一下之前的博文：[为三角形添加纹理](http://blog.csdn.net/ecidevilin/article/details/52443127)



```
Vector2[] uvs = new Vector2[vlen];
		for (int i = 0; i < vlen; i++)
		{
			uvs [i] = new Vector2 (vertices[i].x / radius / 2 + 0.5f, vertices[i].z / radius / 2 + 0.5f);
		}
```

最后把属性赋给mesh就可以了




```
mesh.vertices = vertices;
		mesh.triangles = triangles;
		mesh.uv = uvs;
```

我们测试一下，导入一张图片


![](https://img-blog.csdn.net/20160907072801902)

我特意找了一张圆外不透明的图片。

新建一个material，shader使用Unlit/Texture，选择这张图片作为纹理。

新建一个GameObject，添加TestCircel作为组件，讲MeshRenderer的Material设置为新建的material。

点击运行，就可以看到圆了。




接下来稍微提升一下难度：假如我需要一个圆环Mesh怎么办？

圆形是由segments个等腰三角形组成的，圆环就是有segments个等腰梯形组成的。

那么等腰梯形由什么组成？两个三角形（多个也是可以的）。

首先添加一个变量，内圆半径


`public float InnerRadius = 3;	//内圆半径`
CreateMesh方法也相应的增加了一个参数。


vertices（顶点）



```
int vlen = segments * 2 + 2;
		Vector3[] vertices = new Vector3[vlen];

		float angleDegree = 360;
		float angle = Mathf.Deg2Rad * angleDegree;
		float currAngle = angle / 2;
		float deltaAngle = angle / segments;
		for (int i = 0; i < vlen; i+=2)
		{
			float cosA = Mathf.Cos(currAngle);
			float sinA = Mathf.Sin(currAngle);
			vertices[i] = new Vector3 (cosA * innerRadius, 0, sinA * innerRadius);
			vertices[i + 1] = new Vector3 (cosA * radius, 0, sinA * radius);
			currAngle -= deltaAngle;
		}
```




每一个步长（2）保存梯形（非平行边）的两个顶点的坐标。我们为180度和-180度分别创建了两个值相等的点，是为了方便triangles的计算。

triangles（三角形）



```
int tlen = segments * 6;
		int[] triangles = new int[tlen];
		for (int i = 0, vi = 0; i < tlen; i += 6, vi+=2)
		{
			triangles[i] = vi;
			triangles[i + 1] = vi + 1;
			triangles[i + 2] = vi + 3;
			triangles[i + 3] = vi + 3;
			triangles[i + 4] = vi + 2;
			triangles[i + 5] = vi;
		}
```

把每个分割梯形再分割成两个三角形。仍然需要注意的是：三角形必须是顺时针取顶点。


uv（纹理坐标）代码不变


*~~然后我们就得到了一个被打了马赛克的圆（误）~~*

![](https://img-blog.csdn.net/20160907080043260)


假如我们需要一个扇形/扇面怎么办？其实我已经预留好了修改的余地，把顶点代码片段里的


`		float angleDegree = 360;`去掉，作为参数输入即可。InnerRadius=0就是扇形，InnerRadius>0就是扇面。





加上Gizmos，调整一下代码



```
using UnityEngine;
using System.Collections;

[RequireComponent(typeof(MeshRenderer), typeof(MeshFilter))]
public class TestCircle : MonoBehaviour {

	public float Radius = 5;	//半径
	public int Segments = 50;	//分割数
	public float InnerRadius = 3;	//内圆半径
	public float AngleDegree = 90;

	private CircleMeshCreator _creator = new CircleMeshCreator();

	private MeshFilter _meshFilter;

	void Awake()
	{

		_meshFilter = GetComponent<MeshFilter>();
	}

	void Update()
	{
		_meshFilter.mesh = _creator.CreateMesh(Radius, Segments, InnerRadius, AngleDegree);
	}

	void OnDrawGizmos()
	{
		Gizmos.color = Color.gray;
		DrawMesh();
	}

	void OnDrawGizmosSelected()
	{
		Gizmos.color = Color.green;
		DrawMesh();
	}

	private void DrawMesh()
	{
		Mesh mesh = _creator.CreateMesh(Radius, Segments, InnerRadius, AngleDegree);
		int[] tris = mesh.triangles;
		for (int i = 0; i < tris.Length; i+=3)
		{
			Gizmos.DrawLine(TransformToWorld(mesh.vertices[tris[i]]), TransformToWorld(mesh.vertices[tris[i + 1]]));
			Gizmos.DrawLine(TransformToWorld(mesh.vertices[tris[i]]), TransformToWorld(mesh.vertices[tris[i + 2]]));
			Gizmos.DrawLine(TransformToWorld(mesh.vertices[tris[i+1]]), TransformToWorld(mesh.vertices[tris[i + 2]]));
		}
	}

	private Vector3 TransformToWorld(Vector3 src)
	{
		return transform.TransformPoint(src);
	}

	private class CircleMeshCreator
	{
		private static readonly int PRECISION = 1000;
		private float _radius;
		private int _segments;
		private float _innerRadius;
		private float _angleDegree;

		private Mesh _cacheMesh ;

		public Mesh CreateMesh(float radius, int segments, float innerRadius, float angleDegree)
		{
			if (checkDiff(radius, segments, innerRadius, angleDegree))
			{
				Mesh newMesh = Create(radius, segments, innerRadius, angleDegree);
				if (newMesh != null)
				{
					_cacheMesh = newMesh;
					this._radius = radius;
					this._segments = segments;
					this._innerRadius = innerRadius;
					this._angleDegree = angleDegree;
				}
			}
			return _cacheMesh;
		}

		private Mesh Create(float radius, int segments, float innerRadius, float angleDegree)
		{

			if (segments <= 0)
			{
				segments = 1;
				#if UNITY_EDITOR
				Debug.Log("segments must be larger than zero.");
				#endif
			}

			Mesh mesh = new Mesh();
			int vlen = segments * 2 + 2;
			Vector3[] vertices = new Vector3[vlen];

			float angle = Mathf.Deg2Rad * angleDegree;
			float currAngle = angle / 2;
			float deltaAngle = angle / segments;
			for (int i = 0; i < vlen; i+=2)
			{
				float cosA = Mathf.Cos(currAngle);
				float sinA = Mathf.Sin(currAngle);
				vertices[i] = new Vector3 (cosA * innerRadius, 0, sinA * innerRadius);
				vertices[i + 1] = new Vector3 (cosA * radius, 0, sinA * radius);
				currAngle -= deltaAngle;
			}

			int tlen = segments * 6;
			int[] triangles = new int[tlen];
			for (int i = 0, vi = 0; i < tlen; i += 6, vi+=2)
			{
				triangles[i] = vi;
				triangles[i + 1] = vi + 1;
				triangles[i + 2] = vi + 3;
				triangles[i + 3] = vi + 3;
				triangles[i + 4] = vi + 2;
				triangles[i + 5] = vi;
			}


			Vector2[] uvs = new Vector2[vlen];
			for (int i = 0; i < vlen; i++)
			{
				uvs [i] = new Vector2 (vertices[i].x / radius / 2 + 0.5f, vertices[i].z / radius / 2 + 0.5f);
			}

			mesh.vertices = vertices;
			mesh.triangles = triangles;
			mesh.uv = uvs;

			return mesh;
		}

		private bool checkDiff(float radius, int segments, float innerRadius, float angleDegree)
		{
			return segments != this._segments || (int)((angleDegree - this._angleDegree) * PRECISION) != 0 ||
				(int)((radius - this._radius) * PRECISION) != 0 || (int)((innerRadius - this._innerRadius) * PRECISION) != 0;
		}
	}

}
```


恩，改成这样，应该命名为TestSector更为合适一点。

最后附上源码[下载链接](http://download.csdn.net/detail/ecidevilin/9623677)](https://so.csdn.net/so/search/s.do?q=圆形&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity3D&t=blog)](https://so.csdn.net/so/search/s.do?q=Mesh&t=blog)




