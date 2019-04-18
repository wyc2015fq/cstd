# Apollo项目坐标系研究 - 心纯净，行致远 - CSDN博客





2018年07月11日 18:11:56[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：376








**声明：本文系作者davidhopper原创，未经允许，不得转载！**

百度Apollo项目（[https://github.com/apolloauto](https://github.com/apolloauto)）用到了多种坐标系，其中帮助文档提及的坐标系包括：全球地理坐标系（The Global Geographic coordinate system ）、局部坐标系—东-北-天坐标（The Local Frame – East-North-Up，ENU）、车辆坐标系—右-前-天坐标（The Vehicle Frame —Right-Forward-Up，RFU）。还有一种Frenet坐标系（又称Frenet–Serret公式），Apollo项目文档未提及，但在Apollo项目的规划模块中得到广泛使用。本文首先简介Apollo项目提及的三种坐标系，之后重点介绍Frenet坐标系及其与车辆坐标系之间的转换公式，最后对Apollo项目中Frenet坐标系与车辆坐标系的转换代码进行详细解释。

## 一、Apollo文档提及的坐标系

### （一）全球地理坐标系

Apollo项目使用全球地理坐标系表示高精地图（ the high-definition map，HD Map）中诸元素的几何位置，通常包括：纬度（latitude）、经度（longitude）和海拔（elevation）。全球地理坐标系普遍采用地理信息系统（Geographic Information System，GIS）中用到的WGS-84坐标系（the World Geodetic System dating from 1984），如下图所示，注意海拔定义为椭球体高程（the ellipsoidal height）： 
![1](https://img-blog.csdn.net/20180128145750070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### （二）局部坐标系—东-北-天坐标（ENU）

局部坐标系定义为： 

- X轴：指向东边 

- Y轴：指向北边 

- Z轴：指向天顶 

如下图所示： 
![2](https://img-blog.csdn.net/20180128151107481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ENU局部坐标系采用三维直角坐标系来描述地球表面，实际应用较为困难，因此一般使用简化后的二维投影坐标系来描述。在众多二维投影坐标系中，统一横轴墨卡托（The Universal Transverse Mercator ，UTM）坐标系是一种应用较为广泛的一种。UTM 坐标系统使用基于网格的方法表示坐标，它将地球分为 60 个经度区，每个区包含6度的经度范围，每个区内的坐标均基于横轴墨卡托投影，如下图所示： 
![3](https://img-blog.csdn.net/20180128153319619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### （三）车辆坐标系—右-前-天坐标（RFU）

车辆坐标系定义如下： 

- X轴：面向车辆前方，右手所指方向 

- Y轴：车辆前进方向 

- Z轴：与地面垂直，指向车顶方向 

注意：车辆参考点为后轴中心。 

如下图所示： 
![4](https://img-blog.csdn.net/20180128153927496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 二、Frenet坐标系

Frenet坐标系又称Frenet–Serret公式，Apollo项目文档未提及，但在规划模块中广泛使用。Frenet–Serret公式用于描述粒子在三维欧氏空间 R3ℝ3内沿一条连续可微曲线的运动学特征，如下图所示： 
![5](https://img-blog.csdn.net/20180128155358389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中，T⃗ T→称为切向量（tangent），表示沿曲线运动方向的单位向量；N⃗ N→称为法向量（normal），表示当前曲线运动平面内垂直于T⃗ T→的单位向量；B⃗ B→称为副法向量（binormal），表示同时垂直于T⃗ T→和N⃗ N→的单位向量。 

令r⃗ (t)r→(t)为欧氏空间内随tt改变的一条非退化曲线。所谓非退化曲线就是一条不会退化为直线的曲线，亦即曲率不为0的曲线。令s(t)s(t)是tt时刻时曲线的累计弧长，其定义如下： 

s(t)=∫t0||r′(σ)||dσs(t)=∫0t||r′(σ)||dσ



假定r'≠0r′≠0，则意味着s(t)s(t)是严格单调递增函数。因此可将tt表示为ss的函数，从而有：r⃗ (s)=r⃗ (t(s))r→(s)=r→(t(s))，这样我们就把曲线表示为弧长ss的函数。 

对于采用弧长参数ss表示的非退化曲线r⃗ (s)r→(s)，我们定义其切向量T⃗ T→、法向量N⃗ N→、副法向量B⃗ B→如下： 

T⃗ =dr⃗ ds||dr⃗ ds||T→=dr→ds||dr→ds||



N⃗ =dT⃗ ds||dT⃗ ds||N→=dT→ds||dT→ds||



B⃗ =T⃗ ×N⃗ B→=T→×N→



基于上述定义的Frenet–Serret公式表示为： 

dT⃗ ds=κN⃗ dT→ds=κN→



dN⃗ ds=−κT⃗ +τB⃗ dN→ds=−κT→+τB→



dB⃗ ds=−τN⃗ dB→ds=−τN→



其中κκ、ττ分别表示曲线r⃗ (s)r→(s)的曲率（ curvature）和挠率（ torsion）。直观地讲，曲率是曲线不能形成一条直线的度量值，曲率越趋于0，则曲线越趋近于直线；挠率是曲线不能形成在同一平面内运动曲线的度量值，挠率越趋于0，则曲线越趋近于在同一平面内运动。 

令T⃗ ′=dT⃗ dsT→′=dT→ds，N⃗ ′=dN⃗ dsN→′=dN→ds，B⃗ ′=dB⃗ dsB→′=dB→ds，则Frenet–Serret公式的矩阵表示形式为： 

⎡⎣⎢⎢⎢T⃗ ′N⃗ ′B⃗ ′⎤⎦⎥⎥⎥=⎡⎣⎢0−κ0κ0−τ0τ0⎤⎦⎥⎡⎣⎢⎢T⃗ N⃗ B⃗ ⎤⎦⎥⎥(1)[T→′N→′B→′]=[0κ0−κ0τ0−τ0][T→N→B→](1)



易见参数矩阵是反对称（ skew-symmetric）的。 

对于无人驾驶车辆而言，一般对高度信息不感兴趣，因此可以将车辆运动曲线投影到同一平面内，亦即τ=0τ=0，这样Frenet–Serret公式就可以简化为： 

[T⃗ ′N⃗ ′]=[0−κκ0][T⃗ N⃗ ](2)[T→′N→′]=[0κ−κ0][T→N→](2)



## 三、Frenet坐标系与笛卡尔坐标系的转换公式

为什么要将笛卡尔坐标系转换为Frenet坐标系？因为可以这样可以将车辆的二维运动问题解耦合为两个一维运动问题。显然，一维问题比二维问题容易求解，这就是笛卡尔坐标系转换为Frenet坐标系的必要性。 

前已述及，在不考虑高度信息的前提下，Frenet坐标系可简化为由曲线切向量T⃗ T→与法向量N⃗ N→组成的二维直角坐标系。如下图所示，假定r⃗ (s)r→(s)是参考线（reference line）在弧长ss处的位置，x⃗ x→是当前车辆轨迹（trajectory）点，该向量一般采用笛卡尔坐标系（常用ENU坐标）表示（x⃗ =[x,y,z]Tx→=[x,y,z]T，zz坐标一般忽略），但这里我们采用弧长ss和横向偏移ll（即沿当前参考线位置r⃗ (s)r→(s)法线方向N⃗ rN→r的偏移量）对其描述，即x⃗ =x⃗ (s,l)x→=x→(s,l)。 
![6](https://img-blog.csdn.net/20180129145948315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGF2aWRob3BwZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

令θrθr、T⃗ rT→r、N⃗ rN→r分别为当前参考线r⃗ (s)r→(s)的方位角、单位切向量和单位法向量，θxθx、T⃗ xT→x、N⃗ xN→x分别为当前轨迹点x⃗ (s,l)x→(s,l)的方位角、单位切向量和单位法向量，根据正交基的定义有： 

T⃗ r=[cosθrsinθr]TT→r=[cosθrsinθr]T



N⃗ r=[−sinθrcosθr]TN→r=[−sinθrcosθr]T



T⃗ x=[cosθxsinθx]TT→x=[cosθxsinθx]T



N⃗ x=[−sinθxcosθx]TN→x=[−sinθxcosθx]T



根据平面几何知识易知： 

x⃗ (s,l)=r⃗ (s)+l(s)N⃗ r(s)(3)x→(s,l)=r→(s)+l(s)N→r(s)(3)



从而有（为简洁起见，下面的推导过程均省略参数）： 

lN⃗ TrN⃗ r=N⃗ Tr[r⃗ −x⃗ ]lN→rTN→r=N→rT[r→−x→]



l=N⃗ Tr[x⃗ −r⃗ ]=[x⃗ −r⃗ ]TN⃗ r(4)l=N→rT[x→−r→]=[x→−r→]TN→r(4)



(4)式在数学上美观，但不好编程实现，因此换一个表示方法。设x⃗ x→、r⃗ r→的笛卡尔坐标分别为：(x,y)(x,y)、(xr,yr)(xr,yr)，根据两点间距离公式及N⃗ rN→r的定义，可得： 

l=±(x−xr)2+(y−yr)2−−−−−−−−−−−−−−−−√,if[(y−yr)cosθr−(x−xr)sinθr]>0,positive;otherwisenegetive(4.a)l=±(x−xr)2+(y−yr)2,if[(y−yr)cosθr−(x−xr)sinθr]>0,positive;otherwisenegetive(4.a)



设a为任意一个变量（或向量），记a˙=d(a)dta˙=d(a)dt，a¨=d(a˙)dta¨=d(a˙)dt，a′=d(a)dsa′=d(a)ds，a′′=d(a′)dsa′′=d(a′)ds，根据该约定，有： 

l˙=[x⃗ ˙−r⃗ ˙]TN⃗ r+[x⃗ −r⃗ ]TN⃗ ˙rl˙=[x→˙−r→˙]TN→r+[x→−r→]TN→˙r



根据单位切向量和单位法向量的定义（参见第二部分内容），有： 

x⃗ ˙=d||x⃗ ||dtT⃗ x=vxT⃗ x(5)x→˙=d||x→||dtT→x=vxT→x(5)



r⃗ ˙=d||r⃗ ||dtT⃗ r=s˙T⃗ r(6)r→˙=d||r→||dtT→r=s˙T→r(6)



根据平面几何知识可知： 

x⃗ −r⃗ =lN⃗ r(7)x→−r→=lN→r(7)



根据链式求导法则，N⃗ ˙r=dN⃗ rdsdsdtN→˙r=dN→rdsdsdt，又由二维Frenet–Serret公式可知：N⃗ ′r=−κrT⃗ rN→r′=−κrT→r，于是有： 

N⃗ ˙r=−κrs˙T⃗ r(8)N→˙r=−κrs˙T→r(8)



将(5)-(8)式代入，得： 

l˙=[vxT⃗ x−s˙T⃗ r]TN⃗ r+lN⃗ Tr(−κrs˙T⃗ )rl˙=[vxT→x−s˙T→r]TN→r+lN→rT(−κrs˙T→)r



因为单位切向量和法向量正交，于是有：T⃗ TrN⃗ r=0T→rTN→r=0，N⃗ TrT⃗ r=0N→rTT→r=0，故： 

l˙=vxT⃗ TxN⃗ rl˙=vxT→xTN→r



将T⃗ x=[cosθxsinθx)]TT→x=[cosθxsinθx)]T和 N⃗ r=[−sinθrcosθr]TN→r=[−sinθrcosθr]T代入，得到： 

l˙=vx[−cosθxsinθr+sinθxcosθr]=vxsinΔθ(9)l˙=vx[−cosθxsinθr+sinθxcosθr]=vxsinΔθ(9)



上式中: 

Δθ=θx−θr(10)Δθ=θx−θr(10)



现在来计算vxvx，根据定义：vx=d||x⃗ ||dt=||dx⃗ dt||=||x⃗ ˙||vx=d||x→||dt=||dx→dt||=||x→˙||，下面先计算x⃗ ˙x→˙。由（3）式，有： 

x⃗ ˙=d(r⃗ +lN⃗ r)dt=r⃗ ˙+l˙N⃗ r+lN⃗ ˙rx→˙=d(r→+lN→r)dt=r→˙+l˙N→r+lN→˙r



将(6)式和(8)式代入，得到： 

x⃗ ˙=s˙(1−κr)lT⃗ r+l˙N⃗ r(11)x→˙=s˙(1−κr)lT→r+l˙N→r(11)



于是有： 

vx=||x⃗ ˙||=x⃗ ˙Tx⃗ ˙−−−√=s˙2(1−κr)2l+l˙2−−−−−−−−−−−−−−√(12)vx=||x→˙||=x→˙Tx→˙=s˙2(1−κr)2l+l˙2(12)



下面计算l′l′： 

l′=dds=ddtdtds=l˙s˙l′=dds=ddtdtds=l˙s˙



将(10)式代入，得到： 

l′=vxs˙sinΔθ(13)l′=vxs˙sinΔθ(13)



再将(12)式代入，得到： 

l′=(1−κrl)2+l′2−−−−−−−−−−−−√sinΔθl′=(1−κrl)2+l′2sinΔθ



l′2=[(1−κrl)2+l′2]sin2Δθl′2=[(1−κrl)2+l′2]sin2Δθ



l′2(1−sin2Δθ)=(1−κrl)2sin2Δθl′2(1−sin2Δθ)=(1−κrl)2sin2Δθ



假定车辆实际轨迹一直沿参考线附近运动（即不做与参考线反向的运动），使得： 

|Δθ|<π/2|Δθ|<π/2



1−κrl>01−κrl>0



则求解上述关于l′l′的方程得到： 

l′=(1−κrl)tanΔθ(14)l′=(1−κrl)tanΔθ(14)



将(14)式代入(13)式，可得到速度vxvx的表达式： 

(1−κrl)tanΔθ=vxs˙sinΔθ(1−κrl)tanΔθ=vxs˙sinΔθ



vx=s˙1−κrlcosΔθ(15)vx=s˙1−κrlcosΔθ(15)



令sxsx为车辆当前轨迹x⃗ x→的弧长，则有： 

dds=dsxdsddsx=dsxdtdtdsddsx=vxs˙ddsxdds=dsxdsddsx=dsxdtdtdsddsx=vxs˙ddsx



将(15)式代入，得到： 

dds=1−κrlcosΔθddsx(16)dds=1−κrlcosΔθddsx(16)



对(14)式求关于参考线弧长ss的偏导： 

l′′=(1−κrl)′tanΔθ+(1−κrl)(Δθ)′cos2Δθ(17)l′′=(1−κrl)′tanΔθ+(1−κrl)(Δθ)′cos2Δθ(17)



注意到：Δθ=θx−θrΔθ=θx−θr，于是有： 

(Δθ)′=ddsθx−θ′r(Δθ)′=ddsθx−θr′



根据曲率的定义：κr=θ′r=dθrdsκr=θr′=dθrds，κx=dθxdsxκx=dθxdsx，并将(16)式代入，得到： 

(Δθ)′=1−κrlcosΔθddsxθx−θ′r(Δθ)′=1−κrlcosΔθddsxθx−θr′



(Δθ)′=κx1−κrlcosΔθ−κr(18)(Δθ)′=κx1−κrlcosΔθ−κr(18)



将(18)式代入(17)式，得： 

l′′=−(κ′rl+κrl′)tanΔθ+(1−κrl)cos2Δθ[κx1−κrlcosΔθ−κr](19)l′′=−(κr′l+κrl′)tanΔθ+(1−κrl)cos2Δθ[κx1−κrlcosΔθ−κr](19)



最后求解ax=vx˙=dvxdtax=vx˙=dvxdt。对(15)式求关于时间tt的导数，得： 

ax=s¨1−κrlcosΔθ+s˙dds1−κrlcosΔθs˙ax=s¨1−κrlcosΔθ+s˙dds1−κrlcosΔθs˙



ax=s¨1−κrlcosΔθ+s˙2cosΔθ[(1−κrl)tanΔθ(Δθ)′−(k′rl+krl′)](20)ax=s¨1−κrlcosΔθ+s˙2cosΔθ[(1−κrl)tanΔθ(Δθ)′−(kr′l+krl′)](20)



将(18)式代入(20)式，得： 

ax=s¨1−κrlcosΔθ+s˙2cosΔθ[(1−κrl)tanΔθ[κx1−κrlcosΔθ−κr]−(k′rl+krl′)](21)ax=s¨1−κrlcosΔθ+s˙2cosΔθ[(1−κrl)tanΔθ[κx1−κrlcosΔθ−κr]−(kr′l+krl′)](21)



(4.a)、(14)、(15)、(19)、(21)式便是我们要求的坐标转换公式。



## 四、Apollo项目中Frenet坐标系与笛卡尔坐标系转换代码

函数声明文件planning/math/frame_conversion/cartesian_frenet_conversion.h：

```cpp
#ifndef MODULES_PLANNING_MATH_FRAME_CONVERSION_CARTESIAN_FRENET_CONVERSION_H_
#define MODULES_PLANNING_MATH_FRAME_CONVERSION_CARTESIAN_FRENET_CONVERSION_H_

#include <array>

#include "modules/common/math/vec2d.h"

namespace apollo {
namespace planning {

// Notations:
// s_condition = [s, s_dot, s_ddot]
// s: longitudinal coordinate w.r.t reference line.
// s_dot: ds / dt
// s_ddot: d(s_dot) / dt
// d_condition = [d, d_prime, d_pprime]
// d: lateral coordinate w.r.t. reference line
// d_prime: dd / ds
// d_pprime: d(d_prime) / ds
// l: the same as d.
class CartesianFrenetConverter {
 public:
  CartesianFrenetConverter() = delete;
  /**
   * Convert a vehicle state in Cartesian frame to Frenet frame.
   * Decouple a 2d movement to two independent 1d movement w.r.t. reference
   * line.
   * The lateral movement is a function of longitudinal accumulated distance s
   * to achieve better satisfaction of nonholonomic constraints.
   */
  static void cartesian_to_frenet(const double rs, const double rx,
                                  const double ry, const double rtheta,
                                  const double rkappa, const double rdkappa,
                                  const double x, const double y,
                                  const double v, const double a,
                                  const double theta, const double kappa,
                                  std::array<double, 3>* const ptr_s_condition,
                                  std::array<double, 3>* const ptr_d_condition);
  /**
   * Convert a vehicle state in Frenet frame to Cartesian frame.
   * Combine two independent 1d movement w.r.t. reference line to a 2d movement.
   */
  static void frenet_to_cartesian(const double rs, const double rx,
                                  const double ry, const double rtheta,
                                  const double rkappa, const double rdkappa,
                                  const std::array<double, 3>& s_condition,
                                  const std::array<double, 3>& d_condition,
                                  double* const ptr_x, double* const ptr_y,
                                  double* const ptr_theta,
                                  double* const ptr_kappa, double* const ptr_v,
                                  double* const ptr_a);

  // given sl point extract x, y, theta, kappa
  static double CalculateTheta(const double rtheta, const double rkappa,
                               const double l, const double dl);

  static double CalculateKappa(const double rkappa, const double rdkappa,
                               const double l, const double dl,
                               const double ddl);

  static common::math::Vec2d CalculateCartesianPoint(
      const double rtheta, const common::math::Vec2d& rpoint, const double l);
  /**
   * @brief: given sl, theta, and road's theta, kappa, extract derivative l,
   *second order derivative l:
   */
  static double CalculateLateralDerivative(const double theta_ref,
                                           const double theta, const double l,
                                           const double kappa_ref);

  // given sl, theta, and road's theta, kappa, extract second order derivative
  static double CalculateSecondOrderLateralDerivative(
      const double theta_ref, const double theta, const double kappa_ref,
      const double kappa, const double dkappa_ref, const double l);
};

}  // namespace planning
}  // namespace apollo
```
- 

函数实现文件planning/math/frame_conversion/cartesian_frenet_conversion.cc：

```cpp
#include "modules/planning/math/frame_conversion/cartesian_frenet_conversion.h"

#include <cmath>

#include "modules/common/log.h"
#include "modules/common/math/math_utils.h"

namespace apollo {
namespace planning {

using apollo::common::math::Vec2d;

void CartesianFrenetConverter::cartesian_to_frenet(
    const double rs, const double rx, const double ry, const double rtheta,
    const double rkappa, const double rdkappa, const double x, const double y,
    const double v, const double a, const double theta, const double kappa,
    std::array<double, 3>* const ptr_s_condition,
    std::array<double, 3>* const ptr_d_condition) {
  const double dx = x - rx;
  const double dy = y - ry;

  const double cos_theta_r = std::cos(rtheta);
  const double sin_theta_r = std::sin(rtheta);

  const double cross_rd_nd = cos_theta_r * dy - sin_theta_r * dx;
  // 求解d
  ptr_d_condition->at(0) =
      std::copysign(std::sqrt(dx * dx + dy * dy), cross_rd_nd);

  const double delta_theta = theta - rtheta;
  const double tan_delta_theta = std::tan(delta_theta);
  const double cos_delta_theta = std::cos(delta_theta);

  const double one_minus_kappa_r_d = 1 - rkappa * ptr_d_condition->at(0);
  // 求解d' = dd / ds
  ptr_d_condition->at(1) = one_minus_kappa_r_d * tan_delta_theta;

  const double kappa_r_d_prime =
      rdkappa * ptr_d_condition->at(0) + rkappa * ptr_d_condition->at(1);

  // 求解d'' = dd' / ds
  ptr_d_condition->at(2) =
      -kappa_r_d_prime * tan_delta_theta +
      one_minus_kappa_r_d / cos_delta_theta / cos_delta_theta *
          (kappa * one_minus_kappa_r_d / cos_delta_theta - rkappa);

  // 求解s
  ptr_s_condition->at(0) = rs;
  // 求解ds / dt
  ptr_s_condition->at(1) = v * cos_delta_theta / one_minus_kappa_r_d;

  const double delta_theta_prime =
      one_minus_kappa_r_d / cos_delta_theta * kappa - rkappa;
  // 求解d(ds) / dt
  ptr_s_condition->at(2) =
      (a * cos_delta_theta -
       ptr_s_condition->at(1) * ptr_s_condition->at(1) *
           (ptr_d_condition->at(1) * delta_theta_prime - kappa_r_d_prime)) 
           / one_minus_kappa_r_d;
  return;
}

void CartesianFrenetConverter::frenet_to_cartesian(
    const double rs, const double rx, const double ry, const double rtheta,
    const double rkappa, const double rdkappa,
    const std::array<double, 3>& s_condition,
    const std::array<double, 3>& d_condition, double* const ptr_x,
    double* const ptr_y, double* const ptr_theta, double* const ptr_kappa,
    double* const ptr_v, double* const ptr_a) {
  CHECK(std::abs(rs - s_condition[0]) < 1.0e-6)
      << "The reference point s and s_condition[0] don't match";

  const double cos_theta_r = std::cos(rtheta);
  const double sin_theta_r = std::sin(rtheta);

  *ptr_x = rx - sin_theta_r * d_condition[0];
  *ptr_y = ry + cos_theta_r * d_condition[0];

  const double one_minus_kappa_r_d = 1 - rkappa * d_condition[0];

  const double tan_delta_theta = d_condition[1] / one_minus_kappa_r_d;
  const double delta_theta = std::atan2(d_condition[1], one_minus_kappa_r_d);
  const double cos_delta_theta = std::cos(delta_theta);

  *ptr_theta = common::math::NormalizeAngle(delta_theta + rtheta);

  const double kappa_r_d_prime =
      rdkappa * d_condition[0] + rkappa * d_condition[1];
  *ptr_kappa = (((d_condition[2] + kappa_r_d_prime * tan_delta_theta) *
                 cos_delta_theta * cos_delta_theta) /
                    (one_minus_kappa_r_d) +
                rkappa) *
               cos_delta_theta / (one_minus_kappa_r_d);

  const double d_dot = d_condition[1] * s_condition[1];
  *ptr_v = std::sqrt(one_minus_kappa_r_d * one_minus_kappa_r_d *
                         s_condition[1] * s_condition[1] +
                     d_dot * d_dot);

  const double delta_theta_prime =
      one_minus_kappa_r_d / cos_delta_theta * (*ptr_kappa) - rkappa;

  *ptr_a = s_condition[2] * one_minus_kappa_r_d / cos_delta_theta +
           s_condition[1] * s_condition[1] / cos_delta_theta *
               (d_condition[1] * delta_theta_prime - kappa_r_d_prime);
}

double CartesianFrenetConverter::CalculateTheta(const double rtheta,
                                                const double rkappa,
                                                const double l,
                                                const double dl) {
  return common::math::NormalizeAngle(rtheta + std::atan2(dl, 1 - l * rkappa));
}

double CartesianFrenetConverter::CalculateKappa(const double rkappa,
                                                const double rdkappa,
                                                const double l, const double dl,
                                                const double ddl) {
  double denominator = (dl * dl + (1 - l * rkappa) * (1 - l * rkappa));
  if (std::fabs(denominator) < 1e-8) {
    return 0.0;
  }
  denominator = std::pow(denominator, 1.5);
  const double numerator = rkappa + ddl - 2 * l * rkappa * rkappa -
                           l * ddl * rkappa + l * l * rkappa * rkappa * rkappa +
                           l * dl * rdkappa + 2 * dl * dl * rkappa;
  return numerator / denominator;
}

Vec2d CartesianFrenetConverter::CalculateCartesianPoint(const double rtheta,
                                                        const Vec2d& rpoint,
                                                        const double l) {
  const double x = rpoint.x() - l * std::sin(rtheta);
  const double y = rpoint.y() + l * std::cos(rtheta);
  return Vec2d(x, y);
}

double CartesianFrenetConverter::CalculateLateralDerivative(
    const double rtheta, const double theta, const double l,
    const double rkappa) {
  return (1 - rkappa * l) * std::tan(theta - rtheta);
}

double CartesianFrenetConverter::CalculateSecondOrderLateralDerivative(
    const double rtheta, const double theta, const double rkappa,
    const double kappa, const double rdkappa, const double l) {
  const double dl = CalculateLateralDerivative(rtheta, theta, l, rkappa);
  const double theta_diff = theta - rtheta;
  const double cos_theta_diff = std::cos(theta_diff);
  const double res = -(rdkappa * l + rkappa * dl) * std::tan(theta - rtheta) +
                     (1 - rkappa * l) / (cos_theta_diff * cos_theta_diff) *
                         (kappa * (1 - rkappa * l) / cos_theta_diff - rkappa);
  if (std::isinf(res)) {
    AWARN << "result is inf when calculate second order lateral "
             "derivative. input values are rtheta:"
          << rtheta << " theta: " << theta << ", rkappa: " << rkappa
          << ", kappa: " << kappa << ", rdkappa: " << rdkappa << ", l: " << l
          << std::endl;
  }
  return res;
}
```

参考资料： 

1. [https://github.com/ApolloAuto/apollo/blob/master/docs/specs/coordination.pdf](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/coordination.pdf)

2. [https://en.wikipedia.org/wiki/Frenet%E2%80%93Serret_formulas](https://en.wikipedia.org/wiki/Frenet%E2%80%93Serret_formulas)

3. Optimal trajectory generation for dynamic street scenarios in a Frenét Frame, [http://ieeexplore.ieee.org/document/5509799/citations?tabFilter=papers](http://ieeexplore.ieee.org/document/5509799/citations?tabFilter=papers)



