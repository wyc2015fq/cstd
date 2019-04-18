# [译]OpenGL投影矩阵 - tkokof1的专栏 - CSDN博客

2019年04月09日 16:07:36[tkokof1](https://me.csdn.net/tkokof1)阅读数：22标签：[OpenGL																[投影																[矩阵																[投影矩阵](https://so.csdn.net/so/search/s.do?q=投影矩阵&t=blog)](https://so.csdn.net/so/search/s.do?q=矩阵&t=blog)](https://so.csdn.net/so/search/s.do?q=投影&t=blog)](https://so.csdn.net/so/search/s.do?q=OpenGL&t=blog)
个人分类：[游戏																[算法																[数学](https://blog.csdn.net/tkokof1/article/category/8122393)](https://blog.csdn.net/tkokof1/article/category/642709)](https://blog.csdn.net/tkokof1/article/category/642710)


> 
这是关于OpenGL投影矩阵的一篇译文,原文在[这里](http://www.songho.ca/opengl/gl_projectionmatrix.html).

#### 概览(Overview)

电脑显示屏是一个2D平面,为了能够在这个2D平面上显示OpenGL渲染的3D场景,我们必须将3D场景当作2D图像投影到这个2D平面(计算机屏幕)上.GL_PROJECTION 矩阵就是用来做这种投影变换的.首先,该矩阵将所有观察空间的顶点坐标变换到裁剪空间,接着,将变换后的顶点坐标(即裁剪坐标)的每个分量$(x,y,z,w)$除以坐标的 $w$ 分量,使其变换为标准化设备坐标(NDC).

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040915394542.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

这里我们需要注意的一点就是 : GL_PROJECTION 矩阵同时整合了裁剪(视锥体剔除)和标准化设备坐标(NDC)变换的功能(**译注:这里不是指 GL_PROJECTION 矩阵本身整合了这些功能,而是指 OpenGL 的 GL_PROJECTION 矩阵模式整合了这些功能**).接下来的内容就是描述如何从6个边界参数(left, right, bottom, top, near 和 far) 构造出这个投影矩阵.

值得一提的是,视锥体剔除是在裁剪空间进行的(NDC变换之前) : 裁剪坐标中的 $x_c$, $y_c$ 和 $z_c$ 分量会分别与 $w_c$ 分量进行比较,如果其中任一分量小于 $-w_c$,或者大于 $w_c$,则该坐标对应的顶点就会被丢弃(即发生了裁剪).

接着, 如果发生了裁剪, OpenGL 会重新构建发生裁剪的多边形边缘.

#### 透视投影

在透视投影中,视锥体(观察空间)中的一个3D坐标点会被映射到一个立方体中(NDC);其中 $x$ 坐标范围会从 $[l, r]$ 映射到 $[-1, 1]$, $y$  坐标范围会从 $[b, t]$ 映射到 $[-1, 1]$, $z$ 坐标范围会从 $[-n, -f]$ 映射到 $[-1, 1]$.
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040915400577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

这里需要注意的是,观察空间是在右手坐标系下(OpenGL 使用右手坐标系)定义的,但是 NDC 却是在左手坐标系下定义的.换句话说就是,观察空间中的摄像机是指向 -Z 轴的,但是在 NDC 中,摄像机指向的却是 +Z 轴(**译注:NDC变换会改变左右手坐标系**).由于 glFrustum() 函数只接受正的近/远裁剪面距离,所以我们需要在构造 GL_PROJECTION 矩阵的过程中将近/远裁剪面距离变成负数(**译注:因为在观察空间中,摄像机是指向 -Z 轴的**).

在 OpenGL 中,观察空间中3D坐标点是投影到近裁剪面(即投影面)上的.下面的示意图展示了一个在观察空间中的坐标点 $(x_e, y_e, z_e)$,是如何投影到近裁剪面坐标点 $(x_p, y_p, z_p)$ 上的.

![视锥体顶部视图](https://img-blog.csdnimg.cn/20190409154035572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)
![视锥体侧面视图](https://img-blog.csdnimg.cn/20190409154055700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

从视锥体的顶部视图可以看到, $x_e$(观察空间中的 $x$ 坐标)的投影坐标 $x_p$ 可以使用相似三角形对应边长成比例来求解:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154259549.png)

从视锥体的侧面视图来看,$y_p$ 也可以使用类似的方式求解:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154333288.png)

注意到 $x_p$ 和 $y_p$ 的数值都是依赖于 $z_e$ 的,并且两者的数值大小都反比与 $-z_e$(这两个数值的求解都除以了 $-z_e$).这是我们构建 GL_PROJECTION 矩阵的第一条线索.在观察空间中的坐标经过 GL_PROJECTION 矩阵变换之后,得到的裁剪坐标还是一个[齐次坐标](http://www.songho.ca/math/homogeneous/homogeneous.html),需要将坐标的各个分量除以坐标的 $w$ 分量才能将其变换为标准化设备坐标(NDC).(更多细节可以看[这里](http://www.songho.ca/opengl/gl_transform.html))

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154404390.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154421107.png)

所以,我们可以将裁减坐标的 $w$ 分量设置为 $-z_e$,基于此,GL_PROJECTION 矩阵的第四行便可以确定了,应为 $(0, 0, -1, 0)$.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154440113.png)

接下来,我们要将 $x_p$ 和 $y_p$ 线性映射到 NDC 下的 $x_n$ 和 $y_n$, 即 $[l, r]$ ⇒ $[-1, 1]$ , $[b, t]$ ⇒ $[-1, 1]$.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154527184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154606266.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

然后,我们将 $x_p$ 和 $y_p$ 的表达式代入上面的等式.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154622241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040915463245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

由于要进行透视除法的关系$(x_c/w_c, y_c/w_c)$,我们将等式都调整成了除以 $-z_e$ 的形式.我们先前已经设置了 wc 为 $-z_e$,所以等式括号里项即是裁剪坐标 $x_c$ 和 $y_c$.

通过这些等式,我们就可以确定 GL_PROJECTION 矩阵的第一行和第二行了:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154649324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

现在,我们只需要求解出 GL_PROJECTION 矩阵的第三行便可以了,不过计算 $z_n$ 和之前计算的 $x_n$ 和 $y_n$ 有些不同,因为观察空间中的 $z_e$ 总是会被投影到近裁剪面上(数值为$-n$),而我们需要的是唯一的 $z$ 值以进行裁剪和深度检测,另外的,我们也应该能够"反投影"(unproject,投影的逆变换)$z_n$.由于我们知道 $z$ 坐标并不依赖与 $x$ 坐标和 $y$ 坐标,所以我们可以借助 $w$ 分量来求解 $z_n$ 和 $z_e$ 的关系,计算方法如下:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154708322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

观察空间中, $w_e$ 等于 $1$,所以上面的等式可以化简为:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154717429.png)

为了计算 $A$ 和 $B$ 这两个参数,我们可以利用 $(z_e, z_n)$ 的两个条件关系:$(-n, -1)$ 和 $(-f, 1)$(**译注:即 $z_e = -n$ 时, $z_n = -1$; $z_e = -f$ 时, $z_n = 1$**),代入上面的等式,我们有:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154732649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154743832.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040915475498.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154802207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

通过上面的计算,我们得到了系数 $A$ 和 $B$ 的表达式,于是 $z_e$ 与 $z_n$ 的关系式变为:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154817768.png)

最终,我们得到了完整的 GL_PROJECTION 矩阵:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154839693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

上面的投影矩阵对应于一般的视锥体投影,如果视锥体是上下左右对称的话(即 $r = -l, t = -b$),则上面的投影矩阵可以做如下简化:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154848408.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

在我们继续讲解之前,我们再来观察一下 $z_e$ 和 $z_n$ 的关系,也就是上面的等式$(3)$.注意到该等式是个非线性的有理函数,当 $z_e$ 靠近近裁剪面的时候,对应 $z_n$ 的精度会比较高,当 $z_e$ 靠近远裁剪面的时候,对应 $z_n$ 的精度则比较低.于是,当$[-n, -f]$的范围变大的时候,就会发生深度缓冲的精度问题(z-fighting),因为此时靠近远裁剪面的 $z_e$ 的微小变化并不会影响 $z_n$ 的数值(**译注:数学角度讲,$z_e$ 的任何变化其实都会影响到 $z_n$ 的数值,这里说不会影响 $z_n$ 的数值是从计算机中数值精度表示有限的角度来讲的**),所以我们应该尽量缩短 $n$ 和 $f$ 之间的距离,以最小化上述的深度缓冲精度问题.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154859259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

#### 正交投影

为正交投影构建一个 GL_PROJECTION 矩阵比上面说的透视投影要简单多了.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154910250.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

所有观察空间的 $x_e$, $y_e$ 和 $z_e$ 分量都被线性的映射到 NDC 中,我们要做的就是将长方体(观察空间)缩放成一个立方体(NDC),然后将其移动到原点位置.我们马上来算一下 GL_PROJECTION 矩阵的各个元素:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154921427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154931749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154941465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

由于在正交投影中,我们不需要 $w$ 分量的参与,所以 GL_PROJECTION 矩阵的第四行设置为了 $(0, 0, 0, 1)$.最终的 GL_PROJECTION 矩阵表示如下:

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409154950216.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

同透视投影一样,如果视锥体是上下左右对称的话(即 $r = -l, t = -b$),上面的 GL_PROJECTION 矩阵可以简化为:

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040915495836.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)


