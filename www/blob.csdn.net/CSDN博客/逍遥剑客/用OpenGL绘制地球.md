# 用OpenGL绘制地球 - 逍遥剑客 - CSDN博客
2006年12月08日 00:50:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：9673
本想用自动纹理坐标生成做的，可是红宝书上说得一点都不明白。网上有个人的课程设计是自己画的球，就是一个个四边形的拼个球，然后再给四边形贴图。这样太麻烦了!逛啊逛，让我找到了另一种方法，仅仅几行代码：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// 画地球
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void OpenGL::DrawEarth(void)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    GLUquadricObj *quadObj = gluNewQuadric();//创建一个二次曲面物体
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    gluQuadricTexture(quadObj,GL_TRUE);        //启用该二次曲面的纹理
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    glBindTexture(GL_TEXTURE_2D, texture[filter]);//绑定纹理
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    gluSphere(quadObj,1,20,20);                    //绘制一个球体
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    gluDeleteQuadric(quadObj);                    //删除二次曲面对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
至于怎么生成纹理，就不用写了吧？
![](http://home.qlsc.sdu.edu.cn/attachments/Earth_8R2Xw1baJQLe.bmp)![](http://home.qlsc.sdu.edu.cn/attachments/Earth_9RJllwcb3doD.jpg)
 附二次曲面的相关函数：
　　·gluNewQuadric创建一个二次对象。这个函数创建并返回一个指向新的二次对象的指针。当调用二次描述和控制函数是指向这个对象。如果返回值为0则表明没有足够的空间分配给该对象。
　　·gluQuadricDrawStyle函数指定二次对象的绘制方式。本例中圆柱体的绘制方式为GLU_FILL。含义为用多边形原绘制本二次对象，多边形的绘制方式为逆时针。球体的绘制方式为GL_SILHOUETTE,即除边界外用一系列线来绘制二次对象。圆盘的绘制方式为GL_LINE,即用一系列线来绘制二次对象。部分圆盘的绘制方式为GL_POINT,即用一系列点来绘制二次对象。
　　·gluQuadricNormals，指定二次对象使用的法向量类型。
　　·gluQuadricOrientation，指定二次对象的内面或外面方位。GLU_OUTSIDE为缺省值，表明使用指向内面的法相量绘出二次对象,GLU_INSIDE表明使用指向外面的法相量绘出二次对象。
　　·gluQuadricTexture指定二次对象是否使用纹理。GL_FALSE为缺省值。
　　·void gluCylinder（GLUquadricObj *qobj，GLdouble baseRadius，GLdouble topRadius，GLdouble height，Glint slices，Glint stacks）绘制一个圆柱体。
　　qobj指明是哪个二次对象。
　　baseRadius圆柱体在z=0时的半径。
　　topRadius圆柱体在z=height时的半径。
　　height圆柱体的高。
　　slices围绕着z轴分片的个数。
　　stacks顺着z轴分片的个数。stacks和slices垂直。
　　·void gluSphere（GLUquadricObj *qobj，GLdouble radius，Glint slices，Glint stacks）绘制一个球形。
　　qobj指明是哪个二次对象。
　　radius球体半径。 
　　slices围绕着z轴分片的个数。
　　stacks顺着z轴分片的个数。
　　·void gluDisk（GLUquadricObj *qobj，GLdouble innerRadius，GLdouble outerRadius，Glint slices，Glint loops）绘制一个圆盘。
　　qobj指明是哪个二次对象。
　　innerRadius圆盘的内部半径，可能为0。
　　outerRadius圆盘的外部半径。
　　slices围绕着z轴分片的个数。
　　loops圆盘同心圆个数。
　　·void gluPartialDisk（GLUquadricObj *qobj，GLdouble innerRadius，GLdouble outerRadius，Glint slices，Glint loops，GLdouble startAngle，GLdouble sweepAngle）绘制一个圆盘的一部分。 
　　startAngle起始角度，单位为度。
　　sweepAngle扫描角，单位为度。 
