
# unity3d简介 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d简介](https://www.cnblogs.com/eniac1946/p/7209773.html)
|Posted on|2017-07-20 10:25|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7209773)|[收藏](#)
|一．介绍：
|Unity3D软件：综合开发环境，实时三维动画等类型的多媒体内容，并支持这些内容在Windows、iOS、Android等多种平台的发布。
|Mono：脚本编程基于Mono技术，可使用JavaScript、C\#或Boo语言编写，用来对基于可视化编辑界面的基础开发方式进行功能扩展。
|一个基于Unity3D引擎的游戏，程序结构：
|VR/3D App|
|Unity3D engine|
|mono|
|native code|
|以windows系统为例，3D App与 U3D engine之间通过调用U3D api
|U3D engine主要包括，托管程序（managed文件夹，其中包含UnityEngine.dll和其他xxx.dll）与ASSETS文件（xxx.assets）
|mono文件夹中，主要是mono.dll
|native code是CPU相关的机器码
|二．基本概念
|组件（|Component|）|∈|游戏对象（|GameObject|）|∈|场景（|Scene|）|∈|工程（|Project|）
|Component|构建游戏对象的基础单元，为游戏对象添加特定的功能；
|GameObject|构建游戏的基础（独立）单元
|Scene|单个场景作为一个游戏关卡或游戏主菜单
|Project|单个开发项目
|资源（|Asset|）|：|表示材质、纹理、音频文件、游戏对象等在开发过程中可使用的资源。
|预制件（|Prefab|）：|相当于为这些重复物体创建一个模板；
|脚本（|Script|）：|是游戏业务逻辑的实现。脚本也是一种组件；
|相机（|Camera|）|：在屏幕上所看到的一切均是通过相机视角来展示的
|灯光（|Light|）|：为场景渲染出不同的气氛

|三．视图菜单
![](https://images2015.cnblogs.com/blog/1181483/201707/1181483-20170720102339005-2128836243.png)

|四、坐标系统
|世界坐标（|World Space|）
|视口坐标（|ViewPort Space|）|：坐标点被归一化，并且相对于相机建立坐标系。以相机左下角为(0,0)点，右上角为(1,1)点，Z轴使用相机的世界坐标单位。
|屏幕坐标（Screen Space|）|：坐标点使用像素点进行定义，并且相对于屏幕建立坐标系。以屏幕左下角为（0,0）点，右上角为（pixelWidth,pixelHeight）点，Z轴使用相机的世界坐标单位。
|局部坐标（|Local Space|）|：使用所选择对象的坐标系统。
|五、资源
|网格（|Mesh|）
|材质（|Material|）|：物体表面最基础的材料，如木质、塑料、金属或者玻璃等
|纹理（|Texture|）|：物体表面呈现的线形纹路，是在材质基础上的丰富细节呈现。
|贴图（Map|）|：一种将图片信息投影到曲面的方法。
|六．脚本交互
|图中内容可简化为这样的方法执行次序：Awake --> Start --> Update --> FixedUpdate --> LateUpdate --> OnGUI --> Reset -->  OnDestory
|下图描述了脚本生命周期的脚本方法调用流程与时机。
![](https://images2015.cnblogs.com/blog/1181483/201707/1181483-20170720102404646-749462452.png)
|Awake|：用于在游戏开始之前初始化变量或游戏状态，在脚本整个生命周期内仅被执行一次。Awake在所有游戏对象初始化之后执行，因此可以在方法中安全地与游戏对象进行通信。
|Start|：仅在所有脚本的Update方法第一次被调用前执行，且仅在脚本实例被启用时执行。Start在所有脚本的Awake方法全部执行完成后才执行。
|Update|：在每次渲染新的一帧时执行。由于该方法调用的频率与设备性能、被渲染对象有关，导致同一游戏在不同机器的效果不一致（因为Update方法的执行时间间隔不一致）。
|FixedUpdate|：在固定的时间间隔执行，不受游戏帧率的影响。所以处理RigidBody时最好用FixedUpdate。FixedUpdate的时间间隔可在工程设置中更改（Edit --> Project Setting --> Time）。
|LateUpdate|：所有脚本的Update方法调用后执行。例如相机跟随即是在LateUpdate方法中实现。
|OnGUI|：在渲染和处理GUI事件时执行。
|Reset|：用户点击属性监视面板（Inspector）的Reset按钮或首次添加该组件时执行，仅在编辑模式下执行。
|OnDestroy|：当游戏对象将被销毁时执行。
|需要注意的是，必须所有脚本的Awake方法均执行完毕后才会开始执行这些脚本中的Start方法，而各脚本的Awake方法的调用顺序是随机的。因此，对象的创建可在Awake方法中实现，而对象的获取可在Start方法中实现，保证调用先后次序。
|一个控制object移动的脚本：（C\#编写）
![](https://images2015.cnblogs.com/blog/1181483/201707/1181483-20170720102425849-333426196.png)

|void Update()
|为Unity预先声明好的函数，游戏中的|每一帧都会调用Update（）
|七、unity后台是如何执行多个脚本的？
|Unity后台是如何执行脚本的了。每个脚本的Awake、Start、Update、LateUpdate、FixedUpdate等等，所有的方法在后台都会被|汇总到一起|：
|后台的Awake()
|{
|// 这里暂时按照上图中的脚本执行顺序，后面会谈到其实可以自定义该顺序的
|脚本2中的Awake();
|脚本1中的Awake();
|脚本0中的Awake();
|}
|后台的方法Awake、Update、LateUpdate等等，都是按照顺序，等所有游戏对象上脚本中的Awake执行完毕之后，再去执行Start、Update、LateUpdate等方法的。
|后台的Update()
|{
|// 这里暂时按照上图中的脚本执行顺序，后面会谈到其实可以自定义该顺序的
|脚本2中的Update();
|脚本1中的Update();
|脚本0中的Update();
|}







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
