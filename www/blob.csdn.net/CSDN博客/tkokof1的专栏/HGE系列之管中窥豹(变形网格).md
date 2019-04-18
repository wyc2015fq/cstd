# HGE系列之管中窥豹(变形网格) - tkokof1的专栏 - CSDN博客

2011年06月06日 13:07:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：1673




    HGE系列之管中窥豹(变形网格)


  近来实在忙了一些，一直没有时间继续这个HGE系列，但实际上，时间紧之类的说辞深究起来往往都是有些苍白无力的，尤其当你一心想做成某事时，时间总归是可以挤出来的（时间就像海绵......），所以细细想来还是自己的惰性导致了长时间的“搁笔”，应该好好反省才是啊...... ：）

  话题扯得远了一些，今天我们来看看HGE对于变形网格（hgeDistortionMesh）的支持，这也是一般的“纯”2D引擎很难做到的一点，值得我们细细的研习一番，因为利用她我们可以完成一些很奇妙的画面效果 ：）

一．声明

  按照以前的惯例，我们依然首先来看看hgeDistortionMesh的声明：

// hge 变形网格类

class hgeDistortionMesh

{

public:

     // 普通的构造函数以及析构函数

     hgeDistortionMesh(int cols, int rows);

     hgeDistortionMesh(const hgeDistortionMesh &dm);

     ~hgeDistortionMesh();

     // 自定义 operator = 操作符

     // NOTE: 一般如果你定义了复制构造函数，那么这个定义也是必须的

 hgeDistortionMesh&operator= (const hgeDistortionMesh &dm);

     // 渲染函数

     voidRender(float x, float y);

     // 以指定颜色以及深度值清除

     voidClear(DWORD col=0xFFFFFFFF, float z=0.5f);

     // 设置纹理

     voidSetTexture(HTEXTURE tex);

     // 设置纹理范围

     voidSetTextureRect(float x, float y, float w, float h);

     // 设置纹理混合方式

     voidSetBlendMode(int blend);

     // 设置深度

     voidSetZ(int col, int row, float z);

     // 设置颜色

     voidSetColor(int col, int row, DWORD color);

     // 设置变形数值（位移，相对点）

     voidSetDisplacement(int col, int row, float dx, float dy, int ref);

     // 获取纹理

     HTEXTUREGetTexture() const {return quad.tex;}

     // 获取纹理范围

     voidGetTextureRect(float *x, float *y, float *w, float *h) const { *x=tx; *y=ty; *w=width; *h=height; }

     // 获取纹理混合方式

     intGetBlendMode() const { return quad.blend; }

     // 获取深度值

     floatGetZ(int col, int row) const;

     // 获取颜色

     DWORDGetColor(int col, int row) const;

     // 获取变形数据（位移，相对点）

     voidGetDisplacement(int col, int row, float *dx, float *dy, int ref) const;

     // 获取行数

 intGetRows() { return nRows; }

     // 获取列数

 intGetCols() { return nCols; }

private:

    // 私有化缺省构造函数，达到屏蔽作用

hgeDistortionMesh();

    // 静态HGE指针，hgeDistortionMesh通过她获取必要的基本功能支持

static HGE*hge;

    // 变形顶点数组

hgeVertex*disp_array;

    // 行数，列数

intnRows, nCols;

    // 每个变形‘节点’的宽度，高度

floatcellw,cellh;

    // 纹理坐标，纹理宽度及高度

floattx,ty,width,height;

    // hgeQuad类型，基本可以认为是一张可映射纹理的矩形面片

hgeQuadquad;

};

  好了，以上便是hgeDistortionMesh的全部声明，平坦简洁 ：）

  接下来让我们简单的依次讲解一下：

  缺省构造函数中基本都是一些初始化操作，没有什么特别值得注意的地方

hgeDistortionMesh::hgeDistortionMesh(int cols, int rows)

{

int i;

// 初始化静态hge指针

hge=hgeCreate(HGE_VERSION);

    // 初始化相关数值

nRows=rows;

nCols=cols;

cellw=cellh=0;

quad.tex=0;

quad.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE;

// 初始化变形数据数组

    disp_array=new hgeVertex[rows*cols];

for(i=0;i<rows*cols;i++)

{

disp_array[i].x=0.0f;

disp_array[i].y=0.0f;

disp_array[i].tx=0.0f;

disp_array[i].ty=0.0f;

disp_array[i].z=0.5f;

disp_array[i].col=0xFFFFFFFF;

}

}

  析构函数很简单，释放先前构造时动态申请的变形数据数组并释放hge接口：

hgeDistortionMesh::~hgeDistortionMesh()

{

delete[] disp_array;

hge->Release();

}

  另外的复制构造函数以及赋值运算符函数并没有特别值得注意的地方，只是直观的拷贝数据而已，在此不再赘述 ：)

  接下来的让我们看看几个重要的成员函数：

  首先是设置纹理范围：

void hgeDistortionMesh::SetTextureRect(float x, float y, float w, float h)

