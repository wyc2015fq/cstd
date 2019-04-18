# 关于Opengl各种矩阵变换(MVPW)的自我理解 - L_Andy的专栏 - CSDN博客

2015年01月06日 12:27:02[卡哥](https://me.csdn.net/L_Andy)阅读数：2807


# 关于Opengl各种矩阵变换的理解

OpenGL的矩阵变换一直比较迷糊，貌似懂了又貌似不懂，今天貌似懂了过几天又貌似忘记怎么回事了，还是没有理解透彻，因此痛下决心，一定要把这个事情弄懂。经过几天的反复思考查看资料，我自己理解的结果就是：mv用来调整相机位置组成一个新坐标系，p用来在这个坐标系里面切出一块来变成-1，1之间的立方体显示，w用来把这个-1，1的立方体转到指定的视口上。如果这个不够透彻，请看下面的详细解释。

### 一、相机模拟过程

首先是用相机模拟说明openGL的投影原理。下面这个图应该是把原理解释得比较透彻的一个了。

[](http://photo.blog.sina.com.cn/showpic.html#blogid=537cc4d9010172o9&url=http://s14.sinaimg.cn/orignal/537cc4d9gc3ace055b64d)![](https://img-blog.csdn.net/20150112163636251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
实际上，从三维空间到二维平面，就如同用相机拍照一样，通常都要经历以下几个步骤 （括号内表示的是相应的图形学概念）：

第一步，将相机置于三角架上，让它对准三维景物（视点变换，Viewing Transformation，对应openGL是glLookAt，从哪里看？）。

第二步，将三维物体放在适当的位置（模型变换，Modeling Transformation，对应openGL是各种transform/scal/rotate，比如物体移动旋转缩放等。）。

第三步，选择相机镜头并调焦，使三维物体投影在二维胶片上（投影变换，Projection Transformation，对应openGL就是gluPerspective/glOrtho等）。

第四步，决定二维像片的大小（视口变换，Viewport Transformation，对应openGL就是glViewport等）。

这样，一个三维空间里的物体就可以用相应的二维平面物体表示了，也就能在二维的电脑屏幕上正确显示了。

### 二、数字显示原理

我们建模是3D的，使用各种坐标系，如世界坐标系，用户坐标系等等。屏幕则是2D的，一般都是0-1024，0-768之类的范围。为了使显示的物体能以合适的位置、大小和方向显示出来，必须要通过投影。投影的方法有两种，即正射投影和透视投影。
　　有时为了突出图形的一部分，只把图形的某一部分显示出来，这时可以定义一个三维视景体（Viewing Volume）。正射投影时一般是一个长方体的视景体，透视投影时一般是一个棱台似的视景体。只有视景体内的物体能被投影在显示平面上，其他部分则不能。在屏幕窗口内可以定义一个矩形，称为视口（Viewport），视景体投影后的图形就在视口内显示。
　　为了适应物理设备坐标和视口所在坐标的差别，还要作一适应物理坐标的变换。这个坐标系称为物理设备坐标系（比如Windows的屏幕坐标原点是在屏幕左上方，我们使用的坐标系一般是屏幕左下方为坐标原点）。根据上面所述，三维图形的显示流程应如图8-2所示。

![](https://img-blog.csdn.net/20150112163624765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面的图则是openGL实际上运行的过程。

![](https://img-blog.csdn.net/20150112163707248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

通俗一点，世界坐标系有一个100，100，100的点；用某个相机lookAt后，变成了一个50，100，50的点（相机坐标系中）；用了某个投影矩阵投影后，变成了一个0.5，0.7，0的点，这是投影到平面并且归一化到-1，1区间的结果；设置了视口后，变成了一个500，700的点，这是屏幕坐标系了；和视口Y坐标减一下以后变成了500，68的点，这是屏幕像素物理坐标了（从上往下数68像素）。

### 三、逐个步骤理解

OpenGL里面，这些步骤总结下来，其实就是3个矩阵变换，合起来叫VPW矩阵。理解：V表示相机的观察（view）矩阵；P是投影（Project）矩阵，W是窗口（Window）矩阵，其实叫MVPW矩阵更好，是modelview * projection * window三个矩阵的级联。Model矩阵其实就是用户定义坐标系转成世界坐标系才需要用。

**1、View**

V表示观察矩阵（view），对应opengl是glLookat，作用是把世界坐标系转成相机坐标系。比如世界坐标系100，100的点，在某个相机观察的时候，因为相机点要变成原点，所以可能就变成了50，50，glLootAt参数是一个相机点，一个目标点（这两个点确定Z轴），一个向上的向量（确定X轴），这三个显然就可以确定一个坐标系，等于是把世界坐标系转成相机的UCS坐标系，举例来说，CAD里面平移操作就是移动了相机，透视图里面走动也相当于移动了相机。这时候没有进行任何裁剪。

**2、Project**

P表示投影矩阵（project），有两种：透视投影，和正交投影。主要用途就是从视觉坐标系中裁剪一块立体区域，变换到一个-1，1的正方体中方便裁剪（因为视锥体裁剪很不方便），最后去掉Z坐标落到屏幕平面上。透视投影是裁剪一个锥台区域，正交投影是裁剪一个长方体区域。这些投影函数的参数就需要注意了，不是世界坐标系的参数哦。因为默认情况下lookat是从0，0点向下看，视觉坐标情况和世界坐标系相当，所以不用glulookat的时候设置投影函数是可以用世界坐标系坐标的；如果设定了视觉坐标系，那么就不一样了。

透视投影矩阵相当于变换相机焦距，焦距小范围就大，能看近物，类似广角；焦距大范围就小，但是远处的能过来，类似长焦。对应opengl就是gluPerspectiv/glFrustum 函数，。两个函数中，glFrustum更正规，功能更强，但是也更不好用一点（需要自己计算宽高比，通过剪切面宽高和近裁剪面距离的三角关系计算视角多少）；

void glFrustum(GLdouble left,GLdouble Right,GLdouble bottom,GLdouble top,GLdouble near,GLdouble far)，建议用gluPerspective。

gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar)。

fovy,这个最难理解,我的理解是,眼睛睁开的角度,即,视角的大小,如果设置为0,相当你闭上眼睛了,所以什么也看不到,如果为180,那么可以认为你的视界很广阔.这应该和焦距对应，视角小就是焦距大（长焦），视角大就是焦距小（广角）。对应glFrustum，就是near设成0.01，left/right设置成1之类，趋近于90度了。

aspect,这个好理解,就是实际窗口的纵横比,即x/y

zNear,这个呢,表示你近处,的裁面,

zFar表示远处的裁面,正交投影则用于2D图形，图形不会像透视那样有变形。

glOrtho是正交投影，参数为glOrtho(left, right, bottom, top, near, far)， left表示视景体左面的坐标，right表示右面的坐标，bottom表示下面的，top表示上面的。这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他。如果截取范围大，物体在整个范围内的比例就小，相当于缩小；截取范围小，物体占据比例就大，相当于放大。然后截取范围偏，物体相当于在整个范围内就偏了，相当于平移。这个截取体需要和窗口范围比例一致，否则就会变形。因为不管是透视投影还是正交投影，它们的结果都是把你指定的一定范围内的图形单位化成了-1，1之间的值。

当然这里还需要再次强调，glOrtho和gluPerspect函数，使用的范围，都是基于gluLookAt的，并不是世界坐标里面的范围。

我们假设物体的绘制函数是

glBegin(GL_TRIANGLES);

glVertex3f(100,300,600);

glVertex3f(300,100,400);

glVertex3f(100,100,600);

glEnd();

究竟应该如何用这两个函数设置投影？

最简单的设置方法是glOrtho(-200,200,-200,200,-500,500);

gluLookAt(300,300,600,200,200,5000,1,0);  

可以看到，glOrtho的参数显然不是绝对的世界坐标，而是lookAt出来的坐标系里面的坐标。

**3、Window**

W表示视口矩阵（window），对应opengl就是glviewport。因为屏幕有大小，最后-1，1的之间裁剪结果总要映射回屏幕才行，glViewport就是告诉屏幕多大，怎么映射回去。如果这个和投影时使用的比例不一致，就是变形。gluProject可以直接完成这几个变换（直接把物体的世界坐标系的点变成屏幕上的坐标点，当然Y值还需要掉转，因为OpenGL坐标系和窗口坐标系不一致）。

再回顾一下一下opengl四种坐标变换的含义。模型变换：用于调整物体的大小、位置，模型边换之后得到的物体坐标是相对于全局坐标系统（世界坐标系）的。视图变换（V）：用于设置相机的位置、相机轴线的方向以及拍摄的方向。模型变换和视图变换统称为模型视图变换（modelview），经过模型视图变换之后的坐标是视觉系统内的坐标，其实是模型相对于相机的坐标，因为这里是模型和视图的一种相对关系，因此模型变换和视图变换具有一定的等价性和可转换性。

接着是投影变换（P），投影变换的效果是建立一个裁剪用的视景体（锥台体或者长方体），投影位于其中的物体，对视景体外的物体部分进行截断，投影变换之后的坐标反应的是物体在视景体内的位置属性，比如同样大小的面（在全局坐标系统中），透视投影时靠近视景体前端的占其相对应的截面的比例更大，因此在变换后的面积也更大。正视投影（OrthoView）时就还是同样大小的一个区域了。最后是视口变换，其作用是将确定最终图像的大小，并将帧缓冲区域内的数据转变为能显示在屏幕上的像素。

由于变换是通过左乘矩阵完成的，因此实际坐标变换的顺序和代码中出现的顺序是相反的。一般先设置模型视图变换，再设置投影变换，最后是视口变换，并且由于改变窗口只对投影和视口变换有影响，因此常将这两种变换放在reshape函数里。

### 三、来点实际的

说一千道一万，不实际尝试总不能确定的。主要是弄清楚这几个矩阵和变换之间的情况。下面就是一些实际操作的结果（使用osg）。

首先是第一个，初始情况：

弄了一个-5000，-5000开始的坐标网，及一个0，0到10000，10000的矩形。我直接用了一个投影矩阵，其他什么都没有（视觉矩阵为单位阵，窗口矩阵视口设置就不变了）：

camera->setProjectionMatrixAsOrtho2D(-15000,15000,-15000*scale,15000*scale);

![](https://img-blog.csdn.net/20150112163719354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面这个设置是这样的，没有对投影进行和窗口宽高比的处理，显然就变形了：camera->setProjectionMatrixAsOrtho2D(-15000,15000,-15000,15000);//变形?

![](https://img-blog.csdn.net/20150112163655734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面这个呢，是设置了下面的参数（缩小了10倍），可以看到，只是裁剪了一小块区域了。

camera->setProjectionMatrixAsOrtho2D(-1500,1500,-1500*scale,1500*scale);//放大了？

![](https://img-blog.csdn.net/20150112163745593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面这个增加了一个lookAt函数，等于以前的图都是视觉坐标系和世界坐标系重合，这个呢，视觉坐标系原点移到了5000，5000的位置，整个图形就显得偏了。偏下来了。证明投影时用的是视觉坐标系的点。

camera->setViewMatrixAsLookAt(osg::Vec3(5000,5000,1),osg::Vec3(5000,5000,0),osg::Vec3(0,1,0));

camera->setProjectionMatrixAsOrtho2D(-15000,15000,-15000*scale,15000*scale);//移位![](https://img-blog.csdn.net/20150112163753222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


