# UGUI内核大探究（十八）Raycaster - 凯奥斯的注释 - CSDN博客





2016年09月19日 22:02:35[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：9352
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









# 射线其实是属于事件系统，它在EventSystem/Raycasters目录下，有BaseRaycaster、PhysicsRaycaster和Physics2DRaycaster三个类，命名空间也是UnityEngine.EventSystems。但是UI/Core目录下还有一个GraphicRaycaster文件，命名空间却是UnityEngine.UI。当我们在编辑器里新建（或间接新建）一个Canvas时，会为Canvas添加GraphicRaycaster组件，而PhysicsRaycaster和Physics2DRaycaster似乎在UGUI里没有被添加过。当然，我们在编辑器里可以添加这两个组件。本文就讨论一下这些射线照射器的原理。



按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


BaseRaycaster是其他Raycaster的基类，是一个抽象类。在它OnEnable（调用时机参考[Untiy3D组件小贴士（一）OnEnabled与OnDisabled](http://blog.csdn.net/ecidevilin/article/details/52504231)）里将自己注册到RaycasterManager，并在OnDisable的时候从后者移除。

RaycasterManager是一个静态类，维护了一个BaseRaycaster类型的List。EventSystem（参考[UGUI内核大探究（一）EventSystem](http://blog.csdn.net/ecidevilin/article/details/52503595)）里也通过这个类来管理所有的射线照射器。

PhysicsRaycaster（物理射线照射器）添加了特性



`[RequireComponent(typeof(Camera))]`
说明它依赖于Camera组件。它通过eventCamera属性来获取对象上的Camera组件。
Raycast方法重写了BaseRaycaster的同名抽象方法：




```
public override void Raycast(PointerEventData eventData, List<RaycastResult> resultAppendList)
        {
            if (eventCamera == null)
                return;

            var ray = eventCamera.ScreenPointToRay(eventData.position);
            float dist = eventCamera.farClipPlane - eventCamera.nearClipPlane;

            var hits = Physics.RaycastAll(ray, dist, finalEventMask);

            if (hits.Length > 1)
                System.Array.Sort(hits, (r1, r2) => r1.distance.CompareTo(r2.distance));

            if (hits.Length != 0)
            {
                for (int b = 0, bmax = hits.Length; b < bmax; ++b)
                {
                    var result = new RaycastResult
                    {
                        gameObject = hits[b].collider.gameObject,
                        module = this,
                        distance = hits[b].distance,
                        worldPosition = hits[b].point,
                        worldNormal = hits[b].normal,
                        screenPosition = eventData.position,
                        index = resultAppendList.Count,
                        sortingLayer = 0,
                        sortingOrder = 0
                    };
                    resultAppendList.Add(result);
                }
            }
        }
```

通过Physics.RaycastAll来获取所有被照射到的对象（finalEventMask是通过将Camera的cullingMask属性和编辑器设置中的EventMask属性做与运算获得的）。根据距离进行排序，然后包装成RaycastResult结构，加入到resultAppendList里面。EventSystem会将所有的Raycast的照射结果合在一起并排序，然后输入模块（参考[UGUI内核大探究（三）输入模块](http://blog.csdn.net/ecidevilin/article/details/52528445)）取到第一个结果的对象（距离最短）作为受输入事件影响的对象。
Physics2DRaycaster继承自PhysicsRaycaster，其他都一样，只重写了Raycast方法：




```
public override void Raycast(PointerEventData eventData, List<RaycastResult> resultAppendList)
        {
            if (eventCamera == null)
                return;

            var ray = eventCamera.ScreenPointToRay(eventData.position);

            float dist = eventCamera.farClipPlane - eventCamera.nearClipPlane;

            var hits = Physics2D.RaycastAll(ray.origin, ray.direction, dist, finalEventMask);

            if (hits.Length != 0)
            {
                for (int b = 0, bmax = hits.Length; b < bmax; ++b)
                {
                    var sr = hits[b].collider.gameObject.GetComponent<SpriteRenderer>();

                    var result = new RaycastResult
                    {
                        gameObject = hits[b].collider.gameObject,
                        module = this,
                        distance = Vector3.Distance(eventCamera.transform.position, hits[b].transform.position),
                        worldPosition = hits[b].point,
                        worldNormal = hits[b].normal,
                        screenPosition = eventData.position,
                        index = resultAppendList.Count,
                        sortingLayer =  sr != null ? sr.sortingLayerID : 0,
                        sortingOrder = sr != null ? sr.sortingOrder : 0
                    };
                    resultAppendList.Add(result);
                }
            }
        }
```

改为用Physics2D.RaycastAll来照射对象，并且根据SpriteRenderer组件设置结果变量（在EventSystem里会作为排序依据，毕竟是2D对象）。
GraphicRaycaster继承自BaseRaycaster，它添加了特性：


`[RequireComponent(typeof(Canvas))]`
表示它依赖于Canvas组件（通过canvas属性来获取）。
它重写了三个属性sortOrderPriority、renderOrderPriority（获取Canvas的sortingOrder和renderOrder，这在EventSystem里会作为排序依据，呃……毕竟是UI）和eventCamera（获取canvas.worldCamera，为null则返回Camera.main）。

Raycast方法：




```
[NonSerialized] private List<Graphic> m_RaycastResults = new List<Graphic>();
        public override void Raycast(PointerEventData eventData, List<RaycastResult> resultAppendList)
        {
            if (canvas == null)
                return;

            // Convert to view space
            Vector2 pos;
            if (eventCamera == null)
                pos = new Vector2(eventData.position.x / Screen.width, eventData.position.y / Screen.height);
            else
                pos = eventCamera.ScreenToViewportPoint(eventData.position);

            // If it's outside the camera's viewport, do nothing
            if (pos.x < 0f || pos.x > 1f || pos.y < 0f || pos.y > 1f)
                return;

            float hitDistance = float.MaxValue;

            Ray ray = new Ray();

            if (eventCamera != null)
                ray = eventCamera.ScreenPointToRay(eventData.position);

            if (canvas.renderMode != RenderMode.ScreenSpaceOverlay && blockingObjects != BlockingObjects.None)
            {
                float dist = 100.0f;

                if (eventCamera != null)
                    dist = eventCamera.farClipPlane - eventCamera.nearClipPlane;

                if (blockingObjects == BlockingObjects.ThreeD || blockingObjects == BlockingObjects.All)
                {
                    RaycastHit hit;
                    if (Physics.Raycast(ray, out hit, dist, m_BlockingMask))
                    {
                        hitDistance = hit.distance;
                    }
                }

                if (blockingObjects == BlockingObjects.TwoD || blockingObjects == BlockingObjects.All)
                {
                    RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction, dist, m_BlockingMask);

                    if (hit.collider != null)
                    {
                        hitDistance = hit.fraction * dist;
                    }
                }
            }

            m_RaycastResults.Clear();
            Raycast(canvas, eventCamera, eventData.position, m_RaycastResults);

            for (var index = 0; index < m_RaycastResults.Count; index++)
            {
                var go = m_RaycastResults[index].gameObject;
                bool appendGraphic = true;

                if (ignoreReversedGraphics)
                {
                    if (eventCamera == null)
                    {
                        // If we dont have a camera we know that we should always be facing forward
                        var dir = go.transform.rotation * Vector3.forward;
                        appendGraphic = Vector3.Dot(Vector3.forward, dir) > 0;
                    }
                    else
                    {
                        // If we have a camera compare the direction against the cameras forward.
                        var cameraFoward = eventCamera.transform.rotation * Vector3.forward;
                        var dir = go.transform.rotation * Vector3.forward;
                        appendGraphic = Vector3.Dot(cameraFoward, dir) > 0;
                    }
                }

                if (appendGraphic)
                {
                    float distance = 0;

                    if (eventCamera == null || canvas.renderMode == RenderMode.ScreenSpaceOverlay)
                        distance = 0;
                    else
                    {
                        Transform trans = go.transform;
                        Vector3 transForward = trans.forward;
                        // http://geomalgorithms.com/a06-_intersect-2.html
                        distance = (Vector3.Dot(transForward, trans.position - ray.origin) / Vector3.Dot(transForward, ray.direction));

                        // Check to see if the go is behind the camera.
                        if (distance < 0)
                            continue;
                    }

                    if (distance >= hitDistance)
                        continue;

                    var castResult = new RaycastResult
                    {
                        gameObject = go,
                        module = this,
                        distance = distance,
                        screenPosition = eventData.position,
                        index = resultAppendList.Count,
                        depth = m_RaycastResults[index].depth,
                        sortingLayer =  canvas.sortingLayerID,
                        sortingOrder = canvas.sortingOrder
                    };
                    resultAppendList.Add(castResult);
                }
            }
        }
```
首先将屏幕点转换为Camera的视窗坐标，用于判断是否在视窗外。然后根据blockingObjects来选择Physics或RaycastHit2D，不过这里只是用来计算距离hitDistance。然后调用静态方法Raycast获取屏幕点在其区域内的Graphic的列表m_RaycastResults（会调用Graphic的Raycast方法，参考[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)）。接着遍历m_RaycastResults，判断Graphic的方向向量和Camera的方向向量是否相交，然后判断Graphic是否在Camera的前面，并且距离小于等于hitDistance，满足了这些条件，才会把它打包成RaycastResult添加到resultAppendList里。
由此可见GraphicRaycaster与其他射线照射器的区别就在于，它把照射对象限定为了Graphic，这也是UGUI里的常规用法。



