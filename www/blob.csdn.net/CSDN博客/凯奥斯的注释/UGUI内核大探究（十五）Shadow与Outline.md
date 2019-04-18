# UGUI内核大探究（十五）Shadow与Outline - 凯奥斯的注释 - CSDN博客





2016年09月18日 22:27:40[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4354
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# 想在Unity3D里为一个图片或者文字添加阴影效果，只需要在该对象上在添加一个Shadow组件。Shadow通过为图像或者文字的Mesh添加顶点来实现阴影效果，而Outline是在对象四角上各添加了一个Shadow。由此我们可以得知，Outline的（额外）消耗是Shadow的四倍，所以还是需要谨慎使用。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


Outline继承自Shadow，Shadow继承自BaseMeshEffect，BaseMeshEffect继承自UIBehaviour和IMeshModifier。

[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)中提到过，Graphic在生成完Mesh数据之后，会查找对象上IMeshModifier类型的组件，调用它们的ModifyMesh方法，之后再正式生成Mesh。


BaseMeshEffect是一个抽象类，而ModifyMesh是一个抽象方法（以Mesh为参数的ModifyMesh方法是过期的不再讨论），具体在其子类里实现。它的OnEnable、OnDisable（调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）和OnDidApplyAnimationProperties（当应用动画属性后），会调用graphic属性的SetVerticesDirty方法（设置顶点数据为脏的，重建图像时重新生成顶点数据）。graphic是一个只读属性，获取的是本对象的Graphic组件（Image、RawImage或Text）。


Shadow的ModifyMesh方法：



```
public override void ModifyMesh(VertexHelper vh)
        {
            if (!IsActive())
                return;

            var output = ListPool<UIVertex>.Get();
            vh.GetUIVertexStream(output);

            ApplyShadow(output, effectColor, 0, output.Count, effectDistance.x, effectDistance.y);
            vh.Clear();
            vh.AddUIVertexTriangleStream(output);
            ListPool<UIVertex>.Release(output);
        }
```
获取VertexHelper（暂存顶点数据，用于辅助生成Mesh）里的顶点数据output。然后根据effectColor和effectDistance调整顶点。ApplyShadow会将顶点数乘以2，然后调用ApplyShadowZeroAlloc方法。遍历output的顶点，根据顶点生成一个新的顶点vt，添加到顶点列表里，并将vt的位置加上偏移量，并设置颜色。最后将output重新添加到VertexHelper里面（AddUIVertexTriangleStream会自行生成三角形）。





Outline的ModifyMesh方法：



```
public override void ModifyMesh(VertexHelper vh)
        {
            if (!IsActive())
                return;

            var verts = ListPool<UIVertex>.Get();
            vh.GetUIVertexStream(verts);

            var neededCpacity = verts.Count * 5;
            if (verts.Capacity < neededCpacity)
                verts.Capacity = neededCpacity;

            var start = 0;
            var end = verts.Count;
            ApplyShadowZeroAlloc(verts, effectColor, start, verts.Count, effectDistance.x, effectDistance.y);

            start = end;
            end = verts.Count;
            ApplyShadowZeroAlloc(verts, effectColor, start, verts.Count, effectDistance.x, -effectDistance.y);

            start = end;
            end = verts.Count;
            ApplyShadowZeroAlloc(verts, effectColor, start, verts.Count, -effectDistance.x, effectDistance.y);

            start = end;
            end = verts.Count;
            ApplyShadowZeroAlloc(verts, effectColor, start, verts.Count, -effectDistance.x, -effectDistance.y);

            vh.Clear();
            vh.AddUIVertexTriangleStream(verts);
            ListPool<UIVertex>.Release(verts);
        }
```


Outline将顶点数乘以了5，并在四角上各添加了一组（阴影）顶点。




补充：

除了Shadow和Outline以外，UGUI还内置了一个效果PositionAsUV1。

代码十分简单：

```
public class PositionAsUV1 : BaseMeshEffect
    {
        protected PositionAsUV1()
        {}

        public override void ModifyMesh(VertexHelper vh)
        {
            UIVertex vert = new UIVertex();
            for (int i = 0; i < vh.currentVertCount; i++)
            {
                vh.PopulateUIVertex(ref vert, i);
                vert.uv1 =  new Vector2(vert.position.x, vert.position.y);
                vh.SetUIVertex(vert, i);
            }
        }
    }
```
就是根据坐标点设置uv1坐标（[法线贴图](http://baike.baidu.com/link?url=QU0__Gt41GP34gyJj-ZyYmHVo0yiRaIMrelf4DgYO6MzvBVf2YahkuFbuzAD3J-WzzcHW-laHPDbDZSZgYmXLjX7HB-urFJ90vPM81fTGqVMBoHE3GlS8EqkKuqVxsWX)坐标），为图片或者文字添加法线贴图效果。
原理我们知道了，那么效果是怎么样的呢？

导入一张贴图：

![](https://img-blog.csdn.net/20160919203759581)


导入设置Texture Type为Normal map（法线贴图）。

我们新建一个Material，设置shader为UI/Unlit/Detail。将刚刚导入的法线贴图设置为Detail(RGB)。

在场景里新建一个Image，为它设置Material。我们会发现图片的颜色变深了一点，但是除此之外并没有变化。

接着为这个Image对象添加PositionAsUV1组件。如图：

![](https://img-blog.csdn.net/20160919204516059)




