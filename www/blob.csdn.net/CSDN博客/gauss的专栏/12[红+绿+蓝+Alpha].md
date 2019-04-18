# 12[红+绿+蓝+Alpha] - gauss的专栏 - CSDN博客
2011年12月13日 22:43:23[gauss](https://me.csdn.net/mathlmx)阅读数：1536
第十二集 红+绿+蓝+Alpha
红绿蓝组成了绚烂的颜色世界, 当红绿蓝遇到Alpha后, 这个颜色世界又多了一份神秘.
神秘的Alpha, 我们可以简单的认为它和红, 绿, 蓝一样, 是组成色彩的一种属性， 用来表示透明度.
Directx Graphics中的色彩一般使用"RGB"格式表示, RGB格式有三种原色 --- red(红), green(绿), blue(蓝), 其他颜色都是通过三种原色的不同比例混合来表示.
RGB格式现在最常见存储方式是每种原色一个字节 --- 决定了现在的原色表示范围只有[0, 255], RGB共三个字节(24位); 如再加上一个字节的Alpha值, 有四个字节(32位);
**12.1 色彩**
12.1.1 D3DCOLOR
    32位的RGB格式刚好是一个DWORD --- D3DCOLOR, 通常的顺序如图12.1,
                   |<------------------ 32 bits ------------------>|
                   +-----------+-----------+-----------+-----------+
                   |   alpha   |   red     |   green   |   blue    |
                   +-----------+-----------+-----------+-----------+
                  high bit                                     low bit
图12.1
DirectX Graphics中为我们提供生成这个DWORD的Macro,
#define D3DCOLOR_ARGB(a,r,g,b) /
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)
#define D3DCOLOR_XYUV(y,u,v)   D3DCOLOR_ARGB(0xff,y,u,v)
#define D3DCOLOR_AYUV(a,y,u,v) D3DCOLOR_ARGB(a,y,u,v)
还有一个特殊的单位化表示的D3DCOLOR_COLORVALUE, DirectX Graphics把颜色基本运算合并在一个结构D3DXCOLOR中, 简单实用.
12.1.2 Gamma校正
    Gamma校正是全屏的, 现在的显卡都支持， 一般Gamma校正的用途是调整屏幕显示的明暗度, 同样, Gamma校正可以实现一些特殊效果, 如第一视觉的RPG中, 男主人公被敌人偷袭, 眼前景物渐渐变成血红色…, 男主人公从昏睡中醒过来, 眼前从黑黑一片到看到美丽的女主人公, 精彩的故事由此开始….
    DirectX Graphics中, 色彩到屏幕输出通过Gamma梯度映射校正, 如图12.2
