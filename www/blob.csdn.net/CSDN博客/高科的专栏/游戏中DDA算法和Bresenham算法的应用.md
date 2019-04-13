
# 游戏中DDA算法和Bresenham算法的应用 - 高科的专栏 - CSDN博客

2014年12月09日 15:46:13[高科](https://me.csdn.net/pbymw8iwm)阅读数：7390


在角色扮演或即时战略游戏中,经常会将角色以最佳的方式走到指定地点。游戏场景的地面情况复杂,而且场面大,若采用盲目式搜索,例如盲目穷举法,则几乎要遍历整个场景,效率非常低,造成角色反应速度过慢,实践证明是一种不适合网络游戏寻路方法。而启发式搜索算法在障碍较少的情况下也显得效率过低。
DDA算法和Bresenham算法是游戏寻路中绘制直线的两种常用算法。
在列举这两算法之前，我先定义坐标的结构体代码：

```python
struct PixelNode
{
	uint16_t x;
	uint16_t y;
};
```

数值微分画线算法（DDA）法是直线生成算法中最简单的一种，它是一种单步直线生成算法。它的算法是这样的：首先根据直线的斜率确定是以X方向步进还是以Y方向步进，然后沿着步进方向每步进一个点（象素），就沿另一个坐标变量k，k是直线的斜率，因为是对点阵设备输出的，所以需要对每次计算出来的一对坐标进行圆整。
具体算法的实现，除了判断是按照X方向还是按照Y方向步进之外，还要考虑直线的方向，也就是起点和终点的关系。
DDA算法的描述以及原理：
设(x1，y1)和(x2，y2)分别为所求直线的起点和终点坐标，由直线的微分方程得

可通过计算由x方向的增量△x引起y的改变来生成直线：
也可通过计算由y方向的增量△y引起x的改变来生成直线：

选定x2－x1和y2－y1中较大者作为步进方向(假设x2－x1较大)，取该方向上的增量为一个象素单位(△x=1)。
然后利用式(2－1)计算另一个方向的增量(△y=△x·m=m)。通过递推公式(2－2)至(2－5)，把每次计算出的(xi+1，yi+1)
经取整后送到显示器输出，则得到扫描转换后的直线。   之所以取x2－x1和y2－y1中较大者作为步进方向，是考虑沿
着线段分布的象素应均匀，这在下图中可看出。
另外，算法实现中还应注意直线的生成方向，以决定Δx及Δy是取正值还是负值。
所以他的实现步骤是这样描述的：
1、已知直线的两端点坐标：(x1，y1)，(x2，y2)
2、已知画线的颜色：color
3、计算两个方向的变化量：dx=x2－x1                dy=y2－y1
4、求出两个方向最大变化量的绝对值：
steps=max(|dx|，|dy|)
5、计算两个方向的增量(考虑了生成方向)：                xin=dx/steps                yin=dy/steps
6、设置初始象素坐标：x=x1,y=y1
7、用循环实现直线的绘制：

```python
for(i=1；i<=steps；i++)  
{ 
    	putpixel(x，y，color)；/*在(x，y)处，以color色画点*/     
	x=x+xin；       
	y=y+yin；     
}
```

下面给一段code来说明一下这个算法：

```python
bool DDA::CalcLineNodes(int x1, int y1, int x2, int y2,vector<PixelNode>& pointList) { 
	float increx，increy,x,y;
	int steps,i;  
	if(x1 == x2 && y1 == y2){
		return false;
	}
	pointList.clear();
	if(abs(x2 - x1) > abs(y2 - y1)) {  
		steps= abs(x2 - x1);  
	}
	else { 
		steps = abs(y2－y1);
	}	
	increx = (float)(x2 - x1)/steps;  
	increy = (float)(y2 - y1)/steps; 
	x = x1;  
	y = y1;  
	for(i = 1;i <= steps;i ++)  { 
		//putpixel(x, y, color); //在(x，y)处，以color色画点   
		PixelNode p(x,y);
		pointList.push_back(p);
		x+=increx;
		y+=increy;  
   } 
   return true;
}
```
但是实际上数值微分法（DDA法）产生的直线比较精确，而且逻辑简单，易于用硬件实现，但是步进量x，y和k必须用浮点数表示，每一步都要对x或y进行四舍五入后取整，不利于光栅化或点阵输出。生成直线的时间较长。
bresenham算法的特点以及原理：
由直线的斜率确定选择在x方向或y方向上每次递增（减）1个单位，另一变量的递增（减）量为0或1，它取决于实际直线与最近光栅网格点的距离，这个距离的最大误差为0.5。
假定直线斜率k在0~1之间。此时，只需考虑x方向每次递增1个单位，决定y方向每次递增0或1。
设直线当前点为(xi,y)
直线当前光栅点为(xi,yi)
则下一个直线的点应为(xi+1,y+k)下一个直线的光栅点为右光栅点(xi+1,yi)（y方向递增量0）或为右上光栅点(xi+1,yi+1)（y方向递增量1）
记直线与它垂直方向最近的下光栅点的误差为d，有：d=(y+k)–yi，且
0≤d≤1当d<0.5：下一个象素应取右光栅点(xi+1,yi)当d≥0.5：下一个象素应取右上光栅点(xi+1,yi+1)
在这里我给出部分实现的代码：

```python
//返回列表中 包含起始点，不包含目标点
bool Bresenham::CalcLineNodes(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, std::vector<PixelNode>& rNodesVec)
{
	if (x1 == x2 && y1 == y2)
	{
		return false;
	}
	rNodesVec.clear();
    int dx = abs(x2 - x1);//△x 
    int dy = abs(y2 - y1);//△y
    int p = (2 * dy) - dx ; //P = 2△y - △x
    int PointX = x1;//起始坐标X
    int PointY = y1;//起始坐标Y
    int StepX = 0;
	int StepY = 0;
	if (y1 < y2)
	{
		StepY = 1;//y递增
	}
	else//y1 > y2
	{
		StepY = -1;//y递减
	}
	if (x1 < x2)
	{
		StepX = 1;//x递增	
	}
	else
	{
		StepX = -1;//x递减
	}
    // 达到第一个起始点
	PixelNode pt0;
	pt0.x = PointX;
	pt0.y = PointY;
	rNodesVec.push_back(pt0);
    
	if (dx > dy)//横向跨度大，以x为步长
	{
		int twoDy = 2 * dy ; // 2 △y
		int twoDyDx = 2 * (dy - dx) ; // 2(△y - △x)
		while (PointX != x2)
		{
			PointX += StepX;
				
			if (p < 0)
			{
				p += twoDy;
			}
			else
			{
				PointY += StepY;
				p += twoDyDx;
			}
			
			PixelNode pt;
			pt.x = PointX;
			pt.y = PointY;
			rNodesVec.push_back(pt);
		}
		
	}
	else//纵向跨度大，以y为步长
	{
		int twoDx = 2 * dx ; // 2 △x
		int twoDxDy = 2 * (dx - dy) ; // 2(△x - △y)
		
		while (PointY != y2)
		{
			PointY += StepY;
			if (p < 0)
			{
				p += twoDx;
			}
			else
			{
				PointX += StepX;
				p += twoDxDy;
			}
			PixelNode pt;
			pt.x = PointX;
			pt.y = PointY;
			rNodesVec.push_back(pt);
			
		}
	}
	return true;
}
```


