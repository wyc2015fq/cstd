# openGL中的Texture/MultiTexture Mapping - xqhrs232的专栏 - CSDN博客
2011年01月06日 18:02:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1119标签：[filter																[blend																[byte																[null																[fp																[extension](https://so.csdn.net/so/search/s.do?q=extension&t=blog)](https://so.csdn.net/so/search/s.do?q=fp&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=blend&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
原文地址::[http://www.cnblogs.com/antai/archive/2010/01/19/1651959.html](http://www.cnblogs.com/antai/archive/2010/01/19/1651959.html)
**1 Preface**
openGL中，Texture Mapping--纹理映射，不是什么新鲜话题。最近需要用到多重纹理，觉得必要分享一下自己的一点心得，因为一些东西我们虽然一直在用，但是往往领悟不深刻。下面先贴一个多重纹理的示意图，顺便给自己打个广告：）
![multiTex](http://images.cnblogs.com/cnblogs_com/antai/WindowsLiveWriter/openGLTextureMultiTextureMapping_12B6E/multiTex_thumb.jpg)
**2 From the Very Beginning**
所谓纹理映射，就是把一个纹理的信息映射到我们想要绘制的区域。那么什么是纹理的信息呢？在计算机中说白了就是RGB（RGBA）三（四）个属性的值。关于RGB空间我就不多说了。
好了，那第一步我们就是要读取一张纹理。以BMP文件为例，我们第一要做的工作就是，通过某个函数来parse BMP文件, 并把需要的RGB信息保存起来。你可以通过glaux里的函数来实现，当然你也可以自己写一个函数，像这样：
   unsigned char *l_texture; //指向我们要把读取的纹理存放在内存中的区域
   // 下面这两个就要感谢windows.h，他可以帮我们搞定BMP的类型。BMP文件格式大家可以了解下
     BITMAPFILEHEADER fileheader; 
   BITMAPINFOHEADER infoheader; 
   RGBTRIPLE rgb;
  ……
   if( (l_file = fopen(filename, "rb"))==NULL) return (-1); // 打开文件
   fread(&fileheader, sizeof(fileheader), 1, fp); //从fp文件中读取1次fileheader大小的信 
                                                              //息存放在（缓冲区）fileheader中
   fseek(l_file, sizeof(fileheader), SEEK_SET); // Jump the fileheader 
   fread(&infoheader, sizeof(infoheader), 1, fp); // 同理
   // 下面呢，我们就为纹理信息分配存储空间。想想为什么要乘4呢？ 
   l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4); 
   // 为防止意外发生，全都初始化为0 
   memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
  // 现在我们读取每一个信息 
   for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++) 
   {            
           // 从fp中读取1次大小为rgb字节的信息存在(缓冲区)rgb中
           fread(&rgb, sizeof(rgb), 1, fp); 
           // 现在存到l_texture指向的内存地址
           l_texture[j+0] = rgb.rgbtRed; // Red component 
           l_texture[j+1] = rgb.rgbtGreen; // Green component 
           l_texture[j+2] = rgb.rgbtBlue; // Blue component 
           l_texture[j+3] = 255; // Alpha value，按照需求设定 
           j += 4; // Go to the next position，现在知道为什么当初乘以4了吧：） 
   }
那么，下一步我们就要调用openGL中的函数，生成一张纹理。这里glBindTexture的目的就是告诉openGL要把2D纹理目标绑定到一个名为texture[0]的东东，也就是&texture[0]这个地址上
    glGenTextures(1, &texture[0]);// 生成一张纹理，texture得之前声明好了 
    glBindTexture(GL_TEXTURE_2D, texture[0]);   
下面这两个函数是用来设定filtering的方式，第一行说当纹理被缩放的时候，我们用GL_LINEAR方式过滤，第二行说当纹理被放大的时候也用GL_LINEAR方式。当然还有别的方式，比如GL_NEAREST, 这个方式比较省显卡，但是当纹理被拉伸的时候就成了一块一块的了。还有GL_LINEAR_MIPMAP_NEAREST。一般都用GL_LINEAR了，现在这个年代显卡不至于这个都搞不定：）更多的参数，参见openGL编程指南吧
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  下面这个函数很重要哦，现在我就一个一个变量分析。第一项就是说这是一个二维纹理；第二项表示图像的详细情况（images level of detail), 坦白讲我也不太清楚，总之大多数情况都设定为0，咱就不用管了；第三项说，纹理有几个组成部分，一般都是3或者4，或者写GL_RGB, GL_RGB8, 这里我们以后还会说。第四项和第五项就是说纹理的宽度和高度，也就是纹理的尺寸；第六项的0，指的是边缘的宽度，一般都设0了，好事者可以试试别的。第七项的意思就是我们的按照RGBA的顺序。第八项就是说纹理的信息是GL_UNSIGNED_BYTE存的，为什么是unsigned？很显然颜色空间没有负值啊。好了最后一项告诉openGL从哪取出纹理的信息。这里就是刚才我们把BMP读到的那个数组。
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
这里说一下，这里面如果你可以直接用的是glaux里的函数和变量。比如：
    AUX_RGBImageRec *TextureImage[4];//声明临时存放读取纹理信息的变量
    //打开文件的代码这里省略了
    TextureImage[0] = auxDIBImageLoad("Data/pic.bmp")//这个就直接搞定了
而glTexImage2D函数就成了这种形式
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImage[0]->sizeX, 
     TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
好了，不管你用的是哪种。下一步就是要把临时的那个空间free掉。从读纹理到生成纹理的过程就是这样。
**3 Mapping your 2D Texture**
现在你就可以画一个东西了。当然你肯定最开始得先enable纹理，并且绑定要使用的纹理
glEnable(GL_TEXTURE_2D); 
glBindTexture(GL_TEXTURE_2D, texture[0]);
比如说要画一个四边形，先指定二维纹理的offset，再给定位置坐标,就可以了。
glBegin(GL_QUADS); 
glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); 
glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f); 
glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f); 
glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
glEnd();
结果就是下面这种，下图是著名的NeHe的crate
![images](http://images.cnblogs.com/cnblogs_com/antai/WindowsLiveWriter/openGLTextureMultiTextureMapping_12B6E/images_thumb.jpg)
**4 Advanced Texture Mapping**
**4.1 Something more about glFunctions**
现在我们就再说说相关的函数, 下面这个函数的意思就是纹理的u,v offset如果超过了（0，1）那么就重复。什么意思呢？比如我们想把一块小的砖墙纹理贴很大的区域的时候，就要重复。与GL_REPEAT对应的是GL_CLAMP，紧紧抓牢，如果在（0，1）区间以外，就不贴了，变成黑色。
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
看下面这个语句，它的意思是，我们用纹理信息取代之前的信息。比如说之前有颜色信息，光照信息，用了GL_REPLACE之后就只保留纹理信息。很多人问为什么贴了纹理之后加光照没有效果或者颜色信息没了，这个问题很可能就是由于这个函数的设定。事实上，这个函数第三项默认是GL_MODULATE,进行了混合。在后文中还会讲道这个函数。
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
可能用到函数还有很多，下面这个函数的意思是，对于每一个像素，从读取的纹理信息中，把乘以一个scale之后的结果作为生成的纹理的信息。下面这行意思就是把红色的强度减半。
glPixelTransferf(GL_RED_SCALE,0.5f); //could be GREEN, BLUE also
更多的关于拷贝纹理，三维纹理，遮盖一部分纹理的函数参见openGL编程指南。 
**4.2 MultiTexture:)**
一本文最开始的图的效果为例，下面来说说如何用GL_ARB_multitexture相关的函数生成多重纹理。
首先，GLext.h需要include进来。第一步是声明函数指针：
PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL; 
PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL; 
PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB        = NULL; 
PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;    
第二步获取指针的地址，我是把下面的代码加在init()函数中:
    glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
    glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");
下一步读文件，生成纹理。注意这里glTexImage2D的第三项一定要是GL_RGB8，这样才能有多重纹理的混合。
    glGenTextures(1, &texture[0]);                   
    glBindTexture(GL_TEXTURE_2D, texture[0]); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
第四步，设定与激活。注意下面代码中glTexEnvf函数中最后一项的参数设定。
glActiveTextureARB(GL_TEXTURE0_ARB); 
glEnable(GL_TEXTURE_2D); 
glBindTexture(GL_TEXTURE_2D, texture[0]); 
glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);    
glActiveTextureARB(GL_TEXTURE1_ARB); 
glEnable(GL_TEXTURE_2D); 
glBindTexture(GL_TEXTURE_2D, texture[1]); 
glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);    
好了，现在终于可以用啦：）
glBegin(GL_QUADS); 
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0); 
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0); 
        glVertex3f(-1.0f, -1.0f,  1.0f); 
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 0); 
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0); 
        glVertex3f( 1.0f, -1.0f,  1.0f); 
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 1); 
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1); 
        glVertex3f( 1.0f,  1.0f,  1.0f); 
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1); 
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1); 
        glVertex3f(-1.0f,  1.0f,  1.0f);
glEnd();
**5 Conclusion and Discussion**
有了多重纹理你就可以做很多事情了，比如混合石头和草皮的纹理贴在一个山地模型上，混合毛发和皮肤的纹理贴在人和动物身上。
我的这篇文章仅仅是把基本的纹理/多重纹理讲了。其实openGL只是提供给我们最基本的函数，要完成复杂的效果，需要我们自己编写函数，结合物理和数学知识，综合运用light, material, blend等。
这里要感谢的就是那些为openGL提供很多库函数的大哥们，他们让我们的工作变得如此轻松。
And Here are my questions，欢迎大家讨论留言啊：）
1、blend和多重纹理在功效上的相同和不同？
2、有哪些方法可以分配每个纹理对于总纹理的贡献？
***Reference***
[1] NeHe lesson 6
[2] NeHe lesson 22
[3] spaceship simulator tutorial 4
[4] openGL Programming Guide
[5] OpenGL 中如何使用 MultiTexture. 即如何开启多层贴图
[6] Texture Mapping in OpenGL: Beyond the Basics
[7] The ARB_multitexture extension
[8] opengl.org
