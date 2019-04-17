# UGUI内核大探究（十三）Dropdown - 凯奥斯的注释 - CSDN博客





2016年09月18日 09:07:25[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6926
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# Dropdown（下拉框）可谓是UGUI的集大成者，在Unity Editor里新建一个Dropdown，会随之附赠Text（Label对象）、Image（Arrow对象）、ScrollRect（Template对象）、Toggle（Template\Viewport\Content\item）和ScrollBar（Template\Scrollbar）。点击运行展开下拉框后还会创建一个Button（Blocker），而且还根据Template再实例化一个可见的Dropdown List。如此复杂的一个组件，竟然代码只有600余行，不得不让我们感叹Unity官方深谙组合之道。本文就探究一下Dropdown的神奇之处。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


我们首先看一下Dropdown的内部类DropdownItem（下拉项）。运行状态下展开下拉框，可以看到它被加到Item上面。

DropdownItem继承自MonoBehaviour和IPointerEnterHandler, ICancelHandler两个接口。

它包含了四个属性：text、image、rectTransform和toggle。


OnPointerEnter（当鼠标进入）方法继承自IPointerEnterHandler，调用EventSystem的SetSelectedGameObject将本对象设置为选中的对象（祥参[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)）。具体表现就是Item对象的背景颜色变了。


OnCancel（取消键按下）方法继承自ICancelHandler，获取父对象中的dropdown组件，调用Hide方法。具体表现就是选项表（Dropdown
 List）隐藏了。


Dropdown继承自Selectable和IPointerClickHandler, ISubmitHandler, ICancelHandler三个接口。


Dropdown重写了Awake方法，新建了一个FloatTween类型的TweenRunner变量m_AlphaTweenRunner并初始化，这个变量在显示/隐藏选项表（Dropdown
 List）的时候执行透明度渐变效果。然后设置了m_CaptionImage是否可用，这个变量对应于编辑器里的Caption Image，如果选中的选项（Options）设置了图片的话，就会使用m_CaptionImage显示在Dropdown的标题上。最后设置m_Template为false，这个变量对应于Template对象，用于作为模板实例化选项表。


OnPointerClick（继承自IPointerClickHandler，点击时）和OnSubmit（继承自ISubmitHandler，确认键按下时）调用了Show方法，而ICancelHandler（继承自ICancelHandler，取消键按下时）调用了Hide方法。


Show是Dropdown里最重要的一个方法，虽然很长但是值得贴一下。



```
// Show the dropdown.
        //
        // Plan for dropdown scrolling to ensure dropdown is contained within screen.
        //
        // We assume the Canvas is the screen that the dropdown must be kept inside.
        // This is always valid for screen space canvas modes.
        // For world space canvases we don't know how it's used, but it could be e.g. for an in-game monitor.
        // We consider it a fair constraint that the canvas must be big enough to contains dropdowns.
        public void Show()
        {
            if (!IsActive() || !IsInteractable() || m_Dropdown != null)
                return;

            if (!validTemplate)
            {
                SetupTemplate();
                if (!validTemplate)
                    return;
            }

            // Get root Canvas.
            var list = ListPool<Canvas>.Get();
            gameObject.GetComponentsInParent(false, list);
            if (list.Count == 0)
                return;
            Canvas rootCanvas = list[0];
            ListPool<Canvas>.Release(list);

            m_Template.gameObject.SetActive(true);

            // Instantiate the drop-down template
            m_Dropdown = CreateDropdownList(m_Template.gameObject);
            m_Dropdown.name = "Dropdown List";
            m_Dropdown.SetActive(true);

            // Make drop-down RectTransform have same values as original.
            RectTransform dropdownRectTransform = m_Dropdown.transform as RectTransform;
            dropdownRectTransform.SetParent(m_Template.transform.parent, false);

            // Instantiate the drop-down list items

            // Find the dropdown item and disable it.
            DropdownItem itemTemplate = m_Dropdown.GetComponentInChildren<DropdownItem>();

            GameObject content = itemTemplate.rectTransform.parent.gameObject;
            RectTransform contentRectTransform = content.transform as RectTransform;
            itemTemplate.rectTransform.gameObject.SetActive(true);

            // Get the rects of the dropdown and item
            Rect dropdownContentRect = contentRectTransform.rect;
            Rect itemTemplateRect = itemTemplate.rectTransform.rect;

            // Calculate the visual offset between the item's edges and the background's edges
            Vector2 offsetMin = itemTemplateRect.min - dropdownContentRect.min + (Vector2)itemTemplate.rectTransform.localPosition;
            Vector2 offsetMax = itemTemplateRect.max - dropdownContentRect.max + (Vector2)itemTemplate.rectTransform.localPosition;
            Vector2 itemSize = itemTemplateRect.size;

            m_Items.Clear();

            Toggle prev = null;
            for (int i = 0; i < options.Count; ++i)
            {
                OptionData data = options[i];
                DropdownItem item = AddItem(data, value == i, itemTemplate, m_Items);
                if (item == null)
                    continue;

                // Automatically set up a toggle state change listener
                item.toggle.isOn = value == i;
                item.toggle.onValueChanged.AddListener(x => OnSelectItem(item.toggle));

                // Select current option
                if (item.toggle.isOn)
                    item.toggle.Select();

                // Automatically set up explicit navigation
                if (prev != null)
                {
                    Navigation prevNav = prev.navigation;
                    Navigation toggleNav = item.toggle.navigation;
                    prevNav.mode = Navigation.Mode.Explicit;
                    toggleNav.mode = Navigation.Mode.Explicit;

                    prevNav.selectOnDown = item.toggle;
                    prevNav.selectOnRight = item.toggle;
                    toggleNav.selectOnLeft = prev;
                    toggleNav.selectOnUp = prev;

                    prev.navigation = prevNav;
                    item.toggle.navigation = toggleNav;
                }
                prev = item.toggle;
            }

            // Reposition all items now that all of them have been added
            Vector2 sizeDelta = contentRectTransform.sizeDelta;
            sizeDelta.y = itemSize.y * m_Items.Count + offsetMin.y - offsetMax.y;
            contentRectTransform.sizeDelta = sizeDelta;

            float extraSpace = dropdownRectTransform.rect.height - contentRectTransform.rect.height;
            if (extraSpace > 0)
                dropdownRectTransform.sizeDelta = new Vector2(dropdownRectTransform.sizeDelta.x, dropdownRectTransform.sizeDelta.y - extraSpace);

            // Invert anchoring and position if dropdown is partially or fully outside of canvas rect.
            // Typically this will have the effect of placing the dropdown above the button instead of below,
            // but it works as inversion regardless of initial setup.
            Vector3[] corners = new Vector3[4];
            dropdownRectTransform.GetWorldCorners(corners);
            bool outside = false;
            RectTransform rootCanvasRectTransform = rootCanvas.transform as RectTransform;
            for (int i = 0; i < 4; i++)
            {
                Vector3 corner = rootCanvasRectTransform.InverseTransformPoint(corners[i]);
                if (!rootCanvasRectTransform.rect.Contains(corner))
                {
                    outside = true;
                    break;
                }
            }
            if (outside)
            {
                RectTransformUtility.FlipLayoutOnAxis(dropdownRectTransform, 0, false, false);
                RectTransformUtility.FlipLayoutOnAxis(dropdownRectTransform, 1, false, false);
            }

            for (int i = 0; i < m_Items.Count; i++)
            {
                RectTransform itemRect = m_Items[i].rectTransform;
                itemRect.anchorMin = new Vector2(itemRect.anchorMin.x, 0);
                itemRect.anchorMax = new Vector2(itemRect.anchorMax.x, 0);
                itemRect.anchoredPosition = new Vector2(itemRect.anchoredPosition.x, offsetMin.y + itemSize.y * (m_Items.Count - 1 - i) + itemSize.y * itemRect.pivot.y);
                itemRect.sizeDelta = new Vector2(itemRect.sizeDelta.x, itemSize.y);
            }

            // Fade in the popup
            AlphaFadeList(0.15f, 0f, 1f);

            // Make drop-down template and item template inactive
            m_Template.gameObject.SetActive(false);
            itemTemplate.gameObject.SetActive(false);

            m_Blocker = CreateBlocker(rootCanvas);
        }
```

Show的步骤：
1、调用SetupTemplate方法，设置模板。SetupTemplate方法里判断了一系列限定，接着为item对象添加了DropdownItem组件，并为DropdownItem的四个属性赋值，然后为自己添加Canvas组件，设置overrideSorting为true，并sortingOrder为30000，这可以让选项表尽可能的显示在最前面，然后添加GraphicRaycaster和CanvasGroup组件，为了接受到鼠标事件。

2、调用CreateDropdownList方法，以m_Template为模板创建m_Dropdown（选项表）。并为m_Dropdown修改名字，设置父对象。然后在子对象里找到DropdownItem保存为itemTemplate，以itemTemplate为模板，创建每一个Item（数据为OptionData，对应编辑器里的Options下的Option），为Item的Toggle的onValueChanged事件添加监听OnSelectItem（根据选中的Toggle，找到它在父对象Content中的Index，为Dropdown设置值value，并隐藏Dropdown
 List），最后设置导航。

3、根据Item的数量设置Content的尺寸，Content是Scroll
 Rect（祥参[UGUI内核大探究（十一）ScrollRect与ScrollBar](http://blog.csdn.net/ecidevilin/article/details/52564132)）里面用于显示内容的对象。并且如果Dropdown
 List的高度大于Content的高度，便修正它的高度与Content相同。然后判断Dropdown List的四角是否超出了rootCanvas（Dropdown最上层的Canvas）的边界，便翻转Dropdown List，这种时候，我们将会看到选项表在Dropdown的上面，如图。

![](https://img-blog.csdn.net/20160918090305709)


然后设置Item的位置和尺寸。


4、Alpha渐变（m_AlphaTweenRunner）显示Dropdown
 List，并将m_Template和itemTemplate设置为无效的。

5、调用CreateBlocker创建Blocker。Blocker在rootCanvas下一级，尺寸与rootCanvas相同，sortingOrder比Dropdown
 List的小1（29999）。添加了Image组件，颜色为全透明，添加了Button组件，添加了onClick的监听，回调Hide方法。由此我们可知道Blocker是用于阻挡住鼠标事件，即Dropdown List显示时，点击选项表以外的区域，都只是隐藏选项表，不会触发其他的组件。

Hide方法要简单的多：

```
// Hide the dropdown.
        public void Hide()
        {
            if (m_Dropdown != null)
            {
                AlphaFadeList(0.15f, 0f);
                StartCoroutine(DelayedDestroyDropdownList(0.15f));
            }
            if (m_Blocker != null)
                DestroyBlocker(m_Blocker);
            m_Blocker = null;
            Select();
        }
```
Alpha渐变隐藏Dropdown
 List，并在渐变结束后Destroy所有的Item和Dropdown List。接着DestroyBlocker。最后设置本对象为Select（高亮状态）。




Dropdown的值value是一个属性（Property），对应变量m_Value。它的set访问器（参考[C#语法小知识（六）属性与索引器](http://blog.csdn.net/ecidevilin/article/details/52525080)）里，会将参数值限定在0到options.Count（选项数量） - 1之间。刷新，并发送m_OnValueChanged事件（可在编辑器里设置）。

刷新Refresh方法里，会在options里找到value值对应的OptionData，为m_CaptionText设置文本和m_CaptionImage设置图片，即在Dropdown上显示选中的选项。



