# Direct3D学习（四）：高级着色语言初探 - 逍遥剑客 - CSDN博客
2007年05月18日 02:57:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3407
**传统的3D绘图编程方式**
（1）将顶点代入顶点缓冲区
（2）设定变换函数
（3）设定光源
（4）设定纹理和材质
（5）绘制多边形
用的比较多的明暗处理算法在图形学上我们就学过：朗伯算法和高洛德算法。而在3D加速卡中可以硬件实现的的也就是上面的经典算法。但是随着硬件的发展，1999的SigGraph大会上游戏程序员一致希望将算法直接写入3D显卡（正确地说是3D加速芯片中），会后便出现了GPU（Graphic Processing Unit）编程技术。
以前，像自然光渲染这样的算法，只能呆在纸上，因为运算速度的缓慢决定了它们只能用于离线处理（想想上学期的图形学给的光线追踪程序，差点把我机子跑死）。
![](http://images.blogcn.com//2007/5/17/5/xoyojank,20070517170624573.JPG)
现在有了GPU编程技术，每秒可以实现每秒几十帧的画面，使理论上算法得到了应用。
**Direct3D****的渲染管道**
![](http://images.blogcn.com//2007/5/17/5/xoyojank,20070517170957400.JPG)
如图是D3D的固定管道。
可编程的渲染管道跟上面的差不多，但我们可以控制的只有两部分：顶点着色器（Vertex Shader）和像素着色器（Pixel Shader），相当于上图中的Vertex Processing和Pixel Processing。这两个部分是渲染流程中最重要的两个部分，其他部分由硬件就可心完成。那么它们是用来干什么的呢？顶点着色器是用来处理顶点的，像素着色器是用来处理像素的（好像是句废话）
**HLSL**
HLSL 全称High Level Shading Language . 是MS推出Direct3D 9时的一个重要更新。从名字上可知，HLSL是一种高级语言(High Level)，那么必然有与之对应的Low Level Shading Language，这个低级的语言就是ASM的Shader。它是类似于汇编语言，难以编写和维护，而HLSL则跟我们熟悉的C/C++语言非常类似。大大降低了开发人员学习的成本。HLSL本身就是微软和nVidia联合开发的，nVidia的版本称为Cg，也就是C for Graphics。可想而知，它和C是有同样的血统的。
**Effect****框架**
如果能将VS和PS合起来使用，那就更方便了。D3D提供了一个管理它们的框架，就是Effect框架。
要使用Effect框架，首先要写一个.fx文件，里面就是HLSL;
//变换矩阵
float4x4 matWVP;
//纹理
texture tex;
//输入的顶点形式
struct VS_INPUT
{
float3 pos    :POSITION;
float4 diff   :COLOR0;
float2 tex    :TEXCOORD0;
};
//输出的顶点形式
struct VS_OUTPUT
{
float4 pos    :POSITION;
float4 diff   :COLOR0;
float2 tex    :TEXCOORD0;
};
//声明名为VS的顶点着色器
VS_OUTPUT VS(VS_INPUT In)
{
     VS_OUTPUT Out;
     Out.pos       = mul(float4(In.pos,1),matWVP);  //计算坐标的位置
     Out.diff= In.diff;                           //输入颜色变为输出颜色
Out.tex  = float2(In.tex.x, 1.0f - In.tex.y);//纹理的Y坐标颠倒
return Out;
}
//纹理编译状态
sampler Sampler = sampler_state
{
     Texture       = <tex>;
     MipFilter= LINEAR;
     MinFilter= LINEAR;
     MagFilter= LINEAR;
};
//声明名为PS的像素着色器
float4 PS(VS_OUTPUT In) : COLOR0
{
returntex2D(Sampler, In.tex) * In.diff;
}
//声明MyShader技巧
technique MyShader
{
pass P0//最初的号pass
     {
         Lighting = FALSE;
         Sampler[0] = (Sampler);
//shaders
         VertexShader = compilevs_1_1 VS();
         PixelShader  = compileps_1_1 PS();
     }
}
相信只要熟悉D3D基本编程的人都能看懂，语法跟C语言很像，是吧？
这个.fx的效果就是把纹理上下翻转。
要在D3D程序中使用效果，首先要进行顶点声明和效果的创建：
LPDIRECT3DVERTEXDECLARATION9 g_pDecl;
LPD3DXEFFECT                g_pEffect;
然后在初始化完程序的几何信息（顶点、纹理等）后，装载并设置效果：
//-----------------------------------------------------------------------------
//初始化顶点着色器
//-----------------------------------------------------------------------------
HRESULT InitVS()
{
     ::D3DVERTEXELEMENT9 decl[18];
//使用FVF自动代入顶点的声明值
     D3DXDeclaratorFromFVF(D3DFVF_CUSTOMVERTEX,decl);
//顶点声明值生成g_pDecl
     g_pd3dDevice->CreateVertexDeclaration(decl, &g_pDecl);
//读取FX文件生成Effect界面
     LPD3DXBUFFER pCompilationErrors;
if(FAILED(D3DXCreateEffectFromFile(g_pd3dDevice, "BasicHLSL.fx", NULL, NULL, 0,
         NULL, &g_pEffect, &pCompilationErrors)))
     {
         MessageBox(NULL,(LPCSTR)pCompilationErrors->GetBufferPointer(),NULL,NULL);
return E_FAIL;
     }
else
     {
//将纹理和矩阵值传给ID3DXEffect
         g_pEffect->SetTexture("tex", pTexture);
         matWVP = matWorld * matView * matProj;
         g_pEffect->SetMatrix("matWVP", &matWVP);
return S_OK;
     }
}
其中pCompilationErrors是.fx编译的错误信息，从这里我们可以找出.fx文件中的错误，便于修改。当然，VS.Net可以直接对.fx进行调试。g_pEffect->SetXXXX可以把当前程序中的变量值传递给效果框架，XXXX是参数的类型。如上面的例子中是纹理tex和矩阵matWVP。
设定完参数后，就可以进行渲染了：
//设置顶点声明值和顶点
         g_pd3dDevice->SetVertexDeclaration(g_pDecl);
        g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CustomVertex) );
//设定FX输出使用的方法
         g_pEffect->SetTechnique("MyShader");
         UINT nPass;
//开始使用FX的输出
         g_pEffect->Begin(&nPass, D3DXFX_DONOTSAVESTATE);
//使用号渲染通道,即P0
              g_pEffect->BeginPass(0);
              g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
              g_pEffect->EndPass();
//结束使用FX的输出
         g_pEffect->End();
程序结果如图：
![](http://images.blogcn.com//2007/5/17/5/xoyojank,20070517175000781.JPG)
