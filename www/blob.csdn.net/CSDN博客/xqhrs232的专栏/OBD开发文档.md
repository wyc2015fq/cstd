# OBD开发文档 - xqhrs232的专栏 - CSDN博客
2019年02月12日 18:16:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：87
原文地址::[https://blog.csdn.net/fz835304205/article/details/80661104](https://blog.csdn.net/fz835304205/article/details/80661104)
表一  OBD 系统输出信息的模式/服务
模式
服务
Mode 1
请求动力系统当前数据
Mode 2
请求冻结祯数据
Mode 3
请求排放相关的动力系统诊断故障码
Mode 4
清除/复位排放相关的诊断信息
Mode 5
请求氧传感器监测测试结果
Mode 6
请求非连续监测系统 OBD 测试结果
Mode 7
请求连续监测系统 OBD 测试结果
Mode 8
请求控制车载系统，测试或者部件
Mode 9
读车辆和标定识别号
表 2 返回数据字节
A
B
C
D
A7A6A5A4A3A2A1A0
B7B6B5B4B3B2B1B0
C7C6C5C4C3C2C1C0
D7D6D5D4D3D2D1D0
表 3 模式一相关
PID
返回
描述
最小值
最大值
单位
公式
（十
字节
六进
数
制）
00
4
PID 支持（01-20）
01
4
02
2
冻结 DTC
03
2
燃料系统状态
04
1
计算引擎负载值
0
100
%
A*100/255
05
1
发动机冷却液温度
-40
215
°C
A-40
06
1
短时燃油修正(气缸列 1
-100 减
99.22
%
(A - 128)* 100/128
和 3)
去燃料
添加燃
(丰富的
料(精
条件)
益条
件)
07
1
长期燃油修正(气缸列 1
-100 减
99.22
%
(A - 128)* 100/128
和 3)
去燃料
添加燃
(丰富的
料(精
条件)
益条
件)
08
1
短时燃油修正(气缸列 2
-100 减
99.22
%
(A - 128)* 100/128
和 4)
去燃料
添加燃
(丰富的
料(精
条件)
益条
件)
09
1
长期燃油修正(气缸列 2
-100 减
99.22
%
(A - 128)* 100/128
和 4)
去燃料
添加燃
(丰富的
料(精
条件)
益条
件)
0A
1
燃油压力
0
765(计
)
kPa
A*3
0B
1
进气歧管绝对压力
0
255
（绝 对）
kPa
A
0C
2
发动机转速
0
16383
.75
rpm
((A* 256)+ B)/ 4
0D
1
车辆速度
0
255
KM/
H
A
0E
1
第一缸点火正时提前角
-40
63.5
°(相
(A – 128)/ 2
(不包括机械提前)
对于
汽缸
1)
0F
1
进气温度
-40
215
°C
A-40
10
2
空气流量传感器的空气
0
655.3
g/s
((A* 256)+ B)/ 100
流量
5
11
1
绝对节气门位置
0
100
%
A* 100/255
12
1
二次空气状态指令
13
1
氧传感器位置
[A0…A3)= =  行 1,
传感器 1 - 4。 [A4…A7)= =行
2…
14
1B
-
2
传统 0 到 1V 氧传感器
输出电压(Bx-Sy)及与此 传感器关联的短时燃油 修正(Bx-Sy)
0
-100
1.275
99.2
V
%
A/200
(B - 128)*100/128
(如果 B 传感器不
用于修正计算)
1C
1
OBD 系统的车辆设计要
求
1D
1
氧传感器的位置
类似于 PID 13，
但
[A0 …A7]==[B1S 1、B1S2
B2S1,B2S2,B3S1
,B3S2,B4S1,B4S 2]
1E
1
辅助输入状态
A0==动力输出
状态 1==活跃

未用 A1-A7
1F
2
自发动机起动的时间
0
65535
s
(A* 256)+ B
20
4
pid 支持(21 - 40)
21
2
在 MIL 激活状态下行驶
的里程
0
65535
KM
(A* 256)+ B
22
2
相对于歧管真空度的油
0
5177.
kPa
((A* 256)+ B)*
轨压力
265
0.079
23
2
相对于大气压力的油轨
0
65535
kPa(j
((A* 256)+ B)* 10
压力
0
计)
24
4
O2S1_WR_lambda(1):
0
1.999
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535 or
的等效比(lambda)和电
0
7.999
V
((A*256)+B)/3276
压
8
((C*256)+D)*8/65
535 or
((C*256)+D)/8192
25
4
O2S2_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
26
4
O2S3_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
27
4
O2S4_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
28
4
O2S5_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
29
4
O2S6_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
2A
4
O2S7_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
2B
4
O2S8_WR_lambda(1):
0
2
N/A
((A*256)+B)*2/65
线性或宽带式氧传感器
535
的等效比(lambda)和电
0
8
V
((C*256)+D)*8/65
压
535
2C
1
EGR 指令开度
0
100
%
A*100/255
2D
1
EGR 开度误差 (实际开
度 — 指令开度)/指令 开度*100%
-100
99.00
%
(A-128) * 100/128
2E
1
蒸发冲洗控制指令
0
100
%
A*100/255
2F
1
燃油液位输入
0
100
%
A*100/255
30
1
自故障码被清除之后经
历的暖机循环个数
0
255
N/A
A
31
2
自故障码被清除之后的
行驶里程
0
65535
Km
(A*256)+B
32
2
蒸发系统的蒸气压力
-8192
8192
Pa
((A*256)+B)/4
33
1
大气压
0
255
kPa
A
34-
4
线性或宽带式氧传感器
0
2
N/A
((A*256)+B)/32,76
3B
的等效比(lambda)和电
8
流
-128
128
mA
((C*256)+D)/256 -
128
3C
2
催化器温度 B1S1
-40
6513.
°C
((A*256)+B)/10 -
5
40
3D
2
催化器温度 B2S1
-40
6513.
°C
((A*256)+B)/10 -
5
40
3E
2
催化器温度 B1S2
-40
6513.
°C
((A*256)+B)/10 -
5
40
3F
2
催化器温度 B2S2
-40
6513.
°C
((A*256)+B)/10 -
5
40
40
4
支持 PID[41-60]
41
4
当前驾驶循环的监测状
态
42
2
控制模块电压
0
65.53
5
V
((A*256)+B)/1000
43
2
绝对负荷值
0
25700
%
((A*256)+B)*100/
255
44
2
等效比指令
0
2
N/A
((A*256)+B)/3276
8
45
1
相对节气门位置
0
100
%
A*100/255
46
1
环境空气温度
-40
215
°C
A-40
47
1
绝对节气门位置 B
0
100
%
A*100/255
48
1
绝对节气门位置 C
0
100
%
A*100/255
49
1
加速踏板位置 D
0
100
%
A*100/255
4A
1
加速踏板位置 E
0
100
%
A*100/255
4B
1
加速踏板位置 F
0
100
%
A*100/255
4C
1
节气门执行器控制指令
0
100
%
A*100/255
4D
2
MIL 处于激活状态下的
发动机运转时间
0
65535
Minu
ties
(A*256)+B
4E
2
自故障码清除之后的时
间
0
65535
Minu
ties
(A*256)+B
4F
4
等效比的最大值及对应
0,
255,
V
A,
的氧传感器电压
0,
255,
B,
0,
255,
mA
C,
0
2550
kPa
D*10
50
4
来自空气流量传感器的
最大流量
0
2550
g/s
A*10,;B, C,和 D
预留使用
51
1
当前车辆使用的燃料类
型
见燃料表
52
1
酒精在燃料的百分比
0
100
%
A*100/255
53
2
蒸发系统蒸气压力绝对
0
327.6
kPa
((A*256)+B)/200
值
75
54
2
蒸发系统蒸气压力
-32767
32768
Pa
((A*256)+B)-
32767
55
2
第二个氧传感器的短时
燃油修正(Bank 1 和
Bank 3)
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
56
2
第二个氧传感器的短时
燃油修正(Bank 1 和
Bank 3)
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
57
2
第二个氧传感器的短时
燃油修正(Bank 2 和
Bank 4)
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
58
2
第二个氧传感器的短时
燃油修正(Bank 2 和
Bank 4)
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
59
2
油轨绝对压力
0
65535
0
kPa
((A*256)+B) * 10
5A
1
相对加速踏板位置
0
100
%
A*100/255
5B
1
混合动力电池组剩余的
生命
0
100
%
A*100/255
5C
1
机油温度
-40
210
°C
A-40
5D
2
燃油喷射定时
-210.00
301.9
°
(((A* 256)+ B)-
92
26880)/ 128
5E
2
发动机燃料消耗率
0
3212.
L/h
((A* 256)+ B)*
75
0.05
5F
1
排放车辆设计需求
表4 模式 2 相关
PID
（十 六进 制）
返回
字节 数
描述
最小值
最大值
单位
公式
02
2
DTC 导致冻结帧被存储
BDC 编码
表5 模式 3 相关
PID
（十 六进 制）
返回
字节 数
描述
最小值
最大值
单位
公式
N/A
N*6
请求故障代码
每个消息帧有三个
码
表6 模式 4 相关
PID
（十 六进 制）
返回
字节 数
描述
最小值
最大值
单位
公式
N/A
2
清除故障代码/故障指示
灯(MIL)/检查引擎灯
清除所有存储故障
代码。
Lastedited 29 days ago by Nyq
OBD-II PIDs
Watch this page
OBD-II PIDs (On-board diagnostics Parameter IDs) are codes used to requestdata from a vehicle, usedasa diagnostic tool.
SAE standardJ/1979 defines many PIDs,but manufacturers alsodefine many morePIDsspecific to theirvehicles. All light duty vehicles(i.e. less than 8,500 pounds) sold in North America since 1996,as well asmedium duty vehicles (i.e. 8,500-14,000pounds) beginning in 2005,and heavy duty vehicles(i.e. greater than14,000 pounds) beginning in2010,[citation needed]  are requiredto support OBD-II diagnostics,using a standardizeddata link connector, and a subset of the SAE J/1979 definedPIDs (or SAE J/1939 as applicable for medium/heavy duty vehicles), primarily for statemandatedemissions inspections.
Typically, anautomotive technician will use PIDs with a scan tool connected to the vehicle's OBD-II connector.
§  The technician entersthe PID
§  The scan tool sends it to the vehicle's controller–areanetwork(CAN)-bus,VPW, PWM, ISO, KWP. (After 2008, CANonly)
§  A device onthe bus recognizesthe PIDas one it is responsiblefor, and reportsthe value for that PIDto the bus
§  The scantool reads the response,and displays it to the technician
Contents
·      Modes
·      StandardPIDs
o     Mode 01
o     Mode 02
o     Mode 03
o     Mode 04
o     Mode 05
o     Mode 09
o     Bitwise encoded PIDs
§  Mode 1 PID 00
§  Mode 1 PID 01
§  Mode 1 PID 41
§  Mode 1 PID 78
§  Mode 3(no PID required)
§  Mode 9 PID 08
§  Mode 9 PID 0B
o     Enumerated PIDs
§  Mode 1 PID 03
§  Mode 1 PID 12
§  Mode 1 PID 1C
§  Fuel Type Coding
·      Non-standard PIDs
·      CAN (11-bit) bus format
o     Query
o     Response
·      See also
·      References
·      External links
ModesEdit
There are tenmodesof operation described in thelatest OBD-II standardSAEJ1979. They are asfollows:
Mode (hex)
Description
01
Show current data
02
Show freeze frame data
03
Show stored Diagnostic Trouble Codes
04
Clear Diagnostic Trouble Codes and stored values
05
Test results, oxygen sensor monitoring (non CAN only)
06
Test results, other component/system monitoring (Test results, oxygen sensor monitoring for CAN only)
07
Show pending Diagnostic Trouble Codes (detected during current or last driving cycle)
08
Control operation of on-board component/system
09
Request vehicle information
0A
Permanent Diagnostic TroubleCodes (DTCs) (Cleared DTCs)
Vehicle manufacturersarenot required to support all modes. Each manufacturer maydefine additional modes above #9 (e.g.:mode 22 as definedbySAE J2190 for Ford/GM,mode 21 for Toyota)for other informatione.g. the voltage of the tractionbattery in a hybridelectric vehicle(HEV).[1]
Standard PIDsEdit
The table belowshows the standard OBD-II PIDs asdefined by SAE J1979. The expectedresponse for eachPIDis given, along with informationon how to translate the responseinto meaningful data. Again,not all vehicles will support all PIDs andthere can be manufacturer-defined custom PIDs thatarenot defined in theOBD-II standard.
Note that modes 1 and 2 are basically identical,exceptthat Mode 1 provides current information,whereas Mode 2 providesa snapshot of the same data taken atthe point when the last diagnostic trouble code was set. The exceptionsarePID 01, which is only availablein Mode 1, and PID02, which is only availablein Mode 2. If Mode 2PID02 returns zero, then there is no snapshotandall other Mode 2data is meaningless.
When usingBit-Encoded-Notation,quantities like C4 means bit 4 from data byteC. Each bit is numerated from0 to 7, so 7 is the most significantbit and 0 is the leastsignificant bit.
A
B
C
D
A 7
A 6
A 5
A 4
A 3
A 2
A 1
A 0
B 7
B 6
B 5
B 4
B 3
B 2
B 1
B 0
C 7
C 6
C 5
C 4
C 3
C 2
C 1
C 0
D 7
D 6
D 5
D 4
D 3
D 2
D 1
D 0
Mode 01
PID
(hex
)
Data bytes
Description
Min value
Max value
Units
Formula[a]
return ed

00
4
PIDs supported [01
- 20]
Bit encoded [A7..D0] == [PID
$01..PID $20] See below

01

4
Monitor status since DTCs cleared. (Includes malfunction indicator lamp (MIL) status and number of DTCs.)

Bit encoded. See  below
02
2
Freeze DTC
03
2
Fuel system status
Bit encoded. See  below
04
1
Calculated engine load value
0
100
%
A*100/255
05
1
Engine coolant temperature
-40
215
°C
A-40
06
1
Short term fuel % trim—Bank 1
-100
Subtracti ng Fuel (Rich Conditio n)
99.22
Adding Fuel (Lean Conditio n)
%
(A-128) * 100/128
07
1
Long term fuel % trim—Bank 1
-100
Subtracti ng Fuel (Rich Conditio n)
99.22
Adding Fuel (Lean Conditio n)
%
(A-128) * 100/128
08
1
Short term fuel % trim—Bank 2
-100
Subtracti ng Fuel (Rich Conditio n)
99.22
Adding Fuel (Lean Conditio n)
%
(A-128) * 100/128
09
1
Long term fuel % trim—Bank 2
-100
Subtracti
99.22
Adding
%
(A-128) * 100/128

ng Fuel (Rich Conditio n)
Fuel (Lean Conditio n)
0A
1
Fuel pressure
0
765
kPa (gauge)
A*3
0B
1
Intake manifold absolute pressure
0
255
kPa (absolut e)
A
0C
2
Engine RPM
0
16,383.7
5
rpm
((A*256)+B)/4
0D
1
Vehicle speed
0
255
km/h
A
0E
1
Timing advance
-64
63.5
°
relative to #1 cylinder
(A-128)/2
0F
1
Intake air temperature
-40
215
°C
A-40
10
2
MAF air flow rate
0
655.35
grams/se c
((A*256)+B) / 100
11
1
Throttle position
0
100
%
A*100/255
12
1
Commanded secondary air status
Bit encoded. See  below
13
1
Oxygen sensors present
[A0..A3] == Bank
1, Sensors 1-4. [A4..A7] == Bank 2...
14
2
Bank 1, Sensor 1:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
15
2
Bank 1, Sensor 2:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
16
2
Bank 1, Sensor 3: Oxygen sensor
0
1.275
Volts
%
A/200
(B-128) * 100/128
voltage,
Short term fuel trim
-
100(lean)
99.2(rich
)
(if B==$FF, sensor is not used in trim calc)
17
2
Bank 1, Sensor 4:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
18
2
Bank 2, Sensor 1:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
19
2
Bank 2, Sensor 2:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
1A
2
Bank 2, Sensor 3:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
1B
2
Bank 2, Sensor 4:
0
1.275
Volts
A/200
Oxygen sensor
(B-128) * 100/128
voltage,
(if B==$FF, sensor
Short term fuel
-
99.2(rich
is not used in trim
trim
100(lean)
)
%
calc)
1C
1
OBD standards this vehicle conforms to
Bit encoded. See  below
1D
1
Oxygen sensors present
Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2]
1E
1
Auxiliary input status
A0 == Power Take Off (PTO) status (1 == active) [A1..A7] not used
1F
2
Run time since engine start
0
65,535
seconds
(A*256)+B
20
4
PIDs supported [21
- 40]
Bit encoded [A7..D0] == [PID
$21..PID $40] See below
21
2
Distance traveled with malfunction indicator lamp (MIL) on
0
65,535
km
(A*256)+B
22
2
Fuel Rail Pressure
0
5177.26
kPa
((A*256)+B) *
(relative to
manifold vacuum)
5
0.079
23
2
Fuel Rail Pressure (diesel, or gasoline direct inject)
0
655,350
kPa (gauge)
((A*256)+B) * 10

