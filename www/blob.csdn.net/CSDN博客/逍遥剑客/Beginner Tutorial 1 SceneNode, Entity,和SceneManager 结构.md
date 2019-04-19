# Beginner Tutorial 1: SceneNode, Entity,和SceneManager 结构 - 逍遥剑客 - CSDN博客
2007年08月19日 16:33:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3971标签：[vector																[dll																[网格																[tutorials																[textures																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=textures&t=blog)](https://so.csdn.net/so/search/s.do?q=tutorials&t=blog)](https://so.csdn.net/so/search/s.do?q=网格&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)

**先决条件**
这个教程假设你有C++编程的基础并且可以配置并编译OGRE应用程序 (如果你在配置环境方面有问题,请看**[OGRE + MinGW + Code::Blocks环境的搭建](http://blog.csdn.net/xoyojank/archive/2007/08/14/1742477.aspx)**). 除了配置环境之外,你不需要有任何关于OGRE的知识. 
**介绍**
在这个教程中,我会介绍给你OGRE中最基本的结构: SceneManager, SceneNode, 还有Entity 对象.我们不会涉及大量的代码,而是观注初学OGRE的一些一般概念. 
当你在阅读的过程中,你会慢慢地在你的工程中加入代码并且观察生成的结果.没有比实际的编程更好的方法来熟悉这些概念!所以不要只阅读不动手. 
**开始**
**最初的代码 **
在这个教程里我们会用到一个预先构造的代码结构.除了我们将要加入createScene 方法的,你应该忽视其他的代码. 在随后的教程中,我们会深入地讲解OGRE是怎么运行的,但是现在我们要从最基本的层次开始.用你选择的环境创建一个工程,并加入下面的代码: 
#include "ExampleApplication.h"
class TutorialApplication : public ExampleApplication
{
protected:
public:
    TutorialApplication()
    {
    }
~TutorialApplication()
    {
    }
protected:
void createScene(void)
    {
    }
};
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char**argv)
#endif
{
// Create application object
    TutorialApplication app;
try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s ",
                e.getFullDescription().c_str());
#endif
    }
return0;
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/ogre_t1_1.JPG)
当这个程序运行的时候,用WASD来进行移动,鼠标进行视角的旋转,ESC键退出(好像CS啊). 
**问题分析**
如果你遇到了问题,检查你是否配置好了环境,或者在Ogre.log中查看更详细的信息. 如果你需要更多的帮助,搜索一下[论坛或发贴询问.在这之前你要阅读发贴规则并把Ogre.log,异常信息,错误消息,调试信息等尽量详细的给出来.](http://bbs.gameres.com/showforum.asp?forumid=99),
**Message Box ****问题 **
如果你使用代有Unicode支持的VisualStudio,那么可能会在MessageBox()这里报错,解决办法有两种:一是把MessageBox改为MessageBoxA,另一种就是在工程设置里去除Unicode支持.
**缺少配置文件或DLL **
如果你在运行刚编译好的程序的时候出现缺少DLL或配置文件(*.cfg),确定你设置好环境路径 [ProjectFolder]/bin/release, [ProjectFolder]/bin/debug. 
**资源或插件问题 **
确定plugins.cfg和resources.cfg跟可执行文件在同一目录下(或在bin目录下). Plugins.cfg告诉OGRE 哪个渲染库可用 (Direct3D9, OpenGL等). Resources.cfg是 ExampleApplication 的资源配置文件,里面有textures, meshes 和 scripts的路径. 这两个都是文本文件,所以编辑它们并保证路径的正确性. 否则你的OGRE设置对话框会没有任何渲染方式可选,或者你会在屏幕上或Ogre.log中看到: 
Description: ../../Media/packs/OgreCore.zip - error whilst opening archive: Unable to read zip file
如果发生这种情况,打开resources.cfg 文件并把路径指向OGRE的Media文件夹. 注意你不能使用任何环境变量,如 $(SomeVariable). 
**在Visual Studio中不能启动应用程序 **
一般情况是环境配置问题,使得有些文件找不到所致.详细解决办法不在本文讨论.
**Ogre****怎样工作 **
这是一个主要的话题. 我们从SceneManagers开始直到Entities和SceneNodes.这三个类是构建Ogre应用程序的基础. 
**SceneManager ****基础 **
所有出现有屏幕上的东西都是由SceneManager(场景管理器)来管理的 (想像一下). 当你在场景中放置物体的时候, SceneManager记录了它们的位置. 当你创建Camera来观察场景 (我们会在后面的教程中提到)时, SceneManager 记录了它们. 当你创建plane, billboard, light...等等时,SceneManager 也记录了它们. 
有很多种SceneManager.有渲染地形的,有渲染BSP地图的, 等等. 你可以在[这里查看各种不同类型的SceneManager. 在进行这些教程的过程中,我们会涉及到其它的SceneManager.](http://www.ogre3d.org/wiki/index.php/SceneManagersFAQ)
**Entity ****基础 **
一个 Entity(实体)是一种你能够在场景中绘制的对象. 你可以把Entity想像成3D mesh的替代品. 一个机器人是一个entity, 一条鱼是一个entity, 你的角色走在的地形也是一个巨大的entity. Light(灯光), Billboard(公告牌), Particle(粒子), Camera(摄像机)等就不是entity 了. 
一个在Ogre中需要注意的问题是它把可渲染的对象跟它们的位置与方向进行了分离. 这意味着你不能直接把一个Entity放到场景中去. 所以你必须把Entity 附属于一个SceneNode 对象, 这个SceneNode包含了位置跟方向的信息. 
**SceneNode ****基础 **
上面已经说过, SceneNode(场景结点) 记录了所有附属于它的物体的位置和方向. 当你创建一个Entity的时候,在你把它绑定在一个SceneNode之前,它是不会被绘制的. 相似的, 单独一个SceneNode 不是可以在屏幕上显示的物体. 只有你创建 了一个SceneNode并且在上面绑定一个Entity (或其他对象),它才能真正地在屏幕上显示出来. 
SceneNode可以绑定很多个对象. 比如说你有一个角色在屏幕上走动,而且你希望他周围有灯光. 实现这个的方法是你首先创建一个SceneNode, 再创建一个角色Entity 并绑定在SceneNode上. 接下业你要创建一个Light对象并把它绑定于SceneNode. SceneNode也加以绑定在另外的SceneNode上,这样就使我们可以创建有层次关系的结点了. 我们会在之后的教程中涉及SceneNode的高级配属方式. 
U关于ScenenNode 有一个重要的方面要注意,就是
SenceNode的位置总是相对于它的父级SceneNode的, 并且每个SceneManager 都包含一个根结点来绑定所有其他的SceneNode. 
**你的第一个Ogre应用程序 **
现在回到我们之前创建的代码. 找到TutorialApplication::createScene 成员函数. 在这次的教程中我们只会利用这个函数的内容. 要做的第一件事就是为场景设置环境光源使我们能看到我们在做的事情. 我们可以调用[setAmbientLight](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#Ogre_1_1TerrainSceneManagera89) 函数并指定颜色来完成这件事. 注意ColourValue的构造函数期望的红绿蓝各分量的取值范围是0到1. 在createScene中加入这一行: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );
接下来的我们需要做的是创建一个Entity. 我们可以调用SceneManager的 createEntity成员函数: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Entity *ent1 = mSceneMgr->createEntity( "Robot", "robot.mesh" );
好了,出现了几个问题. 首先, mSceneMgr是从哪里来的?还有我们调用的函数的参数是什么? mSceneMgr变量包含在当前的SceneManager对象中 (继承自ExampleApplication 类). createEntity的第一个参数是我们创建的Entity的名字. **所有entity必须有一个唯一的名字.** 如果你尝试创建两个同名的entity,你会收到一个错误. "robot.mesh"参数指定了我们为Entity使用的mesh(网格). 同样的, 这个网格已经被ExampleApplication类预先加载了. 
既然我们创建了一个, 我们需要创建一个SceneNode来绑定. 因为每个SceneManager都有一个根SceneNode, 我们要创建那个结点的子结点: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode" );
这个长长的语句首先调用了当前SceneManager的getRootSceneNode. 然后又调用了SceneNode的createChildSceneNode方法. createChildSceneNode 的参数是我们创建的SceneNode的名字. 就像Entity类一样, 两个SceneNodes 不能有相同的名字. 
最后, 我们需要把Entity 绑定在SceneNode上来使机器人有一个绘制的位置: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node1->attachObject( ent1 );
好了! 编译并运行你的程序. 你应该在屏幕上看到一个站立的机器人. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/ogre_t1_2.JPG)
注意: Robot.mesh 不在OgreCore.zip中. 跟着这个教程来做到这个时候,你的程序可能能运行但没有显示任何东东. 我发现在resources.cfg 加入适当的路径会使程序正常运行. 
FileSystem=../../media/materials/programs
FileSystem=../../media/materials/scripts
FileSystem=../../media/materials/textures
FileSystem=../../media/models
**坐标系和向量 **
在我们深入之前, 我们需要说一下屏幕坐标系和Ogre向量对象. Ogre (像很多其他图形引擎一样) 使用x和 z轴来做为水平面, y轴做为垂直轴. 当你看向屏幕的时候, x轴应该是从左到右的, 并且右侧是x轴的正方向. Y轴在屏幕是从下到上的,上面那一端是正方向. Z轴是从里到外的,屏幕外的这一端是正方向. 
为什么机器人面朝x轴轴的正方向呢? 那是网格自己的属性,因为原来它就是那个样子. Ogre 并没有设定你的模型的朝向.你加载的每个网格都会有不同的方向. 
Ogre 使用Vector类来表示位置和方向(没有Point类). 向量有2 (Vector2), 3 (Vector3), 还有 4 (Vector4) 维的, 其中Vector3是最常用的. 如果你对向量不熟悉的话, 我建议你在认真学习Ogre之前复习一下. 与向量相关的数学知识在你进行复杂程序的时候是非常有用的. 
**加入其它的物体 **
现在你理解了坐标系统,我们可以回到我们的代码上来了. 在我们写的那三行代码中,并没有设置机器出现的位置. 绝大多数Ogre的函数都有参数的默认值. 例如,Ogre中的 [SceneNode::createChildSceneNode](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea26) 成员函数有3个参数: SceneNode的名字, SceneNode的位置, 还有初始的方向. 就像你看到的一样,位置已经被设为 (0, 0, 0)了. 我们来创建别一个SceneNode, 但是这次我们指定位置:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Entity *ent2 = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node2->attachObject( ent2 );
这看起来很眼熟. 我们跟之前做了一件相同的事, 除了两点. 首先, 我们对Entity 和SceneNode的命名有点差别. 第二点就是我们指定位置在x轴离根SceneNode 50单位的距离上 (记住所有的SceneNode的位置是相对于父结点的). 编译并运行这个demo. 现在有两个并排的机器人了. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/ogre_t1_3.JPG)
**深入了解Entity **
Entity类的应用非常广泛,这里我不会涵盖所有的使用方法...仅仅够你入门就好了. 我愿意指出在Entity中有一些可以直接使用的函数. 
首先是 [Entity::setVisible](http://www.ogre3d.org/docs/api/html/classOgre_1_1MovableObject.html#Ogre_1_1WireBoundingBoxa24) 和 [Entity::isVisible](http://www.ogre3d.org/docs/api/html/classOgre_1_1MovableObject.html#Ogre_1_1WireBoundingBoxa25). 你可以通过调用这个函数来设置任何一个Entity可见. 如果你想暂时隐藏一个Entity,这时候调用这个函数比销毁它再重建要好得多. 注意你不需要缓存这些Entity. 每个物体的网络和纹理只会载入内存一次,所以当你试着保存它们的时候并没有省下多少资源. 唯一节省的是你创建和销毁Entity对象所花费的那一点点时间而已.
[getName](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa21)函数返回的是Entity的名字, [getParentSceneNode](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea38) 返回的是Entity绑定到的SceneNode. 
**深入了解SceneNode **
SceneNode 非常复杂. 很多事情都可以通过SceneNode来完成,但我们只涉及最常用的那些. 
你可以通过[getPosition](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa29) 和 [setPosition](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa27)来得到和设置SceneNode的位置(总是相对于父级SceneNode的). 你可以用[translate](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa37)方法相对于当前位置移动它. 
SceneNode 不仅设置位置,还可以进行物体的放缩和旋转. 你可以通过[scale](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa35)函数来设置物体的比例.你可以用 [yaw](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea30), [roll](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa44), 和 [pitch](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa45) 函数来旋转物体. 你可以调用[resetOrientation](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa26) 重置物体的方向. 你还可以使用[setOrientation](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa24), [getOrientation](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa23), 和 [rotate](http://www.ogre3d.org/docs/api/html/classOgre_1_1Node.html#Ogre_1_1TagPointa44) 函数来进行更高级的旋转. 在最后部分的教程之前我们不会涉及到Quaternion(四元数). 
你已经见过[attachObject](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea3)函数了. 如果你想操作绑定到SceneNode上的对象的话,这些相关的函数也很有用: [numAttachedObjects](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea4), [getAttachedObject](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea5) (这个函数有很多版本), [detachObject](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea7) (同样是多个), [detachAllObjects](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneNode.html#Ogre_1_1SceneNodea10). 还有一个完整的函数集来处理父子结点. 
因为所有的位置和移动都是相对于父结点的, 我们可以非常方便地一块儿移动两个SceneNode.我们现在程序中有这些代码: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Entity *ent1 = mSceneMgr->createEntity( "Robot", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node1->attachObject( ent1 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Entity *ent2 = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node2->attachObject( ent2 );
如果我们把第5行从: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
改成这样: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node2 = node1->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
这样我们就把RobotNode2变为 RobotNode的子结点了. 移动node1 时node2也会随着移动, 但是移动node2不会影响到node1. 例如这些代码可以只移动RobotNode2: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node2->translate( Vector3( 10, 0, 10 ) );
下面的代码会移动RobotNode, 同时由于RobotNode2 是RobotNode的子结点, RobotNode2也会移动:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node1->translate( Vector3( 25, 0, 0 ) );
如果你弄不清, 最简单的方法就是从根结点开始向下. 像这样, 我们把node1 从 (0, 0, 0)开始移动 (25, 0, 0), 从而node1的位置相对于父级变为 (25, 0, 0). node2从 (50, 0, 0)开始移动(10, 0, 10), 所以它相对于父级的新位置是(60, 0, 10). 
现在我们计算出这些东西真正的位置. 从根SceneNode开始. 它是位置总是(0, 0, 0). 现在, node1的位置是(root + node1): (0, 0, 0) + (25, 0, 0) = (25, 0, 0). 这并不意外. node2 是node1的子结点, 所以它的位置为(root + node1 + node2): (0, 0, 0) + (25, 0, 0) + (60, 0, 10) = (85, 0, 10). 这也是一个解释SceneNode的层级结构的好例子. 你很少会需要计算你的结点的绝对位置的. 
最后, 注意你可以通过调用SceneManager 的[getSceneNode](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#Ogre_1_1TerrainSceneManagera82) 和 [getEntity](http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html#Ogre_1_1TerrainSceneManagera85) 方法得到SceneNode和Entity, 所以你不必为你创建的每个SceneNode保存一个指针. 但你可以保存你最常使用的那个. 
**尝试一下 **
到现在你应该基本掌握了Entitiy, SceneNode, 和SceneManager了. 我建议以上面的代码为基础在场景中加入或删除机器人. 当你完成的时候,清空所有createScene方法中的内容, 并玩玩下面的代码段: 
**缩放 **
你可以通过调用SceneNode中的scale方法来缩放网格. 试着更改scale的参数值来看一下你能得到什么: 
**旋转**
你可心用yaw,pitch和roll方法来旋转对象，并且roll方法可以用度或者弧度单位. Pitch是绕x轴的旋转, yaw是绕y轴的旋转, roll是绕z轴的旋转. 用你的右手来做一下示范: 把你的拇指、食指跟中指各成90度，然后拇指向右为x轴，做一下各种旋转试一下. 
试着改变角度，并结合多种变换: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Entity *ent = mSceneMgr->createEntity( "Robot", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode", Vector3( -100, 0, 0 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->attachObject( ent );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->yaw( Degree( -90 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        ent = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->attachObject( ent );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->pitch( Degree( -90 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        ent = mSceneMgr->createEntity( "Robot3", "robot.mesh" );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode3", Vector3( 100, 0, 0 ) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->attachObject( ent );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        node->roll( Degree( -90 ) );
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/ogre_t1_4.JPG)
**Ogre****环境 **
在这个教程中引用的大多数的文件 (.DLL and .CFG) 可以在OgreSDK 的"bin" 下的debug 或 release中找到.你创建的 Debug 程序应该使用debug文件夹中的文件, release程序应该使用release文件夹中的. 
注意这里讨论了许多特定的Windows下的术语. Linux下也一样，不过共享库文件以.so结尾并在其它的地方，有些东西会有细微的不同. 如果你有问题，可以发布到Ogre论坛help版块上去. 
**DLL****和插件 **
现在我们已经接触了一些Ogre环境, 我愿意大体上讲解一下Ogre库是怎么工作的, 还有怎么更方便的去应用它. 
Ogre被分为几组共享的库文件. 第一组是链接库本身还有它依赖的库. Ogre库 在OgreMain.dll中包含着. 这个dll需要一些其它的链接库，如cg.dll. 这些DLL必须毫无例外地包括在每个Ogre应用程序中. 
第二组链接库是插件. Ogre把各种功能合理地分配到链接库中去，因些它们可以根据你的程序的需要来加入. Ogre中基本的插件的文件名以"Plugin_" 开头. 如果的程序需要的话，你可以创建新的插件,但是我们不会在教程中涉及这个. Ogre同样使用插件来进行渲染(如OpenGL, DirectX, 等).这些插件以"RenderSystem_"为前缀. 它们已经可用了，你可以在你的程序中加入或移除它们.当你写OpenGL着色器或特殊应用的并且希望关闭DirectX下运行的能力的时候，这是非常有用的, 你可以简单地移除对应的RenderSystem插件，这样它就会不可用了. 另外，如果你的目标平台是非标准的, 你可以编写你自己的RenderSystem 插件, 但这个会不在教程中涉及到. 我们会在下一个部分提到怎样移除插件. 
第三组链接库是第三方库和其它有所帮助的库. Ogre它本身仅仅是一个图形渲染库. 它并没有包含一些东西，如GUI 系统, 输入控制, 物理引擎, 等等. 你必须用其它的链接库来做这些事情. Ogre demos和SDK包含了一些这样的第三方库. CEGUI 库是一个可以方便地与Ogre进行整合的GUI 系统, 它的DLL以 "CEGUI*"开头并且"OgreGUIRenderer.dll"是它的一部分. CEGUI的使用会在之后的教程中涉及到. 键盘和鼠标输入是通过OIS(一个输入系统)来完成. 它包含在OIS.dll中. 还有一些库(没有包含在SDK中)可以提供更多的功能 (如声音和物理引擎)，你可以在Wiki和论坛上找到更多的信息. 
这些话的用意是当你在本也测试你的程序时, 你可以把所有的东西都"打开" 就是不要移除任何东西). 当你准备发布的你的程序时，你需要在Release模式下生成它,包含所以你的程序用到的release DLL, 并且移除你用不到的DLL. 如果你的程序没有使用CEGUI但是用到了OIS, 那么你不应该包含CEGUI的DLL, 但是必须包含OIS dll，否则你的程序不会正常运行
**配置文件 **
Ogre 有许多配置文件. 它们控制加载哪些插件, 从哪里加载程序资源, 等等. 我看来简单地看一下各个配置文件是干什么的. 如果你有更多的细节问题,你应该直接去Ogre的帮助论坛. 
**plugins.cfg**这个文件包含了你的程序使用的插件. 如果你想加入或去除某个插件, 你需要编辑一下这个文件. 要去除一个插件, 只删除对应的行或在开头加#注释掉就好了. 要加入一个插件的话, 你需要像这样加入一行:"Plugin=[PluginName]". 注意别忘了在插件名称的结尾加上 .DLL. 你的插件并不强制以"RenderSystem_" 或 "Plugin_"开头. 你也可以通过更改"PluginFolder"变量定义Ogre寻找插件的目录. 你可以使用绝对或相对路径, 但是不能使用环境变量,如$(SomeVariable). 
**resources.cfg**这个文件包含了Ogre查找资源的路径列表. 资源包括脚本, 网格, 纹理, 等等. 你可以使用绝对或相对路径, 但是不能使用环境变量,如$(SomeVariable). 注意Ogre不会去扫描子目录,所以如果你有多级目录的话,你必须手动的加入它们. 例如, 你有这样的一个目录树,"res/meshes" 和 "res/meshes/small", 你必须在配置文件加入两行包含这些路径的条目. 
**media.cfg**这个文件告诉Ogre 更详细的资源信息. 好像现在你没必要编辑这个文件, 所以我将跳过这些细节. 更多的信息可以在用户手册和Ogre论坛上找到. 
**ogre.cfg**这个文件是由Ogre的配置对话框生成的. 这个文件可以设置你的计算机和图形选项. 你不应该在共享你的程序的时候把这个文件发行给其他人, 因为他们的设置可能会跟你不同. 注意你不该直接编辑这个文件,而是应该使用配置对话框. 
**quake3settings.cfg**这个文件是由BSPSceneManager使用的. 你不需要这个文件,除非你用到了这个scene manager (现在你就没有用到), 所以忽略它. 你不该发由这个文件,除非,你使用了BSPSceneManager, 即使那时你的程序决定了它可能跟现在完全不同. 
上面这些就是Ogre直接操作的配置文件. Ogre必须能够找到"plugins.cfg", "resources.cfg", 和 "media.cfg" 才能正常运行. 在后面的教程中我们会涉及到怎么改变这些文件的位置并操作他们完成更多高级功能. 
**更好的方法来测试你的程序**
*注意这部分是关于Windows和Visual C++的(xoyojank:所以我只大体提一下吧)*
上面说过你要把bin文件夹的路径加入工程设置,不然程序会找不到DLL之类的文件.比起每次都把DLL拷贝到程序目录下来说,直接复制到Windows/System32/下更方便一些. (略)
**总结 **
到这里你应该对SceneManager, SceneNode, 和 Entity 类有个初步的掌握了. 你不必熟悉所有我介绍过的函数. 既然它们都是最基本的对象, 我们会经常使用的. 你会在接下来的教程中对它们更加了解的. 
你也应该对配置Ogre环境有所熟悉了. 
进行 [Basic Tutorial 2](http://www.ogre3d.org/wiki/index.php/Basic_Tutorial_2)*摄像机, 灯光, 和阴影*
终于翻译完啦.
其实OGRE 3D 中文([http://ogre3d.cn](http://ogre3d.cn/))上已经有人翻译过了,只是去掉了一些东西.我翻译这个,也是想对自己有所锻炼吧.希望继续学习的,可以参照:
[http://ogre3d.cn/wiki/index.php?title=%E6%96%87%E6%A1%A3:%E6%95%99%E7%A8%8B:%E5%9F%BA%E7%A1%80%E6%95%99%E7%A8%8B](http://ogre3d.cn/wiki/index.php?title=%E6%96%87%E6%A1%A3:%E6%95%99%E7%A8%8B:%E5%9F%BA%E7%A1%80%E6%95%99%E7%A8%8B)
[http://www.ogre3d.org/wiki/index.php/Ogre_Tutorials](http://www.ogre3d.org/wiki/index.php/Ogre_Tutorials)
如果你用的是Windows下的OgreSDK, 一定要把这个目录 "[OgreSDK_DIRECTORY]/samples/include" 加入工程的包含路径( ExampleApplication.h 文件从这个位置加载). 如果用的是OGRE的源代码版本,你需要包含"[OgreSource_DIRECTORY]/Samples/Common/include" 目录. 确认你在继续下一部分之前你可以编译并运行这些代码,虽然除了一个有帧速度的框的空白屏幕外没有任何东西.我们会在这个教程后面加入一些能够显示的对象.
