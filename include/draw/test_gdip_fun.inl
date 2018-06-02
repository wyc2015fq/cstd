
void gcClear(gc_t* g, const IRECT* pclip, COLOR clr)  {
  IRECT rc = pclip ? *pclip : iRECT2(0, 0, 10000, 10000);
  //gcPushClipRect(g, rc);
  gcSolidBrush(g, clr);
  gcFillRect2(g, rc);
  //gcPopClipRect(g);
}

#if 0
void menuItem2(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, &c->rc, ColorBlack);
  Bitmap bitmap(L"demo.bmp");
  int iWidth = bitmap.GetWidth();
  int iHeight = bitmap.GetHeight();

  //初始化色彩变换矩阵
  ColorMatrix colorMatrix=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  ImageAttributes imageAtt;

  //从0到1进行修改色彩变换矩阵主对角线上的数值
  //使三种基准色的饱和度渐增
  for(float i=0.0f;i<=1.0f;i+=0.02f)
  {  
    colorMatrix.m[0][0]=i;
    colorMatrix.m[1][1]=i;
    colorMatrix.m[2][2]=i;
    colorMatrix.m[3][3]=i;
    //设置色彩校正矩阵
    imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    //绘制图片
    gcRectImage(g, &bitmap, Rect(0, 0, iWidth, iHeight), 0,0,        iWidth,iHeight,UnitPixel, &imageAtt);
  }

  MyDrawString(g, "下面演示淡出效果...", Font(L"Arial",12), SolidBrush(ColorWhite), Point(20,iHeight+20));

  //从1到0进行修改色彩变换矩阵主对角线上的数值
  //依次减少每种色彩分量
  for(i=1.0f;i>=0.0f;i-=0.02f)
  {  
    colorMatrix.m[0][0]=i;
    colorMatrix.m[1][1]=i;
    colorMatrix.m[2][2]=i;
    colorMatrix.m[3][3]=i;
    //设置色彩校正矩阵
    imageAtt.SetColorMatrix(&colorMatrix,
      ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
    //绘制图片
    g.DrawImage(
      &bitmap, Rect(0, 0, iWidth, iHeight), 
      0,0,
      iWidth,iHeight,
      UnitPixel, 
      &imageAtt);
  }

  gcSolidBrush(g, ColorWhite);
  MyDrawString(g, "完毕", Font(L"Arial",12), Point(20,iHeight+40));
}

//
void GrayScale()
{
  Graphics &g=GetGraphics();
  g.Clear(ColorWhite);
  Bitmap image(L"head.bmp");      
  int Width =image.GetWidth()-1;
  int Height =image.GetHeight()-1;

  //绘制原图
  g.DrawImage(&image,0, 0);
  g.TranslateTransform((REAL)image.GetWidth(),0.f);

  /*image2、image3分别用来保存最大值法
  和加权平均法处理的灰度图像*/
  Bitmap *image2=image.Clone(Rect(0,0,image.GetWidth(), image.GetHeight()),PixelFormatDontCare);
  Bitmap *image3=image.Clone(Rect(0,0,image.GetWidth(), image.GetHeight()),PixelFormatDontCare);

  Color color;
  //使用平均值进行灰度处理
  for(int i=Width; i>=0;i--)
    for( int j=Height; j>=0;j--)
    {
      image.GetPixel(i,j, &color);  
      //求出平均三个色彩分量的平均值
      int middle=(color.GetR()+color.GetG()+color.GetB())/3;
      Color colorResult(255,middle,middle,middle);
      image.SetPixel(i,j, colorResult);
    }  
  //重新绘制灰度化图
  g.DrawImage(&image, Rect(0, 0, Width, Height));

  //在新位置显示最大值法进行灰度处理的结果
  g.TranslateTransform((REAL)image.GetWidth(),0.f);
  //使用最大值法进行灰度处理
  for(i=Width; i>=0;i--)
  {
    for(int j=Height; j>=0;j--)
    {
      image2->GetPixel(i,j, &color);
      int tmp=color.GetR()>color.GetG()? color.GetR():color.GetG();
      int maxcolor=tmp>color.GetB()? tmp:color.GetB();
      Color colorResult(255,maxcolor,maxcolor,maxcolor);
      //设置处理后的灰度信息
      image2->SetPixel(i, j, colorResult);
    }
  }

  //重新绘制灰度化图
  g.DrawImage(image2, Rect(0, 0, Width, Height));
  //在第二行绘制图片
  g.ResetTransform();
  g.TranslateTransform(0.f, (REAL)image.GetHeight());

  //使用加权平均法进行灰度处理  
  for(i=Width; i>=0;i--)
  {
    for(int j=Height; j>=0;j--)
    {
      image3->GetPixel(i, j, &color);  
      int R=(int)(0.3f*color.GetR());
      int G=(int)(0.59f*color.GetG());
      int B=(int)(0.11f*color.GetB());

      Color colorResult(255,R,G,B);
      //设置处理后的灰度信息
      image3->SetPixel(i, j, colorResult);
    }  
  }
  //重新绘制灰度化图
  g.DrawImage(image3, Rect(0, 0, Width, Height));
  
  g.TranslateTransform((REAL)image.GetWidth(),0.f);
  //灰度的还原演示，还原使用最大值法处理的灰度图像image2
  for(i=Width; i>0;i--)
  {
    for(int j=Height; j>0;j--)
    {
      image2->GetPixel(i,j, &color);
      int R=color.GetR();
      int G=color.GetG();
      int B=color.GetB();
      //分别对RGB三种色彩分量进行伪彩色还原

      //进行红色分量的还原
      if(R<127)
        R=0;
      if(R>=192)
        R=255;
      if(R<=191&&R>=128)
        R=4*R-510;

      /*进行绿色分量的还原,为了还原后的绿色分量再次参加比较，
      这里设置一个变量YES表示G是否已经参加了比较*/

      bool yes;
      yes=false;
      if(G<=191&&G>=128&&(!yes))
      {
        G=255;
        yes=true;
      }
      if(G>=192&&(!yes))
      {
        G=1022-4*G;
        yes=true;
      }
      if(G<=63&&(!yes))
      {
        G=254-4*G;
        yes=true;
      }
      if(G<=127&&G>=67&&(!yes))
        G=4*G-257;
      
      //进行蓝色分量的还原
      if(B<=63)
        B=255;
      if(B>=128)
        B=0;
      if(B>=67&&B<=127)
        B=510-4*B;
      
      //还原后的伪彩色
      Color colorResult(255,R,G,B);
      //将还原后的RGB信息重新写入位图
      image2->SetPixel(i, j, colorResult);
          
    }  
  }
  //重新绘制还原后的伪彩色位图
  //重新绘制灰度化图
  g.DrawImage(image2, Rect(0, 0, Width, Height));

  delete image2;
  delete image3;
}

//
void Inverse()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);
  
  Bitmap image(L"head.bmp");        
  int Width =image.GetWidth();
  int Height =image.GetHeight();

  Color colorTemp,color2;
  Color color;
  //绘制原图
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  
  for(int  i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      //将色彩进行反转，获得底片效果
      int r=255-color.GetRed();
      int g=255-color.GetGreen();
      int b=255-color.GetBlue();
      Color colorResult(255,r,g,b);
      //将还原后的RGB信息重新写入位图
      image.SetPixel(i, j, colorResult);
    }
    //动态绘制底片滤镜效果图
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }
  //将已经实现了底片效果的位图再反色(恢复到原图)
  for(i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      int r=255-color.GetRed();
      int g=255-color.GetGreen();
      int b=255-color.GetBlue();
      Color colorResult(255,r,g,b);
      //将还原后的RGB信息重新写入位图
      image.SetPixel(i, j, colorResult);
    }
    //绘制经过两次反色的位图
    gcDrawImage(&image, Rect(Width*2, 0, Width, Height));
  }
}

//
void Emboss()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"head.bmp");        
  int Width =image.GetWidth();
  int Height =image.GetHeight();

  //image2:进行雕刻处理
  Bitmap *image2=image.Clone(Rect(0,0,Width,Height), PixelFormatDontCare);

  //绘制原图
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  Color color, colorTemp,colorLeft;

  //进行图片的浮雕处理
  //依次访问每个像素的RGB值
  for(int i=Width-1; i>0;i--)
  {
    for( int j=Height-1; j>0;j--)
    {
      //获取相邻两个像素的R、G、B值
      image.GetPixel(i, j, &color);
      image.GetPixel(i-1, j-1, &colorLeft);
      //计算与左上角像素的RGB分量之差
      //67：控制图片的最低灰度，128：常量，更改这两个值会得到不同的效果
      int r=max(67,min(255,
        abs(color.GetRed()-colorLeft.GetRed()+128)));
      int g=max(67,min(255,
        abs(color.GetGreen()-colorLeft.GetGreen()+128)));
      int b=max(67,min(255,
        abs(color.GetBlue()-colorLeft.GetBlue()+128)));
      Color colorResult(255,r,g,b);
      //将计算后的RGB值回写到位图
      image.SetPixel(i, j,colorResult);
    }

    //绘制浮雕图
    gcDrawImage(&image, Rect(Width+10, 0, Width, Height));
  }

  //进行图片的雕刻处理
  for(i=0; i<Height-1;i++)
  {
    for( int j=0; j<Width-1;j++)
    {
      image2->GetPixel(j, i, &color);  
      image2->GetPixel(j+1, i+1, &colorLeft);  
      //计算与右下角像素的分量之差
      //67：控制图片的最低灰度，128：常量，更改这两个值会得到不同的效果
      int r=max(67,min(255,abs(color.GetRed()-colorLeft.GetRed()+128)));
      int g=max(67,min(255,abs(color.GetGreen()-colorLeft.GetGreen()+128)));
      int b=max(67,min(255,abs(color.GetBlue()-colorLeft.GetBlue()+128)));
      Color colorResult(255,r,g,b);
      image2->SetPixel(j, i,colorResult);
    }

    //绘制雕刻图
    gcDrawImage(image2, Rect(Width*2+20, 0, Width, image.GetHeight()));
  }
  delete image2;
}
#endif


void CreatePenFromBrush(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
#if 0
  //构造线性渐变画刷
  gcGradientBrushRectS(g, 0,0,10,10, ColorBlue, ColorRed, LinearGradientModeBackwardDiagonal);
  //从线性渐变画刷中构造画笔
  gcPen(&LineargradientBrush);
  pen.SetWidth(10);
  //绘制矩形
  g.DrawRectangle(&pen,10,10,100,100);

  //装入纹理图片
  Bitmap image(L"butterfly.bmp");
  //构造纹理画刷
  TextureBrush  tBrush(&image);
  //将画刷传入画笔的构造函数
  Pen texturedPen(&tBrush,42);

  //设置贝塞尔曲线的起止点及控制点
  Point p1(10, 100);   
  Point c1= Point(100, 10);   
  Point c2= Point(150, 150);  
  Point p2= Point(200, 100);  
  g.TranslateTransform(130,0);
  //绘制贝塞尔曲线
  g.DrawBezier(&texturedPen, p1, c1, c2, p2);
#endif
}


//画笔的线形演示
void DashStyle_Custom(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
  gcPen(g, ColorBlue, 5);
#if 0
  //设置文本输出对齐方式及字体
  StringFormat fmt;
  gcSetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //字体
  Font font(L"Arial",12);
  SolidBrush sBrush(ColorBlack);
  gcTranslateTransform(0,30);
#endif
  //分别使用常见的五种线型绘制直线
  for(i=0;i<5;i++) {
    //设置线型
    gcDashStyle(g, (DashStyle)i);
    gcDrawLine(g, 10, 30*i, 260, 30*i);
    //输出当前线型的名称
    //gcSolidFormatText(g, io->font, sBrush, PointF(260.f,30.f*i), fmt, _T("%d"), i+1);
  }

  //使用自定义义线型
  {
    float dashVals[]=
    {
      5.0f,   // 线长5个像素
        2.0f,   // 间断2个像素
        15.0f,  // 线长15个像素
        4.0f    // 间断4个像素
    };
    gcLineDash(g, 0, dashVals, 4);
    gcPenColor(g, ColorRed);
    gcDrawLine(g, 10, 30*i, 260, 30*i);
    //CString s; s.Format(_T("%d"), i+1);
    //MyDrawString(graphics, s, font, sBrush, PointF(260.f,30.f*i), fmt);
  }
}

void Pen_Align(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
  gcPen(g, _ARGB(155,255,0,0),10);
  for(i=0; i<5;i++) {
    gcPenAlignment(g, (PenAlignment)i);
    gcDrawLine(g, 0 + i*70,10, 60 + i*70,10);
  }
  //gcResetTransform();
  gcPen(g, ColorGray, 1.0f);
  gcDrawLine(g,0,10,600,10);
}
void Pen_Tranform(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
  //构造一支宽度为5的红色画笔
  gcPen(g, ColorRed, 3.0f);
  //将画笔从垂直方向扩充6倍，水平方向保持不变
  //ScaleTransform(1, 6);
  //使用未经旋转处理的画笔画圆
  gcDrawEllipse(g, 0, 50, 80, 80);
  //60°旋转
  // TranslateTransform(100,0);
  //pen.RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 100, 50, 80, 80);
  //120°旋转
  //gcTranslateTransform(100,0);
  // RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 0, 100+50, 80, 80);
  //180°旋转
  //gcTranslateTransform(g, 100,0);
  //RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 100+0, 100+50, 80, 80);
}

//线帽演示
void Pen_LineCap(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
#if 0
  //设置文本输出对齐方式及字体
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //字体
  Font font(L"Arial",12);
  SolidBrush sBrush(ColorBlack);
#endif

  //创建宽度为15的画笔
  gcPen(g, ColorBlack,15);

  //分别使用不同的线帽
  {
    LineCap lincap[]=
    {
      LineCapFlat,
        LineCapSquare,
        LineCapRound,
        LineCapTriangle,
        LineCapNoAnchor,
        LineCapSquareAnchor,
        LineCapRoundAnchor,
        LineCapDiamondAnchor,
        LineCapArrowAnchor,
        LineCapCustom,
    };
    
    for (i=0; i<countof(lincap); i++)
    {
      int y = i*30;
      gcLineCap(g, lincap[i], lincap[i]);//起点 终点
      gcDrawLine(g,50,10+y,300,10+y);
      //输出当前线帽类型（LineCap枚举成员名）
      //CString s; s.Format(_T("%d"), lincap[i]);
      //MyDrawString(graphics, s, font, sBrush, PointF(320.f,10.f), fmt);
      //平移绘图平面
      //gcTranslateTransform(0,30);
    }
  }
}

//画笔的透明度支持
void Pen_TransColor(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
  
  //分别构造蓝色、红色画笔
  //绘制网线
  {
    int x, y = 256;
    for (x = 0; x < 256; x += 5)
    {
      gcPenColor(g, ColorBlue);
      gcDrawLine(g, 0, y, x, 0);
      gcPenColor(g, ColorRed);
      gcDrawLine(g, 256, x, y, 256);  
      y -= 5;
      //延时以查看动态效果
      //sys_sleep(20);
    }    
    //使用绿色画笔绘制不同透明度的线条
    //透明度由上到下依次递减
    for (y = 0; y < 256; y++)
    {
      gcPenColor(g, _A_RGB(y, ColorGreen));
      gcDrawLine(g, 0, y, 256, y);
      //延时以便查看动态效果
      //sys_sleep(20);
    }
    
    //使用绿色画笔绘制不同透明度的线条
    //透明度由左到右依次递减
    for (x = 0; x < 256; x++)
    {
      gcPenColor(g, _A_RGB(x, ColorBlue));
      gcDrawLine(g, x, 100, x, 200);
      //延时以查看动态效果
      //sys_sleep(20);
    }
  }
}

#if 0
//简单的单色画刷示意
void Brush_SolidBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //创建绿色画刷
  SolidBrush greenBrush(ColorGreen);
  //定义曲线、多边形端点坐标
  PointF point1(100.0f, 100.0f);
  PointF point2(200.0f, 50.0f);
  PointF point3(250.0f, 200.0f);
  PointF point4(50.0f, 150.0f);
  PointF point5(100.0f, 100.0f);
  PointF points[]={point1, point2, point3, point4};
  //填充闭合曲线
  gcFillClosedCurve(&greenBrush, points, 4, FillModeAlternate, 1.0f);

  //构造多边形(闭合)
  PointF poly[]={point1, point2, point3, point4,point5};
  //在另一个位置填充多边形
  gcTranslateTransform(300,0);
  gcFillPolygon(&greenBrush, poly, 5, FillModeAlternate);
}
//填充正叶曲线
void Brush_FillVurve()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //创建绿色画刷
  SolidBrush greenBrush(ColorGreen);
  int cx,cy;
  //以当前窗口的中心点绘制正叶曲线
  cx=MyClient.Width/2;
  cy=MyClient.Height/2;
  //设置"叶"长
  int LeafLength=80;
  //设置叶片数量=2* LeafNum
  int LeafNum=5;
  double x,y,x2,y2,r;

  //创建一个图形路径对象，用以容纳正叶曲线的边界线
  GraphicsPath tmpPath(FillModeAlternate);
  //生成曲线边界数据：角度变化为一周PI*2
  for(float i=0.0f;i<PI*2+0.1f;i+=(float)PI/180)
  {
    r=(int)abs(LeafLength*cos(double(LeafNum*i)));
    x=r*cos(i);
    y=r*sin(i);
    x2=cx+x;
    y2=cy+y;
    /*将曲线的边界信息存入临时路径,如果想要查看这些信息所构成的区域
    ，可以在此加入gcDrawLine(&pen,x2,y2,x2-1,y2-1);*/
    tmpPath.AddLine((int)x2,(int)y2,(int)x2,(int)y2);
  }

  //填充路径
  gcFillPath(&greenBrush, &tmpPath);

  //绘制中心坐标轴
  Pen pen(ColorGray);
  gcDrawLine(&pen,0,cy,cx*2,cy);
  gcDrawLine(&pen,cx,0,cx,cy*2);
}
//影线画刷示意
void Brush_HatchBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //构造影线画刷的前后色彩
  Color black=ColorBlack;
  Color white=ColorWhite;

  //使用第一种风格的影线画刷
  HatchBrush brush(HatchStyleHorizontal, black, white);
  gcFillRectangle(&brush, 20, 20, 100, 50);

  //使用第二种风格的影线画刷
  HatchBrush brush1(HatchStyleVertical, black, white);
  gcFillRectangle(&brush1, 120, 20, 100, 50);

  //使用第三种风格的影线画刷
  HatchBrush brush2(HatchStyleForwardDiagonal, black, white);
  gcFillRectangle(&brush2, 220, 20, 100, 50);

  //使用第四种风格的影线画刷
  HatchBrush brush3(HatchStyleBackwardDiagonal, black, white);
  gcFillRectangle(&brush3, 320, 20, 100, 50);

  //使用第五种风格的影线画刷
  HatchBrush brush4(HatchStyleCross, black, white);
  gcFillRectangle(&brush4, 420, 20, 100, 50);

  //使用第六种风格的影线画刷
  HatchBrush brush5(HatchStyleDiagonalCross, black, white);
  gcFillRectangle(&brush5, 520, 20, 100, 50);
}
//列举出所有风格的影线画刷
void Brush_EnumAllStyle()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //设定画刷的前景色为黑色，背景色为白色
  Color black=ColorBlack;
  Color white=ColorWhite;

  //预定义填充区域的宽度及高度
  int WIDTH=140;
  int HEIGHT=40;

  //设定输出文本所需信息
  SolidBrush redBrush(ColorRed);
  Font myFont(L"Arial", 10);

  //column_count表明在每一行能够绘制矩形的总数
  int column_count=MyClient.Width/WIDTH;
  int rol=0;
  int column=0;

  //在当前窗口使用所有的影线画刷种风格填充矩形
  Pen pen(ColorBlue, 1);
  for (int i=HatchStyleHorizontal; i<HatchStyleTotal; i++)
  {
    HatchStyle style=(HatchStyle)i;
    //如果一行已经绘制完毕，换行
    if (rol>column_count-1)
    {
      column+=2;
      rol=0;
    }
    //创建临时画刷
    HatchBrush brush_tmp(style, black, white);
    //填充矩形：设置宽度为WIDTH-20的目的是让矩形之间留出间隔
    gcFillRectangle(&brush_tmp, rol*WIDTH, column*HEIGHT, WIDTH-20, HEIGHT);
    //绘制矩形边框
    gcDrawRectangle(&pen, rol*WIDTH, column*HEIGHT, WIDTH-20, HEIGHT);

    //显示每种画刷风格的枚举名称
    //计算文本输出区域
    RectF layoutRect(rol*WIDTH, (column+1)*HEIGHT, WIDTH, HEIGHT);
    StringFormat format;
    //设置文本输出格式：水平、垂直居中
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);
    //在矩形框中央输出枚举值
    CString s; s.Format(_T("%d"), style);
    MyDrawString(graphics, s, myFont, redBrush, layoutRect, format);
    rol+=1;
  }
}
//设置绘制原点
void Brush_SetRenderingOrigin()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //设定画刷的前景色为黑色，背景色为白色
  Color black=ColorBlack;
  Color white=ColorWhite;
  HatchBrush hatchBrush(HatchStyleDarkDownwardDiagonal,black,white);

  //在竖直方向填充8个矩形，使用默认的画刷原点
  for(int i=0;i<8;i++)
  {
    gcFillRectangle(&hatchBrush, 0, i*50, 100, 50);  
  }

  //使用不同的绘制原点进行区域填充
  for(i=0;i<8;i++)
  {
    //设置画刷原点(水平方向递增)
    gcSetRenderingOrigin(i, 0);
    gcFillRectangle(&hatchBrush, 100, i*50, 100, 50);
  }
}
//纹理画刷的不同加载方式
void Brush_Texture()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"宋体",12);

  //定义纹理画刷的不同填充区域
  RectF rect1(10, 10, 200,200);
  RectF rect2(210, 10, 200, 200);
  RectF rect3(410, 10, 200, 200);

  //装入纹理图片
  Bitmap image(L"nemo.bmp");
  //构造纹理画刷1：使用默认的方式
  TextureBrush tBrush(&image);
  //使用纹理画刷填充圆形区域
  gcFillEllipse(&tBrush,rect1);
  //绘制圆周
  gcDrawEllipse(&pen,rect1);
  MyDrawString(graphics, "图片原始大小", myFont, brush, PointF(40,220));

  //构造纹理画刷2：只使用给定图片的部分区域
  TextureBrush tBrush2(&image, Rect(55,35,55,35));
  gcFillEllipse(&tBrush2,rect2);
  gcDrawEllipse(&pen,rect2);
  MyDrawString(graphics, "使用部分截图", myFont, brush, PointF(240,220)); 

  //构造纹理画刷3：将使用图片的画刷进行缩放
  TextureBrush tBrush3(&image);
  //对画刷进行50%的缩放
  Matrix mtr(0.5f, 0.0f, 0.0f,0.5f, 0.0f, 0.0f);
  tBrush3.SetTransform(&mtr);
  gcFillEllipse(&tBrush3,rect3);
  gcDrawEllipse(&pen,rect3);
  MyDrawString(graphics, "比例缩小图片",myFont,brush, PointF(440,220));
}