24

4
O2S1_WR_lambda
0
1.999
N/A
((A*256)+B)*2/65
535 or
((A*256)+B)/3276
8
(1):
((C*256)+D)*8/65
Equivalence Ratio
535 or
Voltage
0
7.999
V
((C*256)+D)/8192
25
4
O2S2_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
26
4
O2S3_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
27
4
O2S4_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
28
4
O2S5_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535

29
4
O2S6_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
2A
4
O2S7_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
2B
4
O2S8_WR_lambda
0
2
N/A
((A*256)+B)*2/65
(1):
535
Equivalence Ratio
((C*256)+D)*8/65
Voltage
0
8
V
535
2C
1
Commanded EGR
0
100
%
A*100/255
2D
1
EGR Error
-100
99.22
%
(A-128) * 100/128
2E
1
Commanded evaporative purge
0
100
%
A*100/255
2F
1
Fuel Level Input
0
100
%
A*100/255
30
1
# of warm-ups since codes cleared
0
255
N/A
A
31
2
Distance traveled since codes cleared
0
65,535
km
(A*256)+B
32
2
Evap. System Vapor Pressure
-8,192
8,192
Pa
((A*256)+B)/4 (A
and B are two's complementsigned
)
33
1
Barometric pressure
0
255
kPa (Absolut e)
A
34
4
O2S1_WR_lambda
0
1.999
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
127.99
mA
128
35
4
O2S2_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
36
4
O2S3_WR_lambda
0
2
N/A
((A*256)+B)/3276
(1):
-128
128
mA
8
Equivalence Ratio
((C*256)+D)/256 -
Current
128
37
4
O2S4_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
38
4
O2S5_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
39
4
O2S6_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
3A
4
O2S7_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
3B
4
O2S8_WR_lambda
0
2
N/A
((A*256)+B)/32,7
(1):
68
Equivalence Ratio
((C*256)+D)/256 -
Current
-128
128
mA
128
3C
2
Catalyst Temperature Bank 1, Sensor 1
-40
6,513.5
°C
((A*256)+B)/10 - 40
3D
2
Catalyst Temperature Bank 2, Sensor 1
-40
6,513.5
°C
((A*256)+B)/10 - 40
3E
2
Catalyst Temperature Bank 1, Sensor 2
-40
6,513.5
°C
((A*256)+B)/10 - 40
3F
2
Catalyst Temperature Bank 2, Sensor 2
-40
6,513.5
°C
((A*256)+B)/10 - 40
40
4
PIDs supported [41
- 60]
Bit encoded [A7..D0] == [PID
$41..PID $60] See below
41
4
Monitor status this drive cycle
Bit encoded. See  below
42
2
Control module voltage
0
65.535
V
((A*256)+B)/1000
43
2
Absolute load value
0
25,700
%
((A*256)+B)*100/ 255
44
2
Command equivalence ratio
0
2
N/A
((A*256)+B)/3276 8
45
1
Relative throttle position
0
100
%
A*100/255
46
1
Ambient air temperature
-40
215
°C
A-40
47
1
Absolute throttle position B
0
100
%
A*100/255
48
1
Absolute throttle position C
0
100
%
A*100/255
49
1
Accelerator pedal position D
0
100
%
A*100/255
4A
1
Accelerator pedal position E
0
100
%
A*100/255
4B
1
Accelerator pedal position F
0
100
%
A*100/255
4C
1
Commanded throttle actuator
0
100
%
A*100/255
4D
2
Time run with MIL on
0
65,535
minutes
(A*256)+B
4E
2
Time since trouble codes cleared
0
65,535
minutes
(A*256)+B

