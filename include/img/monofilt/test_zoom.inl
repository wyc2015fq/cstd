
int PicZoom0(Image32* Dst,const Image32* Src)
{
  int x, y;
  if (  (0==Dst->width)||(0==Dst->height)
    ||(0==Src->width)||(0==Src->height)) return 0;
  for (x=0;x<Dst->width;++x)
  {
    for (y=0;y<Dst->height;++y)
    {
      long srcx=(x*Src->width/Dst->width);
      long srcy=(y*Src->height/Dst->height);
      Pixels(Dst,Color32,x,y)=Pixels(Src,Color32,srcx,srcy);
    }
  }
  return 0;
}
void PicZoom1(const Image32* Dst,const Image32* Src)
{
  int x, y;
  if (  (0==Dst->width)||(0==Dst->height)
    ||(0==Src->width)||(0==Src->height)) return;
  for (y=0;y<Dst->height;++y)
  {
    for (x=0;x<Dst->width;++x)
    {
      long srcx=(x*Src->width/Dst->width);
      long srcy=(y*Src->height/Dst->height);
      Pixels(Dst,Color32,x,y)=Pixels(Src,Color32,srcx,srcy);
    }
  }
}
void PicZoom2(const Image32* Dst,const Image32* Src)
{
  int x, y;
  if (  (0==Dst->width)||(0==Dst->height)
    ||(0==Src->width)||(0==Src->height)) return;
  //函数能够处理的最大图片尺寸65536*65536
  unsigned long xrIntFloat_16=(Src->width<<16)/Dst->width+1; //16.16格式定点数
  unsigned long yrIntFloat_16=(Src->height<<16)/Dst->height+1; //16.16格式定点数
  //可证明: (Dst->width-1)*xrIntFloat_16<Src->width成立
  
  for (y=0;y<Dst->height;++y)
  {
    for (x=0;x<Dst->width;++x)
    {
      unsigned long srcx=(x*xrIntFloat_16)>>16;
      unsigned long srcy=(y*yrIntFloat_16)>>16;
      Pixels(Dst,Color32,x,y)=Pixels(Src,Color32,srcx,srcy);
    }
  }
}

// 在x的循环中y一直不变，那么可以提前计算与y相关的值; 
//1.可以发现srcy的值和x变量无关，可以提前到x轴循环之前；
//2.展开Pixels函数，优化与y相关的指针计算； 
void PicZoom3(const Image32* Dst,const Image32* Src)
{
  int x, y;
  if (  (0==Dst->width)||(0==Dst->height)
    ||(0==Src->width)||(0==Src->height)) return;
  unsigned long xrIntFloat_16=(Src->width<<16)/Dst->width+1; 
  unsigned long yrIntFloat_16=(Src->height<<16)/Dst->height+1;
  int dst_width=Dst->width;
  Color32* pDstLine=(Color32*)(Dst+1);
  unsigned long srcy_16=0;

  for (y=0;y<Dst->height;++y)
  {
    Color32* pSrcLine=((Color32*)((UInt8*)(Src+1)+Src->byte_width*(srcy_16>>16)));
    unsigned long srcx_16=0;
    for (x=0;x<dst_width;++x)
    {
      pDstLine[x]=pSrcLine[srcx_16>>16];
      srcx_16+=xrIntFloat_16;
    }
    srcy_16+=yrIntFloat_16;
    ((UInt8*&)pDstLine)+=Dst->byte_width;
  }
}

// 每一行的缩放比例是固定的，预先建立一个缩放映射表格
void PicZoom3_Table(const Image32* Dst,const Image32* Src)
{
  int x, y;
  if (  (0==Dst->width)||(0==Dst->height)
    ||(0==Src->width)||(0==Src->height)) return;
  int dst_width=Dst->width;
  unsigned long* SrcX_Table = new unsigned long[dst_width];
  for (x=0;x<dst_width;++x) {
    //生成表 SrcX_Table
    SrcX_Table[x]=(x*Src->width/Dst->width);
  }
  
  Color32* pDstLine=(Color32*)(Dst+1);
  for (y=0;y<Dst->height;++y) {
    unsigned long srcy=(y*Src->height/Dst->height);
    Color32* pSrcLine=((Color32*)((UInt8*)(Src+1)+Src->byte_width*srcy));
    for (x=0;x<dst_width;++x)
      pDstLine[x]=pSrcLine[SrcX_Table[x]];
    ((UInt8*&)pDstLine)+=Dst->byte_width;
  }
  
  delete [] SrcX_Table;
}

