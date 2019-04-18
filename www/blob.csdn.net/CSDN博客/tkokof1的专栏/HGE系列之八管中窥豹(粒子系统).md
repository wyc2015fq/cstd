# HGE系列之八管中窥豹(粒子系统) - tkokof1的专栏 - CSDN博客

2010年10月05日 11:09:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：4948



HGE系列之八管中窥豹(粒子系统)

这次的HGE系列让我们一起来学习一下HGE引擎的粒子系统部分，对于粒子系统不甚了解的朋友可以从[这里](http://en.wikipedia.org/wiki/Particle_system)开始了解。

首先还是让我们从头文件开始：

**类名**：hgeParticle

**功能**：单个粒子的属性结构

**头文件**：hge/hge181/include/hgeParticle.h

**实现文件**：无 

struct hgeParticle

{

// 粒子所在的位置

hgeVectorvecLocation;

// 粒子的速度(Velocity)

hgeVectorvecVelocity;

// 粒子的重力

floatfGravity;

// 粒子的线加速度

floatfRadialAccel;

// 粒子的角加速度

floatfTangentialAccel;

// 粒子的旋转角度

floatfSpin;

// 粒子旋转角度偏移量

floatfSpinDelta;

// 粒子的大小

floatfSize;

// 粒子的大小偏移量

floatfSizeDelta;

// 粒子的颜色（带alpha值）

hgeColorcolColor;// + alpha

// 粒子的颜色偏移量

hgeColorcolColorDelta;

// 粒子的生存时间

floatfAge;

// 粒子的死亡时间

floatfTerminalAge;

};

仅一个简单的数据集合而已，各中变量相信大家一目了然：）

**类名**：hgeParticleSystemInfo

**功能**：粒子系统的相关信息结构

**头文件**：hge/hge181/include/hgeParticle.h

**实现文件**：无

struct hgeParticleSystemInfo

{

// 粒子贴图

hgeSprite*sprite;// texture + blend mode

// 每秒的粒子数

intnEmission; // particles per sec

// 生命周期

floatfLifetime;

// 最小生命周期

floatfParticleLifeMin;

// 最大生命周期

floatfParticleLifeMax;

// 方向

floatfDirection;

// 偏移角度

floatfSpread;

// 绝对值还是相对值，用以计算生成粒子的初始速度

boolbRelative;

// 最低速度

floatfSpeedMin;

// 最高速度

floatfSpeedMax;

// 最小重力

floatfGravityMin;

// 最大重力

floatfGravityMax;

// 最低线加速度

floatfRadialAccelMin;

// 最高线加速度

floatfRadialAccelMax;

// 最低角加速度

floatfTangentialAccelMin;

// 最高角加速度

floatfTangentialAccelMax;

// 起始大小

floatfSizeStart;

// 最终大小

floatfSizeEnd;

// 大小变化值

floatfSizeVar;

// 起始旋转角度

floatfSpinStart;

// 最终旋转角度

floatfSpinEnd;

// 角度变化值

floatfSpinVar;

// 起始颜色

hgeColorcolColorStart; // + alpha

// 最终颜色

hgeColorcolColorEnd;

// 颜色变化值

floatfColorVar;

// alpha变化值

floatfAlphaVar;

};

仍然是一个数据集合而已：）

接着让我们看一看hgeParticleSystem这个类型：

**类名**：hgeParticleSystem

**功能**：粒子系统

**头文件**：hge/hge181/include/hgeParticle.h

**实现文件**： hge/hge181/src/helpers/hgeparticle.cpp

class hgeParticleSystem

{

public:

// 用于存储粒子系统的相关信息（详见hgeParticleSystemInfo的定义）

hgeParticleSystemInfo info;

// 构造函数&析构函数

hgeParticleSystem(const char *filename, hgeSprite *sprite);

hgeParticleSystem(hgeParticleSystemInfo *psi);

hgeParticleSystem(const hgeParticleSystem &ps);

~hgeParticleSystem() { hge->Release(); }

// 重载赋值运算符

hgeParticleSystem&operator= (const hgeParticleSystem &ps);

// 渲染粒子系统

voidRender();

// 在指定位置启动粒子系统

voidFireAt(float x, float y);

// 启动粒子系统

voidFire();

// 停止粒子系统

voidStop(bool bKillParticles=false);

// 更新粒子系统

voidUpdate(float fDeltaTime);

// 移动粒子系统的位置

voidMoveTo(float x, float y, bool bMoveParticles=false);

// 设定粒子系统的位置偏移量

voidTranspose(float x, float y) { fTx=x; fTy=y; }

// 设置粒子系统的缩放比例

voidSetScale(float scale) { fScale = scale; }

// 设置是否跟踪粒子系统的边界盒

voidTrackBoundingBox(bool bTrack) { bUpdateBoundingBox=bTrack; }

// 获取当前粒子系统中活动的粒子个数

intGetParticlesAlive() const { return nParticlesAlive; }

// 获得粒子系统的生命值

floatGetAge() const { return fAge; }

// 获取粒子系统位置

voidGetPosition(float *x, float *y) const { *x=vecLocation.x; *y=vecLocation.y; }

// 获取位置偏移量

voidGetTransposition(float *x, float *y) const { *x=fTx; *y=fTy; }

// 获取缩放比例

floatGetScale() { return fScale; }

// 获取边界盒

hgeRect*GetBoundingBox(hgeRect *rect) const;

private:

// 私有化默认构造函数，达到屏蔽默认构造的作用

hgeParticleSystem();

// HGE静态指针

static HGE*hge;

// 生命值

floatfAge;

// 剩余未有发射（生成）的粒子数量

floatfEmissionResidue;

// 前一个位置

hgeVectorvecPrevLocation;

// 当前位置

hgeVectorvecLocation;

// 偏移量

floatfTx, fTy;

// 缩放因子

floatfScale;

// 活动的粒子个数

intnParticlesAlive;

// 粒子系统的边界盒

hgeRectrectBoundingBox;

// 是否更新边界盒

boolbUpdateBoundingBox;

// hgeParticle数组，用以存储所有的粒子

hgeParticleparticles[MAX_PARTICLES];

};

总的来说，hgeParticleSystem封装了一个相对简洁的粒子系统的各项功能，其头文件声明的数据及函数成员并未有十分费解的地方，下面就让我们看一看其的实现源码：

首先自然是他的构造函数：

hgeParticleSystem::hgeParticleSystem(const char *filename, hgeSprite *sprite)

{

void *psi;

// 创建HGE，以获得其提供的底层接口

hge=hgeCreate(HGE_VERSION);

// 使用Resource_Load加载文件

psi=hge->Resource_Load(filename);

if(!psi) return;

// 将文件中的内容拷贝至info（hgeParticleSystemInfo）中

memcpy(&info, psi, sizeof(hgeParticleSystemInfo));

// 释放资源

hge->Resource_Free(psi);

// 设置粒子精灵类

info.sprite=sprite;

// 初始化粒子系统的位置

vecLocation.x=vecPrevLocation.x=0.0f;

vecLocation.y=vecPrevLocation.y=0.0f;

// 初始化粒子系统的偏移位置

fTx=fTy=0;

// 初始化缩放因子，1.0f代表不做缩放

fScale = 1.0f;

// 初始化未发射（生成）粒子数量

fEmissionResidue=0.0f;

// 初始化活动粒子数

nParticlesAlive=0;

// 初始化粒子生命值

fAge=-2.0;

// 初始化边界盒

rectBoundingBox.Clear();

// 默认不更新边界盒

bUpdateBoundingBox=false;

}

各项操作还是相当易解的，剩下的两个构造函数如出一辙，在此不再赘述，只是就其中的复制构造函数，在这里我想说上一说：

源代码中，关于复制构造函数的实现很简单：

hgeParticleSystem::hgeParticleSystem(const hgeParticleSystem &ps)

{

memcpy(this, &ps, sizeof(hgeParticleSystem));

hge=hgeCreate(HGE_VERSION);

}

由于hgeParticleSystem并不涉及资源的管理（关于HGE的资源管理，可以看看[这篇](http://blog.csdn.net/tkokof1/archive/2010/06/07/5652143.aspx)），所以结构内容基本可以按照[POD](http://en.wikipedia.org/wiki/Plain_old_data_structure)数据的方式进行复制，这也是该函数中大胆使用memcpy的一个原因，但是令我比较意外的是，虽然hgeParticleSystem的头文件中声明了重载赋值运算符函数：

/ / 重载赋值运算符

hgeParticleSystem&operator= (const hgeParticleSystem &ps);

但是却没有加以实现，所以当使用时必然引起链接错误，如果这是作者的初衷的话，我想将其变为私有（private）将更为妥当，而且按照C++中公认的编程规范，一旦你定义了复制构造函数，那么你也应该定义赋值构造函数。对于这个类别，我想比较妥当的定义方法也许可以是这个样子：

hgeParticleSystem::hgeParticleSystem(const hgeParticleSystem &ps)

{

memcpy(this, &ps, sizeof(hgeParticleSystem));

hge=hgeCreate(HGE_VERSION);

}

hgeParticleSystem&hgeParticleSystem::operator= (const hgeParticleSystem &ps)

{

if( this == &ps )

return;

memcpy(this, &ps, sizeof(hgeParticleSystem)); 

}

另外一提的便是fAge这个成员变量，在构造函数中他被设置成了-2.0：

// 初始化粒子生命值

fAge=-2.0;

相信不少朋友对于这个-2.0的由来应该比较奇怪，其实他是一个“[魔数](http://en.wikipedia.org/wiki/Magic_number_(programming))”，代表着粒子系统尚未启动（使用FireAt或者Fire来启动粒子系统），不过这种编程技巧我个人是比较不赞同的，我的建议是至少使用枚举之类明显的变量值进行代替，最低限度也可以使用粗糙的宏定义来简单的包装一下，譬如：

#define PARTICLE_SYSTEM_NOT_START (-2.0)

好了，闲话少叙，接着让我们来看看其他的成员函数：

// 移动粒子系统的位置

void hgeParticleSystem::MoveTo(float x, float y, bool bMoveParticles)

{

int i;

float dx,dy;

// 如果移动所有活动粒子的话

if(bMoveParticles)

{

// 计算目标坐标与当前坐标的偏移量

dx=x-vecLocation.x;

dy=y-vecLocation.y;

// 重新设置所有活动粒子的位置

for(i=0;i<nParticlesAlive;i++)

{

particles[i].vecLocation.x += dx;

particles[i].vecLocation.y += dy;

}

// 重新计算先前所在坐标的位置

vecPrevLocation.x=vecPrevLocation.x + dx;

vecPrevLocation.y=vecPrevLocation.y + dy;

}

// 否则仅移动即将发射的粒子

else

{

// 如果粒子系统尚未启动

if(fAge==-2.0) { vecPrevLocation.x=x; vecPrevLocation.y=y; }

else { vecPrevLocation.x=vecLocation.x;vecPrevLocation.y=vecLocation.y; }

}

// 设置当前位置坐标

vecLocation.x=x;

vecLocation.y=y;

}

可以看到，相关的代码并不复杂，值得注意一下的是代码中对于vecPrevLocation的设置：）

接着是FireAt、Fire以及对应的Stop：

// 在指定位置启动粒子系统

void hgeParticleSystem::FireAt(float x, float y)

{

// 简单三部曲：首先停止，接着移动，最后启动

Stop();

MoveTo(x,y);

Fire();

}

// 启动粒子系统

void hgeParticleSystem::Fire()

{

// 注意fAge的设置

if(info.fLifetime==-1.0f) fAge=-1.0f;

else fAge=0.0f;

}

// 停止粒子系统

void hgeParticleSystem::Stop(bool bKillParticles)

{

// 注意fAge的设置

fAge=-2.0f;

// 如果需要清除粒子的话

if(bKillParticles) 

{

// 重新设置活动粒子数量为0

nParticlesAlive=0;

// 重置粒子系统的边界盒

rectBoundingBox.Clear();

}

}

再者让我们看看如何渲染粒子系统：

// 渲染粒子系统

void hgeParticleSystem::Render()

{

int i;

DWORD col;

hgeParticle *par=particles;

// 获取粒子精灵的颜色

col=info.sprite->GetColor();

// 对于每一个活动的粒子

for(i=0; i<nParticlesAlive; i++)

{

// 设置粒子精灵的颜色

if(info.colColorStart.r < 0)

info.sprite->SetColor(SETA(info.sprite->GetColor(),par->colColor.a*255));

else

info.sprite->SetColor(par->colColor.GetHWColor());

// 渲染粒子精灵，注意各个参数的设置

info.sprite->RenderEx(par->vecLocation.x*fScale+fTx, par->vecLocation.y*fScale+fTy, par->fSpin*par->fAge, par->fSize*fScale);

// 移动到下一活动粒子

par++;

}

// 还原先前的粒子精灵颜色

info.sprite->SetColor(col);

}

边界盒的获取也并不复杂：

hgeRect *hgeParticleSystem::GetBoundingBox(hgeRect *rect) const

{

*rect = rectBoundingBox;

// 根据缩放因子缩放坐标

rect->x1 *= fScale;

rect->y1 *= fScale;

rect->x2 *= fScale;

rect->y2 *= fScale;

return rect;

}

最后，让我们来看一看稍有些复杂的Update函数：

void hgeParticleSystem::Update(float fDeltaTime)

{

int i;

float ang;

hgeParticle *par;

hgeVector vecAccel, vecAccel2;

// 如果粒子系统生命值大于0

if(fAge >= 0)

{

// 累计生命值

fAge += fDeltaTime;

// 如果超过系统生命周期时间，则重新设置为未启动

if(fAge >= info.fLifetime) fAge = -2.0f;

}

// 更新所有活动粒子

// 更新粒子系统边界盒

if(bUpdateBoundingBox) rectBoundingBox.Clear();

par=particles;

// 对于每一个活动粒子

for(i=0; i<nParticlesAlive; i++)

{

// 更新生命值

par->fAge += fDeltaTime;

// 如果达到死亡时间

if(par->fAge >= par->fTerminalAge)

{

// 递减活动粒子数目

nParticlesAlive--;

// 将最后一个活动粒子拷贝至此处

memcpy(par, &particles[nParticlesAlive], sizeof(hgeParticle));

// 递减计数值，以达到下次继续处理该位置粒子的目的

i--;

continue;

}

// 计算粒子的线加速度

vecAccel = par->vecLocation-vecLocation;

vecAccel.Normalize();

vecAccel2 = vecAccel;

vecAccel *= par->fRadialAccel;

// vecAccel2.Rotate(M_PI_2);

// the following is faster

// 计算线加速度方向的切线方向

ang = vecAccel2.x;

vecAccel2.x = -vecAccel2.y;

vecAccel2.y = ang;

// 计算角角速度

vecAccel2 *= par->fTangentialAccel;

// 计算粒子的实际速度

par->vecVelocity += (vecAccel+vecAccel2)*fDeltaTime;

// 考虑到重力的影响

par->vecVelocity.y += par->fGravity*fDeltaTime;

// 计算粒子的位置

par->vecLocation += par->vecVelocity*fDeltaTime;

// 计算粒子的旋转角度

par->fSpin += par->fSpinDelta*fDeltaTime;

// 计算粒子的大小

par->fSize += par->fSizeDelta*fDeltaTime;

// 计算粒子的颜色

par->colColor += par->colColorDelta*fDeltaTime;

// 更新粒子边界盒

if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

// 移动到下一个粒子

par++;

}

// 生成新的粒子

// 如果粒子系统已经启动的话

if(fAge != -2.0f)

{

// 计算需要生成的粒子数

float fParticlesNeeded = info.nEmission*fDeltaTime + fEmissionResidue;

// 强制装换为无符号整数

int nParticlesCreated = (unsigned int)fParticlesNeeded;

// 计算遗留下来未有生成的粒子，等待下一次更新生成

fEmissionResidue=fParticlesNeeded-nParticlesCreated;

par=&particles[nParticlesAlive];

// 对于每一个需要生成的粒子

for(i=0; i<nParticlesCreated; i++)

{

// 如果活动粒子已经超过上界，则不在生成

if(nParticlesAlive>=MAX_PARTICLES) break;

// 设置粒子的各项属性

// 生命值

par->fAge = 0.0f;

par->fTerminalAge = hge->Random_Float(info.fParticleLifeMin, info.fParticleLifeMax);

// 设置粒子的位置

par->vecLocation = vecPrevLocation+(vecLocation-vecPrevLocation)*hge->Random_Float(0.0f, 1.0f);

par->vecLocation.x += hge->Random_Float(-2.0f, 2.0f);

par->vecLocation.y += hge->Random_Float(-2.0f, 2.0f);

// 设置粒子的速度值，注意角度的计算ang=info.fDirection-M_PI_2+hge->Random_Float(0,info.fSpread)-info.fSpread/2.0f;

if(info.bRelative) ang += (vecPrevLocation-vecLocation).Angle()+M_PI_2;

par->vecVelocity.x = cosf(ang);

par->vecVelocity.y = sinf(ang);

par->vecVelocity *= hge->Random_Float(info.fSpeedMin, info.fSpeedMax);

// 设置粒子的重力、线角速度以及角加速度

par->fGravity = hge->Random_Float(info.fGravityMin, info.fGravityMax);

par->fRadialAccel = hge->Random_Float(info.fRadialAccelMin, info.fRadialAccelMax);

par->fTangentialAccel = hge->Random_Float(info.fTangentialAccelMin, info.fTangentialAccelMax);

// 设置粒子的大小及偏移量

par->fSize = hge->Random_Float(info.fSizeStart, info.fSizeStart+(info.fSizeEnd-info.fSizeStart)*info.fSizeVar);

par->fSizeDelta = (info.fSizeEnd-par->fSize) / par->fTerminalAge;

// 设置粒子的旋转角度及偏移量

par->fSpin = hge->Random_Float(info.fSpinStart, info.fSpinStart+(info.fSpinEnd-info.fSpinStart)*info.fSpinVar);

par->fSpinDelta = (info.fSpinEnd-par->fSpin) / par->fTerminalAge;

// 设置粒子颜色

par->colColor.r = hge->Random_Float(info.colColorStart.r, info.colColorStart.r+(info.colColorEnd.r-info.colColorStart.r)*info.fColorVar);

par->colColor.g = hge->Random_Float(info.colColorStart.g, info.colColorStart.g+(info.colColorEnd.g-info.colColorStart.g)*info.fColorVar);

par->colColor.b = hge->Random_Float(info.colColorStart.b, info.colColorStart.b+(info.colColorEnd.b-info.colColorStart.b)*info.fColorVar);

par->colColor.a = hge->Random_Float(info.colColorStart.a, info.colColorStart.a+(info.colColorEnd.a-info.colColorStart.a)*info.fAlphaVar);

// 设置粒子颜色偏移量

par->colColorDelta.r = (info.colColorEnd.r-par->colColor.r) / par->fTerminalAge;

par->colColorDelta.g = (info.colColorEnd.g-par->colColor.g) / par->fTerminalAge;

par->colColorDelta.b = (info.colColorEnd.b-par->colColor.b) / par->fTerminalAge;

par->colColorDelta.a = (info.colColorEnd.a-par->colColor.a) / par->fTerminalAge;

// 更新粒子系统边界盒

if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

// 递增活动粒子数目

nParticlesAlive++;

// 移动到下一个待生成粒子

par++;

}

}

// 设置先前粒子系统位置

vecPrevLocation=vecLocation;

}

好了，至此，我们完全可以使用hgeParticleSystem来生成不错的粒子特效了，不过在这之前，让我们继续来看一下HGE对于hgeParticleSystem的更进一步封装：hgeParticleManager


**类名**：hgeParticleManager

**功能**：粒子系统管理类

**头文件**：hge/hge181/include/hgeParticle.h

**实现文件**： hge/hge181/src/helpers/hgepmanager.cpp

头文件如下：

class hgeParticleManager

{

public:

// 构造函数&析构函数

hgeParticleManager();

~hgeParticleManager();

// 更新函数

voidUpdate(float dt);

// 渲染函数

voidRender();

// 生成粒子系统

hgeParticleSystem*SpawnPS(hgeParticleSystemInfo *psi, float x, float y);

// 粒子系统是否活动

boolIsPSAlive(hgeParticleSystem *ps) const;

// 设置粒子系统偏移

voidTranspose(float x, float y);

// 获取粒子系统位移

voidGetTransposition(float *dx, float *dy) const {*dx=tX; *dy=tY;}

// 销毁指定的粒子系统

voidKillPS(hgeParticleSystem *ps);

// 销毁所有粒子系统

voidKillAll();

private:

// 私有化复制构造函数及赋值操作符函数，已达到屏蔽作用

hgeParticleManager(const hgeParticleManager &);

hgeParticleManager&operator= (const hgeParticleManager &);

// 粒子系统数量

intnPS;

// 偏移位置坐标

floattX;

floattY;

// 粒子系统指针数组

hgeParticleSystem*psList[MAX_PSYSTEMS];

};

hgeParticleManager看来还是相对简单的：）

首先自然是构造函数和析构函数：

hgeParticleManager::hgeParticleManager()

{

// 设定初始值，使用成员列表更好一些

nPS=0;

tX=tY=0.0f;

}

hgeParticleManager::~hgeParticleManager()

{

// 遍历指针数组依次删除

int i;

for(i=0;i<nPS;i++) delete psList[i];

}

没有什么奇特的地方，那么接着让我们来看一看SpawnPS：

hgeParticleSystem* hgeParticleManager::SpawnPS(hgeParticleSystemInfo *psi, float x, float y)

{

// 如果粒子系统超过上界，则返回

if(nPS==MAX_PSYSTEMS) return 0;

// 生成粒子系统

psList[nPS]=new hgeParticleSystem(psi);

// 启动

psList[nPS]->FireAt(x,y);

// 设置偏移

psList[nPS]->Transpose(tX,tY);

// 递增粒子系统数目

nPS++;

return psList[nPS-1];

}

函数很简单，生成粒子系统而已：）

接下来的几个成员函数也并不困难：

bool hgeParticleManager::IsPSAlive(hgeParticleSystem *ps) const

{

// 依次遍历数组查找指定的粒子系统

int i;

for(i=0;i<nPS;i++) if(psList[i]==ps) return true;

return false;

}

void hgeParticleManager::Transpose(float x, float y)

{

// 依次遍历并设置粒子系统偏移

int i;

for(i=0;i<nPS;i++) psList[i]->Transpose(x,y);

// 更新tX和tY

tX=x; tY=y;

}

void hgeParticleManager::KillPS(hgeParticleSystem *ps)

{

int i;

// 依次遍历指针数组

for(i=0;i<nPS;i++)

{

// 如果找到

if(psList[i]==ps)

{

// 删除这个粒子系统

delete psList[i];

// 并将其指向最后一个活动的粒子系统

psList[i]=psList[nPS-1];

// 递减粒子系统数目

nPS--;

return;

}

}

}

void hgeParticleManager::KillAll()

{

// 遍历并删除所有粒子系统

int i;

for(i=0;i<nPS;i++) delete psList[i];

nPS=0;

}

最后，让我们来看一看Update和Render：

void hgeParticleManager::Render()

{

// 依次遍历并渲染每个粒子系统

int i;

for(i=0;i<nPS;i++) psList[i]->Render();

}

Render相当简单，那么Update如何呢：

void hgeParticleManager::Update(float dt)

{

int i;

// 对于每个存在的粒子系统

for(i=0;i<nPS;i++)

{

// 更新该粒子系统

psList[i]->Update(dt);

// 如果该粒子系统尚未启动并且活动粒子数目为0

if(psList[i]->GetAge()==-2.0f && psList[i]->GetParticlesAlive()==0)

{

// 删除这个粒子系统

delete psList[i];

// 将其指向最后一个活动粒子系统

psList[i]=psList[nPS-1];

// 更新粒子系统数目

nPS--;

// 递减计数值，已达到下次循环继续处理该位置粒子系统的目的

i--;

}

}

}

呼，至此，HGE的粒子系统终算泛泛的讲解完毕了，虽然篇幅不短，但也还算简单，平心而论，HGE的粒子系统虽说并不十分复杂，但也提供了非常不错的显示效果和可扩展性，对于我们关于粒子系统的学习还是很有助益的，有兴趣的朋友远可以进一步使用或者扩展，说不定可以搞出令人惊奇的东西（到时务必告知一下，以便观摩学习）！好了好了，我想我废话也够多了，是时候说ByeBye了，那么，下次再见吧：）