//使用图片排列方式
void Brush_Texture_WrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 13);

  //装入纹理图片
  Bitmap image(L"nemo.bmp");
  //构造纹理画刷
  TextureBrush tBrush(&image);
  //将画刷进行缩放
  Matrix mtr(0.5f, 0.0f, 0.0f,0.5f, 0.0f, 0.0f);
  tBrush.SetTransform(&mtr);

  int i=0;
  //对图片不使用排列方式
  tBrush.SetWrapMode(WrapModeClamp);
  gcFillRectangle(&tBrush, Rect(i*150,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150,0,150,150));
  MyDrawString(graphics, "Clamp", myFont,brush, PointF(0,155)); 

  i+=1;
  //对图片使用平铺排列方式  
  tBrush.SetWrapMode(WrapModeTile);
  gcFillRectangle(&tBrush, Rect(i*150+10,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150+10,0,150,150));
  MyDrawString(graphics, "Tile",myFont,brush, PointF(170,155) ); 

  //对图片使用水平翻转排列方式
  i+=1;
  tBrush.SetWrapMode(WrapModeTileFlipX);
  gcFillRectangle(&tBrush, Rect(i*150+20,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150+20,0,150,150));

  MyDrawString(graphics, "TileFlipX",myFont,brush, PointF(320,155)); 

  //对图片使用垂直翻转排列方式
  tBrush.SetWrapMode(WrapModeTileFlipY);
  gcFillRectangle(&tBrush, Rect(0,180,150,150));
  gcDrawRectangle(&pen, Rect(0,180,150,150));
  MyDrawString(graphics, "TileFlipY",myFont,brush,  PointF(0,335)); 

  //对图片使用水平、垂直同时翻转排列方式
  tBrush.SetWrapMode(WrapModeTileFlipXY);
  gcFillRectangle(&tBrush, Rect(160,180,150,150));
  gcDrawRectangle(&pen, Rect(160,180,150,150));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, PointF(170,335)); 
}
//纹理画刷的变换
void Brush_TextureTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //为三种不同的变换方式的画刷定义填充区域
  RectF rect1= RectF (10, 10, 200,200);
  RectF rect2= RectF (210, 10, 200, 200);
  RectF rect3= RectF (410, 10, 200, 200);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"宋体", 12);

  //装入纹理图片
  Bitmap image(L"nemo.bmp");
  //构造纹理画刷
  TextureBrush tBrush(&image);

  //将画刷旋转30度
  tBrush.RotateTransform(30);                     
  gcFillEllipse(&tBrush,rect1);
  gcDrawEllipse(&pen,rect1);
  MyDrawString(graphics, "旋转30度",myFont,brush, PointF(40,220)); 

  //重置变换矩阵：恢复到变化前的状态
  tBrush.ResetTransform();
  //将画刷在水平方向上扩大三倍
  tBrush.ScaleTransform(3, 1);
  gcFillEllipse(&tBrush, rect2);
  gcDrawEllipse(&pen,rect2);
  MyDrawString(graphics, "横向扩充三倍",myFont,brush,  PointF(240,220)); 

  //平移变换
  tBrush.ResetTransform();
  //将画刷在水平方向上平移30个像素
  tBrush.TranslateTransform(30, 0, MatrixOrderAppend);
  gcFillEllipse(&tBrush, rect3);
  gcDrawEllipse(&pen,rect3);
  MyDrawString(graphics, "横向平移30个像素",myFont,brush, PointF(440,220)); 
}
//查询画刷的变换信息
void Brush_GetTextureMatrix()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Matrix matrix;
  float elements[6];
  RectF rect1(10, 10, 200,200);
  Pen pen(ColorBlack, 2);
  SolidBrush brush(ColorBlack);

  Bitmap image(L"nemo.bmp");
  TextureBrush tBrush(&image);
  //进行三种任意变换
  tBrush.RotateTransform(30);
  tBrush.TranslateTransform(5,3);
  tBrush.ScaleTransform(0.5f,2.0f);

  //获取目前已经进行的画刷变换
  tBrush.SetTransform(&matrix);
  //从矩形到数组
  matrix.GetElements(elements);  
  gcFillEllipse(&tBrush,rect1);
  gcDrawEllipse(&pen,rect1);

  //输出矩阵的元素
  for(int j = 0; j <6; ++j)
  {
    CString s; s.Format(_T("%d %f 矩阵元素值"), j, elements[j]);
    AfxMessageBox(s);
  }
}
//线性渐变画刷程序
void Brush_LinearGradientBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //定义一个由红色到蓝色渐变的画刷:水平变换区域的宽度为40
  //竖直方向不进行色彩渐变
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(40, 0),
    ColorRed,  //起点色彩  
    ColorBlue);  //止点色彩

  //定义一个色彩呈对角线变换的区域，区域大小为40*40
  gcGradientBrush linGrBrush2(
    Point(0, 0),
    Point(40, 40),
    ColorRed,  //起点色彩  
    ColorBlue); //止点色彩  

  //分别演示不同的线性渐变画刷对目标区域的不同填充效果
  gcFillRectangle(&linGrBrush, 0, 0, 200, 200);
  gcFillRectangle(&linGrBrush2, 240, 0, 200, 200);

  Pen pen(ColorGray,1);
  //在对角线方向上绘制单个画刷的填充区域
  for(int i=0;i<5;i++)
  {
    gcDrawRectangle(&pen, 240+i*40,i*40,40,40);
  }
}
//控制线性渐变画刷的填充方式
void Brush_LinearArrange()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 13);

  //定义一个色彩呈对角线变换的区域，区域大小为40*40
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(40, 40),
    ColorRed, //起点色彩  
    ColorBlue); //止点色彩  

  int i=0;
  //对渐变画刷使用平铺排列方式(默认方式)
  linGrBrush.SetWrapMode(WrapModeTile);
  //填充一个大小为160的正方形
  gcFillRectangle(&linGrBrush, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "Tile", myFont, brush, Point(20,165)); 

  //对渐变画刷使用水平翻转排列方式
  i+=1;
  linGrBrush.SetWrapMode(WrapModeTileFlipX);
  //重置绘图平面原点
  gcSetRenderingOrigin(160,0);
  gcFillRectangle(&linGrBrush, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "TileFlipX", myFont, brush, Point(170,165)); 

  //对渐变画刷使用垂直翻转排列方式
  linGrBrush.SetWrapMode(WrapModeTileFlipY);
  //重置绘图平面原点
  gcSetRenderingOrigin(0,200);
  gcFillRectangle(&linGrBrush, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "TileFlipY",myFont,brush, Point(0,375)); 

  //对渐变画刷使用水平、垂直同时翻转排列方式
  linGrBrush.SetWrapMode(WrapModeTileFlipXY);
  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush, Rect(160,200,160,160));
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, Point(170,375)); 

  //标注渐变画刷渐变区域的大小：画格子
  Pen pen2(ColorGray, 1);
  for(i=0;i<8;i++)
    gcDrawLine(&pen2,0,i*40,320,i*40);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i*40,0,i*40,360);
}
//定义线性渐变画刷的渐变模式
void Brush_LinearGradientMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //定义一个水平渐变画刷，大小40*20
  gcGradientBrush linGrBrush1(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeHorizontal);  

  //定义一个垂直渐变画刷
  gcGradientBrush linGrBrush2(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeVertical); 

  //从右上到左下的渐变画刷
  gcGradientBrush linGrBrush3(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeForwardDiagonal);  

  //从左上到左下的渐变画刷
  gcGradientBrush linGrBrush4(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeBackwardDiagonal);  

  int i=0;
  //使用水平渐变的画刷填充区域
  gcFillRectangle(&linGrBrush1, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "水平渐变",myFont,brush, PointF(20,165));

  i+=1;
  //重置绘图平面原点
  gcSetRenderingOrigin(160,0);
  //使用垂直渐变的画刷填充区域
  gcFillRectangle(&linGrBrush2, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "垂直渐变",myFont,brush, PointF(170,165)); 

  //重置绘图平面原点
  gcSetRenderingOrigin(0,200);
  //使用从右上到左下渐变的画刷填充区域
  gcFillRectangle(&linGrBrush3, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "左上->右下",myFont,brush, PointF(0,375)); 

  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush4, Rect(160,200,160,160));
  //使用从左上到右下渐变的画刷填充区域
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "右上->左下",myFont,brush, PointF(170,375)); 

  //在不同的区域中标注定义渐变方向的矩形
  Pen pen2(ColorGray, 1);
  for(i=0;i<18;i++)
    gcDrawLine(&pen2,0,i*20,320,i*20);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i* 40,0,i*40,360);
}
//渐变线偏转角度
void Brush_LinearAngle()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //定义一个渐变线偏转角度为30度的渐变画刷，大小40*20
  gcGradientBrush linGrBrush1(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,30.0f);  
  //定义一个渐变线偏转角度为45度的渐变画刷
  gcGradientBrush linGrBrush2(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,45.0f); 
  //定义一个渐变线偏转角度为90度的渐变画刷
  gcGradientBrush linGrBrush3(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,90.0f);  
  //定义一个渐变线偏转角度为180度的渐变画刷
  gcGradientBrush linGrBrush4(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,180.0f);  

  int i=0;
  //使用偏转角度为30度的渐变画刷填充区域
  gcFillRectangle(&linGrBrush1, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "30度偏转",myFont,brush, PointF(20,165)); 

  i+=1;
  //重置绘图平面原点
  gcSetRenderingOrigin(160,0);
  //使用偏转角度为45度的渐变画刷填充区域
  gcFillRectangle(&linGrBrush2, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "45度偏转",myFont,brush, PointF(170,165));

  //重置绘图平面原点
  gcSetRenderingOrigin(0,200);
  //使用偏转角度为90度的渐变画刷填充区域
  gcFillRectangle(&linGrBrush3, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "90度偏转",myFont,brush, PointF(0,375));

  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush4, Rect(160,200,160,160));
  //使用偏转角度为180度的渐变画刷填充区域
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "180度偏转",myFont,brush, PointF(170,375));

  //在不同的区域中标注定义渐变方向的矩形
  Pen pen2(ColorGray, 1);
  for(i=0;i<18;i++)
    gcDrawLine(&pen2,0,i*20,320,i*20);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i* 40,0,i*40,360);
}

//多色渐变画刷
void Brush_LinearInterpolation()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //定义三种参与渐变的色彩
  Color colors[] = 
  {
    ColorRed,   // 红色
    ColorGreen,//过渡色为绿色
    ColorBlue // 蓝色
  };

  float positions[] = 
  {
    0.0f,   // 由红色起
    0.3f,   // 绿色始于画刷长度的三分之一
    1.0f   // 到蓝色止
  };  

  //构造一条从黑色到白色的渐变画刷
  gcGradientBrush linGrBrush(
    Point(0, 0), 
    Point(180, 0), 
    ColorBlack,ColorWhite);

  //设置渐变画刷的多色渐变信息
  //linGrBrush.InterpolationColors=clrBlend;
  linGrBrush.SetInterpolationColors(colors, positions, 3);
  //使用多色渐变画刷填充目标区域
  gcFillRectangle(&linGrBrush, 0, 0, 180, 100);

  //使用普通的方法实现多色渐变
  //由红到绿，长度60
  gcGradientBrush linGrBrush1(
    Point(0, 0), 
    Point(60, 0),
    ColorRed,         
    ColorGreen);  
  //由绿到蓝，长度120
  gcGradientBrush linGrBrush2(
    Point(60, 0), 
    Point(181, 0),
    ColorGreen,        
    ColorBlue);  
  //分别使用两个画刷填充两个相邻区域，形成多色渐变
  gcFillRectangle(&linGrBrush1, 0, 120, 60, 100);
  gcFillRectangle(&linGrBrush2, 60, 120, 120, 100);
}
//自定义渐变过程：三角形
void Brush_LinearCustomize()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  Font myFont(L"宋体", 12);
  //定义一个双色渐变画刷
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(250, 0),
    ColorRed,   
    ColorBlue);  

  //绘制两条使用默认渐变方式填充的矩形，以用作比较
  gcFillRectangle(&linGrBrush, 0, 0, 250, 15);
  gcFillRectangle(&linGrBrush, 500, 0, 250, 15);

  //依次改变合成点位置
  int row=1;
  for(float i=0.0f;i<1.0f;i+=0.1f)
  {
    linGrBrush.SetBlendTriangularShape(i);  
    gcFillRectangle(&linGrBrush, 0, row*15, 250, 15);
    row++;
  }
  MyDrawString(graphics, "改变合成点位置",
    myFont,brush, PointF(40,200));

  //依次改变彩色合成因子
  row=1;
  for(i=0.0f;i<1.0f;i+=0.1f)
  {
    //将合成的相对位置设置在整个区域的50%处
    linGrBrush.SetBlendTriangularShape (0.5f,i);  
    gcFillRectangle(&linGrBrush, 500, row*15, 250, 15);
    row++;
  }
  MyDrawString(graphics, "改变色彩合成因子",
    myFont,brush, PointF(540,200));

}
//基于钟形曲线的渐变画刷    
void Brush_LinearGradientBrush_BellShape()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect myRect(10, 10, 150, 75);
  //定义线性渐变画刷
  gcGradientBrush myLGBrush(
    myRect,ColorBlue,ColorRed,0.0f,true);

  //使用默认的线性渐变画刷填充椭圆
  gcFillEllipse(&myLGBrush, myRect);

  //将渐变过程设置成基于钟形曲线的渐变
  myLGBrush.SetBlendBellShape(.5f, 1.0f);

  //使用自定义渐变过程的画刷填充椭圆
  gcTranslateTransform(160,0);
  gcFillEllipse(&myLGBrush, myRect);
}

void Brush_PathGradientBrush_Star()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorGray,1);
  SolidBrush pthGrBrush(ColorRed);
  SolidBrush blackbrush(ColorBlue);
  gcTranslateTransform(20,20);

  //构造五星的10个边的端点坐标
  Point points[] = 
  {
     Point(75, 0), Point(100, 50), 
     Point(150, 50), Point(112, 75),
     Point(150, 150), Point(75, 100), 
     Point(0, 150), Point(37, 75), 
     Point(0, 50), Point(50, 50),
     Point(75, 0)
   };

  // 创建路径
  GraphicsPath path;
  //在路径中加入直线
  path.AddLines(points, sizeof(points)/sizeof(Point));
  //填充路径
  gcFillPath(&pthGrBrush, &path);
  //绘制边界
  gcDrawLines(&pen, points, sizeof(points)/sizeof(Point));
  //绘制定义10个边的端点
  for(int i=0;i<10;i++)
    //每个圆点的直径为10
    gcFillEllipse(&blackbrush, points[i].X-5,points[i].Y-5,10,10);
}
//使用路径渐变画刷绘制五星
void Brush_PathGradientBrush_Star2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //构造五星的10个边的端点坐标
  Point points[] =
  {
     Point(75, 0), Point(100, 50), 
     Point(150, 50), Point(112, 75),
     Point(150, 150), Point(75, 100), 
     Point(0, 150), Point(37, 75), 
     Point(0, 50), Point(50, 50),
     Point(75, 0)
  };
  // 创建路径
  GraphicsPath path;
  //在路径中添加直线
  path.AddLines(points, sizeof(points)/sizeof(Point));
  //创建路径渐变画刷
  PathGradientBrush pthGrBrush(&path);
  //设置中心点色彩(终点色)
  pthGrBrush.SetCenterColor(ColorRed);

  //设置每个端点的色彩(终点色)
  Color colors[] =
  {
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite, 
    ColorBlack, ColorGreen, 
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen
  };
  //设置路径渐变画刷的边缘色
  int clrCount=sizeof(colors)/sizeof(Color);
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  // 填充目标路径
  gcFillPath(&pthGrBrush, &path);

  //将中心色及边界色都设置成随机色，查看渐变效果
  for(int z=0;z<10;z++)
  {
    //在水平方向上平移绘图平面
    gcTranslateTransform(200.0f, 0.0f);
    //设置中心点色彩为随机色
    pthGrBrush.SetCenterColor(_MakeA_RGB(rand_Next(255)%155,rand_Next(255)%255,rand_Next(255)%255));
    //使用原有的边缘色
    pthGrBrush.SetSurroundColors(colors, &clrCount);
    gcFillPath(&pthGrBrush, &path);
  }
}
//使用多个路径渐变画刷
void Brush_Using_MorePathGradientBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //定义一个六边形，边长为50  
  float fHalf = 50 * (float)sin(120.0f/360.0f*PI); 
  PointF pts[] =
  {
    PointF( 50, 0),           
    PointF( 50 * 1.5f,  0),
    PointF( 50,         0),           
    PointF( 50 / 2,    -fHalf),
    PointF(-50 / 2,    -fHalf),
    PointF(-50,         0),
    PointF(-50 * 1.5f,  0), 
    PointF(-50,         0),
    PointF(-50 / 2,     fHalf),     
    PointF( 50 / 2,     fHalf)
  };

  //构造六边形渐变画刷
  PathGradientBrush pgbrush1(pts, 10);

  //在水平和垂直方向上平移六边形的顶点
  for (int i = 0; i <10; i++)
  {
    pts[i].X += 50*1.5f;
    pts[i].Y += fHalf;
  }

  //根据改变坐标后的点重新生成路径渐变画刷
  PathGradientBrush pgbrush2(pts, 10);

  //设置路径渐变画刷的翻转方式为平铺
  pgbrush1.SetWrapMode(WrapModeTile);
  pgbrush2.SetWrapMode(WrapModeTile);

  //分别设置两个画刷的中心点色彩为红、绿色
  pgbrush1.SetCenterColor(ColorRed);
  pgbrush2.SetCenterColor(ColorGreen);

  //填充当前窗口
  gcFillRectangle(&pgbrush1, 0, 0,MyClient.Width,MyClient.Height);
  //在上次未被填充的区域中再次填充当前窗口的空白部份
  gcFillRectangle(&pgbrush2, 0, 0,MyClient.Width,MyClient.Height);
}
//路径渐变画刷的填充方式
void Brush_PathGradientBrush_WrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //定义五星的边线坐标，为演示翻转效果，将五星的一角拉长
  Point points[] =
  {
    Point(75, 0), Point(100, 50), 
    Point(150, 50), Point(112, 75),
    Point(150, 150), Point(75, 100), 
    Point(0, 190), Point(37, 75), 
    Point(10, 50), Point(50, 50)
  };

  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(Point));

  // 构造路径渐变画刷
  PathGradientBrush pthGrBrush(&path);
  //设置中心点色彩(终点色)
  pthGrBrush.SetCenterColor(ColorRed);

  //设置每个端点的色彩(终点色)
  Color colors[]=
  {
    ColorBlack,   ColorGreen,
    ColorBlue, ColorWhite, 
    ColorBlack,   ColorGreen, 
    ColorBlue, ColorWhite,
    ColorBlack,   ColorGreen
  };

  int clrCount=sizeof(colors)/sizeof(Color);
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  //缩小画刷
  pthGrBrush.ScaleTransform(0.2f,0.2f);
  int i=0;

  //对渐变画刷使用平铺排列方式(默认方式)
  pthGrBrush.SetWrapMode(WrapModeTile);
  gcFillRectangle(&pthGrBrush, Rect(i*120,0,120,120));
  gcDrawRectangle(&pen, Rect(i*120,0,120,120));
  MyDrawString(graphics, "Tile",myFont,brush, PointF(20,125));

  i+=1;
  pthGrBrush.SetWrapMode(WrapModeTileFlipX);
  gcFillRectangle(&pthGrBrush, Rect(i*120,0,120,120));
  gcDrawRectangle(&pen, Rect(i*120,0,120,120));
  MyDrawString(graphics, "TileFlipX",myFont,brush, PointF(170,125));

  //对渐变画刷使用垂直翻转排列方式
  pthGrBrush.SetWrapMode(WrapModeTileFlipY);
  gcFillRectangle(&pthGrBrush, Rect(0,200,120,120));
  gcDrawRectangle(&pen, Rect(0,200,120,120));
  MyDrawString(graphics, "TileFlipY",myFont,brush, PointF(0,325));

  //对渐变画刷使用水平、垂直同时翻转排列方式
  pthGrBrush.SetWrapMode(WrapModeTileFlipXY);
  gcFillRectangle(&pthGrBrush, Rect(120,200,120,120));
  gcDrawRectangle(&pen, Rect(120,200,120,120));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, PointF(170,325));

  //输出路径的约束矩形、中心点信息
  RectF rect;
  //获取画刷的约束矩形对象
  pthGrBrush.GetRectangle(&rect);
  PointF CenterPoint;
  //获取画刷的中心点信息
  pthGrBrush.GetCenterPoint(&CenterPoint);
  CString tmp;

  //格式化字符串
  tmp.Format("当前约束矩形的左上坐标为(%f,%f),宽度=%f 高度=%f\n当前渐变路径的中心点坐标为(%.2f,%.2f)",
    rect.X,rect.Y,rect.Height,rect.Width,
    CenterPoint.X,CenterPoint.Y);
  //输出中心点及约束矩形对象的信息
  MyDrawString(graphics, tmp,myFont,brush, PointF(0,395));
}
//更改路径渐变画刷的中心点
void Brush_PathGradientBrush_CenterPoint()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //构造一个圆形区域
  GraphicsPath path;
  path.AddEllipse(0, 0, 200, 200);
  SolidBrush brush(_MakeA_RGB(155,ColorRed));
  //构造一个圆形路径渐变画刷
  PathGradientBrush pthGrBrush(&path);

  //设置中心点色彩
  pthGrBrush.SetCenterColor(_MakeA_RGB(155,ColorWhite));
  Color colors[] ={_MakeA_RGB(55, ColorBlue)};
  //设置边缘色
  int clrCount=1;
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  //填充区域，使用单色画刷
  gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
  //获取中心点色彩
  PointF center(0,0);
  pthGrBrush.GetCenterPoint(&center);

  //更改画刷的中心点沿圆周的上半部分平移
  for(int i=0;i<200;i++)
  {
    center.X=(REAL)i;
    center.Y=10.f;
    pthGrBrush.SetCenterPoint(center);
    gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
    //标记当前中心点
    gcFillEllipse(&brush, center.X, center.Y, 2.f, 2.f);
  }

  //更改画刷的中心点沿圆周的下半部分平移
  for(i=200;i>0;i--)
  {
    center.X=(REAL)i;
    center.Y=190;
    pthGrBrush.SetCenterPoint(center);
    gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
    //标记当前中心点
    gcFillEllipse(&brush, center.X, center.Y, 2.f, 2.f);
  }

}
//对路径渐变画刷使用多色渐变
void Brush_PathGradientBrush_InterpolationColors()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorRed);
  //设置三角形的三个点
  Point points[]=
  {
    Point(100, 0), 
    Point(200, 200),
    Point(0, 200)
  };
  //创建一个三角形渐变画刷
  PathGradientBrush pthGrBrush(points, 3);
  //-定义参与渐变的色彩
  Color  colors[]=
  {
    ColorRed,     //红
    ColorGreen,     //绿
    ColorBlue // 蓝
  }; 

  //设置合成点的位置
  float pos[] =
  {
    0.0f,    // 红色在区域边界为红色
    0.4f,    //在距离中心40%的位置处使用绿色
    1.0f  //中心点使用蓝色
  };   

  //设置渐变的过渡色
  pthGrBrush.SetInterpolationColors(colors, pos, 3);
  //填充区域
  gcFillRectangle(&pthGrBrush, 0, 0, 300, 300);  
  //标记中心点
  PointF centerpoint(0,0);;
  pthGrBrush.GetCenterPoint(&centerpoint);
  gcFillEllipse(&brush, centerpoint.X-5.f, centerpoint.Y-5.f, 10.f, 10.f);
}
//更改路径渐变画刷的焦点缩放比例
void Brsuh_PathGradietBrush_Focus()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlack,3);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //指定三角形三点坐标,创建三角形渐变画
  Point points[]=
  {
    Point(100, 0),
    Point(200, 200),
    Point(0, 200)
  };

  PathGradientBrush pthGrBrush(points, 3);
  //指明渐变色
  Color colors[] = {ColorRed, ColorBlue};
  //指明色彩合成位置
  float relativePositions[]=
  {
    0.0f,    // 红色做边界
    1.0f // 蓝色为中心
  };

  //设置渐变色
  pthGrBrush.SetInterpolationColors(colors, relativePositions, 2);

  //使用默认缩放因子进行填充
  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200); 

  //获取默认缩放因子 
  PointF FocusScales(0,0);
  pthGrBrush.GetFocusScales(&FocusScales.X, &FocusScales.Y);
  //输出缩放信息
  CString tmp;
  tmp.Format("水平:x=%.2f\n垂直:y=%.2f",FocusScales.X,FocusScales.Y);
  MyDrawString(graphics, tmp,myFont,brush, PointF(0,210));

  //平移绘图平面
  gcTranslateTransform(200,0);
  //更改缩放因子
  FocusScales.X=0.6f;
  FocusScales.Y=0.6f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200);  
  MyDrawString(graphics, "水平:x=0.6\n垂直:y=0.6", myFont,brush, PointF(0,210));

  //水平缩放不等于垂直缩放时特例
  gcTranslateTransform(200,0);
  //更改缩放因子
  FocusScales.X=0.1f;
  FocusScales.Y=0.8f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush,0, 0, 200, 200); 
  MyDrawString(graphics, "水平:x=0.1\n垂直:y=0.8", myFont,brush, PointF(0,210));

  //水平缩放=垂直缩放=1.0
  gcTranslateTransform(200,0);
  FocusScales.X=1.0f;
  FocusScales.Y=1.0f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200); 
  MyDrawString(graphics, "水平:x=1.0\n垂直:y=1.0", myFont,brush, PointF(0,210));

  //水平缩放不等于垂直缩放时特例：放大原有区域
  gcTranslateTransform(200,0);
  FocusScales.X=2.0f;
  FocusScales.Y=1.5f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200);
  MyDrawString(graphics, "水平:x=2.0\n垂直:y=1.5", myFont,brush, PointF(0,210));
}
//路径渐变画刷的变换
void Brush_PathGradientBrush_Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // 定义一个三角形路径渐变画刷
  Point pts[]={Point(50, 0), Point(100, 100), Point(0, 100)};
  PathGradientBrush pthGrBrush(pts, 3);

  //对画刷使用垂直翻转方式
  pthGrBrush.SetWrapMode(WrapModeTileFlipY);
  //缩小画刷  
  pthGrBrush.ScaleTransform(0.5f, 0.5f);
  //填充当前窗口
  gcFillRectangle(&pthGrBrush, 0, 0, MyClient.Width,MyClient.Height);

  //将画刷旋转90度
  pthGrBrush.RotateTransform(90.0f, MatrixOrderAppend);
  //再次填充当前窗口
  gcFillRectangle(&pthGrBrush, 0, 0, MyClient.Width,MyClient.Height);
}

