# GPS 数据格式加强版 - xqhrs232的专栏 - CSDN博客
2011年12月08日 16:46:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：816标签：[system																[date																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)
个人分类：[车载电子技术](https://blog.csdn.net/xqhrs232/article/category/7465105)
**GPS 数据格式**
**一、  GGA**
Global Positioning System Fix Data. Time, Position and fix related data for a GPS receiver
GPS定位数据，GPS接收机时间、位置和定位描述信息
$--GGA, hhmmss.ss, llll.ll, a, yyyyy.yy,a,x,
xx, x.x, x.x, M, x.x, M,x.x,xxxx*hh
                     1        2  3     4   5  6 7   8  9  10 11  12 13   14  15
1.UTC时间
2.纬度
3.N或S(北纬或南纬)
4.经度
5.E或W(东经或西经) 
6.卫星质量因子
       0 - 未定位
       1 - GPS已定位
       2 - 差分GPS定位
       6 – 位置推测模式定位
7.有效卫星数(00～12)
8.水平精度因子HDOP
9.天线海拔高度
10.天线海拔高度单位
11.海平面相对大地椭球面的高度
12.海平面相对大地椭球面的高度单位
13.差分GPS数据时间(从上次定位开始算起)，DGPS时无
14.差分基准站ID号(0000-1023)
15.校验值
**二、GSA**
GPS DOP and active satellites
GPS DOP和有效卫星
$--GSA, a, a, x, x, x, x,x,x,
x, x, x, x, x, x, x, x, 
x.x, x.x, x.x*hh
          1 2  3 4 5                                    14  15 16  17 18
1.选择模式
       M - 手动强制运行2D或3D模式
       A - 自动切换2D或3D模式
2.定位模式
       1 - 未定位
       2 - 2D定位
       3 - 3D定位
3.第1颗已定位的卫星ID号
4.第2颗已定位的卫星ID号
...
14.第12颗已定位的卫星ID号
15.PDOP 位置精度因子
16.HDOP 水平精度因子
17.VDOP 垂直精度因子
18.校验值
**三、GSV**
Satellites in view
可视卫星
$--GSV, x, x, x, x, x, x,x, ...*hh
          1  2 3 4  5 6 7    8     
1.本次GSV语句的总数目
2.本条GSV语句是本次GSV语句的第几条
3.可视卫星的总数
4.卫星编号
5.卫星仰角(最大90度)
6.卫星方位角(0-359度)
7.C/No值
重复4-7项，每行最多有四颗卫星
8.校验值
**四、RMC**
Recommended Minimum Navigation Information
推荐定位信息
$--RMC, hhmmss.ss, A, llll.ll, a, 
yyyyy.yy, a, x.x, x.x, xxxx, x.x, a*hh
1     2  3   4   5     6  7  8    9   10  11 12
1.UTC时间
2.状态
       A - 已定位
       V - 未定位
3.纬度
4.N或S(北纬或南纬)
5.经度
6.E或W(东经或西经)
7.速率 节
8.方位 度
9.日期 ddmmyy
10.磁偏角 度
11.磁偏角方向，E或W(东或西)
12.校验值
**五、VTG**
Track Made Good and Ground Speed
地面速度信息
$--VTG, x.x, T, x.x, M, x.x, 
N, x.x, K*hh
1  2  3  4  5  6  7  8    9
1.以真北为参考基准的地面航向
2.T = True 真实
3.以磁北为参考基准的地面航向
4.M = Magnetic 磁极
5.地面速率 节
6.N = Knots 节
7.地面速率 公里/小时
8.K = Kilometres Per Hour 公里/小时
9.校验值
六、GLL
Geographic Position – Latitude/Longitude
地理位置――经度/纬度
$--GLL, llll.ll, a, yyyyy.yy, a, hhmmss.ss,A*hh
1   2    3    4     5     6  7
1.纬度
2.N或S(北纬或南纬)
3.经度
4.E或W(东经或西经)
5.UTC时间
6.状态
       A - 已定位
       V - 未定位
7.校验值
七、ZDA
Time & Date – UTC, Day, Month, Year and Local Time Zone
时间和日期－UTC，日，月，年和本地时区
$--ZDA, hhmmss.ss, xx, xx, xxxx, xx,xx*hh
1      2  3   4   5  6  7
1.本地时间
2.本地时区，00到+/-13小时
3.年份
4.月份，00到12
5.日期，00到31
6. UTC时间
7.校验值
 //========================================
备注:
 1>CSDN可下载的DOC文档转载
