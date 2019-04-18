# HGE系列之九 管中窥豹(精灵动画) - tkokof1的专栏 - CSDN博客

2011年01月02日 22:19:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：4991


HGE系列之九管中窥豹(精灵动画)

这次的HGE之旅，让我们来看看精灵及动画的实现，毕竟对于一款2D游戏引擎来说，恐怕精灵和动画不是最重要的，也可算是最重要之一了吧：）

HGE内部对于精灵以及动画的实现其实相对简单，主要都是有hgeSprite（精灵）和hgeAnimation（动画）这两个类来完成所需的操作，内部使用的接口也都是hge基类所提供的（具体细节请参看源码实现），基本的原理也并无什么特殊的地方：精灵也便是一张贴图，动画也是传统的逐帧动画：）

好了，让我们闲话少叙，先来看一看hgeSprite的内部构造：

**类名**：hgeSprite

**功能**：精灵类

**头文件**：hge/hge181/include/hgeSprite.h

**实现文件**：hge/hge181/src/helpers/hgeSprite.cpp

先来看看hgeSprite提供的各项功能（对外接口）：

**class** hgeSprite

{

**public:**

// 参数构造函数，注意一下各个参数

hgeSprite(HTEXTURE tex, float x, float y, float w, float h);

hgeSprite(const hgeSprite &spr);

~hgeSprite() { hge->Release(); }

// 渲染函数集

voidRender(float x, float y);

voidRenderEx(float x, float y, float rot, float hscale=1.0f, float vscale=0.0f);

voidRenderStretch(float x1, float y1, float x2, float y2);

voidRender4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

// 设置纹理

voidSetTexture(HTEXTURE tex);

// 设置纹理使用尺寸

voidSetTextureRect(float x, float y, float w, float h, bool adjSize = true);

// 设置颜色

voidSetColor(DWORD col, int i=-1);

// 设置Z Buffer

voidSetZ(float z, int i=-1);

// 设置混合方式

voidSetBlendMode(int blend) { quad.blend=blend; }

// 设置“热点”（中心点）

voidSetHotSpot(float x, float y) { hotX=x; hotY=y; }

// 设置纹理翻转

voidSetFlip(bool bX, bool bY, bool bHotSpot = false);

// 获取纹理

HTEXTUREGetTexture() const { return quad.tex; }

// 获取纹理使用尺寸voidGetTextureRect(float *x, float *y, float *w, float *h) const { *x=tx; *y=ty; *w=width; *h=height; }

// 获取颜色

DWORDGetColor(int i=0) const { return quad.v[i].col; }

// 获取Z Buffer

floatGetZ(int i=0) const { return quad.v[i].z; }

// 获取混合模式

intGetBlendMode() const { return quad.blend; }

// 获取“热点”（中心点）

voidGetHotSpot(float *x, float *y) const { *x=hotX; *y=hotY; }

// 获取纹理翻转值

voidGetFlip(bool *bX, bool *bY) const { *bX=bXFlip; *bY=bYFlip; }

// 获取宽度

floatGetWidth() const { return width; }

// 获取高度

floatGetHeight() const { return height; }

// 获取包围盒

hgeRect*GetBoundingBox(float x, float y, hgeRect *rect) const { rect->Set(x-hotX, y-hotY, x-hotX+width, y-hotY+height); return rect; }

// 获取包围盒（考虑缩放以及旋转）

hgeRect*GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale,hgeRect *rect) const;

**protected:**

// 禁止默认构造函数

hgeSprite();

// hge基类接口，用于调用其开放的接口函数

static HGE*hge;

// hgeQuad类，定义请见hge.h

hgeQuadquad;

// 纹理坐标，使用宽高

floattx, ty, width, height;

// 纹理宽高

floattex_width, tex_height;

// 热点坐标

floathotX, hotY;

// 翻转标志

boolbXFlip, bYFlip, bHSFlip;

};

以上便是hgeSprite的全部内容，看来并不复杂：）那么接下来就让我们来看看这hgeSprite的内部实现吧：