void Brsuh_LinearGradientBrush_UsingGamma()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  gcGradientBrush linGrBrush(
    Point(0, 10),
    Point(200, 10),
    ColorRed,
    ColorBlue);

  gcFillRectangle(&linGrBrush, 0, 0, 200, 50);
  linGrBrush.SetGammaCorrection(true);
  gcFillRectangle(&linGrBrush, 0, 60, 200, 50);

}
//简单的使用字体示意
void Font_UsingFontInGDIPlus()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //新建一个字体对话框
  CFontDialog dlg;
  //允许对字体进行色彩设置
  //dlg.ShowColor=true;
  //调用字体选择对话框
  if(dlg.DoModal()!=IDOK)
    return;
  //获取在字体对话框中的字体信息
  LOGFONT lf; dlg.GetCurrentFont(&lf);
  Font myFont(CWideCharString(dlg.GetFaceName()), lf.lfHeight);
  //根据字体色彩创建画刷
  SolidBrush brush(dlg.GetColor());
  //设置文本输出格式：在当前窗口中居中显示
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //输出文本
  gcDrawString(L"输出文本", 4, &myFont, RectF(0,0,MyClient.Width,MyClient.Height),&fmt, &brush);
}

//枚举所有的字体系列
void Font_EnumAllFonts()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush   solidBrush(ColorBlack);
  FontFamily   fontFamily(L"Arial");
  Font         font(&fontFamily, 8, FontStyleRegular, UnitPoint);
  //设置文本输出格式
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentNear);

  CString tmp="";
  //获取所有已经安装的字体系列
  InstalledFontCollection installedFontCollection;
  int ffcount=installedFontCollection.GetFamilyCount();
  FontFamily *fontfamily = ::new FontFamily[ffcount];
  installedFontCollection.GetFamilies(ffcount, fontfamily, &ffcount);
  int index=0;
  //访问fontfamily数组的每一个成员
  for( int i=0; i<ffcount; i++)
  {
    //获取当前字体系列名称
    WCHAR name[LF_FACESIZE];
    fontfamily[i].GetFamilyName(name);
    CString t=name;
    CString s; s.Format(_T("%s%s"), (i? _T(", "):_T("")), t);
    tmp+=s;
    index++;
  }
  ::delete []fontfamily;

  //在窗口中输出所有的字体系列名
  RectF r(0,10,MyClient.Width,MyClient.Height);
  MyDrawString(graphics, tmp, font, solidBrush, r, fmt);
  //输出字体系列信息
  tmp.Format("在你的系统中已经安装的字体有%d种，其名称分别为:\n",index);
  MyDrawString(graphics, tmp,font,solidBrush, Point(0,0));
}

//启用增强型选择对话框表单
void Font_EnhanceFontDialog()
{
  MyDrawString(GetGraphics(), "无增强型选择对话框表单",
    Font(L"Arial",12), SolidBrush(ColorRed), Point(0,0));
/*
  Form3 f= Form3();
  f.ShowDialog();
*/
}


//使用不同的字体边缘处理方式
void Font_UsingTextRenderHint()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brs(ColorBlack);
  FontFamily  fontFamily(L"Arial");
  Font        font(&fontFamily, 60, FontStyleRegular, UnitPixel);

  //使用不同的边缘处理方式输出六行文本
  for (int i=TextRenderingHintSystemDefault; i<=TextRenderingHintClearTypeGridFit; i++)
  {
    //设置边缘处理方式
    gcSetTextRenderingHint((TextRenderingHint)i);
    //输出文本
    MyDrawString(graphics, "Render",font,brs, PointF(0, 0));
    //绘图平面下移地行
    gcTranslateTransform(0, font.GetHeight(&graphics));
  }
}
//使用私有字体集合
void Font_Privatefontcollection()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 10.0f);
  SolidBrush  solidBrush(ColorRed);
  //创建私有字体集
  PrivateFontCollection privateFontCollection;

  //在私有字体集中追迷你繁启体字库文件"繁启体.TTF"
  WCHAR fontfile[]= L"c:\\繁启体.TTF";
  if (Ok != privateFontCollection.AddFontFile(fontfile))
  {
    AfxMessageBox("字体文件加载失败\n出错啦！");
    return;
  }

  //从私有字体集合中构造繁启体，大小为35像素
  FontFamily pFontFamily(L"迷你繁启体", &privateFontCollection);
  Font tmpFont(&pFontFamily, 35);
  //输出繁启体文字
  MyDrawString(graphics, "沉舟侧畔千帆过", tmpFont,solidBrush, pointF);
  //垂直坐标一移一行，行高为  字体的高度
  pointF.Y +=tmpFont.GetHeight(&graphics);
  MyDrawString(graphics, "病树前头万木春", tmpFont,solidBrush, pointF);
}

//在私有字体集合中使用多种字体
void Font_Privatefontcollection2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 10.0f);
  SolidBrush  solidBrush(ColorRed);
  //创建私有字体集
  PrivateFontCollection privateFontCollection;
  //在私有字体集中追加三个不同的字体文件
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\STCAIYUN.TTF");
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\SIMLI.TTF");
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\Arial.ttf");

  FontFamily fontfamily[200];
  int ffcount=privateFontCollection.GetFamilyCount();
  if (ffcount>200) ffcount=200;
  privateFontCollection.GetFamilies(ffcount, fontfamily, &ffcount);
  int index=0;
  //访问私有字体集合中的所有字体系列
  for(int i=0; i<ffcount; i++)
  {
    Font tmpFont(&fontfamily[i], 35);
    //获取当前字体系列名称
    WCHAR name[LF_FACESIZE];
    fontfamily[i].GetFamilyName(name);
    CString s; s.Format(_T("字体名:  %s"), name);
    //输出繁启体文字
    MyDrawString(graphics, s, tmpFont, solidBrush, pointF);
    gcTranslateTransform(0, tmpFont.GetHeight(&graphics));
  }
/*
  //通过访问PrivateFontCollection类的Families数组成员来字体
  for(int index=0;index< privateFontCollection..Families.Length;index++)
  {
    Font tmpFont(privateFontCollection.Families[index].Name,35);
    //输出繁启体文字
    MyDrawString(graphics, "字体名:  "+privateFontCollection.Families[index].Name,
      tmpFont,solidBrush, pointF);
    gcTranslateTransform(0,tmpFont.Height);
  }
*/
}
//检查字体风格是否可用
void Font_IsStyleAvailable()
{

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorGray);
  PointF      pointF(10.0f, 0.0f);
  SolidBrush  solidBrush(ColorBlack);
  //信息输出所使用的字体
  Font msgfont(L"Arial",12);

  //创建私有字体集
  PrivateFontCollection privateFontCollection;
  //在私有字体集中追加三个字体文件
  //黑体
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\simhei.ttf");
  //Courier New字体
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\CourBI.ttf");
  //华文隶书
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\STLITI.ttf");

  CString     familyName;
  CString     tmpmsg;

  //查询私有字体集中字体系列数量
  int count=privateFontCollection.GetFamilyCount();
  //获取字体系列数组
  FontFamily *pFontFamily = ::new FontFamily[count];      
  //将字体系列中的字体名中加入pFontFamily中
  privateFontCollection.GetFamilies(count, pFontFamily, &count);

  /*对私有字体集中每一个字体系列进行五种字体风格
  的查询，如果字体风格可用，使用该风格进行文本绘制
  */
  for(int j = 0; j< count; ++j)
  {
    //从字体系列中获取字体名
    WCHAR name[LF_FACESIZE];
    pFontFamily[j].GetFamilyName(name);
    familyName=name;
    // 常规风格是否可用
    if(pFontFamily[j].IsStyleAvailable(FontStyleRegular))
    {
      tmpmsg="  字体的常规风格可用";
      //构造字体
      FontFamily fm(CWideCharString(familyName), &privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleRegular, UnitPixel);
      //输出使用常规风格的字体
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //垂直坐标一移一行，行高为  字体的高度
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        CString(L"  字体的常规风格不可用"),msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // 粗体风格是否可用
    if(pFontFamily[j].IsStyleAvailable(FontStyleBold))
    {
      tmpmsg="  字体的粗体风格可用";
      //构造字体
      FontFamily fm(CWideCharString(familyName), &privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleBold, UnitPixel);
      //输出使用常规风格的字体
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //垂直坐标一移一行，行高为  字体的高度
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        CString("  字体的粗体风格不可用"),msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // 斜体风格是否可用
    if(pFontFamily[j].IsStyleAvailable(FontStyleItalic))
    {
      tmpmsg="  字体的斜体风格可用";
      //构造字体
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleItalic, UnitPixel);
      //输出使用常规风格的字体
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //垂直坐标一移一行，行高为  字体的高度
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  字体的斜体风格不可用",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // 查询下划线风格是否可用
    if(pFontFamily[j].IsStyleAvailable(FontStyleUnderline))
    {
      tmpmsg="  字体的下划线风格可用";
      //构造字体
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleUnderline, UnitPixel);
      //输出使用常规风格的字体
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //垂直坐标一移一行，行高为  字体的高度
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  字体的下划线风格不可用",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // 查询强调线风格是否可用
    if(pFontFamily[j].IsStyleAvailable(FontStyleStrikeout))
    {
      tmpmsg="  字体的强调线风格可用";
      //构造字体
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleStrikeout, UnitPixel);
      //输出使用常规风格的字体
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //垂直坐标一移一行，行高为  字体的高度
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  字体的强调线风格不可用",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // 在不同的字体系列之间加上间隔线
    gcDrawLine(&pen,0,(int )pointF.Y,400,(int)pointF.Y);
    pointF.Y += 10.0f;
  }
  ::delete []pFontFamily;
}
//获取字体的大小
void Font_Size()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 0.0f);
  SolidBrush  solidBrush(ColorBlack);

  CString infoString;
  int  ascent;
  float  ascentPixel;            
  int  descent;               
  float  descentPixel;          
  int  lineSpacing;           
  float  lineSpacingPixel;      

  FontFamily   fontFamily(L"Arial");
  //创建一个大小为16像素的Aria字体
  Font  font(&fontFamily, 16, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);

  // 显示字体大小
  infoString.Format("font的Size属性值为%.2f",font.GetSize());
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
  // 下移一行
  pointF.Y += font2.GetHeight(&graphics);
  //显示字体高度：像素
  infoString.Format("font的Height属性值为%.2f", font.GetHeight(&graphics));
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
  // 下移一行
  pointF.Y += font2.GetHeight(&graphics);

  // 显示字体系列的高度(设计单位)
  infoString.Format("使用fontFamily.GetEmHeight函数返回的字体高度为 %d 个设计单位。"
    ,fontFamily.GetEmHeight(FontStyleRegular));
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // 下移两行(Height属性的值是字体的高度，单位为像素)
  pointF.Y += 2.0f*font2.GetHeight(&graphics);

  // 获取字体的Ascent(上部距离)
  ascent = fontFamily.GetCellAscent(FontStyleRegular);

  //将上部距离的设计单位转换成像素单位
  ascentPixel= 
    font.GetSize() * ascent / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("上部距离为 %d个设计单位, %.2f个像素。",
    ascent, ascentPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // 下移一行
  pointF.Y += font2.GetHeight(&graphics);
  // 获取字体的Descent(下部距离),设计单位
  descent = fontFamily.GetCellDescent(FontStyleRegular);

  //将下部距离的设计单位转换成像素单位
  descentPixel = 
    font.GetSize() * descent / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("下部距离为%d 个设计单位, %.2f个像素。",
    descent, descentPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // 下移一行
  pointF.Y += font2.GetHeight(&graphics);
  //获取行距(设计单位)
  lineSpacing = fontFamily.GetLineSpacing(FontStyleRegular);
  //将行距的设计单位转换成像素单位
  lineSpacingPixel = 
    font.GetSize() * lineSpacing / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("行距为%d 个设计单位, %.2f 像素。",
    lineSpacing, lineSpacingPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
}
//设置文本输出基线
void Font_BaseLine()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily   fontFamily(L"Arial");
  //创建一个大小为60像素的Aria字体
  Font  font(&fontFamily, 60, FontStyleRegular, UnitPixel);
  //获取当前窗口的矩形宽、高
  int  cy=MyClient.Height;
  int cx=MyClient.Width;

  Pen pen(ColorBlue, 1);
  SolidBrush brush(ColorBlue);

  //将窗口垂直方向的1/2部分作为基线
  float yBaseline = cy / 2.f;
  //绘制基线
  gcDrawLine(&pen, PointF(0, yBaseline), PointF((REAL)cx,yBaseline));
  //得到字体高度
  float cyLineSpace = font.GetHeight(&graphics);
  //获取行距
  int   iCellSpace  = fontFamily.GetLineSpacing(FontStyleRegular);
  //得到上半部分距离
  int   iCellAscent = fontFamily.GetCellAscent(FontStyleRegular);
  //计算文本输出的起始位置相对于基线的偏移
  float cyAscent    = cyLineSpace * iCellAscent / iCellSpace;

  //从中央向上绘制两条直线，间隔为cyAscent
  gcDrawLine(&pen, 
    PointF(0, yBaseline-cyAscent), PointF(cx,yBaseline-cyAscent));
  gcDrawLine(&pen, 
    PointF(0, yBaseline-2.0f*cyAscent), PointF(cx,yBaseline-2.0f*cyAscent));

  //分别在两条基线上输出文本
  MyDrawString(graphics, "AaFfgSs}", font,brush, PointF(0,yBaseline - cyAscent));
  MyDrawString(graphics, "AaFfgSs}",font,brush, PointF(0,yBaseline - 2.0f*cyAscent));

}

void Font_DrawString()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //输出文本
  CString msg="示例文本";

  Font myFont(L"宋体", 16);
  RectF layoutRect(50, 50, 200, 50);
  //设置对齐方式
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  SolidBrush blackBrush(ColorBlack);

  // 绘制文本
  MyDrawString(graphics, msg, myFont, blackBrush, layoutRect, format);
  // 绘制文本输出的矩形区域
  Pen pen(ColorBlack, 3);
  gcDrawRectangle(&pen, layoutRect);

}

void Font_MeasureString()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //设置一个包含三行文本的字符串
  CString txtOut="123456789\n";
  txtOut+="ABCDEFGHIJKLM\n";
  txtOut+="一二三四五六七八九";

  FontFamily   fontFamily(L"Arial");
  //创建两个个大小不同的Aria字体
  Font  font(&fontFamily, 30, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  SolidBrush brush(ColorBlack);

  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);

  SizeF stringSize, strSize(1000,1000);      
  //测量输出字符串所需要的矩形空间
  CWideCharString txtOutW(txtOut);
  gcMeasureString(txtOutW, txtOutW.GetLength(), &font, strSize, &fmt, &stringSize);

  CString tmp;
  tmp.Format("输出字符串所需要的宽度为:%2f  高度为%2f", stringSize.Width,stringSize.Height);

  // 绘制输出文本的限制矩形
  Pen pen(ColorRed);
  gcDrawRectangle(&pen,
    10.0f,10.0f,stringSize.Width,stringSize.Height);
  //输出字符串的测量信息
  MyDrawString(graphics, txtOut, font, brush,
    RectF(10.0f,10.0f,stringSize.Width,stringSize.Height),fmt);
  gcTranslateTransform(0,10+stringSize.Height);
  MyDrawString(graphics, tmp,font2,brush, PointF(0,0));
}
//计算在指定的区域中显示的字符总数及行数
void Font_MeasureString2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  CString txtOut="123456789";
  txtOut+="ABCDEFGHIJKLM";
  txtOut+="一二三四五六七八九";

  FontFamily   fontFamily(L"Arial");
  //创建两个个大小不同的Aria字体
  Font  font(&fontFamily, 30, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  //设置文本输出矩形
  RectF layoutRect(10, 10,100,100);
  SolidBrush brush(ColorBlack);

  //设置文本显示格式
  StringFormat fmt;    
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);

  int codepointsFitted=0;
  int linesFilled=0;

  SizeF stringSize;
  //计算指定的区域能够显示的字符总数及行数
  CWideCharString txtOutW(txtOut);
  gcMeasureString(txtOutW, txtOutW.GetLength(), 
    &font, SizeF(layoutRect.Width,layoutRect.Height),
    &fmt, &stringSize, &codepointsFitted, &linesFilled);

  CString tmp;
  tmp.Format("欲输出的字串共%d个字符\n其中,在指定的输出矩形中\n只输出了%d行共%d个字符",
    txtOutW.GetLength(), linesFilled, codepointsFitted);

  // 绘制指定的文本输出区域
  Pen pen(ColorBlue,2);
  gcDrawRectangle(&pen, layoutRect);
  //在指定的区域矩形中显示文本
  MyDrawString(graphics, txtOut, font, brush, layoutRect, fmt);

  //输出字符串的测量信息
  gcTranslateTransform(0,10+stringSize.Height);
  MyDrawString(graphics, tmp, font2, brush, PointF(0,0));
}
//文本的分栏输出
void Font_ColumnTextOut()
{
  CString s_Filename;

  //假设读取 <exePath> ../ReadMe.txt
  TCHAR ModuleDirectory[_MAX_PATH];
  ::GetModuleFileName(NULL, ModuleDirectory, _MAX_PATH);
  TCHAR *p=::_tcsrchr(ModuleDirectory, '\\'); *p=0;
  p=::_tcsrchr(ModuleDirectory, '\\'); *p=0;
  ::_tcscat(ModuleDirectory, "\\ReadMe.txt");

  s_Filename=ModuleDirectory;

  FILE *stream = ::fopen(s_Filename, "rt");
  if (!stream)
    return;

  CString str;

  //读取所有的文件内容  
  char buffer[1024]="";
  while (!feof(stream))
  {
    CString s;
    ::fgets(buffer, 1024, stream);
    s=buffer; s.TrimLeft(); s.TrimRight(); s+="\r\n";
    str+=s;
  }
  //关闭文件
  fclose(stream);

  //定义栏宽及栏与栏之间的间隔
  int COLUMWIDTH=256;
  int SPACE=10;

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  Font myFont(L"宋体", 10);
  int codepointsFitted=0;
  int linesFilled=0;

  //设置输出格式
  StringFormat format;
  //禁用自动剪载
  format.SetFormatFlags(StringFormatFlagsNoClip);
  //输出文本以单词结束
  format.SetTrimming(StringTrimmingWord);

  CWideCharString strW(str);
  //从左至右按列(栏)输出文本
  for (int x = 0; strW.GetLength()> 0&&x<MyClient.Width; x +=(COLUMWIDTH+SPACE))
  {
    //设置文本对应的矩形区域(一栏)
    RectF layoutRect(x, 0, COLUMWIDTH,MyClient.Height-myFont.GetHeight(&graphics));
    SizeF stringSize;
    //计算指定的区域能够显示的字符总数及行数
    gcMeasureString(strW, strW.GetLength(),
      &myFont, SizeF(layoutRect.Width,layoutRect.Height),
      &format, &stringSize, &codepointsFitted, &linesFilled);

    //显示一栏文本
    gcDrawString(strW, strW.GetLength(), &myFont, layoutRect, &format, &brush); 
    //去掉已经输出的字符，以便下一栏能够正常显示        
    strW=strW.Mid(codepointsFitted);
  }
}
//设置文本的去尾方式
void Font_StirngTrimming()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"Times New Roman");
  //用于文本输出的字体
  Font        font(&fontFamily, 25, FontStyleRegular, UnitPixel);

  Pen pen(ColorRed);
  StringFormat stringFormat;
  //用于提示信息的字体及格式
  Font        font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  //设置文本输出的格式
  StringFormat msgFormat;
  msgFormat.SetAlignment(StringAlignmentCenter);
  msgFormat.SetLineAlignment(StringAlignmentCenter);

  //提示信息输出区域
  RectF outrect(30, 100, 160, font2.GetHeight(&graphics)*2);

  //去尾方式：Character
  stringFormat.SetTrimming(StringTrimmingCharacter);
  //输出文本
  CString text="One two three four five seven eight nine ten";
  MyDrawString(graphics, text, font, solidBrush, 
    RectF(30, 30, 160, 60), stringFormat);

  //绘制文本输出区域
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  //输出提示信息
  MyDrawString(graphics, "Character",font2,solidBrush,outrect, 
    msgFormat);

  //绘图平面沿水平方向平移160个像素
  gcTranslateTransform(160,0);

  //更改去尾方式：Word
  stringFormat.SetTrimming(StringTrimmingWord);
  MyDrawString(graphics, text,font,solidBrush,Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "Word",font2,solidBrush,outrect, msgFormat);


  //更改去尾方式：EllipsisCharacter
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisCharacter",font2,solidBrush,outrect, msgFormat);


  //更改去尾方式：EllipsisWord
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisWord);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisWord",font2,solidBrush,outrect, msgFormat);

  //更改去尾方式：EllipsisPath
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisPath);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisPath",font2,solidBrush,outrect, msgFormat);

}
//文本输出的剪裁处理
void Font_TextOutClip()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"宋体");
  Font        font(&fontFamily, 25, FontStyleRegular, UnitPixel);

  Pen pen(ColorRed);
  StringFormat stringFormat;
  RectF outtext(10, 10, 120, 45);
  //使用默认值输出文本
  CString text="文本的剪裁示意";
  MyDrawString(graphics, text,font,solidBrush,outtext, 
    stringFormat);
  //绘制文本输出区域
  gcDrawRectangle(&pen, outtext.X,
    outtext.Y,outtext.Width,outtext.Height);

  //在水平方向平移170个像素更
  gcTranslateTransform(120,0);
  RectF out2=outtext;
  //将输出区域的高度调整为字体高度的整数倍
  out2.Height=font.GetHeight(&graphics)*2;
  MyDrawString(graphics, text,font,solidBrush,out2, stringFormat);
  //绘制文本输出区域
  gcDrawRectangle(&pen, out2.X,
    out2.Y,out2.Width,out2.Height);

  gcTranslateTransform(120,0);
  //在输出文本时不使用剪截
  stringFormat.SetFormatFlags(StringFormatFlagsNoClip);
  MyDrawString(graphics, text,font,solidBrush,outtext, stringFormat);

  //绘制文本输出区域
  gcDrawRectangle(&pen, outtext.X,outtext.Y, outtext.Width,outtext.Height);
}
//测量文本局部输出区域
void Font_MeasureCharacterRanges()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // 设置绘制文本、填充区域的画刷
  SolidBrush TextBrush(ColorRed);
  SolidBrush RegionBrush(_MakeA_RGB(100, ColorBlack));
  Pen        blackPen(ColorBlack);

  // 使用三个宽度不同的矩形区域绘制文本，示意不同局部区域测量结果
  RectF   layoutRect_A(20.0f, 20.0f, 120.0f, 180.0f);
  RectF   layoutRect_B(140.0f, 20.0f, 140.0f, 180.0f);
  RectF   layoutRect_C(280.0f, 20.0f, 180.0f, 180.0f);

  //添加需要输出的字串
  CString ts="秦时明月汉时关，";
  ts+="万里长征人未还。";
  ts+="但使龙城飞将在，";
  ts+="不教胡马度阴山。";

  // 在字串中需要局部测量的文本定义区域：每行诗的最后三个字
  CharacterRange charRanges[] =
  { 
    CharacterRange(4, 3),
    CharacterRange(12, 3),
    CharacterRange(20, 3), 
    CharacterRange(28, 3)
  };

  // 绘制文本时使用的字体及大小 
  FontFamily   fontFamily(L"华文新魏");
  Font  myFont(&fontFamily, 25,  FontStyleItalic, UnitPixel);

  StringFormat strFormat;
  //指向需要局部测量区域的数组
  Region CharRangeRegions[4];

  //在字体格式中设置需要局部测量的区域
  strFormat.SetMeasurableCharacterRanges(4, charRanges);

  //在矩形layoutRect_A中对文本进行局部测量，测量的结果保存在CharRangeRegions中
  CWideCharString tsW(ts);
  gcMeasureCharacterRanges(tsW,tsW.GetLength(),&myFont,layoutRect_A, &strFormat,4,CharRangeRegions);
  //输出文本
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_A, strFormat);
  //绘制文本输出边框
  gcDrawRectangle(&blackPen, layoutRect_A.X,layoutRect_A.Y,layoutRect_A.Width,layoutRect_A.Height);
  short   i;                 
  //填充由CharRangeRegions保存的区域
  for ( i = 0; i < 4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

  //在矩形layoutRect_B中对文本进行局部测量，测量的结果保存在CharRangeRegions中
  gcMeasureCharacterRanges(tsW, tsW.GetLength(), &myFont, layoutRect_B, &strFormat,4,CharRangeRegions);
  //输出文本
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_B, strFormat );
  gcDrawRectangle(&blackPen, 
    layoutRect_B.X,layoutRect_B.Y,layoutRect_B.Width,layoutRect_B.Height);
  //填充区域
  for ( i = 0; i <4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

  //在矩形layoutRect_c中对文本进行局部测量，测量的结果保存在CharRangeRegions中
  gcMeasureCharacterRanges(tsW,tsW.GetLength(),&myFont,layoutRect_C, &strFormat,4,CharRangeRegions);
  //输出文本
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_C, strFormat);
  gcDrawRectangle(&blackPen,
    layoutRect_C.X,layoutRect_C.Y,layoutRect_C.Width,layoutRect_C.Height);
  //填充区域
  for ( i = 0; i <4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

}
//控件文本输出方向
void Font_TextoutDirection()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //添加需要输出的字串
  CString ts="秦时明月汉时关，";
  ts+="万里长征人未还。";
  ts+="但使龙城飞将在，";
  ts+="不教胡马度阴山。";

  //设置输出字体
  FontFamily   fontFamily(L"幼圆");
  Font  myFont(&fontFamily, 20,  FontStyleBold, UnitPixel);
  SolidBrush brush(ColorRed);
  Pen pen(ColorBlack);

  //设置输出区间
  Rect f(10,10,120,myFont.GetHeight(&graphics)*8);
  //构造StringFormat对象
  StringFormat strFormat;
  //从至左输出文本
  int flag=StringFormatFlagsDirectionRightToLeft;
  //文本在竖直方向上输出
  flag|=StringFormatFlagsDirectionVertical;
  //设置输出格式标志
  strFormat.SetFormatFlags(flag);

  //设置文本对齐方式
  strFormat.SetAlignment(StringAlignmentCenter);
  strFormat.SetLineAlignment(StringAlignmentCenter);

  //绘制输出区间及文本
  gcDrawRectangle(&pen,f);
  MyDrawString(graphics, ts,myFont,brush,f,strFormat);
}
//设置文本对齐方式
void Font_TextAlignment()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //将当前窗口大小信息保存
  Rect   rect(0, 0,
    MyClient.Width, MyClient.Height);
  SolidBrush brush(ColorBlack);
  Pen pen(ColorGray);

  //以当前窗口中心点为原点绘制坐标轴
  gcDrawLine(&pen,
    PointF(rect.Width/2,0), PointF(rect.Width/2,rect.Height));
  gcDrawLine(&pen,
    PointF(0,rect.Height/2), PointF(rect.Width,rect.Height/2));

  //将StringAlignment枚举的三种不同的对齐方式设置成数组成员，以便直接访问
  StringAlignment Align[3]=
  {
    StringAlignmentNear,
    StringAlignmentCenter,
    StringAlignmentFar
  };
  CString strAlign[] ={ "近", "中", "远" };

  //设置输出字体
  FontFamily   fontFamily(L"幼圆");
  Font  font(&fontFamily, 12,  FontStyleRegular);
  StringFormat strfmt;

  //分别在垂直和水平方向上使用三种不同的对齐方式
  for (int iVert = 0; iVert < 3; iVert++)
    for (int iHorz = 0; iHorz < 3; iHorz++)
    {
      //设置垂直对齐方式
      strfmt.SetLineAlignment((StringAlignment)Align[iVert]);
      //设置水平对齐方式
      strfmt.SetAlignment((StringAlignment)Align[iHorz]);
      //将对齐信息输出
      CString s;
      s.Format("水平对齐:%d\n垂直对齐:%d", strAlign[iVert],strAlign[iHorz]);

      //在当前区域中按照设置的对齐方式绘制文本
      MyDrawString(graphics, s,font,brush,rect, strfmt);
    }
}

