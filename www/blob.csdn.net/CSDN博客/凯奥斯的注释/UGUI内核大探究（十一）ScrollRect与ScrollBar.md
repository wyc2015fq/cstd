# UGUI内核大探究（十一）ScrollRect与ScrollBar - 凯奥斯的注释 - CSDN博客





2016年09月17日 21:33:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：8100
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









当我们在Unity Editor里创建一个Scroll View的时候含有ScrollRect的对象，它下面还有三个子对象，两个含有ScrollBar组件的子对象是作为滚动条，一个Viewport用于限定显示区域。我们可以为Viewport下面的Content对象添加组件（例如Image）或者子对象。点击运行，我们就可以拖动Scroll View，并且看到里面的内容也跟着在滚动。本文就详细分析一下ScrollRect和ScrollBar的源码，了解一下它们是怎么动起来的。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


首先介绍ScrollBar，它继承自Selectable，还继承了IBeginDragHandler, IDragHandler, IInitializePotentialDragHandler, ICanvasElement四个接口。




ScrollBar重写了OnEnable和OnDisable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）方法。OnEnable里会找到m_HandleRect父对象的RectTransform组件作为m_ContainerRect。如果所示：

![](https://img-blog.csdn.net/20160917180945073)


1号对应的是ScrollBar，2号对应的是m_ContainerRect，3号对应的是m_HandleRect。

然后OnEnable会重新设置当前值（value），并刷新表现，即根据当前Value设置m_HandleRect的anchorMin和anchorMax，体现出来就是滚动条的位置发生了变化。

而OnDisable只是调用DrivenRectTransformTracker类型的m_Tracker的Clear方法。（参考[https://docs.unity3d.com/462/Documentation/ScriptReference/DrivenRectTransformTracker.html](https://docs.unity3d.com/462/Documentation/ScriptReference/DrivenRectTransformTracker.html)。）

ScrollBar还重写了Selectable的OnPointerDown方法，设置isPointerDownAndNotDragging为true，使用协程调用了ClickRepeat方法，判断点击事件是否在m_HandleRect外面（一定在Scrollbar里面），如果在外面，就将事件坐标转换到m_HandleRect的本地坐标系里，然后调整value，直到点击事件在m_HandleRect里面。

重写的OnPointerUp方法里面，设置isPointerDownAndNotDragging为false。

另外还重写了Selectable的OnMove、FindSelectableOnLeft、FindSelectableOnRight、FindSelectableOnUp和FindSelectableOnDown方法。当方向键按下并与ScrollBar的方向一致时，便不在导航到下一个Selectable，而是修改value值（加减stepSize），即移动滚动条。

OnBeginDrag是继承自IBeginDragHandler接口的方法，这个方法里记录了拖拽的起始点（m_HandleRect内相对center的坐标）。

OnDrag是继承自IDragHandler接口的方法，这个方法会调用UpdateDrag方法。




```
void UpdateDrag(PointerEventData eventData)
        {
            if (eventData.button != PointerEventData.InputButton.Left)
                return;

            if (m_ContainerRect == null)
                return;

            Vector2 localCursor;
            if (!RectTransformUtility.ScreenPointToLocalPointInRectangle(m_ContainerRect, eventData.position, eventData.pressEventCamera, out localCursor))
                return;

            Vector2 handleCenterRelativeToContainerCorner = localCursor - m_Offset - m_ContainerRect.rect.position;
            Vector2 handleCorner = handleCenterRelativeToContainerCorner - (m_HandleRect.rect.size - m_HandleRect.sizeDelta) * 0.5f;

            float parentSize = axis == 0 ? m_ContainerRect.rect.width : m_ContainerRect.rect.height;
            float remainingSize = parentSize * (1 - size);
            if (remainingSize <= 0)
                return;

            switch (m_Direction)
            {
                case Direction.LeftToRight:
                    Set(handleCorner.x / remainingSize);
                    break;
                case Direction.RightToLeft:
                    Set(1f - (handleCorner.x / remainingSize));
                    break;
                case Direction.BottomToTop:
                    Set(handleCorner.y / remainingSize);
                    break;
                case Direction.TopToBottom:
                    Set(1f - (handleCorner.y / remainingSize));
                    break;
            }
        }
```

这个方法会计算出m_HandleRect左下角的坐标，根据Direction与剩下的尺寸（就是可滑动区域的尺寸）作比，计算出value。


OnInitializePotentialDrag方法是继承自IInitializePotentialDragHandler的方法，它将拖拽事件的useDragThreshold设为true。（这个值为true之后，判断拖拽事件开始时会加入一个对于移动距离的阈值判断）


另外，ScrollBar定义了一个onValueChanged的事件，我们可以在编辑器里添加事件监听。在Set方法里，这个事件可能会被发送出去。




下面我们将ScrollRect。它继承自UIBehaviour，另外还集成了IInitializePotentialDragHandler, IBeginDragHandler, IEndDragHandler, IDragHandler, IScrollHandler, ICanvasElement, ILayoutElement, ILayoutGroup这些接口。

OnEnable方法里添加了m_HorizontalScrollbar和m_VerticalScrollbar的onValueChanged事件的监听（用于监听滚动条的value变化，以调整内容的位置）。并将自己注册到CanvasUpdateRegistry的Layout序列中去（参考[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)）。


OnDisable方法将自己从CanvasUpdateRegistry中移除，并移除了两个ScrollBar的监听。设置m_HasRebuiltLayout为false，清除m_Tracker，设置m_Velocity（横纵速度）为0（在LateUpdate中被调用，用于将超出边界的内容移动回来），并通知LayoutRebuilder需要重建Layout（参考[UGUI内核大探究（十）Layout与Fitter](http://blog.csdn.net/ecidevilin/article/details/52561496)）。


IsActive除了调用了基类的有效性判断（对象有效并组件激活），还and了m_Content（内容）不为null。


OnRectTransformDimensionsChange（当RectTransform维度改变时）调用了SetDirty方法，通知LayoutRebuilder需要重建Layout。


OnInitializePotentialDrag（IInitializePotentialDragHandler）里设置m_Velocity为0。


OnBeginDrag（IBeginDragHandler）里设置将拖拽事件点转换为viewRect坐标系内的点赋值给m_PointerStartLocalCursor。将m_Content.anchoredPosition赋值给m_ContentStartPosition。并设置m_Dragging为true。

![](https://img-blog.csdn.net/20160917191303004)


图中，1为ScrollRect，2为viewRect，3为content。

OnEndDrag（IEndDragHandler）设置m_Dragging为false。


OnDrag（IDragHandler）会根据m_PointerStartLocalCursor和m_ContentStartPosition计算出m_Content新的anchoredPosition。


OnScroll（IScrollHandler）是用于接收鼠标滚动的方法。这个方法根据滚动距离计算出m_Content的位置。


Rebuild是继承自ICanvasElement（参考[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)），它在重建Layout的时候被调用。在Prelayout（预布局）阶段会调用UpdateCachedData（更新缓存数据，包括m_HorizontalScrollbarRect横向滚动条、m_VerticalScrollbarRect纵向滚动条、m_HSliderExpand是否支持横向滑动展开、m_VSliderExpand是否支持纵向滑动展开、m_HSliderHeight横向滚动条高度、m_VSliderWidth纵向滚动条宽度）。在PostLayout（后布局）阶段会更新边界、重置滚动条、保存旧数据（m_PrevPosition保存content的位置、m_PrevViewBounds保存view的边界、m_PrevContentBounds保存content的边界）。

ScrollRect还继承了ILayoutGroup接口，需要实现SetLayoutHorizontal和SetLayoutVertical两个方法。

SetLayoutHorizontal里，如果m_HSliderExpand或m_VSliderExpand为true，便强制立刻重建content的布局。然后根据m_VSliderExpand、vScrollingNeeded(content的高度大于view的高度)、m_HSliderExpand和hScrollingNeeded(content的宽度大于view的宽度)计算viewRect的sizeDelta、m_ViewBounds和m_ContentBounds。（关于sizeDelta是相对于父对象的尺寸，参考[https://docs.unity3d.com/462/Documentation/ScriptReference/RectTransform-sizeDelta.html](https://docs.unity3d.com/462/Documentation/ScriptReference/RectTransform-sizeDelta.html)）


SetLayoutVertical里调用UpdateScrollbarLayout方法并更新m_ViewBounds和m_ContentBounds。


UpdateScrollbarLayout里将横向滚动条的宽度设置为与ScrollRect同样值（如果有纵向滚动条，减掉其宽度），将纵向滚动条的高度设置为与ScrollRect同样值（如果有横向滚动条，减掉其高度）。


ScrollRect还重写了LateUpdate，这个方法是每一帧都会被调用，在所有组件Update调用完之后。在这个方法里，调用EnsureLayoutHasRebuilt确保Layout已经被重建，调用UpdateScrollbarVisibility更新ScrollBar的可见性。接着UpdateBounds更新边界。如果m_Dragging为false，且content已经超出了可滚动范围（例如：content的最小点的x大于view的最小点的x），且m_Velocity速度不为0，便根据速度逐渐将content的坐标修正为合理的值。然后如果在拖动中且m_Inertia（惯性）便根据content的当前位置和m_PrevPosition计算出一个新的惯性速度m_Velocity。然后判断m_ViewBounds、m_ContentBounds、m_Content.anchoredPosition和旧数据不同，更新ScrollBar的位置，发送OnValueChanged(编辑器中可设置)并保存当前数据为旧数据。




最后看一下更新边界的方法：



```
private void UpdateBounds()
        {
            m_ViewBounds = new Bounds(viewRect.rect.center, viewRect.rect.size);
            m_ContentBounds = GetBounds();

            if (m_Content == null)
                return;

            // Make sure content bounds are at least as large as view by adding padding if not.
            // One might think at first that if the content is smaller than the view, scrolling should be allowed.
            // However, that's not how scroll views normally work.
            // Scrolling is *only* possible when content is *larger* than view.
            // We use the pivot of the content rect to decide in which directions the content bounds should be expanded.
            // E.g. if pivot is at top, bounds are expanded downwards.
            // This also works nicely when ContentSizeFitter is used on the content.
            Vector3 contentSize = m_ContentBounds.size;
            Vector3 contentPos = m_ContentBounds.center;
            Vector3 excess = m_ViewBounds.size - contentSize;
            if (excess.x > 0)
            {
                contentPos.x -= excess.x * (m_Content.pivot.x - 0.5f);
                contentSize.x = m_ViewBounds.size.x;
            }
            if (excess.y > 0)
            {
                contentPos.y -= excess.y * (m_Content.pivot.y - 0.5f);
                contentSize.y = m_ViewBounds.size.y;
            }

            m_ContentBounds.size = contentSize;
            m_ContentBounds.center = contentPos;
        }

        private readonly Vector3[] m_Corners = new Vector3[4];
        private Bounds GetBounds()
        {
            if (m_Content == null)
                return new Bounds();

            var vMin = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
            var vMax = new Vector3(float.MinValue, float.MinValue, float.MinValue);

            var toLocal = viewRect.worldToLocalMatrix;
            m_Content.GetWorldCorners(m_Corners);
            for (int j = 0; j < 4; j++)
            {
                Vector3 v = toLocal.MultiplyPoint3x4(m_Corners[j]);
                vMin = Vector3.Min(v, vMin);
                vMax = Vector3.Max(v, vMax);
            }

            var bounds = new Bounds(vMin, Vector3.zero);
            bounds.Encapsulate(vMax);
            return bounds;
        }
```
GetBounds方法是将m_Content的四个顶点的世界坐标转化为viewRect坐标，然后生成一个Bounds，其实就是m_Content相对于viewRect的位置以及尺寸（会在计算m_Content位置的时候用到）。UpdateBounds会继续调整这个值，只有在Unity官方认为不合理的时候（content宽度或高度比view小）才会执行额外的调整，将Bounds的坐标和尺寸调整成合理的值（尺寸和view相同，位置根据pivot调整）。




