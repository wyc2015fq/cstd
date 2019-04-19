# Direct3D学习（六）：动画基础（3）网格模型基础 - 逍遥剑客 - CSDN博客
2007年05月30日 03:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2423
ID3DXMesh继承自ID3DXBaseMesh接口
ID3DXBaseMesh包含一个顶点缓存和一个索引缓存
一个mesh是由一组子集（subset）组成的
子集是具有相同属性的一组三角形
这里的属性指材质，纹理，渲染状态
属性ID存在mesh的属性缓存中，DWORD类型
子集i与材质和纹理数组的第i项对应
Mesh的顶点和索引缓存可以被重组以便更快地渲染，如根据属性把三角形重新排一下序
优化有时要用到邻接数组
Mesh可以被拷贝，拷贝的同时可以改变顶点格式
Mesh不包括顶点法线数据时，可以用D3DXComputeNormals来计算HRESULT D3DXComputeNormals(  
 LPD3DXBASEMESH pMesh, // Mesh to compute normals of. 
const DWORD *pAdjacency// Input adjacency info.  
);  
它会使用平均法线来产生法线
ID3DXPMesh用来表示渐近网格，允许我们通过简化缩减转换（Edge Collapse Transformations,ECT）来简化mesh，这在LOD里很常用
![](http://images.blogcn.com//2007/5/30/3/xoyojank,20070530031336951.JPG)
这根mipmaps纹理很像。近了用高精度，远了用低精度，避免浪费，还可以加快速度呢
包围体
常见的有包围盒和包围球
![](http://images.blogcn.com//2007/5/30/3/xoyojank,20070530031337420.JPG)
常用于可见性检测和碰撞检测
D3DX库提供了计算它们的函数，为别为D3DXComputeBoundingBox和D3DXComputeBoundingSphere
Reference
Introduction.to.3D.Game.Programming.with.DirectX.9.0
