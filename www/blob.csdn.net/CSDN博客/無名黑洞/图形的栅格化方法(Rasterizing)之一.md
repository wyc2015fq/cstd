# 图形的栅格化方法(Rasterizing)之一 - 無名黑洞 - CSDN博客
2009年10月05日 00:02:00[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：3188
图形的栅格化方法也叫光栅化方法，它的作用是把图形变成一个个屏幕上的像素从而显示出来。数学上描述图形用的“点”或“线”是没有大小的。而显示器屏幕是一系列像素组成的正交网格，就好像一个个的格子一样。我们需要把这些“点”“线”用格子描述出来，因此把这个过程称作“栅格化”更加贴切。
首先介绍栅格化图形线（如一条直线）的方法：
**线算法**
最简单的方法是使用x方向的扫描线。
直线方程为
y (x) = m * x + b
泰勒展开为
y (x + dx) = y (x) + f'(x) * x
假设 -1 < m < 1, x0< x1
Line (int x0, int y0, int x1, int y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float m = dy / dx;
    float x = x0, y = y0;
    for ( x = x0; x <= x1; x++)
    {
        setPixel (x, round(y));
        y = y + m;
    }
}
这个方法大致有三个缺点：round函数耗费了很多时间; 使用了float这种浮点算法; 有可能出现锯齿(Aliasing)。
**中点决策法**
有一种避免使用round函数和float的方法，称为中点算法(Midpoint Algorithm)。
此算法在直线方程 f (x, y) = a * x + b * y + c 的基础上定义决策方程(Decision Function) 
d = f (x + 1, y + 0.5) = a * (x + 1) + b * (y + 0.5) + c
以斜率为正(0 < m < 1)举例。若d = 0，表示直线恰好经过右上角和右边的点之间的中点，这时，取右上的点和右边的点均可。若d > 0, 取右上角的点。反之，取右边的点。
所以，我们只需要知道d的值，就可以判断该对哪个点涂色了。为了加快速度，我们并不是每次重新计算d的值，而是在原来d的基础上增加一个delE(当取右边的点)或delNE(当取右上方的点)。可推导：
取delE时有
dd = f (x + 2, y + 0.5) - f (x + 1, y + 0.5) = a
取delNE时有
dd = f (x + 2, y + 1.5) - f (x + 1, y + 0.5) = a + b
初始位置的下一个点的d:
d = f (x0 + 1, y0 + 0.5) = a + b * 0.5  <-- f (x0, y0) = 0 （在直线上）
对d,dd乘以2以避免0.5在程序中出现。
d = 2 * a + b
dd = 2 * a
dd = 2 * (a + b) 
又由 y = -c/b - a/b * x => dy = a/b => a = b * dy
x = -c/a - b/a * y => dx = -b/a　=> b = -a * dx
因为b以及-a都是常数略去不计，则有
a = dy
b = - dx
d = 2 * dy - dx
dd = 2 * dy
dd = 2 * (dy - dx)
Line(int x0, int y0, int x1, int y1)
{
    int dx = x1 – x0; 
    int dy = y1 – y0;
    int d = 2 * dy - dx;
    
    int delE = 2 * dy;
    int delNE = 2 * ( dy - dx );
    
    int x = x0;
    int y = y0;
    
    setPixel(x,y);
    
    while(x < x1)
    {
        if(d<=0)
        {
            d += delE; 
            x = x+1;
        }
        else
        {
            d += delNE; 
            x = x+1; 
            y = y+1;
            setPixel(x,y);
         }
    }
}
**反锯齿法**
反锯齿法与线算法相似， 区别是在涂色的时候用了一个float值代替原来的0或1。
假设 0 < m < 1, x0 < x1:
Line (int x0, int y0, int x1, int y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float m = dy / dx;
    float x = x0, y = y0;
    for ( x = x0; x <= x1; x++)
    {
        int yi = floor(y);
        float f = y - yi;
        setPixel (x, yi, 1 - f);
        setPixel (x, yi + 1, f);
        y = y + m;
    }
}
如果给定一个闭合图形，怎样才能在其内部栅格化呢（如给一个闭合的正方体内部涂上红色）？
