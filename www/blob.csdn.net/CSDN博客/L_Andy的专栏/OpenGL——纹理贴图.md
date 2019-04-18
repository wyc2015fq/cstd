# OpenGL——纹理贴图 - L_Andy的专栏 - CSDN博客

2016年08月28日 20:22:05[卡哥](https://me.csdn.net/L_Andy)阅读数：4340


1. 创建纹理图像

OpenGL要求纹理的高度和宽度都必须是2的n次方大小，只有满足这个条件，这个纹理图片才是有效的。一旦获取了像素值，我们就可以将这些数据传给OpenGL，让OpenGL生成一个纹理贴图：

    glGenTextures(1,@Texture);
    glBindTexture(GL_TEXTURE_2D,Texture);
    glTexImage2D(GL_TEXTURE_2D,0,3,Bit.Width,Bit.Height,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);

glGenTextures和glBindTexture函数用于创建和绑定纹理对象，glTexImage2D函数将Pixels数组中的像素值传给当前绑定的纹理对象，于是便创建了纹理。glTexImage函数的参数分别是纹理的类型，纹理的等级，每个像素的字节数，纹理图像的宽度和高度，边框大小，像素数据的格式，像素值的数据类型，像素数据。

2.OpenGL中的贴图方式

    OpenGL为我们提供了三种纹理——GL_TEXTURE_1D、GL_TEXTURE_2D和GL_TEXTURE_3D。它们分别表示1维纹理、2维纹理和3维纹理。无论是哪一中纹理，使用方法都是相同的：即先创建一个纹理对象和一个存储纹理数据的n维数组，在调用glTexImage*N* D函数来传入相应的纹理数据。除此之外，我们可以一些函数来设置纹理的其他特性。

2.1设置贴图模式

    OpenGL提供了3种不同的贴图模式：GL_MODULATE,GL_DECAL和GL_BLEND。默认情况下，贴图模式是GL_MODULATE，在这种模式下，OpenGL会根据当前的光照系统调整物体的色彩和明暗。第二种模式是GL_DECAL，在这种模式下所有的光照效果都是无效的，OpenGL将仅依据纹理贴图来绘制物体的表面。最后是GL_BLEND，这种模式允许我们使用混合纹理。在这种模式下，我们可以把当前纹理同一个颜色混合而得到一个新的纹理。我们可以调用glTexEnvi函数来设置当前贴图模式：

glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,*TextureMode*);

其中TextureMode就是想要设置的纹理模式，可以为GL_MODULATE,GL_DECAL和GL_BLEND中的任何一种。

另外，对于GL_BLEND模式，我们可以调用

glTexEnvfv(GL_TEXUTRE_ENV,GL_TEXTURE_ENV_COLOR,@*ColorRGBA*);

其中，ColorRGBA为一个表示RGBA颜色的4维数组。

2.2纹理滤镜

在纹理映射的过程中，如果图元的大小不等于纹理的大小，OpenGL便会对纹理进行缩放以适应图元的尺寸。我们可以通过设置纹理滤镜来决定OpenGL对某个纹理采用的放大、缩小的算法。

调用glTexParameter来设置纹理滤镜。如：

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILETER, *MagFilter*);//设置放大滤镜
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, *MinFilter*); //设置缩小滤镜

上述调用中，第一个参数表明是针对何种纹理进行设置，第二个参数表示要设置放大滤镜还是缩小滤镜。第三个参数表示使用的滤镜。可以为下面的值之一：

GL_LINEAR_MIPMAP_LINEAR

在最近多贴图等级的外部和内部线性插值

3纹理映射

3.1纹理坐标

要使用当前的纹理绘制图元，我们必须在绘制每个顶点之前为该顶点指定纹理坐标。只需调用

    glTexCoord2d(s:Double;t:Double);

函数即可。其中，s、t是对于2D纹理而言的s、t坐标。对于任何纹理，它的纹理坐标都如同图6所示的那样：

