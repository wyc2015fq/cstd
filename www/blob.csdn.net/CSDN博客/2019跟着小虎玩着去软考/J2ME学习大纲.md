# J2ME学习大纲 - 2019跟着小虎玩着去软考 - CSDN博客
2009年11月03日 10:36:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1690
**J2ME****学习大纲**
# 1．三大平台
|**ID**|**平台**|**英文全称**|**中文名称**|**用途**|
|----|----|----|----|----|
|1|J2SE|Java2 Standard Edition|标准版|应用程序开发基础平台|
|2|J2EE|Java2 Enterprise Edition|企业版|分布式的服务器应用程序开发平台|
|3|J2ME|Java2 Micro Edition|微型版|嵌入式、小型设备开发平台|
# 2．两类设备
|**比较项**|**CD**|**CLD**|
|----|----|----|
|中文名称|联接设备|有限联接设备|
|英文全称|Connected Device|Connected Limited Device|
|电源|电源稳定|没有稳定的电源供应(通常使用电池)|
|连接|有线连接|无线连接|
|资源|设备外设资源比较受限|设备外设资源极少|
|内存|1～4MB|可能只有512KB|
|处理器主频|75MHz以上|16MHz以上|
|配置|CDC|CLDC|
|应用场景|数字电视、机顶盒、网络电话、车载计算设备、电视机，冰箱|智能手机、PDA|
# 3．一个架构
![](https://p-blog.csdn.net/images/p_blog_csdn_net/littletigerat/EntryImages/20091103/133F21918-0.jpg)
┌───────┐┄┄┄┄┄┄┄
│MIDlet │利用程序
├───────┤┈┈┈┈┈┈┈
│ MIDP │ proflie层
├───────┤┄┄┄┄┄┄┄
│J2ME核心API │ configuration层
├───────┤ CLDC
│ KVM │
├───────┤┈┄┄┄┄┄┄
│特定的硬件│
└───────┘
# 4．两大核心概念
Configuration与Profile
J2ME 使用配置(CDC或CLDC)和简表(Profile)定制JRE(Java Runtime Environment， Java运行时环境)。
一个完整的 JRE由配置和简表组成，配置决定了所使用的JVM(可能是CVM 或 KVM)，而简表通过定义特定的类来为应用程序提供功能上的支持，一个简表定义了设备所提供的 API 集合。
**如果把CDC ****、CLDC ****理解称为硬件配置的话，Profile****可以理解为硬件上的操作系统，这个操作系统提供了程序可以调用的所有功能集合，也就是说Profile****定义了各种功能的API ****函数集。**
# 5．一个标准化组织JCR
在国际上，由Sun主导的标准化组织JCP（Java Community Process），该组织根据领域的不同，分为三个大的工作方向，即J2EE，J2SE和J2ME。
# 6．N个规范JSR
# 7．两大配置
CDC与LCDC
# 8．两个重要规范
LCDC1.1与MIDP2.0
# 9．一个对比数据
|**设备**|**名称**|**配置**|**运行在这一配置上的程序的名称**|**虚拟机**|
|----|----|----|----|----|
|CD|联接设备|CDC|J2ME Application|CVM|
|CLD|有限联接设备|CLDC|MIDlet|KVM|
# 10．一个叫法
**SUN****称在MIDP****上的应用程序为MIDlet**
你是不是感到有些眼熟，想想java中的Applet。
