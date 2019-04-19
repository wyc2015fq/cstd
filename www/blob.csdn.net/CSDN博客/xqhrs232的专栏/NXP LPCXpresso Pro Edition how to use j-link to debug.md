# NXP LPCXpresso Pro Edition how to use j-link to debug - xqhrs232的专栏 - CSDN博客
2017年04月08日 13:29:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：875
原文地址::[https://www.segger.com/nxp-lpcxpresso.html](https://www.segger.com/nxp-lpcxpresso.html)
相关文章
1、Which debug probes are supported by LPCXpresso IDE with which MCUs?----[https://community.nxp.com/message/630901](https://community.nxp.com/message/630901)
2、LPCXpresso with Segger J-Link----[https://www.lpcware.com/content/forum/lpcxpresso-segger-j-link](https://www.lpcware.com/content/forum/lpcxpresso-segger-j-link)
3、How to install the GNU ARM Eclipse plug-ins?----[http://gnuarmeclipse.github.io/plugins/install/](http://gnuarmeclipse.github.io/plugins/install/)
4、J-Link / J-Trace Downloads----[J-Link / J-Trace Downloads](J-Link)[J-Link / J-Trace Downloads](/)[J-Link
 / J-Trace Downloads](J-Trace)[J-Link / J-Trace Downloads](Downloads)
5、LPCXpresso and J-Link: I can't start a debug session----[https://www.lpcware.com/content/forum/lpcxpresso-and-j-link-i-cant-start-debug-session](https://www.lpcware.com/content/forum/lpcxpresso-and-j-link-i-cant-start-debug-session)
6、Could not determine GDB version after sending: C:Program Files\SEGGER\jLink_V502d\JLinkGDBServerCL.exe----[https://www.lpcware.com/zh-hans/node/1113518](https://www.lpcware.com/zh-hans/node/1113518)

# NXP LPCXpresso Pro Edition
In order to get started with J-Link and [LPCXpresso](http://www.lpcware.com/lpcxpresso/download) by NXP install the latest [SEGGER software
 and documentation pack](https://www.segger.com/jlink-software.html). Please verify that your J-Link software is up to date. For detailed instructions on how to install and use LPCXpresso, please refer to the LPCXpresso [webpage](http://www.lpcware.com/lpcxpresso) and
 the getting started [guide](http://www.lpcware.com/system/files/LPCXpresso_User_Guide.pdf) by NXP.
## Debugging configuration options
For LPCXpresso there are two options how to use J-Link support:
- 
Using the [GNU ARM Eclipse Plugin](http://www.segger.com/IDE_Integration_Eclipse.html#Eclipse_GNU_ARM_Plugin) together with LPCXpresso.
- 
Using LPCXpresso as a vanilla Eclipse IDE by using the generic GDB Hardware Debugging plug-in.
SEGGER recommends the usage of option 1 (GNU ARM Eclipse Plugin) as it provides a much cleaner and more easy to use user front end to configure the J-Link connection in LPCXpresso as the standard Eclipse GDB Hardware Debugging plug-in. In the following, both
 methods are described.
## GNU ARM Eclipse plugin
Please read the instructions for setting up the [GNU ARM Eclipse Plugin](http://www.segger.com/IDE_Integration_Eclipse.html#Eclipse_GNU_ARM_Plugin). In addition to following the instructions
 on the GNU ARM website, you also have to add one preference.
- 
Open **Window | Preferences**
- 
Select Run/Debug
- 
Select String Substitution
- 
Click **New**
- 
Enter **cross_prefix** in the "Name:" field.
- 
Enter **arm-none-eabi-** in the "Value:" field.
- 
Click **OK**
The added preference allows the plugin to find the correct version of the GDB.
![](https://www.segger.com/cms/admin/uploads/imageBox/LPCXpresso_Debugging_Insert_Preference.jpg)
## GDB Hardware Debugging plug-in
First of all, the J-Link GDB Server needs to be started (included in the J-Link software suite). Please make sure that the CPU and Interface Type & Speed are set correctly.
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_JLinkGDBServer.PNG)
Open your current project in LPCXpresso.
Open "Debug Configurations" :
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_DebugConfigurations.PNG)
In order to create a new debugging entry, double click "GDB Hardware Debugging". The "Main" Tab should look as follows:
Please make sure the bottom line is set to "Using GDB(DSF) Hardware Debugging Launcher"  and your executable is selected in the field "C/C++ Application"
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_Main.PNG)
In the "Debugger" tab, the "GDB Command" field needs to be set  to the location of the gdb client shipping with LPCXpresso. The default path is **C:\nxp\LPCXpresso_*VersionNumber*\lpcxpresso\tools\bin\arm-none-eabi-gdb.exe**.
Assuming a local setup, default for "Host name" and "Port number"  is "localhost" and "2331".
The "Debugger" tab should look as follows:
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_Debugger.PNG)
Finally, the "Startup" tab should look as follows:
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_Startup.PNG)
Depending on the CPU used, you may have to augment the gdb commands in the field following the checkbox labeled "Halt". 
After saving the settings by pressing **Apply**, you can start a debug session immediately by clicking **Debug**.
![](https://www.segger.com/admin/uploads/imageBox/LPCXpresso_Debugging_v2.png)
