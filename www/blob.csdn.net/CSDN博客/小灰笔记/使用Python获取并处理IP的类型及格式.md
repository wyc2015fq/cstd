# 使用Python获取并处理IP的类型及格式 - 小灰笔记 - CSDN博客





2017年04月16日 14:45:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1633








公网与私有网络的判断其实十分简单，只要记住私有网络的三个网段。不过，对于记性不好的人或者学识不是很高的机器来说，有一种判断方法还是有必要的。

写如下脚本：

  1 from IPy
import IP

  2 

  3 ip1 = IP('192.168.1.2')

  4 ip2 = IP('11.12.13.14')

  5 

  6 print("ip1 type: %s" % ip1.iptype())

  7 print("ip2 type: %s" % ip2.iptype())

  8 

  9 print("ip2 int value: %d" % ip2.int())

 10 print("ip2 hex value: %s" % ip2.strHex())

 11 print("ip2 bin value: %s" % ip2.strBin())

 12 

 13 print("IP for 0x1234567: %s" % IP(0x1234567))




运行结果如下：

ip1 type: PRIVATE

ip2 type: PUBLIC

ip2 int value: 185339150

ip2 hex value: 0xb0c0d0e

ip2 bin value: 00001011000011000000110100001110

IP for 0x1234567: 1.35.69.103




从上面的结果可以看出：

1，ip1位私有地址；

2，ip2是公网地址；

3，IP的不同类型可以进行自由转换；



