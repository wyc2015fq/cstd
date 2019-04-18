# Hough检测直线,圆,椭圆的部分代码 - 深之JohnChen的专栏 - CSDN博客

2006年03月06日 14:24:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：7463


Hough检测直线,圆,椭圆的部分代码

                                                                                                                                     深之JOHNCHEN

1.hough检测直线

typedef struct MAXVALUE{
 int iDist;
 int iAngle;
 int iMax;
}MAXVALUE;

/*
 * 检测直线
 */
void TraceBeeline(int ImageWidth,int ImageHeight,LPBYTE lpSrc,LPBYTE lpDest,int len)
{
// #define pi 3.1415927
 int iMaxAngle = 90;
 int iAngleNumber = 0;
 //最大值
 MAXVALUE MaxValue1;
 memset(&MaxValue1,0,sizeof(MAXVALUE));
 //循环变量
 int i;
 int j;
 int off = 0;
 int iMaxDistance = 0,iDistance = 0;
 iMaxDistance = (int)sqrt(ImageWidth * ImageWidth + ImageHeight * ImageHeight);
 int  *lpTrans = new int[iMaxDistance * iMaxAngle];
 memset(lpTrans,0,iMaxDistance * iMaxAngle * sizeof(int));
    //去掉孤立点
    for(j=1,off=0;j<ImageHeight-1;j++)
  for(i=1;i<ImageWidth-1;i++,off)
  {
   if(1==lpSrc[off])
   {
    if(lpSrc[off-1]==0&&lpSrc[off+1]==0&&lpSrc[off-ImageWidth]==0&&lpSrc[off+ImageWidth]==0 /
     lpSrc[off-ImageWidth-1]==0&&lpSrc[off-ImageWidth+1]==0&&lpSrc[off+ImageWidth-1]==0&&lpSrc[off+ImageWidth+1]==0)
     lpSrc[off]=0;
   }
  }
 //////////////////////////////

 for(j=0;j<ImageHeight;j++)
  for(i=0;i<ImageWidth;i++)
  {
   off = j * ImageWidth + i;
   if(1==lpSrc[off])
   {
    for(iAngleNumber=0;iAngleNumber<iMaxAngle;iAngleNumber++)
    {
     iDistance = (int)fabs(i * cos(iAngleNumber * 2 * PI/180.0) + j * sin(iAngleNumber * 2 * PI/180.0));
     if(iDistance>=0&&iDistance<iMaxDistance)
         lpTrans[iDistance * iMaxAngle + iAngleNumber]++;
    }
   }
  }

 //找到最大值
  for(i = 0;i < iMaxDistance;i++)
   for(iAngleNumber=0;iAngleNumber<iMaxAngle;iAngleNumber++)
   {
    if(lpTrans[i * iMaxAngle + iAngleNumber]>MaxValue1.iMax)
    {
     MaxValue1.iMax = (int)lpTrans[i * iMaxAngle + iAngleNumber];
                    MaxValue1.iAngle = iAngleNumber;
     MaxValue1.iDist = i;
    }
   }

 //  

   for(j=0;j<ImageHeight;j++)
    for(i=0;i<ImageWidth;i++)
    {
     off = j * ImageWidth + i;
                    iDistance = (int)fabs(i * cos(iAngleNumber * 2 * PI/180.0) + j * sin(iAngleNumber * 2 * PI/180.0));
     if(iDistance == MaxValue1.iMax)
      lpDest[off]=1;
    }

 if(lpTrans)
 {
  delete lpTrans;
  lpTrans = NULL;
 }
}

2.hough检测圆
根据园的方程（x－a）×（x－a）＋（y－b）×（y－b）＝R×R，将参数空间增加到a，b，R三维空间.
常用的hough变换计算量太大,然而随机hough变换虽然计算量相对小点,但是准确率没有前者高.现在使用
一种改进的方法:提取边缘图像,然后边缘跟踪,在边缘曲线上进行hough变换检测圆.

double x=0.0,y=0.0,x0=0.0,y0=0.0,cc=0.0,ss=0.0,temp1,temp2;
 int aa=0,bb=0,rr=0,saveaa=0,savebb=0,saverr=0,savexx0=0,saveyy0=0,saveQ=0;
 int pos = ptNumber/3;
    int i = 0,off = 0,k = 0,iBaseFlag = 100,jj = 0;
 int tt = 0,m=0,n=0; 
