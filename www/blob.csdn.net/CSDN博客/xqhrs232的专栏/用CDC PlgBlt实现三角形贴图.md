# 用CDC::PlgBlt实现三角形贴图 - xqhrs232的专栏 - CSDN博客
2010年07月08日 10:12:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1574
原文地址::[http://simplesource.blog.163.com/blog/static/1034140620091125533772/](http://simplesource.blog.163.com/blog/static/1034140620091125533772/)
CDC::PlgBlt函数可以实现将资源设备中的一个方形区域映射到目标设备上的一个平行四边形区域中，而这个函数还提供了一个遮罩位图，利用这个功能再加上合适的计算可以实现将资源位图中的任意一个三角形区域映射到目标设备上的任意三角形区域。而这个功能正是3D绘图中最基本的“三角形贴图”。
算法简要思路:
我们要实现的功能是:将资源设备中ABC三角形映射到目标设备中的A’B’C’三角形区域，而我们可以使用的工具PlgBlt只能将资源设备中的方形区域映射到目标设备中的平行四边形区域，如图
![用CDC::PlgBlt实现三角形贴图 - 简单代码 - 简单代码](http://img.bimg.126.net/photo/Rvtudh03FyrqGjfgUME1zw==/4264064422191329435.jpg)
注意到虽然PlgBlt的功能是复制方形区域到一个平行四边形区域，但是我们可以通过在遮罩位图上绘制一个三角形来实现只绘制特定的三角形区域，如图：
![用CDC::PlgBlt实现三角形贴图 - 简单代码 - 简单代码](http://img.bimg.126.net/photo/esHEDGBnL6KAQUhiCfyA1Q==/4264064422191329436.jpg)
可是光这样做还不行，那会导致三角形的映射关系混乱，因为我们只能把资源中方形的的左上角映射到目标设备的A’点，而现在我们在资源设备中选中的A点已经偏离了方形的左上角。
所以我们要重新计算目标设备的映射点a’b’c’，使得资源设备上方形的三个角abc映射到目标设备的a’b’c’时可以保证资源设备的ABC三点可以映射到目标设备的A’B’C’点。如图
![用CDC::PlgBlt实现三角形贴图 - 简单代码 - 简单代码](http://img.bimg.126.net/photo/dM_6Oi0UXNEN3l5U3u4p4A==/4264064422191329437.jpg)[](http://img.bimg.126.net/photo/dM_6Oi0UXNEN3l5U3u4p4A==/4264064422191329437.jpg)
这是一个坐标系转换的过程，很容易联想到OpenGL中的矩阵，我们知道矩阵的作用之一就是坐标系的转换，所以这个过程可以利用矩阵来实现。
　　于是我们需要计算一个转换矩阵T,使得ABC乘以T等于A’B’C’，可以写成如下形式
![用CDC::PlgBlt实现三角形贴图 - 简单代码 - 简单代码](http://img.bimg.126.net/photo/2cBQYjHvcB0vu7uWPth77A==/4264064422191329438.jpg)[](http://img.bimg.126.net/photo/dM_6Oi0UXNEN3l5U3u4p4A==/4264064422191329437.jpg)[](http://img.bimg.126.net/photo/dM_6Oi0UXNEN3l5U3u4p4A==/4264064422191329437.jpg)
之所以加上1，是为了方便矩阵运算，这个与OpenGL中的矩阵是4维的是一个道理。
　　利用选主元消去法解这个方程计算出T。得到T之后就可以计算出a’b’c’点的坐标，计算公式如下
　　a’ = a×T
　　b’ = b×T
　　c’ = c×T
最后用a’b’c’点的坐标作为PlgBlt的参数就可以实现前面所说的“三角形贴图”功能了
程序截图：
![用CDC::PlgBlt实现三角形贴图 - 简单代码 - 简单代码](http://img.bimg.126.net/photo/ut2ORLtW5m5W5MGF38X7AA==/4264064422191329439.jpg)[](http://img.bimg.126.net/photo/dM_6Oi0UXNEN3l5U3u4p4A==/4264064422191329437.jpg)
根据以上原理写的三角形贴图函数：
//
三角形贴图
bool TriangleBlt(
CDC & dc// 目标设备上下文
,
int x0, int y0,int x1,
int y1, int x2, 
int y2// 目标三角形
, CDC & dcSource//资源设备上下文
,
int sx0, int sy0,int sx1,
int sy1,int sx2, 
int sy2// 资源三角形
)
{
//
生成转换矩阵
double A[3][3];
double B[3][3];
double T[3], m;
int minx, miny, maxx, maxy;
minx = sx0 < sx1 ? sx0 : sx1, miny = sy0 < sy1 ? sy0 : sy1;
maxx = sx0 > sx1 ? sx0 : sx1, maxy = sy0 > sy1 ? sy0 : sy1;
if(minx > sx2)
{
minx = sx2;
}
if(maxx < sx2)
{
maxx = sx2;
}
if(miny > sy2)
{
miny = sy2;
}
if(maxy < sy2)
{
maxy = sy2;
}
if(maxx == minx || maxy == miny)
{
returnfalse;
}
A[0][0] = sx0, A[0][1] = sy0, A[0][2] = 1;
A[1][0] = sx1, A[1][1] = sy1, A[1][2] = 1;
A[2][0] = sx2, A[2][1] = sy2, A[2][2] = 1;
B[0][0] = x0, B[0][1] = y0, B[0][2] = 1;
B[1][0] = x1, B[1][1] = y1, B[1][2] = 1;
B[2][0] = x2, B[2][1] = y2, B[2][2] = 1;
int i, j, k;
for(i = 0; i < 3; i++)
{
k = i;
for(j = i + 1; j < 3; j++)
{
if(A[k][i] < A[j][i])
{
k = j;
}
}
if(A[k][i] == 0)
{
returnfalse;
}
//
换行
memcpy(T, A[k],
sizeof(T));
memcpy(A[k], A[i],
sizeof(T));
memcpy(A[i], T,
sizeof(T));
memcpy(T, B[k],
sizeof(T));
memcpy(B[k], B[i],
sizeof(T));
memcpy(B[i], T,
sizeof(T));
// 消去
m = A[i][i];
for(j = 0; j < 3; j++)
{
A[i][j] /= m;
B[i][j] /= m;
}
for(j =0; j < 3; j++)
{
if(j != i)
{
m = A[j][i];
for(k = 0; k < 3; k++)
{
A[j][k] -= A[i][k] * m;
B[j][k] -= B[i][k] * m;
}
}
}
}
//
生成遮罩位图
CBitmap bmpMask;
int w = maxx - minx, h = maxy - miny;
bmpMask.CreateBitmap(w, h, 1, 1, NULL);
CDC dcMask;
dcMask.CreateCompatibleDC(&dc);
dcMask.SelectObject(&bmpMask);
CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
dcMask.SelectObject(&pen);
dcMask.FillSolidRect(0, 0, w, h, RGB(0, 0, 0));
CPoint pts[3];
pts[0].x = sx0 - minx;
pts[0].y = sy0 - miny;
pts[1].x = sx1 - minx;
pts[1].y = sy1 - miny;
pts[2].x = sx2 - minx;
pts[2].y = sy2 - miny;
dcMask.Polygon(pts, 3);
//
绘制
pts[0].x =
int(minx * B[0][0] + miny * B[1][0] + B[2][0]);
pts[0].y =
int(minx * B[0][1] + miny * B[1][1] + B[2][1]);
pts[1].x =
int(maxx * B[0][0] + miny * B[1][0] + B[2][0]);
pts[1].y =
int(maxx * B[0][1] + miny * B[1][1] + B[2][1]);
pts[2].x =
int(minx * B[0][0] + maxy * B[1][0] + B[2][0]);
pts[2].y =
int(minx * B[0][1] + maxy * B[1][1] + B[2][1]);
dc.PlgBlt(pts, &dcSource, minx, miny, w, h, bmpMask, 0, 0);
returntrue;
}
PS:函数只是随手写写的，可能会有很多BUG
PPS:具体程序和源代码将在下一篇博客中给出
