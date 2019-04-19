# 从零开始学习OpenGL ES之六 – 纹理及纹理映射 - xqhrs232的专栏 - CSDN博客
2010年12月03日 16:44:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：879
原文地址::[http://blog.csdn.net/ipromiseu/archive/2010/08/31/5851418.aspx](http://blog.csdn.net/ipromiseu/archive/2010/08/31/5851418.aspx)
在OpenGL ES中另一种为多边形定义颜色创建材质的方法是将纹理映射到多边形。这是一种很实用的方法，它可以产生很漂亮的外观并节省大量的处理器时间。比如说，你想在游戏中造一个砖墙。你当然可以创建一个具有几千个顶点的复杂物体来定义每块砖以及砖之间的泥灰。
或者你可以创建一个由两个三角形构成的方块（四个顶点），然后将砖的照片映射上去。简单的几何体通过纹理映射的方法比使用材质的复杂几何体的渲染快得多。
功能启动
为使用纹理，我们需要打开OpenGL的一些开关以启动我们需要的一些功能：
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);第一个函数打开所有两维图像的功能。这个调用是必不可缺的；如果你没有打开此功能，那么你就无法将图像映射到多边形上。它可以在需要时打开和关闭，但是通常不需要这样做。你可以启动此功能而在绘图时并不使用它，所以通常只需在setup方法中调用一次。
下一个调用打开了混色（blending）功能。 混色提供了通过指定源和目标怎样组合而合成图像的功能。例如，它可以允许你将多个纹理映射到多边形中以产生一个有趣的新的纹理。然而在OpenGL中，“混色”是指合成任何图像或图像与多边形表面合成，所以即使你不需要将多个图像混合，你也需要打开此功能。
最后一个调用指定了使用的混色方法。混色函数定义了源图像怎样与目标图像或表面合成。OpenGL将计算出（根据我们提供的信息）怎样将源纹理的一个像素映射到绘制此像素的目标多边形的一部分。
一旦 OpenGL ES 决定怎样把一个像素从纹理映射到多边形，它将使用指定的混色函数来确定最终绘制的各像素的最终值。 glBlendFunc()函数决定我们将怎样进行混色运算，它采用了两个参数。第一个参数定义了怎样使用源纹理。第二个则定义了怎样使用目标颜色或纹理。在本文简单的例子中，我们希望绘制的纹理完全不透明而忽略多边形中现存的颜色或纹理，所以我们设置源为 GL_ONE，它表示源图像（被映射的纹理）中各颜色通道的值将乘以1.0或者换句话说，以完全颜色密度使用。目标设置为 GL_SRC_COLOR，它表示要使用源图像中被映射到多边形特定点的颜色。此混色函数的结果是一个完全不透明的纹理。这可能是最常用情况。我可能会在以后的文章中更详细地介绍一下混色功能，但这可能是你使用最多的一种组合，而且是今天使用的唯一混色功能。
注意：如果你已经使用过OpenGL的混色功能，你应该知道 OpenGL ES 并不支持所有 OpenGL 支持的混色功能。下面是 OpenGL ES 支持的：GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, 和 GL_SRC_ALPHA_SATURATE
 (它仅用于源)。
创建纹理
一旦你启动了纹理和混色，就可以开始创建纹理了。通常纹理是在开始显示3D物体给用户前程序开始执行时或游戏每关开始加载时创建的。这不是必须的，但却是一个好的建议，因为创建纹理需要占用一些处理器时间，如果在你开始显示一些复杂的几何体时进行此项工作，会引起明显的程序停顿。
OpenGL中的每一个图像都是一个纹理，纹理是不能直接显示给最终用户的，除非它映射到物体上。但是有一个小小的例外，就是对允许你将图像绘制于指定点的所谓点精灵（point sprites），但它有自己的一套规则，所以那是一个单独的主题。通常的情况下，任何你希望显示给用户的图像必须放置在由顶点定义的三角形中，有点像贴在上面的粘帖纸。
生成纹理名
为创建一个纹理，首先必须通知OpenGL ES生成一个纹理名称。这是一个令人迷惑的术语，因为纹理名实际上是一个数字：更具体的说是一个GLuint。尽管“名称”可以指任何字符串，但对于OpenGL ES纹理并不是这样。它是一个代表指定纹理的整数值。每个纹理由一个独一无二的名称表示，所以传递纹理名给OpenGL是我们区别所使用纹理的方式。.
然而在生成纹理名之前，我们要定义一个保存单个或多个纹理名的GLuint数组：
    GLuint      texture[1];尽管只有一个纹理，但使用一个元素的数组而不是一个GLuint仍是一个好习惯。当然，仍然可以定义单个GLuint进行强制调用。