#define PixelsIsInPic(IMG, X, Y)   ( (X>=0)&&(X<IMG->width) && (Y>=0)&&(Y<IMG->height) )



//图像旋转 

// 浮点实现版本
//函数假设以原图片的中心点坐标为旋转和缩放的中心
void PicRotary0(const Image32* Dst,const Image32* Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y)
{
  if ( (fabs(ZoomX*Src->width)<1.0e-4) || (fabs(ZoomY*Src->height)<1.0e-4) ) return; //太小的缩放比例认为已经不可见
  double rx0=Src->width*0.5;  //(rx0,ry0)为旋转中心 
  double ry0=Src->height*0.5; 
  for (long y=0;y<Dst->height;++y)
  {
    for (long x=0;x<Dst->width;++x)
    {
      long srcx=(long)((x- move_x-rx0)/ZoomX*cos(RotaryAngle) - (y- move_y-ry0)/ZoomY*sin(RotaryAngle) + rx0) ;
      long srcy=(long)((x- move_x-rx0)/ZoomX*sin(RotaryAngle) + (y- move_y-ry0)/ZoomY*cos(RotaryAngle) + ry0) ;
      if (PixelsIsInPic(Src,srcx,srcy))
        Pixels(Dst,Color32,x,y)=Pixels(Src,Color32,srcx,srcy);
    }
  }
}

void PicRotary1(const Image32* Dst,const Image32* Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y)
{
  if ( (fabs(ZoomX*Src->width)<1.0e-4) || (fabs(ZoomY*Src->height)<1.0e-4) ) return; //太小的缩放比例认为已经不可见
  double rZoomX=1.0/ZoomX;
  double rZoomY=1.0/ZoomY;
  double sinA=sin(RotaryAngle);
  double cosA=cos(RotaryAngle);
  double Ax=(rZoomX*cosA); 
  double Ay=(rZoomX*sinA); 
  double Bx=(-rZoomY*sinA); 
  double By=(rZoomY*cosA); 
  double rx0=Src->width*0.5;  //(rx0,ry0)为旋转中心 
  double ry0=Src->height*0.5; 
  double Cx=(-(rx0+move_x)*rZoomX*cosA+(ry0+move_y)*rZoomY*sinA+rx0);
  double Cy=(-(rx0+move_x)*rZoomX*sinA-(ry0+move_y)*rZoomY*cosA+ry0); 
  
  Color32* pDstLine=(Color32*)(Dst+1);
  double srcx0_f=(Cx);
  double srcy0_f=(Cy);
  for (long y=0;y<Dst->height;++y)
  {
    double srcx_f=srcx0_f;
    double srcy_f=srcy0_f;
    for (long x=0;x<Dst->width;++x)
    {
      long srcx=(long)(srcx_f);
      long srcy=(long)(srcy_f);
      if (PixelsIsInPic(Src,srcx,srcy))
        pDstLine[x]=Pixels(Src,Color32,srcx,srcy);
      srcx_f+=Ax;
      srcy_f+=Ay;
    }
    srcx0_f+=Bx;
    srcy0_f+=By;
    ((UInt8*&)pDstLine)+=Dst->byte_width;
  }
}
//x86的浮点计算单元(FPU)有一条指令"fsincos"可以同时计算出sin和cos值
//定义SinCos函数: 同时计算sin(Angle)和cos(Angle)的内嵌x86汇编函数
void __declspec(naked) __stdcall SinCos(const double Angle,double& sina,double& cosa) 
{ 
  _asm
  {
    fld  qword ptr [esp+4]//Angle   
      mov  eax,[esp+12]//&sina
      mov  edx,[esp+16]//&cosa
      fsincos   
      fstp qword ptr [edx]   
      fstp qword ptr [eax]  
      ret 16
  }
} 

