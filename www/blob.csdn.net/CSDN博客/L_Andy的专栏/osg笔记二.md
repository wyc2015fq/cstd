# osg笔记二 - L_Andy的专栏 - CSDN博客

2015年04月28日 16:33:46[卡哥](https://me.csdn.net/L_Andy)阅读数：4681


卡哥（张明奇）

///////////////////////////////////////////////////////////////////

createTexturedQuadGeometry(corner, widthVec, heightVec, l, b, r, t)

这个函数可以方便地创建一个空间四边形，位置和大小由corner，widthVec，heightVec决定；l，b则是纹理坐标的左下角点；r，t是右上角点

Convenience function to be used for creating quad geometry with texture coords. 

这个函数作用是创建一个带有纹理坐标的四边形～～～纹理贴图你在Texture2D里面设置就可以了，跟这个函数没什么关系～～～～～

////////////////////////////////////////////////////////////////

求助！换纹理的函数setImage()与dirty()有什么区别？

void dirty() {_dirty = true;}是告诉 OSG 需要重新读取顶点坐标、纹理等：if (_dirty) update();。 所以 setImage() 之后，需要调用 dirty()

setImage()会自动dirty()

//////////////////////////////////////////////

setImage()第四个参数GLint internalTextureformat的意义，应该如何设置它

也就是glTexImage*()的internalFormat参数，简单来说就是颜色分量的数目

/////////////////////////////////////////////////

这个函数是这样的

void osg::Image::setImage  ( int  s,  

  int  t,  

  int  r,  

  GLint  internalTextureformat,  

  GLenum  pixelFormat,  

  GLenum  type,  

  unsigned char *  data,  

  AllocationMode  mode,  

  int  packing = 1   

) 

其中mode参数有三种：

Enumerator: 

NO_DELETE   

USE_NEW_DELETE   

USE_MALLOC_FREE   

想问下，这三种分别是什么意思呢？还有，最后那个packing有什么作用？谢谢、、

Image对象在默认时会负责维护data的内存区域，例如在Image对象析构时自动释放data所用的空间。

USE_NEW_DELETE即使用new和delete操作符来完成这一操作。而USE_MALLOC_FREE即使用C形式的malloc和free来完成。

NO_DELETE表示内存区域不要由Image负责，而是您自己控制

///////////////////////////////////////////////////////

camera.addSlave中的projectionOffset, ViewOffset怎么理解？

projectionOffset跟NDCS中的坐标范围（-1,1）有什么关系？

具体数学推导过程是什么样的？有没有可参考的书籍？

谢谢！

可以直接理解为从相机的viewmatrix = 主相机matrix * offset

/////////////////////////////////////////////////////////////

setUserData后能不能保存文件？

在osg中，利用setUserData，将一些信息添加到一个节点上，然后将节点保存为*.osg或者*.ive文件，再次读取时，userdata却为空了。有人遇到过同样的问题吗？

setUserData保存的数据通常不能被保存到osg或ive文件中，因为系统不可能知道用户自定义数据是什么内容

通常可以自己写插件实现

/////////////////////////////////////////////////////////////

osgearth关于近地面Camera远近裁剪面的设置

视点接近地面时，由于默认的近裁剪面near太大，导致看不见靠近摄像机的物体，这时需要重新调节near和far的值（或者near/far的比率），

可以使用osgEarth::Util::AutoClipPlaneCullCallback(osgEarth::MapNode* mapNode)

例如：

osgViewer::Viewer viewer;

viewer.getCamera()->addCullCallback( new osgEarth::Util::AutoClipPlaneCullCallback(mapNode) );

//////////////////////////////////////////////////////////////////////////////////////////////////

请教一下osg::EllipsoidModel和osg::CoordinateSystemNode的问题

osg::EllipsoidModel为椭球坐标系模型

osg::CoordinateSystemNode功能和group类似，只是提供了和osg::EllipsoidModel关联的坐标运算接口

osg::CoordinateSystemNode其实不光是一个运算接口而已，OSG会在遇到CoordinateSystemNode时自动对其子节点进行坐标变换以保证大地坐标的正确渲染

////////////////////////////////////////////////////////////////////////////////////////

默认情况下，OpenGL会在每一个顶点同时使用平滑着色与混合颜色：

//////////////////////////////////////////////////////////////////////////////////////////////////////

osgdem进行osgearth文件处理

一、在终端输入：osgdem -h 查看帮助信息

osgdem的作用：

1，可以重投影，从投影生成中间文件就是所需要的文件。

2，生成.ive地景文件

osgdem -t globel.tif -d srtm30plus_stripped.tif --geocentric -l 4 -o TestCommon/output.ive

osgdem -t globel.tif -d srtm30plus_stripped.tif -v 10.0 --geocentric -l 4 -o TestCommon10/output.ive

gdalwarp可以进行切割.tif文件也可以用工具erdas进行切割

gdalwarp -rcs  -t_srs "+proj=latlong"  -te 108 34 109 35 srtm30plus_stripped.tif  10934.tif

//gdalwarp对地图文件进行坐标转换(魔卡托投影坐标-WGS84)

gdalwarp -t_srs WGS84 cuicun.tif cuicun_84.tif

//用ArgGIS进行坐标转换

osgEarth_cache做缓存

osgEarth_cache --seed china-simple.earth --min-level 1 --max-level 3

osgEarth_cache --seed china-simple.earth --bounds 105 30 115 35 --min-level 1 --max-level 5

osgEarth_cache --seed china-simple.earth --bounds 108.5 34 109.5 35 --min-level 1 --max-level 6

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

![](https://img-blog.csdn.net/20160117195511312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//说明 不改源码显示中文,不用修改默认字体环境变量

 支持中文把mif mid改成utf8即可(注意name字段字节数可能不够)

        <styles>

            <style type="text/css">

              cities {

              text-provider: annotation;

              text-content:  [name];

              text-halo:     #3f3f7f;

              text-align:    center_center;

              text-font:     simfang.ttf;

              text-encoding: utf-8;

              }

            </style>

        </styles>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

查看osg第三方库插件是否编译成功

![](https://img-blog.csdn.net/20171010195636127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171010195641276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

osg环境变量配置

在系统变量一栏，新建以下变量：

变量名：OSG_ROOT 变量值：C:\Program Files\OpenSceneGraph;

变量名：OSG_BIN_PATH 变量值：%OSG_ROOT%\bin;

变量名：OSG_INCLUDE_PATH 变量值：%OSG_ROOT%\include;

变量名：OSG_LIB_PATH 变量值：%OSG_ROOT%\lib;

变量名：OSG_SAMPLES_PATH 变量值：%OSG_ROOT%\share\OpenSceneGraph\bin;

变量名：OSG_FILE_PATH 变量值：%OSG_ROOT%\samples\OpenSceneGraph-Data-3.0.0;

在系统变量PATH下添加;C:\Program Files\OpenSceneGraph\bin

