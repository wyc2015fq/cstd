# UGUI内核大探究（九）Image与RawImage - 凯奥斯的注释 - CSDN博客





2016年09月16日 20:56:35[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：22438标签：[UGUI																[Image																[图片																[图像																[Unity3D](https://so.csdn.net/so/search/s.do?q=Unity3D&t=blog)
个人分类：[UGUI](https://blog.csdn.net/ecidevilin/article/category/6399641)

所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)](https://so.csdn.net/so/search/s.do?q=图像&t=blog)




Image组件是UGUI里最常用的组件（可能没有之一），我们知道其实还有一个RawImage组件。那么二者的区别是什么呢？之前的文章[UGUI内核大探究（八）MaskableGraphic](http://blog.csdn.net/ecidevilin/article/details/52555253)中我们提到过，二者（连同Text）都继承自MaskableGraphic。但是同为图片组件，Image的代码有接近一千行之多，而RawImage却只有120行。

RawImage顾名思义，未加工的~~生肉~~图片，RawImage只为我们提供了修改UV的方法，除此之外都是继承自MaskableGraphic的方法。~~是一个清纯不做作的组件。~~

而Image提供了四种ImageType：Simple（普通）、Sliced（切割）、Tiled（平铺）、Filled（填充），而且它还是布局元素（ILayoutElement），可以被各种布局组（ILayoutGroup）所包含，将它和其他布局元素进行布局。

所以假如我们要在UI上添加图片的时候，不妨考虑一下你到底需要的是轻量级的RawImage还是功能丰富的Image。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


那么既然RawImage代码只有120行，那本文就先介绍RawImage。

最重要的一个方法：



```
protected override void OnPopulateMesh(VertexHelper vh)
        {
            Texture tex = mainTexture;
            vh.Clear();
            if (tex != null)
            {
                var r = GetPixelAdjustedRect();
                var v = new Vector4(r.x, r.y, r.x + r.width, r.y + r.height);

                {
                    var color32 = color;
                    vh.AddVert(new Vector3(v.x, v.y), color32, new Vector2(m_UVRect.xMin, m_UVRect.yMin));
                    vh.AddVert(new Vector3(v.x, v.w), color32, new Vector2(m_UVRect.xMin, m_UVRect.yMax));
                    vh.AddVert(new Vector3(v.z, v.w), color32, new Vector2(m_UVRect.xMax, m_UVRect.yMax));
                    vh.AddVert(new Vector3(v.z, v.y), color32, new Vector2(m_UVRect.xMax, m_UVRect.yMin));

                    vh.AddTriangle(0, 1, 2);
                    vh.AddTriangle(2, 3, 0);
                }
            }
        }
```

OnPopulateMesh实在Graphic中被调用的（祥参[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)），它为CanvasRenderer的Mesh提供了顶点、顶点颜色、UV和三角形信息（关于Mesh可以参考[Unity3D
 Mesh小课堂](http://blog.csdn.net/column/details/12888.html)）。关于UV，可以参考[Unity3D
 Mesh小课堂（二）为三角形添加纹理](http://blog.csdn.net/ecidevilin/article/details/52443127)，这里不做赘述，只是简单演示一下：


我们为一个圆形的生肉图片，设置UV矩形，x=0.5，w=0.5，这样我们就看到了一个被压扁的半圆：

![](https://img-blog.csdn.net/20160916175715465)


如果把四个值x,y,w,h都改成0.5，我们就可以看到一个四分之一圆：

![](https://img-blog.csdn.net/20160916175726872)


我们点击Set Native Size按钮，就可以看到，四分之一圆变小了（宽高各减少一半）。那么我们就引入下一个~~比较~~重要的方法：



```
public override void SetNativeSize()
        {
            Texture tex = mainTexture;
            if (tex != null)
            {
                int w = Mathf.RoundToInt(tex.width * uvRect.width);
                int h = Mathf.RoundToInt(tex.height * uvRect.height);
                rectTransform.anchorMax = rectTransform.anchorMin;
                rectTransform.sizeDelta = new Vector2(w, h);
            }
        }
```

很好理解，不再赘述。





分析完轻量级的RawImage，我们便要分析近1000行代码的Image了。

还是先看OnPopulateMesh：



```
protected override void OnPopulateMesh(VertexHelper toFill)
        {
            if (overrideSprite == null)
            {
                base.OnPopulateMesh(toFill);
                return;
            }

            switch (type)
            {
                case Type.Simple:
                    GenerateSimpleSprite(toFill, m_PreserveAspect);
                    break;
                case Type.Sliced:
                    GenerateSlicedSprite(toFill);
                    break;
                case Type.Tiled:
                    GenerateTiledSprite(toFill);
                    break;
                case Type.Filled:
                    GenerateFilledSprite(toFill, m_PreserveAspect);
                    break;
            }
        }
```
这里根据图片设置的不同类型，要生成不同的顶点、顶点颜色、UV和三角形信息，所以分成了四个方法各自计算。


GenerateSimpleSprite方法是最简单的，与RawImage类似，只不过会根据精灵（sprite）的间隙（padding）和RectTransform的尺寸再计算一下顶点，并根据精灵的外侧UV信息（GetOuterUV）来设置UV。

GenerateSlicedSprite会生成36个uv点，如下图：


![](https://img-blog.csdn.net/20160916181806083)


九宫格里每一个格子对应四个uv点。

uv点就36个，顶点也就有36个：

![](https://img-blog.csdn.net/20160916182838498)


同样每个格子对应四个顶点（大致画一下格子）。

由此我们就可以看出实际上是中心区域被拉大了（四角部分不变，上下部分拉宽，左右部分拉长）。

我们取消Fill Center的时候会看到：

![](https://img-blog.csdn.net/20160916183225080)


这是因为这种情况下中心格子的顶点、uv等信息都没有。

GenerateTiledSprite方法如果sprite有边界，那么便会生成跟GenerateSlicedSprite一样的结果（不知道为什么会有这样的设定，虽然貌似很合理，但是体验却不怎么好，而且我也搞不明白为什么要写两份相同功能的代码），如果没有边界，那么就会出现下面这种结果：


![](https://img-blog.csdn.net/20160916185240738)


因为GenerateTiledSprite会计算该区域里可以放下多少个精灵单元（横纵分别向上取整），假设为格子数N，便会有4N个顶点，如果一个小格子可以完整的放下一个精灵单元，uv值便是x从0到1y从0到1的完整纹理坐标。而如果只能放下一部分，那边根据百分比计算uv值。

GenerateFilledSprite是最长的一个方法，因为它还区分了不同的填充方法（Horizontal,Vertical,Radial 90,Radial 180,Radial 360）。


但是分析起来却没那么复杂，Horizontal和Vertical很简单，只需要根据Fill Origin和Fill Amount设置矩形的顶点和UV值。而Radial系列方法不过是根据几何方法，来计算出顶点和UV值。（有兴趣的可以详细研究一下）




除了MaskableGraphic，Image还继承了ISerializationCallbackReceiver, ILayoutElement, ICanvasRaycastFilter三个接口。

ISerializationCallbackReceiver需要实现OnBeforeSerialize（序列化之前）和OnAfterDeserialize（发序列化之后）两个方法。我们知道UnityEditor会把我们设置的参数保存在Scene或者Prefab里面，这就是序列化，加载或运行的时候，需要从Scene或者Prefab读出来，这就是反序列化。


Image的OnBeforeSerialize是个空方法，OnAfterDeserialize里则是矫正了Fill
 Origin和Fill Amount为有效值。

ILayoutElement是布局元素，需要实现一些属性（Property），用于调节尺寸时做参考。具体细节，后续文章介绍Layout的时候再详细介绍。

最后还有一个ICanvasRaycastFilter，它是UnityEngine命名空间下的接口。它在[Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)中被调用，用于筛选出被射线照射到的图像。需要实现接口IsRaycastLocationValid。

Image的IsRaycastLocationValid接口将输入点转化为Image的内部点，然后在转换为Sprite纹理上的内部点，然后判断该点上的alpha值是否大于等于m_EventAlphaThreshold。这个值对应于可读写的属性eventAlphaThreshold。我们可以外部修改这个值，例如改成0.5f，那么点击到alpha小于0.5f的像素点就无法接收事件（参考[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)）。](https://so.csdn.net/so/search/s.do?q=图片&t=blog)](https://so.csdn.net/so/search/s.do?q=Image&t=blog)](https://so.csdn.net/so/search/s.do?q=UGUI&t=blog)




