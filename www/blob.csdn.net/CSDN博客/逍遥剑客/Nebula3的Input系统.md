# Nebula3的Input系统 - 逍遥剑客 - CSDN博客
2008年10月06日 23:26:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1503
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
相对于其他的子系统来说, 输入系统是比较简单的. 很多游戏根本就没有对这一块进行封装, 而直接采用了Win32的消息机制. 
不过经过封装的输入系统使用起来很方便, 呵呵. 
N3中有三种输入设备, 键盘, 鼠标, 手柄. 分别是基于Win32消息, DirectInput, XInput实现的. 这里有一个继承图能够很好的说明输入系统的组织结构:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081006/Nebula3_InputHandler_Class.JPG)
基本的消息处理机制是这样的一个流程:
InputServer里有默认的一个键盘, 一个鼠标, 一个手柄的"handler", 在每帧开始时InputServer会检测当前的输入消息,  得到一个InputEvent, 由相应的InputHandler来处理.  各个InputHandler都保存着当前帧各种输入状态的缓存(如鼠标左键是否按下), 因此, 在程序运行过程中, 我们只要在绘制结束前检测各个InputHandler的状态就相当于知道当前用户是怎样输入的了. 
一般只需要关心这么几个函数就够了:
////////////////////// Mouse//////////////////////////// 
    /// return true if button is currently pressed 
    bool ButtonPressed(Input::MouseButton::Code btn) const;
    /// return true if button was down at least once in current frame 
    bool ButtonDown(Input::MouseButton::Code btn) const;
    /// return true if button was up at least once in current frame 
    bool ButtonUp(Input::MouseButton::Code btn) const;
    /// return true if a button has been double clicked 
    bool ButtonDoubleClicked(Input::MouseButton::Code btn) const;
    /// return true if mouse wheel rotated forward 
    bool WheelForward() const;
    /// return true if mouse wheel rotated backward 
    bool WheelBackward() const;
    /// get current absolute mouse position (in pixels) 
    const Math::float2& GetPixelPosition() const;
    /// get current screen space mouse position (0.0 .. 1.0) 
    const Math::float2& GetScreenPosition() const;
    /// get mouse movement 
    const Math::float2& GetMovement() const;
//////////////////////Keyboard////////////////////// 
    /// return true if a key is currently pressed 
    bool KeyPressed(Input::Key::Code keyCode) const;
    /// return true if key was down at least once in current frame 
    bool KeyDown(Input::Key::Code keyCode) const;
    /// return true if key was up at least once in current frame 
    bool KeyUp(Input::Key::Code keyCode) const;
    /// get character input in current frame 
    const Util::String& GetCharInput() const;
GamePad先略过, 原理相同
测试例子, 在上一次的代码中添加一段:
    void OnRenderFrame()
    {
        if (this->inputServer->GetDefaultMouse()->ButtonDown(MouseButton::LeftButton))
        {
            MessageBoxA(this->displayDevice->GetHwnd(), "Left Button Down", NULL, 0);
        }
    //...// 
    }
效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081006/Nebula3_InputMouse_Test.JPG)
