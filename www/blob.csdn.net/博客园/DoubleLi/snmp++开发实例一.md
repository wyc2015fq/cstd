# snmp++开发实例一 - DoubleLi - 博客园







## 1.官网下载

snmp开发，首先需要机器已经安装了snmp服务，这方面的资料网上比较完备，安装的时候注意每少一个文件，网上都可以下载到，这样可以自己形成一个包，供以后使用。只要最后snmp的服务开启就可以使用snmp服务了(刚开始trap服务可能是未开启的)

snmp++官网[http://www.agentpp.com/](http://www.agentpp.com/)，选择自己需要的开发包，我这里下载了snmp++v3.2.25.tar.gz，如果需要支持v3版本的数据传送功能，还需要下载libdes-l-4.01a。准备工作做完之后，我们的目的就是要生成需要的两个lib库文件。

## 2.编译生成libdes.lib

###    2.1在vs2010中新建一个win32项目，选择静态库，空项目，不需要编译头

### 2.2**将所有需要的头文件和源文件均加入进来，截图如下：**

![](http://img.blog.csdn.net/20140904163956959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###    2.3 生成解决方案，这时会在工程目录下的debug目录下出现一个lib文件，正是我们需要的![](http://img.blog.csdn.net/20140904163826015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3.生成snmplib。

###    3.1再新建一个同样的win32项目，静态库，空项目，不需要编译头

###    3.2将解压后的snmp++/src目录下和libdes-l-4.01a下所有的.cpp都加入到源文件当中，同时还要将所有的.h文件加入，可以直接使用项目属性—C/C++--常规—附加包含目录，将头文件的目录加进来

![](http://img.blog.csdn.net/20140904164256827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://img.blog.csdn.net/20140904164410750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


###    3.3接下来，就生成解决方案，同样也会生成对应的lib库

![](http://img.blog.csdn.net/20140904164102078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 4.拷贝snmplib.lib,libdes.lib,ws2_32.lib

将生成的两个lib文件及下载的ws2_32.lib都拷贝到vs2010安装目录的lib文件夹下，方面后续snmp开发

## 5.建立测试项目

###    5.1新建项目，选择win32控制台应用程序TestCip。

###    5.2导入头文件。

因为要用到snmp，所以还需要将snmp所需要的头文件加入到附加包含目录中，方便起见，可以将libdes的几个头文件均拷贝到snmp++的include目录下，而include下的snmp_pp文件夹是snmp单独的头文件，这样就只需引入一次头文件就可以了。


![](http://img.blog.csdn.net/20140904164529177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



### 5.3**加入lib库。**

在项目属性中，链接器/输入中加入这样几项内容


![](http://img.blog.csdn.net/20140904164708222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



### 5.4编写测试程序




**[cpp]**[view plain](http://blog.csdn.net/zzj806683450/article/details/39054059#)[copy](http://blog.csdn.net/zzj806683450/article/details/39054059#)

[print](http://blog.csdn.net/zzj806683450/article/details/39054059#)[?](http://blog.csdn.net/zzj806683450/article/details/39054059#)

- #include <iostream>  
- #include "snmp_pp/snmp_pp.h"  
- 
- using namespace std;  
- 
- #define SYSDESCR "1.3.6.1.2.1.1.1.0" // Object ID for System Descriptor  
- 
- 
- void get_system_descriptor(){  
- 
- int status;//return status  
-     CTarget ctarget((IpAddress)"192.168.1.111");  
-     Vb vb(SYSDESCR);//SNMP++ variable Binding  
-     Pdu pdu;//SNMP ++ pdu  
- //--------[Construct a SNMP++ SNMP Object--------------  
-     Snmp snmp(status);//create a snmp  session  
- if (status != SNMP_CLASS_SUCCESS)  
-     {  
-         cout << snmp.error_msg(status);//if fail,print error string  
- return;  
-     }  
- //---------[Invoke a SNMP++ Get]------------------------  
-     pdu += vb;//add the variable binding  
- if ((status = snmp.get(pdu,ctarget)) != SNMP_CLASS_SUCCESS)  
-     {  
-         cout << snmp.error_msg(status);  
-     }  
- else {  
-         pdu.get_vb(vb,0);//extract the variable binding  
-         cout << "System Descriptor = " << vb.get_printable_value();  
-     }  
- 
- }  

main函数，这里就不写了，直接调用这个函数就OK

## 6.运行结果

![](http://img.blog.csdn.net/20140904163800333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenpqODA2NjgzNDUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




- 顶
4- 踩










