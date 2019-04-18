# D3d9c的固定渲染管道（fixed function pipeline）与可编程管道（programmable function pipeline）的异同点 - gauss的专栏 - CSDN博客
2011年12月04日 09:00:30[gauss](https://me.csdn.net/mathlmx)阅读数：487标签：[c																[C																[direct3d																[Direct3D																[float																[function																[FUNCTION																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=FUNCTION&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=Direct3D&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
|![](http://ctc.qzs.qq.com/ac/b.gif)![](http://ctc.qzs.qq.com/ac/b.gif)  现在的游戏图形部分越来多依靠GPU来渲染绘制。说起GPU先说着色器，着色器是可编程的（programmable），分为顶点着色器和像素着色器。着色器是一段运行在GPU上的程序，可以取代渲染的固定功能流水线中部分功能。 在D3d9c可以用固定管道渲染（fixed function pipeline），也可以用可编程的管道（programmable function pipeline）渲染。下面大概总结下各自对vertex的绘制流程。看看传统的渲染流程和利用可编程管道渲染的异同。  固定管道。  先来看看初始化部分，前5步基本是在构造函数中调用实现。1: 定义其结构Struct{}struct Vertex{    D3DXVECTOR3 pos;    D3DXVECTOR3 normal0;    D3DXVECTOR3 normal1;    D3DXVECTOR3 normal2;};2:创建一组D3DVERTEXELEMENT9[]结构的元素描述上面定义的Struct{}，以在后面告诉Directx我们定义什么样的顶点结构。· D3DVERTEXELEMENT9 decl[] =· {· {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},· {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},· {0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1},· {0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   2},· D3DDECL_END()· };3：创建了D3DVERTEXELEMENT9[]后，我们便可以获取指向类IDirect3DVertexDeclaration9的指针，其是用表示D3DVERTEXELEMENT9[]结构的。我们通过下面的函数来依据D3DVERTEXELEMENT9[]获取指向IDirect3DVertexDeclaration9的指针。HRESULT IDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)例如：  IDirect3DVertexDeclaration9* d3dVertexDecl = 0;HR(gd3dDevice->CreateVertexDeclaration(decl, &d3dVertexDecl));4:  接下来我们为我们的顶点和顶点序号申请一片内存的存储空间HRESULT IDirect3DDevice9::CreateVertexBuffer(    UINT Length,    DWORD Usage,    DWORD FVF,    D3DPOOL Pool    IDirectSDVertexBuffer9** ppVertexBuffer,    HANDLE* pSharedHandle);HRESULT IDirect3DDevice9::CreateIndexBuffer(    UINT Length,    DWORD Usage,    D3DFORMAT Format,    D3DPOOL Pool,    IDirectSDIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);例如IDirect3DVertexBuffer9* vb;gd3dDevice->CreateVertexBuffer( 8 * sizeof( Vertex ), 0, 0,D3DPOOL_MANAGED, &vb, 0);IDirect3DIndexBuffer9* ib;gd3dDevice->CreateIndexBuffer(36 * sizeof( WORD ),D3DUSASE_DYNAMIC | D3DUSASE_WRITEONLY, D3DFMTJNDEX16,D3DPOOL_DEFAULT, &ib, 0);5  再接着获取指向上面申请到的内存（空白）的指针，以对这片区域进行写操作。HRESULT IDirect3DVertexBuffer9::Lock(   UINT OffsetToLock,   UINT SizeToLock,   BYTE** ppbData,   DWORD Flags);HRESULT IDirect3DIndexBuffer9::Lock(   UINT OffsetToLock,   UINT SizeToLock,   BYTE** ppbData,   DWORD Flags);例如Vertex* vertices;vb->Lock(0, 0, (void**)&vertices, 0);    // lock the entire bufferVertices[0] = Vertex(-1.0f, 0.0f, 2.0f); // write vertices to vertices[1] = Vertex( 0.0f, 1.0f, 2.0f); // the buffervertices[2] = Vertex( 1.0f, 0.0f, 2.0f);vb->Unlock(); // unlock when you're done accessing the buffer     下面几步则是每次系统更新的时候在DrawScene()中调用。6：想调用Direct3D的draw** 函数了？快了。在之前，我们必须告诉Direct3D去哪里找到顶点和顶点序号的数据。Direct3D的draw**系列函数是从顶点流(vertex stream:a vertex stream is essentially an array of vertex component data)中获得顶点数据的。记得我们将我们的顶点放在vertex buffer中，所以为了能够画vertex buffer中的顶点，我们必须将vertex buffer与某个vertex stream联系上。这可以通过下面的函数实现HRESULT IDirect3DDevice9::SetStreamSource(  UINT StreamNumber,  IDirect3DVertexBuffer9* pStreamData,  UINT OffsetInBytes,UINT Stride);例如，假定vb是指向一个已经用顶点类型为Vertex填好了的vertex buffer。那么我们可以通过下面的句子来将这段buffer与vertex stream联系起来。HR(gd3dDevice->SetStreamSource(0, vb, 0, sizeof(Vertex)));7：记得上面我们已经创建了顶点的Declaration来描述我们定义的顶点结构。那么最后我们要做的就是在Direct3D中给这个顶点类型注册，正式告之Direct3D如何处理我们定义的顶点。例如// d3dVertexDecl is a pointer to an IDirect3DVertexDeclaration9 typegd3dDevice->SetVertexDeclaration(d3dVertexDecl);8：最后我们就可以调用Draw**（）来绘制各种图案了。当遇到不同物体需要不同的顶点时，我们就需要重新调用SetStreamSource（）,SetVertexDelaration()等来告之Direct3D。例如下面：可编程管道  可编程管道绘制顶点和固定管道大体上差不多。只是有三个主要的不同点。1:编写HLSL程序，自己制定渲染形式 。2：初始化时创建个与GPU交互的C++接口ID3DXEffect *，并利用ID3DXEffect来对GPU做一些初始化工作，比如建立HLSL中变量与C++中相关变量的联系。3：在最后一步drawScene()中通过ID3DXEffect *来调用HLSL的mhTech来进行绘制。1：HLSL语言？好大的题目，具体自己找本教科书了。下面直接贴代码举个简单例子。uniform extern float4x4 gWVP;struct OutputVS{    float4 posH : POSITION0;};OutputVS TransformVS(float3 posL : POSITION0){    // Zero out our output.OutputVS outVS = (OutputVS)0;// Transform to homogeneous clip space.outVS.posH = mul(float4(posL, 1.0f), gWVP);// Done--return the output.    return outVS;}float4 TransformPS() : COLOR{    return float4(0.0f, 0.0f, 0.0f, 1.0f);}technique TransformTech{    pass P0    {        // Specify the vertex and pixel shader associated with this pass.        vertexShader = compile vs_3_0 TransformVS();        pixelShader  = compile ps_3_0 TransformPS();// Specify the render/device states associated with this pass.FillMode = Wireframe;    }}2：在d3d9的实例的构造函数或者初始化的时候，我们可以通过调用D3DXCreateEffectFromFile()函数来创建与GPU联系的C++指针D3DXEffect *。HRESULT  D3DXCreateEffectFromFile(    LPDIRECT3DDEVICE9 pDevice,    LPCSTR pSrcFile,    CONST D3DXMACRO* pDefines,    LPD3DXINCLUDE pInclude,    DWORD Flags,    LPD3DXEFFECTPOOL pPool,    LPD3DXEFFECT* ppEffect,    LPD3DXBUFFER *ppCompilationErrors);    我们可以通过ID3DXEffect *来建立C++程序与HLSL程序的沟通桥梁。而这个桥梁就是变量D3DXHANDLE。   然后我们可以通过以下函数为HLSL每个变量在C++层面创建一个对应变量，即对一个HLSL变量X'建立一个在C++中的对应变量D3DXHANDLE mX。D3DXHANDLE  ID3DXEffect::GetParameterByName(     D3DXHANDLE hParent, // scope of variable - parent structure     LPCSTR pName        // name of variable );   然后通过下面这个系列的函数来设置HLSL中对应hParameter的变量的值。    HRESULT ID3DXEffect::SetValue(     D3DXHANDLE hParameter,     LPCVOID pData,     UINT Bytes);    ……    ……  HLSL与C++沟通的实例：// 1 ：C++中的变量D3DXMATRIX M;D3DXMatrixRotationY(&M, D3DX_PI);D3DXVECTOR4 V(x, y, z, 1.0f);// 2：为HLSL中的变量创建对应的在C++中的D3DXHANDLE系列的变量。 (assume "mtx" and "vec" are parameters // declared in .fx file).D3DXHANDLE hMatrix = mFX->GetParameterByName(0, "mtx");D3DXHANDLE hVec = mFX->GetParameterByName(0, "vec");// 3：通过D3DXHANDLE把C++中的值赋给HLSL。mFX->SetMatrix(hMatrix, &M);mFX->SetVector(hVec, &V);    全部流程调用实例void MeshDemo::buildFX(){// Create the FX from a .fx file.ID3DXBuffer* errors = 0;HR(D3DXCreateEffectFromFile(gd3dDevice, "transform.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));if( errors )MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);// Obtain handles.mhTech = mFX->GetTechniqueByName("TransformTech");mhWVP  = mFX->GetParameterByName(0, "gWVP");}    最后来比较下在固定管道下与在GPU可编程下的drawScene()函数（自己定义的在周期内调用所有绘图渲染相关函数）的不一样之处。注意黑色加粗的代码。    首先是固定的渲染管道：void CubeDemo::drawScene(){// Clear the backbuffer and depth buffer.HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));HR(gd3dDevice->BeginScene());// Let Direct3D know the vertex buffer, index buffer and vertex // declaration we are using.HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPos)));HR(gd3dDevice->SetIndices(mIB));HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));// World matrix is identity.D3DXMATRIX W;D3DXMatrixIdentity(&W);**HR(gd3dDevice->SetTransform(D3DTS_WORLD, &W));****HR(gd3dDevice->SetTransform(D3DTS_VIEW, &mView));****HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &mProj));****HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));**HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));mGfxStats->display();HR(gd3dDevice->EndScene());// Present the backbuffer.HR(gd3dDevice->Present(0, 0, 0, 0));}再来是可编程管道void MeshDemo::drawScene(){// Clear the backbuffer and depth buffer.HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));HR(gd3dDevice->BeginScene());// Let Direct3D know the vertex buffer, index buffer and vertex // declaration we are using.HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPos)));HR(gd3dDevice->SetIndices(mIB));HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));// Setup the rendering FX**HR(mFX->SetTechnique(mhTech));**// Begin passes.UINT numPasses = 0;HR(mFX->Begin(&numPasses, 0));for(UINT i = 0; i < numPasses; ++i){**HR(mFX->BeginPass(i));****HR(mFX->SetMatrix(mhWVP, &(mView*mProj)));****HR(mFX->CommitChanges());**HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mNumGridVertices, 0, mNumGridTriangles));**HR(mFX->EndPass());**}**HR(mFX->End());**mGfxStats->display();HR(gd3dDevice->EndScene());// Present the backbuffer.HR(gd3dDevice->Present(0, 0, 0, 0));}转自http://blog.csdn.net/qiul12345/article/details/5937701![](http://ctc.qzs.qq.com/ac/b.gif)|// Object 1 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vbl, 0, sizeof( Vertexl ) );gd3dDevice->SetIndices(indexBufferl);gdSdDevice->SetVertexDeclaration(decl1);// DRAW OBJECT 1 HEREgd3dDevice->DrawPrimitive(...);// Object 2 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vb2, 0, sizeof( Vertex2 ) );gd3dDevice->SetIndices(indexBuffer2);gdSdDevice->SetVertexDeclaration(decl2);// DRAW OBJECT 2 HEREgd3dDevice->DrawPrimitive(...);// Object 3 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vb3, 0, sizeof( Vertex3 ) );gd3dDevice->SetIndices(indexBuffer3);gdSdDevice->SetVertexDeclaration(decl3);// DRAW OBJECT 3 HEREgd3dDevice->DrawPrimitive(...);||
|----|----|----|
|// Object 1 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vbl, 0, sizeof( Vertexl ) );gd3dDevice->SetIndices(indexBufferl);gdSdDevice->SetVertexDeclaration(decl1);// DRAW OBJECT 1 HEREgd3dDevice->DrawPrimitive(...);// Object 2 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vb2, 0, sizeof( Vertex2 ) );gd3dDevice->SetIndices(indexBuffer2);gdSdDevice->SetVertexDeclaration(decl2);// DRAW OBJECT 2 HEREgd3dDevice->DrawPrimitive(...);// Object 3 uses its own vertex buffer, index buffer, and// vertex declarationgd3dDevice->SetStreamSource( 0, vb3, 0, sizeof( Vertex3 ) );gd3dDevice->SetIndices(indexBuffer3);gdSdDevice->SetVertexDeclaration(decl3);// DRAW OBJECT 3 HEREgd3dDevice->DrawPrimitive(...);| | |
|| | |
可编程管道
  可编程管道绘制顶点和固定管道大体上差不多。只是有三个主要的不同点。1:编写HLSL程序，自己制定渲染形式 。2：初始化时创建个与GPU交互的C++接口ID3DXEffect *，并利用ID3DXEffect来对GPU做一些初始化工作，比如建立HLSL中变量与C++中相关变量的联系。3：在最后一步drawScene()中通过ID3DXEffect *来调用HLSL的mhTech来进行绘制。
1：HLSL语言？好大的题目，具体自己找本教科书了。
下面直接贴代码举个简单例子。
uniform extern float4x4 gWVP;
struct OutputVS
{
    float4 posH : POSITION0;
};
OutputVS TransformVS(float3 posL : POSITION0)
{
    // Zero out our output.
OutputVS outVS = (OutputVS)0;
// Transform to homogeneous clip space.
outVS.posH = mul(float4(posL, 1.0f), gWVP);
// Done--return the output.
    return outVS;
}
float4 TransformPS() : COLOR
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
technique TransformTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();
// Specify the render/device states associated with this pass.
FillMode = Wireframe;
    }
}
2：在d3d9的实例的构造函数或者初始化的时候，我们可以通过调用D3DXCreateEffectFromFile()函数来创建与GPU联系的C++指针D3DXEffect *。
HRESULT  D3DXCreateEffectFromFile(
    LPDIRECT3DDEVICE9 pDevice,
    LPCSTR pSrcFile,
    CONST D3DXMACRO* pDefines,
    LPD3DXINCLUDE pInclude,
    DWORD Flags,
    LPD3DXEFFECTPOOL pPool,
    LPD3DXEFFECT* ppEffect,
    LPD3DXBUFFER *ppCompilationErrors
);
    我们可以通过ID3DXEffect *来建立C++程序与HLSL程序的沟通桥梁。而这个桥梁就是变量D3DXHANDLE。
   然后我们可以通过以下函数为HLSL每个变量在C++层面创建一个对应变量，即对一个HLSL变量X'建立一个在C++中的对应变量D3DXHANDLE mX。