首先让我们来看看hgeSprite的构造函数：

hgeSprite::hgeSprite(HTEXTURE texture, float texx, float texy, float w, float h)

{

float texx1, texy1, texx2, texy2;

// 创建hge

hge=hgeCreate(HGE_VERSION);

// 根据构造参数设定相应的纹理参数

tx=texx; ty=texy;

width=w; height=h;

// 如果纹理“句柄”存在

if(texture)

{

// 调用hge API获取纹理宽高

tex_width = (float)hge->Texture_GetWidth(texture);

tex_height = (float)hge->Texture_GetHeight(texture);

}

else

{

// 否则设置为1.0

tex_width = 1.0f;

tex_height = 1.0f;

}

// 设定“热点”值为左上起点

hotX=0;

hotY=0;

// 设置翻转标志为false

bXFlip=false;

bYFlip=false;

bHSFlip=false;

// 设置quad结构中的纹理（tex）

quad.tex=texture;

texx1=texx/tex_width;

texy1=texy/tex_height;

texx2=(texx+w)/tex_width;

texy2=(texy+h)/tex_height;

// 设置quad结构中的纹理坐标

quad.v[0].tx = texx1; quad.v[0].ty = texy1;

quad.v[1].tx = texx2; quad.v[1].ty = texy1;

quad.v[2].tx = texx2; quad.v[2].ty = texy2;

quad.v[3].tx = texx1; quad.v[3].ty = texy2;

// 设置Z Buffer为默认值0.5

quad.v[0].z = 

quad.v[1].z = 

quad.v[2].z = 

quad.v[3].z = 0.5f;

// 设置顶点颜色为默认值黑色

quad.v[0].col = 

quad.v[1].col = 

quad.v[2].col = 

quad.v[3].col = 0xffffffff;

// 设置混合方式为默认

quad.blend=BLEND_DEFAULT;

}

接着让我们看一看hgeSprite的渲染函数，篇幅关系，在此仅以Render函数为例讲述，其余三个渲染函数原理类似，有兴趣的朋友请参看实现源码：

void hgeSprite::Render(float x, float y)

{

float tempx1, tempy1, tempx2, tempy2;

// 设置渲染坐标，注意“热点”（中心点）的处理

tempx1 = x-hotX;

tempy1 = y-hotY;

tempx2 = x+width-hotX;

tempy2 = y+height-hotY;

// 根据计算的渲染坐标设置设置顶点坐标

quad.v[0].x = tempx1; quad.v[0].y = tempy1;

quad.v[1].x = tempx2; quad.v[1].y = tempy1;

quad.v[2].x = tempx2; quad.v[2].y = tempy2;

quad.v[3].x = tempx1; quad.v[3].y = tempy2;

// 调用hge API完成渲染

hge->Gfx_RenderQuad(&quad);

}

可以看到，渲染的核心便是**Gfx_RenderQuad**这个API函数了：）

接着让我们来看看hgeSprite是如何获取包围盒的：

hgeRect* hgeSprite::GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale, hgeRect *rect) const

{

float tx1, ty1, tx2, ty2;

float sint, cost;

// 清空rect中的坐标数值

rect->Clear();

// 根据缩放值重新计算顶点坐标

tx1 = -hotX*hscale;

ty1 = -hotY*vscale;

tx2 = (width-hotX)*hscale;

ty2 = (height-hotY)*vscale;

// 如果旋转角度不为零

if (rot != 0.0f)

{

cost = cosf(rot);

sint = sinf(rot);

// 根据旋转值“扩展”包围盒的大小

// 关于Encapsulate的更多信息可以参看[这里](http://blog.csdn.net/tkokof1/archive/2010/05/16/5596678.aspx)

rect->Encapsulate(tx1*cost - ty1*sint + x, tx1*sint + ty1*cost + y);

rect->Encapsulate(tx2*cost - ty1*sint + x, tx2*sint + ty1*cost + y);

rect->Encapsulate(tx2*cost - ty2*sint + x, tx2*sint + ty2*cost + y);

rect->Encapsulate(tx1*cost - ty2*sint + x, tx1*sint + ty2*cost + y);

}

else

{

// 否则“扩展”包围盒时则不考虑旋转

rect->Encapsulate(tx1 + x, ty1 + y);

rect->Encapsulate(tx2 + x, ty1 + y);

rect->Encapsulate(tx2 + x, ty2 + y);

rect->Encapsulate(tx1 + x, ty2 + y);

}

return rect;

}

