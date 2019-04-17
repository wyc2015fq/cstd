# UGUI内核大探究（八）MaskableGraphic - 凯奥斯的注释 - CSDN博客





2016年09月16日 17:25:37[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：8560
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# MaskableGraphic是UGUI的核心组件，它继承自Graphic。MaskableGraphic是一个抽象类，它的派生类有RawImage、Image、Text。顾名思义，MaskableGraphic是可遮罩的图像。本文就介绍一下MaskableGraphic的实现原理。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。





MaskableGraphic重写了OnEnable、OnDisable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）、OnTransformParentChanged（当父对象改变）和OnCanvasHierarchyChanged（当画布层次改变）方法，这些方法间接继承自UIBehavior。这些方法里都设置了m_ShouldRecalculateStencil（是否需要重新计算模板）为true，并调用UpdateClipParent（更新裁剪的父对象）和SetMaterialDirty（设置材质为脏的）。OnDisable方法里还额外从StencilMaterial（静态方法，用于管理模板材质）移除了m_MaskMaterial（遮罩材质），并设置m_MaskMaterial为null。

UpdateClipParent方法使用MaskUtilities的GetRectMaskForClippable方法从父对象中找到RectMask2D组件。我们知道RectMask2D是一个可以根据RectTransform裁剪子对象的组件。如图：

![](https://img-blog.csdn.net/20160916142757970)


我们为Canvas添加了RectMask2D组件，超出Canvas矩形范围的部分就被裁剪掉了。

SetMaterialDirty是Graphic的方法，可以参考[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)。


StencilMaterial是一个静态类，负责管理模板材质。它维护了一个类型为MatEntry的List，外部可以调用Add、Remove和ClearAll方法来对这个List进行操作。


Add方法，会创建一个MatEntry，并将输入的baseMat以及其他参数赋值给MatEntry的变量，并创建一个赋值baseMat新的材质customMat。并将stencilID、operation等参数设置给customMat（实际上是shader的参数）。




MaskableGraphic继承了IClippable，需要实现RecalculateClipping、Cull和SetClipRect方法。这些方法在MaskUtilities和RectMask2D被调用，用于图像的裁剪。


RecalculateClipping方法调用了上文提到过的UpdateClipParent，找到父对象中的RectMask2D组件。它在MaskUtilities中的Notify2DMaskStateChanged被调用（遍历RectMask2D的所有子对象，找到IClippable接口的组件，调用RecalculateClipping方法），这个方法是在RectMask2D的OnEnable、OnDisable以及编辑器模式下的OnValidate方法中被调用。

Cull方法：



```
public virtual void Cull(Rect clipRect, bool validRect)
        {
            if (!canvasRenderer.hasMoved)
                return;

            var cull = !validRect || !clipRect.Overlaps(canvasRect);
            var cullingChanged = canvasRenderer.cull != cull;
            canvasRenderer.cull = cull;

            if (cullingChanged)
            {
                m_OnCullStateChanged.Invoke(cull);
                SetVerticesDirty();
            }
        }
```

如果输入参数validRect为false或者输入的clipRect与所属Canvas的矩形区域不重合，cull为true，并设置给canvasRenderer。canvasRenderer是Graphic里的属性，表示本对象上的CanvasRenderer组件。如果cull的值发生变化回调m_OnCullStateChanged（发送事件），并设置顶点为脏的（参考[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)），等待重新绘制。


SetClipRect方法很简单，只是根据输入validRect为canvasRenderer开启或关闭矩形裁剪（开启时传递参数clipRect）。





MaskableGraphic继承了IMaskable，需要实现RecalculateMasking方法。这个方法会在MaskUtilities中调用，用于图像的遮罩。方法名为NotifyStencilStateChanged（遍历Mask的所有子对象，找到IMaskable接口的组件，调用RecalculateMasking方法），在Mask的OnEnable、OnDisable以及编辑器模式下的OnValidate方法中被调用。

Mask是可以通过图片裁剪图片的组件，如图

![](https://img-blog.csdn.net/20160916172447805)


一个圆形图像的对象包含了一个子对象，是一个方形图像（实际上是默认图像），稍微错开一点。我们为圆形图像添加Mask组件，就看到方形图像超出圆形图像的部分没有被渲染出来。

RecalculateMasking这个方法只是设置了m_ShouldRecalculateStencil为true并SetMaterialDirty。




MaskableGraphic还继承了IMaterialModifier，需要实现GetModifiedMaterial方法。这个方法在Graphic中被调用，用于重建图像时，或许修改过的材质。



```
public virtual Material GetModifiedMaterial(Material baseMaterial)
        {
            var toUse = baseMaterial;

            if (m_ShouldRecalculateStencil)
            {
                var rootCanvas = MaskUtilities.FindRootSortOverrideCanvas(transform);
                m_StencilValue = maskable ? MaskUtilities.GetStencilDepth(transform, rootCanvas) : 0;
                m_ShouldRecalculateStencil = false;
            }

            // if we have a Mask component then it will
            // generate the mask material. This is an optimisation
            // it adds some coupling between components though :(
            if (m_StencilValue > 0 && GetComponent<Mask>() == null)
            {
                var maskMat = StencilMaterial.Add(toUse, (1 << m_StencilValue) - 1, StencilOp.Keep, CompareFunction.Equal, ColorWriteMask.All, (1 << m_StencilValue) - 1, 0);
                StencilMaterial.Remove(m_MaskMaterial);
                m_MaskMaterial = maskMat;
                toUse = m_MaskMaterial;
            }
            return toUse;
        }
```

如果需要重算模板，便会从根画布（或者父对象中overrideSorting为true的Canvas）获取模板的深度。


如果深度大于零，且本对象含有Mask组件，便将baseMaterial（即当前的material）加入StencilMaterial，并移除旧的m_MaskMaterial。最后，替换m_MaskMaterial并返回（似乎m_MaskMaterial这个变量只是为了从StencilMaterial移除才存在的）。

深度获取方法：



```
public static int GetStencilDepth(Transform transform, Transform stopAfter)
        {
            var depth = 0;
            if (transform == stopAfter)
                return depth;

            var t = transform.parent;
            var components = ListPool<Component>.Get();
            while (t != null)
            {
                t.GetComponents(typeof(Mask), components);
                for (var i = 0; i < components.Count; ++i)
                {
                    if (components[i] != null && ((Mask)components[i]).IsActive() && ((Mask)components[i]).graphic.IsActive())
                    {
                        ++depth;
                        break;
                    }
                }

                if (t == stopAfter)
                    break;

                t = t.parent;
            }
            ListPool<Component>.Release(components);
            return depth;
        }
```
简单来讲就是从本对象往上找，找到一个图像有效的Mask组件，便深度加1，直到找到stopAfter（也就是上面说的根画布或者父对象中overrideSorting为true的画布）。




接着顺带介绍一下RectMask2D和Mask两个类。

RectMask2D维护了一个类型为IClippable的List——m_ClipTargets。MaskableGraphic会在UpdateClipParent的时候，将自己从旧的RectMask2D的RemoveClippable方法，将自己从m_ClipTargets中移除，并调用新的RectMask2D方法，加入m_ClipTargets。而m_ClipTargets会在PerformClipping方法中被调用。

PerformClipping是继承自IClipper接口的方法。RectMask2D会在OnEnable的时候将自己注册到ClipperRegistry，并再OnDisable的时候从后者移除。ClipperRegistry是一个单例，是裁剪器的注册处。我们在[UGUI内核大探究（六）CanvasUpdateRegistry](http://blog.csdn.net/ecidevilin/article/details/52548865)中提到过它，当Layout（布局）重建结束后，会调用它的Cull方法，遍历每一个Clipper执行PerformClipping方法。




```
public virtual void PerformClipping()
        {
            // if the parents are changed
            // or something similar we
            // do a recalculate here
            if (m_ShouldRecalculateClipRects)
            {
                MaskUtilities.GetRectMasksForClip(this, m_Clippers);
                m_ShouldRecalculateClipRects = false;
            }

            // get the compound rects from
            // the clippers that are valid
            bool validRect = true;
            Rect clipRect = Clipping.FindCullAndClipWorldRect(m_Clippers, out validRect);
            if (clipRect != m_LastClipRectCanvasSpace)
            {
                for (int i = 0; i < m_ClipTargets.Count; ++i)
                    m_ClipTargets[i].SetClipRect(clipRect, validRect);

                m_LastClipRectCanvasSpace = clipRect;
                m_LastClipRectValid = validRect;
            }

            for (int i = 0; i < m_ClipTargets.Count; ++i)
                m_ClipTargets[i].Cull(m_LastClipRectCanvasSpace, m_LastClipRectValid);
        }
```

MaskUtilities中还剩一个方法没讲，就是GetRectMasksForClip，这个方法将自己和父对象中的所有有效的RectMask2D组件打包给m_Clippers.



Clipping的FindCullAndClipWorldRect方法将这些m_Clippers的canvasRect（最近的Canvas父对象的矩形区域）求交集，得到一个最小的裁剪区域（如果这个裁剪区域不合理，validRect便为false）。接着，如果新的出来的clipRect与旧的不同，那么遍历m_ClipTargets，为他们设置裁剪区域，就是我们前面讲的SetClipRect。最后调用所有IClippable的剔除方法（前面讲的Cull）。




Mask与MaskableGraphic一样，继承了IMaterialModifier接口。Mask的GetModifiedMaterial方法：



```
public virtual Material GetModifiedMaterial(Material baseMaterial)
        {
            if (graphic == null)
                return baseMaterial;

            var rootSortCanvas = MaskUtilities.FindRootSortOverrideCanvas(transform);
            var stencilDepth = MaskUtilities.GetStencilDepth(transform, rootSortCanvas);
            if (stencilDepth >= 8)
            {
                Debug.LogError("Attempting to use a stencil mask with depth > 8", gameObject);
                return baseMaterial;
            }

            int desiredStencilBit = 1 << stencilDepth;

            // if we are at the first level...
            // we want to destroy what is there
            if (desiredStencilBit == 1)
            {
                var maskMaterial = StencilMaterial.Add(baseMaterial, 1, StencilOp.Replace, CompareFunction.Always, m_ShowMaskGraphic ? ColorWriteMask.All : 0);
                StencilMaterial.Remove(m_MaskMaterial);
                m_MaskMaterial = maskMaterial;

                var unmaskMaterial = StencilMaterial.Add(baseMaterial, 1, StencilOp.Zero, CompareFunction.Always, 0);
                StencilMaterial.Remove(m_UnmaskMaterial);
                m_UnmaskMaterial = unmaskMaterial;
                graphic.canvasRenderer.popMaterialCount = 1;
                graphic.canvasRenderer.SetPopMaterial(m_UnmaskMaterial, 0);

                return m_MaskMaterial;
            }

            //otherwise we need to be a bit smarter and set some read / write masks
            var maskMaterial2 = StencilMaterial.Add(baseMaterial, desiredStencilBit | (desiredStencilBit - 1), StencilOp.Replace, CompareFunction.Equal, m_ShowMaskGraphic ? ColorWriteMask.All : 0, desiredStencilBit - 1, desiredStencilBit | (desiredStencilBit - 1));
            StencilMaterial.Remove(m_MaskMaterial);
            m_MaskMaterial = maskMaterial2;

            graphic.canvasRenderer.hasPopInstruction = true;
            var unmaskMaterial2 = StencilMaterial.Add(baseMaterial, desiredStencilBit - 1, StencilOp.Replace, CompareFunction.Equal, 0, desiredStencilBit - 1, desiredStencilBit | (desiredStencilBit - 1));
            StencilMaterial.Remove(m_UnmaskMaterial);
            m_UnmaskMaterial = unmaskMaterial2;
            graphic.canvasRenderer.popMaterialCount = 1;
            graphic.canvasRenderer.SetPopMaterial(m_UnmaskMaterial, 0);

            return m_MaskMaterial;
        }
```



与MaskableGraphic相比，多了stencilDepth大于等于8时的警告，以及desiredStencilBit等于1（stencilDepth等于0）时的特殊处理（无图可遮），并根据m_ShowMaskGraphic来设置遮罩图像是否显示出来，此外额外给canvasRenderer设置了一个材质。（似乎是为了上一级的CanvasRenderer渲染时方便调用，Unity3D官方喜欢藏着掖着，本来属于UI的CanvasRenderer却非要放在UnityEngine里面，所以无法知道SetPopMaterial具体实现了什么逻辑。docs.unity3d.com里关于这个方法的介绍也很含糊，我也没有搜索到相关的资料。等我研究出了结果，再把这一段介绍补完。）






另外RectMask2D和Mask都实现了IsRaycastLocationValid方法，继承自UnityEngine.ICanvasRaycastFilter，实现是判断矩形（RectTransform）区域是否包含输入的点。它在[Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)中被调用，用于筛选出被射线照射到的图像。