4F

4
Maximum value for equivalence ratio, oxygen sensor voltage, oxygen sensor current, and intake manifold absolute pressure

0, 0, 0, 0
255,
255,
255,
2550

, V, mA,
kPa

A, B, C, D*10
50
4
Maximum value for air flow rate from mass air flow sensor
0
2550
g/s
A*10, B, C, and D are reserved for future use
51
1
Fuel Type
From fuel type table seebelow
52
1
Ethanol fuel %
0
100
%
A*100/255
53
2
Absolute Evap system Vapor Pressure
0
327.675
kPa
((A*256)+B)/200
54
2
Evap system vapor pressure
-32,767
32,768
Pa
((A*256)+B)- 32767
55
2
Short term secondary oxygen sensor trim bank 1 and bank 3
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
56
2
Long term secondary oxygen sensor trim bank 1 and bank 3
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
57
2
Short term secondary oxygen sensor trim bank 2 and bank 4
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
58
2
Long term secondary oxygen sensor trim bank 2 and bank 4
-100
99.22
%
(A-128)*100/128 (B-128)*100/128
59
2
Fuel rail pressure (absolute)
0
655,350
kPa
((A*256)+B) * 10
5A
1
Relative accelerator pedal position
0
100
%
A*100/255
5B
1
Hybrid battery pack remaining life
0
100
%
A*100/255
5C
1
Engine oil temperature
-40
210
°C
A - 40
5D
2
Fuel injection timing
-210.00
301.992
°
(((A*256)+B)- 26,880)/128
5E
2
Engine fuel rate
0
3212.75
L/h
((A*256)+B)*0.05
5F
1
Emission requirements to
Bit Encoded
which vehicle is designed

