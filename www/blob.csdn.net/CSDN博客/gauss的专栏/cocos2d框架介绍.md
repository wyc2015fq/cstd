# cocos2d框架介绍 - gauss的专栏 - CSDN博客
2013年02月24日 11:33:40[gauss](https://me.csdn.net/mathlmx)阅读数：365
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
转载：http://blog.csdn.net/liangneo/article/details/6770543
存属于 收藏
# cocos2d引擎
## 2.0 cocos2d简介
Cocos2d最初的版本是用python写的,之后被改成objectiveC,应用于iphone上,之后国内某人又根据该设计理念,将其改为C++版本cocos2dX,该版本可以在iphone,widnows,andriod三个平台上运行.最新的版本是1.0.0,该引擎C++的版本还引入了lua脚本系统.cocos2d是完全开源的引擎,开发者可以任意使用该代码而不用公开源代码.cocos2d被广泛的使用,其中包括Zynga,南宫梦等知名游戏公司.Cocos2d不仅仅只是一个2D图形引擎库,他还提供了一些对游戏逻辑支持的系统.cocos2d提供了一个简单的声音引擎,支持播放mp3,wave,ogg等文件格式的音乐.cocos2d可以和物理引擎box2d配合使用,实现碰撞等物理效果.cocos2d引擎的结构清晰,文档和实例比较丰富.易学易用.
## 2.1 cocos2d 基本结构
Cocos2d采用场景树结构来管理游戏对象,把一个游戏划分为不同的场景,场景又可以分为不同的层,一个层又可以拥有任意个可见的游戏对象.游戏对象又可以执行Action来修改其属性.每一个时刻都有一个场景在独立运行,通过切换不同的场景来完成一个游戏流程.cocos2d还采用了引用计数的方式来管理内存,基本上所有的类都派生于拥有引用计数的机制的CCObject.其基本框架类图如下: 
![](http://hi.csdn.net/attachment/201109/13/0_13158823314YZk.gif)
a.      CCApplication,负责平台相关的初始化,消息处理等,是一个单件类
b.      CCDirector,主要职责是管理场景和切换场景,游戏暂停等.另外他还负责初始化openes相关的一东西.也是一个单件类
c.      CCNode,节点类,是游戏中最重要的对象,在cocos2dk中,任何可见的或不可以的游戏逻辑对象都是一个节点,节点有以下特点:
a)        是一个容器,可以包含任意多个其它节点.
b)        可以执行一个行为(CCAction)
c)        可以执行一定时的任务回调
d)        拥有坐标变换(transform)信息,如postion,scale,rotation等
e)        由于一个节点可以添加任意个子节点,所有的节点组成一个树状结构,因此父节点的坐标变换会影响子节点在世界坐标系中的坐标,一个节点只能拥有一个父节点
d.      CCAction,动作类,CCAtion就好像指派给CCNode的命令一样,在特定时间(或立即)内修改CCNode的属性,例如位置,旋转,缩放等
e.      CCScene,场景类,和PL中的stage类似,游戏场景一般是指在某个时刻能独立运行的部分,包括可见的怪物,地形和不可见的逻辑脚本等.例如一个游戏的一个特定关卡,游戏菜单,过场动画等都是一个独立的场景.cocos2d中的场景又由不同的层(CCLayer)组成.由于CCScene是CCNode的子类,因此可以可以用CCAction来修改其属性
f.       CCLayer类,场景中的层类,一个场景可以拥有多个层,如HUD层,游戏对象层,背景层,天空层等,层是一个可以绘制的区域.可以将ui,sprite等对象添加到其中.除此之外,层还接收输入消息,可以在层的类中处理输入消息,和CCScene一样,CClayer是CCNode的子类,也可以用CCAction来修改其属性
## 2.2资源管理与文件系统
资源管理可以分为两个方面,一是管理一些原生(Raw)的资源,例如模型,贴图,脚本,字体等.另外一种资源是在游戏运行时分配的游戏对象.他们需要能被快速的访问.一般来说,没有被引用的游戏对象一般不会立即被释放,会先缓存到某个地方.在加载新的场景时.才会被释放.在一些大型的渲染系统中,资源会被分为三个阶段,硬盘,内存,显存资源,分别采用不同的策略和cache机制来管理,cocos2d没有明显的提出资源管理的概念,但使用了一些cache的机制来缓存已经加载了的纹理,frame,animation等.对于运行时分配的游戏对象,cocos2d也提供了一个autorelease的机制,在游戏切换场景时,将会对autorelease的对象进行内存回收.由于cocos2d采用了引用计数的机制.循环引用是一个需要关注的问题.
对于文件系统,cocos2d封装了一个简单的类CCFileUtils,该类除了能读取普通的文件之外,还可以从zip文件读取文件.该类还读取plist文件生成一个CCDictionary对象.
## 2.3 sprite与渲染系统
         Cocos2d的Sprite由Texure,frame,和animation组成,由openes负责渲染主要的类关系如下:
![](http://hi.csdn.net/attachment/201109/13/0_1315882434qm05.gif)
a.      Sprite 可以看成是某一时刻,一个2d的图片的一个剪辑部分
b.      frame由texture和一个矩形(quad)组成
c.      texture代表一个可以被绘制的2维的纹理
d.      imgae对应硬盘中不同类型的图片,例如jpg,png,tga等.它知道如何从文件中读取不同类型的图片,生成缓冲数据,供texture使用
e.      CCAnimation由一系列的frame组成,类似于电影的胶片集合
f.       CCAnimate,动画action,实现在指定时间内在sprite上播放指定的动画(CCAnimation)
### 2.3.1 Texture与Image
         Cocos2d中的CCTexture2D用来描述一个2d的纹理,它知道如何将自己绘制到屏幕上.通过该类还可以设置纹理过滤,抗锯齿等参数.该类还提供了一个接口,将字符串创建成纹理.该类所包含的纹理大小必须是2^n.因此纹理的大小不一定就等于图片的大小.CCTexture2D以下成员:
a.      name,指向纹理在显存中的地址
b.      pixelFormat,描述纹理的格式,例如RGBA888,或RGB565等
c.      pixelWidth,pixelHeight,纹理的宽高
d.      maxS,maxT,纹理的最大坐标
既然已经有了CCTexture2D这样的类纹理的渲染,那么还需要Imgae类做什么呢?因为图片的格式多样,不同的格式,不同平台下有不同的加载方法,因此cocos2d使用了一个CCImage类封装不同格式的图片在不同平台下的加载.如果说CCImage中的buffer对应的是内在资源,那么CCTexture2D对应的buffer是显存资源.
### 2.3.2 Frame与Animation
         上面讲过,Frame保存了一个纹理和矩形,在cocos2d中,一般都会给frame起个名字,以字典的形式保存在CCSpriteFrameCache中,方便随时读取.CCSpriteFrameCache支持从plist(类似于xml)文件读取图片,根据文件生成一系列的frame,并且给每个frame都生成一个名字. plist的文件可以使用很多第三方工具生成,例如Zwoptex([http://zwoptexapp.com/](http://zwoptexapp.com/)),TexturePacker([http://code-and-web.de/texturepacker](http://code-and-web.de/texturepacker));
CCAnimation类用来在CCspreite对象上实现动画.他保存了一系列的frame.和frame之间的延时.可以通过使用CCAnimationaction来在sprite上播放一个动画.
### 2.3.3 渲染流程
         由于cocos2d只处理2d图形,渲染采用了固定流水线的方式.当时也可以直接使用openes的函数来完成一些高级的特效.和渲染相关的类有CCTexutre2D,CCTexutreAtlas, CCSprite,CCRenderTexture,CCRenderTexture支持将数据渲染到纹理上.另外CCNode还一个相机类,但一般情况都不使用.只有在实现一些3D特效的时候,才使用相机的一些特性.
## 2.4 粒子系统
Cocos2d支持两种类型的粒子系统,点状(CCParticleSystemPoint)和方形(CCParticleQuadSsytem)粒子系统,Quad类型的系统支持一些额外的特性
a.      可以旋转
b.      可以是任何大小的,而point的尺寸不能超过64
c.      Quad类型的粒子系统可以被整体缩放
d.      Quad类型的粒子系统比Point粒子系统消耗更多的内存和显存
粒子系统还支持从plist文件中读取数据,plist文件可以通过第三方工具生成,例如ParticleDesigner([http://particledesigner.71squared.com/](http://particledesigner.71squared.com/)),和ParticleMint
## 2.5 UI与字体
         Cocos2d中只有简单的ui实现,支持button,label,toggle三种类型的ui,另外还支持简单的文本输入框(TextField).类关系图如下:
![](http://hi.csdn.net/attachment/201109/13/0_1315882476GG9o.gif)
a.      CCMenu从CClayer派生而来,可以处理输入消息,CCMenu包含多个CCMenuItem对象,在接收到输入消息之后,根据触摸位置,将输入消息分发到相应MenuItem,执行回调函数
b.      CCMenu还提供一个对齐菜单项的接口
c.      CCMenuItem 菜单项的基类,目前cocos2d只从CCMenuItem派生了CCMenuItemLabel,CCMenuItemSprite,CCMenuToggle三个类.当然也可以派生新的类,实现其它的菜单项,例如checkbox等.
Cocos2d支持两种类型的字体,一种是平台相关的TTF字体,另外一种是平台无关的美术字体,该类型字体可以通过字体编辑器(例如[http://glyphdesigner.71squared.com/](http://glyphdesigner.71squared.com/))自己来定义形状和大小,使用起来和sprite差不多,比较方便.
## 2.6 简单的TileMap
         Cocos2d支持的是一种TMX(TileMap XML)格式的地图,该地图可以由TileMapEditor生成([http://www.mapeditor.org/),TMX](http://www.mapeditor.org/%29,TMX)地图有以下特点
a.      支持正交(orthogonal),六边形(hexagonal),45度等距(isometric)地图
b.      支持对象组,对象,和属性
和地图相关的类如下:
![](http://hi.csdn.net/attachment/201109/13/0_1315882518MFgH.gif)
a.      以上类图是简化后的,实现的类还包含了objectgroup,property,TileSetInfo等
b.      一个TiledMap拥有多个layer
c.      一个layer拥有多个tile(用sprite来表示)
d.      由于每个tile都被看作一个sprite对象,因此每个tile可以被独立的,移动,旋转,缩放甚至执行action等.
e.      layer保存了一个对应于tileid的二维数组,只有在需要的时候才会根据tile id 创建相应的tile(sprite),这样可以既可以节省存储空间,也可以提高渲染效率.
f.       Tile可以在运行时被添加和删除 
g.      [http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps](http://www.cocos2d-iphone.org/wiki/doku.php/prog_guide:tiled_maps)网页上有关于tileMap使用的详细教程
## 2.7 游戏逻辑的支持
         前面已经讲过Cocos2d不仅仅只是一个2D渲染引擎,它还提供了一个支持游戏逻辑的系统,首先所有可见的对象,场景和layer都以场景树的数据结构来保存,对root(一般都是scene)执行的操作都会影响root的孩子节点.如果root被缩放或者平移,那么root的所有孩子节点也会被缩放和平移.所有的节点类(CCNode)都继承了一个接口 SelectorProtocol,该接口有一系列的回调函数,在特定的条件下回调来完成相应的逻辑处理.其中最基本也是最重要的是与schedule相关的回调接口.下面将介绍一下schedule任务系统
### 2.7.0 Schedule系统
         Scheule系统也叫任务系统或者事件系统,任务指一个时间片内定时发生的事件.在cocos2d中的CCNode可以订制一个计划任务.通过回调函数来完成一个特定的逻辑.相关的类图如下
![](http://hi.csdn.net/attachment/201109/13/0_1315882570A73O.gif)
a.      CCSchedule可以将selectorProtocol接口对象添加到任务列表中.
b.      CCTimer每一帧都检查对应的任务是否达到执行时间点,如果达到则执行回调函数.
c.      CCNode派生于SelecotrProtocol,根据需要重载实现回调函数,实现相应的逻辑
### 2.7.1 Action介绍
         Action就像一些可以被执行的命令,通常情况下,一个action会修改一个CCNode对象的一些属性,让一个CCNode对象执行一些动作,例如改变其的位置,朝向.让一个Sprite播放动画等.如果这个action是在一定时间段内完成的,我们称它为IntervalAction,如果是立即执行的,我们称它为InstantAction.例如CCMoveBy在一定时间段内将一个CCNode对象移动一定的距离.因此它是IntervalAction的子类.而CCCallFunc实现立即执行一个架回调函数.因此它是InstantAction的子类.
         Action的设计采用了组合模式和装饰模式,可以将一系列的action组合起来,让CCNodec对象执行,就像给CCNode下达一系列的命令一样.例如以下代码
CCActionInterval* move =CCMoveBy::actionWithDuration(0.5f, ccp(0,160));
         CCActionInterval*rotate = CCRotateBy::actionWithDuration(2, 360);
         CCActionInterval*scale = CCScaleBy::actionWithDuration(2, 5);
         CCActionInterval*opacity = CCFadeOut::actionWithDuration(2);
         CCActionInterval*fadein = CCFadeIn::actionWithDuration(2);
                  CCActionInterval* scaleback =CCScaleTo::actionWithDuration(1, 1);
CCFiniteTimeAction*seq0 = CCSequence::actions(move, rotate, scale, opacity, fadein, scaleback,NULL);
Actor->runAction(seq0);
         该代码解释如下:
a.      Actor在0.5秒之内向y方向移动160个单位,
b.      让actor在2秒内,旋转360度
c.      将actor2秒内放大5倍
d.      2秒内淡出
e.      2秒内淡入
f.       1秒之内缩放到原始大小
## 2.8 网络支持
         Cocos2d没有提供专门的网络库
## 2.9 脚本支持
         Cocos2dX最近的版本提供了lua脚本支持,在1.1.0版本结合了tolua++,基本上将所有类的成员函数都导出到脚本中.除此之外,还增加了一些脚本回调函数,例如菜单的消息处理可以通过执行一段脚本来完成.也可以添加计划任务来回调脚本函数.cocos2d中虽然有一个简单的实例来说明如何使用lua,但具体的效率如何还待进一步验证
## 2.10 声音引擎支持
         Cocos2d使用了一个简单的声音引擎,支持Wav,mp3,ogg,caf格式音频文件播放.但该引擎没有提供音量设置
## 2.11 物理引擎支持
         Cocos2d可以和box2d,chipmunk物理引擎结合在一起,实现碰撞,重力等物理效果模拟.在代码的实例中,有大量与box2d和chipmunk结合的实例,可以供学习和研究使用.
## 2.12 cocos2d的缺点
         Cocs2d不但是处理了2D渲染,还将整个渲染系统和游戏逻辑紧密的耦合在一起,因此在使用coco2d渲染时,不得不使用他的整个逻辑相关的代码.因此很难在使用其渲染库的同时又自己设计一套逻辑框架.对于一些小型游戏来说,可以使用其可以进行快速的迭代和开发.但对于一些大型游戏,为了使用系统运行更稳定,结构更清晰.更易于扩展,一般都会将各个模块独立开来.以方便不同的人员分工开发和维护
**位图字体工具Bitmap Font Tools**[BMFont](http://www.angelcode.com/products/bmfont) (Windows) 
[Fonteditor](http://code.google.com/p/fonteditor)
[Glyph Designer](http://glyphdesigner.71squared.com/)
[Hiero](http://slick.cokeandcode.com/demos/hiero.jnlp)
[LabelAtlasCreator](http://www.cocos2d-iphone.org/forum/topic/4357)
**粒子编辑工具Particle Editing Tools**
[ParticleCreator](http://www.cocos2d-iphone.org/forum/topic/16363)
[Particle Designer](http://particledesigner.71squared.com/)
**物理编辑工具Physics Editing Tools**
[Mekanimo](http://www.mekanimo.net/)
[PhysicsBench](http://www.cocos2d-iphone.org/forum/topic/9064)
[PhysicsEditor](http://www.physicseditor.de/)
[VertexHelper](http://www.cocos2d-iphone.org/archives/779)
**场景编辑工具Scene Editing Tools**
[CocosBuilder](http://cocosbuilder.com/)
[Cocoshop](http://www.cocos2d-iphone.org/forum/topic/15668)
[LevelHelper](http://www.levelhelper.org/)
**纹理地图集工具Texture Atlas Tools**
[DarkFunction Editor](http://darkfunction.com/)
[SpriteHelper](http://www.spritehelper.org/)
[TexturePacker](http://www.texturepacker.com/)
[Zwoptex](http://zwoptexapp.com/)
**瓦片地图编辑工具Tilemap Editing Tools**
[iTileMaps](http://www.klemix.com/page/iTileMaps.aspx)
[Tiled Map Editor](http://www.mapeditor.org/)
**声音特效编辑工具Audio Effect Editing Tools**
[cfxr](http://thirdcog.eu/apps/cfxr/)
**背景音乐编辑工具Background Music Editing Tools**
[GarageBand](http://itunes.apple.com/cn/app/garageband/id408980954?l=en&mt=12/) (MAC 系统自带)
**BOX2D多边形定义工具VertexHelper**
[VertexHelper](https://github.com/jfahrenkrug/VertexHelper/downloads)
**GIF动画帧导出工具Ulead GIF Animator**
[UleadGifAnimator](http://www.crsky.com/soft/4010.html)