在过程式程序中，纹理通常存于一个全局数组中，但在Objective-C程序中，使用例程变量保存纹理名更为常见。下面是代码：
    glGenTextures(1, &texture[0]);你可以调用glGenTextures()生成多个纹理；传递给OpenGL ES的第一个参数指示了要生成几个纹理。第二个参数需要是一个具有足够空间保存纹理名的数组。我们只有一个元素，所以只要求OpenGL ES产生一个纹理名。此调用后， texture[0] 将保持纹理的名称，我们将在任何与纹理有关的地方都使用texture[0]来表示这个特定纹理。
纹理绑定
在为纹理生成名称后，在为纹理提供图像数据之前，我们必须绑定纹理。绑定使得指定纹理处于活动状态。一次只能激活一个纹理。活动的或“被绑定”的纹理是绘制多边形时使用的纹理，也是新纹理数据将加载其上纹理，所以在提供图像数据前必须绑定纹理。这意味着每个纹理至少被绑定一次以为OpenGL ES提供此纹理的数据。运行时，可能再次绑定纹理（但不会再次提供图像数据）以指示绘图时要使用此纹理。纹理绑定很简单：
    glBindTexture(GL_TEXTURE_2D, texture[0]);因为我们使二维图像创建纹理，所以第一个参数永远是 GL_TEXTURE_2D。常规OpenGL支持其他类型的纹理，但目前分布在iPhone上的OpenGL ES版本只支持二维纹理，坦白地说，甚至在常规OpenGL中，二维纹理的使用也远比其他类型要多得多。
第二个参数是我们需要绑定的纹理名。调用此函数后，先前生成了纹理名称的纹理将成为活动纹理。
图像配置
在第一次绑定纹理后，我们需要设置两个参数。需要的话，有一些参数可以设置，但在iPhone上，这两个参数必须设定，否则纹理将不会正常显示。
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);必须设置这两个参数的原因是默认状态下OpenGL设置了使用所谓mipmap。今天我将不讨论mipmap，简单地说，我们不准备使用它。Mipmap是一个图像不同尺寸的组合，它允许OpenGL选择最为接近的尺寸版本以避免过多的插值计算并且在物体远离观察者时通过使用更小的纹理来更好地管理内存。感谢矢量单元和图形芯片，iPhone在图像插值方面做得很好，所以我们不需要考虑mipmap。我以后可能会专门撰写一篇文章讨论它，但我们今天讨论的是怎样让OpenGL
 ES通过线性插值调整图像到所需的尺寸。因为 GL_TEXTURE_MIN_FILTER 用于纹理需要被收缩到适合多边形的尺寸的情形，而 GL_TEXTURE_MAG_FILTER 则用于纹理被放大到适合多边形的尺寸的情况下，所以必须进行两次调用。在两种情况下，我们传递 GL_LINEAR 以通知OpenGL以简单的线性插值方法调整图像。
