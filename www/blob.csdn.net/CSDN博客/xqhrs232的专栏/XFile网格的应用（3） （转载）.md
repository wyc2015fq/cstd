# XFile网格的应用（3） （转载） - xqhrs232的专栏 - CSDN博客
2009年07月12日 15:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1155
本篇是[XFile网格的应用（2）](http://www.cppblog.com/lovedday/archive/2007/05/18/24345.html)的续篇。
上篇介绍了.X文件网格的渲染方法，如果需要创建自己的网格文件，并将它渲染出来，那么可以考虑创建一个空的网格，然后读取网格文件内容，将顶点，材质和纹理数据写入以上的网格相关缓冲区中。
创建一个自定义顶点格式的空Mesh网格可由 D3DXCreateMeshFVF来实现，来看看它的具体信息说明： 
Creates a mesh object using a flexible vertex format (FVF) code.
** HRESULT D3DXCreateMeshFVF(    DWORD*** NumFaces*** ,    DWORD*** NumVertices*** ,    DWORD*** Options*** ,    DWORD*** FVF*** ,    LPDIRECT3DDEVICE9*** pD3DDevice*** ,    LPD3DXMESH **** ppMesh*** );**
#### Parameters
- *NumFaces*
[in] Number of faces for the mesh. The valid range for this number is greater than 0, and one less than the max DWORD value, typically 232 - 1, because the last index is reserved.
- *NumVertices*
[in] Number of vertices for the mesh. This parameter must be greater than 0.
- *Options*
[in] Combination of one or more flags from the D3DXMESH enumeration, specifying creation options for the mesh.
- *FVF*
[in] Combination of D3DFVF that describes the vertex format for the returned mesh. This function does not support D3DFVF_XYZRHW.
- *pD3DDevice*
[in] Pointer to an IDirect3DDevice9 interface, the device object to be associated with the mesh.
- *ppMesh*
[out] Address of a pointer to an ID3DXMesh interface, representing the created mesh object.
#### Return Values
If the function succeeds, the return value is D3D_OK. If the function fails, the return value can be one of the following: D3DERR_INVALIDCALL, E_OUTOFMEMORY.
空网格创建出来后，用ID3DXMesh接口的LockVertexBuffer函数取得顶点缓冲区的指针，这样就可以将文件的顶点数据写入已锁定的缓冲区内，最后还要调用UnlockVertexBuffer来进行解锁。
来看看 LockVertexBuffer函数的具体信息说明：
Locks a vertex buffer and obtains a pointer to the vertex buffer memory.
** HRESULT LockVertexBuffer(    DWORD*** Flags*** ,    LPVOID **** ppData*** );**
#### Parameters
- *Flags*
Combination of zero or more locking flags that describe the type of lock to perform. For this method, the valid flags are:
- D3DLOCK_DISCARD 
- D3DLOCK_NO_DIRTY_UPDATE 
- D3DLOCK_NOSYSLOCK 
- D3DLOCK_READONLY 
- D3DLOCK_NOOVERWRITE 
[in] For a description of the flags, see D3DLOCK.
*ppData*[out, retval] VOID* pointer to a buffer containing the vertex data.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
#### Remarks
When working with vertex buffers, you are allowed to make multiple lock calls; however, you must ensure that the number of lock calls match the number of unlock calls. DrawPrimitive calls will not succeed with any outstanding
 lock count on any currently set vertex buffer.
来看看参数Flags可以使用的D3DLOCK的具体定义：
A combination of zero or more locking options that describe the type of lock to perform.
|#define|Description|
|----|----|
|D3DLOCK_DISCARD|The application discards all memory within the locked region. For vertex and index buffers, the entire buffer will be discarded. This option is only valid when the resource is created with dynamic usage (see D3DUSAGE).|
|D3DLOCK_DONOTWAIT|Allows an application to gain back CPU cycles if the driver cannot lock the surface immediately. If this flag is set and the driver cannot lock the surface immediately, the lock call will return D3DERR_WASSTILLDRAWING. This flag can only be used when locking a surface created using IDirect3DDevice9::CreateOffscreenPlainSurface, IDirect3DDevice9::CreateRenderTarget, or IDirect3DDevice9::CreateDepthStencilSurface. This flag can also be used with a back buffer.|
|D3DLOCK_NO_DIRTY_UPDATE|By default, a lock on a resource adds a dirty region to that resource. This option prevents any changes to the dirty state of the resource. Applications should use this option when they have additional information about the set of regions changed during the lock operation.|
|D3DLOCK_NOOVERWRITE|Indicates that memory that was referred to in a drawing call since the last lock without this flag will not be modified during the lock. This can enable optimizations when the application is appending data to a resource. Specifying this flag enables the driver to return immediately if the resource is in use, otherwise, the driver must finish using the resource before returning from locking.|
|D3DLOCK_NOSYSLOCK|The default behavior of a video memory lock is to reserve a system-wide critical section, guaranteeing that no display mode changes will occur for the duration of the lock. This option causes the system-wide critical section not to be held for the duration of the lock.The lock operation is time consuming, but can enable the system to perform other duties, such as moving the mouse cursor. This option is useful for long-duration locks, such as the lock of the back buffer for software rendering that would otherwise adversely affect system responsiveness.|
|D3DLOCK_READONLY|The application will not write to the buffer. This enables resources stored in non-native formats to save the recompression step when unlocking.|
再来看看UnlockVertexBuffer函数的具体信息说明：
Unlocks a vertex buffer.
** HRESULT UnlockVertexBuffer();**
#### Parameters
None.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
同样，利用ID3DXMesh接口的LockIndexBuffer和UnlockIndexBuffer函数，可锁定顶点索引缓冲区，并写入三角形面的顶点索引值，然后解锁。
LockIndexBuffer的具体使用信息如下所示：
Locks an index buffer and obtains a pointer to the index buffer memory.
** HRESULT LockIndexBuffer(    DWORD*** Flags*** ,    LPVOID **** ppData*** );**
#### Parameters
- *Flags*
Combination of zero or more locking flags that describe the type of lock to perform. For this method, the valid flags are:
- D3DLOCK_DISCARD 
- D3DLOCK_NO_DIRTY_UPDATE 
- D3DLOCK_NOSYSLOCK 
- D3DLOCK_READONLY 
[in] For a description of the flags, see D3DLOCK.
*ppData*[out, retval] VOID* pointer to a buffer containing the index data. The count of indices in this buffer will be equal to ID3DXBaseMesh::GetNumFaces * 3.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
#### Remarks
When working with index buffers, you are allowed to make multiple lock calls. However, you must ensure that the number of lock calls match the number of unlock calls. DrawPrimitive calls will not succeed with any outstanding lock
 count on any currently set index buffer.
该函数使用信息说明中提到的GetNumFaces用来获取该网格包含的三角形面数，信息说明如下所示：
Retrieves the number of faces in the mesh.
** DWORD GetNumFaces();**
#### Parameters
None.
#### Return Values
Returns the number of faces in the mesh.
再来看看UnlockIndexBuffer的使用说明：
Unlocks an index buffer.
** HRESULT UnlockIndexBuffer();**
#### Parameters
None.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
接着我们可以利用ID3DXMesh接口的LockAttributeBuffer函数锁定属性缓冲区，并取得缓冲区的指针，然后将每个三角形面的子集号写入，最后调用该接口的UnlockAttributeBuffer函数将缓冲区解锁。
来看看LockAttributeBuffer的使用说明：
Locks the mesh buffer that contains the mesh attribute data, and returns a pointer to it.
** HRESULT LockAttributeBuffer(    DWORD*** Flags*** ,    DWORD ***** ppData*** );**
#### Parameters
- *Flags*
Combination of zero or more locking flags that describe the type of lock to perform. For this method, the valid flags are:
- D3DLOCK_DISCARD 
- D3DLOCK_NO_DIRTY_UPDATE 
- D3DLOCK_NOSYSLOCK 
- D3DLOCK_READONLY 
[in] For a description of the flags, see D3DLOCK.
*ppData*[out] Address of a pointer to a buffer containing a DWORD for each face in the mesh.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
#### Remarks
If ID3DXMesh::Optimize has been called, the mesh will also have an attribute table that can be accessed using the ID3DXBaseMesh::GetAttributeTable method.
再来看看UnlockAttributeBuffer的使用说明：
Unlocks an attribute buffer.
** HRESULT UnlockAttributeBuffer();**
#### Parameters
None.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
如果网格没有提供各顶点的法向量坐标数据，可使用ID3DXMesh接口的CloneMeshFVF函数复制生成一个包含法向量的网格，然后调用D3DXComputeNormals计算各顶点的法向量。
来看看CloneMeshFVF的使用说明：
Clones a mesh using a flexible vertex format (FVF) code.
** HRESULT CloneMeshFVF(    DWORD*** Options*** ,    DWORD*** FVF*** ,    LPDIRECT3DDEVICE9*** pDevice*** ,    LPD3DXMESH **** ppCloneMesh*** );**
#### Parameters
- *Options*
[in] A combination of one or more D3DXMESH flags specifying creation options for the mesh.
- *FVF*
[in] Combination of FVF codes, which specifies the vertex format for the vertices in the output mesh. For the values of the codes, see D3DFVF.
- *pDevice*
[in] Pointer to an IDirect3DDevice9 interface representing the device object associated with the mesh.
- *ppCloneMesh*
[out, retval] Address of a pointer to an ID3DXMesh interface, representing the cloned mesh.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be one of the following: D3DERR_INVALIDCALL, E_OUTOFMEMORY.
#### Remarks
ID3DXBaseMesh::CloneMeshFVF is used to reformat and change the vertex data layout. This is done by creating a new mesh object. For example, use it to to add space for normals, texture coordinates, colors, weights, etc. that were
 not present before. 
ID3DXBaseMesh::UpdateSemantics updates the vertex declaration with different semantic information without changing the layout of the vertex buffer. This method does not modify the contents of the vertex buffer. For example, use
 it to relabel a 3D texture coordinate as a binormal or tangent or vice versa.
以及D3DXComputeNormals的使用说明：
Computes unit normals for each vertex in a mesh. Provided to support legacy applications. Use D3DXComputeTangentFrameEx for better results.
** HRESULT D3DXComputeNormals(    LPD3DXBASEMESH*** pMesh*** ,    CONST DWORD **** pAdjacency*** );**
#### Parameters
- *pMesh*
[in, out] Pointer to an ID3DXBaseMesh interface, representing the normalized mesh object. This function may not take an ID3DXPMesh progressive mesh as input.
- *pAdjacency*
[in] Pointer to an array of three DWORDs per face that specify the three neighbors for each face in the created progressive mesh. This parameter is optional and should be set to NULL if it is unused.
#### Return Values
If the function succeeds, the return value is S_OK. If the function fails, the return value can be one of the following: D3DERR_INVALIDCALL, D3DXERR_INVALIDDATA, E_OUTOFMEMORY.
#### Remarks
The input mesh must have the D3DFVF_NORMAL flag specified in its flexible vertex format (FVF).
A normal for a vertex is generated by averaging the normals of all faces that share that vertex.
If adjacency is provided, replicated vertices are ignored and "smoothed" over. If adjacency is not provided, replicated vertices will have normals averaged in from only the faces explicitly referencing them.
This function simply calls **D3DXComputeTangentFrameEx** with the following input parameters:
D3DXComputeTangentFrameEx( pMesh,
                           D3DX_DEFAULT,
                           0,
                           D3DX_DEFAULT,
                           0,
                           D3DX_DEFAULT,
                           0,
                           D3DDECLUSAGE_NORMAL,
                           0,
                           D3DXTANGENT_GENERATE_IN_PLACE | D3DXTANGENT_CALCULATE_NORMALS,
                           pAdjacency,
                           -1.01f,
                           -0.01f,
                           -1.01f,
                           NULL,
                           NULL);
下面的代码通过复制的方法生成了一个具有顶点法向量信息的网格： 
LPD3DXMESH mesh;
LPD3DXMESH mesh_clone = NULL;
mesh->CloneMeshFVF(mesh->GetOptions(), mesh->GetFVF() | D3DFVF_NORMAL, _d3d_device, &mesh_clone);
D3DXComputeNormals(mesh_clone, NULL);
Mesh数据的优化
.X文件的网格数据装入缓冲区后，每个网格子集的数据可以进行优化，以提高子集渲染速度。对于顶点缓冲区来说，可以把位于同一个子集的顶点进行连续排放，减少三角形面的索引，也可以对邻接的顶点进行压缩，节省内存消耗和提高顶点的命中率。对于顶点索引缓冲区来说，该缓冲区提供了网格的所有三角形面的构成信息，如果把位于同一个子集的三角形面集中连续排放，就可以使属性缓冲区的子集编号按序排列。
ID3DXMesh接口提供了OptimizeInplace 函数进行网格数据各种类型的优化，它的使用说明如下所示： 
Generates a mesh with reordered faces and vertices to optimize drawing performance. This method reorders the existing mesh.
** HRESULT OptimizeInplace(    DWORD*** Flags*** ,    CONST DWORD **** pAdjacencyIn*** ,    DWORD **** pAdjacencyOut*** ,    DWORD **** pFaceRemap*** ,    LPD3DXBUFFER **** ppVertexRemap*** );**
#### Parameters
- *Flags*
[in] Combination of one or more D3DXMESHOPT flags, specifying the type of optimization to perform.
- *pAdjacencyIn*
[in] Pointer to an array of three DWORDs per face that specifies the three neighbors for each face in the source mesh. If the edge has no adjacent faces, the value is 0xffffffff.
- *pAdjacencyOut*
[out] Pointer to an array of three DWORDs per face that specifies the three neighbors for each face in the optimized mesh. If the edge has no adjacent faces, the value is 0xffffffff. If the value supplied for this argument is
 NULL, adjacency data is not returned. - *pFaceRemap*
[out] An array of DWORDs, one per face, that identifies the original mesh face that corresponds to each face in the optimized mesh. If the value supplied for this argument is NULL, face remap data is not returned.
- *ppVertexRemap*
[out] Address of a pointer to an ID3DXBuffer interface, which contains a DWORD for each vertex that specifies how the new vertices map to the old vertices. This remap is useful if you need to alter external data based on the
 new vertex mapping. If the value supplied for this argument is NULL, vertex remap data is not returned.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be one of the following: D3DERR_INVALIDCALL, D3DXERR_CANNOTATTRSORT, E_OUTOFMEMORY.
#### Remarks
Before running ID3DXMesh::OptimizeInplace, an application must generate an adjacency buffer by calling ID3DXBaseMesh::GenerateAdjacency. The adjacency buffer contains adjacency data, such as a list of edges and the faces that
 are adjacent to each other.
***Note***    This method will fail if the mesh is sharing its vertex buffer with another mesh, unless the D3DXMESHOPT_IGNOREVERTS is set in Flags.
下面来看看参数Flags可以使用的网格优化类型D3DXMESHOPT的具体定义：
Specifies the type of mesh optimization to be performed.
typedef enum D3DXMESHOPT
{
    D3DXMESHOPT_COMPACT = 0x01000000,
    D3DXMESHOPT_ATTRSORT = 0x02000000,
    D3DXMESHOPT_VERTEXCACHE = 0x04000000,
    D3DXMESHOPT_STRIPREORDER = 0x08000000,
    D3DXMESHOPT_IGNOREVERTS = 0x10000000,
    D3DXMESHOPT_DONOTSPLIT = 0x20000000,
    D3DXMESHOPT_DEVICEINDEPENDENT = 0x40000000,
} D3DXMESHOPT, *LPD3DXMESHOPT;
#### Constants
- D3DXMESHOPT_COMPACT 
Reorders faces to remove unused vertices and faces.
- D3DXMESHOPT_ATTRSORT 
Reorders faces to optimize for fewer attribute bundle state changes and enhanced ID3DXBaseMesh::DrawSubset performance.
- D3DXMESHOPT_VERTEXCACHE 
Reorders faces to increase the cache hit rate of vertex caches.
- D3DXMESHOPT_STRIPREORDER 
Reorders faces to maximize length of adjacent triangles.
- D3DXMESHOPT_IGNOREVERTS 
Optimize the faces only; do not optimize the vertices.
- D3DXMESHOPT_DONOTSPLIT 
While attribute sorting, do not split vertices that are shared between attribute groups.
- D3DXMESHOPT_DEVICEINDEPENDENT 
Affects the vertex cache size. Using this flag specifies a default vertex cache size that works well on legacy hardware.
#### Remarks
The D3DXMESHOPT_STRIPREORDER and D3DXMESHOPT_VERTEXCACHE optimization flags are mutually exclusive.
The D3DXMESHOPT_SHAREVB flag has been removed from this enumeration. Use D3DXMESH_VB_SHARE instead, in D3DXMESH.
D3DXLoadMeshFromX函数装入.X文件时，会自动为网格填充一个三角形面的邻接数组缓冲区，也可以利用 ID3DXMesh接口提供的GenerateAdjacency函数来生成，下面是该函数的具体使用说明:
Generate a list of mesh edges, as well as a list of faces that share each edge.
** HRESULT GenerateAdjacency(    FLOAT*** Epsilon*** ,    DWORD **** pAdjacency*** );**
#### Parameters
- *Epsilon*
[in] Specifies that vertices that differ in position by less than epsilon should be treated as coincident.
- *pAdjacency*
[in] Pointer to an array of three DWORDs per face to be filled with the indices of adjacent faces. The number of bytes in this array must be at least 3 * ID3DXBaseMesh::GetNumFaces * sizeof(DWORD).
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be one of the following: D3DERR_INVALIDCALL, E_OUTOFMEMORY.
#### Remarks
After an application generates adjacency information for a mesh, the mesh data can be optimized for better drawing performance.
The order of the entries in the adjacency buffer is determined by the order of the vertex indices in the index buffer. The adjacent triangle 0 always corresponds to the edge between the indices of the corners 0 and 1. The adjacent
 triangle 1 always corresponds to the edge between the indices of the corners 1 and 2 while the adjacent triangle 2 corresponds to the edge between the indices of the corners 2 and 0.
下面的代码段给出了利用OptimizeInplace进行优化的示例：
    // Generates a mesh with reordered faces and vertices to optimize drawing performance. 
    // This method reorders the existing mesh.
    _mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        (DWORD*) _adjacency_buffer->GetBufferPointer(), NULL, NULL, NULL);
