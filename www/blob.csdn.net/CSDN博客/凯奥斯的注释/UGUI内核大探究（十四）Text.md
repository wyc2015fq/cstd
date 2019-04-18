# UGUI内核大探究（十四）Text - 凯奥斯的注释 - CSDN博客





2016年09月18日 21:54:44[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4639
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# Text是UGUI里非常常用的一个组件，可以根据字符串显示文字。但其实Text的代码并没有很多，因为大部分逻辑是在TextGenerator里实现的，而TextGenerator是UnityEngine命名空间下的类。呃(⊙o⊙)…好吧，乐观一点，至少我们就不需要再为底层的细节而苦恼。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


虽然TextGenerator的实现被Unity官方藏起来了，但是我们可以借助TextMesh来管中窥豹。

新建一个3D
 Text，如图：

![](https://img-blog.csdn.net/20160918210607586)


我们看到它为每个字幕画了一个矩形框（两个三角形），由此我们可猜测TextGenerator根据font文件里每一个字的大小和偏移量创建四个顶点和两个三角形，并按照font文件里每个字所在的位置设置uv坐标。

这就是TextGenerator的Populate方法，它在Text的OnPopulateMesh方法中被调用到。



```
protected override void OnPopulateMesh(VertexHelper toFill)
        {
            if (font == null)
                return;

            // We don't care if we the font Texture changes while we are doing our Update.
            // The end result of cachedTextGenerator will be valid for this instance.
            // Otherwise we can get issues like Case 619238.
            m_DisableFontTextureRebuiltCallback = true;

            Vector2 extents = rectTransform.rect.size;

            var settings = GetGenerationSettings(extents);
            cachedTextGenerator.Populate(text, settings);

            Rect inputRect = rectTransform.rect;

            // get the text alignment anchor point for the text in local space
            Vector2 textAnchorPivot = GetTextAnchorPivot(m_FontData.alignment);
            Vector2 refPoint = Vector2.zero;
            refPoint.x = (textAnchorPivot.x == 1 ? inputRect.xMax : inputRect.xMin);
            refPoint.y = (textAnchorPivot.y == 0 ? inputRect.yMin : inputRect.yMax);

            // Determine fraction of pixel to offset text mesh.
            Vector2 roundingOffset = PixelAdjustPoint(refPoint) - refPoint;

            // Apply the offset to the vertices
            IList<UIVertex> verts = cachedTextGenerator.verts;
            float unitsPerPixel = 1 / pixelsPerUnit;
            //Last 4 verts are always a new line...
            int vertCount = verts.Count - 4;

            toFill.Clear();
            if (roundingOffset != Vector2.zero)
            {
                for (int i = 0; i < vertCount; ++i)
                {
                    int tempVertsIndex = i & 3;
                    m_TempVerts[tempVertsIndex] = verts[i];
                    m_TempVerts[tempVertsIndex].position *= unitsPerPixel;
                    m_TempVerts[tempVertsIndex].position.x += roundingOffset.x;
                    m_TempVerts[tempVertsIndex].position.y += roundingOffset.y;
                    if (tempVertsIndex == 3)
                        toFill.AddUIVertexQuad(m_TempVerts);
                }
            }
            else
            {
                for (int i = 0; i < vertCount; ++i)
                {
                    int tempVertsIndex = i & 3;
                    m_TempVerts[tempVertsIndex] = verts[i];
                    m_TempVerts[tempVertsIndex].position *= unitsPerPixel;
                    if (tempVertsIndex == 3)
                        toFill.AddUIVertexQuad(m_TempVerts);
                }
            }
            m_DisableFontTextureRebuiltCallback = false;
        }
```
Text继承自MaskableGraphic，后者继承自Graphic，在Text里重写了Graphic的OnPopulateMesh方法（参考[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)），在重建图像的时候被调用到（UpdateGeometry方法）。
OnPopulateMesh方法里，根据用户的设置生成了一个TextGenerationSettings，然后调用了TextGenerator的Populate方法，生成mesh的顶点、顶点颜色、三角形和UV坐标。然后根据alignment获取textAnchorPivot（文本锚点轴心），并计算出偏移量（例如左对齐需要空出来左边）。最后遍历TextGenerator的顶点数组，将它们的位置除以pixelsPerUnit（像素每单元）并加上偏移量（如果有的话），得到的结果填到toFill（VertexHelper类型，Graphic用它来创建Mesh）里面。



另外在OnDisable、OnEnable(调用时机参见[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231))或者修改font的时候，会调用FontUpdateTracker的UntrackText和TrackText方法。FontUpdateTracker是个静态类，维护了一个Dictionary<Font, List<Text>>类型的变量m_Tracked。TrackText里将Text加入List，并且如果之前Dictionary是空的，UntrackText会将Text移出List。FontUpdateTracker会监听Font静态类的textureRebuilt事件，回调RebuildForFont。RebuildForFont方法会根据传入的font找到List<Text>，然后调用每一个Text的FontTextureChanged方法（UpdateGeometry更新几何形状重新计算Mesh或SetAllDirty重建布局、顶点和材质）。




顺便再补充一点关于FontData的代码。作为一个配置类，FontData本身没有什么可说的，但是它继承了ISerializationCallbackReceiver接口，这个接口需要实现OnBeforeSerialize和OnAfterDeserialize两个方法。这是两个很有趣的方法（虽然不是线程安全的，需要小心使用），它们分别会在序列化之前和反序列化之后调用。FontData的OnAfterDeserialize里便在反序列化之后，限定了m_FontSize、m_MinSize和m_MaxSize在0到300之间。



