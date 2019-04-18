# unity fixed timestep - fighting！！！ - CSDN博客
2017年06月25日 20:47:59[dujiahei](https://me.csdn.net/dujiahei)阅读数：322标签：[unity																[时间																[函数](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=时间&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
来源：http://blog.csdn.net/trudgecarrot/article/details/53996908
时间[管理器](http://www.manew.com/tag.php?title=%E7%AE%A1%E7%90%86%E5%99%A8) (time
 manager)
时间管理器 (time manager)
[属性](http://www.manew.com/tag.php?title=%E5%B1%9E%E6%80%A7)
属性：
功能：
固定时间步长 (fixed timestep)
独立于帧速率的时间间隔，用来命令何时执行[物理](http://www.manew.com/tag.php?title=%E7%89%A9%E7%90%86)[计算](http://www.manew.com/tag.php?title=%E8%AE%A1%E7%AE%97)和fixedupdate()[事件](http://www.manew.com/tag.php?title=%E4%BA%8B%E4%BB%B6)。
允许的最大时间步长 (maximum allowed timestep)
独立于帧速率的时间间隔，当帧速率低时，它会覆盖最坏的案例场景。执行物理计算和（fixedupdate()） 事件的时间不会超过规定的时间。
时间尺度 (time scale)
时间前进的速度。更改此值以模拟子弹时间[特效](http://www.manew.com/tag.php?title=%E7%89%B9%E6%95%88)。1
 表示实时时间。.5 表示速度减半；2 表示速度加倍。
详细信息
固定时间步长 (fixed timestep)
对于稳定的物理模拟来说，固定时间步长 (fixed time stepping) 非常重要。计算机不尽相同，不同的硬件配置在运行 unity [游戏](http://www.manew.com/tag.php?title=%E6%B8%B8%E6%88%8F)时表现出来的性能也不同。因此，物理计算必须独立于游戏的帧速率进行。诸如[碰撞](http://www.manew.com/tag.php?title=%E7%A2%B0%E6%92%9E)检测和刚体
 (rigidbody) [移动](http://www.manew.com/tag.php?title=%E7%A7%BB%E5%8A%A8)这样的物理计算是在独立于帧速率的离散固定时间步长中进行。这使不同电脑间或帧速率发生变化时模拟的一致性更强。例如，帧速率可能会由于屏幕上多个游戏同时进行或用户在[后台](http://www.manew.com/tag.php?title=%E5%90%8E%E5%8F%B0)执行另一应用程序而降低。
以下是固定时间步长的计算方法。在屏幕上绘制帧之前，unity 将固定时间提前一段固定增量时间，并在到达当前时间之前执行物理计算。这直接与固定时间步长 (fixed timestep)属性相关联。固定时间步长 (fixed timestep）的值越小，进行物理计算的频率越高。每秒的固定帧数的计算方法是，用
 1 除以固定时间步长 (fixed timestep)。因此，1/0.02 = 50（帧每秒） 以及 1/0.05 = 20（帧每秒）。
简而言之，固定更新值越小，物理模拟越精确，但是 cpu 的负担也会更大。
允许的最大时间步长 (maximum allowed timestep)
固定时间步长是稳定的物理模拟的保证。然而，如果游戏对物理负担大并且已经运行缓慢或偶尔会降至低帧速率，那么固定时间步长也可能对性能带来负面影响。帧所用时间越长 - 就必须执行更大的固定更新步长以转到下一个帧。这会导致性能下降。为避免这种场景的出现，unity [iOS](http://lib.csdn.net/base/ios) 引入了允许的最大时间步长
 (maximum allowed timestep)，以确保物理计算的运行时间不会超出规定的阈值。
如果帧所用时间超过允许的最大时间步长 (maximum allowed timestep)中规定的时间，那么物理会“假设”帧仅用了允许的允许的最大时间步长 (maximum allowed timestep)秒数。换句话说，如果帧速率降低到某个阈值以下，那么刚体的速度就会变慢一点，以便让
 cpu 可以赶上。
允许的最大时间步长 (maximum allowed timestep)对物理计算和fixedupdate()事件都会造成影响。
允许的最大时间步长 (maximum allowed timestep)和固定时间步长 (fixed timestep)时，都以秒为单位。因此，如果帧速率降低到 1/0.1 = 10 （帧每秒）以下，那么设置 0.1 就会使物理和 fixedupdate() 事件速度降低。
典型场景
假设固定时间步长 (fixed timestep)是 0.01，也就是说应该每 10 毫秒处理一次 physx、固定更新 (fixedupdate) 和动画。
当您的帧时间大约是 ~33每个视觉帧平均要执行 3 次固定循环。
但是，帧时间不是固定不变的，它取决于许多因素，包括您的场景状态、os 后台任务等。
由于 3. 的原因，帧时间有时会达到 40 - 50 毫秒，也就是说会执行 4 - 5 次固定步长循环。
当您的固定时间步长任务相当繁重时，physx、固定更新 (fixedupdate) 和动画花费的时间会将帧时间延长 10 毫秒，也就是说为所有这些固定时间步长任务增加了一次迭代。
在一些不幸的情况下，5. 中描述的过程时间会延长至处理固定步长循环所用时间的 10 倍及其以上。
这就是引入允许的最大时间步长 (maximum allowed timestep)的原因。它是在单个视觉帧期间限制 physx、固定更新 (fixedupdate) 和动画处理次数的方法。如果将允许的最大时间步长 (maximum allowed timestep)设为 100
 毫秒，并固定时间步长 (fixed timestep)设为 10 毫秒，那么在每个视觉帧会最多执行 10 次固定步长任务。因此，随着固定时间步长迭代次数的增加，小的性能故障有时会引发大的性能故障。通过将允许的允许的最大时间步长 (maximum allowed timestep)降到
 30 毫秒，可将最大固定步长迭代次数限制到 3，这就是说您的 physx、固定更新 (fixedupdate) 和动画不会将帧时间延长太多，但这种限制也有一些负面作用。您的动画和物理在性能故障发生时会变慢。
提示
使玩家可以通过编写脚本，动态地更改时间尺度 (time scale)来控制时间。
如果您的游戏物理负担重或在fixedupdate()事件中花费大量时间,那么请将允许的最大时间步长 (maximum allowed timestep)设为 0.1。这会防止物理以低于 10 帧每秒的速率驱动游戏。
