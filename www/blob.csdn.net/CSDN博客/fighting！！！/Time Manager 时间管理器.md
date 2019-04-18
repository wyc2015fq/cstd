# Time Manager 时间管理器 - fighting！！！ - CSDN博客
2017年06月25日 21:23:30[dujiahei](https://me.csdn.net/dujiahei)阅读数：257标签：[unity																[函数																[时间																[时间管理](https://so.csdn.net/so/search/s.do?q=时间管理&t=blog)](https://so.csdn.net/so/search/s.do?q=时间&t=blog)](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
                
来源：Time Manager 时间管理器 - Unity圣典
http://www.ceeger.com/Components/class-TimeManager.html
# 时间管理器 Time Manager
Date:2012-05-02 13:36
![](http://www.ceeger.com/Components/Images/class-TimeManager-0.jpg)
*The Time Manager* 时间管理器
## Properties 属性
- 
Fixed Timestep 固定时间步
A framerate-independent interval that dictates when physics calculations and **FixedUpdate()** events are performed.
该时间间隔与帧速率无关，用于物理计算和执行FixedUpdate()事件
- 
Maximum Allowed Timestep
最大时间步
A framerate-independent interval that caps the worst case scenario when frame-rate is low. Physics calculations and **FixedUpdate()** events will not be performed for longer time than specified.
该时间间隔与帧速率无关，用于帧速率很低的糟糕情况。物理计算和FixedUpdate() 执行不会超过指定的时间。
- 
Time Scale 
时间比例
The speed at which time progress. Change this value to simulate bullet-time effects. A value of 1 means real-time. A value of .5 means half speed; a value of 2 is double speed.
时间流逝的速度。可以通过修改此值来模拟子弹时间的效果。1表示真实时间，0.5表示一半速度，2表示2倍速度。
## Details 细节
### Fixed Timestep 固定时间步
Fixed time stepping is very important for stable physics simulation. Not all computers are made equal, and different hardware configurations will run Unity games with varying performance. Therefore, physics must be calculated independently of the game's
 frame rate. Physics calculations like collision detection and Rigidbody movement are performed in discrete fixed time steps that are not dependent on frame rate. This makes the simulation more consistent across different computers or when changes in the frame
 rate occur. For example, the frame rate can drop due to an appearance of many game onscreen, or because the user launched another application in the background.
固定时步对稳定的物理模拟非常重要。不同计算机的硬件配置在运行unity的游戏时有不同的性能表现。因此物理模拟必须独立于帧速率进行计算。碰撞检测和刚体运动等物理计算在离散的固定时步中执行，与帧速率无关。这使得物理模拟在不同的计算机上或帧速率发生变化时是一致的。例如，游戏屏幕内容的变化和用户在后台执行程序都会导致帧速率下降。
Here's how the fixed time step is calculated. Before every frame is drawn onscreen, Unity advances the fixed time by fixed delta time and performs physics calculations until it reaches the current time. This directly correlates to the **Fixed Timestep** property.
 The smaller the value of **Fixed Timestep**, the more frequently physics will be calculated. The number of Fixed frames per second can be calculated by dividing 1 by **Fixed Timestep**. Therefore, 1 / 0.02 = 50 fixed frames per second
 and 1 / 0.05 = 20 fixed frames per second.
以下是固定时步如何计算。在每一帧图像绘制到屏幕之前，Unity将固定时间加上固定时步，然后执行物理计算，直到它到达当前时间。该过程与固定时步属性直接相关。固定时步值越小，物理计算就越频繁。将1除以固定时步可以得到每秒计算固定帧的数量。所以，1 /0.02=50固定帧每秒，1 /0.05=20固定帧每秒。
Simply put, a smaller fixed update value leads to more accurate physics simulation but is heavier on the CPU.
简而言之，一个较小的固定更新值可以得到更准确的物理模拟，但CPU的负担会加重。
### Maximum Allowed Timestep 最大时间步
Fixed time stepping ensures stable physics simulation. However it can cause negative impact on performance if game is heavy on physics and is already running slow or occasionally dips to low frame rate. Longer the frame takes to process - more fixed update
 steps will have to be executed for the next frame. This results in performance degradation. To prevent such scenario Unity iOS introduced **Maximum Allowed Timestep** which ensures that physics calculations will not run longer than specified threshold.
固定时步可以确保稳定的物理模拟。然而，如果游戏的物理计算过多导致运行缓慢或者帧速率间或降低，会造成负面影响。帧的处理时间越长，要执行的固定时步更新次数就越多，这会导致性能下降。为了防止这种情况，Unity iOS通过引入最大时步来确保物理计算不会超过指定的阈值。
If frame takes longer to process than time specified in Maximum Allowed Timestep, then physics will "pretend" that frame took only **Maximum Allowed Timestep** seconds. In other words if frame rate drops below some threshold, then rigid bodies
 will slow down a bit allowing CPU to catch up.
如果帧的处理时间超过了最大时步，物理引擎会"假装"该帧只消耗了最大时步的时间。换句话说，如果帧速率下降到低于某个阈值，刚体将会放慢一点让CPU赶上。
**Maximum Allowed Timestep** affects both physics calculation and **FixedUpdate()** events.
最大时步对物理计算和FixedUpdate()事件都有影响。
**Maximum Allowed Timestep** is specified in seconds as **Fixed Timestep**. Therefore setting 0.1 will make physics and **FixedUpdate()** events to slow down, if frame rate dips below 1 / 0.1 = 10 frames per second.
最大时步的单位是秒。因此设置为0.1时，如果帧速率低于1 /0.1=每秒10帧，物理计算和FixedUpdate()事件将会放慢。
### Typical scenario 典型场景
- Let's assume **Fixed Timestep** is 0.01, which means that physx, fixedUpdate and animations should be processed every 10 ms. 
假设固定时步是0.01，这意味着PhysX，fixedUpdate和动画 每10毫秒计算一次。
- When your frame time is ~33 ms then fixed loop is executed 3 times per visual frame on average. 
如果1帧的处理时间是33毫秒，那么固定时步循环将在每帧执行3次。
- But frametime isn't fixed constant and depends on many factors including your scene state, OS background taks, etc. 
但每帧的处理时间不是固定不变的常数，它取决于许多因素，包括场景的状态，操作系统的后台程序等等。
- Because of 3. reasons frametime sometimes can reach 40-50 ms, which means that fixed step loop will be executed 4-5 times. 
由于原因3，帧时间有时可以达到40-50毫秒，这意味着，固定循环会被执行4-5次。
- When your fixed timestep tasks are pretty heavy then time spent on physx, fixedUpdates and animations extend your frametime by another 10 ms, which means one more additional iteration of all these fixed timestep tasks. 
如果固定时步循环中的计算任务十分繁重，physx，fixedUpdates事件和动画导致帧时间延长了额外的10毫秒，又将会导致一次额外的固定时步计算。
- In some unlucky cases process described in 5. could extend to 10 and more times of processing fixed step loop. 
在某些糟糕的情况下， 5中描述的过程可能会导致10次以上的固定时步循环。
- That's why **Maximum Allowed Timestep** was introduced, it is the method to limit how much times physx, fixedUpdates and animations can be processed during single visual frame. If you have** Maximum Allowed Timestep** set
 to 100 ms and your **Fixed Timestep** is 10 ms, then for fixed step tasks will be executed up to 10 times per visual frame. So sometimes small performance hitch could trigger big performance hitch because of increased fixed timestep iteration
 count. By decreasing **Maximum Allowed Timestep** to 30 ms, you are limiting max fixed step iteration count to 3 and this means that your physx, fixedUpdate and animation won't blow your frametime up very much, but there is some negative effect
 of this limiting. Your animations and physics will slow down a bit when performance hitch occurs. 
这就是为什么我们要引入最大时步，它用来限制PhysX，fixedUpdates和动画在一个图形帧中运行的最大次数。如果你将最大时步设置为100毫秒，固定时步为10 ms，固定时步任务在一个图形帧中最多执行10次。因为固定时步循环次数的增加，有时侯小的性能问题可能会导致大的性能问题。如果把最大时步减少至30毫秒，固定时步循环的次数将限制为最多为3次，这意味着PhysX，fixedUpdate和动画不会导致帧的处理时间大大延长。但限制也有一定的负面影响。当性能出现问题时，你的动画和物理模拟会稍稍变慢。
## Hints 提示
- Give the player control over time by changing **Time Scale** dynamically through scripting. 
通过脚本给玩家动态地调整时间比例以控制时间。
- If your game is physics heavy or spends significant amount of time in **FixedUpdate()** events, then set **Maximum Allowed Timestep** to 0.1. This will prevent physics from driving your game below 10 frames
 per second. 
如果游戏的物理计算量很大，或者在FixedUpdate（）事件中消耗了大量时间，你可以设置最大时步为0.1。这可以阻止物理引擎将你的游戏帧率降到10帧/秒。
页面最后更新：2011-10-21

