# TS包头语法结构 - 建建的博客 - CSDN博客
2017年03月23日 09:02:47[纪建](https://me.csdn.net/u013898698)阅读数：382
TS包由4B的包头（header）、可变长度的调整头和净荷（payload）组成；
![](https://img-my.csdn.net/uploads/201210/20/1350723090_1086.jpg)
包头结构定义如下：
struct ts_header{
    char     syn_byte:8;                                       // 包头同步字节，0x47
    char     transport_error_indicator:1;         //传送数据包差错指示器
    char    payload_unit_start_indicator:1;    //有效净荷单元开始指示器
    char    transport_priority:1;                         //传送优先级
    int        PID:13;                                              //包ID
    char    transport_scrambling_control:2;                //传送加扰控制
    char    adaptation_field_control:2;           //调整字段控制
    char    continuity_conunter:4;                    //连续计数器 0-15
}；
syn_byte：值为0x47，是MPEG-2TS的传送包标识符；
transport_error_indicator：值为1时，表示相关的传送包中至少有一个不可纠正的错误位，只有错误位纠正后，该位才能置0；
payload_unit_start_indicator：表示TS包的有效净荷带有PES或PSI数据的情况；当TS包的有效净荷带有PES包数据时，
payload_unit_start_indicator为1，表示TS包的有效净荷以PES包的第一个字节开始；为0，表示TS包的开始不是PES包；
当TS包带有PSI数据时，payload_unit_start_indicator为1，表示TS包带有PSI部分的第一个字节，即第一个字节带有指针pointer_field，为0表示TS包不带有一个PSI部分的第一个字节，即在有效净荷中没有指针pointer_field；对于空包的包，play_unit_start_indicator应该置为0；
transport_priority：置1表示相关的包比其他具有相同PID但transport_priority为0的包有更高的优先级；
PID:表示存储于传送包的有效净荷中数据的类型。
![](https://img-my.csdn.net/uploads/201210/20/1350724704_5480.jpg)
transport_scrambling_control：指示TS包有效净荷的加扰方式，如果首部包括调整字段，则不应该被加扰，对于空包，值要置“00”；
![](https://img-my.csdn.net/uploads/201210/20/1350725246_1125.jpg)
adaptation_field_control：传送流包首部是否跟随有调整字段和/或有效净荷。
![](https://img-my.csdn.net/uploads/201210/20/1350725348_2538.jpg)
continuity_conunter：随着具有相同PID TS包的增加而增加，当达到最大时，又恢复为0，如果调整字段控制值adaptation_field_control为“00”或“10”，则该连续计数器不增加；在TS中，当复用的包可能被作为两个连续的具有相同PID的TS包传送出去时，则复用的传送包与原传送包具有相同的continuity_counter，而adaptation_field_control字段值应为“01”或者"10"。在复用的包中，除了节目参考时钟PCR有效字段的值被重新编码外，原包中每个字节将被复制。
在特定的TS中具有相同PID包的continuity_counter是连续的，或与前一个具有相同PID的包相差1，但是遇到adaptation_field_control为“00”或“10”等不增加条件，或在调整字段中discontinuity_indicator为“1”时，continuity_counter将不连续。