加载图像数据
在我们第一次绑定纹理后，必须为OpenGL ES提供纹理的图像数据。在iPhone上，有两种基本方法加载图像数据。如果你在其他书籍上看到使用标准 C I/O方法加载数据的代码，那也是不错的选择，然而这两种方法应该覆盖了你将遇到的各种情形。
UIImage方法
如果你想使用JPEG, PNG或其他UIImage支持的格式，那么你可以简单地使用图像数据实例化一个UIImage，然后产生图像的RGBA 位图数据：
    NSString *path = [[NSBundle mainBundle] pathForResource:@"texture" ofType:@"png"];
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    UIImage *image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    GLuint width = CGImageGetWidth(image.CGImage);
    GLuint height = CGImageGetHeight(image.CGImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc( height * width * 4 );
    CGContextRef context = CGBitmapContextCreate( imageData, width, height, 8, 4 * width,
        colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( context, 0, height - height );
    CGContextDrawImage( context, CGRectMake( 0, 0, width, height ), image.CGImage );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, imageData);
    CGContextRelease(context);
    free(imageData);
    [image release];
    [texData release];
前面几行代码很容易理解 – 从程序包中加载一个叫做 texture.png 的图像。然后使用一些 core graphics 调用将位图以 RGBA 格式存放。此基本方法是让我们使用任何UIImage支持的图像数据然后转换成OpenGL ES接受的数据格式。
注意：只是因为 UIImage 不支持一种文件类型并不意味着你不能使用此方法。你仍然有可能通过使用Objective-C的分类来增加 UIImage 对额外的图像文件类型的支持。在 这篇文章 我介绍了使 UIImage 支持 Targa 图像文件格式的方法。
一旦具有了正确格式的位图数据，我们就可以调用 glTexImage2D() 传递图像数据给 OpenGL ES。完成后，我们释放了一些内存，包括图像数据和实际 UIImage 的实例。一旦你传递图像数据给 OpenGL ES，它就会分配内存以拥有一份自己的数据拷贝，所以你可以释放所有使用的与图像有关的内存，而且你必须这样做除非你的程序有更重要的与数据相关的任务。即使是来自压缩过图像的纹理也会占用程序相当多的内存。每个像素占用四个字节，所以忘记释放纹理图像数据的内存会导致内存很快被用尽。
PVRTC方法
iPhone的图形芯片（PowerVR MBX）对一种称为 PVRTC 的压缩技术提供硬件支持，Apple推荐在开发iPhone应用程序时使用 PVRTC 纹理。他们甚至提供了一篇很好的 技术笔记 描述了怎样通过使用随开发工具安装的命令行程序将标准图像文件转换为 PVRTC 纹理的方法。
你应该知道当使用 PVRTC 时与标准JPEG或PNG图像相比有可能有些图像质量的下降。是否值得在你的程序中做出一些牺牲取决于一些因素，但使用 PVRTC 纹理可以节省大量的内存空间。
尽管因为没有Objective-C类可以解析 PVRTC 数据获取其宽和高1信息，你想要手工指定图像的高和宽，但加载 PVRTC 数据到当前绑定的纹理实际上甚至比加载普通图像文件更为简单。
下面的例子使用默认的texturetool设置加载一个 512×512 的PVRTC纹理：
    NSString *path = [[NSBundle mainBundle] pathForResource:@"texture" ofType:@"pvrtc"];
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    // This assumes that source PVRTC image is 4 bits per pixel and RGB not RGBA
    // If you use the default settings in texturetool, e.g.:
    //
    //      texturetool -e PVRTC -o texture.pvrtc texture.png
    //
    // then this code should work fine for you.
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, 512, 512, 0,
        [texData length], [texData bytes]);就这么简单。使用glCompressedTexImage2D()从文件加载数据并传送给OpeNGL ES。而随后怎样处理纹理则绝对没有任何区别。