值得一提的是，进行D3DXMESHOPT_ATTRSORT优化操作后，将生成一个Attribute Table，它是一个D3DXATTRIBUTERANGE类型的数组，反映了优化后的子集的三角形面的划分和顶点的划分。 D3DXATTRIBUTERANGE的具体定义如下所示：
Stores an attribute table entry.
typedef struct D3DXATTRIBUTERANGE {
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
} D3DXATTRIBUTERANGE, *LPD3DXATTRIBUTERANGE;
#### Members
- **AttribId**
Attribute table identifier. - **FaceStart**
Starting face. - **FaceCount**
Face count. - **VertexStart**
Starting vertex. - **VertexCount**
Vertex count. 
#### Remarks
An attribute table is used to identify areas of the mesh that need to be drawn with different textures, render states, materials, and so on. In addition, the application can use the attribute table to hide portions of a mesh by
 not drawing a given attribute identifier (AttribId) when drawing the frame.
The LPD3DXATTRIBUTERANGE type is defined as a pointer to the
**D3DXATTRIBUTERANGE** structure. 
typedef D3DXATTRIBUTERANGE* LPD3DXATTRIBUTERANGE;可以调用ID3DXMesh接口的GetAttribute方法来获得指向属性表的指针和属性表的大小，该函数具体说明如下： 
Retrieves either an attribute table for a mesh, or the number of entries stored in an attribute table for a mesh.
** HRESULT GetAttributeTable(    D3DXATTRIBUTERANGE **** pAttribTable*** ,    DWORD **** pAttribTableSize*** );**
#### Parameters
- *pAttribTable*
[in, out] Pointer to an array of D3DXATTRIBUTERANGE structures, representing the entries in the mesh's attribute table. Specify NULL to retrieve the value for pAttribTableSize.
- *pAttribTableSize*
[in, out] Pointer to either the number of entries stored in pAttribTable or a value to be filled in with the number of entries stored in the attribute table for the mesh.
#### Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
#### Remarks
An attribute table is created by ID3DXMesh::Optimize and passing D3DXMESHOPT_ATTRSORT for the Flags parameter.
An attribute table is used to identify areas of the mesh that need to be drawn with different textures, render states, materials, and so on. In addition, the application can use the attribute table to hide portions of a mesh by
 not drawing a given attribute identifier when drawing the frame.
