# UGUI内核大探究（五）Toggle与ToggleGroup - 凯奥斯的注释 - CSDN博客





2016年09月14日 14:40:47[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：16364
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









### [](http://blog.csdn.net/ecidevilin/article/details/52534383)

# Toggle（开关）是UGUI的组件，它都继承自Selectable类，也就是说它可以根据鼠标事件转换状态。[UGUI内核大探究（四）Selectable与Button里](http://blog.csdn.net/ecidevilin/article/details/52534383)我们讲解了Selectable和Button的实现原理。这篇文章就探究一下Toggle的实现原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。





Toggle继承自Selectable，还额外继承了IPointerClickHandler, ISubmitHandler两个接口。（还有一个ICanvasElement，是Canvas渲染的时候调用的接口，这里我们暂时不谈。）

Toggle有个重要的bool变量m_IsOn(对应成员属性IsOn)，表示是否被勾选。与Button类似，Toggle也有一个onValueChanged事件用于外部监听m_IsOn是否改变。




Toggle重写了Start方法，在Start的时候，调用PlayEffect（根据状态改变/渐变图像的透明度）。

OnEnable和OnDisable也被重写（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)），OnEnable时将自己注册到ToggleGroup（稍后会讲到）并调用PlayEffect。OnDisable时将自己移出ToggleGroup。

另外，还重写了OnDidApplyAnimationProperties，判断m_IsOn是否被动画改变（判断图像的Alpha是否接近零）。


OnPointerClick和OnSubmit会调用InternalToggle方法切换Toggle的状态isOn = !isOn，isOn是Toggle的成员[属性](http://blog.csdn.net/ecidevilin/article/details/52525080)，它的set访问器会调用Toggle的Set成员方法。





Set方法用于设置m_IsOn的值，并发送事件：


```
void Set(bool value, bool sendCallback)
        {
            if (m_IsOn == value)
                return;

            // if we are in a group and set to true, do group logic
            m_IsOn = value;
            if (m_Group != null && IsActive())
            {
                if (m_IsOn || (!m_Group.AnyTogglesOn() && !m_Group.allowSwitchOff))
                {
                    m_IsOn = true;
                    m_Group.NotifyToggleOn(this);
                }
            }

            // Always send event when toggle is clicked, even if value didn't change
            // due to already active toggle in a toggle group being clicked.
            // Controls like Dropdown rely on this.
            // It's up to the user to ignore a selection being set to the same value it already was, if desired.
            PlayEffect(toggleTransition == ToggleTransition.None);
            if (sendCallback)
                onValueChanged.Invoke(m_IsOn);
        }
```

除此之外，Set方法还会调用m_Group.NotifyToggleOn(this）。




ToggleGroup继承自UIBehaviour，可以将多个Toggle分到一个组里而保持至多一个Toggle状态为On。它有一个m_Toggles的成员变量，这是一个Toggle类型的List，用于保存注册在该ToggleGroup的所有Toggle。上文也讲到了，Toggle在OnEnable的时候注册到group，OnDisable的时候移出group，另外通过为group属性赋值也可以让Toggle注册到指定的ToggleGroup（一般是编辑器帮我们设置）。




注册和移出的代码很简单，就是将toggle加入m_Toggles或从m_Toggles删除。


NotifyToggleOn这个方法是将除了输入的Toggle之外所有的Toggle都关闭（isOn = false）。


我们也可以调用ToggleGroup的一些方法，例如AnyTogglesOn判断是否有Toggle开启（Toggle的set方法也调用了这个方法，即没有任何Toggle开启且group不允许全部Toggle关闭的时候，强制设为开启），ActiveToggles获取开启的Toggle，SetAllTogglesOff关闭所有Toggle。



[](http://blog.csdn.net/ecidevilin/article/details/52534383)