纹理限制
用于纹理的图像宽和高必须为乘方，比如 2, 4, 8, 16, 32, 64, 128, 256, 512, 或 1024。例如图像可能为 64×128 或 512×512。
当使用 PVRTC 压缩图像时，有一个额外的限制：源图像必须是正方形，所以你的图像应该为 2×2, 4×4 8×8, 16×16, 32×32, 64×64, 128×128, 256×256, 等等。如果你的纹理本身不是正方形，那么你只需为图像加上黑边使图像成为正方形，然后映射纹理使得你需要的部分显示在多边形上。我们现在看看纹理是怎样映射到多边形的。
纹理坐标
当纹理映射启动后绘图时，你必须为OpenGL ES提供其他数据，即顶点数组中各顶点的 纹理坐标。纹理坐标定义了图像的哪一部分将被映射到多边形。它的工作方式有点奇怪。你有一个正方形或长方形的纹理，其左下角为二维平面的原点，高和宽的单位为一。像这样：
这就是我们的“纹理坐标系统”，不使用x 和 y 来代表二维空间，我们使用 s 和 t 作为纹理坐标轴，但原理上是一样的。
除了 s 和 t 轴外，被映射的纹理在多边形同样有两个轴，它们称为 u 和 v轴。这是源于许多3D图像程序中的UV 映射 的术语。
好，我们明白了纹理坐标系统，我们现在讨论怎样使用这些纹理坐标。当我们指定顶点数组中的顶点时，我们需要在另一个数组中提供纹理坐标，它称为纹理坐标数组。 每个顶点，我们将传递两个 GLfloats (s, t) 来指定顶点在上图所示坐标系统的位置。让我们看看一个可能是最为简单的例子，将整个图像映射到一个由三角形条组成的正方形上。首先，我们创建一个由四个顶点组成的顶点数组：
现在将两个框图叠在一起，所使用的坐标数组的值变得很明显：
将其转化为 GLfloat数组：
    static const GLfloat texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };为使用纹理坐标数组，我们必须启动它（正如你预料的那样）。使用 glEnableClientState()：
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);为传递纹理坐标，调用 glTexCoordPointer():
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);就是这样。我们汇总一下把代码置于 drawView: 方法中。它假设纹理已经被绑定和加载了。
- (void)drawView:(GLView*)view;
{
    static GLfloat rot = 0.0;
    glColor4f(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    static const Vertex3D vertices[] = {
        {-1.0,  1.0, -0.0},
        { 1.0,  1.0, -0.0},
        {-1.0, -1.0, -0.0},
        { 1.0, -1.0, -0.0}
    };
    static const Vector3D normals[] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}
    };
    static const GLfloat texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    };
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);
    glRotatef(rot, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    static NSTimeInterval lastDrawTime;
    if (lastDrawTime)
    {
        NSTimeInterval timeSinceLastDraw =
            [NSDate timeIntervalSinceReferenceDate] - lastDrawTime;
        rot+=  60 * timeSinceLastDraw;
    }
    lastDrawTime = [NSDate timeIntervalSinceReferenceDate];
}下面是我使用的纹理：
运行时的结果：
请等下：那并不正确。如果你仔细对比一下纹理图像和上面的截屏，你就会发现它们并不完全相同。截屏中图像的y轴（或t轴）完全颠倒了。它上下颠倒了，并不是旋转，而是翻转了。
T-轴翻转之谜
以OpenGL的角度来看，我们并未做错任何事情，但结果却是完全错误。原因在于iPhone的特殊性。 iPhone中用于Core Graphics的图像坐标系统并与OpenGL ES一致，其y轴在屏幕从上到下而增加。当然在OpenGL ES中正好相反，它的y轴从下向上增加。其结果就是我们早先传递给OpenGL ES中的图像数据从OpenGL ES的角度看完全颠倒了。所以，当我们使用标准的OpenGL ST映射坐标映射图像时，我们得到了一个翻转的图像。
普通图像的修正
当使用非PVRTC图像时，你可以在传递数据到OpenGL ES之前就翻转图像的坐标，将下面两行代码到纹理加载中创建OpenGL环境的语句之后：
        CGContextTranslateCTM (context, 0, height);
        CGContextScaleCTM (context, 1.0, -1.0);这将翻转绘制内容的坐标系统，其产生的数据正是OpenGL ES所需要的。下面是结果：
PVRTC 图像的修正
由于没有UIKit类可以加载或处理PVRTC 图像，所以没有一个简单的方法翻转压缩纹理的坐标系统。当然，我们还是有些方法处理这个问题。
一种方法是使用诸如 Acorn 或 Photoshop之类的程序中将图像转换为压缩纹理前简单地进行垂直翻转。这看似小诡计的方法在很多情况下是最好的解决方法，因为所有的处理都是事前进行的，所以运行时不需要额外的处理时间而且还允许压缩和未压缩图像具有同样的纹理坐标数组。
另一种方法是将t轴的值减一。尽管减法是很快的，但其占用的时间还是会累积，所以在大部分情况下，尽量要避免绘图时进行的转换工作。不论是翻转图像或翻转纹理坐标，都要在显示前进行加载时进行。
更多的映射方式
上个例子中这个图像都被映射到绘制的正方形上。那是因为设定的纹理坐标所决定的。我们可以改变坐标数组仅使用源图像的中心部分。让我们看看仅使用了图像中心部分的另一个框图：
其坐标数组为：
    static const GLfloat texCoords[] = {
        0.25, 0.75,
        0.75, 0.75,
        0.25, 0.25,
        0.75, 0.25
    };运行使用了新映射到程序，屏幕上只显示了图像的中心部分：
类似地，如果我们只希望显示纹理的左下部：
坐标数组为:
    static const GLfloat texCoords[] = {
        0.0, 0.5,
        0.5, 0.5,
        0.0, 0.0,
        0.5, 0.0
    };显示结果：
