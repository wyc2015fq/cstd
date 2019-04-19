# PhysX学习笔记(2): 动力学(1) - 逍遥剑客 - CSDN博客
2009年07月22日 22:50:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2984
场景(Scene):
物理模拟发生的场所, 包含了actor, joint, effector. 跟图形引擎里的Scene差不多, 可以有多个实例存在, 互不干涉.
场景没有特别的大小限制. 功能有重力, 射线拣取, 开关硬拼检测等.
模拟时序(Timing):
物理模拟导致物体的一些属性随着时间进行变化, 如位置, 速度等.
模拟每经过一次时间步进进行一次, 通常要与渲染帧数进行同步.
前进(也可以说是更新):
void simulate(NxReal elapsedTime);
elapsedTime会被分成多个时间步进来决定物理模拟进行了多少
时序可以用setTiming设置, decriptor也可. 默认值就能够满足大多数程序的要求了.
固定的时间步进可以确保稳定且可以重现的物理模拟, 需指定NX_TIMESTEP_FIXED.
异步的步进:
PhysX是多线程的, 物理模拟发生在自己的线程里. 状态变化如下:
1)开始模拟
2)确保数据已被发送到模拟线程
3)检测模拟是否完成, 如果是, 更新状态数据缓冲, 并且->
4)交换状态数据缓冲, 这样结果可以被程序访问, 后台缓冲可以继续参数模拟(就像D3D的BackBuffer)
    NxScene* gScene;
    NxReal myTimestep = 1.0f/60.0f;
    ...
    void mySimulationStepFunction()
    {
        gScene->simulate(myTimestep);
        gScene->flushStream();
        //...perform useful work here using previous frame's state data
        gScene-> fetchResults(NX_RIGID_BODY_FINISHED, true);
    }
注意fetchResults(NX_RIGID_BODY_FINISHED, true)是一个阻塞调用, 可以用这两种方式代替:
        while(!gScene->checkResults(NX_RIGID_BODY_FINISHED, false)
        {
            // do something useful
        }
        while(!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false)
        { 
            // do something useful
        }
注意: 一些函数对于异步的状态改变不可知, 如overlap和raycasting, 直到simulte()/fetchResult()执行后才可.
外壳宽度(Skin Width):
通俗点说, 就是能两个物体能相互插入多么深…..因为模拟是以时间步进为单位的, 所以两次步进之间就会产生一个跳变, 这种互相越界的现象不可避免. 于是乎引入这么一个量, 用于保证稳定性. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090722/SkinWidth.JPG)
