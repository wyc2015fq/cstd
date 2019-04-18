# 【GearVR】Unity VR Samples示例项目分析 - weixin_33985507的博客 - CSDN博客
2017年03月11日 16:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：50
**核心脚本和对象**
在VR场景中，我们需要激活我们正在看着的物体，高亮或者交互。在VRSamples中主要包括三个脚本：VREyeRaycaster, VRInput, 以及VRInteractiveItem。
VREyeRaycaster
![2772495-6de27125a1cdb8bd.png](https://upload-images.jianshu.io/upload_images/2772495-6de27125a1cdb8bd.png)
VREyeRaycaster.png
该脚本需要和Main Camera关联。在每次调用Update()时，该脚本都会使用Physics.Raycast来投射一条射线，以确认该射线是否命中任何collider（碰撞体）。使用该脚本还可以排除特定的Layer。在某些场景中，我们可能为了性能考虑，把所有的可交互对象移到一个单独的层。
如果某个碰撞体被射线命中，那么**该脚本将尝试在GameObject上找到一个VRInteractiveItem组件**。来看下该类的主要代码：
// 从相机位置向前发射一条射线
Ray ray = new Ray(m_Camera.position, m_Camera.forward);
RaycastHit hit;
//做射线投射，看是否有物体相交
if (Physics.Raycast(ray, out hit, m_RayLength, ~m_ExclusionLayers))
{
VRInteractiveItem interactible = hit.collider.GetComponent(); //获取相交物体上的VRInteractiveItem组件
m_CurrentInteractible = interactible;
//如果相交物体跟上次的物体不同，那么判断悬停
if (interactible && interactible != m_LastInteractible)
interactible.Over();
// 停用上次相交物体，该函数内置空
if (interactible != m_LastInteractible)
DeactiveLastInteractible();
m_LastInteractible = interactible;
//有物体相交的话，瞄准星的位置要更新。在该函数中，会根据距离做缩放；使用相交物体的法线方向做旋转。
if (m_Reticle)
m_Reticle.SetPosition(hit);
//调用事件处理函数
if (OnRaycasthit != null)
OnRaycasthit(hit);
}
else
{
//没有相交物体，置空上次使用的可交互物体
DeactiveLastInteractible();
m_CurrentInteractible = null;
//把瞄准星放到默认位置
if (m_Reticle)
m_Reticle.SetPosition();
}
该类在OnEnable函数中和OnDisable函数中会把VRInteractiveItem上的处理事件函数与VRInput中的事件绑定起来（订阅事件）。
VRInput
![2772495-c1fd92c1581df51e.png](https://upload-images.jianshu.io/upload_images/2772495-c1fd92c1581df51e.png)
image2.png
VRInput是个简单的类，可以判断用户在GearVR上（或是使用DK2时在PC上）所进行的一些简单操作，比如滑动、触碰、或双触。
VRInteractiveItem
我们可以把该组件添加到任何希望在VR场景中进行交互的GameObject上，在该物体上需要绑定一个碰撞体。
交互可视化元素SelectionRadial和SelectionSlider
我们同时利用了radial选择条（SelectionRadial），以及选择滑动条(SelectionSlider)，这样用户就可以按住Fire1来确认某个交互：
![2772495-1577c1378e1dbabe.png](https://upload-images.jianshu.io/upload_images/2772495-1577c1378e1dbabe.png)
image3.png
![2772495-c7878a0501995f64.png](https://upload-images.jianshu.io/upload_images/2772495-c7878a0501995f64.png)
image4.png
当按住输入键时，选择条会进行填充，并在填充完整后分发OnSelectionComplete或OnBarFilled事件。关于此部分的代码，可以在SelectionRadial.cs和SelectionSlider.cs中找到，并进行了详细的注释。
在VR的世界里，从用户交互的角度看，用户需要时刻知道自己在做什么，而且可以掌控一切。通过这种“held input”的确认输入方式，可以确保用户不会出现误操作。
VR Sample项目中的各个场景分析
**先来分析一下各个场景中对象和脚本的作用**
MainMenu场景
MainMenu场景可以认为是整个VR项目的入口，用户首先在这里选择目标场景，然后进入。因此非常重要。该场景中的主要交互如下：
Reticle只有瞄准星的效果：
![2772495-7cb6bfccd3dfbe2a.png](https://upload-images.jianshu.io/upload_images/2772495-7cb6bfccd3dfbe2a.png)
image5.png
当用户“凝视”菜单选项时，空白的Selection Radial可见。
![2772495-6eea1de334dabaa5.png](https://upload-images.jianshu.io/upload_images/2772495-6eea1de334dabaa5.png)
image6.png
Selection Radial 填充（当用户“凝视”菜单选项，且按下fire1输入键）
![2772495-b9e2634086d3ae6d.png](https://upload-images.jianshu.io/upload_images/2772495-b9e2634086d3ae6d.png)
image7.png
在整个示例项目中，都是这样的风格，也就是使用bar和radial以固定的速度进行填充。在此建议大家在开发自己的VR项目时注意到这一点，因为交互设计中的连贯性对用户很重要，特别是对于VR这种新媒介。
**Selector/MenuSelectorChild**对象是控制界面下边的瘦长进度条，根据用户目前凝视的对象（不同的菜单项），会把进度条移动到这些对象的下面，在鼠标移进移出的时候有一个popout的效果，根据几个public字段可以设置相应的动画参数。该脚本还收集了所有的菜单项，用他们的transform来作为目标位置的参考。
![2772495-fea9f1ac2fc6f7b7.png](https://upload-images.jianshu.io/upload_images/2772495-fea9f1ac2fc6f7b7.png)
image8.png
**VRCameraFade**是用来控制场景之间切换效果的脚本。包含场景淡入淡出的效果。Fade效果有两种，一种是FadeIn，从全黑色逐渐变为无色透明；一种是FadeOut，从无色透明逐渐变为全黑色。该组件public的字段中，FadeColor默认是全黑色，即FadeIn中的起始颜色和FadeOut中的终止颜色，该颜色会作用于FadeImage的颜色。另外一个颜色是代码中写死的，即无色透明。FadeDuration是指fade效果的持续时间。FadeInOnSceneLoad指的是当场景加载的时候，启用Fade效果，在Unity的OnLevelWasLoaded回调中会使用到。FadeInOnStart和上面的作用类似，但是它控制的是Fade效果在Start回调中使用。
![2772495-ab456b2232d9ff04.png](https://upload-images.jianshu.io/upload_images/2772495-ab456b2232d9ff04.png)
image10.png
**Menu/Items对象及其子对象**
**MenuButton**处理一个VRInteractiveItem和交互界面（瞄准星）的事件。主要是三个事件处理函数：HandleOver（reticle进入）、HandleOut（reticle移出）、HandleSelectionComplete（选择完成）。当redial的选择结束的时候，会调用HandleSelectionComplete函数，该函数启动一个Coroutine，让当前场景淡出，然后加载指定的场景（Scene To Load指定）。
![2772495-f17add0372b8090c.png](https://upload-images.jianshu.io/upload_images/2772495-f17add0372b8090c.png)
image11.png
**MenuItemPopout**控制用户凝视menu item时popout的效果。
![2772495-e6f4a06c2fed9bdf.png](https://upload-images.jianshu.io/upload_images/2772495-e6f4a06c2fed9bdf.png)
image12.png
**MenuAnimator**当用户凝视物体的时候，播放动画（实际上是一系列的纹理图片）。
![2772495-4e0040fc928a185e.png](https://upload-images.jianshu.io/upload_images/2772495-4e0040fc928a185e.png)
image13.png
**SelectionSlider**当用户凝视一个物体、并按住Fire键的时候，用来控制进度条的行为。这里的Renderer引用的就是上面提到的瘦长进度条，Selection Radial引用的是上面提到的瞄准星周围一圈进度条。并且通过VRInput对象的回调来处理输入的Down和Up事件，Down的时候开始改变Slider的value，up的时候清零。通过InteractiveItem对象的回调来处理可交互对象的slider充满和瞄准星悬停事件，这里是播放声音。
![2772495-bd5b7f7bb7c558c7.png](https://upload-images.jianshu.io/upload_images/2772495-bd5b7f7bb7c558c7.png)
image14.png
**多场景通用的对象**
**WarningTextCanvas**是当用户在场景中错误的输入之后，在场景中显示警告信息的界面，采用了world space的canvas界面。该脚本可以设置警告字符串，引用了VRInput对象，用来处理单击双击等事件。引用了Reticle对象，用它的transform信息来放置警告信息。引用Camera是为了让警告信息知道朝向哪里。
![2772495-b0cd82fbdb734855.png](https://upload-images.jianshu.io/upload_images/2772495-b0cd82fbdb734855.png)
image15.png
**MainCamera这是最重要的一个对象，包含了上述的很多代码组件：**
**VREyeRaycaster, VRInput, SelectionRadial和VRInteractiveItem**上面都已经介绍过了。
**VRCameraUI**脚本确保相机UI能正确的渲染。代码中把canvas的sortingOrder放到了最大值，然后强制刷新canvas。
**Reticle**是瞄准星类。它是屏幕中心的一个小红点（颜色可设定），用来瞄准当前场景中的物体。如果从相机发射出来的ray没有和物体（具有VRInteractiveItem组件）相交的时候，它在默认的位置上；如果有相交物体的话，它的位置在物体的表面。位置的设定是调用其中SetPosition函数进行实现的。Default Distance可以设置默认距离。Use Normal是设定是否根据相交物体的法线来设置Reticle的旋转，效果见下面的图。
![2772495-43325313e0af334d.png](https://upload-images.jianshu.io/upload_images/2772495-43325313e0af334d.png)
image16.png
下图展示了准星如何匹配墙壁的法线：
![2772495-b0a97f9a6a1d9d61.png](https://upload-images.jianshu.io/upload_images/2772495-b0a97f9a6a1d9d61.png)
image17.png
下图展示了准星如何匹配地板的法线：
![2772495-76d36a566b4b9f2c.png](https://upload-images.jianshu.io/upload_images/2772495-76d36a566b4b9f2c.png)
image18.png
**ReturnToMainMenu**用来允许用户返回主菜单。Menu Scene Name用来设置场景名称。引用VRInput对象是为了处理用户Cancel的输入。引用VRCameraFade是需要做场景弹出效果。
![2772495-d66c1c787379d15b.png](https://upload-images.jianshu.io/upload_images/2772495-d66c1c787379d15b.png)
image19.png
**VRTrackingReset**是用于使场景重新居中的脚本。里面只有一个回调函数OnApplicationPause，调用了InputTracking.Recenter()。
![2772495-5530084ec082a6e2.png](https://upload-images.jianshu.io/upload_images/2772495-5530084ec082a6e2.png)
image20.png
Intro场景
该场景中包含了通用prefab对象：**MainCamera和WarningTextCanvas**。场景中包含了三个主要的介绍界面：How to Use、How to Use Confirm、Return。还有一个VRDeviceManager对象。所有的调度控制在System/IntroManager对象下面的IntroManager脚本中。
先从一个简单的说起：
**VRDeviceManager**脚本使用了单例模式，主要用于配置不同的VR平台。代码中使用预定的宏例如UNITY_ANDROID，进行条件编译。
GUI对象下面是场景中的界面元素，结构如下图所示。以InstructionsGUI为例进行说明。
![2772495-6b5569b71d397c1b.png](https://upload-images.jianshu.io/upload_images/2772495-6b5569b71d397c1b.png)
image21.png
**UIFader**脚本是用来对一组元素进行淡入淡出效果的，并且包含了很多不同的方法。这里把InstructionsGUI的两个直接子对象都加到淡入淡出组里面去。还支持设置淡入淡出速度。
![2772495-96c8836b997f0062.png](https://upload-images.jianshu.io/upload_images/2772495-96c8836b997f0062.png)
image22.png
**PlatformDependentText**脚本用来支持不同平台的文字组件。
**IntroInstructions**只是简单的显示文字对象。**InstructionsSelectionSlider**是按钮状的滑动条对象，包含了好几个已经熟悉的脚本VRInteractiveItem、SelectionSlider。UITint是第一次见到，它是给图片染色（tint）的脚本，添加了VRInteractiveItem对象的OnOver、OnOut两个事件的处理函数。当瞄准星与该物体相交的时候，该脚本根据Tint变量设置的颜色，改变图片的颜色，相当于高亮的效果。这里并非完全的颜色替换，而是一种线性加成，计算颜色的代码如下：
m_ImagesToTint[i].color += m_Tint * m_TintPercent;
![2772495-314ceabc30f7db15.png](https://upload-images.jianshu.io/upload_images/2772495-314ceabc30f7db15.png)
image23.png
**GUIArrows**脚本用来控制场景中出现的箭头出现、消失、淡入淡出的效果，VR场景中可以使用这种方式提示用户应该看往正确的方向。Fade Duration指定了淡入淡出的时间；show angle指的是当用户偏离目标方向多少角度的时候，箭头出现；Desired Direction可以设定一个transform来规定目标方向，如果是空的话使用的world space下的forward；引用相机对象是为了获取当前的朝向；Arrow Renderers是要渲染的对象。
![2772495-77d26becc032b70b.png](https://upload-images.jianshu.io/upload_images/2772495-77d26becc032b70b.png)
image24.png
**IntroManager**脚本引导用户一步一步通过介绍场景。代码的核心是Coroutine机制。
![2772495-41c7d00595c56bcd.png](https://upload-images.jianshu.io/upload_images/2772495-41c7d00595c56bcd.png)
image25.png
Flyer场景
Flyer场景是个计时“无尽飞行”的第三人称游戏，在其中玩家可以通过四处看来引导飞船的方向，并使用Fire1输入键进行射击，通过击中陨石或是引导飞船穿越空中的门来得分，跟Pilotwings或Starfox这两款游戏有点类似。
在交互方面，Flyer使用了更简单的方式，也就是让FlyerLaserController订阅VRInput的OnDown事件，从而发射激光。
Vehicles是场景中的飞机对象，还包括了飞机的附属对象，比如飞机尾部的喷气、冒烟的粒子系统，飞机的子弹发射口，以及用来显示生命值、时间、得分的界面元素。
![2772495-bc572bb160004140.png](https://upload-images.jianshu.io/upload_images/2772495-bc572bb160004140.png)
image26.png
**FlyerPlayership**对象上主要包含了飞机开火、飞行、爆炸的音效，还有飞机行为的控制脚本、UI的控制脚本。
**FlyerLaserController**主要控制飞机发射Laser的行为。Laser采用了对象池技术，从池中取对象。引用VRInput是为了处理鼠标单击事件；引用GameController是为了知道当前游戏的状态，游戏没开始的话，不能发射laser。LaserObjectPool是用来存放Laser对象的对象池。**LaserSpawnPosLeft**和LaserSpawnPosRight是Laser发出的位置和旋转，LaserAudio是Laser发出时的音效。
![2772495-fc37eea8317e6454.png](https://upload-images.jianshu.io/upload_images/2772495-fc37eea8317e6454.png)
image27.png
**FlyerMovementController**脚本用于控制飞机的移动（飞机是一直往前飞的，可以使用头盔控制上下左右活动）。DistanceFromCamera用于控制相机和飞机的距离；Speed控制飞机飞行的速度；Damping是飞机运动时的阻尼系数；Flyer是飞机的Transform；TagetMarker是飞机前面两个圆圈标记的transform；还引用了Camera和CameraContainer的transform，以及CurrentScore的文本界面。
![2772495-8a82341a3d3c49d1.png](https://upload-images.jianshu.io/upload_images/2772495-8a82341a3d3c49d1.png)
image28.png
FlyerAlignmentChecker脚本控制的是当飞机接近一个场景中圆环Ring的时候，使得圆环和飞机对齐，并改变圆环的颜色。
![2772495-6a2c80dd3378c94f.png](https://upload-images.jianshu.io/upload_images/2772495-6a2c80dd3378c94f.png)
image29.png
FlyerHealthController脚本用于控制飞机的生命状况，包括生命值，爆炸的prefab，生命条还有音效等。飞机在爆炸的时候要隐藏，毕竟飞机和爆炸的碎片完全是两个毫不相关的物体。
![2772495-70c49311b546ccb5.png](https://upload-images.jianshu.io/upload_images/2772495-70c49311b546ccb5.png)
image30.png
**System**对象下面包含很多游戏控制的对象和脚本，还有对象池，是这个场景核心的内容。
![2772495-1f197d8cb117e226.png](https://upload-images.jianshu.io/upload_images/2772495-1f197d8cb117e226.png)
image31.png
**FlyerGameController**是游戏控制脚本，把整个游戏流程和其他脚本整合在了一起。
![2772495-366a01a51831b7e9.png](https://upload-images.jianshu.io/upload_images/2772495-366a01a51831b7e9.png)
image32.png
**EnvironmentController**脚本控制了游戏场景中小行星（石头）和圆环Ring的生成。
![2772495-279f4e9cb0981039.png](https://upload-images.jianshu.io/upload_images/2772495-279f4e9cb0981039.png)
image33.png
**UIController**脚本对外提供了淡入淡出的效果。
![2772495-d2dba1d1f882095e.png](https://upload-images.jianshu.io/upload_images/2772495-d2dba1d1f882095e.png)
image34.png
此外还有场景中动态生成的物体，Laser有FlyerLaser脚本控制，石头（小行星）由Asteroid脚本控制、Gate由Ring脚本控制。
Maze场景
Maze（迷宫）游戏中提供了一个桌面式的交互示例，其中我们可以指引游戏角色到出口，并避免触发炮塔（Turret）。在选择角色的目的地时，会出现一个目的地标记，同时还会显示一个角色的路径。玩家可以通过在触摸板上使用swipe，按下方向键，或是使用游戏操纵杆上的左键来旋转视图。
![2772495-afbe4f04e2332e26.png](https://upload-images.jianshu.io/upload_images/2772495-afbe4f04e2332e26.png)
image35.png
该场景中最主要的还是System和Maze对象。
![2772495-c9584daf185fd145.png](https://upload-images.jianshu.io/upload_images/2772495-c9584daf185fd145.png)
image36.png
**Maze**对象包括了整个场景的元素，墙壁、地面、炮塔、开关、标记等等。
**MazeCourse**游戏对象是一个parent对象，其中包含了MazeFloor和MazeWalls GameObjects，这两个对象依次包含了迷宫布局中的几何信息。在MazeFloor游戏对象上关联了MeshCollider和VRInteractiveItem，从而允许在VR场景中进行交互。
**MazeCourse**关联了一个MazeTargetSetting脚本，该脚本提供了OnTargetSet事件，并确定什么时候才能触发这个事件。
![2772495-e66d9af09a6ed502.png](https://upload-images.jianshu.io/upload_images/2772495-e66d9af09a6ed502.png)
image37.png
**ExitArea**脚本提供了游戏结束的事件OnGameComplete，确定了角色到达终点区域的行为。
Turret脚本控制炮塔的行为，Switch Button控制开关的行为。
System对象中主要是MazeGameController脚本。MazeGameController是整个场景的控制器，它负责整个游戏的流程、逻辑、不同游戏阶段游戏对象的开关。
![2772495-0f5ddda28ec87069.png](https://upload-images.jianshu.io/upload_images/2772495-0f5ddda28ec87069.png)
image38.png
Shooter180和Shooter360场景
在VR Samples包含了两个射击游戏，其中一个是回廊射击游戏，玩家在180度视角的走廊中对潜在目标射击，场景如下左图。另外还有一个竞技场射击游戏，玩家被类似X战警场景的潜在目标包围，场景如下右图。
![2772495-d7424d96a630ee1c.png](https://upload-images.jianshu.io/upload_images/2772495-d7424d96a630ee1c.png)
image39.png
![2772495-43c2e75009e1bfcc.png](https://upload-images.jianshu.io/upload_images/2772495-43c2e75009e1bfcc.png)
image40.png
第一人称的胳膊和手枪是摆在相机前面的模型：
![2772495-998ce256a39987d2.png](https://upload-images.jianshu.io/upload_images/2772495-998ce256a39987d2.png)
image41.png
**System**对象下面有4个子对象，用来控制游戏流程和逻辑，包括简介、目标生成、结尾。
![2772495-a935684e1e1e88db.png](https://upload-images.jianshu.io/upload_images/2772495-a935684e1e1e88db.png)
image42.png
**ShootingGalleryController**脚本中的字段分别设置了游戏类型、理想目标数量、生成目标的概率、游戏时长、生成间隔、结尾延迟等等。
![2772495-192a6ac771be6c77.png](https://upload-images.jianshu.io/upload_images/2772495-192a6ac771be6c77.png)
image43.png
**ObjectPool**脚本是对象池技术，避免频繁创建和删除对象。
**UIController**封装了游戏中UI的切换和显示动作。
**ShooterWeapon**是场景中的胳膊、武器和倒计时、数字界面的根节点。它是可以跟随着相机转动的。
![2772495-4292f7e0617b3d72.png](https://upload-images.jianshu.io/upload_images/2772495-4292f7e0617b3d72.png)
image44.png
**ShootingGalleryGun**脚本控制了枪的行为包括移动和射击。
![2772495-301c6e1ae6a7849b.png](https://upload-images.jianshu.io/upload_images/2772495-301c6e1ae6a7849b.png)
image45.png
DefaultLineLength指的是，如果没有目标被击中的话，线渲染器的长度；
Damping阻尼，设定的是这个物体跟随相机的阻尼；
GunFlareVisibleSeconds设置的是枪开火之后的flare持续几秒；
GunContainerSmoothing胳膊和枪的模型跟随瞄准星多快；
GunAudio设计的声音；
ShootingGalleryController引用这个对象是为了在游戏没有开始的时候，不让枪射击；
EyeRaycaster检测枪正在瞄准的东西；
VRInput告诉枪什么时候射击；
CameraTransform为了获取朝前的方向；
GunContainer要引用的胳膊和枪模型；
GunEnd枪的结束位置，用来作为线渲染的起点;
GunFlare用来画射击轨迹；
Reticle瞄准星的位置；
FlareParticles是个粒子系统，枪开火的时候播放；
FlareMeshes枪开火的时候随机激活其中之一；
**UIMovement**在VR场景中很有用，它使得UI元素始终保持在相机的前面一定距离，并跟随相机旋转。可以设置ui元素是否朝向相机、是否跟随相机旋转以及跟随的速度等等。
![2772495-88607fd4c8c44eaa.png](https://upload-images.jianshu.io/upload_images/2772495-88607fd4c8c44eaa.png)
image46.png
**ShootingGalleryScore**用来显示玩家分数。
场景中会生成许多目标对象供玩家射击，每个都是一个ShooterTarget对象，该对象主要是包括Mesh Collider，VRInteractiveItem，还有ShootingTarget脚本。ShootingTarget脚本中可以设置target对象消失的时长，被击中后消失的时长，粉碎的对象，还有几种声效。
![2772495-e2ee2c2f6457ef49.png](https://upload-images.jianshu.io/upload_images/2772495-e2ee2c2f6457ef49.png)
image47.png
总结
VR场景中基本的界面和交互元素在这些示例场景中均有涉及，能满足绝大部分的需求，我们可以从中学习借鉴很多东西。作为官方给出的示例项目，该工程代码组织和实现有很多值得学习的地方：
大量使用事件机制，设计松耦合；
大量使用Coroutine，实现场景进度逻辑控制，特殊渐变效果；
对于场景中需要反复创建和销毁的对象，采用了对象池技术，避免了频繁的内存分配与释放。
