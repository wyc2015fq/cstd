# UGUI内核大探究（十）Layout与Fitter - 凯奥斯的注释 - CSDN博客





2016年09月17日 12:29:20[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3613
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# [UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)中我们讲到Graphic组件SetLayoutDirty里会通知LayoutRebuilder布局需要重建，那么布局是具体是怎样重建的呢？我们知道UGUI有三种布局组，HorizontalLayoutGroup（水平布局组）、VerticalLayoutGroup（垂直布局组）和GridLayoutGroup（格子布局组），为对象添加某一种布局组，就可以让其子对象按照对应布局方法排列。那么这些LayoutGroup与Graphic又有什么关系呢？本文就带着这些问题就探究一下Layout系列组件的原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


在研究Layout系列组件之前，我们首先要看一下LayoutRebuilder这个类。Graphic是通过MarkLayoutForRebuild这个静态方法标记自己需要重建的。



```
public static void MarkLayoutForRebuild(RectTransform rect)
        {
            if (rect == null)
                return;

            RectTransform layoutRoot = rect;
            while (true)
            {
                var parent = layoutRoot.parent as RectTransform;
                if (!ValidLayoutGroup(parent))
                    break;
                layoutRoot = parent;
            }

            // We know the layout root is valid if it's not the same as the rect,
            // since we checked that above. But if they're the same we still need to check.
            if (layoutRoot == rect && !ValidController(layoutRoot))
                return;

            MarkLayoutRootForRebuild(layoutRoot);
        }
```

这个方法里，会找到父对象中最上层的ILayoutGroup类型的组件（ValidLayoutGroup）layoutRoot，如果没有找到，便判断一下自己是否是ILayoutController类型（ValidController），然后为符合条件的layoutRoot创建Rebuilder。


```
private static void MarkLayoutRootForRebuild(RectTransform controller)
        {
            if (controller == null)
                return;

            var rebuilder = s_Rebuilders.Get();
            rebuilder.Initialize(controller);
            if (!CanvasUpdateRegistry.TryRegisterCanvasElementForLayoutRebuild(rebuilder))
                s_Rebuilders.Release(rebuilder);
        }
```

s_Rebuilders是一个LayoutRebuilder类型的ObjectPool，通过Get方法创建（或从Pool里取出）一个实例rebuilder（再通过Release方法回收这一段内存）。我们看到这段代码调用了TryRegisterCanvasElementForLayoutRebuild，将rebuilder注册到CanvasUpdateRegistry。

LayoutRebuilder继承了ICanvasElement接口。[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)中介绍过当Canvas绘制前会调用CanvasUpdateRegistry，而后者会遍历所有注册到它的ICanvasElement类型的组件，调用他们的Rebuild方法。



```
public void Rebuild(CanvasUpdate executing)
        {
            switch (executing)
            {
                case CanvasUpdate.Layout:
                    // It's unfortunate that we'll perform the same GetComponents querys for the tree 2 times,
                    // but each tree have to be fully iterated before going to the next action,
                    // so reusing the results would entail storing results in a Dictionary or similar,
                    // which is probably a bigger overhead than performing GetComponents multiple times.
                    PerformLayoutCalculation(m_ToRebuild, e => (e as ILayoutElement).CalculateLayoutInputHorizontal());
                    PerformLayoutControl(m_ToRebuild, e => (e as ILayoutController).SetLayoutHorizontal());
                    PerformLayoutCalculation(m_ToRebuild, e => (e as ILayoutElement).CalculateLayoutInputVertical());
                    PerformLayoutControl(m_ToRebuild, e => (e as ILayoutController).SetLayoutVertical());
                    break;
            }
        }
```

PerformLayoutCalculation从m_ToRebuild(也就是保存在rebuilder里的layoutRoot)获取ILayoutElement类型的组件，根据第二个参数（Lamda表达式）调用他们的对应的方法（CalculateLayoutInputHorizontal或CalculateLayoutInputVertical）。再调用方法之前，以m_ToRebuild的子对象为参数递归调用PerformLayoutCalculation。（由此我们可知，Graphic实际上是为Image和Text调用MarkLayoutForRebuild方法）

PerformLayoutControl从m_ToRebuild获取ILayoutController（ILayoutGroup继承自这个接口）组件，根据第二参数调用他们对应的方法（SetLayoutHorizontal或SetLayoutVertical），然后以m_ToRebuild的子对象为参数递归调用PerformLayoutControl。


在PerformLayoutControl里我们看到ILayoutSelfController（ILayoutController的派生接口）类型的组件会优先于其他组件调用方法。那么ILayoutSelfController是什么样的组件呢？

![](https://img-blog.csdn.net/20160917091917294)


上图中，我们为一个Text添加了ContentSizeFitter组件，并设置Horizontal Fit和Vertical
 Fit都为Preferred Size，就可以看到这个对象的尺寸变成了跟Text内容一样的尺寸。

ContentSizeFitter就是继承了ILayoutSelfController接口的组件，同样继承了后者的还有AspectRatioFitter。二者是调节自身尺寸的组件，所以是ILayoutSelfController。


在ContentSizeFitter里SetLayoutHorizontal和SetLayoutVertical方法会调用HandleSelfFittingAlongAxis方法：



```
private void HandleSelfFittingAlongAxis(int axis)
        {
            FitMode fitting = (axis == 0 ? horizontalFit : verticalFit);
            if (fitting == FitMode.Unconstrained)
                return;

            m_Tracker.Add(this, rectTransform, (axis == 0 ? DrivenTransformProperties.SizeDeltaX : DrivenTransformProperties.SizeDeltaY));

            // Set size to min or preferred size
            if (fitting == FitMode.MinSize)
                rectTransform.SetSizeWithCurrentAnchors((RectTransform.Axis)axis, LayoutUtility.GetMinSize(m_Rect, axis));
            else
                rectTransform.SetSizeWithCurrentAnchors((RectTransform.Axis)axis, LayoutUtility.GetPreferredSize(m_Rect, axis));
        }
```

根据适应方式为rectTransform设置尺寸（最小尺寸或首选尺寸）。



AspectRatioFitter是方面比例适应器，它的SetLayoutHorizontal和SetLayoutVertical方法虽然是空方法，但是aspectMode、aspectRatioaspectRatio改变以及OnEnable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）时会调用SetDirty，继而调用UpdateRect：




```
private void UpdateRect()
        {
            if (!IsActive())
                return;

            m_Tracker.Clear();

            switch (m_AspectMode)
            {
#if UNITY_EDITOR
                case AspectMode.None:
                {
                    if (!Application.isPlaying)
                        m_AspectRatio = Mathf.Clamp(rectTransform.rect.width / rectTransform.rect.height, 0.001f, 1000f);

                    break;
                }
#endif
                case AspectMode.HeightControlsWidth:
                {
                    m_Tracker.Add(this, rectTransform, DrivenTransformProperties.SizeDeltaX);
                    rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, rectTransform.rect.height * m_AspectRatio);
                    break;
                }
                case AspectMode.WidthControlsHeight:
                {
                    m_Tracker.Add(this, rectTransform, DrivenTransformProperties.SizeDeltaY);
                    rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, rectTransform.rect.width / m_AspectRatio);
                    break;
                }
                case AspectMode.FitInParent:
                case AspectMode.EnvelopeParent:
                {
                    m_Tracker.Add(this, rectTransform,
                        DrivenTransformProperties.Anchors |
                        DrivenTransformProperties.AnchoredPosition |
                        DrivenTransformProperties.SizeDeltaX |
                        DrivenTransformProperties.SizeDeltaY);

                    rectTransform.anchorMin = Vector2.zero;
                    rectTransform.anchorMax = Vector2.one;
                    rectTransform.anchoredPosition = Vector2.zero;

                    Vector2 sizeDelta = Vector2.zero;
                    Vector2 parentSize = GetParentSize();
                    if ((parentSize.y * aspectRatio < parentSize.x) ^ (m_AspectMode == AspectMode.FitInParent))
                    {
                        sizeDelta.y = GetSizeDeltaToProduceSize(parentSize.x / aspectRatio, 1);
                    }
                    else
                    {
                        sizeDelta.x = GetSizeDeltaToProduceSize(parentSize.y * aspectRatio, 0);
                    }
                    rectTransform.sizeDelta = sizeDelta;

                    break;
                }
            }
        }
```


虽然有个不明对象m_Tracker，但是并不妨碍我们读代码。
HeightControlsWidth（高度控制宽度）和WidthControlsHeight（宽度控制高度）就不多讲了。FitInParent是当aspectRatio小于父对象的宽高比例时，以父对象的高为基准按照aspectRatio设置宽，大于时以父对象的宽为基准按照aspectRatio设置高。而FitInParent则相反。





接着我们看ILayoutController的另一个派生接口ILayoutGroup。LayoutGroup继承了ILayoutGroup。

LayoutGroup是一个抽象类，HorizontalOrVerticalLayoutGroup和GridLayoutGroup继承自它。而HorizontalOrVerticalLayoutGroup同样是个抽象类，HorizontalLayoutGroup和VerticalLayoutGroup继承自它。


LayoutGroup不仅继承了ILayoutGroup，还继承了ILayoutElement，这表明它也可以作为布局元素添加到别的布局组或适应器里。



```
public interface ILayoutElement
    {
        // After this method is invoked, layout horizontal input properties should return up-to-date values.
        // Children will already have up-to-date layout horizontal inputs when this methods is called.
        void CalculateLayoutInputHorizontal();
        // After this method is invoked, layout vertical input properties should return up-to-date values.
        // Children will already have up-to-date layout vertical inputs when this methods is called.
        void CalculateLayoutInputVertical();

        // Layout horizontal inputs
        float minWidth { get; }
        float preferredWidth { get; }
        float flexibleWidth { get; }
        // Layout vertical inputs
        float minHeight { get; }
        float preferredHeight { get; }
        float flexibleHeight { get; }

        int layoutPriority { get; }
    }
```



先讲继承自ILayoutGroup的方法。LayoutGroup的SetLayoutHorizontal和SetLayoutVertical是抽象方法，具体是在GridLayoutGroup、HorizontalLayoutGroup和VerticalLayoutGroup里实现的。
GridLayoutGroup的SetLayoutHorizontal和SetLayoutVertical调用了SetCellsAlongAxis方法，这个方法里根据CellSize和Spacing等参数为它包含的子对象设置尺寸和位置。


而HorizontalLayoutGroup和VerticalLayoutGroup组件的SetLayoutHorizontal和SetLayoutVertical调用了基类HorizontalOrVerticalLayoutGroup的SetChildrenAlongAxis，它会按照自己的尺寸水平或竖直分割成N份（N为子对象的数量），并为子对象设置位置和尺寸。




SetCellsAlongAxis和SetChildrenAlongAxis使用到的rectChildren（矩形子对象）是通过CalculateLayoutInputHorizontal获得的。这个方法是继承自ILayoutElement的方法，前面提到过。


这个方法将子对象中不忽略Layout的RectTransform保存在rectChildren里。

而LayoutGroup的子类各自重写了CalculateLayoutInputHorizontal以及CalculateLayoutInputVertical，除了上面的操作之外，还计算了minWidth、preferredWidth、flexibleWidth、minHeight、preferredHeight和flexibleHeight属性，而这些属性也是ILayoutElement接口的属性，而这些属性在HorizontalOrVerticalLayoutGroup计算子对象尺寸时用到。