![](https://p-blog.csdn.net/images/p_blog_csdn_net/jaredz/193444/o_B02.JPG)
图12.2
图中梯度映射校正最大为65535, 表示梯度使用WORD表示. 每种原色都有各自的梯度映射校正, 默认的三原色梯度映射校正都是相同的梯度斜率1, 例如对于RGB(0, 128, 255)进行校正, 如图12.3
![](https://p-blog.csdn.net/images/p_blog_csdn_net/jaredz/193444/o_B03.JPG)
图12.3
0   [梯度映射校正]--> 0     [梯度到原色] --> 0   [最后输出颜色的RED分量]
128 [梯度映射校正]--> 32896 [梯度到原色] --> 128 [最后输出颜色的GREEN分量]
255 [梯度映射校正]--> 65535 [梯度到原色] --> 255 [最后输出颜色的BLUE分量]
现在我们改变GREEN分量的梯度, 使其斜率为2, 如图12.4, 则有
![](https://p-blog.csdn.net/images/p_blog_csdn_net/jaredz/193444/o_B04.JPG)
图12.4
0   [梯度映射校正]--> 0     [梯度到原色] --> 0   [最后输出颜色的RED分量]
128 [梯度映射校正]--> 65535 [梯度到原色] --> 255 [最后输出颜色的GREEN分量]
255 [梯度映射校正]--> 65535 [梯度到原色] --> 255 [最后输出颜色的BLUE分量]
使用这种Gamma校正, 屏幕会明显偏向于绿色.
DirectX Graphics中, RED, GREEN, BLUE的梯度通过D3DGAMMARAMP结构定义,
/* Gamma Ramp: Same as DX7 */
typedef struct _D3DGAMMARAMP
{
    WORD                red  [256];
    WORD                green[256];
    WORD                blue [256];
} D3DGAMMARAMP;
然后通过IDirect3DDevice9的函数SetGammaRamp设置,
void SetGammaRamp(UINT  iSwapChain,
                  DWORD Flags,
                  CONST D3DGAMMARAMP * pRamp);
void GetGammaRamp(UINT  iSwapChain,
                  D3DGAMMARAMP * pRamp);
这里注意有些显卡使用WORD中的高位字节, 有些使用低位字节, 所以最好两个字节设置相同的数值, 如0xCOCO.
**12.2 Alpha Test**
    Alpha字节的用处之一, 就是根据Alpha的数值和一特定数值的逻辑比较运算的结果决定当前位的颜色是否显示.
12.2.1 逻辑比较运算类型
    逻辑运算类型在枚举结构中定义,
typedef enum _D3DCMPFUNC {
    D3DCMP_NEVER                = 1,
    D3DCMP_LESS                 = 2,
    D3DCMP_EQUAL                = 3,
    D3DCMP_LESSEQUAL            = 4,
    D3DCMP_GREATER              = 5,
    D3DCMP_NOTEQUAL             = 6,
    D3DCMP_GREATEREQUAL         = 7,
    D3DCMP_ALWAYS               = 8,
    D3DCMP_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
} D3DCMPFUNC;
a.
D3DCMP_NEVER : 所有比较都返回失败
b.
D3DCMP_LESS  : 当前Alpha值小于特定值时返回成功
c.
D3DCMP_EQUAL : 当前Alpha值等于特定值时返回成功
d.
D3DCMP_LESSEQUAL : 当前Alpha值小于等于特定值时返回成功
e.
D3DCMP_GREATER : 当前Alpha值大于特定值时返回成功
f.
D3DCMP_NOTEQUAL : 当前Alpha值不等于特定值时返回成功
g.
D3DCMP_GREATEREQUAL : 当前Alpha值大于等于特定值时返回成功
h.
D3DCMP_ALWAYS : 所有比较都返回成功
12.2.2 Alpha-Test的过程
1.
打开Alpha-Test标志(一般只是在渲染的时候打开, 渲染结束后关闭)
   IDirect3DDevice9::SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
2.
设置逻辑比较的类型(默认类型为D3DCMP_ALWAYS)
   IDirect3DDevice9::SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
3.
设置比较用的特定数值(默认为0)
   IDirect3DDevice9::SetRenderStateD3DRS_ALPHAREF, 0);
**12.3 Alpha Test例子**
12.3.1 代码更新
我们来看看game12的主要更新的代码(下载game12 project),
---------------------------------------------------------------
// d9graphics.cpp中, 根据鼠标选取的比较运算和特殊比较值更新
VOID CD9Graphics::UpdateAlpha()
{
   m_pDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)g_nValue);
   m_pDev->SetRenderState(D3DRS_ALPHAFUNC, g_aTest[g_nTest]);
}
// d9graphics.cpp中, 根据鼠标选取的不同Gamma校正更新屏幕明暗度
VOID CD9Graphics::UpdateGamma(INT nIndex)
{
   if (g_gamma == nIndex)
   {
        return;
   }
   g_gamma = nIndex;
   INT nGamma = g_aGamma[nIndex];
   D3DGAMMARAMP map = { 0 };
   for (INT i = 0; i < 256; i++)
   {
        map.red[i]   = g_ramp.red[i] * nGamma;
        map.green[i] = g_ramp.green[i] * nGamma;
        map.blue[i]  = g_ramp.blue[i] * nGamma;
   }
   m_pDev->SetGammaRamp(0, D3DSGR_CALIBRATE, &map);
}
VOID CD9Graphics::Render()
{
// reset device if last render detect the device lost
   HRESULT h = 0L;
   if (g_bLastDev)
   {
        h = m_pDev->TestCooperativeLevel();
        if (h == D3DERR_DEVICENOTRESET)
        {
            TermObject();
            m_pDev->Reset(&g_d3dpm);
            InitObject();
            g_bLastDev = FALSE;
        }
        else
        {
            return;
        }
   }
m_pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if (FAILED(m_pDev->BeginScene()))
   {
        return;
   }
   m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
   m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(MYVERTEXTEX));
   m_pDev->SetFVF(D3DFVF_MYVERTEXTEX);
   m_pDev->SetTexture(0, m_pTexture);
   m_pDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0,  2);
   m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pDev->EndScene();
    h = m_pDev->Present(NULL, NULL, NULL, NULL);
   if (FAILED(h))
   {
        if (h == D3DERR_DEVICELOST)
        {
            g_bLastDev = TRUE;
        }
        else if (h == D3DERR_DRIVERINTERNALERROR)
        {
            DestroyWindow(g_hWnd);
        }
   }
}
---------------------------------------------------------------
12.3.2 例子说明
例子中使用的天蓝色256 * 256的图片每位都有特定的Alpha值, 当使用不同的比较运算同不同的比较数值进行比较后, 得到的结果影响显示的效果. Alpha-Test的运算效率优于Alpha-Blend, 所以如果能用Alpha-Test代替Alpha-Blend得到相同的效果, 建议使用Alpha-Test.
![](https://p-blog.csdn.net/images/p_blog_csdn_net/Jaredz/193444/o_B05.JPG)
**12.4 Alpha Blend**
Alpha Blend中也使用到了Alpha字节, 但Alpha字节并不是起决定性作用的.  Alpha Blend的作用是将要写入帧缓冲器的颜色(源颜色)和帧缓冲器中原有颜色(目标颜色)按比例系数进行混合, 混合时分成三原色分量进行, 颜色混合公式,
FinalColor = SrcColor * SrcBlendFactor + DestColor * DestBlendFactor
12.4.1 混合比例系数
SrcBlendFactor和DestBlendFactor取值定义在D3DBLEND中, DirectX Graphics SDK中有详细的说明
12.4.2 Alpha Blend的过程
1.
打开Alpha Blend标志(一般只是在渲染的时候打开, 渲染结束后关闭)
   IDirect3DDevice9::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
2.
设置源和目标比例系数
(默认源比例系数为D3DBLEND_ONE, 目标比例系数为D3DBLEND_ZERO)
   IDirect3DDevice9::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE)