void Font_TextAlignment2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //将当前窗口大小信息保存
  Rect   rect(0, 0, MyClient.Width, MyClient.Height);
  SolidBrush brush(ColorBlack);
  Pen pen(ColorGray);

  //以当前窗口中心点为原点绘制坐标轴
  gcDrawLine(&pen,
    PointF(rect.Width/2,0), PointF(rect.Width/2,rect.Height));
  gcDrawLine(&pen,
    PointF(0,rect.Height/2), PointF(rect.Width,rect.Height/2));

  //将StringAlignment枚举的三种不同的对齐方式设置成数组成员，以便直接访问
  StringAlignment Align[3]=
  {
    StringAlignmentNear,
    StringAlignmentCenter,
    StringAlignmentFar
  };
  CString strAlign[]={ "近", "中", "远" };

  //设置输出字体
  FontFamily   fontFamily(L"幼圆");
  Font  font(&fontFamily, 12,  FontStyleRegular);
  StringFormat strfmt;

  //分别在垂直和水平方向上使用三种不同的对齐方式
  for (int iVert = 0; iVert < 3; iVert+=2)
    for (int iHorz = 0; iHorz < 3; iHorz+=2)
    {
      //设置垂直对齐方式
      strfmt.SetLineAlignment((StringAlignment)Align[iVert]);
      //设置水平对齐方式
      strfmt.SetAlignment((StringAlignment)Align[iHorz]);
      //将对齐信息输出
      CString s;
      s.Format("水平对齐:%d\n垂直对齐:%d", strAlign[iVert],strAlign[iHorz]);

      //在当前区域中按照设置的对齐方式绘制文本
      MyDrawString(graphics, s,font,brush, PointF(MyClient.Width/2,MyClient.Height/2),strfmt);
    }
}
//使用制表位
void Font_TextoutUsingTabs()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  StringFormat stringFormat;
  //输出文本使用的字体
  FontFamily   fontFamily(L"Arial");
  Font         font(&fontFamily, 30, FontStyleBold, UnitPixel);
  SolidBrush solidBrush(ColorBlack);

  //不使用制表位信息的输出
  CString text = "A\tB\tC\t\t\tD";
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);

  //设置制表位信息1
  float  tabs[]={40.0f, 30.0f,40};
  stringFormat.SetTabStops(0.0f, sizeof(tabs)/sizeof(float), tabs);

  //使用制表位信息的进行文本输出
  gcTranslateTransform(0,30);
  text="A\tB\tC\tD";
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);

  //设置制表位信息2
  float tabs2[] = {60.0f, 60.0f, 60};
  stringFormat.SetTabStops(0.0f, 3, tabs2);
  gcTranslateTransform(0,30);
  MyDrawString(graphics, text, font, solidBrush, PointF(0,10), stringFormat);

  //获取制表信息  
  float  firstTabOffset = 0;
  float tabStops[3];
  stringFormat.GetTabStops(3, &firstTabOffset, tabStops);

  //清空制表位
  for(int j = 0; j <3; ++j)
    tabStops[j]=0;
  stringFormat.SetTabStops(0.0f, 3, tabStops);

  //在清空制表位信息后输出文本
  gcTranslateTransform(0,30);
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);
}

//定义商品信息结构
struct SListItem
{
  public: CString name;//商品名
  public: float price;//单价
  public: float  num;//数量
  //初如化
  public: SListItem(CString p1, float p2,float p3) 
  {
    name = p1;
    price=p2;
    num = p3;    
  }
};
void Font_UsingTabs()
{
  //初始化商品信息
  SListItem items[5]=
  {
    SListItem(L"MP3播放机",1228,10),
    SListItem(L"笔记本电脑",17200,1),
    SListItem(L"激光打印机",3200,5),
    SListItem(L"喷墨打印机",620,1),
    SListItem(L"数码相机",4800,5)
  };

  //定义表头
  CString title;
  title.Format("\n%s\t%s\t%s\t%s\n","商品名","单价","数量","总价");
  //格式化每种商品对应的字符串
  CString iteminfo[5];
  for (int i=0;i<5;i++)
  {
    //计算总价
    float total=items[i].price*items[i].num;
    //定制输出格式
    CString s;
    s.Format("%d\t%.2f\t%.2f\t%.2f\n",
      items[i].name,items[i].price,items[i].num,total);
    iteminfo[i]=s;
  }

  //在表格每一行的内容合并
  CString xxx;;
  for (i=0;i<5;i++)
    xxx+=iteminfo[i];
  //表头+表体
  title+=xxx;

  //在当前窗口中输出表格
  RectF  rect(0.0f,0.0f,
    MyClient.Width,MyClient.Height);

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush   solidBrush(ColorBlack);
  //定制输出字体
  FontFamily   fontFamily(L"宋体");
  Font         font(&fontFamily,
    16, FontStyleRegular, UnitPixel);

  StringFormat stringFormat;
  //设置制表位信息
  float   tabs[] = {120.0f, 120.0f,60.0f};
  stringFormat.SetTabStops(0.0f, sizeof(tabs)/sizeof(float), tabs);
  //输出表格
  MyDrawString(graphics, title,font, solidBrush,rect, stringFormat);
}

void Font_TextoutHotkeyPrefix()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"宋体");
  Font   font(&fontFamily, 20, FontStyleRegular, UnitPixel);
  Pen pen(ColorRed);

  StringFormat stringFormat;
  //显示快捷键字符，不解释"&"号
  stringFormat.SetHotkeyPrefix(HotkeyPrefixNone);
  MyDrawString(graphics, "工具栏(&T )\n状态栏(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  //绘制边框
  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

  //解释""号为下划线
  stringFormat.SetHotkeyPrefix(HotkeyPrefixShow);
  gcTranslateTransform(0,font.GetHeight(&graphics)*3);
  MyDrawString(graphics, "工具栏(&T )\n状态栏(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  //绘制边框
  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

  //隐藏""号
  gcTranslateTransform(0,font.GetHeight(&graphics)*3);
  stringFormat.SetHotkeyPrefix(HotkeyPrefixHide);
  MyDrawString(graphics, "工具栏(&T )\n状态栏(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

}

void Font_TextoutShadow()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //对文本使用去锯齿的边缘处理
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"华文楷体");
  Font        font(&fontFamily, 100, FontStyleBold, UnitPixel);

  //文本输出框
  RectF textout(font.GetHeight(&graphics),
    MyClient.Height/2,MyClient.Width,MyClient.Height);

  //在两个不同的位置绘制文本，形成阴影
  //solidBrush的色彩透明度为100，暗黑
  SolidBrush solidBrush(_MakeA_RGB(100,ColorBlack));
  SolidBrush redBrush(ColorRed);
  MyDrawString(graphics, "阴影字",font, solidBrush,
    PointF(27.0f, 27.0f));
  MyDrawString(graphics, "阴影字",font, redBrush,
    PointF(12.0f, 20.0f));

  //另一种阴影字，阴影为线条
  //构造影线画刷
  HatchBrush brush_tmp(
    HatchStyleDarkDownwardDiagonal,ColorBlack, ColorWhite);

  int reptime=40;
  //先画背景
  for(int i=0;i<reptime;i++)
    MyDrawString(graphics, "阴影字",font,brush_tmp,
      PointF(textout.X+i+2,textout.Y+i+2));

  //再画前景
  MyDrawString(graphics, "阴影字",font, redBrush, PointF(textout.X,textout.Y));
}

void Font_TextoutHashline()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //对文本使用去锯齿的边缘处理
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"华文楷体");
  Font        font(&fontFamily, 100, FontStyleBold, UnitPixel);

  //文本输出框
  RectF textout(font.GetHeight(&graphics),0,MyClient.Width,MyClient.Height);
  //画刷样本显示框
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));
  //创建一个影线画刷
  HatchBrush brush_tmp(HatchStyle75Percent,ColorBlack, ColorWhite);

  //设置文本输出格式：水平靠左、垂直居中
  StringFormat format;
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);

  //显示画刷样本
  gcFillRectangle(&brush_tmp,brushdemo);
  //使用画刷绘制文本
  MyDrawString(graphics, "影线字",font,brush_tmp,textout,format);

}

void Font_TextoutTexture()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //对文本使用去锯齿的边缘处理
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"华文楷体");
  Font        font(&fontFamily, 70, FontStyleBold, UnitPixel);

  //文本输出框
  RectF textout(0,0,MyClient.Width,MyClient.Height);
  //画刷样本显示框
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));

  //装入纹理图片
  Bitmap image(L"TEXTURE3.bmp");
  //构造纹理画刷
  TextureBrush brush_tmp(&image);

  //设置文本输出格式：水平、垂直居中
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  format.SetLineAlignment(StringAlignmentCenter);

  //显示画刷样本
  gcFillRectangle(&brush_tmp,brushdemo);
  //使用画刷绘制文本
  MyDrawString(graphics, "纹理字",font,brush_tmp,textout,format);
}

void Font_TextoutGradient()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //对文本使用去锯齿的边缘处理
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"华文楷体");
  Font        font(&fontFamily, 70, FontStyleBold, UnitPixel);

  RectF textout(0,0,MyClient.Width,MyClient.Height);
  //画刷样本显示框
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));
  //定义一个多色渐变画刷
  Color colors[] =
  {
    ColorRed,   // 红色
    ColorGreen,//过度色为绿色
    ColorBlue  // 蓝色
  };
  float positions[] = 
  {
    0.0f,   // 由红色起
    0.3f,   // 绿色始于画刷长度的三分之一
    1.0f // 由蓝色止
  }; 

  //构造一条从黑色到白色的渐变画刷
  gcGradientBrush brush_tmp(
    Point(0, 0), 
    Point(100, font.GetHeight(&graphics)), 
    ColorWhite,         // 黑
    ColorWhite);  // 白

  //设置渐变画刷的多色渐变信息
  //ColorBlend clrBlend(3);
  //clrBlend.Colors=colors;
  //clrBlend.Positions=positions;
  brush_tmp.SetInterpolationColors(colors, positions, 3);

  //设置文本输出格式：水平靠左、垂直居中
  StringFormat format;
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);
  //显示画刷样本
  gcFillRectangle(&brush_tmp,brushdemo);
  //使用画刷绘制文本
  MyDrawString(graphics, "渐变字",font,brush_tmp,textout,format);
}
//构造路径
void Path_Construct()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //定义构成路径的点坐标
  PointF data[]=
{
  PointF(40,140),
  PointF(275,200),
  PointF(105,225),
  PointF(190,300),
  PointF(50,350),
  PointF(20,180)
};

  //设置定义点的类型将每点都处理成直线的端点
  BYTE typeline[]=
{
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine
};

  //构造路径
  GraphicsPath tmp1(data,typeline,6);
  //填充路径
  gcFillPath(&SolidBrush(ColorRed),&tmp1);

  //绘制定义路径的端点
  for(int i=0;i<6;i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      RectF(data[i].X-5,data[i].Y-5,10,10));

  //绘图平面右移
  gcTranslateTransform(360,0);

  //更改路径的端点类型
  BYTE type[]=
{
  (BYTE)PathPointTypeStart,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine
};
  //构造并填充路径
  GraphicsPath tmp(data,type,6);
  gcFillPath(&SolidBrush(ColorRed),&tmp);

  //绘制定义路径的端点
  for(i=0;i<6;i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      RectF(data[i].X-5,data[i].Y-5,10,10));
}

void Path_AddLines()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  //创建路径对象
  GraphicsPath path;

  Rect rect1(20, 20, 100, 200);
  Rect rect2(40, 40, 100, 200);
  //添加两段弧线
  path.AddArc(rect1, 0.0f, 180.0f); 
  path.AddArc(rect2, 0.0f, 180.0f); 
  gcDrawPath(&pen, &path);
}
//封闭图形
void Path_CloseFigure()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  //创建路径对象
  GraphicsPath path;

  Rect rect1(20, 20, 100, 200);
  Rect rect2(40, 40, 100, 200);
  GraphicsPath path2;
  path2.AddArc(rect1, 0.0f, 180.0f);  
  //封闭弧线1
  path2.CloseFigure();
  path2.AddArc(rect2, 0.0f, 180.0f);  
  //封闭弧线2
  path2.CloseFigure();
  gcDrawPath(&pen, &path2);
}
//路径的填充
void Path_FillPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  GraphicsPath path1;
  GraphicsPath path;
  Pen pen(_MakeA_RGB(128, ColorBlue), 5);
  SolidBrush brush(ColorRed);

  //添加三条直线：开放的图形
  path1.AddLine(10,10,10,50);
  path1.AddLine(10,50,50,50);
  path1.AddLine(50,50,50,10);
  path1.StartFigure();

  //添加一个闭合的图形
  RectF rect(110,10,40,40);
  //开始第二个子路径的图形追加
  path1.AddRectangle(rect);
  //绘制、填充路径
  gcDrawPath(&pen, &path1);
  gcFillPath(&brush, &path1);

  //添加开放的弧线
  path.AddArc(0, 0, 150, 120, 30, 120);
  //添加封闭的椭圆
  path.AddEllipse(50, 50, 50, 100);

  //使用默认的填充方式填充路径
  gcFillPath(&brush, &path);
  gcDrawPath(&pen, &path);
}

void Path_AddSubPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  Rect rect(0, 0, 100, 50);
  GraphicsPath path;

  //在路径中按垂直方向添加五段弧线
  path.AddArc(0, 0, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 30, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 60, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 90, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 120, 100, 50, 0.0f, 180.0f);
  gcDrawPath(&pen, &path);

  //获取路径所占区域
  RectF rect2; path.GetBounds(&rect2);

  //绘图平面右移
  gcTranslateTransform(rect2.Width+10,0);

  GraphicsPath path2;
  //构造第一个子路径
  path2.AddArc(0, 0, 100, 50, 0.0f, 180.0f);
  path2.AddArc(0, 30, 100, 50, 0.0f, 180.0f);
  path2.AddArc(0, 60, 100, 50, 0.0f, 180.0f);

  //在不封闭当前子路径的情况下开始第二个子路径的绘制
  path2.StartFigure();
  //添加弧线
  path2.AddArc(0, 90, 100, 50, 0.0f, 180.0f);

  //在封闭当前子路径的情况下开始第三个子路径的绘制
  path2.CloseFigure();
  path2.AddArc(0, 120, 100, 50, 0.0f, 180.0f);

  //绘制子路径
  gcDrawPath(&pen, &path2);
}

void Path_GetSubPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,3);
  GraphicsPath path;
  // 字路径1
  path.AddRectangle(Rect(20, 20, 60, 30));   
  // 字路径2
  path.AddLine(100, 20, 160, 50);            
  path.AddArc(180, 20, 60, 30, 0.0f, 180.0f);
  // 字路径3
  path.AddRectangle(Rect(260, 20, 60, 30)); 
  // 字路径4
  path.AddLine(340, 20, 400, 50);           
  path.AddArc(340, 20, 60, 30, 0.0f, 180.0f);
  path.CloseFigure();
  // 字路径5
  path.AddRectangle(Rect(420, 20, 60, 30));  
  //绘制路径
  gcDrawPath(&bluePen, &path);

  // 建立路径描述对象iterator
  GraphicsPathIterator iterator(&path);
  GraphicsPath subpath;
  int isClosed=false;
  int count=0;
  //连续调用两次NextSubpath,获取子路径
  //将路径的当前位置移动到1
  count =iterator.NextSubpath(&subpath, &isClosed);
  //将路径的当前位置移动到2
  count = iterator.NextSubpath(&subpath, &isClosed);

  //在第二行绘制提取的子路径
  gcTranslateTransform(0,50);   
  gcDrawPath(&bluePen, &subpath);

  //复位所有的位置移动信息
  iterator.Rewind();
  count = iterator.NextSubpath(&subpath, &isClosed);
  //在第三行绘制新抽取的子路径
  gcTranslateTransform(0,50);   
  Pen redPen(ColorRed,3);
  gcDrawPath(&redPen, &subpath);

  //连续调用两次NextSubpath，以获取子路径的定义点信息
  int start=0;
  int end=0;
  CString msg;
  //复位所有的位置移动信息
  iterator.Rewind();
  count = iterator.NextSubpath(&start, &end, &isClosed);
  //保存位置索引信息
  msg.Format(_T("第一次移动子路径的定义点起止位置为%d\t和\t%d\n"),start,end);
  count = iterator.NextSubpath(&start, &end, &isClosed);
  CString s;
  s.Format(_T("第二次移动子路径的定义点起止位置为%d\t和\t%d\n"),start,end);
  msg+=s;
  //绘制子路径的定义点位置索引信息
  FontFamily  fontFamily(L"宋体");
  Font font(&fontFamily,12);
  gcTranslateTransform(0,60);
  MyDrawString(graphics, msg,font,SolidBrush(ColorBlack), PointF(0,0));
}

void Path_GetPoints()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //创建路径对象
  GraphicsPath path;
  //添加直线
  path.AddLine(20, 100, 150, 120);
  //添加矩形
  path.AddRectangle(Rect(40, 30, 20, 60));
  //添加椭圆
  path.AddEllipse(Rect(70, 30, 100, 40));
  //添加曲线　
  Point points[] =
  {
    Point(300, 40),
    Point(350, 60),
    Point(300, 80),
    Point(300, 100),
    Point(350, 150)
  };
  path.AddCurve(points,5);

  //绘制路径
  Pen pen(ColorBlue);
  gcDrawPath(&pen, &path);

  // 获取定义路径的点总数
  int count =path.GetPointCount();
  //绘制点定义路径的点
  SolidBrush brush(ColorRed);
  PointF *points0 = new PointF[count];
  path.GetPathPoints(points, count);
  for(int j = 0; j < count; ++j)
  {
    gcFillEllipse(
      &brush, 
      points0[j].X - 3.0f, 
      points0[j].Y - 3.0f,
      6.0f,
      6.0f);
  }
  delete points0;
  /*另外一一种遍历数组的方式
  foreach( PointF x in path.PathPoints)
    gcFillEllipse(
      brush, 
      x.X - 3.0f, 
      x.Y - 3.0f,
      6.0f,
      6.0f);
  */
}

void Path_PathData()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //曲线的定义点
  Point points[] =
{
  Point(40, 60), 
  Point(50, 70), 
  Point(30, 90)
};

  Pen pen(ColorRed, 2);
  GraphicsPath path;
  //弧线
  path.AddArc(175, 50, 50, 50, 0.0f, -180.0f);
  //直线
  path.AddLine(100, 0, 250, 20);
  path.StartFigure();
  //直线
  path.AddLine(50, 20, 5, 90);
  //曲线
  path.AddCurve(points, 3);
  path.AddLine(50, 150, 150, 180);
  path.CloseAllFigures();
  //绘制路径
  gcDrawPath(&pen, &path);
  //获取PathData属性值
  PathData pathData; 
  path.GetPathData(&pathData);
  SolidBrush brush(ColorRed);

  //绘制定义点
  for (int i=0; i<pathData.Count; i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      pathData.Points[i].X - 5.0f,pathData.Points[i].Y - 5.0f,10.0f,10.0f);
  //输出每一个路径定义点的类型
  FontFamily  fontFamily(L"宋体");
  Font font(&fontFamily,12);
  RectF rect;
  path.GetBounds(&rect);
  gcTranslateTransform(rect.Width+10,0);
  for (i=0; i<pathData.Count; i++)
  {
    CString msg;
    msg.Format("第%d点的类型为:\t%d", i+1, pathData.Types[i]);
    MyDrawString(graphics, msg,
      font,SolidBrush(ColorBlack), PointF(0,(i+1)*font.GetHeight(&graphics)));
  }
}

