# 一个困扰我一个多星期的Nebula3的BUG - 逍遥剑客 - CSDN博客
2009年08月11日 22:08:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1952
最近看了一些PhysX的东西, 本想写个demo练练手, 结果遇到一个非常诡异的问题
当我兴冲冲的用Wizard建了一个工程, 编译, 运行, 竟然一片黑?
奇怪啊, 这个Wizard程序应该没问题才对, 要不然我怎么敢发布出来?
换成Release, 编译...结果正常!
用PIX跟踪了一下发现, 两个版本的变换矩阵不一样!
当时就茫然了...拿SDK自带的testviewer编译运行了一下, debug版竟然是这样的:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090811/N3_BUG.jpg)
只能看到两只脚, 鼠标操作没有反应!
太崩溃了~
代码肯定没修改, 因为我本机是架设了SVN的, 一切修改都有记录
重新安装一个Arip2009, 结果仍然一样!
然后QQ询问同样研究Nebula的朋友, 他们的结果是对的!
我就把我编译的testviewer.debug.exe发过去, 结果在他们那边也是正常的...
然后我就理所当然的排除了代码错误的可能性
首先怀疑自己的软件环境是不是有冲突, 因为我习惯装一大堆软件.....
正好赶上Win7出RTM, 还正好在超频论坛发现了我主板的激活BIOS(当初花钱弄了个超频专用的, 结果买回来也没超-_-), 于是就升级了...
虽说身为一个软件开发人员, 用盗版是可耻的, 但为了解决眼下的问题, 我等不到Win7开始卖了
Xp->Win7, DX SDK March 2008->March 2009, VS2005->VS2008, 9800GT driver, SVN, etc,  updated, too......
然后我就满怀希望地在全新的环境下又编译了一遍......结果仍然不对
我当时就郁闷了, 难道是硬件出了问题? 不对啊, release是正常的-_-
翻出N3 SDK的上个版本, 编译之, 正常...
难道是编译选项的问题? 全部改回去, 编译, 仍然不行
又找了个跟我软件硬件环境都几乎一样的哥们测试了一下, 他那儿也正常!
又找了个哥们, 终于他那里出现一样的症状了, 不过他不搞3D, 没法一起查了-_-
跟倒数第二个哥们辩论了一番, 他说了句: 为什么不能操作呢?
一句话惊醒梦中人: 一直以来我都是把注意力放到变换矩阵为什么不正确了, 以为矩阵不正确所以才没法操作
于找到操作的处理代码, 看到了这样一段:
    // process gamepad input
    float zoomIn = 0.0f;
    float zoomOut = 0.0f;
    float2 panning(0.0f, 0.0f);
    float2 orbiting(0.0f, 0.0f);
    if (gamePad->IsConnected())
    {
        const float gamePadZoomSpeed = 50.0f;
        const float gamePadOrbitSpeed = 10.0f;
        const float gamePadPanSpeed = 10.0f;
        if (gamePad->ButtonDown(GamePad::AButton))
        {
            this->mayaCameraUtil.Reset();
        }
        if (gamePad->ButtonDown(GamePad::StartButton) ||
            gamePad->ButtonDown(GamePad::BackButton))
        {
            this->SetQuitRequested(true);
        }
        float frameTime = (float) this->GetFrameTime();
        zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime * gamePadZoomSpeed;
        zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime * gamePadZoomSpeed;
        panning.x()  += gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime * gamePadPanSpeed;
        panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime * gamePadPanSpeed;
        orbiting.x() += gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime * gamePadOrbitSpeed;
        orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime * gamePadOrbitSpeed;
    }
嗯, 我是烧的插了个360的手柄, 不会放那儿不动也会影响操作吧? 注释掉, 编译, debug结果终于正常了!
下断点观察, 发现        float frameTime = (float) this->GetFrameTime();返回的是一个无效值.....汗
转到frameTime的老窝, 发现了这样一件事:
//------------------------------------------------------------------------------
/**
*/
RenderApplication::RenderApplication() :
    //time(0.0),
    //frameTime(0.0),
    quitRequested(false)
{
    // empty
}
这俩变量竟然没初始化? 疯掉了...当初也想过变量未初始化的原因, 因为这是导致debug/release不一样结果最常见的原因.
但是在别人机器上运行正常让我排除了代码错误的可能性, 一直在找运行环境的问题
问了下那个同命相连的哥们, 他也插了个360的手柄玩MGS来着, 崩溃
最后解决办法, 把上面那两个变量的注释符去掉, 编译, OK.........
