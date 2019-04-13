
# Meshlab导入obj文件 - paohui0134的博客 - CSDN博客


2016年12月07日 21:23:37[paohui0134](https://me.csdn.net/paohui0134)阅读数：2194


# Meshlab导入obj文件
代码位于vcglib\wrap\io_trimesh（注：为Meshlab中vcg的源代码）
static int Open( OpenMeshType &m, const char * filename, Info &oi)
## 导入文件的流程
### 1.读取大致信息
```python
if
```
```python
(oi.mask ==
```
```python
0
```
```python
)
   LoadMask(filename, oi);
```
meshlab在Open之前有单独调用LoadMask
大致流程如下：
1.获取文件长度，初始化各种标志
2.遍历每一行，统计顶点数量、法线数量、纹理坐标数量、面数量，同时修改部分标记量的值
3.根据统计的信息打开部分表计量，比如如果顶点数量与纹理坐标数量一致，打开IOM_VERTTEXCOORD标记等
接着将通过文件分析得到的mask与mesh本身设置的mask统一
```python
Mask:
```
```python
:ClampMask<OpenMeshType>(m,oi
```
```python
.mask
```
```python
)
```
```python
;
```
### 读取点数据
1.根据点的数量分配内存
```python
VertexIterator vi
```
```python
=
```
```python
vcg
```
```python
::tri
```
```python
::Allocator
```
```python
<
```
```python
OpenMeshType
```
```python
>
```
```python
::AddVertices
```
```python
(m,oi
```
```python
.
```
```python
numVertices);
```
2.解析以v开头的行，读取点的坐标信息，如果有颜色就读取颜色
### 读取纹理坐标数据
解析以vt开头的行，先将纹理坐标数据存放在临时的vector中`std::vector<ObjTexCoord>    texCoords;`
### 解析法线数据
解析以vn开头的行，先将法线信息存放在临时的vector中`std::vector<CoordType>  normals`
### 解析面信息
解析以f或者q开头的行，此处以常规的f为例，也就是TRIMESH mesh
#### 相关数据结构：
```python
1.
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
std
```
```python
::
```
```python
vector
```
```python
<vcg::Point3f>
```
```python
>
```
```python
polygonVect(
```
```python
1
```
```python
) 多边形容器
```
```python
2.
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
indexVVect(vertexesPerFace)  顶点序号
```
```python
3.
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
indexNVect(vertexesPerFace); 法线序号
```
```python
4.
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
indexTVect(vertexesPerFace); 纹理坐标序号
```
```python
5.
```
```python
std
```
```python
::
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
indexTriangulatedVect;       三角形序号
```
#### 解析面数据的序号信息
```python
//分别解析顶点  法线  纹理的序号
SplitToken(tokens[pi+1], indexVVect[pi],indexNVect[pi],indexTVect[pi], inputMask);
//确保序号正确
GoodObjIndex(indexVVect[pi],numVertices);
GoodObjIndex(indexTVect[pi],oi.numTexCoords);
//根据之前读取的定点数据获取顶点坐标
polygonVect[
```
```python
0
```
```python
][
```
```python
pi
```
```python
].Import(m.vert[indexVVect[pi]].cP());
```
#### 三角化
根据面的定点数对面进行三角化，获取三角形的顶点序号。一般mesh数据是三个点，那么这种情况下只是简单的获取了序号
#### 处理每个三角形
根据三角化之后的顶点序号处理每一个三角形
```python
//首先获取面上每个点的顶点序号、纹理序号、法线序号
```
```python
//三角化之后每个顶点的相对序号，也就是在当前面顶点中的序号
```
```python
locInd[iii]=indexTriangulatedVect[pi+iii];
```
```python
//根据相对序号获取绝对序号，也就是在所有顶点中的序号
```
```python
ff.v[iii]=indexVVect[ locInd[iii] ];
ff.n[iii]=indexNVect[ locInd[iii] ];
ff.t[iii]=indexTVect[ locInd[iii] ];
```
获取面使用的纹理的序号
```python
ff
```
```python
.tInd
```
```python
=materials[currentMaterialIdx]
```
```python
.index
```
```python
;
```
判断顶点是否重复
```python
if ((ff
```
```python
.v
```
```python
[
```
```python
0
```
```python
] == ff
```
```python
.v
```
```python
[
```
```python
1
```
```python
]) || (ff
```
```python
.v
```
```python
[
```
```python
0
```
```python
] == ff
```
```python
.v
```
```python
[
```
```python
2
```
```python
]) || (ff
```
```python
.v
```
```python
[
```
```python
1
```
```python
] == ff
```
```python
.v
```
```python
[
```
```python
2
```
```python
]))
```
#### 将面信息添加到CMeshO
之前解析的面信息都在`std::vector<ObjIndexedFace> indexedFaces`中，所有的面解析完成之后，通过该容器创建面。
```python
//分配内存
```
```python
vcg::tri::Allocator<OpenMeshType>::AddFaces(m,numTriangles);
```
```python
//填写面信息
```
```python
for
```
```python
(
```
```python
unsigned
```
```python
int
```
```python
j=
```
```python
0
```
```python
;j<indexedFaces[i]
```
```python
.v
```
```python
.size
```
```python
();++j)
    *******
```