int firstBase = 5;
 int secendBase = (2*firstBase+1);
 int aaBase = (R.right+R.left)/2 - firstBase,bbBase = (R.bottom+R.top)/2 -firstBase;
 LONG minrr = 0;
 minrr = (R.right - R.left)>(R.bottom - R.top)?(R.bottom - R.top):(R.right - R.left);
    int memsize = (int)(minrr+2*firstBase) * secendBase *secendBase;
 BYTE * lpabr = new BYTE[memsize];
 memset(lpabr,0,sizeof(BYTE)*memsize);
 int maxcount = -1;

/* 局部hough变换检测圆曲线段的一部分*/
 for(k=0;k<ptNumber;k+=pos)
 {
  for(aa=(int)(R.left+R.right)/2 - firstBase;aa<=(int)(R.left+R.right)/2 + firstBase;aa++)
   {
       if(aa<0)
     continue;
    if(aa>ImageWidth)
     break;
    for(bb=(int)(R.top+R.bottom)/2 -firstBase;bb<=(int)(R.top+R.bottom)/2 + firstBase;bb++)
    {
     if(bb<0)
      continue;
     if(bb>ImageHeight)
      break;
     temp1 = (pt[k].x-aa)*(pt[k].x-aa) + (pt[k].y-bb)*(pt[k].y-bb);
         rr = (int)sqrt(temp1);
     if(rr>=10&&rr<=(int)(minrr/2) + firstBase)
     lpabr[rr*secendBase*secendBase+(aa-aaBase)*secendBase+(bb-bbBase)]++; 
    }

   }

 }

 for(rr=10;rr<=(int)(minrr/2) + firstBase;rr++)
  for(aa=(int)(R.left+R.right)/2 - firstBase;aa<=(int)(R.left+R.right)/2 + firstBase;aa++)
  {
   if(aa<0)
    continue;
   if(aa>ImageWidth)
    break;
   for(bb=(int)(R.top+R.bottom)/2 - firstBase;bb<=(int)(R.top+R.bottom)/2 + firstBase;bb++)
   {
    if(bb<0)
     continue;
    if(bb>ImageHeight)
     break;
    if(maxcount<lpabr[rr*secendBase*secendBase+(aa-aaBase)*secendBase+(bb-bbBase)])
    {
     maxcount = lpabr[rr*secendBase*secendBase+(aa-aaBase)*secendBase+(bb-bbBase)];
     saverr = rr;
     saveaa = aa;
     savebb = bb;
    }
   }
  }
if(maxcount>=4)
{
//记录保存中心,半经
     saverr = rr;
     saveaa = aa;
     savebb = bb;

}

3.hough变换检测椭圆的程序
一个MATLAB程序，有些地方你自己修改以下就可以了。
[row col]=size(fedge);
minofa=a;
maxofa=round(row/2);
minofy0=round(col/2)-30;
maxofy0=round(col/2)+30;
minofb=round(col/2)-60;
maxofb=round(col/2);
maxofx=round(row/2);
scalor=4;
H=zeros(floor((maxofa-minofa)/scalor)+1,floor((maxofa-minofa)/scalor)+1,...
floor((maxofy0-minofy0)/scalor)+1,floor((maxofb-minofb)/scalor)+1);
for x=1:maxofx
for y=1:col
temp=fedge(x,y);
if temp==255
for a=minofa:scalor:maxofa
for x0=a:scalor:maxofa
for b=minofb:scalor:maxofb
for y0=minofy0:scalor:maxofy0
temp=((y-y0)/b)^2+((x-x0)/a)^2;
if abs(temp-1)<=0.01
xtemp=floor((x0-minofa)/scalor)+1;
atemp=floor((a-minofa)/scalor)+1;
ytemp=floor((y0-minofy0)/scalor)+1;
btemp=floor((b-minofb)/scalor)+1;
H(xtemp,atemp,ytemp,btemp)=H(xtemp,atemp,ytemp,btemp)+1;
end
end
end
end
end
end
end
end

maxofH=max(max(max(max(H))));
for i=1:floor((maxofa-minofa)/scalor)+1
for j=1:floor((maxofa-minofa)/scalor)+1
for m=1:floor((maxofy0-minofy0)/scalor)+1
for n=1:floor((maxofb-minofb)/scalor)+1
temp=H(i,j,m,n);
if temp==maxofH
xtemp=i;
atemp=j;
ytemp=m;
btemp=n;
break;
end
end
end
end
end

x0=(xtemp-1)*scalor+minofa;
a=(atemp-1)*scalor+minofa;
y0=(ytemp-1)*scalor+minofy0;
b=(btemp-1)*scalor+minofb;
figure;
imshow(fedge,[]);
hold on;
plot(y0,x0,'r+');
t=0:pi/180:2*pi;
plot(y0+b*sin(t),x0+a*cos(t),'r-');
hold off; 