60
4
PIDs supported [61
- 80]
Bit encoded [A7..D0] == [PID
$61..PID $80] See below
61
1
Driver's demand engine - percent torque
-125
125
%
A-125
62
1
Actual engine - percent torque
-125
125
%
A-125
63
2
Engine reference torque
0
65,535
Nm
A*256+B

64

5

Engine percent torque data

-125

125

%
A-125 Idle
B-125 Engine point 1
C-125 Engine point 2
D-125 Engine point 3
E-125 Engine point 4
65
2
Auxiliary input / output supported
Bit Encoded
66
5
Mass air flow sensor
67
3
Engine coolant temperature
68
7
Intake air temperature sensor
69
7
Commanded EGR and EGR Error

6A
5
Commanded Diesel intake air flow control and relative intake air flow position
6B
5
Exhaust gas recirculation temperature

6C
5
Commanded throttle actuator control and relative throttle position
6D
6
Fuel pressure control system
6E
5
Injection pressure control system
6F
3
Turbocharger compressor inlet pressure
70
9
Boost pressure control
71
5
Variable Geometry turbo (VGT) control
72
5
Wastegate control
73
5
Exhaust pressure
74
5
Turbocharger RPM
75
7
Turbocharger temperature
76
7
Turbocharger temperature
77
5
Charge air cooler temperature (CACT)
78
9
Exhaust Gas temperature (EGT) Bank 1
Special PID. See  below
79
9
Exhaust Gas temperature (EGT) Bank 2
Special PID. See  below
7A
7
Diesel particulate filter (DPF)
7B
7
Diesel particulate filter (DPF)

7C
9
Diesel Particulate filter (DPF) temperature
7D
1
NOx NTE control area status
7E
1
PM NTE control area status
7F
13
Engine run time

80
4
PIDs supported [81
- A0]
Bit encoded [A7..D0] == [PID
$81..PID $A0] See
below
81
21
Engine run time for Auxiliary Emissions Control Device(AECD)

82
21
Engine run time for Auxiliary Emissions Control Device(AECD)
83
5
NOx sensor
84
Manifold surface temperature
85
NOx reagent system
86
Particulate matter (PM) sensor
87
Intake manifold absolute pressure

A0
4
PIDs supported [A1 - C0]
Bit encoded [A7..D0] == [PID
$A1..PID
$C0] Seebelow
C0
4
PIDs supported [C1 - E0]
Bit encoded [A7..D0] == [PID
$C1..PID $E0] See
below
C3
?
?
?
?
?
Returns numerous data, including