{

int i,j;

float tw,th;

tx=x; ty=y; width=w; height=h;

    // 首先取得纹理宽高

if (quad.tex)

{

tw=(float)hge->Texture_GetWidth(quad.tex);

th=(float)hge->Texture_GetHeight(quad.tex);

}

else

{

tw = w;

th = h;

}

    // 计算每个变形单元（Cell）的宽高

cellw=w/(nCols-1);

cellh=h/(nRows-1);

for(j=0; j<nRows; j++)

for(i=0; i<nCols; i++)

{

            // 设置每个变形单元的纹理坐标

            // NOTE：注意设置方式

disp_array[j*nCols+i].tx=(x+i*cellw)/tw;

disp_array[j*nCols+i].ty=(y+j*cellh)/th;

            // 设置每个单元的实际坐标

disp_array[j*nCols+i].x=i*cellw;

disp_array[j*nCols+i].y=j*cellh;

}

}

  接着让我们看看清除函数：

void hgeDistortionMesh::Clear(DWORD col, float z)

{

int i,j;

for(j=0; j<nRows; j++)

for(i=0; i<nCols; i++)

{

            // 将纹理坐标设置为初始值

disp_array[j*nCols+i].x=i*cellw;

disp_array[j*nCols+i].y=j*cellh;

// 重新设置颜色和深度为给定值

            disp_array[j*nCols+i].col=col;

disp_array[j*nCols+i].z=z;

}

}

  然后是渲染函数：

void hgeDistortionMesh::Render(float x, float y)

{

int i,j,idx;

// 遍历所有变形节点

// NOTE：注意索引的控制

for(j=0; j<nRows-1; j++)

for(i=0; i<nCols-1; i++)

{

            // 计算基索引

idx=j*nCols+i;

            // 设置第一个定点的参数，索引为 idx

quad.v[0].tx=disp_array[idx].tx;

quad.v[0].ty=disp_array[idx].ty;

quad.v[0].x=x+disp_array[idx].x;

quad.v[0].y=y+disp_array[idx].y;

quad.v[0].z=disp_array[idx].z;

quad.v[0].col=disp_array[idx].col;

            // 设置第二个定点的参数，索引为 idx + 1

quad.v[1].tx=disp_array[idx+1].tx;

quad.v[1].ty=disp_array[idx+1].ty;

quad.v[1].x=x+disp_array[idx+1].x;

quad.v[1].y=y+disp_array[idx+1].y;

quad.v[1].z=disp_array[idx+1].z;

quad.v[1].col=disp_array[idx+1].col;

            // 设置第三个定点的参数，索引为 idx+nCols+1

quad.v[2].tx=disp_array[idx+nCols+1].tx;

quad.v[2].ty=disp_array[idx+nCols+1].ty;

quad.v[2].x=x+disp_array[idx+nCols+1].x;

quad.v[2].y=y+disp_array[idx+nCols+1].y;

quad.v[2].z=disp_array[idx+nCols+1].z;

quad.v[2].col=disp_array[idx+nCols+1].col;

// 设置第四个定点的参数， 索引为 idx+nCols

            quad.v[3].tx=disp_array[idx+nCols].tx;

quad.v[3].ty=disp_array[idx+nCols].ty;

quad.v[3].x=x+disp_array[idx+nCols].x;

quad.v[3].y=y+disp_array[idx+nCols].y;

quad.v[3].z=disp_array[idx+nCols].z;

quad.v[3].col=disp_array[idx+nCols].col;

            // 渲染这个hgeQuad

hge->Gfx_RenderQuad(&quad);

}

}

  渲染函数并不复杂，关于其中的顶点索引可能值得一提：

![](http://hi.csdn.net/attachment/201106/6/0_130733683680U8.gif)

  相信大家看了上面这张示意图之后应该不难理解Render中顶点的设置方式了，好，让我们继续上路，接着来看看hgeDistortionMesh的SetDisplacement：

void hgeDistortionMesh::SetDisplacement(int col, int row, float dx, float dy, int ref)

{

// 前置检查

if(row<nRows && col<nCols)

{

switch(ref)

{

            // HGEDISP_NODE，则dx,dy根据当前单元坐标进行调整

case HGEDISP_NODE:dx+=col*cellw; dy+=row*cellh; break;

            // HGEDISP_CENTER，则dx,dy根据中心点坐标进行调整

case HGEDISP_CENTER:dx+=cellw*(nCols-1)/2;dy+=cellh*(nRows-1)/2; break;

            // HGEDISP_TOPLEFT，则dx,dy不进行调整

case HGEDISP_TOPLEFT:break;

}

disp_array[row*nCols+col].x=dx;

disp_array[row*nCols+col].y=dy;

}

}

  好，至此关于hgeDistortionMesh的重点内容都已讲述完毕，有兴趣的朋友可以参考一下hge自带的范例程序5以获得进一步的hgeDistortionMesh使用信息 ：）

  总体而言，hge虽然对于变形网格仅提供了基本的支持，但同时也保持了简单和高效，这也是我比较喜欢hge的一点（KISS ：）），也是他易于上手的原因之一，经过了此次hgeDistortionMesh的探究，我们对于hge的辅助类别基本都已讲述完毕，剩下的就是hge称为core的大头了，哈哈，算是一个阶段的“告成”吧（礼花彩蛋准备先:))，好了好了，小小庆祝之余，还是那句：我们下次再见吧 ：）






