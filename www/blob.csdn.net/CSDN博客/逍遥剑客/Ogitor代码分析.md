# Ogitor代码分析 - 逍遥剑客 - CSDN博客
2010年01月17日 22:36:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4825

# 引擎状态监听(观察者)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/Ogitor01.png)
底层变化时, 需要高层UI做出反应. 这个有一个原则, 就是下层逻辑不能依赖上层逻辑, 所以通过一个接口进行消息的派发. 如果是.net的话, 可以用delegate/event实现. C++的话, 可以自己用模板写个delegate用, 效率比用接口高…
# 实体抽象(工厂方法)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/Ogitor02.png)
扩展性是很强, 但是…类太多了-_-. 这里有种基于DLL的插件模型可以学习. 工厂方法需要知道具体的工厂对象, 这里要求初始化工厂的对象(OgitorRoot)要知道具体的工厂定义(C++的#include做得太失败了, 导致编译效率低下…). 但是做成DLL插件的话, 就可以利用DLL的导出函数反向调用OgitorRoot进行注册. 不过, 对于内部使用, 进行二进制分离有意义吗?
# Undo/Redo(命令模式)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/Ogitor03.png)
这机制实现的前提是, 所有CBaseEditor对象都可以用一系列的”属性”来进行初始化/设置. 所以每一个Command都是针对属性的修改. 对于Create/Destory来说, 可以看成对所有属性的修改外加对象的创建/删除. 属性的变量类型并没有进行抽象, 而是转换成了字符串, 需要时再转换回来. 如果是.net的话, 就方便许多, 直接可以获得PropertyInfo[], 而且能跟PropertyGrid控件进行属性的自动绑定. 不过这属于语言的”反射”应用, 标准C++不具备这个特性(@_@).
BaseEditor^ object = root->FindObject(objName);
if (nullptr == object)
	return;
Dictionary<String^, Object^> properties = undoCmd->Properties;
for each(KeyValuePair<String^, Object^> pair in properties)
{
	PropertyInfo^ info = object->GetType()->GetProperty(pair->Key);
	if (nullptr != info)
		info->SetValue(object, pair->Value, nullptr);
}
# 鼠标编辑
![](http://images.cnblogs.com/cnblogs_com/xoyojank/Ogitor04.png)
就是移动, 旋转, 缩放对象的操作响应还有可视化表示. 操作时的选中轴是所有编辑对象统一使用一套, 自定义的表示, 如下图中的spot light, 是通过派生CVisualHelper来进行定制绘制的. 
![](http://www.cnblogs.com/images/cnblogs_com/xoyojank/Ogitor06.png)
# 多选处理
这里到是没用什么有用的模式, 不过对于OgitorsRoot::VolumeSelect()的算法到是很感兴趣.
在viewport上的矩形选框, 可以对应3D空间一个5个平面组成的包围体(远面不包含), 以这个包围体到场景树去遍历查询所有实体, 就能得到选中的实体列表. 所有选中对象组合成一个CMultiSelEditor进行移动/旋转/缩放. 
# 属性编辑
把SetXXX全部定义到一个函数指针数组, 根据类型(ID)进行索引, 避免了一堆if-else. 无论是UI到引擎, 还是引擎到UI的通知, 都是以这个ID来进行查找的. PropertyGrid的Item绑定的数据, 也是这个ID, 而不是实际的对象. 但是, 这样也带来另一个问题: 每扩充一个属性就会去改动这个类型定义的头文件, 那DLL中定义的特殊属性怎么办?
/// The definition for any SETXXX function
/// BIND_ID: is the BINDING ID between the PropertyGrid and the Actual Functions
////param val     : Since the actual variable types change, PropertyGrid passes
///                 Its values as String and refresh
////param refresh : Since those functions can be called by any object other than PropertyGrid
///                 this parameter defines if the value at PropertyGrid should be refreshed
///                 used as "false" from PropertyGrid
typedef void (THISCALL CBaseEditor::* BINDINGSETFUNC)(int BIND_ID,Ogre::String val,bool refresh);
static BINDINGSETFUNC   m_SetFunctions[MAXIMUM_BINDING_FUNCTIONS]; ///< List of BINDING FUNCTIONS
# 序列化
这里有能体现出属性进行抽象的好处了, 所有对象直接写进XML格式的工程文件. 用XML的好处就是, 增删属性不用改文件序列化的代码, 缺点是解析速度慢, 占用空间大. 而二进制就比较郁闷, 格式一改动就要进行代码变更, 通常还要兼容几个版本的文件. 地形方面由于是插件的原因, 文件是独立的. 不过不管什么场景, 地形独立出来其实没什么不好的, 因为地形的格式比较稳定, 不会经常变动, 独立出来更方便做版本管理. 
