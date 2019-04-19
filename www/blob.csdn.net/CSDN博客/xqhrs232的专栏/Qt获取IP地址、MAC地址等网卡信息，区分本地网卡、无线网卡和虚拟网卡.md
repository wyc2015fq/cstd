# Qt获取IP地址、MAC地址等网卡信息，区分本地网卡、无线网卡和虚拟网卡 - xqhrs232的专栏 - CSDN博客
2018年07月12日 11:06:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：196
原文地址::[https://blog.csdn.net/caoshangpa/article/details/51073138](https://blog.csdn.net/caoshangpa/article/details/51073138)
1.只有一个本地网卡
        此时获取IP地址最简单的方法如下所示，但是这种方法只能获取IP地址。需包含头文件#include <QNetworkInterface>，.pro文件中加入network模块。
```cpp
```
- 
voidgetLocalIpWithQt()
- 
{
- 
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
- 
    foreach(QHostAddress ipItem, ipList)
- 
    {
- 
//只显示以192开头的IP地址
- 
if(ipItem.protocol()==QAbstractSocket::IPv4Protocol&&ipItem!=QHostAddress::Null
- 
                &&ipItem!=QHostAddress::LocalHost&&ipItem.toString().left(3)=="192")
- 
        {
- 
            qDebug()<<ipItem.toString();
- 
        }
- 
    }
- 
}
2.只有虚拟网卡和一个本地网卡
        安装了虚拟机后，获取网卡信息时，会自动获取到虚拟网卡的信息，但是我们大多数时候并不需要虚拟网卡信息，因此必须以虚拟网卡的MAC地址为条件将其过滤掉。如下所示，"00:50:56:C0:00:01"和"00:50:56:C0:00:08"是虚拟网卡的MAC地址。
```cpp
```
- 
voidgetAdapterInfoWithQt()
- 
{
- 
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
- 
- 
    foreach(QNetworkInterface interfaceItem, interfaceList)
- 
    {
- 
if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp)
- 
                &&interfaceItem.flags().testFlag(QNetworkInterface::IsRunning)
- 
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast)
- 
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast)
- 
                &&!interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack)
- 
                &&interfaceItem.hardwareAddress()!="00:50:56:C0:00:01"
- 
                &&interfaceItem.hardwareAddress()!="00:50:56:C0:00:08")
- 
        {
- 
            QList<QNetworkAddressEntry> addressEntryList=interfaceItem.addressEntries();
- 
            foreach(QNetworkAddressEntry addressEntryItem, addressEntryList)
- 
            {
- 
if(addressEntryItem.ip().protocol()==QAbstractSocket::IPv4Protocol)
- 
                {
- 
                    qDebug()<<"------------------------------------------------------------";
- 
                    qDebug()<<"Adapter Name:"<<interfaceItem.name();
- 
                    qDebug()<<"Adapter Address:"<<interfaceItem.hardwareAddress();
- 
                    qDebug()<<"IP Address:"<<addressEntryItem.ip().toString();
- 
                    qDebug()<<"IP Mask:"<<addressEntryItem.netmask().toString();
- 
                }
- 
            }
- 
        }
- 
    }
- 
}
        通过这种方法同样需要需包含头文件#include <QNetworkInterface>，.pro文件中加入network模块。与第一种方法相比，这种方法能获取更多的网卡信息，除了IP地址，还包括网卡名，MAC地址，和子网掩码，但是这种方法不能区分本地网卡和无线网卡。
3.区分本地网卡、无线网卡和虚拟网卡
        第二种方法能区分虚拟网卡和本机网卡（本地网卡和无线网卡），不能区分本地网卡和无线网卡，是因为通过Qt库无法达到区分本地网卡和无线网卡的目的，需要调用windows库来实现。win7下实现代码如下，需包含头文件#include <WinSock2.h>和#include <iphlpapi.h>，并在.pro文件中引入Iphlpapi.lib。
```cpp
```
- 
voidgetAdapterInfoWithWindows()
- 
{
- 
//PIP_ADAPTER_INFO结构体存储本机网卡信息,包括本地网卡、无线网卡和虚拟网卡
- 
    PIP_ADAPTER_INFO pAdapterInfo=(IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
- 
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
- 
//调用GetAdaptersInfo函数,填充pAdapterInfo指针变量，其中ulOutBufLen参数既是输入也是输出
- 
if(GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
- 
    {
- 
//如果分配失败，释放后重新分配
- 
        GlobalFree (pAdapterInfo);
- 
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
- 
    }
- 
if(GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == NO_ERROR)
- 
    {
- 
while (pAdapterInfo)
- 
        {
- 
//pAdapter->Description中包含"PCI"为本地网卡，pAdapter->Type是71为无线网卡
- 
if(strstr(pAdapterInfo->Description,"PCI") > 0|| pAdapterInfo->Type == 71)
- 
            {
- 
//这里假设每个网卡只有一个IP
- 
char * ipAddress=pAdapterInfo->IpAddressList.IpAddress.String;
- 
char tempIpAddress[3]={'\0'};
- 
memcpy(tempIpAddress,ipAddress,3);
- 
//只显示IP以192开头的网卡信息
- 
if(strstr(tempIpAddress,"192"))
- 
                {
- 
printf("------------------------------------------------------------\n");
- 
printf("Adapter Name: \t%s\n", pAdapterInfo->AdapterName);
- 
printf("Adapter Description: \t%s\n", pAdapterInfo->Description);
- 
printf("Adapter Address: \t");
- 
for (UINT i = 0; i < pAdapterInfo->AddressLength; i++)
- 
                    {
- 
printf("%02X%c", pAdapterInfo->Address[i],
- 
                        i == pAdapterInfo->AddressLength - 1 ? '\n' : '-');
- 
                    }
- 
printf("Adapter Type: \t%d\n", pAdapterInfo->Type);
- 
printf("IP Address: \t%s\n", pAdapterInfo->IpAddressList.IpAddress.String);
- 
printf("IP Mask: \t%s\n", pAdapterInfo->IpAddressList.IpMask.String);
- 
                }
- 
            }
- 
            pAdapterInfo = pAdapterInfo->Next;
- 
        }
- 
    }
- 
else
- 
    {
- 
printf("Call to GetAdaptersInfo failed.\n");
- 
    }
- 
if(pAdapterInfo)
- 
    {
- 
        GlobalFree (pAdapterInfo);
- 
    }
- 
}
第三种方法在我笔记本上的运行结果如下所示，Adapter Type为71的是无线网卡，Adapter Type为6的是本地网卡。
![](https://img-blog.csdn.net/20160406115030935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
总结
        这三个方法都只适合每种网卡只有一个的情况，比如说电脑上如果有两个无线网卡，就无法对着两个无线网卡进行区分。
源码链接：见http://blog.csdn.net/caoshangpa/article/details/51073138的评论
