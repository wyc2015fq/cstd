# Unity3D白皮书（四） Unity3D在渲染方面都帮我们做了什么？ - 凯奥斯的注释 - CSDN博客





2018年01月17日 20:58:00[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：932
所属专栏：[Unity3D白皮书](https://blog.csdn.net/column/details/13147.html)









未尽和错误之处欢迎打脸…… 
![这里写图片描述](https://img-blog.csdn.net/20180117201710809)

1、顶点数据：包括顶点位置、顶点色、法线、uv等。这些数据从模型文件（例如FBX）中读取出来，组织成一个数据结构——Mesh，但最终渲染的时候还是从Mesh中将这些数据（可能会被更改）取出来传给GPU。 

2、Vertex shader & Fragment shader：对Unity3D开发者来说是透明的。Unity3D开发者使用ShaderLab编写Unity3D shader，这些shader会（根据平台）被编译成Vertex shader & Fragment shader传给GPU。 

3、Material：笼统上来讲，就是配置表。假设我们要直接调用OpenGL（或DirectX），就需要在代码中为Shader设置一些属性（或者说变量）。Unity3D使用Material这个“配置表”就能够动态地设置这些属性（其实还包含载入纹理的操作）。 

4、Renderer：再一次封装，Unity3D渲染（在CPU阶段）的基本单位，其实还要算上Transform和针对于MeshRenderer的MeshFilter（求问：为什么要分开？）。之所以加这一次封装，应该也考虑了不同Mesh和Material之间的组合。在送进GPU之前的各种操作都是以Renderer为单位的。 

5、Culling：有官方文档，不废话。[https://docs.unity3d.com/Manual/OcclusionCulling.html](https://docs.unity3d.com/Manual/OcclusionCulling.html)

注：区别于渲染管线的裁剪。 

6、Sorting：以Shader中的Queue和Renderer中的sortingLayerID+sortingOrder为基准（Queue的优先级更大）。 

7、Batching：有官方文档，不废话。[https://docs.unity3d.com/Manual/DrawCallBatching.html](https://docs.unity3d.com/Manual/DrawCallBatching.html)

还是要废两句话：注意官方文档里有一句话：
> 
Because it works by transforming all GameObject vertices into world space on the CPU, it is only an advantage if that work is smaller than doing a draw call. 


这就反映了一个问题：Batching并不总是好的。




