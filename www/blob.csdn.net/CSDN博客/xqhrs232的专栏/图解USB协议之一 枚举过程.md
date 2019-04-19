# 图解USB协议之一 枚举过程 - xqhrs232的专栏 - CSDN博客
2018年02月09日 16:58:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：369
原文地址::[http://blog.csdn.net/myarrow/article/details/8270060](http://blog.csdn.net/myarrow/article/details/8270060)
相关文章
1、USB设备的枚举过程----[http://blog.csdn.net/cl2006ky/article/details/42025721](http://blog.csdn.net/cl2006ky/article/details/42025721)
2、用USB Monitor监视USB枚举（配置、识别）过程（USB枚举过程分析----[http://www.doc88.com/p-1071924478854.html](http://www.doc88.com/p-1071924478854.html)
## 0. 枚举流程
   • 连接了设备的 HUB 在 HOST 查询其状态改变端点 时返回对应的 bitmap，告知HOST 某个PORT状态发生了改变。 
   • 主机向 HUB 查询该PORT的状态，得知有设备连接，并知道了该设备的基本特性。  
   • 主机等待（至少 100mS）设备上电稳定，然后向 HUB 发送请求，复位并使能该PORT。 
   • HUB执行PORT复位操作，复位完成后该PORT就使能了。现在设备进入到defalut状态，可以从Vbus获取不超过 100mA 的电流。主机可以通过 0地址与其通讯。 
   • 主机通过0地址向该设备发送get_device_descriptor标准请求，获取设备的描述符。
   • 主机再次向 HUB 发送请求，复位该PORT。 
   • 主机通过标准请求 set_address给设备分配地址。 
   • 主机通过新地址向设备发送 get_device_descriptor标准请求，获取设备的描述符。 
   • 主机通过新地址向设备发送其他 get_configuration请求，获取设备的配置描述符。
   • 根据配置信息，主机选择合适配置，通过 set_configuration请求对设备而进行配置。这时设备方可正常使用。
## 1. GET_DESCRIPTOR
      总线复位及向默认地址0发送GET_DESCRIPTOR指令包，请求设备描述。如（图一）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354869515_5938.PNG)
（图一）
1）Index[4 - 5]：表示USB插入总线复位
2）Index[7 - 8]：表示主机向默认地址发送GET_DESCRIPTOR指令包，详细信息也抓出来了，如（图二）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354872345_6448.gif)
（图二）
3）Index[15 - 17]：表示设备向主机发送设备描述数据Index[16]
4）Index[18 - 19]：表示主机完成GET_DESCRIPTOR指令后，给设备发送一个空应答；
## 2. SET_ADDRESS
     再次复位总线及向设备发送SET_ADDRESS指令包，设置设备地址。如（图三）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354872462_9853.gif)
（图三）
1）Index[22 - 23]：表示再次总线复位，该复位自动完成，不是手工插拔USB完成
2）Index[25 - 27]：表示主机向默认地址发送SET_ADDRESS指令包，详细信息如（图四）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354872558_2146.gif)
（图四）
3）Index[29 - 31]：表示设备完成SET_ADDRESS指令后，给主机发送一个空应答；
## 3. GET_DESCRIPTOR<请求设备描述>
    向第二步设定的设备地址发送GET_DESCRIPTOR指令包，请求设备描述。如（图五）所示：
[](http://space.ednchina.com/upload/2010/1/6/49f3fe96-e189-455f-bb3f-db25784de77d.GIF)
![](https://img-my.csdn.net/uploads/201212/07/1354872658_6697.gif)
（图五）
1）Index[33 - 35]：表示主机向地址01发送GET_DESCRIPTOR指令包，详细信息见（图六）
![](https://img-my.csdn.net/uploads/201212/07/1354872698_1437.gif)
（图六）
2）Index[41 - 43]：表示设备向主机发送设备描述数据Index[42]
3）Index[45 - 47]：表示设备向主机发送设备描述数据Index[46]
4）Index[48 - 50]：表示主机完成GET_DESCRIPTOR指令后，给设备发送一个空应答；
## 4. GET_DESCRIPTOR<请求配置描述>
向第二步设定的设备地址发送GET_DESCRIPTOR指令包，请求配置描述。如（图七）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354872768_4003.gif)
（图七）
1）Index[52 - 54]：表示主机向地址01发送GET_DESCRIPTOR指令包，详细信息见（图八）
![](https://img-my.csdn.net/uploads/201212/07/1354872825_4116.gif)
（图八）
2）Index[57 - 59]：表示设备向主机发送配置描述数据Index[58]
3）Index[60 - 62]：表示主机完成GET_DESCRIPTOR指令后，给设备发送一个空应答；
## 5. 读取完整设备描述及配置描述
       重复向第二步设定的设备地址发送GET_DESCRIPTOR指令包，取完所有设备描述及配置描述；
## 6. SET_CONFIGURATION
       向第二步设定的设备地址发送SET_CONFIGURATION指令包，设置配置描述。如（图九）所示：
![](https://img-my.csdn.net/uploads/201212/07/1354873022_3727.gif)
（图九）
1）Index[139 - 141]：表示主机向地址01发送SET_CONFIGURATION指令包，详细信息见（图十）
![](https://img-my.csdn.net/uploads/201212/07/1354873065_9959.gif)
（图十）
2）Index[143 - 145]：表示设备完成SET_CONFIGURATION指令后，给主机发送一个空应答
至此，枚举过程结束，设备可通过驱动与主机通信了。
       以上的USB枚举过程是根据现有特定设备捕获得到，并不完整涉及到USB协议的所有指令包，如GET_STATUS指令包、CLEAR_FEATURE指令包、SET_FEATURE指令包、SET_DESCRIPTOR指令包、GET_CONFIGURATION指令包、GET_INTERFACE指令包、SET_INTERFACE指令包、SYNCH_FRAME指令包等。
转自：[http://forum.eet-cn.com/BLOG_ARTICLE_2806.HTM?jumpto=view_welcomead_forum_1354841784500](http://forum.eet-cn.com/BLOG_ARTICLE_2806.HTM?jumpto=view_welcomead_forum_1354841784500)

