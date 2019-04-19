# Direct3D纹理映射 - 逍遥剑客 - CSDN博客
2008年01月03日 23:11:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4270
**创建纹理对象**
1: HRESULT D3DXCreateTextureFromFile**(**2:   LPDIRECT3DDEVICE9 pDevice**,//D3D****设备对象接口**3:   LPCTSTR pSrcFile**,//****图片路径**4:   LPDIRECT3DTEXTURE9 * ppTexture**//****返回生成的D3D纹理对象指针**5: **);**
**纹理过滤方式**
·Nearest-**point** sampling(最近点采样)
·**Linear** texture filtering(线性纹理过滤)
·**Anisotropic** texture filtering(各向异性纹理过滤)
·Texture filtering with **mipmaps**(多级渐进纹理过滤)
|D3DTEXF_POINT|取距离最近的像素的颜色,适用于纹理与图元的大小相近时|
|----|----|
|D3DTEXF_LINEAR|对上下左右4个纹理元素进行加权平均|
|D3DTEXF_ANISOTROPIC|对映射点周围方形8个或更多的像素进行取样，获得平均值后映射到像素点上|
|D3DSAMP_MIPFILTER|采用不同分辨率的纹理序列|
游戏中常见的三种方式:
·**Bilinear** Interpolation （双线过滤D3DTEXF_LINEAR）
这种处理方式较适用于有一定景深的静态影像，不过无法提供最佳品质。其最大问题在于，当三维物体变得非常小时，一种被称为Depth Aliasing artifacts（深度赝样锯齿），也不适用于移动中的物件。 
·**Trilinear** Interpolation （三线过滤D3DSAMP_MIPFILTER+ D3DTEXF_LINEAR）
一个“双线过滤”需要三次混合，而“三线过滤”就得作七次混合处理，所以每个像素就需要多用21/3倍以上的计算时间。还需要两倍大的存储器时钟带宽。但是“三线过滤”可以提供最高的贴图品质，会去除材质的“闪烁”效果。对于需要动态物体或景深很大的场景应用方面而言，只有“三线过滤”才能提供可接受的材质品质。
·**Anisotropic** Interpolation （各向异性过滤D3DTEXF_ANISOTROPIC）
它在取样时候，会取8个甚至更多的像素来加以处理，所得到的质量最好。
可以与线性纹理过滤和多级渐进纹理过滤结合使用.一般来说4X以上才有效果
**纹理寻址模式**
当纹理坐标在[0.0, 1.0]范围之外时的情况
1.Wrap texture address mode(重叠)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Texture_Address_Wrap.JPG)
2.Mirror texture address mode(镜像)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Texture_Address_Mirror.JPG)
3.Clamp texture address mode(夹取)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Texture_Address_Clamp.JPG)
4.Border color texture address mode(边框颜色)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Texture_Address_BorderColor.JPG)
纹理阶段混合
颜色混合,纹理混合,Alpha混合,还有个凹凸映射?
**纹理包装**
Texture Wrapping,改变D3D光栅器使用纹理坐标对有纹理的多边形进行光栅化操作的基本方式.
U,V纹理包装的使用会影响D3D在U,V方向上对纹理坐标间最短线的选取.
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Texture_Wrapping.JPG)
上图中的U方向上的包装使AB之间的纹理坐标内插操作就穿越了0.0和1.0所在的交界线.
可以用纹理包装来做环境映射(现在通常都是用Cubemap吧?)略过
