# 使用Cocos2d-x 和粒子编辑器实现“天天爱消除”场景特效 - weixin_33985507的博客 - CSDN博客
2014年09月12日 11:16:45[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
**引言**
	Cocos2d-x引擎提供了强大的[粒子系统](http://cn.cocos2d-x.org/article/index?type=cocos2d-x&url=/doc/cocos-docs-master/manual/framework/native/v3/particle-system/zh.md)，
它在模仿自然现象、物理现象及空间扭曲上具备得天独厚的优势，为我们实现一些真实自然而又带有随机性的特效（如爆炸、烟花、水流）提供了方便。尽管如此，
它众多的粒子属性还是着实让人头疼。因为如果要想自己编码写出炫丽的粒子效果，这里有太多的属性需要手动设置和调节。不管是对新手还是资深的老油条程序员
来说，都存在不同程度的不便性。
**效果**
	所以，本文将结合 “天天爱消除” 游戏场景中的粒子特效，讲解如何使用粒子编辑器编辑实现飘飘扬扬往下飞落的的粒子效果。效果如下：
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126161612_1.gif)
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126161622_1.jpg)
**粒子编辑器的使用**
	现在网络中普遍使用的是 Particle 
Designer编辑器，不过它只支持在Mac系统下使用。对于像我这样的穷屌丝来说，怎么用的起！所以本文将介绍另一个粒子编辑器particle-
editor的使用方法，它的功能同样强大，而且还支持在windows系统下使用。两种编辑器的用法大同小异，都可以把编辑出的粒子效果直接导出为
plist文件。
下载地址
	particle-editor的下载地址：
[https://code.google.com/p/cocos2d-windows-particle-editor/downloads/detail?name=ParticleEditor%20V2.0.7z&can=2&q=](https://code.google.com/p/cocos2d-windows-particle-editor/downloads/detail?name=ParticleEditor%20V2.0.7z&can=2&q=)
使用方法
	particle-editor编辑器无需安装，下载解压后即可使用，它的编辑界面如下：
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162040_1.jpg)
	菜单栏的 Samples 下为用户提供了一些常用的粒子特效，如烟花、火、流星等，你可根据需要创建不同的粒子群。
**属性讲解**
编辑器的属性：
	1.IsBackgroundMove 编辑器背景图片是否滚动，如果你觉得它动来动去的很烦人，你可以把它设置为False
	2.IsSaveTextureImageData 是否把图片数据编码到文件里导出，这里我建议把它设置为False
	3.Scale 编辑器画布缩放比例大小
	编辑器的属性只影响编辑器内的预览效果，不对保存的.plist文件产生影响。而下面的所有属性都对应Cocos2dx中[粒子系统](http://cn.cocos2d-x.org/article/index?type=cocos2d-x&url=/doc/cocos-docs-master/manual/framework/native/v3/particle-system/zh.md)的各属性，会直接影响生成的.plist文件。
主要属性：
	1.Duration 发射器生存时间，即它可以发射粒子的时间，注意这个时间和粒子生存时间不同。单位秒，-1表示永远；粒子发射结束后可点击工具栏的播放按钮再次发射
	2.EmissionRate 每秒喷发的粒子数目
	3.IsAutoRemoveOnFinish 粒子结束时是否自动删除
	4.Mode 喷发器模式，有重力模式（Gravity）和半径模式（Radius，也叫放射模式）两种
	5.TotalParticles 场景中存在的最大粒子数目，往往与EmissionRate配合起来使用
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162120_1.jpg)
半径模式：半径模式可以使粒子以圆圈方式旋转，它也可以创造螺旋效果让粒子急速前进或后退。下列各属性只在半径模式下起作用。
	1.EndRadius 结束半径
	2.EndRadiusVar 结束半径变化范围，即结束半径值的范围在 （EndRadius – EndRadiusVar） 和 （EndRadius + EndRadiusVar ）之间，下面类似。
	3.RotatePerSecond 粒子每秒围绕起始点的旋转角度
	4.RotatePerSecondVar 粒子每秒围绕起始点的旋转角度变化范围
	5.StartRadius 初始半径
	6.StartRadius 初始半径变化范围
重力模式：顾名思义，重力模式模拟重力，可让粒子围绕一个中心点移近或移远，它的优点是非常动态，而且移动有规则。下列各属性只在重力模式下起作用。
	1.GravityX 重力X
	2.GravityY 重力Y
	3.RadiaAccel 粒子径向加速度，即平行于重力方向的加速度
	4.RadiaAccelVar 粒子径向加速度变化范围
	5.Speed 速度
	6.SpeedVar 速度变化范围
	7.TangentialAccel 粒子切向加速度，即垂直于重力方向的加速度
	8.TangentialAccelVar 粒子切向加速度变化范围
生命属性：
	1.Life 粒子生命，即粒子的生存时间
	2.LifeVar 粒子生命变化范围
大小属性：
	1.EndSize 粒子结束时的大小，-1表示和初始大小一致
	2.EndSize 粒子结束大小的变化范围
	3.StartSize 粒子的初始大小
	4.StartSize 粒子初始大小的变化范围
角度属性：
	1.Angle 粒子角度
	2.AngleVar 粒子角度变化范围
