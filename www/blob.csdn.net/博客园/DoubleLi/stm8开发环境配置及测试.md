# stm8开发环境配置及测试 - DoubleLi - 博客园






需要准备的软件，硬件，IAR for stm8 （EWSTM8）、stm8s标准固件库、ST-LINK、STM8s003f3核心板



安装IAR（其中包括st-link的驱动），

到这个网址下载stm8s标准固件库（STSW-STM8069）,http://www.st.com/web/en/catalog/tools/PF258009

使用IAR新建一个工程，具体步骤如下：

1,File->New->Workspace

2,创建一个工程：Project->Create New Project...

3,在弹出的对话框中选择C，-》OK

4，根据提示保存你的工程

5，从stm8s标准固件库中复制Libraries目录到你的工程目录，复制\STM8S_StdPeriph_Lib_V2.1.0\Project\STM8S_StdPeriph_Examples\GPIO到你的工程目录

6，IAR的工程配置：在工程中添加两个组Libraries，MyApp。右击你的工程->Add->Add Group

7，将库中的*.c文件和主程序目录的*.c文件分别添加到Libraries, MyApp;右击相应组->add->add files

8，选择单片机型号：右击你的工程Options->General Options->选择你单片机的型号

9，配置头文件搜索路径：上一步切换到C/C++ Complier->Preprocessor->Additional include directory: ( one per line)

10,选择调试器：切换到Debugger->Driver选择ST-Link->OK

现在开始编译拍错

1，右键main.c->Comliper会提示保存workspace，按提示操作，编译中会提示没有定义

Fatal Error[Pe035]: #error directive: "Please select first the target STM8S/A device used in your application (in stm8s.h file)" C:\Documents and Settings\Administrator\桌面\EWSTM8_turial\Libraries\STM8S_StdPeriph_Driver\inc\stm8s.h 65 

双击该条提示，自动定位到错误位置，表示没有定义单片机型号，定义上

#define STM8S003F3



2，再次执行1，这次的提示变为

Error[Pe020]: identifier "GPIOH" is undefined C:\Documents and Settings\Administrator\桌面\EWSTM8_turial\GPIO\GPIO_Toggle\main.c 54 

双击该提示，定位到错误位置，向上找到 #define LED_GPIO_PORT  (GPIOH) 

将GPIOH改为GPIOD（STM8S003F3没有GPIOH这个IO）



3，再次执行1，OK没有错误

4，整体编译一遍：右击工程->Make.

在编译过程中有一部分文件会报错，不要担心，这些是STM8S003F3中没有的资源，将报错的文件删除即可

另外对固件库了解后，可以只添加需要的部分，比如这次使用的是stm8s_gpio.c,那么只添加stm8s_gpio.c就可以了

5，删除不需要的，再编译：OK没有错误，没有警告

下载并调试，

1，将STM8核心板通过SWIM与ST-Link连接起来，注意线序

2，将ST-Link连接到电脑，（在设备管理器中usb总线中会看到ST-link的相关信息）

3，给stm8核心板加电

4，点击IAR的Download and debug，点击Go。核心板上的灯会闪烁









