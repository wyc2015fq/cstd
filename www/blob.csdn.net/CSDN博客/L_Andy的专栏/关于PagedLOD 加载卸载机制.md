# 关于PagedLOD 加载卸载机制 - L_Andy的专栏 - CSDN博客

2018年08月06日 16:56:43[卡哥](https://me.csdn.net/L_Andy)阅读数：227


分页对于大型场景而言是一个必不可少的调度渲染技术。当你发现osg自身就带有PagedLOD 功能时，何尝不兴致冲冲的立即使用。可是，在使用时突然发现只有加载没有卸载，内存随着运行不断攀升，过期的PagedLOD 节点竟然没有被osg踢出内存？苦恼啊，纠结啊？难道是bug？其实不然，osg的PagedLOD 技术已十分成熟，那么究竟是何原因造成的？焦躁的你请耐心阅读

1、导致PagedLOD 不被卸载的第一个原因

在使用osgViewr::Viewer::setSceneData设置场景给Viewr之前，没有把你的PagedLOD 节点加入到场景根节点下。考虑如下两种情况：

```
osg::Group * root=new osg::Group;
osg::PagedLOD * lod1=new osg::PagedLOD;
lod1->setFileName(0,"cow.osg");　　//添加子节点
lod1->setRange(0,0,10);　　　　　　　　//设置子节点显示范围

root->addChild(lod1);
viewer->setSceneData(root /*createTeapot()*/ );
```
- 

```cpp
osg::Group * root=new osg::Group;
osg::PagedLOD * lod1=new osg::PagedLOD;
lod1->setFileName(0,"cow.osg");
lod1->setRange(0,0,10);

viewer->setSceneData(root /*createTeapot()*/ );
root->addChild(lod1);
```

两种情况下lod1节点都会在过期时删除吗？

不是！

只有第一种情况中的PagedLOD 会在过期时从内存中卸载。

说起原因，就不得不告诉你一个setSceneData函数的小秘密。看下面代码：

```cpp
void Scene::setSceneData(osg::Node* node)
{
_sceneData = node;

if (_databasePager.valid())
{ 
// register any PagedLOD that need to be tracked in the scene graph
if (node) _databasePager->registerPagedLODs(node);
}
}
```

在setSceneDAta时向DatabasePagerr注册了场景中的所有PagedLOD 节点。

如果此时场景中还没有设置PagedLOD 节点，那么很抱歉，即使你在随后设置了，DatabasePager中注册的PagedLOD 节点依然是当时注册的那些，你随后设置的只能是计划外人口，黑户，对不起DatabasePager是不管这些黑PagedLOD 的死活。

既然DatabasePager不管这些PagedLOD 节点的死活，那么为什么这些PagedLOD 节点可以被动态加载呢？

因为PagedLOD 节点的动态加载请求是PagedLOD 节点自身发出，而卸载则是由DatabasePager管理！请查看PagedLOD 的traverse函数，它会告诉你一切。

讲到这里，也许你已经心中有数，使用PagedLOD 节点时应该预先设置然后再setSceneData。

如果我想在运行过程中添加PagedLOD 节点又想让其可以卸载该怎么办呢？

答案就是改变时请注意调用DatabasePager::registerPagedLODs(node);函数注册户口。

**注意**：这种错误在新版本中已经没有了。

2、导致PagedLOD 节点不能卸载的第二个原因：

如果你使用PagedLOD 节点完全避免1中提到的问题，但是你的PagedLOD 节点依然顽固的赖在内存中不走，那么请你看看这里。

我们知道分页的功能与内存有关，那么内存不够时PagedLOD 节点就应该自动退出内存才是，为什么他还赖在那里呢？

答案是，他并不知道内存不够用了，需要你告诉他！DatabasePager::setTargetMaximumNumberOfPageLOD函数或环境变量OSG_MAX_PAGEDLOD就是干这个的。他告诉DatabasePager我的电脑内存有限只能容纳指定数量的PagedLOD ，超出这个数的过期PagedLOD 就让他滚蛋吧。

也许你会问PagedLOD 个数和内存使用情况有个毛关系？

确实有关系，在你进行PagedLOD 分页规划时你的PagedLOD 节点的每一级都有一定的大小，那么这个大小和个数相乘就是要占用的总内存。

值得一提的是osg中此数默认数量是300，osg觉得你的电脑配置很高！

