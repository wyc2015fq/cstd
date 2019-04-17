# Unity3D Mesh小课堂（五）CombineMeshes合并网格 - 凯奥斯的注释 - CSDN博客





2016年09月09日 23:40:59[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：13860标签：[Unity3D																[Mesh																[Material																[合并网格																[Combine](https://so.csdn.net/so/search/s.do?q=Combine&t=blog)
个人分类：[Unity3D](https://blog.csdn.net/ecidevilin/article/category/6456233)

所属专栏：[Unity3D Mesh小课堂](https://blog.csdn.net/column/details/12888.html)](https://so.csdn.net/so/search/s.do?q=合并网格&t=blog)




# 合并网格可以提升性能，而且也可以让我们更好更灵活的管理模型。

例如下面这三个模型：

![](https://img-blog.csdn.net/20160909223112213)


有些时候我们会希望这三个模型使用同一个Collider，但是我们显然不能让美术再给我们专门为碰撞做模型来。因为他们可能会有不同的组合，或者跟其他的物体进行组合，如果每种组合都做一个碰撞模型的话，美术大人会砍死我们的，而且这样也不利于维护（调整一下坐标，这个碰撞模型就得重做）。

那么只有我们程序来实现了。

这里我们用到了Untiy的一个API——[CombineMeshes](http://docs.unity3d.com/ScriptReference/Mesh.CombineMeshes.html)。

CombineMeshes有三个参数：

combine:[CombineInstance](http://docs.unity3d.com/ScriptReference/CombineInstance.html)（struct）类型的数组，这个结构里包括了需要合并的网格（mesh），子网格的索引（subMeshIndex）和转换矩阵（transform）。

mergeSubMeshes:bool值，是否需要将所有的Mesh合并成一个。

useMatrices:bool值，是否使用矩阵，如果为false，那么CombineInstance里的transform将会被忽略。

那么我们来实现这样一个功能。

我们选取图中某个对象做为父对象，将其他两个对象作为它的子对象。

并新建一个脚本，添加到父对象上。脚本如下：



```
using UnityEngine;
using System.Collections;

[RequireComponent(typeof(MeshRenderer), typeof(MeshFilter))]
public class CombineMesh : MonoBehaviour {

	void Start()
	{
		MeshFilter[] meshFilters = GetComponentsInChildren<MeshFilter> ();
		CombineInstance[] combine = new CombineInstance[meshFilters.Length];
		Material[] mats = new Material[meshFilters.Length];
		Matrix4x4 matrix = transform.worldToLocalMatrix;
		for (int i = 0; i < meshFilters.Length; i++) {
			MeshFilter mf = meshFilters [i];
			MeshRenderer mr = meshFilters [i].GetComponent<MeshRenderer> ();
			if (mr == null) {
				continue;
			}
			combine[i].mesh = mf.sharedMesh;
			combine[i].transform = matrix * mf.transform.localToWorldMatrix;
			mr.enabled = false;
			mats [i] = mr.sharedMaterial;
		}
		MeshFilter thisMeshFilter = GetComponent<MeshFilter> ();
		Mesh mesh = new Mesh ();
		mesh.name = "Combined";
		thisMeshFilter.mesh = mesh;
		mesh.CombineMeshes (combine, false);
		MeshRenderer thisMeshRenderer = GetComponent<MeshRenderer> ();
		thisMeshRenderer.sharedMaterials = mats;
		thisMeshRenderer.enabled = true;

		MeshCollider thisMeshCollider = GetComponent<MeshCollider> ();
		if (thisMeshCollider != null) {
			thisMeshCollider.sharedMesh = mesh;
		}

	}
}
```

脚本里我们做了几件事情：


1、将MeshFilter里的[sharedMesh](http://docs.unity3d.com/ScriptReference/MeshFilter-sharedMesh.html)保存给CombineInstance。

2、计算transform矩阵，将子对象（或本对象）本地转世界的矩阵和本对象世界转本地的矩阵相乘，就得到了子对象（或本对象）转换到本对象本地的矩阵。

3、将MeshRenderer的sharedMaterial保存到数组里（关于sharedMaterial可以参考我之前的[文章](http://blog.csdn.net/ecidevilin/article/details/52461525)）。并且将MeshRenderer设置为不可用的，这样子对象就不会再渲染了。如果这里换成gameObject.SetActive(false)是不恰当的，因为那样的话，gameObject上挂载的其他组件也不能执行。

4、新建一个mesh，赋给MeshFilter，并调用CombineMesh的方法合并子mesh。因为我们要用不同的Material，所以第二个参数mergeSubMeshes为false。（顺口一提，如果第三个参数useMatrices为false的话，所有mesh都会从local原点开始）

5、将Material数组赋值给MeshRenderer的sharedMaterials，并将MeshRenderer设置为可用的。

6、最后判断如果存在MeshCollider，将MeshCollider的sharedMesh赋值为新建的mesh。




*最后要补充一点，如果本对象和所有子对象都是用相同的材质和纹理的话，那么这段代码就需要修改一下，把Material相关的代码去掉即可，然后CombineMesh的第二个参数要改为true（默认true）。当然，添加一个public的bool变量来控制是否使用同一个Material的话也是不错的选择。*

*附上可用的代码[下载链接](http://download.csdn.net/detail/ecidevilin/9628241)。*](https://so.csdn.net/so/search/s.do?q=Material&t=blog)](https://so.csdn.net/so/search/s.do?q=Mesh&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity3D&t=blog)




