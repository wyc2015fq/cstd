# 寄存器PLC地址与寄存器modbus协议地址 - 深之JohnChen的专栏 - CSDN博客

2017年10月24日 22:25:28[byxdaz](https://me.csdn.net/byxdaz)阅读数：25909


       寄存器PLC地址指存放于控制器中的地址，这些控制器可以是PLC，也可以使触摸屏，或是文本显示器。PLC地址一般采用10进制描述，共有5位，其中第一位代码寄存器类型。第一位数字和寄存器类型的对应关系如表1所示。PLC地址例如40001、30002等。

       寄存器modbus协议地址指通信时使用的寄存器地址，在实际编程中，由于寄存器PLC地址前缀的区分作用，所以只需说明后4位数，而且需转换为4位十六进制地址。例如PLC地址40001对应寻址地址0x0000，40002对应寻址地址0x0001，寄存器寻址地址一般使用16进制描述。再如，PLC寄存器地址40003对应协议地址0002，PLC寄存器地址30003对应协议地址0002。在实际编程中，由于前缀的区分作用，所以只需说明后4位数，而且需转换为4位十六进制地址。

       支持 Modbus 协议的设备或软件，使用时用户直接设置或看到的应 当是 Modbus 数据地址。Modbus 地址所访问的数据，是通过各种 “功能”读写而来。 功能码是 Modbus 地址的底层。 如果 Modbus 通 信的一方提供的所谓 Modbus 协议只有功能码，则需要注意了解此 功能号与 Modbus 地址间的对应关系。

       使用 modbus 地址时应注意下述问题：

 1）40001~4xxxx 是美国 modicon 公司和 ge 公司 plc 使用的 modbus 地址，它是基于1的地址，即同类元件的首地址为1。西门子 plc 的 modbus 地址是基于0的地址。 

2）美国的 modbus 地址左起第2位用来表示元件的类型，例如 i0.0的 modbus 地址为010001。 因为数据类型已经包含在功能码中了， 西门子的 i0.0的 modbus 地址实际上为000000， i2.0的 modbus 地址为 000016（或十六进制数16#0010） ，而不是010017。

3）、无论V寄存器还是M寄存器，里面的不同数据类型（VB、VW、VD、MB、MW、MD）都是基于寄存器内存总字节地址为偏移地址。

![](https://img-blog.csdn.net/20171028111534922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20171028111547443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4）、在数据报文中,所有的modbus地址都是从0开始的。也就是首次出现的数据项在报文中的地址为0。比如：（All data addresses in Modbus messages are referenced to zero. The first occurrence of a data item is addressed as item number zero.）

5）、Modbus地址实际上分为两种情况。

第一种情况：PLC作Modbus主站，Modbus地址和PLC手册里的地址一致，例如作主站的S7-200的MBUS_MSG指令用于向Modbus从站发送请求消息，和处理从站返回的响应消息。要读取从站（另一台S7-200）的I0.0开始的地址区时，它的输入参数Addr（Modbus地址）为10001。S7-200从站保持寄存器的V区起始地址为VB200时，要读取从站VW200开始的V存储区时，保持寄存器的地址是40001。

第二种情况：PLC作从站，PLC不用管什么Modbus地址，等着主站来读写它的地址区就是了。

主站的计算机软件（例如DCS或组态软件）的编程人员需要编写实现Modbus通信的程序，首先需要确定Modbus RTU的报文结构。他们一般不熟悉PLC，因此PLC的编程人员往往需要和上位机软件的编程人员一起来讨论Modbus的报文结构。

最容易出问题的就是报文里Modbus地址与PLC存储区地址的对应关系。

S7 PLC手册给出的Modbus地址与Modicon公司和GE公司PLC使用的地址相同，是基于1的地址，即同类元件的首地址为1。而西门子PLC采用的是基于0的地址，即同类元件的首地址为0。Modbus报文中西门子PLC的Modbus地址也采用基于0的地址。

PLC系统手册中的Modbus地址的最高位用来表示地址区的类型，例如I0.0的Modbus地址为10001。因为地址区类型的信息已经包含在报文的功能码中了，报文中S7-200的I0.0的Modbus地址不是10001，而是0。报文中其他地址区的Modbus地址也应按相同的原则处理。例如当S7-200从站保持寄存器的V区起始地址为VB200时，VW200对应的保持寄存器在报文中的Modbus地址为0，而不是40001。

施耐德plc modbus地址对照表

000001至000XXX是实际输出，对应%QX0.0-%QXX.X

010001至010XXX是实际输入，对应%IX0.0-%IXX.X

030001至030032是模拟量寄存器，对应%AIW0，16位一路依次类推

040001至040XXX是保持寄存器，对应%MX0.0-%MXX.X，1对1，要是%MW就对应16位，依次类推。施耐德PLC中  W表示字 16位，D表示双字 32位。比如：%MW10和%MD10。B表示字节 8位，X表示位。比如：%MB0和%MX0.0~%MX0.7

%MW10.4

%MW10是指一个字，一个字分为2个字节，一个字节8个位，也就是说%MW10中有16个位，从%MW10.0~%MW10.15。

%QW0.5.1.0.10

Q是输出 ，0是机架，5是0号机架的第五个模块，1是第五号模块的第一个通道，0是第一个通道的第0个字，10是第0个字的第10个位。

![](https://img-blog.csdn.net/20171028113841678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

无论MX、MB、MW、MD都是基于内存总字节地址为偏移地址，也就是以%MB之后的地址为偏移地址。

施耐德plc寄存器地址对应的modubs地址

寄存器地址 modubs 说明 公式

%QX1.0 000001.0 开关量输出 %QXx.y=000001+x.y

%IX1.0 010001.0 开关量输入 %IXx.y=010001+x.y

%AIW0 030001 模拟量输入 %AIWx=030001+x

%MD40 0400801 保持寄存器 %MDx.y=040001+x*2.y

%MW50.0         040051.0 保持寄存器 %MWx.y=040001+x.y

%MX100.0 040051.0 保持寄存器 %MXx.y=040001+x/2.y

注意：

1、%MX100.0表示某个字节地址，因为%MW50这个地址中包含了%MB100、%MB101，%MX100.0其实是%MW50的第0位即40051.0

2、%MD40表示双字，%MD40对应的首地址是%MW*2，也就是%MW80，%MD40对应的modbus地址为0400801

西门子s7系列plc modbus地址对照表
![](https://img-blog.csdn.net/20171024223111897)

        例如：地址：40040；对应S7200地址:VW78;对应S7300地址:DB1.DBW78;

三菱FX系列PLC modbus地址对照表

![](https://img-blog.csdn.net/20171024223217157)

OMRON PLC地址转MODBUS协议对照表

将OMRON PLC接入支持MODBUS协议的上位DCS系统中，通过ASC模块可满足用户的这一需求。具体方法参考：

[https://wenku.baidu.com/view/3ff1d16da5e9856a5712601d.html](https://wenku.baidu.com/view/3ff1d16da5e9856a5712601d.html)

台达PLC寄存器地址与Modbus寄存器地址对照表
[https://wenku.baidu.com/view/f884db6d102de2bd97058836.html](https://wenku.baidu.com/view/f884db6d102de2bd97058836.html)

永宏PLC寄存器Modbus地址表
[https://wenku.baidu.com/view/4843e77c30126edb6f1aff00bed5b9f3f90f72c8.html](https://wenku.baidu.com/view/4843e77c30126edb6f1aff00bed5b9f3f90f72c8.html)

信捷PLC寄存器Modbus地址表

![](https://img-blog.csdn.net/20180407120001258)