![](https://img-blog.csdn.net/20160828201907990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[](http://photo.blog.sina.com.cn/showpic.html#blogid=62dfaf550100g2l1&url=http://s5.sinaimg.cn/orignal/62dfaf55x782dacdacf34&690)

对于任何纹理，无论纹理的真正大小如何，其顶端（左上角）的纹理坐标恒为(0,0),右下角的纹理坐标恒为(1,1)。也就是说，纹理坐标应是一个介于0到1之间的一个小数。

例如，下面的代码将使用当前纹理绘制一个三角形：

     glBindTexture(Tex);
      glBegin(GL_TRIANGLES);
        glTexCoord2d(0,0);
        glVertex3f(-10,-10,0);
        glTexCoord2d(0,1);
        glVertex3f(-10,10,0);

        glTexCoord2d(1,1);
        glVertex3f(10,10,0);

      glEnd();

3.2纹理缠绕

前面提到，纹理坐标应位于0-1之间。那么当纹理坐标大于这个值会出现什么情况呢？

我们可以对OpenGL进行设置，以决定当纹理坐标不位于这一区间时应采取的操作。我们可以指定两种操作：GL_CLAMP和GL_REPEAT。对于GL_CLAMP,超出纹理坐标的区域会使用纹理图像的边界颜色来代替，如图所示。

![](https://img-blog.csdn.net/20160828201916506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[](http://photo.blog.sina.com.cn/showpic.html#blogid=62dfaf550100g2y4&url=http://s13.sinaimg.cn/orignal/62dfaf55x784021ab8f6c&690)

而GL_REPEAT方式则是对纹理坐标进行重置而得到重复的图像。观察图，你就能很容易地发现这一点。

![](https://img-blog.csdn.net/20160828201920457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://photo.blog.sina.com.cn/showpic.html#blogid=62dfaf550100g2y4&url=http://s14.sinaimg.cn/orignal/62dfaf55x784026c7102d&690)

可以调用glTexParameter设置缠绕方式：

glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,*WrapMode*);//在s方向上的缠绕方式
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,*WrapMode*);//在t方向上的缠绕方式

其中，WrapMode可取GL_CLAMP或者GL_REPEAT。

4.纹理对象

创建和使用文理对象

在OpenGL中，我们使用glGenTextures创建纹理对象：

glGenTextures(Count:Integer;TexObjs:Pointer);

其中，Count是我们要创建的纹理数目，当我们只想创建一个纹理时，只需调用

varTexture:GLUint;
...
glGenTextures(1,@Texture);

这样，Texture变量中就存储了我们创建的纹理的ID号。

创建之后，我们使用glBindTexture将创建的纹理绑定到当前纹理。这样所有的纹理函数都将针对当前纹理。

glBindTexture(Texture:GLUint);

这样，我们就可以调用glTexParameter、glTexImage2D等函数来设置这个纹理对象了。

删除纹理对象

在纹理资源使用完毕后(一般是程序退出或场景转换时)，一定要删除纹理对象，释放资源。

调用

glDeleteTextures(Count:Integer;TexObj:Pointer);

来删除纹理对象。例如

glDeleteTextures(1,@Texture);

5.多贴图纹理

多贴图纹理(Mip Mapping)为一个纹理对象生成不同尺寸的图像。在需要时，根据绘制图形的大小来决定采用的纹理等级或者在不同的纹理等级之间进行线性内插。使用多贴图纹理的好处在于消除纹理躁动。这种情况在所绘制的景物离观察者较远时常常发生(如图6.6-1和6.6-2)。由于多贴图纹理现在的渲染速度已经很快，以至于和普通纹理没有什么区别，我们现在一般都使用多贴图纹理。

![](https://img-blog.csdn.net/20160828201911989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[](http://photo.blog.sina.com.cn/showpic.html#blogid=62dfaf550100g2y4&url=http://s6.sinaimg.cn/orignal/62dfaf55x78402aafdcb5&690)

使用多贴图纹理并不麻烦。首先，我们需要创建不同等级（尺寸）的纹理图片。我们需要调用n次glTexImage2D函数，生成不同等级的纹理贴图。例如：
glTexImage2D(GL_TEXTURE_2D,0,3,8,8,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);
glTexImage2D(GL_TEXTURE_2D,1,3,4,4,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);
glTexImage2D(GL_TEXTURE_2D,2,3,2,2,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);
glTexImage2D(GL_TEXTURE_2D,3,3,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);

这些函数调用的第二个参数表示当前纹理的等级。0级的分辨率最大。之后，每一级的分辨率是上一级分辨率的一半。这样的函数调用应一直进行下去，直至图像的高度和宽度都为1。

但有时候，这样做总并不是很方便。我们可以借助一个glu函数帮我们自动生成这些多贴图纹理。只需要把生成纹理图像的函数调用由glTexImage2D改为gluBuild2DMipMaps即可：

gluBuild2DMipMaps(GL_TEXTURE_2D,3,Bit.Width,Bit.Height,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);

此外，还必须把纹理的滤镜改为MIP_MAP滤镜。例如：

glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

