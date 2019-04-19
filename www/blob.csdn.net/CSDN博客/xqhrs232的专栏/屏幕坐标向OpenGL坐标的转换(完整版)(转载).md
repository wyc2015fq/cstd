# 屏幕坐标向OpenGL坐标的转换(完整版)(转载) - xqhrs232的专栏 - CSDN博客
2010年01月28日 16:11:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1843
原文地址：[http://chiefman.blog.hexun.com/5400795_d.html](http://chiefman.blog.hexun.com/5400795_d.html)
很多人用OpenGL绘图会遇到一个问题即屏幕坐标向OpenGL坐标转换，在网上流传着如下类似的代码：
 GLint    viewport[4]; 
 GLdouble modelview[16]; 
 GLdouble projection[16]; 
 GLfloat  winX, winY, winZ; 
 GLdouble posX, posY, posZ; 
 glGetIntegerv(GL_VIEWPORT, viewport); 
 glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
 glGetDoublev(GL_PROJECTION_MATRIX, projection); 
 winX = (float)x; 
 winY = viewport[3] - (float)y;
 glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
 gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    注：(x, y)是屏幕坐标，(winX, winY, winZ)是视景体坐标及深度坐标，(posX, posY, posZ是OpenGL坐标。
    上述代码并不保险，只针对一种特殊情况才好使，即glViewport(0, 0, screenWidth, screenHeight)，screenWidth、screenHeight分别是客户区的宽和高，视口左下角坐标恰好是（0，0），并且未经过任何模型变换。
    从屏幕坐标向OpenGL坐标要经过两步，第一步是屏幕坐标向视景体坐标转换，第二步是视景体坐标向OpenGL坐标转换。上述代码中winX = (float)x;  winY = viewport[3] - (float)y;反映第一步，gluUnProject是第二步。一般说来，gluUnProject的转换是不会出问题的。
    如何进行正确的转换呢？首先，在glGetIntegerv之前添上模型变换的代码，和绘图时使用的模型变换代码一样，另外必须保证平移，缩放，旋转的顺序和绘图时的一样。其次，屏幕坐标向视景体坐标转换有两种方式（注意！在多视口情况下，活动视应当最后绘制，它将作为当前的视口，保证glGetIntegerv等取值函数能得到正确的值）。①winx = x – viewport[0]; winy = screenHeight – viewport[1] - y; viewport[0] = viewport[1]
 = 0;②winx = x; winy = screenHeight – y;第一种比较直观，前两句是将屏幕坐标转换为视景体内的坐标，后两句将视景体的左下角点坐标改为（0，0），因为在设置裁剪视口时，使用glViewport设置视口的左下角点坐标不一定是（0，0），而在视景体内的点其视景体坐标与左下角点是相对的，即把视景体坐标看作是坐标系原点。第二种方式比较简略，但是同样的道理，只不过是glUnproject函数对winx和winy又做了一次转换。
    好了，现在给出完整的代码，如下：
 GLint    viewport[4]; 
 GLdouble modelview[16]; 
 GLdouble projection[16]; 
 GLfloat  winX, winY, winZ; 
 GLdouble posX, posY, posZ; 
 glPushMatrix();
 // 变换要绘图函数里的顺序一样，否则坐标转换会产生错误
 glScalef(m_srtMatrix[0], m_srtMatrix[1], m_srtMatrix[2]); // 缩放、平移、旋转变换
 glRotatef(m_srtMatrix[3], 1.0f, 0.0f, 0.0f);
 glRotatef(m_srtMatrix[4], 0.0f, 1.0f, 0.0f);
 glRotatef(m_srtMatrix[5], 0.0f, 0.0f, 1.0f);
 glTranslatef(m_srtMatrix[6], m_srtMatrix[7], m_srtMatrix[8]);
 glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
 glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
 glGetDoublev(GL_PROJECTION_MATRIX, projection); 
 glPopMatrix();
 winX = x; 
 winY = screenHeight - y;
 glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
 gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
