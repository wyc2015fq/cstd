# USB标准请求命令 - xqhrs232的专栏 - CSDN博客
2018年01月29日 17:10:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：279
原文地址::[http://blog.csdn.net/alien75/article/details/4625021](http://blog.csdn.net/alien75/article/details/4625021)
相关文章
1、USB标准请求----[https://wenku.baidu.com/view/c6b613f7f61fb7360b4c6576.html](https://wenku.baidu.com/view/c6b613f7f61fb7360b4c6576.html)
2、USB标准请求命令----[https://wenku.baidu.com/view/728e945c804d2b160b4ec0ac.html?mark_pay_doc=0&mark_rec_page=1&mark_rec_position=4&clear_uda_param=1](https://wenku.baidu.com/view/728e945c804d2b160b4ec0ac.html?mark_pay_doc=0&mark_rec_page=1&mark_rec_position=4&clear_uda_param=1)
3、标准的USB设备请求命令----[https://wenku.baidu.com/view/6bc18a1aff00bed5b9f31d1f.html?mark_pay_doc=0&mark_rec_page=1&mark_rec_position=3&clear_uda_param=1](https://wenku.baidu.com/view/6bc18a1aff00bed5b9f31d1f.html?mark_pay_doc=0&mark_rec_page=1&mark_rec_position=3&clear_uda_param=1)
标准的USB设备请求命令是用在控制传输中的“初始设置步骤”里的数据包阶段（即DATA0，由八个字节构成）。标准USB设备请求命令共有
11个，大小都是8个字节，具有相同的结构，由5 个字段构成（字段是标准请求命令的数据部分），结构如下（括号中的数字表示字节数，首字
母bm,b,w分别表示位图、字节，双字节）：
bmRequestType(1)+bRequest(1)+wvalue(2)+wIndex(2)+wLength(2)
各字段的意义如下：
1、bmRequestType：D7D6D5D4D3D2D1D0
     D7=0主机到设备
       =1设备到主机；
     D6D5=00标准请求命令
         =01 类请求命令
         =10用户定义的命令
         =11保留值
     D4D3D2D1D0=00000 接收者为设备
               =00001 接收者为设备
               =00010 接收者为端点
               =00011 接收者为其他接收者
               =其他 其他值保留
2、bRequest：请求命令代码，在标准的USB命令中，每一个命令都定义了编号，编号的值就为字段的值，编号与命令名称如下（要注意这里的命令代码要与其他字段结合使用，可以说命令代码是标准请求命令代码的核心，正是因为这些命令代码而决定了11个USB标准请求命令）：
     1.获取状态 Get Status (00H)
       A:[To Device]获取设备的状态:
         *.位0:自供电(0表示总线供电;1表示自供电).
         *.位1:远程唤醒(0表示不支持远程唤醒;1表示远程唤醒).
         *.位2~15:保留.
         *.一般选择总线供电,不支持远程唤醒,所以返回数据就是0x0000.
       B:[To Interface]获取接口的状态:
         *.接口状态的16位字节全部保留,所以返回数据就是0x0000.
       C:[To Endpoint]获取端点的状态:
         *.位0:Halt(0表示端点允许;1表示端点禁止).
         *.位1~15:保留(复位为0).
     2.清除特性 Clear Feature (01H)
       A:[To Device]清除设备的远程唤醒功能,并返回一个空包.
       B:[To Endpoint]解禁端点.
     3.设置特性 Set Feature (03H)
       A:[To Device]设置设备的远程唤醒功能,并返回一个空包.
       B:[To Endpoint]禁止端点.
     4.设置地址 Set Address (05H)
       A:设置设备地址.
     5.获取描述符 Get Descriptor (06H)
       A:[To Device]获取设备描述符:
         *.描述当前USB协议的版本号.设备端点0的FIFO大小.USB设备的ID号等.
       B:[To Configuration]获取配置描述符:
         *.描述USB设备接口个数及是否有自供电能力等.
       C:[To Interface]获取接口描述符:
         *.描述端点0以外的物理端点个数等信息.
       D:[To Endpoint]获取端点描述符:
         *.描述端点0各端点的传输类型和最大信息包大小和端点的传输方向(IN/OUT).
     6.设置描述符(可选,无法更新) Set Descriptor (07H)
     7.获取配置信息 Get Configuration (08H)
     8.设置配置 Set Configuration (09H)
       A:[To Configuration]设置配置描述符.
       B:[To Interface]设置接口描述符.
       C:[To Endpoint]设置端点描述符.
      9.获取接口信息 Get Interface (0AH)     
     10.设置接口 Set Interface (0BH)
     11.SYNCH_FRAME(0CH)
        用于设备设置和报告一个端点的同步帧.
     标准的USB请求命令中的Descriptor
     Descriptor即描述符，是一个完整的数据结构，可以通过C语言等编程实现，并存储在USB设备中，用于描述一个USB设备的所有属性，USB主机是通过一系列命令来要求设备发送这些信息的。它的作用就是通过问答节中的命令Get Descriptor作来给主机传递信息，从而让主机知道设备具有什么功能、属于哪一类设备、要占用多少带宽、使用哪类传输方式及数据量的大小，只有主机确定了这些信息之后，设备才能真正开始工作，所以描述符也是十分重要的部分，要好好掌握。标准的描述符有5种，USB为这些描述符定义了编号：
     1——设备描述符
     2——配置描述符
     3——字符描述符
     4——接口描述符
     5——端点描述符
     上面的描述符之间有一定的关系，一个设备只有一个设备描述符，而一个设备描述符可以包含多个配置描述符，而一个配置描述符可以包含多个接口描述符，一个接口使用了几个端点，就有几个端点描述符。这间描述符是用一定的字段构成的

