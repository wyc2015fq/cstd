# opengl工具库glfw<一> 创建窗口 - walkabc - 博客园
# [opengl工具库glfw<一> 创建窗口](https://www.cnblogs.com/hitfire/articles/3235869.html)
　　众所周知，opengl是跨平台，所以opengl中API中没有关于创建窗口，管理用户输入的函数，因为每个系统的窗口管理器不一定相同，所以关于窗口管理这一部分通常是由第三方软件提供，比较知名的是glut，glfw，今天要介绍的就是glfw。最新的glfw的版本已经是3.0版本，但是cocos2d-x中使用的部分是2.0版本，所以就先分析glfw2.0版本。
```
//============================================================================
// Name        : TestOpenGL.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <GL/gl.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    const float window_width = 800, window_height = 600;
    /**初始化glfw环境*/
    if (!glfwInit())
    {
        exit(1);
    }
    glfwOpenWindow(window_width, window_height, 8, 8, 8, 8, 32, 0, GLFW_WINDOW);
    glfwSetWindowTitle("The GLFW Window");
    while (true)
    {
        if (GLFW_PRESS == glfwGetKey(GLFW_KEY_ESC))
        {
            break;
        }
        sleep(0.05);
    }
    return 0;
}
```
　　上面的例子是创建一个宽800，高600的窗口，输入esc键，程序将终止。例子是在linux环境测试，其他平台不保证正常运行。

