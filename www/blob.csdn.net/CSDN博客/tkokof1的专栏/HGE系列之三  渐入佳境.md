# HGE系列之三    渐入佳境 - tkokof1的专栏 - CSDN博客

2009年12月03日 10:38:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：4439



HGE系列之三渐入佳境

前两次“乱七八糟”的讲述了一些HGE的基础知识，不知看过的朋友有何感想，反正我自己都觉着有些不知所谓（**！**），但本着坚持到底的原则，今天继续献上拙文一篇，如果有朋友实在看不过去，大可不吝嘲笑、大肆放言：）

好了，转至今天的主题，这次我们来更深入的揣摩一番HGE的示例程序，为了简单起见，我选择了HGE SDK中自带的示例程序三，因其深度不深不浅，依其进一步讲解HGE着实适当：）那么，首先的首先呢，先让我们瞅瞅这程序运行起来是个什么样子：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/tkokof1/EntryImages/20091203/HGE_Tutorial_3633954349747968750.JPG)


怎样，有些简约的味道吧：）当然，光凭一纸图片也远不能描绘程序中各方操作表现，所以在此我严重建议有兴致的朋友可以自己亲身试上一把（**注意相关游戏资源需要置于同一目录下**），想必之后对于下面的**示例代码**会有更深的体会：

**//****必须包含的一些头文件**

**//****提供HGE****的底层接口操作**

#include "../../include/hge.h"

**// HGE****精灵类**

#include "../../include/hgesprite.h"

**// HGE****字体类，用于显示文字信息**

#include "../../include/hgefont.h"

**// HGE****粒子类**

#include "../../include/hgeparticle.h"

**// ****全局的HGE****接口类指针**

HGE *hge=0;

**// ****全局的HGE****对象指针**

**// HGE****精灵，spr****为图示中的小球，spt****则为粒子（图示中的小五角星）**

hgeSprite*spr;

hgeSprite*spt;

**// HGE****字体类，用于显示图示中左上角的帧率信息**

hgeFont*fnt;

**// ****用以管理粒子生成与消亡的hgeParticleSystem****类**

hgeParticleSystem*par;

**// HGE****资源句柄（实际上即是DWORD****类型）**

**// ****纹理句柄**

HTEXTUREtex;

**// ****声效句柄**

HEFFECTsnd;

**// ****一些全局的变量信息**

**// x****、y ****为小球的当前位置坐标**

float x=100.0f, y=100.0f;

**// dx****、dy****为小球的位置改变（偏移）量**

float dx=0.0f, dy=0.0f;

**//****小球的速度**

const float speed=90;

**//****摩擦系数，用以削减小球的实际位移量**

const float friction=0.98f;

**// ****播放声效**

void boom() {

**//****设置声道**

int pan=int((x-400)/4);

**//****设置音高**

float pitch=(dx*dx+dy*dy)*0.0005f+0.2f;

**//****播放（****具体细节请参见HGE****帮助文档****：）下同****）**

hge->Effect_PlayEx(snd,100,pan,pitch);

}

**//****帧函数（HGE****应用程序必须编写并设置该函数）**

bool FrameFunc()

{

**// ****获得时间改变量**

float dt=hge->Timer_GetDelta();

**// ****处理按键**

if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

if (hge->Input_GetKeyState(HGEK_LEFT)) dx-=speed*dt;

if (hge->Input_GetKeyState(HGEK_RIGHT)) dx+=speed*dt;

if (hge->Input_GetKeyState(HGEK_UP)) dy-=speed*dt;

if (hge->Input_GetKeyState(HGEK_DOWN)) dy+=speed*dt;

**// ****执行位移量的计算和碰撞检测（防止小球越界）**

dx*=friction; dy*=friction; x+=dx; y+=dy;

if(x>784) {x=784-(x-784);dx=-dx;boom();}

if(x<16) {x=16+16-x;dx=-dx;boom();}

if(y>584) {y=584-(y-584);dy=-dy;boom();}

if(y<16) {y=16+16-y;dy=-dy;boom();}

**// ****更新粒子系统**

par->info.nEmission=(int)(dx*dx+dy*dy)*2;

par->MoveTo(x,y);

par->Update(dt);

return false;

}

**//****渲染函数**

bool RenderFunc()

{

**//****渲染图形**

**//****开始渲染场景**

hge->Gfx_BeginScene();

**//****以黑色（0****）清屏**

hge->Gfx_Clear(0);

**//****渲染粒子系统**

par->Render();

**//****渲染小球**

spr->Render(x, y);

**//****输出信息**

fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f/nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());

**//****结束渲染场景**

hge->Gfx_EndScene();

return false;

}

**//****标准WinMain****函数**

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

{

**//****创建HGE****实例**

hge = hgeCreate(HGE_VERSION);

**//****设置日志文件名，用以记录一些程序运行的信息**

hge->System_SetState(HGE_LOGFILE, "hge_tut03.log");

**//****设置帧函数**

hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

**//****设置渲染函数**

hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

**//****设置程序标题**

hge->System_SetState(HGE_TITLE, "HGE Tutorial 03 - Using helper classes");

**//****设置帧率**

hge->System_SetState(HGE_FPS, 100);

**//****设置为窗口程序**

hge->System_SetState(HGE_WINDOWED, true);

**//****设置窗口宽高及位深**

hge->System_SetState(HGE_SCREENWIDTH, 800);

hge->System_SetState(HGE_SCREENHEIGHT, 600);

hge->System_SetState(HGE_SCREENBPP, 32);

**//****初始化HGE**

if(hge->System_Initiate()) {

**// ****加载声音及纹理**

snd=hge->Effect_Load("menu.wav");

tex=hge->Texture_Load("particles.png");

if(!snd || !tex)

{

**// ****如果其中一项加载失败则报告错误并关闭HGE**

**// ****简单的MessageBox****信息框**

MessageBox(NULL, "Can't load one of the following files:/nMENU.WAV, PARTICLES.PNG, FONT1.FNT, FONT1.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);

hge->System_Shutdown();

hge->Release();

return 0;

}

**// ****创建并设置HGE****精灵**

spr=new hgeSprite(tex, 96, 64, 32, 32);

spr->SetColor(0xFFFFA000);

spr->SetHotSpot(16,16);

**// ****加载字体**

fnt=new hgeFont("font1.fnt");

**// ****创建并设置HGE****粒子类**

spt=new hgeSprite(tex, 32, 32, 32, 32);

spt->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);

spt->SetHotSpot(16,16);

par=new hgeParticleSystem("trail.psi",spt);

par->Fire();

// **Let's rock now!**

hge->System_Start();

**// ****删除创建的对象并释放加载的资源**

delete par;

delete fnt;

delete spt;

delete spr;

hge->Texture_Free(tex);

hge->Effect_Free(snd);

}

**// ****关闭HGE**

hge->System_Shutdown();

hge->Release();

return 0;

}

好了，程序代码就是这些，相信通过注释大家应该能够理通其中的十之八九，但毫无疑问的是，在其中我也略去了很多细节的讲述，譬如小球的移动方式（小球实际上是会反弹滴：）），粒子系统创建时所使用的参数等等，但是我发现如果全部讲述的话，不仅表述冗长，叙述中心也会偏移，毕竟我们只是想对HGE的使用有一个更全面一些的认识，所以嘛，本着[KISS原则](http://zh.wikipedia.org/wiki/KISS%E5%8E%9F%E5%88%99)（**如果你说****KISS****原则不是这么用的…****嗯…****好吧，我承认你是对的**），就让我们在此点到辄止，至于更深入的内容就以后再说吧：）

最后的最后，还是那句：下次再见喽：）



