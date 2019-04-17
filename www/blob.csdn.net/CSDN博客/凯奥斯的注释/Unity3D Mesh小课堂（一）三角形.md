# Unity3D Mesh小课堂（一）三角形 - 凯奥斯的注释 - CSDN博客





2016年09月04日 19:01:56[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：9188标签：[Unity3D																[Mesh](https://so.csdn.net/so/search/s.do?q=Mesh&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity3D&t=blog)
个人分类：[Unity3D](https://blog.csdn.net/ecidevilin/article/category/6456233)

所属专栏：[Unity3D Mesh小课堂](https://blog.csdn.net/column/details/12888.html)








# Mesh顾名思义“网格”，Unity3D里面所有的模型都是由Mesh组成的，UI也不例外。

例如下图，模型上的一个个小网格就是Mesh，这些Mesh有不同的三维顶点（Vector3），共同组成了一个3D模型。




![](https://img-blog.csdn.net/20160904120425106)

Unity3D中Mesh的基本单位是三角形，学习应该由浅入深，所以今天我们就从最基本最简单的等腰三角形开始画起。




*本文作者尚为初学者，如有理解不到位的地方，欢迎指正。*



首先我们新建一个名为TestTriangle的CSharp脚本，然后打开TestTriangle，我们开始编写代码。



```
using UnityEngine;
using System.Collections;

/* ==============================================================================
 * 功能描述：创建三角形Mesh
 * 创 建 者：Eci
 * 创建日期：2016/09/04
 * ==============================================================================*/
[RequireComponent(typeof(MeshRenderer), typeof(MeshFilter))]
public class TestTriangle : MonoBehaviour {


	public float sideLength = 2;
	public float angleDegree = 100;

	private MeshFilter meshFilter;

	[ExecuteInEditMode]
	private void Awake()
	{

		meshFilter = GetComponent<MeshFilter>();
		meshFilter.mesh = Create (sideLength, angleDegree);
	}

	private void Update()
	{
	}
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
}
```





RequireComponent这一行，表示我们需要MeshRenderer和MeshFilter这两个组件，当我们将TestTriangle的代码挂在GameObject上的时候，会自动添加这两个组件。而我们要移除MeshRenderer或MeshFilter的时候，编辑器就会提示不能移除。


然后我们给出了两个公开变量，sideLength边长和angleDegree角度，因为我们这里要画的是等腰三角形，这代表的是等腰边长和等腰边长的夹角。

ExecuteInEditMode表示会在编辑器模式下运行。


Awake里，我们获取了MeshFilter并为它创建了Mesh。

Create方法里面，我们看到，先后为新建的Mesh创建了vertices（定点），triangles（三角形），uv（纹理坐标）。

vertices很简单，就是计算三角形三个顶点的坐标，因为是个二维图形，所以y坐标都为零。


triangles里保存的是vertices的下标。


uv暂时我们用不到，所以全部设为零。在后面文章中我们会介绍uv的用法。

最后返回mesh。

在编辑器里，点击运行，我们就可以看到一个紫色（因为没有材质）的三角形。

但是只能在运行的时候才看得到这个三角形，编辑器里看不到怎么办？添加下面这段代码：



```
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
		Mesh mesh = Create(sideLength, angleDegree);
		int[] tris = mesh.triangles;
		Gizmos.DrawLine(mesh.vertices[tris[0]], mesh.vertices[tris[1]]);
		Gizmos.DrawLine(mesh.vertices[tris[0]], mesh.vertices[tris[2]]);
		Gizmos.DrawLine(mesh.vertices[tris[1]], mesh.vertices[tris[2]]);
	}
```





关于OnDrawGizmos和OnDrawGizmosSelected可以参考下面这个链接：
[http://www.ceeger.com/Script/Gizmos/Gizmos.html](http://www.ceeger.com/Script/Gizmos/Gizmos.html)
简单来讲就是在编辑器模式下，绘制辅助线框。

这样一个简单的等腰三角形Mesh的绘制就完成了。什么？你不满意？我们稍微整理一下代码：



```
using UnityEngine;
using System.Collections;

/* ==============================================================================
 * 功能描述：创建三角形Mesh
 * 创 建 者：Eci
 * 创建日期：2016/09/04
 * ==============================================================================*/
[RequireComponent(typeof(MeshRenderer), typeof(MeshFilter))]
public class TestTriangle : MonoBehaviour {


	public float sideLength = 2;
	public float angleDegree = 100;
	private static readonly int ANGLE_DEGREE_PRECISION = 1000;
	private static readonly int SIDE_LENGTH_PRECISION = 1000;

	private MeshFilter meshFilter;

	private TriangleMeshCreator creator = new TriangleMeshCreator();

	[ExecuteInEditMode]
	private void Awake()
	{

		meshFilter = GetComponent<MeshFilter>();
	}

	private void Update()
	{
		meshFilter.mesh = creator.CreateMesh(sideLength, angleDegree);
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
		Mesh mesh = creator.CreateMesh(sideLength, angleDegree);
		int[] tris = mesh.triangles;
		Gizmos.DrawLine(transformToWorld(mesh.vertices[tris[0]]), transformToWorld(mesh.vertices[tris[1]]));
		Gizmos.DrawLine(transformToWorld(mesh.vertices[tris[0]]), transformToWorld(mesh.vertices[tris[2]]));
		Gizmos.DrawLine(transformToWorld(mesh.vertices[tris[1]]), transformToWorld(mesh.vertices[tris[2]]));
	}

	private Vector3 transformToWorld(Vector3 src)
	{
		return transform.TransformPoint(src);
	}

	private class TriangleMeshCreator
	{
		private float _sideLength;
		private float _angleDegree;

		private Mesh _cacheMesh ;
		public Mesh CreateMesh(float sideLength, float angleDegree)
		{
			if (checkDiff(sideLength, angleDegree))
			{
				Mesh newMesh = Create(sideLength, angleDegree);
				if (newMesh != null)
				{
					_cacheMesh = newMesh;
					this._sideLength = sideLength;
					this._angleDegree = angleDegree;
				}
			}
			return _cacheMesh;
		}

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

		private bool checkDiff(float sideLength, float angleDegree)
		{
			return (int)((sideLength - this._sideLength) * SIDE_LENGTH_PRECISION) != 0 ||
				(int)((angleDegree - this._angleDegree) * ANGLE_DEGREE_PRECISION) != 0;
		}
	}


}
```


为GameObject的MeshRenderer添加材质，我们就可以看到有颜色的三角形了。因为uv值都为0，所以是单一颜色。不过没关系，下一堂课，我们会为三角形添加不同的纹理。

[附上代码下载链接](http://download.csdn.net/download/ecidevilin/9621503)






