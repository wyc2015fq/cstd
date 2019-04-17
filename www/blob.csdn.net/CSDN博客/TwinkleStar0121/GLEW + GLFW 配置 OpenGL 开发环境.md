# GLEW + GLFW 配置 OpenGL 开发环境 - TwinkleStar0121 - CSDN博客





2018年07月24日 11:27:56[TwinkleStar0121](https://me.csdn.net/jvandc)阅读数：214










- - - [SourceLink](#sourcelink)
- [配置步骤](#配置步骤)
- [附注](#附注)






### SourceLink

1、[GLEW](http://glew.sourceforge.net/)

2、[GLFW](http://www.glfw.org/index.html)

3、[GLFW初体验](http://www.cnblogs.com/graphics/p/3661500.html)

4、[GLEW+VS2010配置](http://www.cnblogs.com/graphics/p/3661500.html)

5、[GLEW | GLFW | OPENGL | MESA 3D](http://blog.csdn.net/taozhi20084525/article/details/42804879)
### 配置步骤

1、下载GLEW1.13.0和GLFW3.1.1 

2、解压，在其build的目录下有v12等工程文件，打开，编译，Debug版本可以得到glew32d.lib，如果是动态链接库同时可以得到glew32d.dll。 
![](https://img-blog.csdn.net/20180724112150536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p2YW5kYw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180724112300125?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p2YW5kYw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3、解压，使用CMake生成VS工程文件，打开，编译项目，编译静态库，得到glfw3.lib 
![](https://img-blog.csdn.net/20180724112523547?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p2YW5kYw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、测试GLFW环境，新建一个VS项目，命名为glfwTtest，配置头文件目录，库目录，添加依赖库glfw3.lib，opengl32.lib，测试代码[GLFW初体验](http://www.cnblogs.com/graphics/p/3661500.html).
```
#include "glfw3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Draw a triangle */
        glBegin(GL_TRIANGLES);

        glColor3f(1.0, 0.0, 0.0);    // Red
        glVertex3f(0.0, 1.0, 0.0);

        glColor3f(0.0, 1.0, 0.0);    // Green
        glVertex3f(-1.0, -1.0, 0.0);

        glColor3f(0.0, 0.0, 1.0);    // Blue
        glVertex3f(1.0, -1.0, 0.0);

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
```

5、测试GLEW环境，新建项目glewTest，同样配置头文件目录、库目录，添加库glew32d.lib，项目使用glfw，所以添加库glfw3.lib，测试代码.

```
#include <stdio.h>
#include "glew.h"
#include "glfw3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() == GLEW_OK)
    {
        printf("glew init ok!\n");
    }

    printf("opengl version: %s\n", glGetString(GL_VERSION));

    while (!glfwWindowShouldClose(window))
    {

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
```

6、完成.

### 附注

1、开发环境 

软件：Win7 + VS2013 

硬件：Intel E3-1230 + Nivdia GeForece GTX 660 

支持OpenGL4.5.0
2、GLEW和GLFW关系 

GLEW和GLFW两者之间并没有什么关系，在一些论坛上面也会有一些网友在问类似的问题，比如[gamedev](http://www.gamedev.net/topic/670250-glfw-or-glew-which-opengl-library-should-i-use/)就有网友在询问。 

GLEW是为了保证开发者能够使用到更高级的OpenGL API而开发的类库（3.2或者更高版本的）。 

GLFW是一个窗口、键盘、事件管理的类库，类似GLUT（GLUT比较旧了）。
备注:[更加详细的OpenGL相关工具组件和API](https://www.opengl.org/wiki/Related_toolkits_and_APIs#Context.2FWindow_Toolkits)







