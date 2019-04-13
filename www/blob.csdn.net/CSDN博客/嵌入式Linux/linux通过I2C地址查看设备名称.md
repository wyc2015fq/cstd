
# linux通过I2C地址查看设备名称 - 嵌入式Linux - CSDN博客

2015年04月17日 10:22:53[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：6251


﻿﻿
[root@android:/sys/bus/i2c](mailto:root@android:/sys/bus/i2c)\# cd devices
cd devices
[root@android:/sys/bus/i2c/devices](mailto:root@android:/sys/bus/i2c/devices)\# ls
ls
0-0020
0-0022
0-0036
0-0078
1-000c
1-000d
1-001d
1-0028
1-0029
1-002a
1-0038
1-0060
1-0068
2-001c
i2c-0
i2c-1
i2c-2
[root@android:/sys/bus/i2c/devices](mailto:root@android:/sys/bus/i2c/devices)\# cd 0-0036
cd 0-0036
[root@android:/sys/bus/i2c/devices/0-0036](mailto:root@android:/sys/bus/i2c/devices/0-0036)\# ls
ls
driver
modalias
name
power
subsystem
uevent
[root@android:/sys/bus/i2c/devices/0-0036](mailto:root@android:/sys/bus/i2c/devices/0-0036)\# cat name
cat name
msm_actuator
[root@android:/sys/bus/i2c/devices/0-0036](mailto:root@android:/sys/bus/i2c/devices/0-0036)\#