获取包围盒的基本思想便是根据目前quad的大小（考虑缩放以及旋转）来获取一个完整包围其尺寸的最小矩形，有点简化的AABB的味道：）

接着让我们再来看看hgeSprite是如何设置翻转的：

void hgeSprite::SetFlip(bool bX, bool bY, bool bHotSpot)

{

float tx, ty;

// 如果设置了“热点”翻转以及X轴翻转，则重新计算“热点”X轴坐标

if(bHSFlip && bXFlip) hotX = width - hotX;

// 如果设置了“热点”翻转以及Y轴翻转，则重新计算“热点”Y轴坐标

if(bHSFlip && bYFlip) hotY = height - hotY;

// 重新设置翻转“热点”标志

bHSFlip = bHotSpot;

// 重新计算“热点”坐标

if(bHSFlip && bXFlip) hotX = width - hotX;

if(bHSFlip && bYFlip) hotY = height - hotY;

// 如果X轴设置翻转

if(bX != bXFlip)

{

// 交换顶点（0、1顶点对以及2、3定点对）的纹理坐标

tx=quad.v[0].tx; quad.v[0].tx=quad.v[1].tx; quad.v[1].tx=tx;

ty=quad.v[0].ty; quad.v[0].ty=quad.v[1].ty; quad.v[1].ty=ty;

tx=quad.v[3].tx; quad.v[3].tx=quad.v[2].tx; quad.v[2].tx=tx;

ty=quad.v[3].ty; quad.v[3].ty=quad.v[2].ty; quad.v[2].ty=ty;

bXFlip=!bXFlip;

}

// 如果Y轴设置翻转

if(bY != bYFlip)

{

// 交换顶点（0、3顶点对以及1、2定点对）的纹理坐标

tx=quad.v[0].tx; quad.v[0].tx=quad.v[3].tx; quad.v[3].tx=tx;

ty=quad.v[0].ty; quad.v[0].ty=quad.v[3].ty; quad.v[3].ty=ty;

tx=quad.v[1].tx; quad.v[1].tx=quad.v[2].tx; quad.v[2].tx=tx;

ty=quad.v[1].ty; quad.v[1].ty=quad.v[2].ty; quad.v[2].ty=ty;

bYFlip=!bYFlip;

}

}

设置翻转的原理其实非常简单，交换纹理坐标而已：）

最后让我们来看看hgeSprite是如何来重新设置纹理的：

void hgeSprite::SetTexture(HTEXTURE tex)

{

float tx1,ty1,tx2,ty2;

float tw,th;

// 重新设置quad结构纹理

quad.tex=tex;

// 获取纹理高宽

if(tex)

{

tw = (float)hge->Texture_GetWidth(tex);

th = (float)hge->Texture_GetHeight(tex);

}

else

{

tw = 1.0f;

th = 1.0f;

}

// 如果重新设置的纹理与原始纹理高宽不符

if(tw!=tex_width || th!=tex_height)

{

// 还原原始纹理“绝对”坐标

tx1=quad.v[0].tx*tex_width;

ty1=quad.v[0].ty*tex_height;

tx2=quad.v[2].tx*tex_width;

ty2=quad.v[2].ty*tex_height;

// 重新设置纹理高宽

tex_width=tw;

tex_height=th;

// 重新设置纹理“相对”坐标

tx1/=tw; ty1/=th;

tx2/=tw; ty2/=th;

// 设置quad结构纹理坐标

quad.v[0].tx=tx1; quad.v[0].ty=ty1; 

quad.v[1].tx=tx2; quad.v[1].ty=ty1; 

quad.v[2].tx=tx2; quad.v[2].ty=ty2; 

quad.v[3].tx=tx1; quad.v[3].ty=ty2; 

}

}

