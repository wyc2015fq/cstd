# opengl工具库glfw<二> 画一个多边形 - walkabc - 博客园
# [opengl工具库glfw<二> 画一个多边形](https://www.cnblogs.com/hitfire/articles/3235886.html)
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
const float window_width = 800, window_height = 600;
void draw();
void initCoordinates();
int main()
{
    /**初始化glfw环境*/
    if (GL_TRUE != glfwInit())
    {
        exit(1);
    }
    glfwOpenWindow(window_width, window_height, 8, 8, 8, 8, 32, 0, GLFW_WINDOW);
    glfwSetWindowTitle("The GLFW Window");
    initCoordinates();
    while (true)
    {
        if (GLFW_PRESS == glfwGetKey(GLFW_KEY_ESC))
        {
            break;
        }
        draw();
        glfwSwapBuffers();
        sleep(0.05);
    }
    return 0;
}
/**对投影矩阵，模型矩阵进行操作*/
void initCoordinates()
{
    /**对投影矩阵进行操作*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, 0, window_height, -1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
/**画出来一个矩形*/
void draw()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3f(100, 100, 0);
    glVertex3f(400, 100, 0);
    glVertex3f(400, 400, 0);
    glVertex3f(100, 400, 0);
    glEnd();
}
```