D3DXHANDLE  ID3DXEffect::GetParameterByName(     D3DXHANDLE hParent, // scope of variable - parent structure     LPCSTR pName        // name of variable );
   然后通过下面这个系列的函数来设置HLSL中对应hParameter的变量的值。
    HRESULT ID3DXEffect::SetValue(     D3DXHANDLE hParameter,     LPCVOID pData,     UINT Bytes);
    ……
    ……
  HLSL与C++沟通的实例：
// 1 ：C++中的变量
D3DXMATRIX M;
D3DXMatrixRotationY(&M, D3DX_PI);
D3DXVECTOR4 V(x, y, z, 1.0f);
// 2：为HLSL中的变量创建对应的在C++中的D3DXHANDLE系列的变量。 (assume "mtx" and "vec" are parameters
 // declared in .fx file).
D3DXHANDLE hMatrix = mFX->GetParameterByName(0, "mtx");
D3DXHANDLE hVec = mFX->GetParameterByName(0, "vec");
// 3：通过D3DXHANDLE把C++中的值赋给HLSL。
mFX->SetMatrix(hMatrix, &M);
mFX->SetVector(hVec, &V);
    全部流程调用实例
void MeshDemo::buildFX()
{
// Create the FX from a .fx file.
ID3DXBuffer* errors = 0;
HR(D3DXCreateEffectFromFile(gd3dDevice, "transform.fx", 
0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
if( errors )
MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
// Obtain handles.
mhTech = mFX->GetTechniqueByName("TransformTech");
mhWVP  = mFX->GetParameterByName(0, "gWVP");
}
    最后来比较下在固定管道下与在GPU可编程下的drawScene()函数（自己定义的在周期内调用所有绘图渲染相关函数）的不一样之处。注意黑色加粗的代码。
    首先是固定的渲染管道：
void CubeDemo::drawScene()
{
// Clear the backbuffer and depth buffer.
HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
HR(gd3dDevice->BeginScene());
// Let Direct3D know the vertex buffer, index buffer and vertex 
// declaration we are using.
HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPos)));
HR(gd3dDevice->SetIndices(mIB));
HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));
// World matrix is identity.
D3DXMATRIX W;
D3DXMatrixIdentity(&W);
**HR(gd3dDevice->SetTransform(D3DTS_WORLD, &W));**
**HR(gd3dDevice->SetTransform(D3DTS_VIEW, &mView));**
**HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &mProj));**
**HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));**
HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));
mGfxStats->display();
HR(gd3dDevice->EndScene());
// Present the backbuffer.
HR(gd3dDevice->Present(0, 0, 0, 0));
}
再来是可编程管道
void MeshDemo::drawScene()
{
// Clear the backbuffer and depth buffer.
HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
HR(gd3dDevice->BeginScene());
// Let Direct3D know the vertex buffer, index buffer and vertex 
// declaration we are using.
HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexPos)));
HR(gd3dDevice->SetIndices(mIB));
HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));
// Setup the rendering FX
**HR(mFX->SetTechnique(mhTech));**
// Begin passes.
UINT numPasses = 0;
HR(mFX->Begin(&numPasses, 0));
for(UINT i = 0; i < numPasses; ++i)
{
**HR(mFX->BeginPass(i));**
**HR(mFX->SetMatrix(mhWVP, &(mView*mProj)));**
**HR(mFX->CommitChanges());**
HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mNumGridVertices, 0, mNumGridTriangles));
**HR(mFX->EndPass());**
}
**HR(mFX->End());**
mGfxStats->display();
HR(gd3dDevice->EndScene());
// Present the backbuffer.
HR(gd3dDevice->Present(0, 0, 0, 0));
}
转自http://blog.csdn.net/qiul12345/article/details/5937701
![](http://ctc.qzs.qq.com/ac/b.gif)