至此我们基本便将hgeSprite简单剖分了一遭，但是单单的精灵有时还是缺乏不少动感，有时我们还需要动画的帮助，于是hgeAnimation便诞生了：

**类名**：hgeAnimation

**功能**：精灵动画类

**头文件**：hge/hge181/include/hgeAnim.h

**实现文件**：hge/hge181/src/helpers/hgeAnim.cpp

依例，让我们首先来看看hgeAnimation的头文件：

// hgeAnimation继承于hgeSprite

class hgeAnimation : public hgeSprite

{

public:

// 参数构造函数

hgeAnimation(HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h);

hgeAnimation(const hgeAnimation &anim);

// 播放动画

voidPlay();

// 停止播放

voidStop() { bPlaying=false; }

// 回复播放

voidResume() { bPlaying=true; }

// 更新动画状态

voidUpdate(float fDeltaTime);

// 是否在播放

boolIsPlaying() const { return bPlaying; }

// 重新设置纹理

voidSetTexture(HTEXTURE tex) { hgeSprite::SetTexture(tex); orig_width = hge->Texture_GetWidth(tex, true); }

// 重新设置使用纹理

voidSetTextureRect(float x1, float y1, float x2, float y2) { hgeSprite::SetTextureRect(x1,y1,x2,y2); SetFrame(nCurFrame); }

// 设置播放模式

voidSetMode(int mode);

// 设置播放速度

voidSetSpeed(float FPS) { fSpeed=1.0f/FPS; }

// 设置当前帧

voidSetFrame(int n);

// 设置动画帧数

voidSetFrames(int n) { nFrames=n; }

// 获取动画播放模式

intGetMode() const { return Mode; }

// 获取播放速度

floatGetSpeed() const { return 1.0f/fSpeed; }

// 获取当前帧

intGetFrame() const { return nCurFrame; }

// 获取动画帧数

intGetFrames() const { return nFrames; }

private:

// 隐藏默认构造函数

hgeAnimation();

// 原始纹理宽度？

intorig_width;

// 是否在播放

boolbPlaying;

// 播放速度

floatfSpeed;

// 与上一帧播放的时间间隔

floatfSinceLastFrame;

// 播放模式

intMode;

// 播放间隔帧数

intnDelta;

// 动画帧数

intnFrames;

// 当前帧

intnCurFrame;

};

然我们来看看hgeAnimation的构造函数：

hgeAnimation::hgeAnimation(HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h)

: hgeSprite(tex, x, y, w, h)

{

// 获取原始纹理宽度

orig_width = hge->Texture_GetWidth(tex, true);

// 设置经过时间

fSinceLastFrame=-1.0f;

// 设置播放速度

fSpeed=1.0f/FPS;

// 设置是否播放（默认为否）

bPlaying=false;

// 设置动画帧数

nFrames=nframes;

// 设置播放模式（默认为向前并循环）

Mode=HGEANIM_FWD | HGEANIM_LOOP;

// 设置播放间隔（默认为1帧）

nDelta=1;

// 设置当前帧为第零帧

SetFrame(0);

}

hgeAnimation的构造函数不无多少新奇的地方，所做的工作基本亦是初始化相关变量。

接着来看看hgeAnimation如何设置播放模式：

void hgeAnimation::SetMode(int mode)

{

Mode=mode;

// 如果播放模式为反向播放

if(mode & HGEANIM_REV)

{

// 设置播放帧间隔为-1

nDelta = -1;

// 并设置当前帧为动画最后一帧

SetFrame(nFrames-1);

}

else

{

// 否则设置播放间隔为1

nDelta = 1;

// 并设置当前帧为动画第一帧

SetFrame(0);

}

}

哈哈，是不是相当简单，那么让我们再来看看hgeAniamtion是如何设置当前帧的：

void hgeAnimation::SetFrame(int n)

