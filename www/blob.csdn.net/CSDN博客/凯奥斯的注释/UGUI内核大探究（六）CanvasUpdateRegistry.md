# UGUI内核大探究（六）CanvasUpdateRegistry - 凯奥斯的注释 - CSDN博客





2016年09月15日 10:52:28[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4843标签：[unity																[UGUI																[画布																[更新																[注册](https://so.csdn.net/so/search/s.do?q=注册&t=blog)
个人分类：[UGUI](https://blog.csdn.net/ecidevilin/article/category/6399641)

所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)](https://so.csdn.net/so/search/s.do?q=更新&t=blog)




# CanvasUpdateRegistry（画布更新注册处）是一个单例，它是UGUI与Canvas之间的中介，继承了ICanvasElement接口的组件都可以注册到它，它监听了Canvas即将渲染的事件，并调用已注册组件的Rebuild等方法。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


本文就讲解一下CanvasUpdateRegistry这个类。

首先可以看到ICanvasElement这个接口：



```
public interface ICanvasElement
    {
        void Rebuild(CanvasUpdate executing);
        Transform transform { get; }
        void LayoutComplete();
        void GraphicUpdateComplete();
        // due to unity overriding null check
        // we need this as something may not be null
        // but may be destroyed
        bool IsDestroyed();
    }
```

我们知道UGUI组件都继承自UIBehaviour，而UIBehavior实现了IsDestroyed方法。所有组件都继承自Component，而Component实现了transform属性。所以继承自ICanvasElement的UGUI组件不必再实现这两个成员。另外三个Rebuild（重建）、LayoutComplete（布局完成）、GraphicUpdateComplete（图像更新完成）就需要在代码中实现。
（一个例外LayoutRebuilder，它并不是组件，是一个负责重建布局的类，在后续文章中我们会讲到）




CanvasUpdateRegistry维护了两个索引集（不会存放相同的元素）：




```
private readonly IndexedSet<ICanvasElement> m_LayoutRebuildQueue = new IndexedSet<ICanvasElement>();
        private readonly IndexedSet<ICanvasElement> m_GraphicRebuildQueue = new IndexedSet<ICanvasElement>();
```

一个是布局重建序列（m_LayoutRebuildQueue），一个是图像重建序列（m_GraphicRebuildQueue）。
m_LayoutRebuildQueue是通过RegisterCanvasElementForLayoutRebuild和TryRegisterCanvasElementForLayoutRebuild方法添加元素。

m_GraphicRebuildQueue是通过RegisterCanvasElementForGraphicRebuild和TryRegisterCanvasElementForGraphicRebuild方法添加元素。


二者通过UnRegisterCanvasElementForRebuild移除注册元素。




CanvasUpdateRegistry的构造函数：



```
protected CanvasUpdateRegistry()
        {
            Canvas.willRenderCanvases += PerformUpdate;
        }
```


willRenderCanvases是静态类Canvas的静态事件，事件是一种特殊的委托（关于委托，参考[C#语法小知识（四）委托delegate](http://blog.csdn.net/ecidevilin/article/details/52486015)）。在CanvasUpdateRegistry的构造函数里，为willRenderCanvases事件添加了一个监听PerformUpdate。从字面意思我们可以知道，在渲染（所有）Canvas之前会抛出willRenderCanvases事件，继而调用到CanvasUpdateRegistry的PerformUpdate方法。




介绍PerformUpdate之前，我们先把CanvasUpdateRegistry文件滚到最上，我们看到一个枚举类型：



```
public enum CanvasUpdate
    {
        Prelayout = 0,
        Layout = 1,
        PostLayout = 2,
        PreRender = 3,
        LatePreRender = 4,
        MaxUpdateValue = 5
    }
```

除了最后一个枚举项，其他五个项分别代表了布局的三个阶段和渲染的两个阶段。







在PerformUpdate里会从两个序列里删除掉不可用的元素（如果元素是LayoutRebuilder，会调用的LayoutComplete）。

接着对m_LayoutRebuildQueue依据父对象数量进行排序。然后分别以Prelayout、Layout和PostLayout为参数调用每一个元素的Rebuild方法，然后调用所有元素的LayoutComplete方法并清除所有元素。

完成布局之后，调用ClipperRegistry.instance.Cull()。（ClipperRegistry是另外一个注册处单例，用于在布局之后调用组件的修剪方法。）

继而分别以PreRender和LatePreRender为参数调用m_GraphicRebuildQueue的每一个元素的Rebuild方法，然后调用所有元素的LayoutComplete（？似乎是BUG）方法并清除所有元素。




至此，一个完整的更新流程就完成了。](https://so.csdn.net/so/search/s.do?q=画布&t=blog)](https://so.csdn.net/so/search/s.do?q=UGUI&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)




