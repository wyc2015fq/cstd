# Direct3D学习（六）：动画基础（1）动画和运动中的时间 - 逍遥剑客 - CSDN博客
2007年05月24日 04:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3104
**基于时间的动作**
时间单位：ms
速度慢的电脑可以通过丢帧来保证动画的速度
在Windows中读取时间
用timeGetTime()函数，详见MSDN
可以在函数的开头用静态变量来存储时间：void FrameUpdate()
{
  static DWORD LastTime = timeGetTime();
**时间相关的动画**
记录动画的开始时间，然后存储每一帧相对开始时间的偏移量
关键帧的格式，包括时间和变换矩阵：typedef struct sKeyframe {
      DWORD Time;
      D3DMATRIX matTransformation;
} sKeyframe;通过ElapsedTime来决定使用哪一帧
计算出值在0~1之间的一个标量值来确定当前时间相对于前后两个关键帧的位置
float Scalar = (Time - Keyframes[Keyframe].Time)/TimeDiff;
由这个标量值可以插值出当前的变换矩阵：// Calculate the difference in transformations
D3DXMATRIX matInt =                                      
    D3DXMATRIX(Keyframes[Keyframe2].matTransformation) -
    D3DXMATRIX(Keyframes[Keyframe].matTransformation);
matInt *= Scalar; // Scale the difference这样动画就可以平滑地“播放”了
**时间相关的运动**
通常运动是相对于位移的，因此要计算它：float CalcMovement(DWORD ElapsedTime, float PixelsPerSec)
{
  return (PixelsPerSec / 1000.0f * (float)ElapsedTime);
}
**沿轨道的运动**
即不受用户控制沿预定轨道的运动
**直线轨道**
这个简单，知道起点、终点和当前的标量值就可求出：D3DXVECTOR3 vecPos = (vecEnd - vecStart) * Scalar + vecStart;
**曲线轨道**
曲线不只是圆弧，这里我们采用更为广泛的曲线：Bezier曲线
![](http://images.blogcn.com//2007/5/24/4/xoyojank,20070524041408100.JPG)
我们需要4个点来确定曲线的方向和弯曲程度
那么它是怎么画出来的呢？看下面的图：
![](http://images.blogcn.com//2007/5/24/4/xoyojank,20070524041408672.JPG)
对每条线段进行均分，分得越多，曲线越光滑。然后把分割点像这样连接起来：
![](http://images.blogcn.com//2007/5/24/4/xoyojank,20070524041409008.JPG)
Beaier曲线就这样画出来了，HOHO~
计算位置有个公式，怎么推导这里就不介绍了，可以查阅相关资料：
![](http://images.blogcn.com//2007/5/24/4/xoyojank,20070524041409313.JPG)
P0~P3是那4个控制点，s是scalar值
**定义路径**
复杂路径是一系列的轨道的组合，不仅是直线或曲线的问题，有时候是两者的结合
![](http://images.blogcn.com//2007/5/24/4/xoyojank,20070524041409678.JPG)
可以把它定义成类似于链表的形式：enum { PATH_STRAIGHT = 0, PATH_CURVED };
typedef struct {
  DWORD       Type;                          //直线还是曲线
  D3DXVECTOR3 vecStart, vecEnd;         //直点和终点
  D3DXVECTOR3 vecPoint1, vecPoint2;   //如果是曲线，这就是中间那两个控制点
} sPath;
路径用来移动游戏中的角色和其它相关物体是很酷的，比如你的袜子随风飘下
其中一个重要的应用就是用来控制摄像机实现电影中的各种镜头效果
**例子，**NND，编译不了！不说了
**Reference:**
《Advanced Animation With DirectX》
