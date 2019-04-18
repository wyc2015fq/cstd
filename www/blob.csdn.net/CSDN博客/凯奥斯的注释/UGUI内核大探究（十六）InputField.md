# UGUI内核大探究（十六）InputField - 凯奥斯的注释 - CSDN博客





2016年09月19日 09:09:43[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：7947
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# InputField是UGUI的重要组件，可以提供文本输入功能，是与用户交互的一个重要手段。我们可以在编辑器里，为OnValueChanged和OnEndEdit两个事件添加监听，这样就可以获得用户输入的文本。本文就探究一下InputField的实现原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


在编辑器新建一个InputField，它会附带两个子对象Placeholder和Text。

![](https://img-blog.csdn.net/20160919072946598)


Placeholder（占位符）是当输入文字为空时显示的提示文字，Text显示的便是输入文字。

我们先看一下InputField的继承关系：



```
public class InputField
        : Selectable,
        IUpdateSelectedHandler,
        IBeginDragHandler,
        IDragHandler,
        IEndDragHandler,
        IPointerClickHandler,
        ISubmitHandler,
        ICanvasElement
```

InputField重写了OnEnable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）方法，方法里设置了一些变量，然后调用了m_TextComponent的RegisterDirtyVerticesCallback方法两次，添加MarkGeometryAsDirty和UpdateLabel回调，最后再调用一下UpdateLabel。m_TextComponent对应的是Text对象的Text组件，而RegisterDirtyVerticesCallback则是Graphic的方法（Text间接继承自Graphic），当Graphic需要重建Mesh（将顶点数据设为脏的）的时候，会回调方法。MarkGeometryAsDirty方法将自己注册到CanvasUpdateRegistry的图像重建序列（参考[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)）。




```
/// <summary>
        /// Update the visual text Text.
        /// </summary>

        protected void UpdateLabel()
        {
            if (m_TextComponent != null && m_TextComponent.font != null && !m_PreventFontCallback)
            {
                // TextGenerator.Populate invokes a callback that's called for anything
                // that needs to be updated when the data for that font has changed.
                // This makes all Text components that use that font update their vertices.
                // In turn, this makes the InputField that's associated with that Text component
                // update its label by calling this UpdateLabel method.
                // This is a recursive call we want to prevent, since it makes the InputField
                // update based on font data that didn't yet finish executing, or alternatively
                // hang on infinite recursion, depending on whether the cached value is cached
                // before or after the calculation.
                //
                // This callback also occurs when assigning text to our Text component, i.e.,
                // m_TextComponent.text = processed;

                m_PreventFontCallback = true;

                string fullText;
                if (Input.compositionString.Length > 0)
                    fullText = text.Substring(0, m_CaretPosition) + Input.compositionString + text.Substring(m_CaretPosition);
                else
                    fullText = text;

                string processed;
                if (inputType == InputType.Password)
                    processed = new string(asteriskChar, fullText.Length);
                else
                    processed = fullText;

                bool isEmpty = string.IsNullOrEmpty(fullText);

                if (m_Placeholder != null)
                    m_Placeholder.enabled = isEmpty;

                // If not currently editing the text, set the visible range to the whole text.
                // The UpdateLabel method will then truncate it to the part that fits inside the Text area.
                // We can't do this when text is being edited since it would discard the current scroll,
                // which is defined by means of the m_DrawStart and m_DrawEnd indices.
                if (!m_AllowInput)
                {
                    m_DrawStart = 0;
                    m_DrawEnd = m_Text.Length;
                }

                if (!isEmpty)
                {
                    // Determine what will actually fit into the given line
                    Vector2 extents = m_TextComponent.rectTransform.rect.size;

                    var settings = m_TextComponent.GetGenerationSettings(extents);
                    settings.generateOutOfBounds = true;

                    cachedInputTextGenerator.Populate(processed, settings);

                    SetDrawRangeToContainCaretPosition(caretSelectPositionInternal);

                    processed = processed.Substring(m_DrawStart, Mathf.Min(m_DrawEnd, processed.Length) - m_DrawStart);

                    SetCaretVisible();
                }
                m_TextComponent.text = processed;
                MarkGeometryAsDirty();
                m_PreventFontCallback = false;
            }
        }
```

UpdateLabel方法会为字符串添加光标，然后根据字符串是否为空来设置Placeholder是否显示。如果是密码格式，把字符改为*，接着调用TextGenerator的Populate生成Mesh数据，然后调用SetDrawRangeToContainCaretPosition方法，根据TextGenerator和caretPos（光标位置）来计算m_DrawStart和m_DrawEnd，并根据这两个值取字符串的子字符串，设置光标可见（使用协程闪烁）。最后把加工过的字符串赋值给m_TextComponent的text属性，并调用MarkGeometryAsDirty。

InputField重写OnDisable方法，方法停止了光标闪烁，反激活了输入区域，解除了m_TextComponent的两个回调，解除了CanvasUpdateRegistry的注册，设置m_CachedInputRenderer（绘制光标和选中区域）的mesh为null，并Destroy了m_Mesh（光标和选中区域的Mesh）。

OnPointerDown也被重写，将自己的对象设为EventSystem的选中对象（会调用对象上的OnSelect方法），如果自己已经被选中了，那么就设置光标的位置，并调用UpdateLabel更新文本。


OnSelect方法激活输入区域，OnDeselect反激活输入区域。


Selectable的DoStateTransition也被重写，会让点击过后的输入区域一直处于高亮状态，直到输入区域被反激活。

OnUpdateSelected继承自IUpdateSelectedHandler接口，输入模块切换到StandaloneInputModule时会被调用（参考[UGUI内核大探究（三）输入模块](http://blog.csdn.net/ecidevilin/article/details/52528445)）。


OnBeginDrag继承自IBeginDragHandler接口，设置m_UpdateDrag为true。


OnDrag继承自IDragHandler接口，会根据鼠标位置设置选中区域，如果拖拽事件在InputField之外，便开启协程调用MouseDragOutsideRect方法，根据方向不断移动选中区域，并调用UpdateLabel方法更新。m_UpdateDrag为false或者拖拽事件回到InputField里面时，便停止协程。

OnEndDrag继承自IEndDragHandler接口，设置m_UpdateDrag为false。

OnPointerClick继承自IPointerClick接口，调用ActivateInputField激活输入区域。


OnSubmit继承自ISubmit接口（当确认键按下），设置m_ShouldActivateNextUpdate为true，会在LateUpdate里激活输入区域。

Rebuild继承自ICanvasElement接口，CanvasUpdateRegistry重建图像是会回调Rebuild方法。方法里调用了UpdateGeometry方法。





ActivateInputField方法会激活m_Keyboard（触屏键盘），并设置text。设置m_ShouldActivateNextUpdate为true。


DeactivateInputField会反激活m_Keyboard，并发送onEndEdit事件。然后调用MarkGeometryAsDirty等待重建。


UpdateGeometry方法会为InputField对象新建一个InputField
 Input Caret子对象，子对象添加了CanvasRenderer组件，用于显示光标或者选中区域。调用OnFillVBO方法为CanvasRenderer生成Mesh。

OnFillVBO会调用GenerateCursor生成光标Mesh，或者GenerateHightlight生成选中Mesh。GenerateCursor根据光标位置计算出四个顶点，GenerateHightlight根据选中区域的起始点和结束点计算出四个顶点。

LateUpdate方法会每一帧调用，在所有对象Update执行过后，如果m_ShouldActivateNextUpdate为true，便执行ActivateInputFieldInternal方法开启触屏键盘，并设置光标可见。如果m_Keyboard的输入文本与之前的不同，便会遍历字符根据内容类型（ContentType）判断输入是否合法。并根据文本长度限制裁剪字符串，然后发送onValueChange时间，并调用UpdateLabel方法更新文本。最后如果键盘结束输入，便会反激活输入区域。



