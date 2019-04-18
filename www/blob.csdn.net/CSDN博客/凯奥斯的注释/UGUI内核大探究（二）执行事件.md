# UGUI内核大探究（二）执行事件 - 凯奥斯的注释 - CSDN博客





2016年09月12日 08:42:20[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3976
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)我们探究了事件系统，其中我们讲到EventSystem可以通过ExecuteEvents这个类来执行事件，那么事件是如何执行的呢？这里涉及到了两个文件EventInterface和ExecuteEvents。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。

EventInterface声明了一系列interface(可以参考我之前的博客[关于接口的介绍](http://blog.csdn.net/ecidevilin/article/details/52497378))，如果经常接触UI的话，我相信你对这些接口一定不陌生。例如IPointerEnterHandler（指针进入事件接口）。一个组件添加这个接口的继承之后，再实现OnPointerEnter方法，便可以接收到指针进入事件，也就是当鼠标滑入对象所在的区域之后，便会回调OnPointerEnter方法。这些接口全都继承自IEventSystemHandler，而后者也是声明在EventInterface里的接口。

以上这些接口都会在ExecuteEvents里被调用。ExecuteEvents类是个[静态类](http://baike.baidu.com/link?url=WE5snEmytVaQS_3gi1IGx8O0it-BNhO25be6DOSl4QZj3_crza5Kpzv-iMTKt2yA0g8vECupX9KFJuQ0TnT-hK)，不能被实例化，所有的公共方法都通过ExecuteEvents.XXXX来调用。ExecuteEvents里声明了一个delegate的类型（可以参考我之前的博客[关于委托的介绍](http://blog.csdn.net/ecidevilin/article/details/52486015)）EventFunction，这是一个[泛型委托](https://msdn.microsoft.com/zh-cn/library/sx2bwtw7.aspx)，委托的第一个参数handler可以是不同的类型。然后对EventInterface里除了IEventSystemHandler外每一个接口声明了一个EventFunction类型的委托变量和方法。例如：



```
private static readonly EventFunction<IPointerEnterHandler> s_PointerEnterHandler = Execute;

        private static void Execute(IPointerEnterHandler handler, BaseEventData eventData)
        {
            handler.OnPointerEnter(ValidateEventData<PointerEventData>(eventData));
        }
```




就是声明了一个参数类型为IPointerEnterHandler的委托变量s_PointerEnterHandler，并实现了委托所执行的方法Execute。

然后又声明了一系列属性，这些属性是获取上述委托变量的只读属性，用于在外部调用。

而外部统一调用执行事件的方法是


`public static bool Execute<T>(GameObject target, BaseEventData eventData, EventFunction<T> functor) where T : IEventSystemHandler`我们先看一下它的使用方法，例如在BaseInputModule里，我们看到:



`ExecuteEvents.Execute(t.gameObject, currentPointerData, ExecuteEvents.pointerEnterHandler);`
前两个参数很好理解，第三个参数就是调用了ExecuteEvents里的pointerEnterHandler属性（也就是上面的s_PointerEnterHandler）,这样泛型T也就变成了IPointerEnterHandler。


Execute方法里面，通过GetEventList获得target上的T类型的组件列表，然后遍历这些组件，并执行


`functor(arg, eventData);`
以pointerEnterHandler为例，我们可以了解这个方法实际上执行的是：



`arg.OnPointerEnter(ValidateEventData<PointerEventData>(eventData));`
也就是调用了IPointerEnterHandler类型的组件的OnPointerEnter方法。


至此，我们就了解到了UGUI里的事件是如何执行的：指定某个接口类型，由Execute方法调用目标对象的接口方法。

接着，补充一下ExecuteEvents类里面其他方法的介绍。

ExecuteHierarchy方法会通过GetEventChain获取target的所以父对象，并对这些对象（包括target）执行Execute方法。
GetEventHandler会遍历目标对象及其父对象，判断他们是否包含某个指定接口，如果包含则作为返回值返回。而判断方法是CanHandleEvent，通过GetEventList方法获取target上的T类型的组件列表，判断列表数量不为零。GetEventHandler主要在输入模块里被调用，用于获取某个输入事件的响应对象。（详细分析会在后续的文章中介绍）