IDirect3DDevice9::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO)
**12.5 Alpha Blend例子**
12.5.1 代码更新
我们来看看game13的主要更新的代码(下载game13 project),
---------------------------------------------------------------
// d9graphics.cpp中, 根据鼠标选取的源和目标比例系数更新
VOID CD9Graphics::UpdateAlpha()
{
   m_pDev->SetRenderState(D3DRS_SRCBLEND, g_aBlend[g_nSrc]);
   m_pDev->SetRenderState(D3DRS_DESTBLEND, g_aBlend[g_nDst]);
}
VOID CD9Graphics::Render()
{
//...
m_pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if (FAILED(m_pDev->BeginScene()))
   {
        return;
   }
   m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(MYVERTEXTEX));
   m_pDev->SetFVF(D3DFVF_MYVERTEXTEX);
   m_pDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0,  2);
   m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pDev->EndScene();
    h = m_pDev->Present(NULL, NULL, NULL, NULL);
//...
}
---------------------------------------------------------------
12.5.2 例子说明
例子中使用RGB(0, 0, 128)的帧缓冲器背景色, 物体颜色为RGB(128, 0, 0), 根据不同的源和目标比例系数, 混合的颜色不同. 例子中没有使用光照, 加上光照Alpha Blend效果相同, 只是源颜色加上了光源提供的颜色.
a.
源比例系数 : D3DBLEND_ONE; 目标比例系数 : D3DBLEND_ZERO
RGB(128, 0, 0) = RGB(128, 0, 0, X) * (1, 1, 1, 1) +
                 RGB(0, 0, 128, X) * (0, 0, 0, 0)
b.
源比例系数 : D3DBLEND_SRCCOLOR; 目标比例系数 : D3DBLEND_ZERO
这时的源比例为单位化的物体颜色 == (0.5, 0, 0, X)
RGB(64, 0, 0) = RGB(128, 0, 0, X) * (0.5, 0, 0, X) +
                 RGB(0, 0, 128) * (0, 0, 0, 0)
c.
源比例系数 : D3DBLEND_SRCCOLOR; 目标比例系数 : D3DBLEND_DESTCOLOR
源比例为单位化的物体颜色 == (0.5, 0, 0, X)
目标比例为单位化的帧缓冲器背景色 == (0, 0, 0.5, X)
RGB(64, 0, 64) = RGB(128, 0, 0, X) * (0.5, 0, 0, X) +
                 RGB(0, 0, 128, X) * (0, 0, 0.5, X)
其他类型的Alpha Blend同样可以计算出混合的颜色.
Alpha Blend在多重纹理中是非常重要的一项功能.
![](https://p-blog.csdn.net/images/p_blog_csdn_net/Jaredz/193444/o_B06.JPG)
第十二集 小结
颜色运算是DirectX Graphics中很多高级应用的基础.
