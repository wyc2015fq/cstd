# UGUI内核大探究（一）EventSystem - 凯奥斯的注释 - CSDN博客





2016年09月11日 15:31:52[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：11265
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# UGUI是Unity3D官方推出的UI系统，为了更好的使用UGUI，我们就需要去了解它。

UGUI代码开源，我们可以从bitbucket下载到[源码](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。

虽然Unity官方喜欢藏着掖着，例如RectTransform和Canvas系列的源码都放在了UnityEngine里面，但是这并不妨碍我们对已开源的代码进行分析（其实多少也有些妨碍吧。。。）

今天我们就探究了一下UGUI的事件系统EventSystem。

虽然名字起得很大，但是实际上EventSystem处理和管理的其实是点击、键盘输入、触摸等事件，所以应该成为输入事件系统InputEventSystem更为恰当。

我们看到EventSystem最开始声明了一个List和一个变量



```
private List<BaseInputModule> m_SystemInputModules = new List<BaseInputModule>();

        private BaseInputModule m_CurrentInputModule;
```




系统输入模块表和当前输入模块。

BaseInputModule是一个抽象类，PointerInputModule继承自它，同样也是抽象类，而StandaloneInputModule和TouchInputModule又继承自PointerInputModule。StandaloneInputModule是面向“PC,
 Mac& Linux Standalone”这个平台的输入模块，而TouchInputModule是面向“iOS”、“Android”等可触摸移动平台的输入模块。

EventSystem会每一帧都处理这些输入模块，首先是调用TickModules方法，更新每一个InputModule。

然后遍历m_SystemInputModules判断这些module是否支持当前的平台（例如StandaloneInputModule需要判断是否鼠标已连接，而TouchInputModule需要判断是否支持触摸）且module是否可激活（可激活条件祥参StandaloneInputModule和TouchInputModule的源码）。

如果有符合条件的module便赋值给m_CurrentInputModule（当前输入模块）并break。

如果没有符合条件的module便选取第一个支持当前平台的module作为m_CurrentInputModule。

最后如果m_CurrentInputModule的值变化了并且m_CurrentInputModule不为null便调用：
`m_CurrentInputModule.Process();`而这个方法会将各种输入事件（如点击、拖拽等事件）传递给EventSystem当前选中的GameObject（m_CurrentSelected）。


而m_CurrentSelected大部分情况是Selectable组件（继承自它的有Button、Dropdown、InputField等组件）设置的，相关代码我们会在后续的文章中介绍。

而设置m_CurrentSelected的时候，会通过ExecuteEvents这个类对之前的对象执行一个被取消事件，且对新选中的对象执行一个被选中事件。这就是OnSelect和OnDeselect两个方法的由来。




EventSystem的RaycastAll方法使用射线从相机到某个点（设为点E）照射在UI上，然后对被照射到的所有对象进行排序，大致是远近排序。

而这个方法实在PointerInputModule中使用的，如果点击（或触摸）事件移动的时候，被该事件影响的对象也会发生变化，通过RaycastAll方法（传入的eventData中的position属性作为点E）获得到第一个被射线照射到的对象，如果与之前的对象不同，便变更对象（同时会回调原对象的OnPointerExit和新对象的OnPointerEnter方法）。

IsPointerOverGameObject是EventSystem类里特别常用的一个方法，用于判断是否点击在UI上，具体是在PointerInputModule中实现的，以后我们研究UGUI的输入模块的时候会深入讲解，概括一下就是判断当前是否存在某个键位（默认是-1鼠标左键）的数据。


最后我们注意到EventSystem有一个static属性：


`        public static EventSystem current { get; set; }`
因为是静态属性，所以只存在一个current，而并不会每个实例对象会有一个.


当一个EventSystem组件OnEnable的时候会将这个对象赋值给current。[参考OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)。



```
protected override void OnEnable()
        {
            base.OnEnable();
            if (EventSystem.current == null)
                EventSystem.current = this;
#if UNITY_EDITOR
            else
            {
                Debug.LogWarning("Multiple EventSystems in scene... this is not supported");
            }
#endif
        }
```

OnDisable的时候会将current赋值为null（当current==this）。[参考OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)。




```
protected override void OnDisable()
        {
            if (m_CurrentInputModule != null)
            {
                m_CurrentInputModule.DeactivateModule();
                m_CurrentInputModule = null;
            }

            if (EventSystem.current == this)
                EventSystem.current = null;

            base.OnDisable();
        }
```


这就是为什么我们在Unity Editor里面增加多个EventSystem的时候会报警告。


当然在游戏运行的时候，我们也不能随意的增加删除EventSystem，一来似乎并没有什么必要，二来虽然EventSystem本身并没有太多东西，但是毕竟输入模块里面还是保存了一些数据的。



