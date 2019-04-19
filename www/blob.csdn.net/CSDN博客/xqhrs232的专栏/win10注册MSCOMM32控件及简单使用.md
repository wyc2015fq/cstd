# win10注册MSCOMM32控件及简单使用 - xqhrs232的专栏 - CSDN博客
2018年03月30日 21:13:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1302
原文地址::[https://www.jianshu.com/p/5c54338b67b2](https://www.jianshu.com/p/5c54338b67b2)
相关文章
1、解决Win10中MSCOMM32.ocx没注册问题----[https://blog.csdn.net/mylovepwy/article/details/50239103](https://blog.csdn.net/mylovepwy/article/details/50239103)
2、[解决在win10
 64位下无法注册mscomm32.ocx的控件问题](http://www.cnblogs.com/iotrookie/p/6076656.html)----[https://www.cnblogs.com/iotrookie/p/6076656.html](https://www.cnblogs.com/iotrookie/p/6076656.html)
## 前言 & 吐槽
前几天重装了系统，然后之前写的用**MsComm控件**（Microsoft Communications Control）进行串口通信的程序无法运行了，用VS打开项目进入资源视图，发现打不开包含MsComm控件的对话框资源，提示未在此计算机上注册ActiveX控件。而VS工具箱提供的COM组件里也没有MsComm控件。
根本原因是这个ActiveX控件根本不是win10安装自带的，高版本VS也不会自带的，你要写这种串口通信程序，要么用底层API，要么用微软大力支持的.Net系语言，而不是MFC这个非常非常过时的破东西了，然而就这个破东西，很多时候还是被迫得写，嗯，前几天才看到2018年的二级C/C++上机环境终于从万年不变的VC6变成了VS2010，简直了。
## 注册MsComm控件
首先给下载地址，其实谷歌MsComm.ocx第一条就是
[https://www.ocxme.com/files/mscomm32_ocx](https://link.jianshu.com?t=https%3A%2F%2Fwww.ocxme.com%2Ffiles%2Fmscomm32_ocx)
当然国内用百度的多，一堆文章全部都指向了CSDN花几个积分的下载链接，好点的有百度盘链接，不过天知道过段时间会不会被和谐。还是这种专门的免费下载站更舒心。
下完解压后就是MSCOMM32.OCX，然后需要注意（我踩了这个坑），对于64位系统需要把它放在`C:\Windows\SysWOW64`目录下，32位系统才是`C:\Windows\system32`目录。之前我放在system32目录然后注册出了问题。
然后右键管理员权限打开cmd窗口，进入对应目录输入指令`regsvr32 MSCOMM32.OCX`即可（如下图所示）
网上有些教程还有一步是修改注册表，不过我发现regsvr32命令已经修改了注册表，无需手动修改。
## 为何使用MsComm控件？
其实直接用底层API进行串口编程未尝不可，MSDN也给出了示例，但是事件回调的步骤得自己写，如果不是时间充裕用来学习/练手的情况，没必要重复造轮子。
网上也可以搜到不少包装好的C++类，但是串口这东西本来就是古老物了，搜到的代码还是很多年前的，代码风格不一定很好。而且有的接口已经和现代C++标准不兼容了。
比如之前师兄用了一个简单小巧的库（只包含1个头文件和1个源文件，添加进工程即可）提供了这样的接口
```cpp
```cpp
void        WriteToPort(char* string);
```
```
然而C++11标准在近几个版本的VS里已经得到了支持，[字符串字面值](https://link.jianshu.com?t=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FString_literal)是不能直接转换成char*类型的，也就是说实际调用的时候得像这样
`xxx.WriteToPort((char*)"hello")`或`xxx.WriteToPort(const_cast<char*>("hello"))`
然后错误信息字符串操作全都是基于char*的，MFC默认Unicode，而且方便移植的代码应该都对TCHAR*来操作，直接编译会出错的，需要一个个用宏_T()把字符串包含起来。当然，会正则表达式的话起来替换相对会比较轻松。
我对MsComm控件不甚了解，但是明显这也是古老物了，毕竟还不支持64位程序，VS里用Debug或Release x64来编译的话会失败。
但是好处在于，这个控件是官方的，值得信赖，不像很多开源库那样缺乏大量测试。
## 为何不使用MsComm控件？
这篇文章是2018-01-17发布的，我在2018-02-01完成课题程序时发现了问题。正如上文所言，MsComm控件是古老物了，只适用于32位的程序。如果程序必须编译成64位的，那么MsComm控件无法派上用场。我的程序里需要用到OpenCV和一个第三方库，两者刚好都只提供了64位的lib和dll，所以只有使用MsComm控件，后来使用了上文提到的[CSerialPort](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fitas109%2FCSerialPort)，才发现这个古老版本原来国内有不少人进行了维护，现在还是非常好用的，强力推荐。
## 添加MsComm控件
首先需要在VS中添加该控件到工具箱中

按上述操作点确定即可
PS：可以看到版本才1.1，还是version 6.0，目测从VC6之后再也没更新过……
然后在对话框资源编辑框中右键，插入ActiveX控件


不同于常规的界面控件，MsComm控件不会显示出来，所以随便拖到哪个位置都可以。
拖了控件之后就是为控件添加变量了

这一步，VS会自动生成一对.h和.cpp文件，然后在xxxDlg.h中添加成员变量
`CMscomm1 m_comm1;`
并在xxxDlg.cpp中添加数据交换操作
```cpp
```cpp
void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSCOMM1, m_comm1);
}
```
```
可以发现和使用其他控件一样的套路。
## 从串口发送和接收字节
文本框/复选框这种自定义参数输入的控件就不祥述了，以我的需求为例
> 
波特率: 9600 停止位: 1 传送位数: 8位 奇偶校验: 无
串口协议是7个字节表示1个数据包
MsComm控件类型为CMscomm_las，变量名为m_commLaser
- 打开/关闭串口
```php
```php
m_commLaser.put_CommPort("COM1");
        m_commLaser.put_InputMode(CMscomm_las::comInputModeBinary);
        m_commLaser.put_InBufferSize(512);  // 接收缓冲区大小
        m_commLaser.put_OutBufferSize(512);  // 发送缓冲区大小
        m_commLaser.put_Settings(_T("9600,n,8,1"));
        if (!m_commLaser.get_PortOpen())
        {
            try {
                m_commLaser.put_PortOpen(TRUE);  // 打开串口
            }
            catch (CException* e) {
                TCHAR error_msg[1024];
                e->GetErrorMessage(error_msg, 1024);
                MessageBox(error_msg);
                return;
            }
            m_commLaser.put_RThreshold(7);  // 每当接收缓冲区有7个字符时则接收串口数据
            m_commLaser.put_InputLen(0);
            m_commLaser.get_Input();
        }
        else
        {
            MessageBox(_T("打开端口失败!"));
        }
```
```
注意put_Settings的参数，n代表无奇偶校验，是[DCB结构](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fzh-cn%2Flibrary%2Fwindows%2Fdesktop%2Faa363214%28v%3Dvs.85%29.aspx)的Parity成员的可选取值NOPARITY的缩写，同理，奇校验ODDPARITY是o，偶校验EVENPARITY是e。
打开串口是put_PortOpen(TRUE)，关闭串口自然就是put_PortOpen(FALSE)。
- 发送字节序列
`void put_Output(VARIANT newValue)`
直接调用上述方法即可，问题来了，VARIANT类型是什么？这就是ActiveX控件的蛋疼之处，它的数据交换必须用Ole那一套来。
正常来说，发送给串口的都是字节序列，即uint8_t或BYTE数组。因此这里只需要知道怎么转换成VARIANT类型即可。
MFC是上古时期的产物，那个时候C++98标准都没确立，因此微软弄了一堆自定义的容器类，虽然除了CString外几乎都被C++标准库的STL取代了。但在这里，MFC的[CByteArray](https://link.jianshu.com?t=https%3A%2F%2Fmsdn.microsoft.com%2Fzh-cn%2Flibrary%2Fxew0w8h9.aspx)刚好能直接用来构造COleVariant对象，COleVariant继承自VARIANT（C结构体的typedef别名），仅仅是增加了若干方法，可以隐式类型转换。
比如发送3个字节0x01 0x02 0x04给串口的代码如下
```
```
CByteArray m_baSend;
    m_baSend.Add(0x01);
    m_baSend.Add(0x02);
    m_baSend.Add(0x04);
    m_commLaser.put_Output(COleVariant(m_baSend));
```
```
- 
接收字节序列
给MsComm控件添加OnComm事件处理程序即可

和其他控件一样，MsComm控件也是事件驱动，在后台接收数据，然后处理不同的事件
```cpp
```cpp
enum
{
    comEvSend = 1,
    comEvReceive = 2,
    comEvCTS = 3,
    comEvDSR = 4,
    comEvCD = 5,
    comEvRing = 6,
    comEvEOF = 7
}OnCommConstants
```
```
接收数据只需要处理comEvReceive事件即可，其他几个事件等真正有需求的时候再去处理。下面给出我接收7个字节的代码
```
void CMsCommDemoDlg::OnCommMscommLas()
{
    if (m_commLaser.get_CommEvent() == CMscomm_las::comEvReceive)
    {
        // 读取串口的接收缓冲区(之前打开串口时设置过缓冲区大小)
        COleSafeArray safearray_obj = m_commLaser.get_Input();
        // 填充数据到自定义缓冲区中
        const int BUFF_SIZE = 7;
        BYTE buffer[BUFF_SIZE];
        for (long i = 0; i < BUFF_SIZE; i++)
        {
            safearray_obj.GetElement(&i, &buffer[i]);
        }
        // TODO: 处理缓冲区buffer[]的数据
    }
}
```
这里就和发送数据反过来了，接收的数据类型是VARIANT，需要转换成BYTE数组来处理。套路就是借助COleSafeArray这个中间物及其GetElement方法。
## 总结
其实这个程序是我几周之前写的Demo，最近要重新写了，照着我的代码写下来，自己也梳理了用MsComm控件的步骤。
控件方面，由于这东西实在太古老了而被抛弃了，而官方也没给C++提供什么替代品，于是得手动引入
- 注册MSCOMM32.OCX
- 在VS工具箱的COM组件中找到MsComm控件添加进来
- 像使用其他MFC控件一样使用它（添加关联的控件变量/事件处理函数）
代码方面，虽然COM接口的实现看起来非常复杂且蛋疼，但是其实接口很清晰，都是一堆put（设置）和get（获取）方法，注意打开/关闭串口也是通过putxxx来执行的，相当于设置串口的连接状态。
难点在于数据交换格式都是VARIANT，因此需要借用MFC专门提供的类来转换成方便处理的BYTE数组。大致过程如下
```
```
BYTE[] -> CByteArray -> COleVariant -> VARIANT
VARIANT -> COleSafeArray -> BYTE[]
```
```
小礼物走一走，来简书关注我
作者：哈莉_奎茵
链接：https://www.jianshu.com/p/5c54338b67b2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

