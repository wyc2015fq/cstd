# OBD数据读取 - xqhrs232的专栏 - CSDN博客
2017年08月24日 23:32:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4486
原文地址::[http://blog.csdn.net/w7822055/article/details/23854901](http://blog.csdn.net/w7822055/article/details/23854901)
相关文章
1、OBD完整协议----[http://download.csdn.net/download/sqxsny/8356281](http://download.csdn.net/download/sqxsny/8356281)
我国采用了EOBD相同的要求即ISO15031-5(道路车辆-车辆与排放诊断相关装置通信标准-5排放有关的诊断服务)协议。所以只要该车支持ISO15031-5的OBD2标准协议中所有项，则可以通过OBD接口读取出ECU中所有信息；若该车支持标准协议中部分项，则读取出支持项信息。
在ISO15031-5协议中，油耗不能读取，只能读取燃油液位输入 (读出油箱剩余油量与油箱容量的百分比)。在车上通过燃油液位传感器实现对剩余油量检测。
OBD输出信息中跟里程相关只有：故障灯点亮后行驶的里程数、消除故障后行驶的里程数。
里程获取办法：
1、虽然不能直接获得总里程，但可以总里程=安装前里程数+故障灯点亮后行驶的里程数+消除故障后行驶的里程数。
2、OBD2协议中无法直接读取仪表上数据，只有通过购买汽车厂家的OBD2协议的扩展，可获得汽车仪表系统数据获取，肯定能获取汽车总里程和车门窗信息。由于成本太高，所以不现实。
3、在车轮处安装及车轮转过圈数的传感器
4、还有通过GPS获取总里程。
在ISO15031-5的OBD协议中不支持读取和控制车门窗的状态信息。读取信息是从ISO15031-5协议中分析出来：
我们关注输出信息有：
系统输出的每个参数都对应一个使用16进制表示的PID(Parameter 2、 3、 4、 注：PID：OBDIdentification)，即参数标识。
PID$01 故障码清除之后的监测状态
PID$05 发动机冷却液温度
PID$0C 发动机转速 可以读取实时转速或者故障时转速。
数据类型：data/4 rpm (0<data<1638375)
PID$0D 车速 可以读取实时车速或者故障时车速。
数据类型：data km/h (0<data<255)
PID$2F 燃油液位输入 读出油箱剩余油量与油箱容量的百分比
读取详细OBD数据流见下面二。
三、数据读取（以ISO15765-4的CAN总线协议为例）
1、硬件接线
硬件上接线：
CAN_H-----接OBD接口第6引脚
CAN_L-----接OBD接口第14引脚
电源正端---接OBD接口第16引脚
公共地-----------接OBD接口第4引脚
信号地------接OBD接口第5引脚
2、软件协议
下面以读取MODE1实时车速为例：外面设备--------》CAN-----------》汽车ECU
发送格式：CAN的ID PCI MODE PID
标准：7DF 01 01 0d 00 00 00 00 00 扩展：18DB33F101 01 0d 00 00 00 00 00
数据域:PCI+MODE+PID，其中PCI表示协议控制信息的字节数量· 01----Mode 1: 请求动力系当前数据; PID 0D----车速
[](http://blog.csdn.net/w7822055/article/details/23854901#)[](http://blog.csdn.net/w7822055/article/details/23854901#)[](http://blog.csdn.net/w7822055/article/details/23854901#)[](http://blog.csdn.net/w7822055/article/details/23854901#)[](http://blog.csdn.net/w7822055/article/details/23854901#)[](http://blog.csdn.net/w7822055/article/details/23854901#)