void Path_ViewPointFLAG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.8f,0.8f);


  SolidBrush brush(ColorRed);
  //创建一个具有四个子路径的路径
  GraphicsPath path;
  path.AddRectangle(Rect(20, 20, 60, 30));
  path.AddLine(100, 20, 160, 50);
  path.AddArc(180, 20, 60, 30, 0, 180);
  path.AddRectangle(Rect(260, 20, 60, 30));
  path.AddLine(340, 20, 400, 50);
  path.AddArc(340, 20, 60, 30, 0, 180);
  path.CloseAllFigures();
  //填充路径
  gcFillPath(&brush, &path);

  //将路径的标记信息传入GraphicsPathIterator对象
  GraphicsPathIterator iterator(&path);

  //获取标记点信息
  int start=0;
  int end=0;
  int count=0;
  //获取第一个标记区间
  count = iterator.NextMarker(&start, &end);
  //为点坐标、类型分配空间
  PointF *points = new PointF[count];
  BYTE *types = new BYTE[count];
  //从iterator复制信息
  iterator.CopyData(points, types, start, end);
  //根据复制的信息新创建一个路径
  GraphicsPath pathnew(points, types, count);

  //在新位置绘制路径的定义点
  gcTranslateTransform(0,60);
  for(int j = 0; j < count; ++j)
    gcFillEllipse(&SolidBrush(ColorBlack),points[j].X - 2.0f,
      points[j].Y - 2.0f,  4.0f,4.0f);

  //填充新创建的路径
  gcTranslateTransform(0,60);
  gcFillPath(&brush, &pathnew);
  gcTranslateTransform(0,60);

  //输出所有的标记点信息
  CString msg;
  for(j = 0; j < count; ++j)
  {
    CString s;
    s.Format("第%s的信息:X=%.2f\tY=%.2f\t类型=%d\n",
      j,points[j].X,points[j].Y,types[j]);
    msg+=s;
  }

  //绘制字符串
  FontFamily  fontFamily(L"宋体");
  Font   font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  MyDrawString(graphics, msg, font,SolidBrush(ColorBlack), PointF(0,0));

  delete types;
  delete points;
}

void Path_SubPathRange()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorRed);
  //创建一个具有五个子路径的路径
  GraphicsPath path;
  // *************path.SetMarkers();**************
  //添加第一个子路径
  path.AddRectangle(Rect(20, 20, 60, 30));

  //添加第二个子路径
  path.AddLine(100, 20, 160, 50);
  //第二个标记将第二个子路径从中央截断
  path.SetMarker(); 
  path.AddArc(180, 20, 60, 30, 0, 180);

  //添加第三个子路径
  path.AddRectangle(Rect(260, 20, 60, 30));
  //第三个标记
  path.SetMarker(); 

  //添加第四个子路径
  path.AddLine(340, 20, 400, 50);
  path.AddArc(340, 20, 60, 30, 0, 180);

  //填充路径
  gcFillPath(&brush,&path);

  //将路径的标记信息传入GraphicsPathIterator对象
  GraphicsPathIterator iterator(&path);
  int d=iterator.GetSubpathCount();
  //获取标记点信息
  int start;
  int end;
  int count;
  //移到第二个标记
  count = iterator.NextMarker(&start, &end);
  //移到第三个标记
  count = iterator.NextMarker(&start, &end);
  //为点坐标、类型分配空间
  PointF *points = new PointF[count];
  BYTE *types = new BYTE[count];

  //从iterator复制信息
  iterator.CopyData(points, types, start, end);
  //根据复制的信息新创建一个路径
  GraphicsPath pathnew(points, types, count);

  //在新位置绘制标记点
  gcTranslateTransform(0,60);
  for(int j = 0; j < count; ++j)
    gcFillEllipse(&SolidBrush(ColorBlack),
      points[j].X - 2.0f,points[j].Y - 2.0f,
      4.0f,
      4.0f);

  //填充新创建的路径
  gcTranslateTransform(0,60);
  gcFillPath(&brush,&pathnew);

  //输出所有的标记点信息
  CString msg;
  for( j = 0; j < count; ++j)
  {
    CString s;
    s.Format(_T("第%d点的信息:X=%.2f\tY=%.2f \t类型＝%d\n"),
      j,points[j].X,points[j].Y,types[j]);
    msg+=s;
  }

  FontFamily  fontFamily(L"宋体");
  Font        font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  //绘制字符串
  gcTranslateTransform(0,60);
  MyDrawString(graphics, msg, font,SolidBrush(ColorBlack), PointF(0,0));

  delete points;
  delete types;
}


void Path_Flatten()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //曲线定义点
  Point pts[] =
  {
    Point(20,50), 
    Point(60,70), 
    Point(80,10), 
    Point(120,50)
  };

  GraphicsPath path;
  path.AddCurve(pts, 4);
  path.AddEllipse(160, 10, 150, 80);
  path.AddBezier(20, 100, 20, 150, 50, 80, 100, 60);

  //按照默认的方式绘制路径
  Pen pen(ColorBlack,3);
  gcDrawPath(&pen, &path);
  //获取路径点信息
  PathData pathData;
  path.GetPathData(&pathData);

  // 绘制路径定义点
  SolidBrush brush(ColorRed);
  for(int j = 0; j <pathData.Count; ++j)
  {
    gcFillEllipse(&brush, pathData.Points[j].X - 3.0f,
      pathData.Points[j].Y - 3.0f,6.0f, 6.0f);
  }

  //绘图平面下移100个单位 
  gcTranslateTransform(0,120);
  //进行曲线转  
  path.Flatten(&Matrix(), 14.0f);
  pen.SetColor(ColorGreen);
  //绘制转换后的路径  
  gcDrawPath(&pen, &path);
}


void Path_Warp()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // 创建一个全部由线条组成的路径
  PointF points[] =
  {
    PointF(20.0f, 60.0f),
    PointF(30.0f, 90.0f),
    PointF(15.0f, 110.0f),
    PointF(15.0f, 145.0f),
    PointF(55.0f, 145.0f),
    PointF(55.0f, 110.0f),
    PointF(40.0f, 90.0f),
    PointF(50.0f, 60.0f)
  };

  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(PointF));
  path.CloseFigure();

  // 绘制扭曲前的路径
  Pen bluePen(ColorBlue,3);
  gcDrawPath(&bluePen, &path);

  // 定义放置路径的源矩形
  RectF srcRect(10.0f, 50.0f, 50.0f, 100.0f);
  //定义矩形映射的目标点
  PointF destPts[] =
  {
    PointF(220.0f, 10.0f),
    PointF(280.0f, 10.0f),
    PointF(100.0f, 150.0f),
    PointF(300.0f, 150.0f)
  };

  //扭曲矩形
  path.Warp(destPts, 4, srcRect);
  // 绘制扭曲后的路径
  gcDrawPath(&bluePen, &path);

  // 绘制源矩形和目标的多边形
  Pen blackPen(ColorBlack,1);
  gcDrawRectangle(&blackPen, 
    srcRect.X,srcRect.Y,srcRect.Width,srcRect.Height);
  gcDrawLine(&blackPen, destPts[0], destPts[1]);
  gcDrawLine(&blackPen, destPts[0], destPts[2]);
  gcDrawLine(&blackPen, destPts[1], destPts[3]);
  gcDrawLine(&blackPen, destPts[2], destPts[3]);
}

void Path_Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  GraphicsPath path;
  //添加一个矩形子路径   
  path.AddRectangle(Rect(40, 10, 200, 50));
  //绘制坐标变换前的路径
  gcDrawPath(&Pen(ColorBlue,4), &path);

  //旋转路径
  Matrix matrix;
  matrix.Rotate(15.0f);
  path.Transform(&matrix);
  //绘制进行坐标变换后的路径
  gcDrawPath(&Pen(ColorRed,4), &path);
}

void Path_Widen()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,1);
  Pen greenPen(ColorGreen, 10);
  //定义曲线
  PointF points[] =
  {
    PointF(10.0f, 10.0f),
    PointF(130.0f, 90.0f),
    PointF(140.0f, 60.0f),
    PointF(60.0f, 90.0f)
  };

  GraphicsPath path;
  //添加四条曲线
  path.AddClosedCurve(points, 4);

  //复制三个相同的路径进行轮廓拓宽处理
  GraphicsPath *path2=path.Clone();
  GraphicsPath *path3=path.Clone();
  GraphicsPath *path4=path.Clone();

  //使用宽度为10的画笔绘制路径
  gcDrawPath(&Pen(ColorBlue,10), &path);
  RectF rect; path.GetBounds(&rect);

  //使用绿色画笔拓宽路径轮廓
  path2->Widen(&greenPen);
  //绘图平面右移，绘制拓宽的路径
  gcTranslateTransform(rect.Width+10,0);
  gcDrawPath(&bluePen, path2);

  //下移
  gcTranslateTransform(-rect.Width,rect.Height);
  //使用间断线风格的绿色画笔拓宽路径
  greenPen.SetDashStyle(DashStyleDash);
  path3->Widen(&greenPen);
  //填充路径
  gcFillPath(&SolidBrush(ColorRed), path3);

  //对画笔进行缩放变换 
  Pen pentmp(ColorBlue);
  pentmp.ScaleTransform(3.0f, 20.0f);
  path4->Widen(&pentmp);
  //绘图平面右移
  gcTranslateTransform(rect.Width,0);
  gcDrawPath(&bluePen, path4);

  delete path2;
  delete path3;
  delete path4;
}

void Path_WidenDemo()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,1);
  Pen greenPen(ColorGreen, 20);
  GraphicsPath path;
  //添加一条直线
  path.AddLine(10,20,100,20);
  GraphicsPath *path2=path.Clone();

  PathData pathData;
  PathData pathData2;

  path.GetPathData(&pathData);
  int num1=path.GetPointCount();

  //保存路径的定义点信息
  CString msg;
  for(int i=0;i<num1;i++)
  {
    CString s;
    s.Format("点%d的坐标分别为：X=%.2f\tY=%.2f\n",
      i,pathData.Points[i].X,pathData.Points[i].Y);
    msg+=s;
  }
  //绘制路径
  gcDrawPath(&Pen(ColorBlue), &path);

  //使用绿色画笔拓宽路径轮廓
  path2->Widen(&greenPen);
  path2->GetPathData(&pathData2);
  int num2=path2->GetPointCount();

  //绘图平面右移，绘制拓宽的路径
  RectF rect; path.GetBounds(&rect);
  gcTranslateTransform(rect.Width*2,0);
  gcDrawPath(&bluePen, path2);
  gcResetTransform();
  //保存拓宽后路径的定义点信息
  CString msg2;
  for(i=0;i<num2;i++)
  {
    CString s;
    s.Format("拓宽后的路径点%d的坐标分别为：X=%.2f\tY=%.2f\n",
      i,pathData2.Points[i].X,pathData2.Points[i].Y);
    msg2+=s;
  }

  //输出路径拓宽前后的定义点坐标位置信息
  FontFamily  fontFamily(L"宋体");
  Font  font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  gcTranslateTransform(0,120);
  MyDrawString(graphics, msg,font,SolidBrush(ColorBlack), PointF(0,0));  
  gcTranslateTransform(0,font.GetHeight(&graphics)*(num1+1));
  MyDrawString(graphics, msg2,font,SolidBrush(ColorBlack), PointF(0,0));  

}

void Region_FromPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //设定文本输出质量
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  FontFamily   fontFamily(L"隶书");
  //创建路径区域
  GraphicsPath path;

  //向区域中追加文本
  path.AddString(L"文字区域",4,&fontFamily,
    (int)FontStyleRegular,100.0f, Point(0, 0), &StringFormat() );

  Pen pen(ColorRed);
  //绘制路径
  gcDrawPath(&pen, &path);
  gcTranslateTransform(0,80);
  //从路径中构造区域
  Region region(&path);
  //填充区域  
  gcFillRegion(&SolidBrush(ColorRed),&region);
}

void Region_Calculate()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily  fontFamily(L"宋体");
  Font   font(&fontFamily, 16, FontStyleRegular, UnitPixel);

  SolidBrush solidBrush2(ColorRed);
  SolidBrush solidBrush(ColorGreen);
  Pen pen(ColorCyan);

  //定义多边形的端点坐标
  Point points[]=
  {
    Point(75, 0), 
    Point(100, 50), 
    Point(150, 50), 
    Point(112, 75),
    Point(150, 150), 
    Point(75, 120), 
    Point(10, 150), 
    Point(37, 75), 
    Point(0, 50), 
    Point(50, 50),
    Point(75, 0)
  };

  //创建路径并添加线条
  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(Point));

  //缩小路径
  Matrix matrix;
  matrix.Scale(0.5f,0.5f);
  path.Transform(&matrix);

  //获取路径所占的矩形区域
  RectF tmp; path.GetBounds(&tmp);

  //定义区域2
  RectF rect(0,10,tmp.Width,50); 
  //计算文本信息输出区域
  RectF textout(0,tmp.Height,tmp.Width,font.GetHeight(&graphics)*2);

  //设置文本输出格式：水平居中、垂直靠下
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  format.SetLineAlignment(StringAlignmentFar);

  //填充路径 
  gcFillPath(&solidBrush2,&path);

  gcTranslateTransform(tmp.Width+10,0);
  //填充矩形 
  gcFillRectangle(&solidBrush,rect);

  //从路径中创建区域
  Region region(&path); 

  //求交集
  region.Intersect(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //填充交集
  gcFillRegion(&solidBrush2, &region);
  MyDrawString(graphics, "交集",font,SolidBrush(ColorBlack),textout,format);  
  //绘制路径所占的矩形区域
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //求并集
  Region region2(&path); 
  region2.Union(rect);
  gcTranslateTransform(tmp.Width+10,0);
  gcFillRegion(&solidBrush2, &region2);
  MyDrawString(graphics, "并集",font,solidBrush,textout,format);  
  //绘制路径所占的矩形区域
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //换行输出区域运算结果
  gcTranslateTransform(-(tmp.Width+10)*3,tmp.Height+textout.Height);

  //求异并集
  Region region3(&path); 
  region3.Xor(rect);
  //填充异并集
  gcFillRegion(&solidBrush2, &region3);
  MyDrawString(graphics, "异并集",font,SolidBrush(ColorBlack),textout,format);  
  //绘制路径所占的矩形区域
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //求补集
  Region region4(&path); 
  region4.Complement(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //填充补集
  gcFillRegion(&solidBrush2, &region4);
  MyDrawString(graphics, "补集",font,SolidBrush(ColorBlack),textout,format);  
  //绘制路径所占的矩形区域
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //求排斥集
  Region region5(&path); 
  region5.Exclude(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //填排斥集
  gcFillRegion(&solidBrush2, &region5);
  MyDrawString(graphics, "排斥集",font,SolidBrush(ColorBlack),textout,format);  
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);
}

void Region_GetRects()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush solidBrush(ColorRed);
  Pen pen(ColorBlack);
  GraphicsPath path;
  Matrix matrix;

  //从路径中创建区域
  path.AddEllipse(10, 0, 80, 120);
  Region pathRegion(&path);    
  //填充区域
  gcFillRegion(&solidBrush, &pathRegion);

  //获取路径的矩形组成集信息（不使用变换）
  int count = pathRegion.GetRegionScansCount(&matrix);
  RectF *rects = new RectF[count];
  pathRegion.GetRegionScans(&matrix, rects, &count);
  //绘制平面右移，以绘制矩形集中所有的矩形
  gcTranslateTransform(90,0); 
  for(int i=0; i<count; i++)
    gcDrawRectangle(&pen, rects[i]);  

  //实施变换
  matrix.Scale(1.0f,0.75f);
  matrix.Shear(0.2f,0.3f);
  //获取路径的矩形组成集信息（使用变换）
  count = pathRegion.GetRegionScansCount(&matrix);
  delete rects;
  rects = new RectF[count];
  pathRegion.GetRegionScans(&matrix, rects, &count);
  //绘制平面右移，以绘制变换后的矩形集
  gcTranslateTransform(90,0); 
  for(i=0; i<count; i++)
    gcDrawRectangle(&pen, rects[i]);  

  delete rects;
}

void Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush mySolidBrush1(ColorBlue);
  SolidBrush mySolidBrush2(_MakeA_RGB(155,ColorChocolate));
  GraphicsPath myGraphicsPath;
  Rect myRect(60, 60, 60, 60);
  //在路径中添加矩形
  myGraphicsPath.AddRectangle(myRect);
  //填充路径
  gcFillPath(&mySolidBrush1, &myGraphicsPath);

  // 进行坐标变换
  Matrix myPathMatrix;
  //水平方向扩大两倍，垂直方向保持不变
  myPathMatrix.Scale(2, 1);
  myPathMatrix.Rotate(30.0f);
  //对路径实话坐标变换
  myGraphicsPath.Transform(&myPathMatrix);

  //填充变换后的路径
  gcFillPath(&mySolidBrush2, &myGraphicsPath);
}

void TranslateTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"nemo.bmp");
  //定义图片的显示区域
  Rect rect(0,0,110,70);
  for(int i=0;i<10;i++)
  {
    //显示图片
    gcDrawImage(&image,rect);
    //绘图平面右移，每次沿水平方面右移110个像素
    gcTranslateTransform(110,0);
  }
}

void RotateTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"nemo.bmp");

  //获取当前窗口的中心点
  Rect rect(0,0,MyClient.Width,MyClient.Height);
  PointF center(rect.Width/2,rect.Height/2);

  float offsetX=0;
  float offsetY=0;
  offsetX=center.X-image.GetWidth()/2;
  offsetY=center.Y-image.GetHeight()/2;
  //构造图片显示区域:让图片的中心点与窗口的中心点一致
  RectF picRect(offsetX,offsetY,image.GetWidth(),image.GetHeight());
  PointF Pcenter(picRect.X+picRect.Width/2,
    picRect.Y+picRect.Height/2);

  //让图片绕中心旋转一周
  for(int i=0;i<361;i+=10)
  {
    // 绘图平面以图片的中心点旋转
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    gcRotateTransform(i);
    //恢复绘图平面在水平和垂直方向的平移
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //绘制图片并延时
    gcDrawImage(&image,picRect);  
    ::sys_sleep(100);
    //重置绘图平面的所有变换
    gcResetTransform();
  }
}

void DrawWatch()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);

  //设置路码表的高和宽
  float WIDTH=clientrect.Width/2;
  float HEIGHT =clientrect.Height/2;

  //设置文本输出外观
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  FontFamily  fontFamily(L"Times New Roman");
  Font   font(&fontFamily, 14, FontStyleBold, UnitPixel);

  //将绘图平面的坐标原点移到窗口中心
  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  //绘制仪表盘
  gcFillEllipse(&SolidBrush(ColorBlack),
    HEIGHT/-2,HEIGHT/-2,HEIGHT, HEIGHT);
  //输出文本
  MyDrawString(graphics, "公里/小时",font,SolidBrush(ColorGreen),
    PointF(-26,HEIGHT/-4-font.GetHeight(&graphics)));

  //绘制刻度标记
  gcRotateTransform(225);
  for(int x = 0; x<55;x++)
  {
    gcFillRectangle(&SolidBrush(ColorGreen),
      Rect(-2, -(HEIGHT/2-2), 3, 15));
    gcRotateTransform(5);
  }

  //重置绘图平面的坐标变换
  gcResetTransform();

  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  gcRotateTransform(225);
  int sp = 0;
  CString tmp;
  //绘制刻度值(整数类)
  for(x = 0; x<7;x++)
  {
    tmp.Format("%d",sp);

    //绘制红色刻度
    gcFillRectangle(&SolidBrush(ColorRed),Rect(-3,(HEIGHT/2 -2)* -1,6,25));
    //绘制数值
    MyDrawString(graphics, tmp,font,SolidBrush(ColorGreen), PointF(tmp.GetLength()*(-6),
      (HEIGHT/-2) + 25));
    //旋转45度
    gcRotateTransform(45);
    sp += 20;
  }

  //重置绘图平面的坐标变换
  gcResetTransform();
  gcTranslateTransform(WIDTH /2,HEIGHT/2);

  //绘制指针在30公里/秒的情形
  float angle= 30*2.25f+ 225;
  gcRotateTransform((float)angle);

  Pen pen(ColorBlue,14);
  //设置线帽
  pen.SetEndCap(LineCapArrowAnchor);
  pen.SetStartCap(LineCapRoundAnchor);
  //画指针
  gcDrawLine(&pen, PointF(0,0), PointF(0,(-1)*(HEIGHT/2.75F)));

  gcResetTransform();
  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  //绘制中心点
  gcFillEllipse(&SolidBrush(ColorBlack),-6,-9,14,14);
  gcFillEllipse(&SolidBrush(ColorRed),-7,-7,14,14);

  //绘制速度极限标记
  pen.SetWidth(4);
  pen.SetColor(ColorBlack);
  pen.SetEndCap(LineCapRound);
  pen.SetStartCap(LineCapFlat);
  gcDrawLine(&pen, PointF(HEIGHT/15.75F,HEIGHT/3.95F),
    PointF(HEIGHT/10.75F,HEIGHT/5.2F));
  pen.SetColor(ColorRed);
  gcDrawLine(&pen, PointF(HEIGHT/15.75F,HEIGHT/3.95F),
    PointF(HEIGHT/15.75F,HEIGHT/4.6F));
}

void ScaleTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);
  //获取当前窗口的中心点
  float width=clientrect.Width/2;
  float height=clientrect.Height/2;
  PointF center(width,height);

  //设置矩形初始大小为30*30
  float offsetX=center.X-15;
  float offsetY=center.Y-15;

  //构造矩形的缩放区域:让矩形的中心点与窗口的中心点一致
  RectF rotateRect(offsetX,offsetY,30,30);
  PointF Pcenter(rotateRect.X+rotateRect.Width/2,
    rotateRect.Y+rotateRect.Height/2);

  //设置初始缩放倍数
  int scale=1;
  //对矩形rotateRect旋转360度并不断放大矩形
  for(int i=0;i<360;i+=60)
  {
    // 绘图平面以图片的中心点旋转
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    //在水平和垂直方向上同时扩大矩形
    gcScaleTransform(scale,scale);
    gcRotateTransform(i);
    //恢复绘图平面在水平和垂直方向的平移
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //绘制矩形并延时
    gcDrawRectangle(&Pen(ColorBlack),
      rotateRect.X,rotateRect.Y,rotateRect.Width,rotateRect.Height);
    ::sys_sleep(100);
    //重置绘图平面的所有变换
    gcResetTransform();
    scale++;
  }
}

void RectScale()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);
  //获取当前窗口的中心点
  float width=clientrect.Width/2;
  float height=clientrect.Height/2;
  PointF center(width,height);

  //设置矩形初始大小为30*30
  float offsetX=center.X-15;
  float offsetY=center.Y-15;

  //构造矩形的缩放区域:让矩形的中心点与窗口的中心点一致
  RectF rotateRect(offsetX,offsetY,30,30);
  PointF Pcenter(rotateRect.X+rotateRect.Width/2,
    rotateRect.Y+rotateRect.Height/2);

  //设置初始缩放倍数
  int scale=0;
  //对矩形rotateRect旋转360度并不断放大矩形
  for(int i=0;i<361;i+=60)
  {
    // 绘图平面以图片的中心点旋转
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    //在水平和垂直方向上同时增加矩形的宽度和高度
    rotateRect.Inflate(scale*15,scale*15);
    gcRotateTransform(i);
    //恢复绘图平面在水平和垂直方向的平移
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //绘制图片并延时
    gcDrawRectangle(&Pen(ColorBlack),
      rotateRect.X,rotateRect.Y,rotateRect.Width,rotateRect.Height);
    ::sys_sleep(100);
    //重置对矩形大小的所有变换
    gcResetTransform();
    rotateRect.Inflate(-scale*15,-scale*15);
    scale++;
  }

}