好了，现在来看一个例子。
需要在工程中设置链接d3dx9.lib d3d9.lib dinput8.lib dxguid.lib winmm.lib。
由于文件中用到了GE_APP和GE_INPUT这两个类，它的具体使用说明请参阅[主窗口和DirectInput的封装。](http://www.cppblog.com/lovedday/archive/2007/05/07/23546.html)
文件中同时还用到了GE_TIMER这个类，它的具体使用说明请参阅 [游戏中时间的封装](http://www.cppblog.com/lovedday/archive/2007/05/07/23582.html)。
[源码与素材下载](http://www.cppblog.com/Files/lovedday/XFileLoad.rar)
来看看XFileLoad.h的定义：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->/*************************************************************************************
 [Include File]
 PURPOSE: 
    Load .X file.
*************************************************************************************/
#ifndef BASIC_XFILE_H
#define BASIC_XFILE_H
class BASIC_XFILE
{
private:
    IDirect3D9*         _d3d;
    IDirect3DDevice9*   _d3d_device;
    ID3DXBuffer*        _adjacency_buffer;
    ID3DXBuffer*        _material_buffer;
    D3DMATERIAL9*       _material_array;
    IDirect3DTexture9** _texture_array;
    DWORD               _num_materials;
    ID3DXMesh*          _mesh;
    D3DXVECTOR3         _eye_point;
public:
    BASIC_XFILE();
~BASIC_XFILE();
bool Create_D3D_Device(HWND hwnd, bool full_screen =true);
bool Load_XFile(char* x_filename);
void Render();
void Set_Camera();
void Set_Projection();
void Rotate(float angle);
void Release_Direct3D();
};
#endif
再来看看XFileLoad.cpp的定义：
/*************************************************************************************
 [Implement File]
 PURPOSE: 
    Load .X file.
*************************************************************************************/
#include "GE_COMMON.h"
#include "XFileLoad.h"
//------------------------------------------------------------------------------------
// Constrcutor, initialize data member.
//------------------------------------------------------------------------------------
BASIC_XFILE::BASIC_XFILE()
{
    _d3d_device         = NULL;
    _adjacency_buffer   = NULL;
    _material_buffer    = NULL;
    _material_array     = NULL;
    _texture_array      = NULL;
    _mesh               = NULL;
    _num_materials      =0;
   _eye_point = D3DXVECTOR3(3.0f, 0.0f, -3.0f);
}
//------------------------------------------------------------------------------------
// Destructor, release all Direct3D resource has allocated.
//------------------------------------------------------------------------------------
BASIC_XFILE::~BASIC_XFILE()
{
    Release_Direct3D();
}
//------------------------------------------------------------------------------------
// Create direct3D interface and direct3D device.
//------------------------------------------------------------------------------------
bool BASIC_XFILE::Create_D3D_Device(HWND hwnd, bool full_screen)
{
// Create a IDirect3D9 object and returns an interace to it.
    _d3d = Direct3DCreate9(D3D_SDK_VERSION);
if(_d3d == NULL)
returnfalse;
// retrieve adapter capability
    D3DCAPS9 d3d_caps;    
    _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3d_caps);
bool hardware_process_enable = (d3d_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ?true : false);
// Retrieves the current display mode of the adapter.
    D3DDISPLAYMODE display_mode;
if(FAILED(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
returnfalse;
// set present parameter for direct3D device
    D3DPRESENT_PARAMETERS present_param;
    ZeroMemory(&present_param, sizeof(present_param));
    present_param.BackBufferWidth        = WINDOW_WIDTH;
    present_param.BackBufferHeight       = WINDOW_HEIGHT;
    present_param.BackBufferFormat       = display_mode.Format;
    present_param.BackBufferCount        =1;
    present_param.hDeviceWindow          = hwnd;
    present_param.Windowed               =!full_screen;
    present_param.SwapEffect             = D3DSWAPEFFECT_FLIP;
    present_param.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT;
    present_param.EnableAutoDepthStencil = TRUE;
    present_param.AutoDepthStencilFormat = D3DFMT_D16;
// Creates a device to represent the display adapter.
    DWORD behavior_flags;
    behavior_flags = hardware_process_enable ? D3DCREATE_HARDWARE_VERTEXPROCESSING : 
                                           D3DCREATE_SOFTWARE_VERTEXPROCESSING;
if(FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior_flags, 
&present_param, &_d3d_device)))
    {
returnfalse;
    }
// create successfully
returntrue;
}
//------------------------------------------------------------------------------------
// Load .X file
//------------------------------------------------------------------------------------
bool BASIC_XFILE::Load_XFile(char* x_filename)
{
// Loads a mesh from a DirectX .x file
if(FAILED(D3DXLoadMeshFromX(
        x_filename,             // Pointer to a string that specifies the filename
        D3DXMESH_MANAGED,       // specifies creation options for the mesh
        _d3d_device,            // Pointer to an IDirect3DDevice9 interface
&_adjacency_buffer,     // Pointer to a buffer that contains adjacency data
&_material_buffer,      // Pointer to a buffer containing materials data
        NULL,                   // Pointer to a buffer containing an array of effect instances
&_num_materials,        // Pointer to the number of D3DXMATERIAL structures 
&_mesh)))               // Address of a pointer to an ID3DXMesh interface
    {
        MessageBox(NULL, "Load .X file failed.", "ERROR", MB_OK);
returnfalse;
    }
// invalid data
if(_material_buffer == NULL || _num_materials ==0)
returnfalse;
// retrieves a pointer to the data in the buffer
    D3DXMATERIAL* material = (D3DXMATERIAL*) _material_buffer->GetBufferPointer();
if(material != NULL)
    {
// allocate memory for material array and texture array
        _material_array =new D3DMATERIAL9[_num_materials];
        _texture_array  =new IDirect3DTexture9*[_num_materials];    
for(DWORD i =0; i < _num_materials; i++)
        {
// assign material to array
            _material_array[i] = material[i].MatD3D;
if(material[i].pTextureFilename != NULL)
            {
if(FAILED(D3DXCreateTextureFromFile(_d3d_device, material[i].pTextureFilename, &_texture_array[i])))
                    _texture_array[i] = NULL;
            }
        }
    }
// Generates a mesh with reordered faces and vertices to optimize drawing performance. 
// This method reorders the existing mesh.
    _mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        (DWORD*) _adjacency_buffer->GetBufferPointer(), NULL, NULL, NULL);
    _material_buffer->Release();
    _adjacency_buffer->Release();
returntrue;
}
//------------------------------------------------------------------------------------
// Render mesh.
//------------------------------------------------------------------------------------
void BASIC_XFILE::Render()
{
// Clear a render target and the depth buffer
    _d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    _d3d_device->BeginScene();
// draw all face in the mesh
for(DWORD i =0; i < _num_materials; i++)
    {
// Sets the material properties for the device
        _d3d_device->SetMaterial(&_material_array[i]);
// Assigns a texture to a stage for a device
        _d3d_device->SetTexture(0, _texture_array[i]);
// Draws a subset of a mesh
        _mesh->DrawSubset(i);
    }
    _d3d_device->EndScene();
// Presents the contents of the next buffer in the sequence of back buffers owned by the device
    _d3d_device->Present(NULL, NULL, NULL, NULL);
}
//------------------------------------------------------------------------------------
// Set camera's view transformation matrix
//------------------------------------------------------------------------------------
void BASIC_XFILE::Set_Camera()
{
    D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX view_matrix;
// get transformation matrix to transform world coordinate into view coordinate
    D3DXMatrixLookAtLH(&view_matrix, &_eye_point, &at, &up);
// set view transformation matrix to D3D device
    _d3d_device->SetTransform(D3DTS_VIEW, &view_matrix);
}
//------------------------------------------------------------------------------------
// Set projection matrix for D3D device.
//------------------------------------------------------------------------------------
void BASIC_XFILE::Set_Projection()
{
    D3DXMATRIX proj_matrix;
// build a perspective matrix using left hand 
    D3DXMatrixPerspectiveFovLH(&proj_matrix, D3DX_PI/2, WINDOW_WIDTH/WINDOW_HEIGHT, 1.0f, 1000.0f);
// set projection matrix to D3D device
    _d3d_device->SetTransform(D3DTS_PROJECTION, &proj_matrix);
// enable automatic normalization of vertex normals
    _d3d_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
// set magnification filter with linear mode
    _d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
// set minification filter with linear mode
    _d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
// set mipmap filter with point mode
    _d3d_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
// disable lighting
    _d3d_device->SetRenderState(D3DRS_LIGHTING, false);
}
//------------------------------------------------------------------------------------
// Rotate eye point with specified angle around y axis.
//------------------------------------------------------------------------------------
void BASIC_XFILE::Rotate(float angle)
{
    D3DXMATRIX mat_rotate;
// Builds a matrix that rotates around the y-axis
    D3DXMatrixRotationY(&mat_rotate, angle);
// Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
    D3DXVec3TransformCoord(&_eye_point, &_eye_point, &mat_rotate);
}
//------------------------------------------------------------------------------------
// Release all Direct3D object.
//------------------------------------------------------------------------------------
void BASIC_XFILE::Release_Direct3D()
{
for(DWORD i =0; i < _num_materials; i++)
        Safe_Release(_texture_array[i]);
    Safe_Release(_mesh);
    Safe_Release(_d3d_device);    
}
最后看看测试文件main.cpp的实现：
/*************************************************************************************
 [Implement File]
 PURPOSE: 
    Test for loading .X file.
*************************************************************************************/
#define DIRECTINPUT_VERSION 0x0800
#include "GE_COMMON.h"
#include "GE_APP.h"
#include "GE_INPUT.h"
#include "GE_TIMER.h"
#include "XFileLoad.h"
#pragma warning(disable : 43054996)
constfloat ROTATE_ANGLE =0.05;
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmd_line, int cmd_show)
{
    GE_APP ge_app;
    GE_INPUT ge_input;
    GE_TIMER ge_timer;
    BASIC_XFILE x_file;
    MSG msg = {0};
// create window
if(! ge_app.Create_Window("Transparent blending test", instance, cmd_show))
returnfalse;
    ge_timer.Init_Game_Time();
    HWND hwnd = ge_app.Get_Window_Handle();    
    SetWindowPos(hwnd, 0, 0,0,0,0, SWP_NOSIZE);
    SetCursorPos(0, 0);    
// create direct input
    ge_input.Create_Input(instance, hwnd);    
// Create direct3D interface and direct3D device.
if(! x_file.Create_D3D_Device(hwnd, false))
returnfalse;
if(x_file.Load_XFile("tiger.x"))
    {
        x_file.Set_Camera();
        x_file.Set_Projection();
        x_file.Render();
    }
float last_render_time = ge_timer.Get_Game_Play_Time();
while(msg.message != WM_QUIT)
    {
if(PeekMessage(&msg, NULL, 0,0 , PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
else
        {
// get keyboard input
if(ge_input.Read_Keyboard())
            {
if(ge_input.Is_Key_Pressed(DIK_ESCAPE))
                    PostQuitMessage(0);
// if it is time to render again
if(ge_timer.Get_Game_Play_Time() > last_render_time +30)
                {
// rotate camera
                    x_file.Rotate(ROTATE_ANGLE);
// reset camera
                    x_file.Set_Camera();
                    x_file.Render();
// update last render time
                    last_render_time = ge_timer.Get_Game_Play_Time();
                }
            }
        }
    }    
    UnregisterClass(WINDOW_CLASS_NAME, instance);
returntrue;
}
运行效果：
![](http://www.cppblog.com/images/cppblog_com/lovedday/4157/r_xfileload.jpg)
