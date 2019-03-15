# 图像滤镜艺术---(Punch Filter)交叉冲印滤镜


  (Punch Filter)交叉冲印滤镜

本文介绍一种交叉冲印效果的代码实现，至于原理，不在累赘，直接看代码：

```csharp
int f_TPunchFilter(unsigned char* srcData, int width, int height, int stride, unsigned char* dstData, int dw, int dh, int dStride)
 {
if(srcData == NULL || dstData == NULL)
return -1;
unsigned char* pSrc = srcData;
unsigned char* pDst = dstData;
int i, j, gray, value, r, g, b;
int offset = stride - width * 4;
int rMap[256], gMap[256], bMap[256];
for(int i = 0;i<256;i++)
{
value = i < 128 ? i : 256 - i;
gray = value * value * value / 16384;
rMap[i] = i < 128 ? gray : 256 - gray;
gray = value * value / 128;
gMap[i] = i < 128 ? gray : 256 - gray;
bMap[i] = i / 2 + 0x25;
}
for(j = 0; j < height; j++)
{
for(i = 0; i < width; i++)
{
b = bMap[pSrc[0]];
g = gMap[pSrc[1]];
r = rMap[pSrc[2]];
pDst[0] = CLIP3(b,0,255);
pDst[1] = CLIP3(g,0,255);
pDst[2] = CLIP3(r,0,255);
pDst[3] = 255;
pSrc+=4;
pDst+=4;
}
pSrc += offset;
pDst += offset;
}
printf("%s","Welcom to our website: www.zealpixel.com");
return 0;
 }
```


效果如下：

[![img](http://www.zealpixel.com/data/attachment/portal/201507/28/163740f2ejdjrn4tfa6jt4.png)](http://www.zealpixel.com/data/attachment/portal/201507/28/163740f2ejdjrn4tfa6jt4.png)、

原图

[![img](http://www.zealpixel.com/data/attachment/portal/201507/28/163735roiwqoh3whh7kqwz.png)](http://www.zealpixel.com/data/attachment/portal/201507/28/163735roiwqoh3whh7kqwz.png)

交叉冲印效果图

最后放上一个完整的C#程序DEMO下载连接：<http://www.zealpixel.com/thread-68-1-1.html>