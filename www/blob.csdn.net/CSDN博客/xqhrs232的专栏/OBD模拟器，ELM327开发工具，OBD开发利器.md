# OBD模拟器，ELM327开发工具，OBD开发利器 - xqhrs232的专栏 - CSDN博客
2018年02月24日 17:22:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：380
原文地址::[http://blog.csdn.net/wangzhenhui/article/details/25009351](http://blog.csdn.net/wangzhenhui/article/details/25009351)
相关文章
1、车载娱乐系统电路设计图集锦 —电路图天天读（139） - 全文----[http://www.elecfans.com/article/88/131/190/2015/20150410367914_a.html](http://www.elecfans.com/article/88/131/190/2015/20150410367914_a.html)

OBD MINI模拟器是一款模拟车辆OBD总线协议的模拟器装置。具有动态模拟引擎参数；实时LCD显示主要参数内容。
1设备基本说明：
下图是设备实物图
![](http://img03.taobaocdn.com/imgextra/i3/783720311/T2xJbrXIdaXXXXXXXX-783720311.jpg)
l OBD接口为标准16Pin接口
l 电源输入为12V直流2A
l 电源开关为12V电源开启关闭控制
l 参数动态更新旋钮，能够更新主要OBD的几项参数
n 010C RPM代表引擎转速Engine RPM
n 010D VSS代表车辆速度Vehicle Speed
n 0110 MAF代表车辆空气流量传感器的空气流量  MAF air flow rate
n 0105 ECT代表引擎冷却液温度  Engine coolant temperature
n 010B MAP代表进气歧管绝对压力  Intake Maniford absolute pressure
n 0114 O1V代表Bank1 Sensor 1氧传感器的短时修正
l LCD显示器在启动时显示设备模拟器的协议选择等参数。在正常工作后，显示引擎转速，车辆速度，以及MAF空气流量参数。
l DTC按钮，控制模拟器是否产生DTC错误码
l 通信指示，通信指示为三个LED指示灯指示，模拟器与OBD检测设备建立起总线连接后，CONNECT指示灯点亮，通信具体数据包的时候，T/R指示灯闪烁。
2，模拟器特点描述
l 2.1，支持协议
n ISO 15765 CAN 250k 11bit标准协议
n ISO 15765 CAN 250k 29bit扩展协议
n ISO 15765 CAN 500k 11bit标准协议
n ISO 15765 CAN 500k 29bit扩展协议
n ISO 9141协议
n ISO 14230快速初始化协议
n ISO 14230 5波特率初始化协议
l 2.2，支持故障码，DTC生成定义
l 关键参数，RPM转速，VSS车速，MAF油耗相关，VCT冷却液等液晶显示
![](http://img01.taobaocdn.com/imgextra/i1/783720311/T2wgbsXShXXXXXXXXX-783720311.jpg)
3，简单检测工作说明
l a，设置OBD工作模式，所有开关拨到右侧，CAN BUS开关拨到左侧，设置模拟车辆CAN BUS总线（典型车辆）。
l b，接入电源，打开电源开关，
l c，插入OBD被测装置，观察NET指示灯和T/R收发指示灯。
l d，当OBD双向通信建立起连接后，NET指示灯亮，LCD显示器显示Connect。同时有T/R收发。
4，ScanMaster效果截图
![](http://img02.taobaocdn.com/imgextra/i2/783720311/T2r5PzXN0XXXXXXXXX-783720311.jpg)
![](http://img02.taobaocdn.com/imgextra/i2/783720311/T2.4zzXRXXXXXXXXXX-783720311.jpg)
5，部分安卓程序故障码解析
![](http://img04.taobaocdn.com/imgextra/i4/783720311/T2taryXH0aXXXXXXXX-783720311.png)
![](http://img01.taobaocdn.com/imgextra/i1/783720311/T2d4YzXPpXXXXXXXXX-783720311.png)
价格298元
淘宝购买链接
[http://item.taobao.com/item.htm?id=38737987149](http://item.taobao.com/item.htm?id=38737987149)

