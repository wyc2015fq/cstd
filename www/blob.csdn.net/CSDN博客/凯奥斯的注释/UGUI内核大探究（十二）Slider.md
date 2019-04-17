# UGUI内核大探究（十二）Slider - 凯奥斯的注释 - CSDN博客





2016年09月17日 22:32:16[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4139
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# Slider是UGUI的一个组件，使用它可以实现滑动条，算是一个比较常用的组件，它与ScrollBar（参考[UGUI内核大探究（十一）ScrollRect与ScrollBar](http://blog.csdn.net/ecidevilin/article/details/52564132)）有些类似，但又不太相同。本文那就分析一下Slider的原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


Slider继承自Selectable，并继承了IDragHandler, IInitializePotentialDragHandler, ICanvasElement三个接口。

Slider重写了OnEnable和OnDisable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）方法。OnEnable里m_FillRect的Image（m_FillImage）和transform组件及其父对象的RectTransform，并找到m_HandleRect的transform组件及其父对象的RectTransform。

![](https://img-blog.csdn.net/20160917214718212)


如图1对应Slider，2对应m_FillRect，3对应m_HandleRect。

OnEnable还会调用Set方法设置当前值，并更新表现，也就是：根据当前Value设置m_FillImage的fillAmount（ImageType为Filled，祥参[UGUI内核大探究（九）Image与RawImage](http://blog.csdn.net/ecidevilin/article/details/52556724)）或anchorMin和anchorMax（其他ImageType）以及m_HandleRect的anchorMin和anchorMax，体现出来就是滑动条的填充区域和滑动按钮的位置发生了变化。


而OnDisable只是调用DrivenRectTransformTracker类型的m_Tracker的Clear方法。（参考[https://docs.unity3d.com/462/Documentation/ScriptReference/DrivenRectTransformTracker.html](https://docs.unity3d.com/462/Documentation/ScriptReference/DrivenRectTransformTracker.html)。）


OnDidApplyAnimationProperties（当应用动画属性后）会判断动画是否影响了表现，将表现修正回来。


OnRectTransformDimensionsChange（当RectTransform维度变化）会更新表现。


OnPointerDown（当鼠标或触摸点下）会判断事件位置是否在m_HandleRect里面，如果在里面，将它转换为m_HandleRect的本地位置，设置它为拖拽的起始点（m_Offset），如果不在，直接调用UpdateDrag将value设置为对应的值。


另外还重写了Selectable的OnMove、FindSelectableOnLeft、FindSelectableOnRight、FindSelectableOnUp和FindSelectableOnDown方法。当方向键按下并与Slideer的方向一致时，便不在导航到下一个Selectable，而是修改value值（加减stepSize），即移动滑动条。


OnDrag是继承自IDragHandler的方法，也会调用UpdateDrag，将时间点转换为本地点，减去m_Offset，除以尺寸得到normalizedValue，normalizedValue的set访问器（参考[C#语法小知识（六）属性与索引器](http://blog.csdn.net/ecidevilin/article/details/52525080)）里会将它转化为给value。

OnInitializePotentialDrag是继承自IInitializePotentialDragHandler的方法，它设置事件useDragThreshold为false，即在拖拽事件开始前不需要额外的阈值判断。





Set方法：



```
protected virtual void Set(float input, bool sendCallback)
        {
            // Clamp the input
            float newValue = ClampValue(input);

            // If the stepped value doesn't match the last one, it's time to update
            if (m_Value == newValue)
                return;

            m_Value = newValue;
            UpdateVisuals();
            if (sendCallback)
                m_OnValueChanged.Invoke(newValue);
        }
```

ClampValue是将input限定在编辑器里设置的Min Value和Max Value之间。如果value改变，且sendCallback为true，便会发送m_OnValueChanged事件。我们可以在编辑器里添加对这个事件的监听。




当我们在编辑器里设置Slider的Direction时候，我们发现Slider发生了旋转。这就涉及到SetDirection方法：

```
public void SetDirection(Direction direction, bool includeRectLayouts)
        {
            Axis oldAxis = axis;
            bool oldReverse = reverseValue;
            this.direction = direction;

            if (!includeRectLayouts)
                return;

            if (axis != oldAxis)
                RectTransformUtility.FlipLayoutAxes(transform as RectTransform, true, true);

            if (reverseValue != oldReverse)
                RectTransformUtility.FlipLayoutOnAxis(transform as RectTransform, (int)axis, true, true);
        }
```


当赋值完direction属性的后，再调用axis（Horizontal或Vertical）和reverseValue（反转值）属性会更新。如果axis改变了，调用FlipLayoutAxes翻转坐标轴。如果reverseValue改变了，FlipLayoutOnAxis在轴上翻转（水平翻转或竖直翻转）。这样就只需要根据坐标轴来取值赋值就可以了，不用做复杂的判断。