void FontRotate()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily  fontFamily(L"宋体");
  Font        myFont(&fontFamily, 20, FontStyleBold, UnitPixel);
  SolidBrush redBrush(ColorRed);

  //设置文本输出区域
  RectF layoutRect(myFont.GetHeight(&graphics),myFont.GetHeight(&graphics),
    MyClient.Width/2,MyClient.Height/2);

  StringFormat format;
  //设置文本输出格式
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);
  //将文本的输出起点设置成窗口的中心
  gcTranslateTransform(layoutRect.Width,layoutRect.Height);

  //在旋转时每隔30度输出文本
  for(int i=0;i<360;i+=30)
  {
    //旋转绘图平面
    gcRotateTransform(i);
    MyDrawString(graphics, " 旋转字体",myFont, redBrush,
      layoutRect, format);
    //恢复所做的旋转
    gcRotateTransform(-i);
  }

}

void MirrorText()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  SolidBrush redBrush(ColorRed);

  //构造字体，用于镜像文本输出
  FontFamily  fontFamily(L"华文楷体");
  Font        myFont(&fontFamily,
    48, FontStyleRegular, UnitPixel);
  //构造英文字体，用于演示文本的输出方向
  FontFamily  fontFamily2(L"Courier New");
  Font        myFont2(&fontFamily2, 16, FontStyleRegular, UnitPixel);

  //设置文本输出区域
  RectF layoutRect(myFont.GetHeight(&graphics),myFont.GetHeight(&graphics),
    MyClient.Width/2,MyClient.Height/2);

  StringFormat format;

  //设置文本输出时精确位置
  //行距－下行部分　(设计单位)
  float ascent =fontFamily.GetLineSpacing(FontStyleRegular)- 
    fontFamily.GetCellDescent(FontStyleRegular);
  //行距－上行部分　(设计单位)
  float ascent2 =fontFamily.GetLineSpacing(FontStyleRegular)-
    fontFamily.GetCellAscent(FontStyleRegular);

  //将上、下部距离的设计单位转换成像素单位
  float ascentPixel =   myFont.GetSize()*
    ascent /(fontFamily.GetEmHeight(FontStyleRegular));
  float ascentPixel2 =   myFont.GetSize()* 
    ascent2 /(fontFamily.GetEmHeight(FontStyleRegular));

  //绘图平面下移
  gcTranslateTransform(0,MyClient.Height/4);
  //绘制正常方向的文本
  MyDrawString(graphics, "镜像输出",myFont,redBrush,
    PointF(0,-myFont.GetHeight(&graphics)+ascentPixel2), format);

  //绘图平面右移，演示当前文本输出文向
  gcTranslateTransform(200,0);
  CString msg;
  for(int i=0;i<3;i++)
  {
    msg.Format("Line %d...........",i);
    MyDrawString(graphics, msg,myFont2,redBrush, 
      PointF(0,myFont2.GetHeight(&graphics)*i),format);
  }

  //恢复对绘图平面的右移
  gcTranslateTransform(-200,0);

  //绘图平面在垂直方向倒置
  gcScaleTransform(1,-1);
  //输出镜像文本
  MyDrawString(graphics, "镜像输出",myFont,SolidBrush(ColorGray), 
    PointF(0,-ascentPixel), format);
  //绘图平面再次右移，演示当前文本输出文向
  gcTranslateTransform(200,0);
  for(i=0;i<3;i++)
  {
    msg.Format("Line %d...........",i);
    MyDrawString(graphics, msg,myFont2, SolidBrush(ColorGray),
      PointF(0,myFont2.GetHeight(&graphics)*i), format);
  }

  //重置在绘图平面上进行的所有变换：演示水平镜像文本输出
  gcResetTransform ( ) ; 
  //绘图平面下移
  gcTranslateTransform(MyClient.Width/2,
    MyClient.Height/2+myFont.GetHeight(&graphics)*2);
  //绘制正常方向的文本
  MyDrawString(graphics, "镜像输出",myFont, redBrush,
    PointF(0,0), format);
  //绘图平面在水平方向倒置
  gcScaleTransform(-1,1);
  //输出水平镜像文本
  MyDrawString(graphics, "镜像输出",myFont, SolidBrush(ColorGray),
    PointF(0,0), format);
}


void Matrix_ListElements_Click_1()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush redBrush(ColorRed);
  FontFamily  fontFamily(L"华文楷体");
  Font  myFont(&fontFamily,20,
    FontStyleRegular, UnitPixel);    

  //进行平移变换  
  gcTranslateTransform(10,10);
  gcTranslateTransform(20,10); 
  Matrix m;;
  gcGetTransform(&m);
  //获取已经实施在绘图平面的坐标变换
  CString msg;

  float x[6]; m.GetElements(x);
  //输出矩阵的每一个元素
  for(int i=0; i<6; i++)
  {      
    msg.Format("第%d个矩阵元素为%.2f",i,x[i]);
    MyDrawString(graphics, msg,myFont, redBrush,
      PointF(0, myFont.GetHeight(&graphics)*(i+1)));
  }
}

void MatrixPos()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,4);
  Pen pen2(ColorGray);

  FontFamily  fontFamily(L"宋体");
  Font  myFont(&fontFamily,
    20, FontStyleRegular, UnitPixel);

  //获取当前窗口的大小
  Rect rect(0,0,
    MyClient.Width,MyClient.Height);

  //设置定义椭圆的矩形
  Rect r(-40, -65,80, 130);
  Matrix matrix;
  gcTranslateTransform(rect.Width/2, rect.Height/2); 
  //使用默认的旋转顺序(MatrixOrderPrepend)
  gcRotateTransform(20);   
  //获取对绘图平面已经进行的坐标变换
  gcGetTransform(&matrix);
  //绘制椭圆
  gcDrawEllipse(&pen, r);

  //使用MatrixOrderAppend旋转顺序
  gcResetTransform();
  gcTranslateTransform(rect.Width/2, rect.Height/2); 
  gcRotateTransform(20,MatrixOrderAppend);   
  //使用红色画笔绘制另一个椭圆
  gcDrawEllipse(&Pen(ColorRed,1), r);
  Matrix matrix2;
  gcGetTransform(&matrix2);

  //重置坐标变换、以窗口中心点为原点绘制坐标轴
  gcResetTransform();
  gcDrawLine(&pen2, 0, rect.Height/2, 
    rect.Width, rect.Height/2);
  gcDrawLine(&pen2, rect.Width/2, 
    0, rect.Width/2, rect.Height);  

  CString msg;
  //获取矩阵元素值
  float x[6]; matrix.GetElements(x);
  //输出矩阵元素值
  int i=0;
  for (i=0; i<6; i++)
  {
    msg.Format("%8.2f",x[i]);
    //输出行向量
    if(i==1||i==3||i==5)
    {
      //垂直方向上下移一行
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
      MyDrawString (graphics, msg, myFont, SolidBrush(ColorBlack), PointF(0,0));
    }
  }
}

void Martrix_Invert()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen myPen(ColorBlue,4);
  Matrix matrix(1.0f, 0.0f, 0.0f, 1.0f, 30.0f, 20.0f);
  //检查矩阵是否可逆？
  if(!matrix.IsInvertible())
  {
    AfxMessageBox("该矩形为不可逆矩形，无法演示");
    return;
  }
  gcSetTransform(&matrix);
  for(int i=0;i<200;i++) 
  {
    //计算逆矩形
    matrix.Invert();
    //对绘图平面进行坐标变换
    gcSetTransform(&matrix);
    gcDrawRectangle(&myPen, 35, 25, 200, 100);  
  }
}

void Matrix_Multiply()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"Arial");
  Font    myFont(&fontFamily,
    16, FontStyleRegular, UnitPixel);
  //加载图片
  Bitmap image(L"jieba.bmp");

  //缩放
  Matrix matrix1(0.80f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f); 
  //位移
  Matrix matrix2(1.0f, 0.0f, 0.0f, 1.0f, 20.0f, 10.0f);  
  //从右上往左下翻转图片：X轴与Y轴对调
  Matrix matrix3(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

  //第一次追加变换
  matrix1.Multiply(&matrix2, MatrixOrderAppend);
  //第二次追加变换
  matrix1.Multiply(&matrix3,MatrixOrderAppend);
  //进行复合变换
  gcSetTransform(&matrix1);

  //绘制图片
  gcDrawImage(&image, 0, 0);  
  //获取已经作用在绘图平面上的坐标变换
  Matrix matrix;
  gcGetTransform(&matrix);
  gcResetTransform();
  //在新位置上查看变换矩阵信息
  gcTranslateTransform(image.GetHeight(),0);

  //获取矩阵元素值
  float x[6]; matrix.GetElements(x);
  //输出每一个矩阵元素的值
  CString msg;
  int i=0;
  for(i=0; i<6; i++)
  {
    msg.Format("%6.2f",x[i]);
    //输出行向量
    if(i==1||i==3||i==5)
    {
      //垂直方向上下移一行
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
      MyDrawString(graphics, msg,myFont,SolidBrush(ColorBlack), 
        PointF(0,0));
    }
  }
}

void Matrix_TransformPoints()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush redBrush(ColorRed);
  SolidBrush blueBrush(ColorBlue);
  Pen pen1(ColorBlue,2);
  Pen pen2(ColorRed,1);

  //定义构成曲线的点坐标
  Point points[] =
  {
    Point(50, 100), 
    Point(100, 50),
    Point(150, 125),
    Point(200, 100),
    Point(250, 125),
  };

  //绘制曲线(未使用变换时)
  gcDrawCurve(&pen1, points, 5);
  //绘制曲线定义点信息
  for(int i=0;i<5;i++)  
  {
    gcFillEllipse(&blueBrush,
      points[i].X-5,points[i].Y-5,10,10);
  }

  //定义一个在竖直方向上缩放因子为0.5的矩阵
  Matrix matrix(1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f);
  //对points数据中的每一个成员进行矩形运算
  matrix.TransformPoints(points);

  //绘制曲线(使用变换后)  
  gcDrawCurve(&pen2, points,5);

  //再次绘制曲线定义点信息
  for(i=0;i<5;i++)  
  {
    gcFillEllipse(&redBrush,
      points[i].X-5,points[i].Y-5,10,10);
  }
}

void Matrix_TransformPoints2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //构造字体系列
  FontFamily   fontFamily(L"隶书");

  SolidBrush redBrush(ColorRed);
  //设置文本输出质量
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  //创建路径区域
  GraphicsPath path;
  //向区域中追加文本，字体大小为60
  path.AddString(L"文字特效",4 ,&fontFamily,(int)FontStyleRegular, 
    60, Point(0, 0), &StringFormat());

  //获取路径的点信息
  //获取路径的点类型信息
  PathData pathdata;
  path.GetPathData(&pathdata);

  //将文本在水平方向上缩小，在垂直方向上拉伸
  Matrix matrix(0.50f, 0.0f, 0.0f, 3.5f, 0.0f,0.0f);  

  //对points数据中的每一个成员进行矩形运算
  matrix.TransformPoints(pathdata.Points, pathdata.Count);
  //根据计算后的点重新构造路径
  GraphicsPath newpath(pathdata.Points,pathdata.Types,pathdata.Count);
  //填充路径
  gcFillPath(&redBrush,&newpath);

  //第二种特效
  Matrix matrix2(0.6f, 0.5f, 0.20f, 1.5f, 160.0f, -40.0f);
  PathData pathdata2;
  path.GetPathData(&pathdata2);
  //对所有的点进行计算
  matrix2.TransformPoints(pathdata2.Points, pathdata2.Count);
  //根据计算后的点重新构造路径
  GraphicsPath newpath2(pathdata2.Points,pathdata2.Types,pathdata2.Count);
  gcFillPath(&redBrush,&newpath2);
}

void Matrix_TransformVectors()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 7);
  pen.SetEndCap(LineCapArrowAnchor);
  SolidBrush brush(ColorBlue);

  // 定义一个点和一个向量
  PointF point[]={PointF(100, 50)};
  PointF vector[]={PointF(100, 50)};

  // 绘制点point
  gcFillEllipse(&brush, RectF(point[0].X-5, point[0].Y-5, 10, 10));
  //将点(10,10)与vector表示的位置连接起来
  gcDrawLine(&pen, PointF(10, 10), vector[0]);

  // 定义变换矩阵
  Matrix matrix(0.8f, 0.6f, -0.6f, 0.8f, 100.0f, 0.0f);
  //对点point的位置信息进行变换
  matrix.TransformPoints(point);
  //对点vector的位置信息进行二维向量变换
  matrix.TransformVectors(vector);

  // 绘制变换后的点
  pen.SetColor(ColorRed);
  brush.SetColor(ColorRed);
  gcFillEllipse(&brush, RectF(point[0].X - 5, point[0].Y - 5, 10, 10));
  //将点(10,10)与变换后的vector表示的位置连接起来
  gcDrawLine(&pen, PointF(10, 10), vector[0]); 
}

void Matrix_RotateAt()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,4);
  Pen pen2(ColorGray);

  //获取当前窗口区域
  RectF rect(0,0,MyClient.Width,MyClient.Height);;
  //构造并平移矩阵到窗口中心点
  Matrix matrix;
  matrix.Translate(rect.Width/2, rect.Height/2);  

  //将绘图平面依次旋转一周
  for(int i=0;i<360;i+=30)
  {
    //旋转矩阵，相对于窗口中心点
    matrix.RotateAt(i, PointF(rect.Width/2, 
      rect.Height/2), MatrixOrderAppend);
    //将旋转后的矩阵作用于绘图平面
    gcSetTransform(&matrix);
    //绘制椭圆
    gcDrawEllipse(&pen, -80, -30, 160, 60);
    //重置作用在绘图平面是所有的变换
    gcResetTransform();
  }

  //以窗口中心点为原点绘制坐标轴
  gcDrawLine(&pen2, PointF(0, rect.Height/2),
    PointF(rect.Width, rect.Height/2));
  gcDrawLine(&pen2, PointF(rect.Width/2, 0), 
    PointF(rect.Width/2, rect.Height));
}

void Matrix_Shear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"jieba.bmp");
  //定义图片的显示区域
  Rect rect(0,0,100,150);
  //绘制源图
  gcDrawImage(&image,rect);

  Matrix matrix;
  //设置水平投射因子
  matrix.Shear(0.8f,0.0f);
  //对绘图平面使用投射变换
  gcSetTransform(&matrix);
  gcTranslateTransform(100,0);
  //绘制投射变换后的图片
  gcDrawImage(&image,rect);

  gcResetTransform();
  Matrix matrix2;
  //设置垂直投射因子
  matrix2.Shear(0.0f,0.8f);
  //对绘图平面使用投射变换
  gcSetTransform(&matrix2);
  gcTranslateTransform(200,0);
  //绘制投射变换后的图片
  gcDrawImage(&image,rect);
}

void Matrix_TextoutShear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"华文楷体");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  Font myfont(L"Times New Roman", 100);  
  Matrix mymat;
  //投射
  mymat.Shear( -1.4f, 0.0f ) ;
  //缩放
  mymat.Scale( 1, 0.5f ) ;
  //平移
  mymat.Translate ( 236, 170 ) ; 
  //对绘图平面实施坐标变换、、
  gcSetTransform(&mymat);

  SolidBrush mybrush(ColorGray);
  SolidBrush redbrush(ColorRed);
  //绘制阴影
  MyDrawString(graphics, "Hello",myfont,mybrush, PointF(0, 50));
  gcResetTransform ( ) ;
  //绘制前景
  MyDrawString(graphics, "Hello",myfont,redbrush, PointF(0, 50));
  CString msg;
  //获取矩阵元素值
  float x[6]; mymat.GetElements(x);
  //输出每一个矩阵元素的值
  int i=0;
  for(; i<6; i++)
  {
    msg.Format("{0,%6.2f}",x[i]);
    //输出行向量
    if(i==1||i==3||i==5)
    {
      MyDrawString(graphics, msg,myFont,SolidBrush(ColorBlack), 
        PointF(0,0));
      //垂直方向上下移一行
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
    }
  }
}

void Matrix_ChangeFontHeight()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //构造字体系列
  FontFamily   fontFamily(L"隶书");
  //创建路径区域
  GraphicsPath path;
  SolidBrush redBrush(ColorRed);

  //设置文本输出质量
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  //向区域中追加文本，字体大小为80
  path.AddString(L"大小渐变",4,&fontFamily,(int)FontStyleRegular, 
    80, Point(0, 0), &StringFormat());
  //获取路径所占的矩形区域
  RectF bound; path.GetBounds(&bound);
  //获取路径区域的中心点
  float halfH=bound.Height/2;
  float halfW=bound.Width/2;

  //对路径实施变换，更改路径区域的中心点
  Matrix pathMartrix(1,0,0,1,-halfW,-halfH);
  path.Transform(&pathMartrix);

  //获取路径的点信息
  //获取路径的点类型信息
  PathData pathdata;
  path.GetPathData(&pathdata);

  //依次对路径的定义点的Y值进行缩放
  for(int i=0;i<pathdata.Count;i++)
  {
    //根据该点距路径起点的距离占整个路径长度的比例更改Y值
    pathdata.Points[i].Y*=2*(bound.Width-abs(pathdata.Points[i].X))/bound.Width;
  }

  //根据更改后的路径定义点重新构造路径
  GraphicsPath newpath(pathdata.Points,pathdata.Types,pathdata.Count);
  //将绘图平面的原点移到窗口中心
  gcTranslateTransform(MyClient.Width/2,MyClient.Height/2-40);
  //填充路径
  gcFillPath(&redBrush,&newpath);
}

void ColorMatrix_Start()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap image(L"ColorInput.bmp");
  int width = image.GetWidth();
  int  height = image.GetHeight();
  ImageAttributes  imageAttributes;

  //定义色彩变换矩阵
  ColorMatrix colorMatrix=
  {
    2.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //绘制源图
  gcDrawImage(&image, 0, 0);
  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void TranslateColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap  image(L"ColorBar.bmp");
  ImageAttributes  imageAttributes;
  int  width = image.GetWidth();
  int  height = image.GetHeight();

  //定义色彩变换矩阵
  ColorMatrix colorMatrix=
  {
    1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
    0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.75f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(&colorMatrix, 
    ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);

  //绘制源图
  gcDrawImage(&image, 0, 0);
  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);
}

void ScaleColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap          image(L"ColorBar.bmp");
  ImageAttributes imageAttributes;
  int             width = image.GetWidth();
  int             height = image.GetHeight();

  //定义色彩变换矩阵1
  ColorMatrix colorMatrix=
  {
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //定义色彩变换矩阵2
  ColorMatrix colorMatrix2=
  {
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 500.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵1
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //绘制源图
  gcDrawImage(&image, 0, 0);
  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);

  //清除已经采取的色彩变换
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //重新加载另一变换矩阵Matrix2
  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //演示使用Matrix2的色彩调整情况
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void RotateColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap    image(L"Colorinput.bmp");
  int   width = image.GetWidth();
  int   height = image.GetHeight();
  float   degrees = 90;
  //从角度到弧度
  double           r = degrees * PI/ 180.0f;  
  ImageAttributes  imageAttributes;
  //绘制源图
  gcDrawImage(&image, 0, 0);

  //红色绕着蓝色旋转
  ColorMatrix colorMatrix=
  {
    (float)cos(r),  (float)sin(r), 0.0f, 0.0f, 0.0f,
    -(float)sin(r), (float)cos(r), 0.0f, 0.0f, 0.0f,
    0.0f,    0.0f,   1.0f, 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  //使用色彩变换矩阵输出图片(R->B)
  gcTranslateTransform(width+10,0);
  //设置R->B色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  gcDrawImage(
    &image, 
    Rect(0, 10, width, height),  
    0, 0,width,height,      
    UnitPixel,
    &imageAttributes);

  //清除已经采取的色彩变换
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);

  //重新加载另一变换矩阵Matrix2
  //绿色绕着红色旋转
  ColorMatrix colorMatrix2=
  {
    1,  0, 0.0f, 0.0f, 0.0f,
    0, (float)cos(r), (float)sin(r), 0.0f, 0.0f,
    0.0f, -(float)sin(r), (float)cos(r), 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //在第二行输出
  gcResetTransform();
  gcTranslateTransform(0,height+10);
  gcDrawImage(&image, 
    Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,
    &imageAttributes);

  //清除已经采取的色彩变换
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //蓝色绕着红色旋
  ColorMatrix colorMatrix3=
  {
    (float)cos(r),  0, -(float)sin(r), 0.0f, 0.0f,
    0, 1, 0.0f, 0.0f, 0.0f,
    (float)sin(r), 0, (float)cos(r), 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  //重新加载另一变换矩阵Matrix3
  imageAttributes.SetColorMatrix(
    &colorMatrix3, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void ColorShear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap  image(L"Colorinput.bmp");
  ImageAttributes  imageAttributes;
  int   width = image.GetWidth();
  int   height = image.GetHeight();

  //定义色彩变换矩阵
  ColorMatrix colorMatrix=
  {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //绘制源图
  gcDrawImage(&image, 0, 0);
  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

}

void ColorRemap()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载蓝色背景的图片
  Bitmap            image(L"Nemo_Blue.bmp");
  ImageAttributes  imageAttributes;

  int   width = image.GetWidth();
  int   height = image.GetHeight();
  //将蓝色替换成白色,以达到抠除的效果
  ColorMap   colorMap;
  colorMap.oldColor = Color(255, 0, 0, 255); 
  colorMap.newColor = Color(255, 255,255,255);
  //设置色彩转换表
  ColorMap remapTable[1];  remapTable[0]=colorMap;

  //设置图片的色彩信息
  imageAttributes.SetRemapTable(1, remapTable, ColorAdjustTypeBitmap);
  //绘制原始图像
  gcDrawImage(&image, 0, 0, width, height);
  //绘制已经抠除背景色的新图像
  gcDrawImage(&image, 
    Rect(width+10, 0, width, height),  //目标区域
    0, 0,        // 源图左上角坐标
    width,       // 源图宽
    height,      // 源图宽
    UnitPixel,
    //图片的色彩信息
    &imageAttributes);
}

void SetRGBOutputChannel()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap  image(L"jieba.bmp");
  //绘制源图
  gcDrawImage(&image, 0, 0);

  int  width = image.GetWidth();
  int height = image.GetHeight();
  ImageAttributes  imageAttributes;

  //设置红色通道
  ColorMatrix colorMatrix=
  {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

  //清除已经采取的色彩变换
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //设置绿色通道
  ColorMatrix colorMatrix2=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  //使用色彩变换矩阵输出图片
  gcResetTransform();
  gcTranslateTransform(0,height+10);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

  //清除已经采取的色彩变换
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);

  //设置蓝色通道
  ColorMatrix colorMatrix3=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //启用色彩变换矩阵
  imageAttributes.SetColorMatrix(
    &colorMatrix3, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //使用色彩变换矩阵输出图片
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);
}

void Metafile()
{
  //  Note:  To record a metafile, you must construct a Graphics
  //object based on a Metafile object. The recording of the metafile
  //ends when that Graphics object is deleted or goes out of scope.

  //新建一个图元文件
  HDC hdc =GetGraphics().GetHDC();
  Metafile metaFile1(L"图元文件示例.emf", hdc);
  {
    //使用Metafile对象的地址做为绘图平面
    Graphics graphics(&metaFile1);

    //定义一个由红到蓝的渐变色画刷
    gcGradientBrush RtoBBrush(
      Point(0, 10),
      Point(200, 10),
      ColorRed,   
      ColorBlue);

    //定义一个由蓝到黄的渐变色画刷
    gcGradientBrush BtoYBrush(
      Point(0, 10),
      Point(200, 10),
      ColorBlue,   
      ColorYellow);
    Pen bluePen(ColorBlue);

    // 以下的操作是往屏幕上绘制一八卦图形
    Rect ellipseRect(0, 0, 200, 200);
    Rect left(0, 50, 100, 100);
    gcDrawArc(&bluePen,left,180.0f,180.0f);
    Rect right(100, 50, 100, 100);
    gcFillPie(&RtoBBrush, ellipseRect,0.0f,180.0f);
    gcFillPie(&BtoYBrush, ellipseRect,180.0f,180.0f);
    gcFillPie(&RtoBBrush, left,180.0f,180.0f);
    gcFillPie(&BtoYBrush, right,0.0f,180.0f);

    //文本输出 
    SolidBrush solidBrush(ColorBlack);
    FontFamily fontFamily(L"隶书");
    Font font(&fontFamily, 27,
      FontStyleRegular, UnitPixel);
    MyDrawString(graphics, "图元文件示例", font, solidBrush,
      PointF(20.0f, 80.0f));
    //到此，GDI+进行的只是往硬盘中存放图片信息的操作
  }

  //将上面的绘图信息进行回放
  Graphics playbackGraphics(hdc);
  playbackGraphics.Clear(ColorWhite);
  //打开并显示图元文件
  Metafile metaFile2(L"图元文件示例.emf");
  playbackGraphics.DrawImage(&metaFile2, Point(0,0));
  //释放HDC
  playbackGraphics.ReleaseHDC(hdc);
}

void CroppingAndScaling()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap image(L"nemo.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  // 目标显示区域在源图大小的基础上放大1.4倍
  RectF destinationRect(
    width+10, 0.0f, 1.4f* width, 1.4f* height);
  //绘制源图
  gcDrawImage(&image, 0, 0);

  //在目标区域内输出位图
  gcDrawImage(
    &image,
    destinationRect,
    0.f, 0.f,    // 原图左上角
    0.65f*width,      // 仅显示原图宽度的65%部分
    0.65f*height,      // 仅显示原图高度的65%部分
    UnitPixel);
}

void UsingInterpolationMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"eagle.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  //绘制源图
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),  //目标区域 
    0, 0,        //源图左上角坐标
    width,       //源图宽度
    height,      //源图高
    UnitPixel);

  //绘图平面右移
  gcTranslateTransform( width+10,0);
  //最临近插值法(低质量)
  gcSetInterpolationMode(InterpolationModeNearestNeighbor);
  gcDrawImage(
    &image,
    RectF(0.0f, 0.0f, 0.6f*width, 0.6f*height),  //目标区域
    0, 0,        //源图左上角坐标
    width,       //源图宽度
    height,      //源图高
    UnitPixel);

  //绘图平面右移
  gcTranslateTransform( 0.6f*width+10,0);
  // 高质量双线性插值法
  gcSetInterpolationMode(InterpolationModeHighQualityBilinear);
  gcDrawImage(
    &image,
    RectF(0, 0, 0.6f * width, 0.6f * height),  //目标区域
    0, 0,        //源图左上角坐标
    width,       //源图宽度
    height,      //源图高
    UnitPixel);

  //绘图平面右移
  gcTranslateTransform(width*0.6f+10, 0.f);
  // 高质量双三次插值法
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);
  gcDrawImage(
    &image,
    RectF(0, 0, 0.6f * width, 0.6f * height),  //目标区域
    0, 0,        //源图左上角坐标
    width,       //源图宽度
    height,      //源图高
    UnitPixel);
}

