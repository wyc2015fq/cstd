# UGUI内核大探究（四）Selectable与Button - 凯奥斯的注释 - CSDN博客





2016年09月14日 09:21:40[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6538标签：[UGUI																[Selectable																[Button																[内核																[源码](https://so.csdn.net/so/search/s.do?q=源码&t=blog)
个人分类：[UGUI](https://blog.csdn.net/ecidevilin/article/category/6399641)

所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)](https://so.csdn.net/so/search/s.do?q=内核&t=blog)




# Selectable是UGUI的核心组件，除了最常用的Button，它还是Scrollbar、Dropdown、Slider、Toggle、InputField这些组件的基类。在本文中，我们将分析Selectable和Button的源码，探究二者的实现原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


在Selectable之前，我们看到，它被添加了四个属性：



```
[AddComponentMenu("UI/Selectable", 70)]
    [ExecuteInEditMode]
    [SelectionBase]
    [DisallowMultipleComponent]
```

[AddComponentMenu](https://docs.unity3d.com/462/Documentation/ScriptReference/AddComponentMenu.html)是为Component菜单添加选项，顺序为70。当点击一个GameObject的AddComponent按钮时，会弹出一个菜单，点击UI选项，就可以看到排在最后一个的就是Selectable。



[ExecuteInEditMode](https://docs.unity3d.com/462/Documentation/ScriptReference/ExecuteInEditMode.html)表示该组件会在编辑器下执行。

[SelectionBase](https://docs.unity3d.com/462/Documentation/ScriptReference/SelectionBaseAttribute.html)标记该对象为选择基本对象。


[DisallowMultipleComponent](https://docs.unity3d.com/462/Documentation/ScriptReference/DisallowMultipleComponent.html)不允许一个GameObject上出现同种组件。我们无法为一个对象添加两个Selectable组件，也无法为一个Button对象再添加一个InputField组件。





Selectable继承自UIBehaviour，并且继承了7个接口。



```
public class Selectable
        :
        UIBehaviour,
        IMoveHandler,
        IPointerDownHandler, IPointerUpHandler,
        IPointerEnterHandler, IPointerExitHandler,
        ISelectHandler, IDeselectHandler
```

UIBehaviour是所有UI组件的基类，它被放在EventSystem目录下，除了IsDestroyed以外，其他都是虚函数，我们也可以认为它是事件系统的一部分，它添加了Awake（当脚本实例被加载，即AddComponent时）、OnRectTransformDimensionsChange（当RectTransform维度变化）等方法来接收来自UnityEngine的事件。



Selectable的Awake获取了一个Graphic的组件实例m_TargetGraphic（Image组件间接继承自Graphic）。如果我们选择的Transition为Color
 Tint，当Selectable的状态变化（普通、高亮、按压、禁用）时，会调用m_TargetGraphic的CrossFadeColor方法，即将当前图像渐变为指定颜色。

OnEnable（被调用的时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）里，会将该实例添加至Selectable的静态列表s_List里（s_List里存放了所有可用的Selectable，用于UI导航的查询），然后设置实例的状态为Normal或者Highlighted（亦或Disabled）。

OnDisable（被调用的时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）里，将实例清除状态（需要恢复颜色和图片，以及播放normal动画），并从s_List中将该实例移除。

OnDidApplyAnimationProperties（当应用动画属性后）里，会通过OnSetProperty方法调用InternalEvaluateAndTransitionToSelectionState，刷新当前状态。


OnCanvasGroupChanged（当CanvasGroup变化时）里，会判断新的GanvasGroup的interactable，如果GanvasGroup的interactable为false，那么Selectable本身也就被禁用了。接着，刷新当前状态。


然后我们看Selectable继承的几个接口，可以先看一下[UGUI内核大探究（三）输入模块](http://blog.csdn.net/ecidevilin/article/details/52528445)了解这些接口的触发时机。

继承自IMoveHandler需要实现OnMove方法。根据移动方向，导航到下一个Selectable组件。

继承自IPointerDownHandler需要实现OnPointerDown方法。调用EventSystem.current.SetSelectedGameObject将自己设为当前选中对象（会调用自己的OnSelect和前任对象的OnDeselect），标记isPointerDown为true并刷新状态（当isPointerInside和isPointerDown同事为true的时候为Pressed状态）。

继承自IPointerUpHandler需要实现OnPointerUp方法。标记isPointerDown为false并刷新状态。

继承自IPointerEnterHandler需要实现OnPointerEnter方法。标记isPointerInside为true并刷新状态。

继承自IPointerExitHandler需要实现OnPointerExit方法。标记isPointerInside为false并刷新状态。

继承自ISelectHandler需要实现OnSelect方法。标记hasSelection为true并刷新状态（hasSelection为true时为Highlighted状态，另外isPointerInside和isPointerDown也是判断Highlighted状态的依据，稍后会提到）。

继承自IDeselectHandler需要实现OnDeselect方法。标记hasSelection为false并刷新状态。

IsHighlighted方法：



```
protected bool IsHighlighted(BaseEventData eventData)
        {
            if (!IsActive())
                return false;

            if (IsPressed())
                return false;

            bool selected = hasSelection;
            if (eventData is PointerEventData)
            {
                var pointerData = eventData as PointerEventData;
                selected |=
                    (isPointerDown && !isPointerInside && pointerData.pointerPress == gameObject) // This object pressed, but pointer moved off
                    || (!isPointerDown && isPointerInside && pointerData.pointerPress == gameObject) // This object pressed, but pointer released over (PointerUp event)
                    || (!isPointerDown && isPointerInside && pointerData.pointerPress == null); // Nothing pressed, but pointer is over
            }
            else
            {
                selected |= isPointerInside;
            }
            return selected;
        }
```

参数为输入模块传递过来的事件数据，主要用于判断事件响应对象是否是本对象（或者是null）。


上述的OnPointerDown等方法是通过EvaluateAndTransitionToSelectionState方法评估并刷新状态。在这个方法里，UpdateSelectionState调用IsPressed和IsHighlighted判断当前状态。InternalEvaluateAndTransitionToSelectionState判断当前组件是否被禁用，然后调用DoStateTransition方法。



```
protected virtual void DoStateTransition(SelectionState state, bool instant)
        {
            Color tintColor;
            Sprite transitionSprite;
            string triggerName;

            switch (state)
            {
                case SelectionState.Normal:
                    tintColor = m_Colors.normalColor;
                    transitionSprite = null;
                    triggerName = m_AnimationTriggers.normalTrigger;
                    break;
                case SelectionState.Highlighted:
                    tintColor = m_Colors.highlightedColor;
                    transitionSprite = m_SpriteState.highlightedSprite;
                    triggerName = m_AnimationTriggers.highlightedTrigger;
                    break;
                case SelectionState.Pressed:
                    tintColor = m_Colors.pressedColor;
                    transitionSprite = m_SpriteState.pressedSprite;
                    triggerName = m_AnimationTriggers.pressedTrigger;
                    break;
                case SelectionState.Disabled:
                    tintColor = m_Colors.disabledColor;
                    transitionSprite = m_SpriteState.disabledSprite;
                    triggerName = m_AnimationTriggers.disabledTrigger;
                    break;
                default:
                    tintColor = Color.black;
                    transitionSprite = null;
                    triggerName = string.Empty;
                    break;
            }

            if (gameObject.activeInHierarchy)
            {
                switch (m_Transition)
                {
                    case Transition.ColorTint:
                        StartColorTween(tintColor * m_Colors.colorMultiplier, instant);
                        break;
                    case Transition.SpriteSwap:
                        DoSpriteSwap(transitionSprite);
                        break;
                    case Transition.Animation:
                        TriggerAnimation(triggerName);
                        break;
                }
            }
        }
```

根据状态设置颜色、图片或动画名称，再通过StartColorTween、DoSpriteSwap或TriggerAnimation方法在UI上体现出状态的改变。





最后讲解一下Button组件。Button继承自Selectable并额外继承了IPointerClickHandler、ISubmitHandler两个接口。它还添加了一个UnityEvent类型的事件onClick。onClick事件可以添加用户自定义的监听，具体方法可以通过编辑器添加也可以通过onClick.AddListener添加。

OnPointerClick会调用Press方法来回调onClick。


OnSubmit也会调用Press方法，并切换状态为Pressed，并开启协程调用OnFinishSubmit，让状态渐变为当前状态（通过UpdateSelectionState获得的状态）。





可以看出Button就是相对于Selectable添加了响应点击和确认事件的接口，并且开放了可添加用户自定义监听的onClick事件。

而Selectable的作用在于提供了基于鼠标事件的四种状态变化。一方面，为Button、Dropdown等派生类提供了基础的逻辑，另一方面，我们也可以根据Selectable派生出新的自定义组件。](https://so.csdn.net/so/search/s.do?q=Button&t=blog)](https://so.csdn.net/so/search/s.do?q=Selectable&t=blog)](https://so.csdn.net/so/search/s.do?q=UGUI&t=blog)