Drive Condition ID and Engine Speed*
C4
?
?
?
?
?
B5 is Engine Idle Request
B6 is Engine Stop Request*
PID
(hex
)
Data bytes return ed
Description
Min value
Max value
Units
Formula[a]
Mode 02
Mode 02 accepts thesame PIDsas mode 01, with the samemeaning, but informationgiven is from whenthe freeze framewascreated.
Youhaveto send the framenumber in the data sectionof the message.
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
02
2
DTC that caused freeze frame to be stored.
BCD
encoded. Decoded as  inmode3
Mode 03
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
N/A
n*6
Request trouble codes
3 codes per message frame.See below
Mode 04
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
N/A
0
Clear trouble codes / Malfunction indicator lamp (MIL) / Check engine light
Clears all stored trouble codes and turns the MIL off.
Mode 05
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]

0100
OBD Monitor IDs supported ($01 –
$20)
0101
O2 Sensor Monitor Bank 1 Sensor 1
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0102
O2 Sensor Monitor Bank 1 Sensor 2
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0103
O2 Sensor Monitor Bank 1 Sensor 3
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0104
O2 Sensor Monitor Bank 1 Sensor 4
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0105
O2 Sensor Monitor Bank 2 Sensor 1
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0106
O2 Sensor Monitor Bank 2 Sensor 2
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0107
O2 Sensor Monitor Bank 2 Sensor 3
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0108
O2 Sensor Monitor Bank 2 Sensor 4
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0109
O2 Sensor Monitor Bank 3 Sensor 1
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
010A
O2 Sensor Monitor Bank 3 Sensor 2
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
010B
O2 Sensor Monitor Bank 3 Sensor 3
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
010C
O2 Sensor Monitor Bank 3 Sensor 4
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage

010D
O2 Sensor Monitor Bank 4 Sensor 1
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
010E
O2 Sensor Monitor Bank 4 Sensor 2
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
010F
O2 Sensor Monitor Bank 4 Sensor 3
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0110
O2 Sensor Monitor Bank 4 Sensor 4
0.00
1.275
Volts
0.005 Rich to lean sensor threshold voltage
0201
O2 Sensor Monitor Bank 1 Sensor 1
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0202
O2 Sensor Monitor Bank 1 Sensor 2
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0203
O2 Sensor Monitor Bank 1 Sensor 3
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0204
O2 Sensor Monitor Bank 1 Sensor 4
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0205
O2 Sensor Monitor Bank 2 Sensor 1
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0206
O2 Sensor Monitor Bank 2 Sensor 2
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0207
O2 Sensor Monitor Bank 2 Sensor 3
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0208
O2 Sensor Monitor Bank 2 Sensor 4
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0209
O2 Sensor Monitor Bank 3 Sensor 1
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage

020A
O2 Sensor Monitor Bank 3 Sensor 2
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
020B
O2 Sensor Monitor Bank 3 Sensor 3
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
020C
O2 Sensor Monitor Bank 3 Sensor 4
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
020D
O2 Sensor Monitor Bank 4 Sensor 1
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
020E
O2 Sensor Monitor Bank 4 Sensor 2
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
020F
O2 Sensor Monitor Bank 4 Sensor 3
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
0210
O2 Sensor Monitor Bank 4 Sensor 4
0.00
1.275
Volts
0.005 Lean to Rich sensor threshold voltage
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
Mode 09
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
00
4
Mode 9 supported
PIDs (01 to 20)
Bit encoded. [A7..D0] = [PID $01..PID $20] See
below
01
1
VIN Message Count in PID 02. Only for ISO 9141-2, ISO
14230-4 and SAE J1850.

Usually value will be 5.
02
17-20
Vehicle  Identification Number(VIN)
17-char VIN, ASCII-
encoded and left-padded with null chars (0x00) if needed to.

03
1
Calibration ID message count for PID 04. Only for ISO 9141-2, ISO 14230-4 and SAE J1850.
It will be a multiple of 4 (4 messages are needed for each ID).
04
16
Calibration ID
Up to 16 ASCII chars. Data bytes not used will be reported as null bytes (0x00).

05

1
Calibration verification numbers (CVN) message count for PID06. Only for ISO 9141-2, ISO 14230-4 and SAE J1850.

06
4
Calibration Verification Numbers (CVN)
Raw data left-padded with null characters (0x00). Usually displayed as hex string.

07

1
In-use performance tracking message count for
PID 08 and 0B. Only for ISO 9141-2, ISO 14230-4 and SAE J1850.

8