//用定点数代替浮点计算
void PicRotary2(const Image32* Dst,const Image32* Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y)
{
  if ( (fabs(ZoomX*Src->width)<1.0e-4) || (fabs(ZoomY*Src->height)<1.0e-4) ) return; //太小的缩放比例认为已经不可见
  double tmprZoomXY=1.0/(ZoomX*ZoomY);  
  double rZoomX=tmprZoomXY*ZoomY;
  double rZoomY=tmprZoomXY*ZoomX;
  double sinA,cosA;
  SinCos(RotaryAngle,sinA,cosA);
  long Ax_16=(long)(rZoomX*cosA*(1<<16)); 
  long Ay_16=(long)(rZoomX*sinA*(1<<16)); 
  long Bx_16=(long)(-rZoomY*sinA*(1<<16)); 
  long By_16=(long)(rZoomY*cosA*(1<<16)); 
  double rx0=Src->width*0.5;  //(rx0,ry0)为旋转中心 
  double ry0=Src->height*0.5; 
  long Cx_16=(long)((-(rx0+move_x)*rZoomX*cosA+(ry0+move_y)*rZoomY*sinA+rx0)*(1<<16));
  long Cy_16=(long)((-(rx0+move_x)*rZoomX*sinA-(ry0+move_y)*rZoomY*cosA+ry0)*(1<<16)); 
  
  Color32* pDstLine=(Color32*)(Dst+1);
  long srcx0_16=(Cx_16);
  long srcy0_16=(Cy_16);
  for (long y=0;y<Dst->height;++y)
  {
    long srcx_16=srcx0_16;
    long srcy_16=srcy0_16;
    for (long x=0;x<Dst->width;++x)
    {
      long srcx=(srcx_16>>16);
      long srcy=(srcy_16>>16);
      if (PixelsIsInPic(Src,srcx,srcy))
        pDstLine[x]=Pixels(Src,Color32,srcx,srcy);
      srcx_16+=Ax_16;
      srcy_16+=Ay_16;
    }
    srcx0_16+=Bx_16;
    srcy0_16+=By_16;
    ((UInt8*&)pDstLine)+=Dst->byte_width;
  }
}

//优化内部循环的判断函数PixelsIsInPic，将判断展开到内部循环之外，跳过不需要处理的目标像素；
//TRotaryClipData类用于寻找旋转需要处理的边界范围；算法思路是首先寻找原图片中心点对应的；
//那条扫描线，然后依次向上和向下寻找边界；  如果想要更快速的边界寻找算法，可以考虑利用像素的直线
//绘制原理来自动生成边界(有机会的时候再来实现它)；


struct TRotaryClipData{
public:
    long src_width;
    long src_height;
    long dst_width;
    long dst_height;
    long Ax_16; 
    long Ay_16; 
    long Bx_16; 
    long By_16; 
    long Cx_16;
    long Cy_16; 
public:
    long out_dst_up_y;
    long out_dst_down_y;

    long out_src_x0_16;
    long out_src_y0_16;
private:
    long out_dst_up_x0;
    long out_dst_up_x1;
    long out_dst_down_x0;
    long out_dst_down_x1;
public:
    inline long get_up_x0(){ if (out_dst_up_x0<0) return 0;  else return out_dst_up_x0; }
    inline long get_up_x1(){ if (out_dst_up_x1>=dst_width) return dst_width;  else return out_dst_up_x1; }
    inline long get_down_x0(){ if (out_dst_down_x0<0) return 0;  else return out_dst_down_x0; }
    inline long get_down_x1(){ if (out_dst_down_x1>=dst_width) return dst_width;  else return out_dst_down_x1; }

