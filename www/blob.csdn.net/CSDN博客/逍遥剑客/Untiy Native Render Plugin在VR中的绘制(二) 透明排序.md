# Untiy Native Render Plugin在VR中的绘制(二): 透明排序 - 逍遥剑客 - CSDN博客
2016年04月21日 10:34:23[xoyojank](https://me.csdn.net/xoyojank)阅读数：2733标签：[Unity																[VR																[Plugin																[Native](https://so.csdn.net/so/search/s.do?q=Native&t=blog)](https://so.csdn.net/so/search/s.do?q=Plugin&t=blog)](https://so.csdn.net/so/search/s.do?q=VR&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity&t=blog)
个人分类：[引擎研究																[虚拟现实](https://blog.csdn.net/xoyojank/article/category/5954851)](https://blog.csdn.net/xoyojank/article/category/337622)
上篇已经能在VR中画出来了, 但是还存在两个问题: 
1. 透明物体会被Native画的东西挡住 
2. VR中Native画的东西透视关系有点问题, 跟Unity绘制的场景不能很好地融合在一起
先来解决一个透明排序的问题, 这个问题有两个思路去解决:
# 双Camera
双Camera的思路就是, 一个Camera只画不透明物体, OnPostRender中回调Native Renderer, 另一个Camera只画透明物体. 这样Native的绘制就能在两者之间进行, 有几个细节:
- 先保证两个Camera的参数一样
- 第一个Camera的Culling Mask把TransparentFX去掉
- 第二个Camera的Culling Mask只保留TransparentFX
- 所有透明物体设置layer为TransparentFX
- 第二个Camera的Clear Flags设置为Don’t Clear, 这样两个Camera的画面才能结合到一起
- 第二个Camera的Depth设置成比第一个Camera大, 代表后画
# 插入CommandBuffer
Unity5.2在CommandBuffer入加入了调用NativeRenderPlugin的支持, 这就可以让我们可以在渲染管线的各个阶段之前插入我们想要的效果. 具体到我们的需求, 可以简化为:
```
private CommandBuffer commandBuffer;
    void Start()
    {
        SetUnityStreamingAssetsPath(Application.streamingAssetsPath);
        commandBuffer = new CommandBuffer();
        commandBuffer.IssuePluginEvent(GetRenderEventFunc(), 1);
        camera.AddCommandBuffer(CameraEvent.BeforeForwardOpaque, commandBuffer);
        GetComponent<MeshFilter>().mesh.bounds = new Bounds(Vector3.zero, Vector3.one * 1000);
    }
    void OnWillRenderObject()
    {
        // Set time for the plugin
        SetTimeFromUnity(Time.timeSinceLevelLoad);
        if (Camera.current != null)
        {
            var viewMatrix = Camera.current.worldToCameraMatrix;
            var projectionMatrix = GL.GetGPUProjectionMatrix(Camera.current.projectionMatrix, VRSettings.enabled);
            SetCameraMatrix(viewMatrix.toFloatArray(), projectionMatrix.toFloatArray(), VRSettings.enabled);
        }
    }
```
一个细节, OnWillRenderObject必须在有Mesh时才会调用, 所以这里挂了个Box到NativeRender的GameObject上, 并把包围盒设置成非常大, 一方面保证了SetCameraMatrix可以一直调用到, 另一方面也能”隐藏”掉这个Box(内部看经过back face culling是不可见的).
相对于双Camera的做法省去了为透明物体设置layer mask的工作, 也更为灵活和高效. 
