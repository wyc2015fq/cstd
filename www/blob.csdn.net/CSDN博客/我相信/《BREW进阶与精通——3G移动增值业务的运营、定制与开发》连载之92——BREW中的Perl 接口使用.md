# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之92——BREW中的Perl 接口使用 - 我相信...... - CSDN博客





2010年11月30日 15:17:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1208








BREW 中提供了Perl 接口库，开发者可以通过Perl接口编写脚本，来实现批处理命令的自动化执行。其中的API 列表包括：

•连接和控制目标手机

uConnecttoDevice(char* portName, char* dllName)

uPutDeviceOffline(int)

uResetDevice()

•复制文件和目录

uCopyFileToDevice(char* src, char* dest, bool overwrite)

uCopyFileFromDevice(char* src, char* dest)

uCopyDirDevice(const char* pPCDirName, const char* pDeviceDirName)

•记录手机的日志信息

uSetLogMask(char* logMask)

uSetLogDuration(unsigned long x)

uStartLogger()

•模拟手机的按键事件

uStartGrinder(char* targetName)

uSetEventInterval(long lEventsInterval)

uSendKeys(int count, char **eventCodes)

下面是Grinder与BTIL结合使用的脚本示例。

use GrinderPl;

use enum qw( AVK_ASTERISK=1000 AVK_NUMBER_SIGN AVK_SELECT AVK_UP AVK_DOWN AVK_LEFT AVK_RIGHT AVK_SEND AVK_CLR AVK_END);

use enum qw( NORMAL=0 NO_VOICE_CALL AFTER_VOICE_CALL CONFIG_SCREEN=4 HIDDEN_CONFIG=8 FORCE_UNIQUE=32 SCR_SAVER=64 BACKGROUND=160);

#

# A Grinder perl application to configure Shaker on the phone.

#

#1) It initializes the perl module.

#2) Loads the default settings.

#3) Initializes the phone.

#4) Starts the BREW applet with flag to start it in background.

#5) Releases the perl module.

#

#Initialize the perl module. This should be the first function to be called.

GrinderPl::InitPerlModule();



#Load the default settings. 

GrinderPl::LoadSettings(0);

if(GrinderPl::InitDeviceEx("COM1", "BTILOEM.dll")) {

$i = 0x12345678;# Id of Applet to be started

GrinderPl::BTILStartApplet($i, BACKGROUND);

}

GrinderPl::ReleasePerlModule();