    inline bool is_in_src(long src_x_16,long src_y_16)
    {
         return ( ( (src_x_16>=0)&&((src_x_16>>16)<src_width) )
               && ( (src_y_16>=0)&&((src_y_16>>16)<src_height) ) );
    }
    void find_begin_in(long dst_y,long& out_dst_x,long& src_x_16,long& src_y_16)
    {
        src_x_16-=Ax_16;
        src_y_16-=Ay_16;
        while (is_in_src(src_x_16,src_y_16))
        {
            --out_dst_x;
            src_x_16-=Ax_16;
            src_y_16-=Ay_16;
        }
        src_x_16+=Ax_16;
        src_y_16+=Ay_16;
    }
    bool find_begin(long dst_y,long& out_dst_x0,long dst_x1)
    {
        long test_dst_x0=out_dst_x0-1;
        long src_x_16=Ax_16*test_dst_x0 + Bx_16*dst_y + Cx_16;
        long src_y_16=Ay_16*test_dst_x0 + By_16*dst_y + Cy_16;
        for (long i=test_dst_x0;i<=dst_x1;++i)
        {
            if (is_in_src(src_x_16,src_y_16))
            {
                out_dst_x0=i;
                if (i==test_dst_x0)
                    find_begin_in(dst_y,out_dst_x0,src_x_16,src_y_16);
                if (out_dst_x0<0)
                {
                    src_x_16-=(Ax_16*out_dst_x0);
                    src_y_16-=(Ay_16*out_dst_x0);
                }
                out_src_x0_16=src_x_16;
                out_src_y0_16=src_y_16;
                return true;
            }
            else
            {
                src_x_16+=Ax_16;
                src_y_16+=Ay_16;
            }
        }
        return false;
    }
    void find_end(long dst_y,long dst_x0,long& out_dst_x1)
    {
        long test_dst_x1=out_dst_x1;
        if (test_dst_x1<dst_x0) test_dst_x1=dst_x0;

        long src_x_16=Ax_16*test_dst_x1 + Bx_16*dst_y + Cx_16;
        long src_y_16=Ay_16*test_dst_x1 + By_16*dst_y + Cy_16;
        if (is_in_src(src_x_16,src_y_16))
        {
            ++test_dst_x1;
            src_x_16+=Ax_16;
            src_y_16+=Ay_16;
            while (is_in_src(src_x_16,src_y_16))
            {
                ++test_dst_x1;
                src_x_16+=Ax_16;
                src_y_16+=Ay_16;
            }
            out_dst_x1=test_dst_x1;
        }
        else
        {
            src_x_16-=Ax_16;
            src_y_16-=Ay_16;
            while (!is_in_src(src_x_16,src_y_16))
            {
                --test_dst_x1;
                src_x_16-=Ax_16;
                src_y_16-=Ay_16;
            }
            out_dst_x1=test_dst_x1;
        }
    }

    bool inti_clip(double move_x,double move_y)
    {
        //计算src中心点映射到dst后的坐标
        out_dst_down_y=(long)(src_height*0.5+move_y);
        out_dst_down_x0=(long)(src_width*0.5+move_x);
        out_dst_down_x1=out_dst_down_x0;
        //得到初始out_???
        if (find_begin(out_dst_down_y,out_dst_down_x0,out_dst_down_x1))
            find_end(out_dst_down_y,out_dst_down_x0,out_dst_down_x1);
        out_dst_up_y=out_dst_down_y;
        out_dst_up_x0=out_dst_down_x0;
        out_dst_up_x1=out_dst_down_x1;
        return (out_dst_down_x0<out_dst_down_x1);
    }
    bool next_clip_line_down()
    {
        ++out_dst_down_y;
        if (!find_begin(out_dst_down_y,out_dst_down_x0,out_dst_down_x1)) return false;
        find_end(out_dst_down_y,out_dst_down_x0,out_dst_down_x1);
        return (out_dst_down_x0<out_dst_down_x1);
    }
    bool next_clip_line_up()
    {
        --out_dst_up_y;
        if (!find_begin(out_dst_up_y,out_dst_up_x0,out_dst_up_x1)) return false;
        find_end(out_dst_up_y,out_dst_up_x0,out_dst_up_x1);
        return (out_dst_up_x0<out_dst_up_x1);
    }
};

