# hisi平台mii网络模式和rmii网络模式的uboot制作 - DoubleLi - 博客园






MII网络uboot编译说明


一：编译生成默认的uboot
1. 进入到uboot目录
a. cd /home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot
2. 新建临时文件夹
a. mkdir u-boot-2010.06-mii-debug
3. 解压
a. tar -xzvf u-boot-2010.06.tgz -C u-boot-2010.06-mii-debug
4. 进入目录
cd u-boot-2010.06-mii-debug/u-boot-2010.06
5. 生成配置文件
a. make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- hi3518c_config
6. 编译
a. make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux-（完成后将在当前目录生成u-boot和u-boot.bin，我们需要的是u-boot.bin）


二：生成寄存器配置表文件
1. 拷贝寄存器配置表格文件到windows中
a. 寄存器配置表格文件目录（~SDK/osdrv/tools/pc_tools/uboot_tools/uboot-Hi3518C-bvt_No2_440_200_400.xlsm）
2. 打开寄存器配置表格文件
a. 必须用Miscosoft Excel，WPS的Excel不能用
b. 必须启用宏选项
3. 生成寄存器配置文件
a. 点击mail页面的Generate reg bin file按钮
b. 成功后将在当前目录生成reg_info.bin文件


三：生成最终的uboot镜像文件
1. 拷贝制作脚本mkboot.sh
a. mkboot.sh所在目录(~SDK/osdrv/tools/pc_tools/uboot_tools)
b. 将制作脚本mkboot.sh拷贝到~SDK/osdrv/uboot/u-boot-2010.06-mii-debug目录
2. 拷贝寄存器配置文件reg_info.bin
a. reg_info.bin文件的目录在windows下与uboot-Hi3518C-bvt_No2_440_200_400.xlsm同级目录下
b. 将reg_info.bin拷贝到/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug目录
3. 拷贝u-boot.bin文件
a. u-boot.bin文件所在目录/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug/u-boot-2010.06
b. 拷贝u-boot.bin到/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug目录
4. uboot制作目录下文件有
a. mkboot.sh
b. reg_info.bin
c. u-boot.bin
5. 制作最终的uboot镜像文件
a. ./mkboot.sh reg_info.bin u-boot.bin
b. 执行后的u-boot.bin文件即为最终的uboot镜像文件


##############################################################################################


RMII网络模式下UBOOT制作方法
RMII:另外一种网络连接方式，它使用的GPIO口少于MII所使用的GPIO，剩余的GPIO口可做其他用途，比如云台


一：编译生成默认的uboot
1. 进入到uboot目录
a. cd /home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot
2. 新建临时文件夹
a. mkdir u-boot-2010.06-rmii-debug
3. 解压
a. tar -xzvf u-boot-2010.06.tgz -C u-boot-2010.06-rmii-debug
4. 进入目录
cd u-boot-2010.06-rmii-debug/u-boot-2010.06
5. 修改配置文件
a. 配置文件目录u-boot-2010.06/include/configs/hi3518c.h
b. 修改项
将以下行
181     #define HIETH_MII_RMII_MODE_U       HISFV_MII_MODE
182     #define HIETH_MII_RMII_MODE_D       HISFV_MII_MODE
修改为：
181     #define HIETH_MII_RMII_MODE_U       HISFV_RMII_MODE//HISFV_MII_MODE
182     #define HIETH_MII_RMII_MODE_D       HISFV_RMII_MODE//HISFV_MII_MODE
6. 修改网络时钟
a. 修改文件：u-boot-2010.06/drivers/net/hisfv300/mii-drv.c
b. 修改项
在函数unsigned int get_phy_device(char *devname, unsigned char phyaddr)增加以下语句
47 /* PHY-8201 */ 
48 if ((phy_id & 0xFFFFFFF0) == 0x1cc810) 
49 { 
50 if (HIETH_MII_RMII_MODE_U == HISFV_RMII_MODE) 
51 { 
52 unsigned short reg; 
53 miiphy_write(devname, phyaddr, 0x1F, 0x7); 
54
55 miiphy_read(devname, phyaddr, 0x10, ®); 
56 reg |= (1 << 12); // set phy RMII 50MHz clk; 
57 miiphy_write(devname, phyaddr, 0x10, reg); 
58
59 miiphy_write(devname, phyaddr, 0x1F, 0x0); 
60 } 
61 }
7. 生成配置文件
a. make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- hi3518c_config
8. 编译
a. make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux-（完成后将在当前目录生成u-boot和u-boot.bin，我们需要的是u-boot.bin）


二：生成寄存器配置表文件
1. 拷贝寄存器配置表格文件到windows中
a. 寄存器配置表格文件目录（~SDK/osdrv/tools/pc_tools/uboot_tools/uboot-Hi3518C-bvt_No2_440_200_400.xlsm）
2. 打开寄存器配置表格文件
a. 必须用Miscosoft Excel，WPS的Excel不能用
b. 必须启用宏选项
3. 修改相应的项
a. 修改点1
在pll表格的PERI_CRG58上一行加入以下行（将HD3518C的ETH网络模式配置成RMII
）
寄存器名称 偏移地址
写入寄存器的值/读出判断的值 delay值
选择读/写 读写多少bit
从第几bit开始读写 寄存器读写属性
PERI_CRG51  0xcc 0x2  0 写 1
2 0x0000100D
b. 修改点2
修改mutilplex表格的该行(将输出时钟由MII时钟改为RMII时钟)
muxctrl_reg23  0x5c 0x01  0 写 1
0 0x0000000D
修改为
muxctrl_reg23  0x5c 0x03  0 写 1
0 0x0000000D
4. 生成寄存器配置文件
a. 点击mail页面的Generate reg bin file按钮
b. 成功后将在当前目录生成reg_info.bin文件


三：生成最终的uboot镜像文件
1. 拷贝制作脚本mkboot.sh
a. mkboot.sh所在目录(/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/tools/pc_tools/uboot_tools)
b. 将制作脚本mkboot.sh拷贝到/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug目录
2. 拷贝寄存器配置文件reg_info.bin
a. reg_info.bin文件的目录在windows下与uboot-Hi3518C-bvt_No2_440_200_400.xlsm同级目录下
b. 将reg_info.bin拷贝到/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug目录
3. 拷贝u-boot.bin文件
a. u-boot.bin文件所在目录/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug/u-boot-2010.06
b. 拷贝u-boot.bin到/home/satan/Hi3518_SDK_V1.0.7.0/osdrv/uboot/u-boot-2010.06-mii-debug目录
4. uboot制作目录下文件有
a. mkboot.sh
b. reg_info.bin
c. u-boot.bin
5. 制作最终的uboot镜像文件
a. ./mkboot.sh reg_info.bin u-boot.bin
b. 执行后的u-boot.bin文件即为最终的uboot镜像文件





from:http://blog.csdn.net/u014780165/article/details/43193099