等一下，还有更多的方式
实际上，并不是真正还有更多的映射方式，只是说此功能在正方形到正方形的映射时并不很明显。同样的步骤适合于几何体中任何三角形，而且你甚至可以通过非常规方式的映射来扭曲纹理。例如，我们可以定义一个等腰三角形：
但将底部顶点映射到纹理的左下角：
这样的映射并不会改变几何体 – 它仍然是等腰三角形而不是直角三角形，但OpenGL ES将扭曲纹理使得第二个图中的三角形部分以等腰三角形的形式显示出来。代码如下：
- (void)drawView:(GLView*)view;
{
    glColor4f(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    static const Vertex3D vertices[] = {
        {-1.0,  1.0, -0.0},
        { 1.0,  1.0, -0.0},
        { 0.0, -1.0, -0.0},
    };
    static const Vector3D normals[] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
    };
    static const GLfloat texCoords[] = {
        0.0, 1.0,
        1.0, 0.0,
        0.0, 0.0,
    };
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}运行时结果如下：
注意到纹理正方形左下角的弧形花纹现在处于三角形的底部了吗？总而言之，纹理上的任何一点都可以映射到多边形的任何一点。或者换而言之，你可以对任何地点（u，v）使用任何（s，t）而OpenGL ES则为你进行映射。
平铺和箝位
我们的纹理坐标系统在两个轴上都是从0.0 到 1.0，如果设置超出此范围的值会怎么样？根据视图的设置方式有两种选择。
平铺（也叫重复）
一种选择是平铺纹理。按OpenGL的术语，也叫“重复”。如果我们将第一个纹理坐标数组的所有1.0改为2.0：
    static const GLfloat texCoords[] = {
        0.0, 2.0,
        2.0, 2.0,
        0.0, 0.0,
        2.0, 0.0
    };那么我们得到以下结果：
如果这就是你希望的结果，那么你应该在setupView:方法中通过glTexParameteri()函数启动它，像这样：
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);箝位
另一种可能的选择是让OpenGL ES简单地将超过1.0的值限制为1.0，任何低于0.0的值限制为 0.0。这实际会引起边沿像素重复，从而产生奇怪的效果。下图是使用了箝位的效果：
如果这是你希望的效果，那么你应该在setupView：方法中使用下面两行代码：
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);注意 s 和 t 轴是分别设置的，这样有可能在一个方向上使用平铺而在另一个方向使用箝位。
结论
本文介绍了OpenGL ES映射纹理到多边形的基本机制。尽管它很简单，但需要动下脑筋并自己动手才能真正理解它到底是怎样工作的，请下载texture_projects 自己测试。
下次我们将介绍矩阵，希望你到时回来。
--------------------------------------------------------------------------------
注脚 
实际上有一个先行版的示例代码介绍了怎样从文件中读取PVRTC头信息以决定图像的宽和高以及有关压缩图像文件的详细信息。我没有使用它是因为 a) 它还没有正式发布，如果我使用有可能违背了NDA， b) 此代码并不能读取所有的PVRTC文件（包括本文使用的图像）
感谢 George Sealy 和 Daniel Pasco关于t轴问题的帮助。感谢Apple Dev论坛的”Colombo”。
原文见：OpenGL ES From the Ground Up, Part 6: Textures and Texture Mapping
来源：[http://www.iphone-geek.cn/%e7%bc%96%e7%a8%8b/%e4%bb%8e%e9%9b%b6%e5%bc%80%e5%a7%8b%e5%ad%a6%e4%b9%a0opengl-es%e4%b9%8b%e5%85%ad-%e7%ba%b9%e7%90%86%e5%8f%8a%e7%ba%b9%e7%90%86%e6%98%a0%e5%b0%84](http://www.iphone-geek.cn/%E7%BC%96%E7%A8%8B/%E4%BB%8E%E9%9B%B6%E5%BC%80%E5%A7%8B%E5%AD%A6%E4%B9%A0opengl-es%E4%B9%8B%E5%85%AD-%E7%BA%B9%E7%90%86%E5%8F%8A%E7%BA%B9%E7%90%86%E6%98%A0%E5%B0%84)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/ipromiseu/archive/2010/08/31/5851418.aspx](http://blog.csdn.net/ipromiseu/archive/2010/08/31/5851418.aspx)