void PicRotary3_CopyLine(Color32* pDstLine,long dstCount,long Ax_16,long Ay_16,
                        long srcx0_16,long srcy0_16,Color32* pSrcLine,long src_byte_width)
{
    for (long x=0;x<dstCount;++x)
    {
        pDstLine[x]=((Color32*)((CHAR*)pSrcLine+src_byte_width*(srcy0_16>>16)))[srcx0_16>>16];
        srcx0_16+=Ax_16;
        srcy0_16+=Ay_16;
    }
}

void PicRotary3(const Image32* Dst,const Image32* Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y)
{
    if ( (fabs(ZoomX*Src->width)<1.0e-4) || (fabs(ZoomY*Src->height)<1.0e-4) ) return; //太小的缩放比例认为已经不可见
    double tmprZoomXY=1.0/(ZoomX*ZoomY);  
    double rZoomX=tmprZoomXY*ZoomY;
    double rZoomY=tmprZoomXY*ZoomX;
    double sinA,cosA;
    SinCos(RotaryAngle,sinA,cosA);
    long Ax_16=(long)(rZoomX*cosA*(1<<16)); 
    long Ay_16=(long)(rZoomX*sinA*(1<<16)); 
    long Bx_16=(long)(-rZoomY*sinA*(1<<16)); 
    long By_16=(long)(rZoomY*cosA*(1<<16)); 
    double rx0=Src->width*0.5;  //(rx0,ry0)为旋转中心 
    double ry0=Src->height*0.5; 
    long Cx_16=(long)((-(rx0+move_x)*rZoomX*cosA+(ry0+move_y)*rZoomY*sinA+rx0)*(1<<16));
    long Cy_16=(long)((-(rx0+move_x)*rZoomX*sinA-(ry0+move_y)*rZoomY*cosA+ry0)*(1<<16)); 

    TRotaryClipData rcData;
    rcData.Ax_16=Ax_16;
    rcData.Bx_16=Bx_16;
    rcData.Cx_16=Cx_16;
    rcData.Ay_16=Ay_16;
    rcData.By_16=By_16;
    rcData.Cy_16=Cy_16;
    rcData.dst_width=Dst->width;
    rcData.dst_height=Dst->height;
    rcData.src_width=Src->width;
    rcData.src_height=Src->height;
    if (!rcData.inti_clip(move_x,move_y)) return;

    Color32* pDstLine=(Color32*)(Dst+1);
    ((UInt8*&)pDstLine)+=(Dst->byte_width*rcData.out_dst_down_y);
    while (true) //to down
    {
        long y=rcData.out_dst_down_y;
        if (y>=Dst->height) break;
        if (y>=0)
        {
            long x0=rcData.get_down_x0();
            PicRotary3_CopyLine(&pDstLine[x0],rcData.get_down_x1()-x0,Ax_16,Ay_16,
                rcData.out_src_x0_16,rcData.out_src_y0_16,(Color32*)(Src+1),Src->byte_width);
        }
        if (!rcData.next_clip_line_down()) break;
        ((UInt8*&)pDstLine)+=Dst->byte_width;
    }
   
    pDstLine=(Color32*)(Dst+1);
    ((UInt8*&)pDstLine)+=(Dst->byte_width*rcData.out_dst_up_y);
    while (rcData.next_clip_line_up()) //to up
    {
        long y=rcData.out_dst_up_y;
        if (y<0) break;
        ((UInt8*&)pDstLine)-=Dst->byte_width;
        if (y<Dst->height)
        {
            long x0=rcData.get_up_x0();
            PicRotary3_CopyLine(&pDstLine[x0],rcData.get_up_x1()-x0,Ax_16,Ay_16,
                rcData.out_src_x0_16,rcData.out_src_y0_16,(Color32*)(Src+1),Src->byte_width);
        }
    }
}

