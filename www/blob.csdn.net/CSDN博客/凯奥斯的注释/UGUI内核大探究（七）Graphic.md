# UGUI内核大探究（七）Graphic - 凯奥斯的注释 - CSDN博客





2016年09月15日 12:38:47[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：10792
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# Graphic是UGUI的核心组件，负责显示图像。它是一个抽象类，是MaskableGraphic（可遮罩图像）的基类（同样是抽象类），而后者是RawImage、Image和Text的基类。虽然Selectable系列组件与Graphic没有明确的依赖关系，但是在[UGUI内核大探究（四）Selectable与Button](http://blog.csdn.net/ecidevilin/article/details/52534383)中，我们讲到Selectable使用Graphic（或Image）来显示状态变化。今天我们就探究一下Graphic的原理。



按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


Graphic的Attribute（特性）：



```
[DisallowMultipleComponent]
    [RequireComponent(typeof(CanvasRenderer))]
    [RequireComponent(typeof(RectTransform))]
    [ExecuteInEditMode]
```

DisallowMultipleComponent：不允许一个对象有两个相同的组件，即不能有两个Image或者一个RawImage和一个Text。



RequireComponent：依赖于CanvasRenderer（画布渲染器）和RectTransform（矩形变换）两类组件。


ExecuteInEditMode：在编辑模式执行。


Graphic是一个抽象类，这意味着不能创建它的实例，它为MaskableGraphic及其子类提供了公用的可继承的方法。

Graphic继承自UIBehaviour，后者也是一个抽象类，它是所有UI组件的基类，负责接收来自UnityEngine或者UnityEditor的事件。

Graphic还继承了ICanvasElement，后者是一个接口，负责接收Canvas重新渲染的事件（参考[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)）。




继承自UIBehaviour的方法：

OnEnable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）里调用了CacheCanvas方法，获取父对象中的Canvas组件。然后注册到GraphicRegistry（一个简单的注册处，可以获取指定Canvas所包含的Graphic）。接着设置s_WhiteTexture（对应属性MainTexture）。最后SetAllDirty（分别设置Layout布局、Vertices顶点和Material材质为Dirty）。


OnDisable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）里，从GraphicRegistry和CanvasUpdateRegistry移除注册并清理canvasRenderer，最后调用告知LayoutRebuilder需要重建布局（关于LayoutRebuilder，后续文章会详细讲解）。


OnRectTransformDimensionsChange（当RectTransform维度改变时）里，将顶点和布局设为Dirty。


OnBeforeTransformParentChanged（在父对象改变前）里，从GraphicRegistry移除注册，并告知LayoutRebuilder需要重建布局。


OnTransformParentChanged（当父对象改变时）里，重新获取父对象中的Canvas，重新注册到GraphicRegistry，并SetAllDirty。


OnDidApplyAnimationProperties（当应用动画属性后）里，SetAllDirty。


OnCanvasHierarchyChanged（当画布层次改变时）里，如果Canvas改变，重新注册到GraphicRegistry。





继承自ICanvasElement的方法（参考[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)）：


Rebuild，在Canvas渲染前被调用，在这个方法里会调用UpdateGeometry和UpdateMaterial更新顶点和材质。

UpdateGeometry里会根据useLegacyMeshGeneration这个bool值分别调用DoLegacyMeshGeneration或DoMeshGeneration。两个方法类似，我们就以DoMeshGeneration为例：



```
private void DoMeshGeneration()
        {
            if (rectTransform != null && rectTransform.rect.width >= 0 && rectTransform.rect.height >= 0)
                OnPopulateMesh(s_VertexHelper);
            else
                s_VertexHelper.Clear(); // clear the vertex helper so invalid graphics dont draw.

            var components = ListPool<Component>.Get();
            GetComponents(typeof(IMeshModifier), components);

            for (var i = 0; i < components.Count; i++)
                ((IMeshModifier)components[i]).ModifyMesh(s_VertexHelper);

            ListPool<Component>.Release(components);

            s_VertexHelper.FillMesh(workerMesh);
            canvasRenderer.SetMesh(workerMesh);
        }
```
OnPopulateMesh假装绘制了一个矩形Mesh（关于Mesh绘制，参考专栏[Unity3D
 Mesh小课堂](http://blog.csdn.net/column/details/12888.html)），实际上只是把顶点和三角形信息保存到了s_VertexHelper里。然后获取IMeshModifier类型的组件（IMeshModifier是一个接口，需要依据顶点信息的组件继承自它，例如Shadow就间接继承自它），调用它们的ModifyMesh方法，修改Mesh信息。最后将s_VertexHelper里修改后的信息赋值给workerMesh，并将workerMesh设置给canvasRenderer。



UpdateMaterial方法就简单的多。



```
protected virtual void UpdateMaterial()
        {
            if (!IsActive())
                return;

            canvasRenderer.materialCount = 1;
            canvasRenderer.SetMaterial(materialForRendering, 0);
            canvasRenderer.SetTexture(mainTexture);
        }
```

只是重设canvasRenderer的材质和纹理。


我们再看materialForRendering这个属性：



```
public virtual Material materialForRendering
        {
            get
            {
                var components = ListPool<Component>.Get();
                GetComponents(typeof(IMaterialModifier), components);

                var currentMat = material;
                for (var i = 0; i < components.Count; i++)
                    currentMat = (components[i] as IMaterialModifier).GetModifiedMaterial(currentMat);
                ListPool<Component>.Release(components);
                return currentMat;
            }
        }
```

获取修改后的材质，例如经过Mask（遮罩）处理后的材质。





我们再回到之前的SetAllDirty



```
public virtual void SetAllDirty()
        {
            SetLayoutDirty();
            SetVerticesDirty();
            SetMaterialDirty();
        }

        public virtual void SetLayoutDirty()
        {
            if (!IsActive())
                return;

            LayoutRebuilder.MarkLayoutForRebuild(rectTransform);

            if (m_OnDirtyLayoutCallback != null)
                m_OnDirtyLayoutCallback();
        }

        public virtual void SetVerticesDirty()
        {
            if (!IsActive())
                return;

            m_VertsDirty = true;
            CanvasUpdateRegistry.RegisterCanvasElementForGraphicRebuild(this);

            if (m_OnDirtyVertsCallback != null)
                m_OnDirtyVertsCallback();
        }

        public virtual void SetMaterialDirty()
        {
            if (!IsActive())
                return;

            m_MaterialDirty = true;
            CanvasUpdateRegistry.RegisterCanvasElementForGraphicRebuild(this);

            if (m_OnDirtyMaterialCallback != null)
                m_OnDirtyMaterialCallback();
        }
```

SetLayoutDirty里会通知LayoutRebuilder布局需要重建。SetVerticesDirty和SetMaterialDirty里会注册CanvasUpdateRegistry，等待重建Canvas时重建Graphic。



三者都会广播对应的事件（回调），可以通过RegisterDirtyLayoutCallback等方法添加事件监听。




在[UGUI内核大探究（四）Selectable与Button](http://blog.csdn.net/ecidevilin/article/details/52534383)里我们讲到，当Selectable状态变化的时候会调用Graphic的CrossFadeColor方法。



```
private void CrossFadeColor(Color targetColor, float duration, bool ignoreTimeScale, bool useAlpha, bool useRGB)
        {
            if (canvasRenderer == null || (!useRGB && !useAlpha))
                return;

            Color currentColor = canvasRenderer.GetColor();
            if (currentColor.Equals(targetColor))
                return;

            ColorTween.ColorTweenMode mode = (useRGB && useAlpha ?
                                              ColorTween.ColorTweenMode.All :
                                              (useRGB ? ColorTween.ColorTweenMode.RGB : ColorTween.ColorTweenMode.Alpha));

            var colorTween = new ColorTween {duration = duration, startColor = canvasRenderer.GetColor(), targetColor = targetColor};
            colorTween.AddOnChangedCallback(canvasRenderer.SetColor);
            colorTween.ignoreTimeScale = ignoreTimeScale;
            colorTween.tweenMode = mode;
            m_ColorTweenRunner.StartTween(colorTween);
        }
```

ColorTween是实现缓变效果的类，而m_ColorTweenRunner是在构造函数里创建的，是TweenRunner<ColorTween>的实例，是ColorTween的载体，通过协程来运行。






最后介绍Raycast这个方法，通过ICanvasRaycastFilter（Image继承了这个接口），判断射线在该组件是否有效。这个方法在GraphicRaycaster的Raycast方法里被使用到（之前提到的GraphicRegistry的GetGraphicsForCanvas方法也被调用到了），用于筛选出被射线照射到的Graphic。GraphicRaycaster继承自BaseRaycaster，我们在[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)介绍过，输入模块通过Raycast来获取被影响的对象。