颜色属性：
	1.EndColor 粒子结束颜色
	2.EndColorVar 粒子结束颜色变化范围
	3.StartColor 粒子初始颜色
	4.StartColorVar 粒子初始颜色变化范围
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162419_1.jpg)
	如果你不想编辑出五颜六色的粒子效果，那应该把EndColorVar，StartColorVar尽量设置为（0, 0, 0, 0)。
位置属性:
	1.PositionType 粒子位置类型，有自由模式（Free）、相对模式（Relative）和打组模式（Grouped）三种
	2.PosVarX 发射器位置的横向变化范围，烟花出现在位置相对中心位置的像素偏移
	3.PosVarY 发射器位置的纵向变化范围
	4.SourcePositionX 发射器原始X坐标位置
	5.SourcePositionY 发射器原始Y坐标位置
	Free自由模式，相对于屏幕自由，不会随粒子节点移动而移动（可产生火焰、蒸汽等效果）；Relative相对模式，相对于被绑定的Node静止，粒子
发射器会随Node移动而移动，可用于制作移动角色身上的特效等等；Grouped打组模式是相对于发射点的，粒子随发射器移动而移动。
自旋属性：
	1.EndSpin 粒子结束自旋角度
	2.EndSpinVar 粒子结束自旋角度变化范围
	3.StartSpin 粒子开始自旋角度
	4.StartSpinVar 粒子开始自旋角度变化范围
纹理渲染属性:
	1.DestBlendFunc 目的纹理的混合模式
	2.SrcBlendFunc 源纹理的混合模式
	3.TextureImageData 纹理数据
	4.TexturePath 纹理路径，目前只可为Debug.win32目录下文件名
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162613_1.jpg)
	Cocos2dx使用OpenGL混合原理对图形进行渲染绘制。混合就是指把两种颜色混在一起，具体一点就是把某一像素位置原来的颜色和将要画上去的颜
色，通过某种方式混在一起，从而实现特殊的效果。它是一种常用的技巧，通常可以用来实现半透明，你也可以通过不同的设置得到不同的混合结果，产生一些有趣
或者奇怪的图象。
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162656_1.png)
	这里的源纹理和目标纹理跟绘制的顺序有关，先绘制的纹理是目标纹理，后绘制的纹理理所应当的就成为了源纹理。下面具体来看看混合模式的属性值。
DestBlendFunc和SrcBlendFunc的常用值为以下几种情况：
	1.GL_ZERO： 表示使用0.0作为因子，实际上相当于不使用这种颜色参与混合运算。
	2.GL_ONE： 表示使用1.0作为因子，实际上相当于完全的使用了这种颜色参与混合运算。
	3.GL_SRC_ALPHA：表示使用源颜色的alpha值来作为因子。
	4.GL_DST_ALPHA：表示使用目标颜色的alpha值来作为因子。
	5.GL_ONE_MINUS_SRC_ALPHA：表示用1.0减去源颜色的alpha值来作为因子。
	6.GL_ONE_MINUS_DST_ALPHA：表示用1.0减去目标颜色的alpha值来作为因子。
	最常用的组合模式有两种：GL_ONE和GL_ONE_MINUS_SRC_ALPHA，GL_ONE_MINUS_SRC_ALPHA和GL_SRC_ALPHA。
	介绍完编辑器的各属性项以后，我们就可以根据需要编辑出不同效果的粒子效果了。下图是类似天天爱消除游戏中的“雪花”粒子特效。
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162737_1.jpg)
	资源图片
![](http://cc.cocimg.com/cms/uploads/allimg/140126/4196_140126162827_1.png)
	在使用粒子编辑器编辑粒子时，应放在粒子编辑器的ParticleEditor目录下；导出plist文件时，应把资源图片和plist文件放一起。你可以点这里下载该plist
注意：这里目的纹理的混合模式设置为GL_ONE，源纹理的混合模式设置为GL_ONE_MINUS_SRC_ALPHA。
**cocos2dx中加载使用plist文件**
	只要编辑出粒子效果，那在cocos2d-x引擎中加载plist文件就非常简单了。使用以下代码来读取粒子效果：
```cpp
```
```cpp
//贴背景图片  
    Sprite* pSprite = Sprite::create("IMG1.jpg");  
    pSprite->setPosition(Point(-20 , 0));  
    pSprite->setAnchorPoint(Point(0, 0));  
    this->addChild(pSprite, 0);  
    
    //读取粒子效果  
    ParticleSystem* m_emitter1 = ParticleSystemQuad::create("snow.plist");  
    m_emitter1->retain();  
    ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());  
    batch->addChild(m_emitter1);  
    addChild(batch, 10);  
    m_emitter1->release();  
    
    //或读取粒子效果  
    //ParticleSystem* m_emitter1 = ParticleSystemQuad::create("snow.plist");  
    //addChild(m_emitter1);
```
	这里ParticleBatchNode可以引用且只可以引用1个texture(一个图片文件，一个texture图集)，增加到SpriteBatchNode中的ParticleSystem都是在OpenGL ES调用绘图函数时绘制的。
如果ParticleSystem没有增加到ParticleBatchNode中，OpenGL ES会调用每个[粒子系统](http://cn.cocos2d-x.org/article/index?type=cocos2d-x&url=/doc/cocos-docs-master/manual/framework/native/v3/particle-system/zh.md)的绘图函数，这样做效率会比较低。
	好了，基本上就这些，比想象中简单多了吧。
	工程下载地址：[https://github.com/renshan/btnTest/tree/master/Test-snow](https://github.com/renshan/btnTest/tree/master/Test-snow)