10
8 if sixteen (16) values are required to be reported, 9 if eighteen
(18) values are required to be reported, and 10 if twenty (20) values are required to be reported (one message reports two values, each one consisting in two bytes).
08
4
In-use performance tracking for spark ignition vehicles
4 or 5 messages, each one containing 4 bytes (two values). See below
09
1
ECU name message count for PID 0A
0A
20
ECU name
ASCII-coded. Right- padded with null chars (0x00).
0B
4
In-use performance tracking for
5 messages, each one containing 4 bytes (two values). See below
compression ignition vehicles
PID
(hex)
Data bytes returned
Description
Min value
Max value
Units
Formula[a]
1.     In the formula column,lettersA, B,C, etc. represent the decimal equivalent of the first,second, third, etc. bytes of data. Where a (?) appears, contradictory or incomplete information wasavailable.
Bitwise encoded PIDs
Some of the PIDsin the above table cannotbe explainedwith a simple formula.A more elaborate explanation ofthese data is provided here:
Mode 1 PID 00
A request for this PID returns4 bytesofdata. Each bit, from MSBto LSB,representsone of the next 32 PIDs andis giving information aboutif it is supported.
For example,if the car response isBE1FA813, it can be decoded likethis:
Hexa deci mal
B
E
1
F
A
8
1
3
Binar y
1
0
1
1
1
1
1
0
0
0
0
1
1
1
1
1
1
0
1
0
1
0
0
0
0
0
0
1
0
0
1
1
Supp orted
?
Y
e s
N
o
Y
e s
Y
e s
Y
e s
Y
e s
Y
e s
N
o
N
o
N
o
N
o
Y
e s
Y
e s
Y
e s
Y
e s
Y
e s
Y
e s
N
o
Y
e s
N
o
Y
e s
N
o
N
o
N
o
N
o
N
o
N
o
Y
e s
N
o
N
o
Y
e s
Y
e s
PID
num ber
0
1
0
2
0
3
0
4
0
5
0
6
0
7
0
8
0
9
0
A
0
B
0
C
0
D
0
E
0
F
1
0
1
1
1
2
1
3
1
4
1
5
1
6
1
7
1
8
1
9
1
A
1
B
1
C
1
D
1
E
1
F
2
0
So, supported PIDs
are: 01, 03, 04, 05, 06, 07, 0C, 0D,0E, 0F, 10, 11, 13, 15, 1C,1F and20
Mode 1 PID 01
A request forthis PID returns4 bytesof data.
The first byte contains two pieces of information.BitA7 (MSB of byte A, the firstbyte) indicates whether ornot the MIL (checkengine light)is illuminated.
BitsA6 through A0 representthe number of diagnostic troublecodescurrently flagged in theECU.
The second, third, and fourthbytes giveinformationabout the availability andcompleteness of certainon-boardtests. Note that test availabilityis indicated by set
(1) bit and completeness is indicated by reset (0) bit.
Bit
Name
Definition
A7
MIL
Off or On, indicates if the CEL/MIL is on (or should be on)
A6- A0
DTC_CNT
Number of confirmed emissions-related DTCs available for display.
B7
RESERVED
Reserved (should be 0)
B3
NO NAME
0 = Spark ignition monitors supported
1 = Compression ignition monitors supported
Here are the common bit B definitions, they are testbased.
Test available
Test incomplete
Misfire
B0
B4
Fuel System
B1
B5
Components
B2
B6
The third and fourthbytes are to be interpreted differently depending on if the engine is spark ignitionor compressionignition. Inthesecond (B) byte, bit 3 indicates how to interpret theC and D bytes,with0 being sparkand1 (set) being compression.
The bytes C andD forspark ignition monitors:
Test available
Test incomplete
Catalyst
C0
D0
Heated Catalyst
C1
D1
Evaporative System
C2
D2
Secondary Air System
C3
D3
A/C Refrigerant
C4
D4
Oxygen Sensor
C5
D5
Oxygen Sensor Heater
C6
D6
EGR System
C7
D7
Andthe bytesC and D for compressionignitionmonitors:
Test available
Test incomplete
NMHC Catalyst[a]
C0
D0
NOx/SCR Monitor
C1
D1
Boost Pressure
C3
D3
Exhaust Gas Sensor
C5
D5
PM filter monitoring
C6
D6
EGR and/or VVT System
C7
D7
1.  NMHC may stand for Non-Methane HydroCarbons, butJ1979 does not enlightenus.
Mode 1 PID 41
A request for this PID returns4 bytesof data.Thefirst byte is alwayszero. The second,third, and fourth bytes giveinformation aboutthe availability and completenessof certain on-board tests.Note that test availability is represented by a set(1) bit and completeness is represented by a reset (0)bit:
Test enabled
Test incomplete
Reserved
B3
B7
Components
B2
B6
Fuel System
B1
B5
Misfire
B0
B4
EGR System
C7
D7
Oxygen Sensor Heater
C6
D6
Oxygen Sensor
C5
D5
A/C Refrigerant
C4
D4
Secondary Air System
C3
D3
Evaporative System
C2
D2
Heated Catalyst
C1
D1
Catalyst
C0
D0
Mode 1 PID 78
A request for this PID will return9 bytesof data. The first byte is a bit encoded fieldindicating whichEGTsensorsaresupported:
Byte
Description
A
Supported EGT sensors
B-C
Temperature read by EGT11
D-E
Temperature read by EGT12
F-G
Temperature read by EGT13
H-I
Temperature read by EGT14
The firstbyte isbit-encoded asfollows:
Bit
Description
A7-A4
Reserved
A3
EGT bank 1, sensor 4 Supported?
A2
EGT bank 1, sensor 3 Supported?
A1
EGT bank 1, sensor 2 Supported?
A0
EGT bank 1, sensor 1 Supported?
The remaining bytesare 16 bit integersindicating the temperature in degreesCelsius in the range -40 to 6513.5 (scale 0.1), using the
usual                                                   formula (MSB is A, LSB isB).Only valuesfor which the corresponding sensoris supported are meaningful.
The samestructure appliesto PID79, but values are for sensorsof bank 2.
Mode 3(no PID required)
A request for this mode returns alist of the DTCs that havebeen set. The listis encapsulated using the ISO15765-2 protocol.
If there are twoor fewer DTCs(4 bytes) they are returnedin an ISO-TP Single Frame(SF). Three ormoreDTCs in the list are reported inmultiple frames, with the exactcount of framesdependenton the communication type and addressing details.
Each trouble code requires2 bytesto describe.Thetext description of a trouble code may be decodedas follows. The first character in thetrouble code is determinedby the first two bits in the first byte:
A7-A6
First DTC character
00
P - Powertrain
01
C - Chassis
10
B - Body
11
U - Network
The two following digits are encoded as2 bits. The second character in the DTC is a number defined by the following table:
A5-A4
Second DTC character
00
0
01
1
10
2
11
3
The third characterin the DTC is a number defined by
A3-A0
Third DTC character
0000
0
0001
1
0010
2
0011
3
0100
4
0101
5
0110
6
0111
7
1000
8
1001
9
1010
A
1011
B
1100
C
1101
D
1110
E
1111
F
The fourth and fifth characters are definedin the same way asthe third, but using bits B7-B4 and B3-B0. The resultingfive-charactercodeshould look something like "U0158" and canbe looked up in a table ofOBD-II DTCs. Hexadecimalcharacters(0-9,A-F), while relatively rare, areallowed in the last 3 positions of the code itself. Mode 9 PID 08
It provides informationabout track in-use performance forcatalyst banks,oxygen sensorbanks, evaporative leak detectionsystems,EGR systemsand secondary air system.
The numerator for each componentor systemtracks thenumber of times that allconditions necessary for aspecific monitor to detect a malfunction have beenencountered. Thedenominator for each component or system tracksthe number of times that thevehicle has beenoperated in the specifiedconditions.
All data itemsof the In-use Performance Tracking record consist of two (2) bytesandarereported in this order (eachmessage containstwo items, hence themessage lengthis 4):
Mnemonic
Description
OBDCOND
OBD Monitoring Conditions Encountered Counts
IGNCNTR
Ignition Counter
CATCOMP1
Catalyst Monitor Completion Counts Bank 1
CATCOND1
Catalyst Monitor Conditions Encountered Counts Bank 1
CATCOMP2
Catalyst Monitor Completion Counts Bank 2
CATCOND2
Catalyst Monitor Conditions Encountered Counts Bank 2
O2SCOMP1
O2 Sensor Monitor Completion Counts Bank 1
O2SCOND1
O2 Sensor Monitor Conditions Encountered Counts Bank 1
O2SCOMP2
O2 Sensor Monitor Completion Counts Bank 2
O2SCOND2
O2 Sensor Monitor Conditions Encountered Counts Bank 2
EGRCOMP
EGR Monitor Completion Condition Counts
EGRCOND
EGR Monitor Conditions Encountered Counts
AIRCOMP
AIR Monitor Completion Condition Counts (Secondary Air)
AIRCOND
AIR Monitor Conditions Encountered Counts (Secondary Air)
EVAPCOMP
EVAP Monitor Completion Condition Counts
EVAPCOND
EVAP Monitor Conditions Encountered Counts
SO2SCOMP1
Secondary O2 Sensor Monitor Completion Counts Bank 1
SO2SCOND1
Secondary O2 Sensor Monitor Conditions Encountered Counts Bank 1
SO2SCOMP2
Secondary O2 Sensor Monitor Completion Counts Bank 2
SO2SCOND2
Secondary O2 Sensor Monitor Conditions Encountered Counts Bank 2
Mode 9 PID 0B
Itprovides information about track in-use performance for NMHCcatalyst,NOx catalyst monitor, NOx adsorbermonitor, PM filter monitor, exhaust gas sensormonitor, EGR/ VVT monitor, boost pressure monitor andfuel system monitor.
Alldata items consist of two (2)bytesandare reported in this order (each messagecontains two items, hence message lengthis 4):
Mnemonic
Description
OBDCOND
OBD Monitoring Conditions Encountered Counts
IGNCNTR
Ignition Counter
HCCATCOMP
NMHC Catalyst Monitor Completion Condition Counts
HCCATCOND
NMHC Catalyst Monitor Conditions Encountered Counts
NCATCOMP
NOx/SCR Catalyst Monitor Completion Condition Counts
NCATCOND
NOx/SCR Catalyst Monitor Conditions Encountered Counts
NADSCOMP
NOx Adsorber Monitor Completion Condition Counts
NADSCOND
NOx Adsorber Monitor Conditions Encountered Counts
PMCOMP
PM Filter Monitor Completion Condition Counts
PMCOND
PM Filter Monitor Conditions Encountered Counts
EGSCOMP
Exhaust Gas Sensor Monitor Completion Condition Counts
EGSCOND
Exhaust Gas Sensor Monitor Conditions Encountered Counts
EGRCOMP
EGR and/or VVT Monitor Completion Condition Counts
EGRCOND
EGR and/or VVT Monitor Conditions Encountered Counts
BPCOMP
Boost Pressure Monitor Completion Condition Counts
BPCOND
Boost Pressure Monitor Conditions Encountered Counts
FUELCOMP
Fuel Monitor Completion Condition Counts
FUELCOND
Fuel Monitor Conditions Encountered Counts
Enumerated PIDs
SomePIDsare to be interpreted specially,and aren't necessarily exactly bitwiseencoded, orin any scale. The values for these PIDsare enumerated.
Mode 1 PID 03
A request forthis PID returns2 bytesof data.Thefirst byte describes fuelsystem #1.
Value
Description
1
Open loop due to insufficient engine temperature
2
Closed loop, using oxygen sensor feedback to determine fuel mix
4
Open loop due to engine load OR fuel cut due to deceleration
8
Open loop due to system failure
16
Closed loop, using at least one oxygen sensor but there is a fault in the feedback system
Any other valueis an invalid response. There can only be one bitset at most.
The second byte describes fuelsystem#2 (if it exists)and is encoded identically to the firstbyte.
Mode 1 PID 12
A request for this PID returnsa single byte of data which describes the secondary air status.
Value
Description
1
Upstream
2
Downstream of catalytic converter
4
From the outside atmosphere or off
8
Pump commanded on for diagnostics
Any other valueis an invalid response. There can only be one bitset at most.
Mode 1 PID 1C
A request for this PID returnsa single byte of data which describes which OBDstandards this ECU was designed to comply with. The differentvalues the data byte canhold are shown below,nextto what they mean:
Value
Description
1
OBD-II as defined by the CARB
2
OBD as defined by the EPA
3
OBD and OBD-II
4
OBD-I
5
Not OBD compliant
6
EOBD (Europe)
7
EOBD and OBD-II
8
EOBD and OBD
9
EOBD, OBD and OBD II
10
JOBD (Japan)
11
JOBD and OBD II
12
JOBD and EOBD
13
JOBD, EOBD, and OBD II
14
Reserved
15
Reserved
16
Reserved
17
Engine Manufacturer Diagnostics (EMD)
18
Engine Manufacturer Diagnostics Enhanced (EMD+)
19
Heavy Duty On-Board Diagnostics (Child/Partial) (HD OBD-C)
20
Heavy Duty On-Board Diagnostics (HD OBD)
21
World Wide Harmonized OBD (WWH OBD)
22
Reserved
23
Heavy Duty Euro OBD Stage I without NOx control (HD EOBD-I)
24
Heavy Duty Euro OBD Stage I with NOx control (HD EOBD-I N)
25
Heavy Duty Euro OBD Stage II without NOx control (HD EOBD-II)
26
Heavy Duty Euro OBD Stage II with NOx control (HD EOBD-II N)
27
Reserved
28
Brazil OBD Phase 1 (OBDBr-1)
29
Brazil OBD Phase 2 (OBDBr-2)
30
Korean OBD (KOBD)
31
India OBD I (IOBD I)
32
India OBD II (IOBD II)
33
Heavy Duty Euro OBD Stage VI (HD EOBD-IV)
34-250
Reserved
251-255
Not available for assignment (SAE J1939 special meaning)
FuelType Coding
Mode 1 PID 51 returns a value froman enumerated list giving the fueltype ofthe vehicle. The fuel type is returnedasa single byte, and the valueis givenbythe following table:
Value
Description
0
Not available
1
Gasoline
2
Methanol
3
Ethanol
4
Diesel
5
LPG
6
CNG
7
Propane
8
Electric
9
Bifuel running Gasoline
10
Bifuel running Methanol
11
Bifuel running Ethanol
12
Bifuel running LPG
13
Bifuel running CNG
14
Bifuel running Propane
15
Bifuel running Electricity
16
Bifuel running electric and combustion engine
17
Hybrid gasoline
18
Hybrid Ethanol
19
Hybrid Diesel
20
Hybrid Electric
21
Hybrid running electric and combustion engine
22
Hybrid Regenerative
23     Bifuel running diesel
Any othervalue is reserved by ISO/SAE.There are currently nodefinitionsfor flexible-fuel vehicle.
Non-standard PIDsEdit
The majority ofall OBD-II PIDs in use are non-standard.Formost modern vehicles, there are many more functions supported on the OBD-II interface than are coveredby the standard PIDs,and there is relatively minor overlap betweenvehicle manufacturers for these non-standard PIDs.
There is very limitedinformation availablein the public domain for non-standard PIDs. The primary source ofinformation on non-standard PIDs acrossdifferent manufacturers is maintained by the US-based Equipmentand ToolInstituteand onlyavailable to members. The price ofETI membership foraccess to scan codes startsfrom US$7,500.[2]
However, even ETI membership will not provide fulldocumentation for non-standardPIDs. ETI state:[2]
Some OEMs refuse to use ETI as a one-stop source of scan tool information.
They prefer to do business with each tool company separately. These companiesalso require that you enter into a contract with them. The charges vary but here is a snapshot of today's[when?] per year charges as we know them: GM $50,000
Honda$5,000 Suzuki $1,000
BMW $17,500 plus $1,000 per update. Updates occur every quarter. (This is more now, but do not have exact number)
CAN (11-bit) busformatEdit
The PIDquery andresponse occurson the vehicle's CANbus. Standard OBD requests andresponsesusefunctional addresses. The diagnosticreaderinitiatesa query using CAN ID$7DF,which acts as a broadcast address, and accepts responses fromanyID in the range $7E8 to $7EF. ECUs that canrespond to OBD querieslisten both to the functionalbroadcast ID of$7DF and one assigned IDin the range $7E0to
$7E7. Their response has an ID of their assigned ID plus 8 e.g. $7E8 through$7EF.
This approach allows up to eight ECUs, eachindependently responding to OBD queries. Thediagnostic reader canusethe ID in the ECU response frame to continue communication with aspecific ECU. In particular, multi-framecommunication requiresa responseto the specific ECU ID ratherthan to ID $7DF.
CAN bus may also be used for communication beyondthe standard OBD messages.Physical addressing uses particular CAN IDsfor specific modules(e.g.,720 for the instrument cluster in Fords) withproprietary frame payloads.
Query
The functional PID query is sentto the vehicle on the CANbus at ID 7DFh, using8 data bytes.The bytes are:
Byte
PID
Type
0
1
2
3
4
5
6
7
SAE
Standard
Number of additional data bytes: 2
Mode
01 = show current data; 02 = freeze frame;
etc.
PID code (e.g.: 05 =
Engine coolant temperature)
not used (may be 55h)
Vehicle specific
Number of additional data bytes: 3
Custom mode: (e.g.: 22 =
enhanced data)
PID code (e.g.: 4980h)
not used (may be 00h or 55h)
Response
The vehicle responds to the PID query on theCAN bus with message IDs that depend on which module responded. Typically the engine or mainECU responds at ID7E8h. Other modules, like the hybrid controlleror battery controllerin a Prius, respond at 07E9h, 07EAh, 07EBh, etc.These are 8h higherthanthe physical addressthemodule responds to. Even though thenumber of bytes in the returnedvalue is variable, the message uses8 data bytesregardless (CAN bus protocol form Frameformat with8 data bytes).Thebytes are:
Byte
PID
Type
0
1
2
3
4
5
6
7
SAE
Standa rd 7E8h,
7E9h,
7EAh,
etc.
Number of addition al
data bytes: 3 to 6
Custom mode Same as query, except that 40h is added to the mode value. So:
41h =

