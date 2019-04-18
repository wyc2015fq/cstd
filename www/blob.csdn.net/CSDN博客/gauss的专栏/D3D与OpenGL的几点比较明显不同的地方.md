# D3D与OpenGL的几点比较明显不同的地方 - gauss的专栏 - CSDN博客
2011年12月04日 09:05:27[gauss](https://me.csdn.net/mathlmx)阅读数：505标签：[测试																[blend																[float																[up																[api																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=up&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=blend&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
(转载http://hi.baidu.com/419836321/blog/item/cf653c98811a19186e068c0a.html)
D3D与OpenGL的几点比较明显不同的地方:
(一)、正交投影时：OpenGL以屏幕左上角为(0,0), 而D3D却以屏幕中心为(0,0)
(二)、OpenGL使用右手坐标系， 而D3D使用左手坐标系
(三)、OpenGL使用旋转操作等转入的角度参数是 角度, 而D3D是 弧度，所以注意要PI * Angle / 180
下面我把具体地API对照关系列出来(不是很全，以后添加中.......)
1. 坐标变换
pos = D3DXVECTOR3(0,2,-1.5);
at = D3DXVECTOR3(0,0,0);
up = D3DXVECTOR3(0,1,0);
D3DXMatrixLookAtLH(&view,&pos,&at,&up);
pd3dDevice->SetTransform(D3DTS_VIEW,&view);
2. 绘制
pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
     DrawPrimitive()
DrawIndexedPrimitive()
DrawPrimitiveUP()
DrawIndexedPrimitiveUP()
3. 颜色
4. 片段测试
(1) 深度测试
g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);              //glEnable(GL_DEPTH_TEST);    
g_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);      //glDepthFunc(GL_LEQUAL);
//--------------------------------------------------------------------------------------------------------
g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);        //glEnable(GL_CULL_FACE);
(2) Alpha测试
//--------------------------------------------------------------------------------------------------------
g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);                 //glEnable(GL_ALPHA_TEST);
g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);      //glAlphaFunc(GL_GREATER, 0.1f);
g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0.1 * 255); //取值范围 0 ~ 255
(3) 剪裁测试 (平面剪切)
//--------------------------------------------------------------------------------------------------------
// Enable clip plane for reflection map
CMatrix44f pWorldViewProjIT=m_pWorldViewProj; 
//pWorldViewProjIT.Transpose();
pWorldViewProjIT.Invert();     
// Transform plane to clip-space
float pClipSpacePlane[4];
float pClipPlane[]= { 0, 0, 1, 0};     
// Check if camera is below water surface, if so invert clip plane
CVector3f pEye=(CVector3f)m_pCamera.GetPosition();
if(-pEye.m_fZ<0.0)
{
    pClipPlane[2]=-pClipPlane[2];
}
MatrixTransformPlane(pClipSpacePlane, pClipPlane, pWorldViewProjIT);
// enable clip plane now
g_pDevice->SetClipPlane(0, pClipSpacePlane);    
g_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1);
(4) 模板测试
//--------------------------------------------------------------------------------------------------------
g_pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
     g_pDevice->SetRenderState(D3DRS_STENCILFUNC, 3DCMP_ALWAYS);
     g_pDevice->SetRenderState(D3DRS_STENCILREF, 0x1); //取值范围 0 ~ 255
     Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
5. 纹理操作
g_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
g_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
g_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
g_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    g_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
6. 缓冲区操作
(1) 颜色缓冲
//--------------------------------------------------------------------------------------------------------
g_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
g_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x000000F);
(2) 深度缓冲
//--------------------------------------------------------------------------------------------------------
g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);              //glEnable(GL_DEPTH_TEST);
g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);           //glDepthMask(GL_TRUE);
(3) 模板缓冲
//--------------------------------------------------------------------------------------------------------
(4) 渲染到纹理
//--------------------------------------------------------------------------------------------------------
// Render targets
IDirect3DSurface9 *m_plD3DBackbufferSurf,
    *m_plD3DDepthStencilSurfAA, 
    *m_plD3DDepthStencilSurf;
CRenderTarget *m_pRTRefraction, *m_pRTReflection; //(自定义纹理类)
//-----------------------------------------------------------------------------------
// Get backbuffer
g_pDevice->GetRenderTarget(0, &m_plD3DBackbufferSurf);
// Get depthstencil 
g_pDevice->GetDepthStencilSurface(&m_plD3DDepthStencilSurfAA);
// Restore previous states
g_pDevice->SetRenderTarget(0, m_plD3DBackbufferSurf);
g_pDevice->SetDepthStencilSurface(m_plD3DDepthStencilSurfAA);
// (1)折射图--------------------------------------------------------------------------
//下面的语句调用了 g_pDevice->CreateRenderTarget(iWidth, iHeight, (D3DFORMAT) iFormat, (D3DMULTISAMPLE_TYPE)iAASamples, 0, 0, &m_plD3Surf, 0));
if(FAILED(m_pRTRefraction->Create(m_fWidth>>1, m_fHeight>>1, D3DFMT_A8R8G8B8)))
{
    return APP_ERR_INITFAIL;
}
// Create depthstencil withouth multisampling
g_pDevice->CreateDepthStencilSurface(m_fWidth, m_fHeight, D3DFMT_D24X8, (D3DMULTISAMPLE_TYPE)0, 0, 0, &m_plD3DDepthStencilSurf, 0);
g_pDevice->SetRenderTarget(0, m_pRTReflection->GetSurface());
g_pDevice->StretchRect(m_plD3DBackbufferSurf, 0, m_pRTRefraction->GetSurface(), 0, D3DTEXF_NONE);
// (2)反射图-----------------------------------------------------------------------------------
m_pRTReflection=new CRenderTarget;
if(FAILED(m_pRTReflection->Create(m_fWidth>>2, m_fHeight>>2, D3DFMT_A8R8G8B8))) 
{
    return APP_ERR_INITFAIL;
}
g_pDevice->SetRenderTarget(0, m_pRTReflection->GetSurface());
//-----------------------------------------------------------------------------------
g_pDevice->SetRenderTarget(0, m_pRTReflection->GetSurface());
g_pDevice->SetDepthStencilSurface(m_plD3DDepthStencilSurf);
g_pDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 128), 1.0f, 0);    
SetViewport(m_pRTReflection->GetWidth(), m_pRTReflection->GetHeight());
//-----------------------------------------------------------------------------------
D3DXSaveTextureToFile("imageTex.jpg",D3DXIFF_JPG,(IDirect3DTexture9*)m_pWavesBump->GetTexture(),NULL);
7. 混合操作
g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);      //glDisable(GL_BLEND);
g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
8. 灯光与材质
g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //glDisable(GL_LIGHTING);
D3DMATERIAL9 mtrl;
mtrl.Ambient = a;
mtrl.Diffuse = d;
mtrl.Specular = s;
mtrl.Emissive = e;
mtrl.Power     = p;
Device->SetMaterial(&mtrl); //在设置纹理前设定
//设置当前使用的纹理