void RotateFlip()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //加载图片  
  Bitmap photo(L"nemo2.bmp");

  //得到图片尺寸
  int iWidth = photo.GetWidth();
  int iHeight = photo.GetHeight();
  //绘制原始图片
  gcDrawImage(&photo, 10+photo.GetWidth()+2,
    10, photo.GetWidth(), photo.GetHeight());
  //水平翻转图片 
  photo.RotateFlip(RotateNoneFlipX);  
  //旋转后的图片
  gcDrawImage(&photo, 10, 10, photo.GetWidth(), photo.GetHeight());   
}

void ImageSkewing()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //定义图形的目标显示区域
  Point destination[]=
  {
    Point(200, 20),   //原始图像左上角映射后的坐标
    Point(110, 100), //原始图像右上角映射后的坐标
    Point(250, 30)  //原始图像左下角映射后的坐标
  };
  Bitmap image(L"Stripes.bmp");
  // 绘制原始图像
  gcDrawImage(&image, 0, 0);

  // 绘制基于平行四边形映射后的图像
  gcTranslateTransform(image.GetWidth(),0);
  gcDrawImage(&image, destination, 3);  

}


void Cubeimage()
{
  int WIDTH=200;
  int LEFT=200;
  int TOP=200;

  Graphics &graphics=GetGraphics();
  //使用蓝色做背景色清屏
  gcClear(ColorBlue);

  //设置插值模式：高质量双三次插值法
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);

  //分别装入张贴在立方体三面的图片
  Bitmap face(L"rose.bmp");
  Bitmap top(L"flower.bmp");
  Bitmap right(L"yujinxiang.bmp");

  //重新定义用于张贴在正面的图片坐标
  Point destinationFace[] =
  {
    Point(LEFT,TOP), 
    Point(LEFT+WIDTH, TOP),
    Point(LEFT, TOP+WIDTH)
  };  
  //张贴正面图像
  gcDrawImage(&face,destinationFace,3); 

  //重新定义用于张贴在顶部的图片坐标  
  PointF destinationTop[]=
  {
    PointF(LEFT+WIDTH/2, TOP-WIDTH/2),
    PointF(LEFT+WIDTH/2+WIDTH, TOP-WIDTH/2), 
    PointF(LEFT, TOP)
  };  
  //张贴顶部面图像    
  gcDrawImage(&top, destinationTop, 3); 

  //重新定义用于张贴在右侧的图片坐标        
  Point destinationRight[]=
  {
    Point(LEFT+WIDTH, TOP),
    Point(LEFT+WIDTH/2+WIDTH, TOP-WIDTH/2), 
    Point(LEFT+WIDTH,TOP+WIDTH)
  };
  //张贴右侧面图像            
  gcDrawImage(&right, destinationRight, 3);
}

void ThumbnailImage()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //设置插值模式：高质量双三次插值法
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);

  //加载欲查看缩略图的图片
  Bitmap image(L"flower.bmp");
  //获取当前窗口大小
  Rect client(0,0,
    MyClient.Width,MyClient.Height);

  float width=image.GetWidth();
  float height=image.GetHeight();

  //获取指定大小的缩略图
  Image *pThumbnail = image.GetThumbnailImage(40,40,  NULL, NULL);

  //将缩略图作为画刷
  TextureBrush picBrush(pThumbnail);
  //填充窗口
  gcFillEllipse(&picBrush,client);
}

void Clone()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"head.bmp");
  int Height=image.GetHeight();
  int Width=image.GetWidth();

  //定义将图片切分成四个部分的区域
  RectF block[4]=
  {
    RectF(0,0,Width/2,Height/2),
    RectF(Width/2,0,Width/2,Height/2),
    RectF(0,Height/2,Width/2,Height/2),
    RectF(Width/2,Height/2,Width/2,Height/2)
  };

  //分别克隆图片的四个部分  
  Bitmap *s[4];
  s[0]=image.Clone(block[0],PixelFormatDontCare);
  s[1]=image.Clone(block[1],PixelFormatDontCare);
  s[2]=image.Clone(block[2],PixelFormatDontCare);
  s[3]=image.Clone(block[3],PixelFormatDontCare);
  //绘制图片的四个部分，各部分绘制时间间隔为1秒
  gcDrawImage(s[0],0,0);

  //延时，以达到分块显示的效果
  ::sys_sleep(1000);
  gcDrawImage(s[1],Width/2,0);
  ::sys_sleep(1000);
  gcDrawImage(s[3],Width/2,Height/2);
  ::sys_sleep(1000);
  gcDrawImage(s[2],0,Height/2);
  delete s[0];
  delete s[1];
  delete s[2];
  delete s[3];
}

void Picturescale()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"photo.bmp");

  //定义图片的显示区域
  Rect rect(0,0,image.GetWidth(),image.GetHeight());
  gcDrawImage(&image,rect);

  //局部缩小的区域大小为80*80
  gcTranslateTransform(image.GetWidth()+10,0);
  Rect smallrect(0,0,80,80);
  //局部缩小
  gcDrawImage(&image,smallrect,80,10,106,112,UnitPixel);

  gcTranslateTransform(0,100);
  //局部放大的区域大小为80*80
  Rect largerect(0,0,80,80);
  //绘制放大后的局部图像
  gcDrawImage(&image,largerect,56,101,35,40,UnitPixel);
}

void ImageAttributesSetNoOp()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"ColorTable.bmp");
  int width=image.GetWidth();
  //绘制标准图片  
  gcDrawImage(&image,0,0);

  gcTranslateTransform(image.GetWidth()+10,0);
  ImageAttributes imAtt;

  //构造一个红色转换到绿色的变换矩阵   


  ColorMatrix brushMatrix=
  {
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  //设置色彩校正
  imAtt.SetColorMatrix(
    &brushMatrix, 
    ColorMatrixFlagsDefault, 
    ColorAdjustTypeBitmap);

  //使用色彩校正绘制图片
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标位置
    0, 0, image.GetWidth(), image.GetHeight(),        //源位置
    UnitPixel,
    &imAtt);

  //临时关闭色彩校正
  imAtt.SetNoOp(ColorAdjustTypeBitmap);

  //不使用色彩校正绘制图片红色->红色
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标位置
    0, 0, image.GetWidth(), image.GetHeight(),          //源位置
    UnitPixel,
    &imAtt);

  //撤消对色彩校正的关闭
  imAtt.ClearNoOp(ColorAdjustTypeBitmap);

  //使用色彩校正绘制图片：红色->绿色
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标位置
    0, 0, image.GetWidth(), image.GetHeight(),          //源位置
    UnitPixel,
    &imAtt);
}


void CreateMetaFile()
{
  Graphics &metagraph=GetGraphics();

  //新建一个图元文件
  HDC hdc =metagraph.GetHDC();
  Metafile metaFile1(CWideCharString("ddd.emf"), hdc);
  //使用Metafile对象的地址做为绘图平面
  Graphics *graphics=Graphics::FromImage(&metaFile1);
  graphics->ScaleTransform(0.8f,0.8f);

  //在沿水平方向输出三个椭圆并填充
  graphics->SetSmoothingMode(SmoothingModeHighQuality);
  //红色椭圆
  graphics->DrawEllipse(&Pen(Color.Red,10), Rect(0, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorRed), Rect(0, 0, 75, 95));

  //绿色椭圆
  graphics->DrawEllipse(&Pen(ColorGreen,10), Rect(40, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorGreen), Rect(40, 0, 75, 95));

  //蓝色椭圆
  graphics->DrawEllipse(&Pen(Color.Blue,10), Rect(80, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorBlue), Rect(80, 0, 75, 95));

  // 追加三种色彩的文本
  FontFamily fontFamily(CWideCharString("Arial"));
  Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
  graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

  //红色文字
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorRed), PointF(-80.0f, 0.0f));
  //绿色文字
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorGreen), PointF(-80.0f, font.GetHeight(graphics)));
  //蓝色文字
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorBlue), PointF(-80.0f, font.GetHeight(graphics)*2));
  //释放所有资源。
  //graphics->Dispose();
  //metaFile1.Dispose();
  metagraph.ReleaseHDC(hdc);
  //metagraph.Dispose();
  delete graphics;
}

void SetColorMatrices()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图元文件  
  Metafile image(L"ddd.emf");
  Unit  unit=UnitPixel;
  //获取图片区间
  RectF rect; image.GetBounds(&rect, &unit);

  //不使用任何色彩校正输出图片
  gcDrawImage(&image, 0.0f, 0.0f, rect.Width, rect.Height);
  ImageAttributes  imAtt;

  //定义一个使红色分量递增1.5的矩阵
  ColorMatrix defaultColorMatrix=
  { 
    1.5f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //定义一个使绿色分量递增1.5的矩阵
  ColorMatrix defaultGrayMatrix=
  {  
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.5f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //画笔的彩色色彩信息较校正矩阵：蓝色分量递增1.5的矩阵
  ColorMatrix penColorMatrix= 
  {  
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.5f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //画笔的灰度色矩阵：所有分量递增1.5的矩阵
  ColorMatrix penGrayMatrix=
  {
    1.5f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.5f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.5f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  // 设置默认的彩色及灰度校正矩阵.
  //ColorAdjustTypeDefault:修改所有的色彩信息
  imAtt.SetColorMatrices(
    &defaultColorMatrix,
    &defaultGrayMatrix, 
    ColorMatrixFlagsAltGray,
    ColorAdjustTypeDefault);
  //使用校正矩阵绘制图元文件：校正所有的色彩
  gcTranslateTransform(image.GetWidth()+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt);

  //设置画笔的彩色及灰度色彩校正矩阵
  //ColorAdjustTypePen：修正画笔色彩
  imAtt.SetColorMatrices(
    &penColorMatrix,
    &penGrayMatrix, 
    ColorMatrixFlagsAltGray,
    ColorAdjustTypePen); 
  //在第二行绘制
  gcResetTransform();
  gcTranslateTransform(0,image.GetHeight());
  //使用修正后的画笔绘制图片 
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //清除在画笔上的所有变换
  imAtt.ClearColorMatrix(ColorAdjustTypePen);
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt); 
}

void SetOutputChannelColorProfile()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"car.bmp");
  ImageAttributes imAtt;
  int width = image.GetWidth();
  int height = image.GetHeight();
  Rect rect(0, 0, width, height);
  //绘制原始图片
  gcDrawImage(&image, rect);

  gcTranslateTransform(width,0);
  //设置色彩配置文件
  imAtt.SetOutputChannelColorProfile(
    CWideCharString("kodak_dc.ICM"), ColorAdjustTypeBitmap);

  //使用色彩配置文件输出图片
  gcDrawImage(
    &image,
    rect,  
    0, 0, width, height,          
    UnitPixel,
    &imAtt);
}

void Gammaadjust()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入原始图片
  Bitmap image(L"warrior.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  // 设置字体信息
  Font myFont(L"宋体", 12);
  //设置提示信息的显示区域
  PointF origin(width+10, height+20);
  SolidBrush blackBrush(ColorBlack);

  ImageAttributes imAtt;   
  CString msg;
  //从0-3依次调整输出图片时所使用的Gamma值
  for(float i=0.0f;i<3.0f;i+=0.1f)
  {  
    //绘制原始图片
    gcDrawImage(&image,0,0);

    //设置Gamma值
    imAtt.SetGamma(i,ColorAdjustTypeBitmap);
    //使用修改后Gamma值进行图片输出
    gcDrawImage(
      &image,
      Rect(width+10, 0, width, height),  //目标区域
      0, 0, width, height,           //源区域
      UnitPixel,
      &imAtt);
    msg.Format("正在修改Gamma值，Gamma=%.2f",i);
    //显示当前的Gamma值信息
    MyDrawString(graphics, msg,myFont,blackBrush,origin);
    //延时一秒以便观看效果
    ::sys_sleep(100);
    gcClear(ColorWhite);
    imAtt.ClearGamma();
  }
}

void SetOutputChannel()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入图片
  Bitmap image(L"jieba.bmp");
  //图片的高度
  int width = image.GetWidth();
  int height = image.GetHeight();

  //绘制原始图片
  gcDrawImage(&image, RectF(0, 0, width, height));
  ImageAttributes imAtt;
  //设置色彩输出通道cyan
  imAtt.SetOutputChannel(ColorChannelFlagsC,
    ColorAdjustTypeBitmap);
  //右移，绘制图片
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),  
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //设置色彩输出通道:magenta 
  imAtt.SetOutputChannel(ColorChannelFlagsM, 
    ColorAdjustTypeBitmap);
  //右移，绘制图片
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //设置色彩输出通道:yellow
  imAtt.SetOutputChannel(ColorChannelFlagsY, 
    ColorAdjustTypeBitmap);
  //右移，绘制图片
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //设置色彩输出通道:black
  imAtt.SetOutputChannel(ColorChannelFlagsK, 
    ColorAdjustTypeBitmap);
  //右移，绘制图片
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,            
    UnitPixel,
    &imAtt); 

}

void Colorkey()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //装入前后背景图片
  Bitmap forground(L"grid.bmp");
  Bitmap background(L"snike.bmp");

  int width=background.GetWidth();
  int height=background.GetHeight();
  Rect rect(0,0,width,height);

  //将红色设置成关键色
  ImageAttributes imAtt;
  imAtt.SetColorKey(
    ColorRed,
    ColorRed,
    ColorAdjustTypeBitmap);

  //绘制背景
  gcDrawImage(&background,0,0); 
  //绘制前景
  gcDrawImage(
    &forground, 
    rect,
    0, 0, forground.GetWidth(), forground.GetHeight(),
    UnitPixel,
    &imAtt);

  gcTranslateTransform(width+20,0);
  gcDrawImage(&background,0,0); 
  //清除已经应用的关键色信息
  imAtt.ClearColorKey(ColorAdjustTypeBitmap);   
  ///将蓝色设置成关键色
  imAtt.SetColorKey(
    ColorBlue,
    ColorBlue,
    ColorAdjustTypeBitmap);
  gcDrawImage(&forground, 
    rect,
    0, 0, forground.GetWidth(), forground.GetHeight(),        
    UnitPixel,
    &imAtt);
  gcTranslateTransform(width+20,0);

  //绘制源图
  gcDrawImage(&background,0,0); 
}

void Setthreshold()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"box-2.bmp");
  int Width=image.GetWidth();
  int Height=image.GetHeight();
  //绘制原始图片
  gcDrawImage(&image, 10, 10, Width, Height);

  //将阈值从0到1依次运用
  ImageAttributes imAtt;
  for(float i=0.0f;i<1.0f;i+=0.1f)
  {
    //设置输出图片时使用的阈值
    imAtt.SetThreshold(i, ColorAdjustTypeBitmap);
    //绘制已经使用了阈值的图片
    gcDrawImage(&image,
      Rect(10+Width, 10, Width, Height),  
      0, 0, Width, Height, 
      UnitPixel,
      &imAtt);
    //延时
    ::sys_sleep(1000);
  }
}

void AdjustedPalette()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  //加载图片
  Bitmap image(L"lord-256.bmp");
  //复制图片
  Bitmap image2(L"lord-256.bmp");
  //获取图片使用的调色板信息
  int size=image.GetPaletteSize();
  ColorPalette *palette= (ColorPalette *)new BYTE[size];
  image.GetPalette(palette, size);

  //获取调色板所包含的色彩总数
  int count=palette->Count;
  if(count<1)
  {
    AfxMessageBox("图片无调色板信息可用");
    return;
  }  

  //更改调色板中的每一种色彩信息    
  for(int i=0;i<count;i++)
  {
    Color u(palette->Entries[i]);
    int r=u.GetR()/2;
    int g=u.GetG()/2;
    int b=u.GetB()/2;

    if(r<1)
      r=0;
    if(g<1)
      g=0;
    if(b<1)
      b=0;
    palette->Entries[i]=u.MakeARGB(255, r,g,b);
  }

  //设置图像的新调色板
  image.SetPalette(palette);
  //绘制原图
  gcDrawImage(&image2,0,0);
  //绘制修改后的图片
  gcDrawImage(&image,image.GetWidth()+10,0);
  delete palette;
}

void SetWrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //加载图片
  Bitmap image(L"yueru.bmp");
  ImageAttributes imAtt;

  //设置图片排列方式为WrapModeClamp：图片不进行平铺
  imAtt.SetWrapMode(WrapModeClamp,ColorRed);

  //缩小显示源图
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标区域
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),      //源图片区域
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //设置图片排列方式为WrapModeTileFlipXY：图片在水平和垂直方向上同时翻转
  imAtt.SetWrapMode(WrapModeTileFlipXY);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标区域
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //源图片区域
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //设置图片排列方式为WrapModeTileFlipX：图片在水平上翻转
  imAtt.SetWrapMode(WrapModeTileFlipX);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标区域
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //源图片区域
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //设置图片排列方式为WrapModeTileFlipY：图片在垂直上翻转
  imAtt.SetWrapMode(WrapModeTileFlipY);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //目标区域
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //源图片区域
    UnitPixel,
    &imAtt);
}

void ListAllImageEncoders()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"华文楷体");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  //获取编码器信息
  UINT num, size;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*)new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);

  //输出编码信息
  CString msg;
  for(UINT j = 0; j <num; ++j)
  { 
    CString s;
    s.Format("编码器名称:%s\t文件格式扩展名:%s\t\n", 
      CString(pImageCodecInfo[j].CodecName),
      CString(pImageCodecInfo[j].FilenameExtension));
    msg+=s;
  }
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
  delete pImageCodecInfo;
}

//帮助函数
CString MyStringFromGUID2(const GUID &rguid)
{
  WCHAR strGuid[39];
  StringFromGUID2(rguid, strGuid, 39);
  return CString(strGuid);
}

void ListImageEncoder_Detail()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //获取编码器信息
  UINT num, size;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);
  //查找指定格式文件的编码器信息
  CString msg;
  //查询所有的编码器信息
  for(UINT j = 0; j <num; j++)
  {
    CString s;
    s.Format("开始描述第%d种图形编码信息\n\n",j); msg+=s;
    s.Format("编码标识: %s\n",MyStringFromGUID2(pImageCodecInfo[j].Clsid));  msg+=s;
    s.Format("文件格式标识: %s\n", MyStringFromGUID2(pImageCodecInfo[j].FormatID)); msg+=s;
    s.Format("编码器名称: %s\n", CString(pImageCodecInfo[j].CodecName)); msg+=s;
    s.Format("编码器依存的动态连接库名: %s\n",CString(pImageCodecInfo[j].DllName)); msg+=s;
    s.Format("编码描述: %s\n", CString(pImageCodecInfo[j].FormatDescription)); msg+=s;
    s.Format("编码器对应的文件扩展名: %s\n",CString(pImageCodecInfo[j].FilenameExtension)); msg+=s;
    s.Format("编码器的MIME类型描述: %s\n",CString(pImageCodecInfo[j].MimeType)); msg+=s;
    s.Format("ImageCodecFlags枚举的标记集: %d\n",pImageCodecInfo[j].Flags); msg+=s;
    s.Format("编码器版本: %d\n",pImageCodecInfo[j].Version); msg+=s;

    int sigCount = pImageCodecInfo[j].SigCount;
    s.Format("与编码器对应的编码器签名的数组大小:%d\n", sigCount); msg+=s;
    s.Format("第%d种图形编码信息描述完毕\n\n",j); msg+=s;
    AfxMessageBox(msg);
    msg.Empty();
  }
  delete pImageCodecInfo;

  //将编码器的详细信息写入文件
  //StreamWriter sw= StreamWriter(@"listinfo.txt",false,System.Text.Encoding.Unicode);
  //sw.Write(msg);
  //sw.Close();
  //AfxMessageBox("操作结束，请打开当前目录下的listinfo.txt查看编码器消息");
}

void ListImageDecoder()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"华文楷体");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  //获取编码器信息
  UINT num, size;
  GetImageDecodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageDecoders(num, size, pImageCodecInfo);

  //输出每一个解码器的详细信息
  CString msg;
  for(UINT j = 0; j <num; j++)
  {
    CString s;
    s.Format("解码器名称:%s\t文件格式扩展名:%s\t\n", 
      CString(pImageCodecInfo[j].CodecName),CString(pImageCodecInfo[j].FilenameExtension));
    msg+=s;
  }
  delete pImageCodecInfo;
  //显示信息
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
}

GUID GetEncoderClsid(CString format)
{
  GUID picGUID;
  //获取编码器信息
  UINT num=0, size=0;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);
  //查找指定格式文件的编码器信息
  CWideCharString formatW(format);
  for(UINT i = 0; i <num; i++)
  {  //MimeType：编码方式的具体描述
    if(formatW == CWideCharString(pImageCodecInfo[i].MimeType))
    {
      picGUID= pImageCodecInfo[i].Clsid;
      break;
    }
  }
  delete pImageCodecInfo;
  return picGUID;
}

void GetEncoderParameter()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"宋体");
  Font        myFont(&fontFamily, 16, FontStyleRegular, UnitPixel);

  CString msg;
  // 为了查询将位图保存为JPEG格式的图片，新建一个位图
  Bitmap bitmap(1,1);

  //获取JPEG格式的图像编码器的标识
  GUID encoderClsid;
  encoderClsid=GetEncoderClsid(L"image/jpeg");

  // 获取转换成JPG所需要的具体参数信息
  UINT size=bitmap.GetEncoderParameterListSize(&encoderClsid);
  EncoderParameters *pEncoderParameters= (EncoderParameters *)new BYTE[size];
  bitmap.GetEncoderParameterList(&encoderClsid, size, pEncoderParameters);
  // 查看pEncoderParameters对象中多少个EncoderParameter类
  int count=pEncoderParameters->Count;
  msg.Format("在编码参数数组中有%d个EncoderParameter类\n每个类的详细信息为：\n", count);
                      
  EncoderParameter *pEncoderParameter=pEncoderParameters->Parameter;
  /*分别查看EncoderParameters对象中的
  每一个EncoderParameter对象的成员变量
  GUID、NumberOfValues、Type*/
  for(int i=0;i<count;i++)
  {
    CString s;
    //将GUID值转换成字串 
    s.Format("所需设置的第%d个参数种类(GUID):%s\n", i, MyStringFromGUID2(pEncoderParameter[i].Guid));
    msg+=s;
    //查看在每一个参数种类下，可以设置的参数信息
    s.Format("\t在该参数参数种类下，你可以进行的设置的参数分别如下\n");
    msg+=s;
    s.Format("\t\t变量总数=%d\n", pEncoderParameter[i].NumberOfValues);
    msg+=s;
    s.Format("\t\t参数类型=%d\n", pEncoderParameter[i].Type);
    msg+=s;
  }
  //输出编码参数列表信息
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
  delete pEncoderParameters;
}

HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars);
CString _EncoderParameterCategoryFromGUID(GUID guid)
{
   CONST MAX_CATEGORY_LENGTH = 50;
   WCHAR strParameterCategory[MAX_CATEGORY_LENGTH] = L"";
   if (Ok==EncoderParameterCategoryFromGUID(guid, strParameterCategory, MAX_CATEGORY_LENGTH))
     return CString(strParameterCategory);
   return _T("失败");
}

CString ShowAllEncoderParameters(CString format)
{
  CString outmsg;    
  
  //以位图为例，查看pImageCodecInfo的详细信息  
  Bitmap bitmap(L"head.bmp");

  GUID encoderClsid=GetEncoderClsid(format);
  UINT size=bitmap.GetEncoderParameterListSize(&encoderClsid);
  if (0==size)
  {
    return _T("错误");
  }
  EncoderParameters *encodersarameters= (EncoderParameters *)new BYTE[size];
  bitmap.GetEncoderParameterList(&encoderClsid, size, encodersarameters);

  // 获取编码所需的参数列表
  //获取EncoderParameter对象总数
  int count=encodersarameters->Count;
  outmsg.Format("在EncoderParameters中，有%d个 EncoderParameter对象。\n", 
    count);
  EncoderParameter *pEncoderParameter=encodersarameters->Parameter;

  // 查看每一个EncoderParameter对象信息
  for(int k = 0; k <count;++k)
  {
    //还原GUID信息
    CString strParameterCategory=_EncoderParameterCategoryFromGUID(
      pEncoderParameter[k].Guid);
    CString s;
    s.Format("\t参数种类: %s.\n", strParameterCategory); outmsg+=s;
    s.Format("\t该参数的属性值一共有 %d个\n",
      pEncoderParameter[k].NumberOfValues); outmsg+=s;
    s.Format("\t数据类型%d.\n", pEncoderParameter[k].Type); outmsg+=s;
  } 
  delete encodersarameters;
  //将所有信息导出outmsg
  return outmsg;
}

void GetAllEncoderParameter()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //设置输出信息时使用的字体、画刷
  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"宋体");
  Font  myFont(&fontFamily, 16, FontStyleRegular,UnitPixel);

  //将参数列表详细信息保存到msg之中
  CString msg=ShowAllEncoderParameters(L"image/jpeg");
  //显示参数列表信息
  MyDrawString(graphics, msg, myFont, SolidBrush(ColorBlack), PointF(0,0));
}

void menuItem17()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"snike.bmp");
  RectF rect(0,0,image.GetWidth()/2,image.GetHeight()/2);
  gcDrawImage(&image,rect);

  //将BMP保存为PNG文件,不使用编码参数
  GUID pngGuid = GetEncoderClsid("image/png");
  MyImageSave(image, L"snike.png", &pngGuid, NULL);

  //使用第二种方法设置encoder参数
  MyImageSave(image,L"snike2.png", &pngGuid);
    
  //分别打开两种方法保存的图像
  Bitmap image_png1(L"snike.png");
  Bitmap image_png2(L"snike2.png");

  //绘制PNG格式的图片
  gcTranslateTransform(rect.Width,0);
  gcDrawImage(&image_png1,rect);
  gcTranslateTransform(rect.Width,0);
  gcDrawImage(&image_png2,rect);
}

void SaveBmp2tif()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap myBitmap(L"jieba.bmp");

  // 获取TIFF格式文件的编码信息
  GUID tiffGuid = GetEncoderClsid("image/tiff");

  EncoderParameters params;  //其中已经包含了一个 EncoderParameter
  params.Count=1;
  // 使用LZW压缩方式将图图存为 TIFF文件
  params.Parameter[0].Guid= EncoderCompression;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueCompressionLZW;

  MyImageSave(myBitmap,L"jieba.tif", &tiffGuid, &params);

  delete params.Parameter[0].Value;
}

void SaveBMP2JPG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //打开BMP文件
  Bitmap myBitmap(L"car.bmp");
  //获取显示图片所需要区域
  Rect imgrect(0,0, myBitmap.GetWidth(),myBitmap.GetHeight());

  gcDrawImage(&myBitmap, imgrect);

  //获取JPEG格式的编码方式
  GUID jpegGuid= GetEncoderClsid("image/jpeg");

  //分别设置JPEG文件的图片质量
  //编码参数种类为 Quality,
  EncoderParameters params;  //其中已经包含了一个 EncoderParameter
  params.Count=1;
  params.Parameter[0].Guid= EncoderQuality;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  //设置JPEG图片质量为25级
  ((long*)params.Parameter[0].Value)[0] = 25L;
  MyImageSave(myBitmap,L"car025.jpg", &jpegGuid, &params);

  //设置JPEG图片质量为50级
  ((long*)params.Parameter[0].Value)[0] = 50L;
  MyImageSave(myBitmap,L"car050.jpg", &jpegGuid, &params);

  //设置JPEG图片质量为75级
  ((long*)params.Parameter[0].Value)[0] = 75L;
  MyImageSave(myBitmap,L"car075.jpg", &jpegGuid, &params);

  delete params.Parameter[0].Value;

  //分别显示不同图片质量的JPEG文件
  Bitmap image01(L"car025.jpg");
  //绘图平面右移
  gcTranslateTransform(imgrect.Width+10,0);
  gcDrawImage(&image01,imgrect);

  Bitmap image02(L"car050.jpg");
  //重置绘图平面，下移
  gcResetTransform();

  gcTranslateTransform(0,imgrect.Height+10);
  gcDrawImage(&image02,imgrect);

  Bitmap image05(L"car075.jpg");
  gcTranslateTransform(imgrect.Width+10,0);
  gcDrawImage(&image02,imgrect);
}

void TransformingJPEG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  
  //打开JPEG文件
  Bitmap myBitmap(L"car.jpg");
  //获取显示图片所需要区域
  Rect imgrect(0,0, myBitmap.GetWidth(), myBitmap.GetHeight());
  //绘制原图
  gcDrawImage(&myBitmap,imgrect);

  //获取JPEG格式的编码方式
  GUID jpegGuid= GetEncoderClsid("image/jpeg");

  //分别设置JPEG文件的位置变换信息
  //编码参数种类为 Transformation
  EncoderParameters params;  //其中已经包含了一个 EncoderParameter
  params.Count=1;
  params.Parameter[0].Guid= EncoderTransformation;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueTransformRotate270;

  // 将图片旋转90度后保存
  MyImageSave(myBitmap,L"car0_rotate.jpg", &jpegGuid, &params);

  delete params.Parameter[0].Value;

  //绘制旋转后的图片
  gcTranslateTransform(imgrect.Width,0);
  Bitmap myBitmap2(L"car0_rotate.jpg");
  //获取显示图片所需要区域
  Rect imgrect2(0,0, myBitmap2.GetWidth(), myBitmap2.GetHeight());
  gcDrawImage(&myBitmap2, imgrect2);
}
void MultipleFrameImage()
{
  //装入四张不同格式的图片
  Bitmap multi(L"dog.bmp");
  Bitmap page2(L"dog.gif");
  Bitmap page3(L"cute.jpg");
  Bitmap page4(L"cat.png");
  
  //获取tiff图像格式的编码信息
  GUID tiffGuid = GetEncoderClsid("image/tiff");
  
  EncoderParameters params;  //其中已经包含了一个 EncoderParameter
  params.Count=1;
  //参数类型：SaveFlag
  params.Parameter[0].Guid= EncoderSaveFlag;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueMultiFrame;
  
  //保存第一张图片
  MyImageSave(multi,L"Multiframe.tiff", &tiffGuid, &params);
  
  //保存第二张图片
  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueFrameDimensionPage;
  multi.SaveAdd(&page2, &params);
  
  //保存第三张图片
  multi.SaveAdd(&page3, &params);
  
  //保存第四张图片
  multi.SaveAdd(&page4, &params);

  // 关闭TIFF文件
  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueFlush;
  multi.SaveAdd(&params);

  delete params.Parameter[0].Value;

  AfxMessageBox("操作结束，请打开当前目录下的Multiframe.tiff查看图片添加结果");
}

void GetImageFromMultyFrame()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //将窗口切分成四个部分，用以显示四幅帧图片
  Rect ClientRect(0,0,
    MyClient.Width,MyClient.Height);
  Rect r1(0,0,
    ClientRect.Width/2,ClientRect.Height/2);
  Rect r2(ClientRect.Width/2,0,
    ClientRect.Width/2,ClientRect.Height/2);
  Rect r3(0,
    ClientRect.Height/2,ClientRect.Width/2,ClientRect.Height/2);
  Rect r4(ClientRect.Width/2,
    ClientRect.Height/2,ClientRect.Width/2,ClientRect.Height/2);

  //打开TIF文件
  Bitmap  multi(L"Multiframe.tiff");

  //FrameDimension.Page:子帧图片
  GUID pageGuid = FrameDimensionPage;

  GUID pngGuid = GetEncoderClsid("image/png");

  //显示并保存第一帧图片
  multi.SelectActiveFrame(&pageGuid, 0);
  gcDrawImage(&multi, r1);
  MyImageSave(multi,L"Page0.png", &pngGuid);

  //显示并保存第二帧图片
  multi.SelectActiveFrame(&pageGuid, 1);
  gcDrawImage(&multi,r2);
  MyImageSave(multi,L"Page1.png",&pngGuid);

  ////显示并保存第三帧图片
  multi.SelectActiveFrame(&pageGuid, 2);
  gcDrawImage(&multi, r3);
  MyImageSave(multi,L"Page2.png",&pngGuid);

  ////显示并保存第四帧图片
  multi.SelectActiveFrame(&pageGuid, 3);
  gcDrawImage(&multi, r4);
  MyImageSave(multi,L"Page3.png",&pngGuid);
}

void QueryImage()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  FontFamily  fontFamily(L"宋体");
  Font        myFont(&fontFamily, 16, FontStyleRegular, UnitPixel);

  Bitmap image(L"car.jpg");
  //获取图像的属性项
  UINT size, count=0;
  image.GetPropertySize(&size, &count);
  PropertyItem *propItem= (PropertyItem *)new BYTE[size];
  image.GetAllPropertyItems(size, count, propItem);

  // 图像中共有多少属性名可供查询
  if(count == 0)
  {
    AfxMessageBox("无属性名可供查询");
    return;
  }

  CString tmp;
  for(UINT j = 0; j < count; ++j)
  {
    CString s;
    s.Format("第 %d 个可供查找询的属性名的标记值为（16进制）：%x\n",
      j,propItem[j].id); tmp+=s;
    s.Format("第 %d 个属性名对应的属性项的信息为：\n",j); tmp+=s;

    //获取属性项详细描述信息
    s.Format("\t属性项的长度为%d\n\t数据类型为%d\n",
      propItem[j].length, propItem[j].type); tmp+=s;
  }

  delete propItem;

  //输出图像的属性信息
  MyDrawString(graphics, tmp, myFont, brush, PointF(0,0));
}

void SetProp()
{
  //装入图片以供修改
  Bitmap image(L"car.jpg");

  // 设置图片的作者为Jasmine
  BYTE newWriterValue[] = {(BYTE)'J', (BYTE)'a', (BYTE)'s', (BYTE)'m', (BYTE)'i', (BYTE)'e', 0};

  //获取图像的属性项
  UINT size, count=0;
  image.GetPropertySize(&size, &count);
  PropertyItem *pp= (PropertyItem *)new BYTE[size];
  image.GetAllPropertyItems(size, count, pp);

  //0x13b：该标记值对应图片作者
  pp[0].id=0x13b;
  //属性值长度
  pp[0].length=strlen((const char*)newWriterValue);
  //2：属性值的数据类型为字符串
  pp[0].type=2;
  pp[0].value=newWriterValue;

  //设置图片属性
  image.SetPropertyItem(pp+0);
  GUID jpegGuid= GetEncoderClsid("image/jpeg");
  MyImageSave(image,L"newwriter.jpg", &jpegGuid);

  delete pp;

  //重新加载修改后的图片
  Bitmap image2(L"newwriter.jpg");
  //查看图片的作者信息
  size = image2.GetPropertyItemSize(0x13b);
  pp= (PropertyItem *)new BYTE[size];
  image2.GetPropertyItem(0x13b, size, pp);
  CString msg;
  msg.Format("图片的作者已经更改为\n\n%s",CString((BYTE*)pp->value));
  AfxMessageBox(msg);

  delete pp;
}

void OnCanvas()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"box-2.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  Color color;
  gcDrawImage(&image,
    Rect(0, 0, Width, Height));
  
  //产生随机数序列
  for(int i=0;i<Width-5;i++)
  {
    for( int j=0; j<Height-5;j++)
    {
      int a=rand_Next(1000)%5;
      
      image.GetPixel(i+a, j+a, &color);
      //将该点的RGB值设置成附近五点之内的任一点
      image.SetPixel(i, j, color);
    }
    //动态绘制滤镜效果图
    gcDrawImage(&image,
      Rect(Width, 0, Width, Height));
  }
}

void OnWood()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"box-2.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  Color colorTemp,color2;
  Color color;
  //绘制原图
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  int tmp;
  for(int i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      //根据该点RGB的平均值来确认该点的”明暗”
      int avg;
      avg=(color.GetRed()+color.GetGreen()+color.GetBlue())/3;
      if(avg>=128)
        tmp=255;
      else
        tmp=0;
      colorTemp = Color(255,tmp,tmp,tmp);
      //将计算后的RGB值回写到位图
      image.SetPixel(i, j, colorTemp);
    }
    //动态绘制滤镜效果图
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }

}
//计算两点A、B之间的绝对距离
float fDistance(Point A,Point B)
{
  double i=((A.X-B.X)*(A.X-B.X))+((A.Y-B.Y)*(A.Y-B.Y));
  return (float)sqrt(i);
}

void Flashligt()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  
  Bitmap image(L"sports.bmp");
  int Width = image.GetWidth();
  int Height = image.GetHeight();
  int A=Width/2;
  int B=Height/2;
  //Center:图片中心点，发亮此值会让强光中心发生偏移
  Point Center= Point(A,B);
  //R：强光照射面的半径，即”光晕”
  int R=100;
  Color colorTemp,color2;
  Color color;
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  //依次访问每个像素
  for(int x=0;x<Width;x++)
  {
    for( int y=0; y<Height;y++)
    {
      Point tmp= Point(x,y);
      //如果像素位于”光晕”之内
      if(fDistance(tmp,Center)<R)
      {
        image.GetPixel(x, y, &color);
        int r,g,b;
        //根据该点距离强光中心点的距离，分别让RGB值变量
        //220:亮度增加常量，该值越大，光亮度越强
        float tmp_r=220.0f*(1.0f-fDistance(tmp,Center)/R);
        
        r=color.GetRed()+(int)tmp_r;
        r=max(0,min(r,255));
        
        g=color.GetGreen()+(int)tmp_r;
        g=max(0,min(g,255));

        b=color.GetBlue()+(int)tmp_r;
        b=max(0,min(b,255));
    
        colorTemp = Color(255,(int)r,(int)g,(int)b);
        //将增亮后的像素值回写到位图
        image.SetPixel(x, y, colorTemp);
      }
    }
    //动态绘制滤镜效果图
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }
}

void BlurAndSharpen()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.8f,0.8f);

  Bitmap image(L"snike.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  //image2:进行锐化处理
  Bitmap *image2=image.Clone(0,0,Width,Height,PixelFormatDontCare);

  Color colorTemp;
  Color color[3][3];
  //绘制原图
  gcDrawImage(&image, Rect(0, 0, Width, Height));

  for(int i=1;i<Width-2;i++)
  {
    for( int j=1; j<Height-2;j++)
    {
      //访问周围9个点的RGB值
      image.GetPixel(i-1, j-1, &color[0][0]);
      image.GetPixel(i-1, j, &color[0][1]);
      image.GetPixel(i-1, j+1, &color[0][2]);

      image.GetPixel(i, j-1, &color[1][0]);
      image.GetPixel(i, j, &color[1][1]);
      image.GetPixel(i, j+1, &color[1][2]);

      image.GetPixel(i+1, j-1, &color[2][0]);
      image.GetPixel(i+1, j, &color[2][1]);
      image.GetPixel(i+1, j+1, &color[2][2]);

      int rSum=0;
      int gSum=0;
      int bSum=0;
      //分别求出周围9个点的R、G、B之和
      for(int n=0;n<3;n++)
        for(int nn=0;nn<3;nn++)
        {
          rSum+=color[n][nn].GetR();  
          gSum+=color[n][nn].GetG();  
          bSum+=color[n][nn].GetB();  
        }
      //用RGB的平均值做为当前点的RGB值
      colorTemp = Color(255,(int)(rSum/9),(int)(gSum/9),(int)(bSum/9));
      //将计算后的RGB值回写到位图
      image.SetPixel(i, j,colorTemp);
    }
    //绘制经过平滑处理的效果图
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }

  //进行锐化处理
  Color colorLeft,colornow;
  //常量dep：锐化系数，此值越大，锐化效果越明显
  float dep=0.550f;
  for(i=1;i<Width-1; i++)
  {
    for( int j=1;j<Height-1; j++)
    {
      image2->GetPixel(i, j, &colornow);  
      image2->GetPixel(i-1, j-1, &colorLeft);  

      float r=colornow.GetR()+(colornow.GetR()
        -colorLeft.GetR()*dep);
      r=min(255,max(0,r));

      float g=colornow.GetG()+(colornow.GetG() -colorLeft.GetG()*dep);
      g=min(255,max(0,g));
      float b=colornow.GetB()+(colornow.GetB()-colorLeft.GetB()*dep);
      b=min(255,max(0,b));

      colorTemp = Color(255,(int)r,(int)g,(int)b);

      //将计算后的RGB值回写到位图
      image2->SetPixel(i, j, colorTemp);
    }

    gcDrawImage(image2, Rect(Width*2, 0, Width, Height));
  }
  delete image2;
}


void ShowAllEncoderParameters(ImageCodecInfo*, CString &msg);
HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars);
HRESULT ValueTypeFromULONG(ULONG index, WCHAR* strValueType, UINT maxChars);

void ShowAllEncoder()
{
   UINT  num;        // Number of image encoders
   UINT  size;       // Size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo;

   // How many encoders are there?
   // How big (in bytes) is the array of all ImageCodecInfo obects?
   GetImageEncodersSize(&num, &size);

   // Create a buffer large enough to hold the array of ImageCodecInfo
   // objects that will be returned by GetImageEncoders.
   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

   // GetImageEncoders creates an array of ImageCodecInfo objects
   // and copies that array into a previously allocated buffer. 
   // The third argument, imageCodecInfos, is a pointer to that buffer. 
   GetImageEncoders(num, size, pImageCodecInfo);
  
   // For each ImageCodecInfo object in the array, show all parameters.
   for(UINT j = 0; j < num; ++j)
   {
    CString msg;
    ShowAllEncoderParameters(&(pImageCodecInfo[j]), msg);
    AfxMessageBox(msg);
   }

   free(pImageCodecInfo);
}


VOID ShowAllEncoderParameters(ImageCodecInfo* pImageCodecInfo, CString &msg)
{
   CONST MAX_CATEGORY_LENGTH = 50;
   CONST MAX_VALUE_TYPE_LENGTH = 50;
   WCHAR strParameterCategory[MAX_CATEGORY_LENGTH] = L"";
   WCHAR strValueType[MAX_VALUE_TYPE_LENGTH] = L"";

  CString s(pImageCodecInfo->MimeType);
  s+="\n";
  msg+=s;

   // Create a Bitmap (inherited from Image) object so that we can call
   // GetParameterListSize and GetParameterList.
   Bitmap bitmap(1, 1);

   // How big (in bytes) is the encoder's parameter list?
   UINT listSize = 0; 
   listSize = bitmap.GetEncoderParameterListSize(&pImageCodecInfo->Clsid);
   s.Format("  The parameter list requires %d bytes.\n", listSize);
  msg+=s;

   if(listSize == 0)
      return;

   // Allocate a buffer large enough to hold the parameter list.
   EncoderParameters* pEncoderParameters = NULL;
   pEncoderParameters = (EncoderParameters*)malloc(listSize);

   if(pEncoderParameters == NULL)
      return;

   // Get the parameter list for the encoder.
   bitmap.GetEncoderParameterList(
      &pImageCodecInfo->Clsid, listSize, pEncoderParameters);

   // pEncoderParameters points to an EncoderParameters object, which
   // has a Count member and an array of EncoderParameter objects.
   // How many EncoderParameter objects are in the array?
   s.Format("  There are %d EncoderParameter objects in the array.\n", 
      pEncoderParameters->Count);
  msg+=s;

   // For each EncoderParameter object in the array, list the
   // parameter category, data type, and number of values.
   for(UINT k = 0; k < pEncoderParameters->Count; ++k)
   {
      EncoderParameterCategoryFromGUID(
         pEncoderParameters->Parameter[k].Guid, strParameterCategory, MAX_CATEGORY_LENGTH);

      ValueTypeFromULONG(
         pEncoderParameters->Parameter[k].Type, strValueType, MAX_VALUE_TYPE_LENGTH);

      s.Format("    Parameter[%d]\n", k); msg+=s;
      s.Format("      The category is %s.\n", CString(strParameterCategory)); msg+=s;
      s.Format("      The data type is %s.\n", strValueType); msg+=s;

      s.Format("      The number of values is %d.\n",
      pEncoderParameters->Parameter[k].NumberOfValues); msg+=s;
   } // for

   free(pEncoderParameters);
} // ShowAllEncoderParameters


HRESULT MyStringCchCopy(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc)
{
  return wcsncpy(pszDest, pszSrc, cchDest)? Ok: E_FAIL;
}

HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars)
{
   HRESULT hr = E_FAIL;

   if(guid == EncoderCompression)
      hr = MyStringCchCopy(category, maxChars, L"Compression");
   else if(guid == EncoderColorDepth)
      hr = MyStringCchCopy(category, maxChars, L"ColorDepth");
   else if(guid == EncoderScanMethod)
      hr = MyStringCchCopy(category, maxChars, L"ScanMethod");
   else if(guid == EncoderVersion)
      hr = MyStringCchCopy(category, maxChars, L"Version");
   else if(guid == EncoderRenderMethod)
      hr = MyStringCchCopy(category, maxChars, L"RenderMethod");
   else if(guid == EncoderQuality)
      hr = MyStringCchCopy(category, maxChars, L"Quality");
   else if(guid == EncoderTransformation)
      hr = MyStringCchCopy(category, maxChars, L"Transformation");
   else if(guid == EncoderLuminanceTable)
      hr = MyStringCchCopy(category, maxChars, L"LuminanceTable");
   else if(guid == EncoderChrominanceTable)
      hr = MyStringCchCopy(category, maxChars, L"ChrominanceTable");
   else if(guid == EncoderSaveFlag)
      hr = MyStringCchCopy(category, maxChars, L"SaveFlag");
   else
      hr = MyStringCchCopy(category, maxChars, L"Unknown category");

   return hr;
} // EncoderParameterCategoryFromGUID


HRESULT ValueTypeFromULONG(ULONG index, WCHAR* strValueType, UINT maxChars)
{
   HRESULT hr = E_FAIL;

   WCHAR* valueTypes[] = {
      L"Nothing",                  // 0
      L"ValueTypeByte",            // 1
      L"ValueTypeASCII",           // 2
      L"ValueTypeShort",           // 3
      L"ValueTypeLong",            // 4
      L"ValueTypeRational",        // 5
      L"ValueTypeLongRange",       // 6
      L"ValueTypeUndefined",       // 7
      L"ValueTypeRationalRange"};  // 8

   hr = MyStringCchCopy(strValueType, maxChars, valueTypes[index]);
   return hr;

} // ValueTypeFromULONG

#endif