{

float tx1, ty1, tx2, ty2;

bool bX, bY, bHS;

// 设置动画列数

int ncols = int(orig_width) / int(width);

// 设置当前帧数（取余）

n = n % nFrames;

// 如果n为负，则代表从后往前计算帧数

if(n < 0) n = nFrames + n;

nCurFrame = n;

// 为当前帧计算纹理坐标

ty1 = ty;

tx1 = tx + n*width;

// 如果当前纹理宽度大于边界（threshold）值，则进行“**换行**"处理

if(tx1 > orig_width-width)

{

// 减去所在纹理“第一行”的余帧，

// 即得到当前的所在帧数（换行后）

n -= int(orig_width-tx) / int(width);

// 重新计算纹理X坐标

tx1 = width * (n%ncols);

// 重新计算纹理Y坐标

ty1 += height * (1 + n/ncols);

}

tx2 = tx1 + width;

ty2 = ty1 + height;

// 计算“相对”纹理坐标

tx1 /= tex_width;

ty1 /= tex_height;

tx2 /= tex_width;

ty2 /= tex_height;

// 重新设置quad结构纹理坐标

quad.v[0].tx=tx1; quad.v[0].ty=ty1;

quad.v[1].tx=tx2; quad.v[1].ty=ty1;

quad.v[2].tx=tx2; quad.v[2].ty=ty2;

quad.v[3].tx=tx1; quad.v[3].ty=ty2;

// 重新设置翻转

bX=bXFlip; bY=bYFlip; bHS=bHSFlip;

bXFlip=false; bYFlip=false;

SetFlip(bX,bY,bHS);

}

从源码实现可以看出，hgeAnimation内部使用的是一个“矩阵型”的动画纹理，随着播放帧数的改变，内建的这个SetFrame函数会正确的设置相应的纹理坐标。

最后，让我们以hgeAnimation的Update函数进行结尾：

void hgeAnimation::Update(float fDeltaTime)

{

// 如果不在播放则直接返回

if(!bPlaying) return;

// 如果是第一次更新，则重新设置经过时间为0

if(fSinceLastFrame == -1.0f)

fSinceLastFrame=0.0f;

else

// 否则便累计经过时间

fSinceLastFrame += fDeltaTime;

// 当达到播放时间间隔时间

while(fSinceLastFrame >= fSpeed)

{

// 更新经过时间

fSinceLastFrame -= fSpeed;

// 如果达到了最后一帧

if(nCurFrame + nDelta == nFrames)

{

switch(Mode)

{

// 向前模式

case HGEANIM_FWD:

// 反向、往返（PingPong）模式

case HGEANIM_REV | HGEANIM_PINGPONG:

// 则停止播放

bPlaying = false;

break;

// 向前、往返模式

case HGEANIM_FWD | HGEANIM_PINGPONG:

// 向前、往返、循环模式

case HGEANIM_FWD | HGEANIM_PINGPONG | HGEANIM_LOOP:

// 反向、往返、循环模式

case HGEANIM_REV | HGEANIM_PINGPONG | HGEANIM_LOOP:

// 更新播放间隔

nDelta = -nDelta;

break;

}

}

// 已经到达了第一帧

else if(nCurFrame + nDelta < 0)

{

switch(Mode)

{

// 反向模式

case HGEANIM_REV:

// 前向、往返模式

case HGEANIM_FWD | HGEANIM_PINGPONG:

bPlaying = false;

break;

// 反向、往返模式

case HGEANIM_REV | HGEANIM_PINGPONG:

// 反向、往返、循环模式

case HGEANIM_REV | HGEANIM_PINGPONG | HGEANIM_LOOP:

// 前向、往返、循环模式

case HGEANIM_FWD | HGEANIM_PINGPONG | HGEANIM_LOOP:

// 更新播放间隔

nDelta = -nDelta;

break;

}

}

// 最后设置当前帧

if(bPlaying) SetFrame(nCurFrame+nDelta);

}

}

好了，hge的精灵以及动画至此算是讲了一个梗概，更细节的问题大家可以参照源码以及文档，那么最后，下次再见吧：）