PID code (e.g.: 05 =
Engine coolant temperatu re)
value of the specifie d paramet er, byte 0

value, byte 1 (optiona l)

value, byte 2 (option al)

value, byte 3 (option al)

not used (may be 00h or 55h)
show current data; 42h =
freeze frame; etc.

Vehicl e specifi c 7E8h,
or 8h + physic al ID of modul e.

Number of addition al
data bytes: 4to 7
Custom mode: same as query, except that 40h is added to the mode value.(e. g.: 62h =
response to mode 22h request)


PID code (e.g.: 4980h)

value of the specifie d paramet er, byte 0

value, byte 1 (option al)

value, byte 2 (option al)

value, byte 3 (option al)
Vehicl
Number
7Fh this
Custom

31h

not used
mode:
e
a general
(e.g.: 22h
specifi
response
=
c
usually
enhanced
7E8h,
indicatin
diagnostic
or 8h +
of
g the
data by
physic
addition
module
PID, 21h
al ID
al
doesn't
=
of
data
recogniz
enhanced
modul
bytes:
e the
data by
e.
3
request.
offset)
(may be 00h)
See alsoEdit
§  On-board diagnostics
§  Engine controlunit
ReferencesEdit
1.     "Escape PHEV TechInfo - PIDs". Electric AutoAssociation - Plug in Hybrid ElectricVehicle. Retrieved11 December 2013.
2.     "ETI Full Membership FAQ". Retrieved 29 November 2013. showing cost of access toOBD-II PID documentation
External linksEdit
§  OBD II Error Codes DefinitionandLookup, including manufacturer-specificcodes.
§  OBD-II Error Codes Definition,descriptionand repair information formost makes of vehicles.
§  Generic/Manufacturer OBD2 Codes andTheirMeanings
§  Directive 98/69/EC of the European Parliament andof the Council of 13October  1998.
§  CAN BusVehicles Partial list of 2003-2007 vehicles whichsupport the OBD-II CAN bus standard.
§  Fault Code Examples Sample fault code data read using the OBDKey Bluetooth, OBDKey USB andOBDKey WLANvehicle interface units.
Read in another language
·     
o     Mobile
o     Desktop
·        Content isavailable underCC BY-SA 3.0unlessotherwisenoted.
·        Termsof Use
·        Privacy
