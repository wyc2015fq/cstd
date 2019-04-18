# RGB彩色空间的不同转换公式 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 13:45:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：10012


在做图像处理时，我们一般采用的是RGB空间，但是在某些特殊情况下，我们也会用到其他的颜色空间。本文主要介绍一些常见的颜色空间的概念和转换公式。

颜色的实质是一种光波。它的存在是因为有三个实体：光线、被观察的对象以及观察者。人眼是把颜色当作由被观察对象吸收或者反射不同波长的光波形成的。例如，当在一个晴朗的日子里，我们看到阳光下的某物体呈现红色时，那是因为该物体吸收了其它波长的光，而把红色波长的光反射到我们人眼里的缘故。当然，我们人眼所能感受到的只是波长在可见光范围内的光波信号。当各种不同波长的光信号一同进入我们的眼睛的某一点时，我们的视觉器官会将它们混合起来，作为一种颜色接受下来。同样我们在对图像进行颜色处理时，也要进行颜色的混合，但我们要遵循一定的规则，即我们是在不同颜色模式下对颜色进行处理的。

1.RGB颜色模式 

　　虽然可见光的波长有一定的范围，但我们在处理颜色时并不需要将每一种波长的颜色都单独表示。因为自然界中所有的颜色都可以用红、绿、蓝(RGB)这三种颜色波长的不同强度组合而得，这就是人们常说的三基色原理。因此，这三种光常被人们称为三基色或三原色。有时候我们亦称这三种基色为添加色(Additive Colors)，这是因为当我们把不同光的波长加到一起的时候，得到的将会是更加明亮的颜色。把三种基色交互重叠，就产生了次混合色：青(Cyan)、洋红(Magenta)、黄(Yellow)。这同时也引出了互补色(Complement Colors)的概念。基色和次混合色是彼此的互补色，即彼此之间最不一样的颜色。例如青色由蓝色和绿色构成，而红色是缺少的一种颜色，因此青色和红色构成了彼此的互补色。在数字视频中，对RGB三基色各进行8位编码就构成了大约16.7万种颜色，这就是我们常说的真彩色。顺便提一句，电视机和计算机的监视器都是基于RGB颜色模式来创建其颜色的。

2.Lab颜色模式 

　　Lab颜色是由RGB三基色转换而来的，它是由RGB模式转换为HSB模式和CMYK模式的桥梁。该颜色模式由一个发光率(Luminance)和两个颜色(a,b)轴组成。它由颜色轴所构成的平面上的环形线来表示颜色的变化，其中径向表示色饱和度的变化，自内向外，饱和度逐渐增高；圆周方向表示色调的变化，每个圆周形成一个色环；而不同的发光率表示不同的亮度并对应不同环形颜色变化线。它是一种具有“独立于设备”的颜色模式，即不论使用任何一种监视器或者打印机，Lab的颜色不变。

RGB=>Lab 
|X|   |0.433910  0.376220  0.189860| |R/255|
|Y| = |0.212649  0.715169  0.072182|*|G/255|
|Z|   |0.017756  0.109478  0.872915| |B/255|

L = 116*Y1/3      for Y>0.008856
L = 903.3*Y      for Y<=0.008856

a = 500*(f(X)-f(Y))
b = 200*(f(Y)-f(Z))
其中  f(t)=t1/3              for t>0.008856
      f(t)=7.787*t+16/116    for t<=0.008856
3.HSB颜色模式 

　　从心理学的角度来看，颜色有三个要素：色泽(Hue)、饱和度(Saturation)和亮度(Brightness)。HSB颜色模式便是基于人对颜色的心理感受的一种颜色模式。它是由RGB三基色转换为Lab模式，再在Lab模式的基础上考虑了人对颜色的心理感受这一因素而转换成的。因此这种颜色模式比较符合人的视觉感受，让人觉得更加直观一些。它可由底与底对接的两个圆锥体立体模型来表示，其中轴向表示亮度，自上而下由白变黑；径向表示色饱和度，自内向外逐渐变高；而圆周方向，则表示色调的变化，形成色环。

RGB=>HSB
V=max(R,G,B)
S=(V-min(R,G,B))*255/V   if V!=0, 0 otherwise

       (G - B)*60/S,  if V=R
H= 180+(B - R)*60/S,  if V=G
   240+(R - G)*60/S,  if V=B

若 H<0，则 H=H+360

使用上面从 0° 到 360° 变化的公式计算色调（ hue）值，确保它们被 2 除后能试用于8位。 

4.YUV颜色模式 

　　这是电视系统中常用的颜色模式，即电视中所谓的分量(Component)信号。该模式由一个亮度信号Y和两个色差信号U、V组成。它是利用了人眼对亮度信号敏感而对色度信号相对不敏感的特点，将RGB颜色通过亮度信号公式Y=039R＋050G＋011B转换为一个亮度信号Y和两个色差分量信号U(R－Y)、V(B－Y)，即对色差信号进行了频带压缩。毫无疑问，这是以牺牲信号的质量为代价的。
RGB<=>YUVY = 0.299R + 0.587G + 0.114B
U = -0.147R - 0.289G + 0.436B
V = 0.615R - 0.515G - 0.100B

