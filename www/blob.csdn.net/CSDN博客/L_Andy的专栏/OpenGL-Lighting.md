# OpenGL - Lighting - L_Andy的专栏 - CSDN博客

2015年08月31日 16:46:44[卡哥](https://me.csdn.net/L_Andy)阅读数：955


1简单光照

OpenGL简单光照光源分为：

辐射光(Emitted Light)是最简单的一种光，它直接从物体发出并且不受任何光源影响。

环境光(Ambient Light)是由光源发出经环境多次散射而无法确定其方向的光，即似乎来自所有方向。

漫射光(Diffuse Light)来自一个方向，它垂直于物体时比倾斜时更明亮。

镜面光(Specular Light)来自特定方向并沿另一方向反射出去。

1.1创建光源

光源有许多特性，如颜色、位置、方向等。

设置光源特性参数的函数：

void glLight{if}[v]( GLenum light , GLenum pname , TYPEparam )

参数light指定所创建的光源号，如GL_LIGHT0，GL_LIGHT1，GL_LIGHT2
 ... ...

参数pname指定光源特性

参数param设置相应光源特性值

|pname参数名|缺省值|说明|
|----|----|----|
|GL_AMBIENT|( 0.0 , 0.0 , 0.0 , 1.0 )|环境光的颜色|
|GL_DIFFUSE|( 1.0 , 1.0 , 1.0 , 1.0 )|漫反射光的颜色|
|GL_SPECULAR|( 1.0 , 1.0 , 1.0 , 1.0 )|镜面光的颜色|
|GL_POSITION|( 0.0 , 0.0 , 1.0 , 0.0 )|光源位置坐标|
|GL_SPOT_DIRECTION|( 0.0 , 0.0 , -1.0 )|点光源聚光方向矢量|
|GL_SPOT_EXPONENT|0.0|点光源聚光指数|
|GL_SPOT_CUTOFF|180.0|点光源聚光截止角|
|GL_CONSTANT_ATTENUATION|1.0|常数衰减因子|
|GL_LINER_ATTENUATION|0.0|线性衰减因子|
|GL_QUADRATIC_ATTENUATION|0.0|平方衰减因子|

注：只有GL_LIGHT0的GL_DIFFUSE和GL_SPECULAR的缺省值为(
 1.0 ,1.0 ,1.0,1.0 )

其他光源的GL_DIFFUSE和GL_SPECULAR缺省值均为(
 0.0 ,0.0 ,0.0,1.0 )

1.2启动光照

glEnable( GL_LIGHTING ); //启用光照

glEnable( GL_LIGHT0 ); //启用指定光源

2光照模型

OpenGL光照模型由以下三部分组成：

2.1全局环境光

2.2近视点和无穷远视点

2.3双面光照

2.1全局环境光

每个光源都能对场景提供环境光。

此外，还有一个环境光，它不来自任何特定的光源，称之为全局环境光。

这也就是为什么我们不加任何自定义光源，也能看见绘制的物体的原因。

设置全局环境光：

float Light_Model_Ambient[] = { 0.2 , 0.2 , 0.2 , 1.0 };//缺省值

glLightModelfv( GL_LIGHT_MODEL_AMBIENT ,Light_Model_Ambient );

Light_Model_Ambient参数值指定了整个场景中的环境RGBA浓度。

最大值为1.0，最小值为-1.0。

2.2局部视点和无穷远视点

视点位置能影响镜面反射中高光的计算。

即顶点的高光强度不仅取决于顶点法向量，而且取决于顶点到光源的方向以及顶点到视点的方向。

对于无穷远视点，视点到任何顶点的方向都是相同的。

而对于局部视点，视点到每个顶点的方向是不同的。

所以需要计算视点到每个顶点的方向，因而性能降低了，但效果质量提高了。

缺省时，使用的是无穷远视点。

glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER , GL_TRUE );//将视点设置为局部视点

glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER , GL_FALSE );//将视点设置为无穷远视点

2.3双面光照

光照计算是对所有多边形(包括其正面和背面)进行的。

而一般情况下，只设置正面光照条件，而忽略背面。

但有时候需要看到物体内侧的光照效果，这就需要用到双面光照。

glLightModeli( LIGHT_MODEL_TWO_SIDE , GL_TRUE ); //启用双面光照

glLightModeli( LIGHT_MODEL_TWO_SIDE , GL_FALSE ); //禁用双面光照

2.3.1双面光照-示例

绘制一个没有底面的圆锥，这样就可以看见圆锥内部。

程序截图

未启用双面光照，看不见圆锥内部，看到圆锥的底面为绿色，这是由环境光造成的。

启用双面光照后，就可以看见圆锥内部了。

3光源衰减

真实的光，离光源越远则光强越小。

而方向光源是无穷远光源，因此距离对光强没有影响，所以方向光没有衰减，但位置光则有衰减。

OpenGL是通过光源光强乘以衰减系数来计算衰减光照的。

衰减系数= 1 / ( Kc + Kl*d + Kq*d*d )

d =光源位置到物体顶点的距离

Kc = GL_CONSTANT_ATTENUATION常数衰减因子

Kl = GL_LINER_ATTENUATION线性衰减因子

Kq = GL_QUADRATIC_ATTENUATION二次衰减因子

缺省时，衰减因子为( 1 , 0 , 0 )即不进行衰减

glLightf( GL_LIGHT0 , GL_CONSTANT_ATTENUATION , 1.0 );

glLightf( GL_LIGHT0 , GL_LINEAR_ATTENUATION , 0.0 );

glLightf( GL_LIGHT0 , GL_QUADRATIC_ATTENUATION , 0.0 );

环境光、漫反射光和镜面光的强度都衰减，

辐射光和全局环境光的强度不衰减

4聚光

位置光源可以定义成聚光形式，即将光的形状限制在一个圆锥内。

用来模拟现实世界中的聚光灯。聚光的具体使用步骤如下：

4.1设置聚光源位置

GLfloat light_position[]={ 1.0 , 1.0 , 1.0 , 1.0 };

glLightfv( GL_LIGHT0 , LIGHT_POSITION , light_position );

4.2设置聚光最大散布角

聚光最大散布角就是光锥的轴与中心线的夹角，也就是光锥顶角的一半。

聚光最大散布角可以选择在[ 0.0 , 90.0 ]之间的值，以及一个特定值：180.0

缺省时最大散布角为180.0，即顶角为360度，光源照向所有方向。

glLightf( GL_LIGHT0,GL_SPOT_CUTOFF , 45.0 );

4.3设置聚光方向

聚光方向决定光锥的轴，缺省值为( 0.0 , 0.0 ,-1.0 ) ,即指向负Z轴。

GLfloat spot_direction[]={ -1.0 , -1.0 , 0.0 };

glLightfv( GL_LIGHT0 , GL_SPOT_DIRECTION , spot_direction);

4.4设置聚光指数

聚光指数控制光的集中程度，光锥中心的光强最大，越靠边的光强越小。

缺省时为0，即均匀照射。

glLightf( GL_LIGHT0 , GL_SPOT_EXPONENT , 2.0 );

5辐射光

通过GL_EMISSION定义一个RGBA值，使物体看起来象发出这种颜色的光一样。

而现实中，除光源外，其他物体都不发光；可以利用这一特性来模拟光源。

0GLfloat mat_emission[]={ 0.1 , 0.2 , 0.3, 1.0 };

glMaterialfv( GL_FRONT , GL_EMISSION , mat_emission );

6控制光源位置

光源的位置也可以通过变换矩阵，调整光源函数和视点变换函数来获得三种不同的效果：

1.光源位置保持固定

2.光源绕物体移动

3.光源随视点移动

6.1物体旋转光源平移-示例

