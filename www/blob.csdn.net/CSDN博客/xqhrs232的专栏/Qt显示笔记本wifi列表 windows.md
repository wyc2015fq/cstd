# Qt显示笔记本wifi列表 windows - xqhrs232的专栏 - CSDN博客
2019年02月11日 15:15:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：72
原文地址::[https://blog.csdn.net/gaobobo138968/article/details/11696137](https://blog.csdn.net/gaobobo138968/article/details/11696137)
相关文章
1、使用Qt库时出现winsock2.h和windos.h包含顺序错误----[https://blog.csdn.net/yangguihao/article/details/50619271](https://blog.csdn.net/yangguihao/article/details/50619271)
2、WLANAPI详解----[https://blog.csdn.net/softwave/article/details/41389907](https://blog.csdn.net/softwave/article/details/41389907)
3、native wifi api 详解----[https://blog.csdn.net/xuanmuzhao/article/details/46128891](https://blog.csdn.net/xuanmuzhao/article/details/46128891)
4、Windows wlan简单操作----[https://blog.csdn.net/moyebaobei1/article/details/82494331](https://blog.csdn.net/moyebaobei1/article/details/82494331)
5、windows 获取的Wlan列表 怎么读取第二位及其以后的Wlan信息----[https://ask.csdn.net/questions/642299](https://ask.csdn.net/questions/642299)
6、关于wlan的一些东西----[https://blog.csdn.net/void_function/article/details/7832450](https://blog.csdn.net/void_function/article/details/7832450)
7、windows获取wifi的bssid（mac地址）进行wifi定位----[https://blog.csdn.net/gupar/article/details/50854480](https://blog.csdn.net/gupar/article/details/50854480)
8、如何在程序中获得wifi信息----[https://blog.csdn.net/PenyoHong/article/details/50630922](https://blog.csdn.net/PenyoHong/article/details/50630922)
关于怎样设计你程序显示自己电脑的wifi列表的问题，微软已经提供相关api.这里也是参考网上的资源在这里用Qt实现。
首先用到的几个类：
    PWLAN_INTERFACE_INFO_LIST    
    PWLAN_INTERFACE_INFO        
    PWLAN_AVAILABLE_NETWORK_LIST 
    PWLAN_AVAILABLE_NETWORK     
    WLAN_CONNECTION_PARAMETERS 
关于这几个类得说明以及使用msdn上都有描述。这里直接上代码：
wifi::wifi(QWidget *parent, Qt::WFlags flags)
 : QMainWindow(parent, flags)
{
 ui.setupUi(this);
 PWLAN_INTERFACE_INFO_LIST    pIfList = NULL;
    PWLAN_INTERFACE_INFO         pIfInfo = NULL;
    PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
    PWLAN_AVAILABLE_NETWORK      pBssEntry = NULL;
 unsigned long  dwResult,dwCurVersion,dwMaxClient=2;
 void *hCilent = NULL;
  dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion,&hCilent);
    if (dwResult != ERROR_SUCCESS) {
        wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
        return ;
    }
  dwResult = WlanEnumInterfaces(hCilent, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS)
 {
       // wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        return ;
        // You can use FormatMessage here to find out why the function failed
    }
 else
 {
  for (int i = 0; i < (int) pIfList->dwNumberOfItems; i++)
  {
            pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
  }
 }
    dwResult = WlanGetAvailableNetworkList(hCilent, &pIfInfo->InterfaceGuid,0, NULL, &pBssList);
    if (dwResult != ERROR_SUCCESS)
 {
        //wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n",dwResult);
        dwResult = 1;
        // You can use FormatMessage to find out why the function failed
    } else {
        //wprintf(L"WLAN_AVAILABLE_NETWORK_LIST for this interface\n");
        // wprintf(L"  Num Entries: %lu\n\n", pBssList->dwNumberOfItems);
        for (int j = 0; j < pBssList->dwNumberOfItems; j++) {
            pBssEntry = (WLAN_AVAILABLE_NETWORK *) & pBssList->Network[j];
            // wprintf(L"  Profile Name[%u]:  %ws\n", j, pBssEntry->strProfileName);
   qDebug()<<pBssEntry->strProfileName;
   char *ch = UnicodeToAnsi(pBssEntry->strProfileName);
   QString str;
   str.append(ch);
   qDebug()<<str;
   char *p;
    if (pBssEntry->dot11Ssid.uSSIDLength != 0)
     {
      p = (char*) pBssEntry->dot11Ssid.ucSSID;
      QString str1;
      str1.append(p);
      qDebug()<<str1;
    }
            }
 }
}
wifi::~wifi()
{
}
关于连接请参考msdn  http://msdn.microsoft.com/en-us/library/windows/desktop/ms706613(v=vs.85).aspx
PS:关于Qt怎么使用windows的api，这里笔者取了一下巧，就是用VS去写了Qt的程序，只要在项目属性连接器输入里加入wlanapi.lib;ole32.lib;就可以了，如果用QtCreater写的话，请参考http://bbs.csdn.net/topics/360069804