R = Y + 1.14V
G = Y - 0.39U - 0.58V
B = Y + 2.03U
5.CMYK颜色模式 

　　这是彩色印刷使用的一种颜色模式。它由青(Cyan)、洋红(Magenta)、黄(Yellow)和黑(Black)四种颜色组成。其中黑色之所以用K来表示，是为避免和RGB三基色中的蓝色(Blue，用B表示)发生混淆。该种模式的创建基础和RGB不同，它不是靠增加光线，而是靠减去光线，因为和监视器或者电视机不同的是，打印纸不能创建光源，它不会发射光线，只能吸收和反射光线。因此通过对上述四种颜色的组合，便可以产生可见光谱中的绝大部分颜色了。

RGB<=CMYK

   R = (255 - C) * ((255 - K) / 255) 
    G = (255 - M) * ((255 - K) / 255) 
    B = (255 - Y) * ((255 - K) / 255) 

6.部分程序code

void HSI2RGB(BYTE &BR,BYTE &BG,BYTE &BB,BYTE BH,BYTE BS,BYTE BI)
{
 int nHValue = static_cast<int>(BH);
 int nSValue = static_cast<int>(BS);
 int nLValue = static_cast<int>(BI);

 float fHAngle = ((float)nHValue ) / 255 * 360;

 float H = fHAngle / 180 * PI;
 float S = ((float)nSValue ) / 100;
 float I = ((float)nLValue ) / 100;

 float R = -1;
 float G = -1;
 float B = -1;

 if(fHAngle >= 0 && fHAngle < 120)
 {
  B = I * ( 1.0 - S );
  R = I * ( 1.0 + ( S * cos( H ) / cos( 60.0 / 180 * PI - H ) ) ); 
  G = 3.0 * I - ( B + R );
 }
 else if(fHAngle >= 120 && fHAngle < 240)
 {
  R = I * ( 1.0 - S );
  G = I * ( 1.0 + S * cos( H - 120.0 / 180 * PI ) / cos( 180.0 / 180 * PI - H )  );
  B = 3.0 * I - ( R + G );
 }
 else if(fHAngle >= 240 && fHAngle < 360)
 {
  G = I * ( 1.0 - S );
  B = I * ( 1.0 + S * cos( H - 240.0 / 180 * PI ) / cos( 300.0 / 180 * PI - H ) );
  R = 3.0 * I - ( G + B );
 }
 int R_value_in_rgb = R * 255;
 int G_value_in_rgb = G * 255;
 int B_value_in_rgb = B * 255;
 BR = static_cast<BYTE>(R_value_in_rgb);
 BG = static_cast<BYTE>(G_value_in_rgb);
 BB = static_cast<BYTE>(B_value_in_rgb);
}

void  RGB2HSI(BYTE r,BYTE g,BYTE b,BYTE &h,BYTE &s,BYTE &i)
{
 short m_fr = static_cast<short>(r);
 short m_fg = static_cast<short>(g);
 short m_fb = static_cast<short>(b);

 float m_fiR = static_cast<float>(m_fr) / 255;
 float m_fsG = static_cast<float>(m_fg) / 255;
 float m_fhB = static_cast<float>(m_fb) / 255;

 if( m_fr == m_fg && m_fg == m_fb)
 {
  int iHValue = 0;
  int iSValue = 0;
  int iLValue = ((float)m_fr)/ 255 * 100;
  h = static_cast<BYTE>(iHValue);
  s = static_cast<BYTE>(iSValue);
  i = static_cast<BYTE>(iLValue);  
  return;
 }
 float max_value_of_rgb = GetMax( m_fiR, m_fsG, m_fhB );
 float min_value_of_rgb = GetMin( m_fiR, m_fsG, m_fhB );
 float fSumRGB =m_fiR + m_fsG + m_fhB ;
 if( fSumRGB <= 0.0 )
  fSumRGB = 0.001;
 float I = (m_fiR + m_fsG + m_fhB) / 3;
 float S = 1.0 - 3.0 * min_value_of_rgb / fSumRGB;
 float H = acos( (( m_fiR - m_fsG ) + ( m_fiR - m_fhB ))/2 / sqrt( ( m_fiR - m_fsG )*( m_fiR - m_fsG ) + ( m_fiR -m_fhB ) * ( m_fsG - m_fhB)  + 0.0001 ) );
 float fHAngle = H / PI * 180;
 if( m_fsG < m_fhB )
  fHAngle = 360 - fHAngle;
 if( fHAngle > 360.0 )
  fHAngle = 360.0;
 int nHValue = fHAngle / 360 * 255;
 int nSValue = S * 100;
 int nLValue = I * 100;
 h = nHValue;
 s = nSValue;
 i = nLValue;
}

