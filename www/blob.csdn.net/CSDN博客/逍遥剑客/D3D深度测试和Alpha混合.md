# D3D深度测试和Alpha混合 - 逍遥剑客 - CSDN博客
2008年01月22日 12:07:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4647
1.深度测试
a)深度缓冲区:屏幕上每个像素点的深度信息的一块内存缓冲区.D3D通过比较当前绘制的像素点的深度和对应深度缓冲区的点的深度值来决定是否绘制当前像素.
b)D3DPRESENT_PARAMETERS. AutoDepthStencilFormat = D3DFMT_D16
表示深度值由16位二进制表示
开启深度测试:pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
深度测试函数:D3DRS_ZFUNC:
|D3DCMP_NEVER|总是返回FALSE|
|----|----|
|D3DCMP_LESS (常用)|小于深度缓冲区的相应值时返回TRUE|
|D3DCMP_EQUAL|等于|
|D3DCMP_LESSEQUAL|小于等于|
|D3DCMP_GREATER|大于|
|D3DCMP_NOTEQUAL|不等于|
|D3DCMP_GREATEREQUAL|大于等于|
|D3DCMP_ALWAYS|总是返回TRUE|
更新缓冲区:保持深度缓冲区不变还是用当前像素的深度值更新
pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
2.Alpha混合
a)原理:Color = (RGBsrc * Ksrc) OP (RGBdst * Kdst) 
最常用的方法:Color = (RGBsrc * Alphasrc) + (RGBdst * (1 – Alphasrc)
b)应用
启用:pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
|**Blend mode factor**|**Description**|
|----|----|
|D3DBLEND_ZERO|(0, 0, 0, 0)|
|D3DBLEND_ONE|(1, 1, 1, 1)|
|D3DBLEND_SRCCOLOR|(Rs, Gs, Bs, As)|
|D3DBLEND_INVSRCCOLOR|(1-Rs, 1-Gs, 1-Bs, 1-As)|
|D3DBLEND_SRCALPHA|(As, As, As, As)|
|D3DBLEND_INVSRCALPHA|(1-As, 1-As, 1-As, 1-As)|
|D3DBLEND_DESTALPHA|(Ad, Ad, Ad, Ad)|
|D3DBLEND_INVDESTALPHA|(1-Ad, 1-Ad, 1-Ad, 1-Ad)|
|D3DBLEND_DESTCOLOR|(Rd, Gd, Bd, Ad)|
|D3DBLEND_INVDESTCOLOR|(1-Rd, 1-Gd, 1-Bd, 1-Ad)|
|D3DBLEND_SRCALPHASAT|(f, f, f, 1); f = min(As, 1-Ad)|
Alpha混合方法
|D3DBLENDOP_ADD|源计算结果与颜色缓冲区计算结果相加|
|----|----|
|D3DBLENDOP_SUBTRACT|源计算结果减去颜色缓冲区计算结果|
|D3DBLENDOP_REVSUBTRACT|颜色缓冲区计算结果减去源计算结果|
|D3DBLENDOP_MIN|MIN(源计算结果, 颜色缓冲区计算结果)|
|D3DBLENDOP_MAX|MAX(源计算结果, 颜色缓冲区计算结果)|
Color = (RGBsrc * Alphasrc) + (RGBdst * (1 – Alphasrc) )的设置示例:
pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
pDevice->SetRenderState( D3DBLENDOP, D3DBLENDOP_ADD );(默认值,可选)
3.Alpha测试
a)原理
根据Alpha测试条件来决定当前像素是否绘制,并不需要对颜色缓冲进行操作,所以速度比Alpha混合要快.
b)应用
启用:pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
Alpha测试函数
D3DCMP_FUNC枚举,默认为D3DCMP_ALWAYS
参考值:
pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000081);
