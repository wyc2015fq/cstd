# 用GetVolumeInformation得到的不是硬盘的序列号，不要再抄这样的错误好吗？ - weixin_33985507的博客 - CSDN博客
2005年02月27日 08:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
网上怎么到处泛滥这种错误？！！！ 
用API 函数GetVolumeInformation得到的不是硬盘的序列号！！！ 
BOOL GetVolumeInformation( 
  LPCTSTR lpRootPathName, 
  LPTSTR lpVolumeNameBuffer, 
  DWORD nVolumeNameSize, 
  LPDWORD lpVolumeSerialNumber, 
  LPDWORD lpMaximumComponentLength, 
  LPDWORD lpFileSystemFlags, 
  LPTSTR lpFileSystemNameBuffer, 
  DWORD nFileSystemNameSize 
); 
看看英文啊：VolumeSerialNumber!得到的只是卷区序列号！ 
硬盘的序列号应该是 
Drive Model Number________________: WDC WD400EB-00CPF0 
Drive Serial Number_______________: WD-WCAATF083586 
Drive Controller Revision Number__: 06.04G06 
Controller Buffer Size on Drive___: 2097152 bytes 
Drive Type________________________: Fixed 
Drive Size________________________: 40020664320 bytes 
中： 
Drive Serial Number_______________: WD-WCAATF083586 这才是硬盘Serial Number！！！！ 
这个号是不会因为你格式化硬盘而改动，也不是网上流传的修改工具能改的，（其实网上流传的修改工具的也不过是卷区号而已，真是哭笑不得！） 
搜吧，搜搜GetVolumeInformation吧，看看有多少抄来抄去的错误吧！.... 
