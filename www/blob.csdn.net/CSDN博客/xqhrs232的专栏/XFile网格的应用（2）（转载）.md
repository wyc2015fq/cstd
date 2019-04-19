# XFile网格的应用（2）（转载） - xqhrs232的专栏 - CSDN博客
2009年07月12日 15:38:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1104
本篇是[XFile网格的应用（1）](http://www.cppblog.com/lovedday/archive/2007/05/18/24312.html)的续篇。
.X文件的数据装入
在上一篇的.X文件中，主场景框架 Scene_Root提供了一个变换矩阵和一个子框架Quad。这个Quad具有一个Mesh网格（包括顶点，材质，纹理等数据），其中 Scene_Root框架的变换矩阵（实际为单位矩阵）说明了Quad子框架的Mesh网格数据应做的变换，以正确摆放在父框架的空间中（这里指世界空间）。对于仅含有单一网格的.X文件来说，利用D3DXLoadMeshFromX函数可实现简单网格数据的加载。
来看看D3DXLoadMeshFromX的具体使用说明： 
Loads a mesh from a DirectX .x file.
** HRESULT D3DXLoadMeshFromX(    LPCTSTR*** pFilename*** ,    DWORD*** Options*** ,    LPDIRECT3DDEVICE9*** pD3DDevice*** ,    LPD3DXBUFFER **** ppAdjacency*** ,    LPD3DXBUFFER **** ppMaterials*** ,    LPD3DXBUFFER **** ppEffectInstances*** ,    DWORD **** pNumMaterials*** ,    LPD3DXMESH **** ppMesh*** );**
#### Parameters
- *pFilename*
[in] Pointer to a string that specifies the filename. If the compiler settings require Unicode, the data type LPCTSTR resolves to LPCWSTR. Otherwise, the string data type resolves to LPCSTR. See Remarks.
- *Options*
[in] Combination of one or more flags from the D3DXMESH enumeration, which specifies creation options for the mesh.
- *pD3DDevice*
[in] Pointer to an IDirect3DDevice9 interface, the device object associated with the mesh.
- *ppAdjacency*
[out] Pointer to a buffer that contains adjacency data. The adjacency data contains an array of three DWORDs per face that specify the three neighbors for each face in the mesh. For more information about accessing the buffer,
 see ID3DXBuffer. - *ppMaterials*
[out] Pointer to a buffer containing materials data. The buffer contains an array of D3DXMATERIAL structures, containing information from the DirectX file. For more information about accessing the buffer, see
**ID3DXBuffer** . - *ppEffectInstances*
[out] Pointer to a buffer containing an array of effect instances, one per attribute group in the returned mesh. An effect instance is a particular instance of state information used to initialize an effect. See D3DXEFFECTINSTANCE.
 For more information about accessing the buffer, see **ID3DXBuffer** .
- *pNumMaterials*
[out] Pointer to the number of **D3DXMATERIAL** structures in the ppMaterials array, when the method returns.
- *ppMesh*
[out] Address of a pointer to an ID3DXMesh interface, representing the loaded mesh.
#### Return Values
If the function succeeds, the return value is D3D_OK. If the function fails, the return value can be one of the following values: D3DERR_INVALIDCALL, E_OUTOFMEMORY.
#### Remarks
The compiler setting also determines the function version. If Unicode is defined, the function call resolves to D3DXLoadMeshFromXW. Otherwise, the function call resolves to D3DXLoadMeshFromXA because ANSI strings are being used.
All the meshes in the file will be collapsed into one output mesh. If the file contains a frame hierarchy, all the transformations will be applied to the mesh.
For mesh files that do not contain effect instance information, default effect instances will be generated from the material information in the .x file. A default effect instance will have default values that correspond to the members
 of the D3DMATERIAL9 structure.
The default texture name is also filled in, but is handled differently. The name will be Texture0@Name, which corresponds to an effect variable by the name of "Texture0" with an annotation called "Name." This will contain the string
 file name for the texture.
来看看参数Option可以指定的网格创建模式：
Flags used to specify creation options for a mesh.
typedef enum D3DXMESH
{
    D3DXMESH_32BIT = 0x001,
    D3DXMESH_DONOTCLIP = 0x002,
    D3DXMESH_POINTS = 0x004,
    D3DXMESH_RTPATCHES = 0x008,
    D3DXMESH_NPATCHES = 0x4000,
    D3DXMESH_VB_SYSTEMMEM = 0x010,
    D3DXMESH_VB_MANAGED = 0x020,
    D3DXMESH_VB_WRITEONLY = 0x040,
    D3DXMESH_VB_DYNAMIC = 0x080,
    D3DXMESH_VB_SOFTWAREPROCESSING = 0x8000,
    D3DXMESH_IB_SYSTEMMEM = 0x100,
    D3DXMESH_IB_MANAGED = 0x200,
    D3DXMESH_IB_WRITEONLY = 0x400,
    D3DXMESH_IB_DYNAMIC = 0x800,
    D3DXMESH_IB_SOFTWAREPROCESSING = 0x10000,
    D3DXMESH_VB_SHARE = 0x1000,
    D3DXMESH_USEHWONLY = 0x2000,
    D3DXMESH_SYSTEMMEM = 0x110,
    D3DXMESH_MANAGED = 0x220,
    D3DXMESH_WRITEONLY = 0x440,
    D3DXMESH_DYNAMIC = 0x880,
    D3DXMESH_SOFTWAREPROCESSING = 0x18000,
} D3DXMESH, *LPD3DXMESH;
#### Constants
- D3DXMESH_32BIT 
The mesh has 32-bit indices instead of 16-bit indices. See Remarks.
- D3DXMESH_DONOTCLIP 
Use the D3DUSAGE_DONOTCLIP usage flag for vertex and index buffers.
- D3DXMESH_POINTS 
Use the D3DUSAGE_POINTS usage flag for vertex and index buffers.
- D3DXMESH_RTPATCHES 
Use the D3DUSAGE_RTPATCHES usage flag for vertex and index buffers.
- D3DXMESH_NPATCHES 
Specifying this flag causes the vertex and index buffer of the mesh to be created with D3DUSAGE_NPATCHES flag. This is required if the mesh object is to be rendered using N-patch enhancement using Direct3D.
- D3DXMESH_VB_SYSTEMMEM 
Use the D3DPOOL_SYSTEMMEM usage flag for vertex buffers.
- D3DXMESH_VB_MANAGED 
Use the D3DPOOL_MANAGED usage flag for vertex buffers.
- D3DXMESH_VB_WRITEONLY 
Use the D3DUSAGE_WRITEONLY usage flag for vertex buffers.
- D3DXMESH_VB_DYNAMIC 
Use the D3DUSAGE_DYNAMIC usage flag for vertex buffers.
- D3DXMESH_VB_SOFTWAREPROCESSING 
Use the D3DUSAGE_SOFTWAREPROCESSING usage flag for vertex buffers.
- D3DXMESH_IB_SYSTEMMEM 
Use the D3DPOOL_SYSTEMMEM usage flag for index buffers.
- D3DXMESH_IB_MANAGED 
Use the D3DPOOL_MANAGED usage flag for index buffers.
- D3DXMESH_IB_WRITEONLY 
Use the D3DUSAGE_WRITEONLY usage flag for index buffers.
- D3DXMESH_IB_DYNAMIC 
Use the D3DUSAGE_DYNAMIC usage flag for index buffers.
- D3DXMESH_IB_SOFTWAREPROCESSING 
Use the D3DUSAGE_SOFTWAREPROCESSING usage flag for index buffers.
- D3DXMESH_VB_SHARE 
Forces the cloned meshes to share vertex buffers.
- D3DXMESH_USEHWONLY 
Use hardware processing only. For mixed-mode device, this flag will cause the system to use hardware (if supported in hardware) or will default to software processing.
- D3DXMESH_SYSTEMMEM 
Equivalent to specifying both D3DXMESH_VB_SYSTEMMEM and D3DXMESH_IB_SYSTEMMEM.
- D3DXMESH_MANAGED 
Equivalent to specifying both D3DXMESH_VB_MANAGED and D3DXMESH_IB_MANAGED.
- D3DXMESH_WRITEONLY 
Equivalent to specifying both D3DXMESH_VB_WRITEONLY and D3DXMESH_IB_WRITEONLY.
- D3DXMESH_DYNAMIC 
Equivalent to specifying both D3DXMESH_VB_DYNAMIC and D3DXMESH_IB_DYNAMIC.
- D3DXMESH_SOFTWAREPROCESSING 
Equivalent to specifying both D3DXMESH_VB_SOFTWAREPROCESSING and D3DXMESH_IB_SOFTWAREPROCESSING.
#### Remarks
A 32-bit mesh (D3DXMESH_32BIT) can theoretically support (2^32)-1 faces and vertices. However, allocating memory for a mesh that large on a 32-bit operating system is not practical.
参数ppAdjacency, ppMaterials, ppEffectInstances的类型都是LPD3DXBUFFER，它实际上是指向ID3DXBUFFER的指针：
The ID3DXBuffer interface is used as a data buffer, storing vertex, adjacency, and material information during mesh optimization and loading operations. The buffer object is used to return arbitrary length data. Also, buffer objects
 are used to return object code and error messages in methods that assemble vertex and pixel shaders.
## ID3DXBuffer Members
|Method|Description|
|----|----|
|ID3DXBuffer::GetBufferPointer|Retrieves a pointer to the data in the buffer.|
|ID3DXBuffer::GetBufferSize|Retrieves the total size of the data in the buffer.|
#### Remarks
The **ID3DXBuffer** interface is obtained by calling the D3DXCreateBuffer function.
The LPD3DXBUFFER type is defined as a pointer to the
**ID3DXBuffer** interface. 
typedef interface ID3DXBuffer ID3DXBuffer;
typedef interface ID3DXBuffer *LPD3DXBUFFER;
来看看ID3DXBuffer提供的两个方法，先来看看GetBufferPointer的具体使用信息：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->Retrieves a pointer to the data in the buffer.
LPVOID GetBufferPointer();
Parameters
None.
Return Values
Returns a pointer to the data in the buffer.
再来看看GetBufferSize的具体使用信息：
Retrieves the total size of the data in the buffer.
DWORD GetBufferSize();
Parameters
None.
Return Values
Returns the total size of the data in the buffer, in bytes.
ID3DXBuffer可利用DirectX API函数D3DXCreateBuffer来创建，不过，一般不需要直接创建，下面是该函数的具体使用信息：
Creates a buffer object.
HRESULT D3DXCreateBuffer(
  DWORD NumBytes,
  LPD3DXBUFFER * ppBuffer
);
Parameters
NumBytes 
[in] Size of the buffer to create, in bytes. 
ppBuffer 
[out] Address of a pointer to an ID3DXBuffer interface, representing the created buffer object.
Return Values
If the function succeeds, the return value is D3D_OK. If the function fails, the return value can be one of the following: E_OUTOFMEMORY.
利用pNumMaterials指针参数可获得网格mesh所使用的材质的数量，ppMesh参数则是返回的一个ID3DXMESH 接口的指针地址，利用ID3DXMESH 接口提供的方法，可对装入的网格数据进行读取，优化和渲染等。
由于利用D3DXLoadMeshFromX函数装入的材质和纹理贴图文件名被封装在一个扩展的D3DXMATERIAL结构体内，因此通常需要将材质取出，放入材质数组中，将纹理贴图文件名取出，创建相应的纹理对象，再将这些纹理对象放入纹理对象数组中，以方便后续的纹理网格渲染。
来看看D3DXMATERIAL的具体信息：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->Returns material information saved in Direct3D (.x) files.
typedef struct D3DXMATERIAL {
    D3DMATERIAL9 MatD3D;
    LPSTR pTextureFilename;
} D3DXMATERIAL, *LPD3DXMATERIAL;
Members
MatD3D 
D3DMATERIAL9 structure that describes the material properties. 
pTextureFilename 
Pointer to a string that specifies the file name of the texture. 
Remarks
The D3DXLoadMeshFromX and D3DXLoadMeshFromXof functions return an array of D3DXMATERIAL structures that specify the material color and name of the texture for each material in the mesh. The application is then required to load the texture.
The LPD3DXMATERIAL type is defined as a pointer to the D3DXMATERIAL structure. 
typedef struct D3DXMATERIAL* LPD3DXMATERIAL;
其中用到了结构体D3DMATERIAL9，它的具体定义是：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->Specifies material properties.
typedef struct D3DMATERIAL9 {
    D3DCOLORVALUE Diffuse;
    D3DCOLORVALUE Ambient;
    D3DCOLORVALUE Specular;
    D3DCOLORVALUE Emissive;
    float Power;
} D3DMATERIAL9, *LPD3DMATERIAL9;
Members
Diffuse 
Value specifying the diffuse color of the material. See D3DCOLORVALUE. 
Ambient 
Value specifying the ambient color of the material. See D3DCOLORVALUE. 
Specular 
Value specifying the specular color of the material. See D3DCOLORVALUE. 
Emissive 
Value specifying the emissive color of the material. See D3DCOLORVALUE. 
Power 
Floating-point value specifying the sharpness of specular highlights. The higher the value, the sharper the highlight.
Remarks
To turn off specular highlights, set D3DRS_SPECULARENABLE to FALSE, using D3DRENDERSTATETYPE. This is the fastest option because no specular highlights will be calculated.
For more information about using the lighting engine to calculate specular lighting, see Specular Lighting (Direct3D 9).
下面的代码演示了怎样加载.X文件，以及加载后怎样获得相应的数据。 
IDirect3D9*         _d3d;
 IDirect3DDevice9*   _d3d_device;
 ID3DXBuffer*        _adjacency_buffer;
 ID3DXBuffer*        _material_buffer;
 D3DMATERIAL9*       _material_array;
 IDirect3DTexture9** _texture_array; DWORD               _num_materials;
 ID3DXMesh*          _mesh;//------------------------------------------------------------------------------------
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
        return false;
    }    // invalid data
    if(_material_buffer == NULL || _num_materials == 0)
        return false;    // retrieves a pointer to the data in the buffer
    D3DXMATERIAL* material = (D3DXMATERIAL*) _material_buffer->GetBufferPointer();    if(material != NULL)
    {
        // allocate memory for material array and texture array
        _material_array = new D3DMATERIAL9[_num_materials];
        _texture_array  = new IDirect3DTexture9*[_num_materials];            for(DWORD i = 0; i < _num_materials; i++)
        {
            // assign material to array
            _material_array[i] = material[i].MatD3D;            if(material[i].pTextureFilename != NULL)
            {
                if(FAILED(D3DXCreateTextureFromFile(_d3d_device, material[i].pTextureFilename, &_texture_array[i])))
                    _texture_array[i] = NULL;
            }
        }
    }    // Generates a mesh with reordered faces and vertices to optimize drawing performance. 
    // This method reorders the existing mesh.
    _mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        (DWORD*) _adjacency_buffer->GetBufferPointer(), NULL, NULL, NULL);    _material_buffer->Release();
    _adjacency_buffer->Release();    return true;
}
Mesh数据的处理
执行D3DXLoadMeshFromX函数完成.X文件的加载后，Mesh的各种数据将存放在顶点缓冲区，顶点索引缓冲区，材质缓冲区，邻接顶点缓冲区，特效属性缓冲区，网格缓冲区中。其中，材质缓冲区（包含材质信息和纹理贴图文件名信息），邻接顶点缓冲区以及特效属性缓冲区的地址由 D3DXLoadMeshFromX函数成功执行后返回，其他的缓冲区地址可利用ID3DXMesh接口函数来获得。
.X网格的渲染和优化就是使用上面缓冲区的数据来进行的。顶点缓冲区给出了整个网格的顶点坐标列表（包括顶点坐标和顶点纹理坐标），顶点索引缓冲区给出了每个三角形面的顶点构成信息（三个顶点的索引值），从而间接给出了整个网格的所有三角形面的顶点信息说明，属性缓冲区为每个三角形面的一个网格子集编号，即给出了网格的每个三角形面的分组号。具有同一个Subset编号的三角形，将使用相同的材质和纹理对象进行渲染。
Mesh网格的渲染，可以用一个for循环来进行，循环的次数为总的材质数。每一次循环都将那些具有相同Subset子集号的三角形面渲染出来，即渲染每个子集Subset。不过每次循环首先要设置渲染管道线的材质和纹理对象，如下所示：
//------------------------------------------------------------------------------------
// Render mesh.
//------------------------------------------------------------------------------------
void BASIC_XFILE::Render()
{
    // Clear a render target and the depth buffer
    _d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);    _d3d_device->BeginScene();    // draw all face in the mesh
    for(DWORD i = 0; i < _num_materials; i++)
    {
        // Sets the material properties for the device
        _d3d_device->SetMaterial(&_material_array[i]);        // Assigns a texture to a stage for a device
        _d3d_device->SetTexture(0, _texture_array[i]);        // Draws a subset of a mesh
        _mesh->DrawSubset(i);
    }    _d3d_device->EndScene();    // Presents the contents of the next buffer in the sequence of back buffers owned by the device
    _d3d_device->Present(NULL, NULL, NULL, NULL);
}
上面的代码中使用Clear对Z缓冲区进行了清除，因此必须在初始化D3D设备时，开启Z缓冲区，否则该函数调用失败并且屏幕将出现重影。 
D3DPRESENT_PARAMETERS present_param;
present_param.EnableAutoDepthStencil = TRUE;
如果没有加入上面的代码，则出现重影，如下图所示：
![](http://www.cppblog.com/images/cppblog_com/lovedday/4157/r_mesh1.jpg)
Render函数中使用DrawSubset函数来渲染每一个Mesh子集，该函数只需要指定一个子集号，就可将网格中具有该子集号的三角形面渲染出来，下面是该函数的具体使用信息：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->Draws a subset of a mesh.
HRESULT DrawSubset(
  DWORD AttribId
);
Parameters
AttribId 
[in] DWORD that specifies which subset of the mesh to draw. This value is used to differentiate faces in a mesh as belonging to one or more attribute groups.
Return Values
If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be D3DERR_INVALIDCALL.
Remarks
The subset that is specified by AttribId will be rendered by the IDirect3DDevice9::DrawIndexedPrimitive method, using the D3DPT_TRIANGLELIST primitive type, so an index buffer must be properly initialized.
An attribute table is used to identify areas of the mesh that need to be drawn with different textures, render states, materials, and so on. In addition, the application can use the attribute table to hide portions of a mesh by not drawing a given attribute
 identifier (AttribId) when drawing the frame. 
阅读下篇：[XFile网格的应用（3）](http://www.cppblog.com/lovedday/archive/2007/05/18/24351.html)
