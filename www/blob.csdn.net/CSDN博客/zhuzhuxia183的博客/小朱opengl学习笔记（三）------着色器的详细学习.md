# 小朱opengl学习笔记（三）------着色器的详细学习 - zhuzhuxia183的博客 - CSDN博客





2017年11月26日 16:43:52[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：134








### 上篇博客，主要是介绍了Opengl的大致使用流程，由于中间过程可能比较复杂，

所以今天笔者首先梳理一遍整个的流程。 

祭宝图哈。

![这里写图片描述](https://img-blog.csdn.net/20171126143541597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上篇博客我们的重点是放在由  顶点数据——>顶点着色器 的这个过程上面，如果对于顶点着色器，以及片段着色器介绍了它们是用shadow语言写的，然后如何调用使用它们的。我们的使用方式为:   在shadowprogram里面指定好两个   ：顶点着色器，以及片段着色器。然后我们输入数据，并且规定好  从顶点数据到我们的顶点着色器之间的读取规则。
好的，今天呢，我们重点来关注下顶点着色器以及片段着色器的相关知识。其实，昨天里面就已经包含了顶点着色器，片段着色器了，现在我们来细细的分析下：

```
// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
```

以上是两个着色器代码，这里注意哈，虽然是用GLchar* 指示的一个变量，像是字符串，但是这里我们应该认为是代码哈，就是可以运行的哈。两个着色器感觉差不多，我们就取一个来讲。而且这里的的\n其实就是一个换行符，所以整理下为：

```
#version 330 core
layout (location = 0) in vec3 position; // position变量的属性位置值为0

out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
    gl_Position = vec4(position, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // 把输出变量设置为暗红色
}
```

ok，一步一步分析: 

第一行，就是用#开头，后面的内容就是说指我们的使用版本是多少多少。这个的话，不是很影响，了解就行。 

接着，
`layout (location = 0) in vec3 position;`

这行代码：layout(location)和我们之前设置的有关系，贴个图：

![这里写图片描述](https://img-blog.csdn.net/20171126150554235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ok，好的，要是不清楚的可以回到之前再看下哈。（对于有件事，由于本人也是在学习opengl，所以在学习的时候会参考一些学习网站，比如[https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/04%20Hello%20Triangle/](https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/04%20Hello%20Triangle/)，大家感兴趣，可以去看下哈，这里说的很好，但是可能第一次阅读不是很好懂，我的博客这两篇基本上都是来对于上面的内容进行二次总结。唉，没办法啊，小白入门真的不容易啊）继续，后面的in表示就是指我们输入的是一个vec3的向量，然后变量名为position，就像函数的参数一样。好，接着，

`out vec4 vertexColor; // 为片段着色器指定一个颜色输出`

这里的话，我们还需要从那个图说起

![这里写图片描述](https://img-blog.csdn.net/20171126143541597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们看 着色器的运行是是有一个先后顺序的，要注意，着色器之间是不可以相互通行的，他们就像是一环套一环一样，所以前一个的输出就是另一个的输入。好的，这里的out表示就是输出的意思，表示输出一个vec4的向量，vertexcolor。之后，我们在main函数中的
```cpp
gl_Position = vec4(position, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // 把输出变量设置为暗红色们
```

这里做的事情，就是把我们的glposition给确定好，然后把我们的颜色向量设置好。 

看 片段着色器：

```
#version 330 core
in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

out vec4 color; // 片段着色器输出的变量名可以任意命名，类型必须是vec4

void main()
{
    color = vertexColor;
```

ok,我们看这里的in vertexColor不就是我们的顶点着色器输出的那个verextColor变量，就是通过这种方式，才能把我们的着色器程序之间相互关联起来。好的，我们可以预想，这里的vec4(0.5f, 0.0f, 0.0f, 1.0f);就是我们绘制的顶点颜色。当然由于内部机制原因，整个图形都是这个颜色。好的，其实到现在为止，我们已经基本了解了。然后我们看个案例吧。

```cpp
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n\0";

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);


    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top 
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
```

同样的配置，现在的效果是：

![这里写图片描述](https://img-blog.csdn.net/20171126153817994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ok，这个代码和我们之前讲的那个三角形代码，基本上差不多，只是改了如下的地方
```cpp
GLfloat vertices[] = {
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top 
    };
```

和

```
// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
```

和

```
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
```

这三个地方的代码，主要的意思，就是说我们在存入顶点的坐标数据的时候还写了顶点的颜色rgb值，但是由于的问题是说，我们的片段着色器后面并不是只单单给我们的三个顶点赋值了颜色，其余的位置也是按照线性变化赋值了相应的rgb值。所以最后结果如上图所示。 

ok,其实到这里，我觉得对于着色器的相关使用方法已经介绍完了哈。但是可能有一个地方我们需要改进下，就是如果我们的shadow程序太过复杂了，我们都写在.cpp文件下，会显得十分的繁琐，所以我们可以另外写一个文件，然后我们在使用shadow程序之前，用文件流去读写，然后我们就可以写进内容，然后用GLchar*来保存，之后就和之前是一样的。 

然后为了方便我们的使用，我们新增加一个类，把读取的相关操作用一个函数进行封装。所有的东西就直接写在.h文件中，内容为：
```cpp
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Shader Program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    // Uses the current shader
    void Use() 
    { 
        glUseProgram(this->Program); 
    }
};

#endif
```

之后的话，我们使用起来，就直接用shader函数就可以加载啦。具体使用的案例，我会给一个vs中project，然后直接直接添加到你的解决方案中去。当然配置和之前一样不能少噢。[代码下载链接](http://download.csdn.net/download/zhuzhuxia183/10133379%20%E4%BB%A3%E7%A0%81%E4%B8%8B%E8%BD%BD%E4%BD%8D%E7%BD%AE)

当然这个只是换了一种方式，最后的效果一样哈，还是这样的

![这里写图片描述](https://img-blog.csdn.net/20171126153817994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ok，以上就是我对于opengl中着色器的相关使用理解，希望可以帮助到你们哈。








