# cocos - LC900730的博客 - CSDN博客
2017年06月23日 11:59:14[lc900730](https://me.csdn.net/LC900730)阅读数：153标签：[游戏开发](https://so.csdn.net/so/search/s.do?q=游戏开发&t=blog)
个人分类：[cocos2dx](https://blog.csdn.net/LC900730/article/category/6987521)
导演：CCDirector，游戏开始和结束都需要调用CCDirector的方法完成游戏初始化和销毁工作。提供一些管理场景的方法，实现场景中的工作。 
场景：CCScene：存放的是需要渲染的布景、人物和菜单，可以作为一个整体被切换使用等 
图层：CCLayer，一个场景可以由很多图层，每一层负责各自的任务，如专门显示颜色等； 
精灵：CCSprtite，是整个游戏开发处理的主要对象，天上的飞机、地上的坦克、玩家控制的人物等都是精灵。从技术上讲，精灵就是一个可以不断变化的图片。这些变化包括位置、旋转、放大、缩小和运动等。 
动作：CCAction，主要是配合精灵使用，将精灵绑定相应动作。 
镜头：CCCamera，每一个结点(CCNode)都需要使用CCCamera。当节点发生缩放、旋转和位置变化，都需要覆盖CCCamera，让这个节点通过CCCamera重新渲染 
锚点：在cocos是比例坐标，如锚点(0.3,0.5) 
那就是矩形宽的1/3为0.3，高的1/2为0.5
每秒钟刷新场景的次数 
(1.0/60)就是1s刷新60次
## 节点
创建节点：Node * childNode=Node::create(); 
增加节点：node->addChild(childNode,0,123) 
第三个参数Tag其实是一个整数，为每一个对象分配一个id，通过tag来对这个对象进行操作 
查找子节点：Node * node=node->getChildByTag(123); 
node->removeChildByTag(123,true);删除子节点，并删除子节点一切动作； 
node->removeChild(childNode)通过node指针删除节点 
node->removeAllChildrenWithCleanup(true):删除所有子节点，并且停止这些子节点上的一切操作； 
node->removeFromParentAndCleanup(true):从父Node结点删除
## 结点中2个重要属性：位置与锚点
### 1.anchorPoint
锚点是一个相对位置，一般是0-1之间 
锚点左下角(0,0) 
右上角(1,1) 
x的2/3,y的1/2那么锚点就是(0.66,0.5);
### 1.anchorPoint
每一个游戏程序都有一个循环在不断运行，它是由导演对象来管理维护。我们可以在游戏循环中使用定时器来对精灵对象进行调度，因为Node类封装了Schedule类，我们也可以直接使用Node调用函数。
void schduleUpdate(void)每个Node对象只要调用该函数，那么这个Node对象就会定时每帧回调一次自己的update(float dt)函数
void schduleUpdate(SEL_SCHEDULE selector,float inerval) 
与schduleUpdate不同的是，我们可以指定回调函数(通过selector指定)，也可以更加需要指定回调时间间隔
void unschdule(SEL_SCHEDULE selector) 
可以指定具体函数停止调度
void unscheduleAllSelector(void) 
可以停止调度
virtual函数
## Node结点类
Node继承自Ref，Ref是一个内存管理的类。 
Node类在cocos-2d很重要，任何需要画在屏幕上的对象都是结点类，最常用的包括场景类(Scene)、布景层类(Layer)精灵类(Sprite)菜单类(Menu)
结点
- 1.每个节点都可以包含子节点
- 2.节点都可以包含调度方法(schedule,unschedule等)
- 3.节点可以包含动作
1.当需要一个父节点来管理很多子节点，我们可以添加一个Node类作为父节点，从而管理这些子节点； 
2.定义我们自己的Node类，让Node类成父类，我们可以重写Node类中的虚方法，从而我们实现我们需要 的效果 
如： 
重写初始化的方法和周期性回调方法； 
重写渲染的绘制方法
## Node提供的函数
1.节点属性 
2.节点操作 
3.动作相关Action 
4.定时器schedule 
5.整合NodeRBGA类 
6.查找子节点enumerateChildren 
7.渲染顺序zOrder 
8.坐标转换
