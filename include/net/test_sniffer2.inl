/* June 2nd，2002
* Project for graduation qualification By Bby Team 19 */
#include <stdio.h>
#include <conio.h>
//必须加路径，必须把头文件packet32.h包含进去
#include "..\..\Include\packet32.h"
#include "..\..\Include\ntddndis.h"
#define Max_Num_Adapter 10
// Prototypes原形
//发包
void PrintPackets(LPPACKET lpPacket);
//设备列表
char AdapterList[Max_Num_Adapter][1024];
// 主程序开始
int main()
{
  　//define a pointer to an ADAPTER strUCture设备指针
  　LPADAPTER lpAdapter = 0;
  　//define a pointer to a PACKET structure包指针
  　LPPACKET lpPacket;
  　int i;
  　DWord dwErrorCode;
  　DWORD dwVersion;
  　DWORD dwWindowsMajorVersion;
  　//Unicode strings (WinNT)
  　WCHAR AdapterName[8192]; //网络适配器设备列表
  　WCHAR* temp, *temp1;
  　//ASCII strings (Win9x)
  　char AdapterNamea[8192]; //网络适配器设备列表
  　char* tempa, *temp1a;
  　int AdapterNum = 0, Open;
  　ULONG AdapterLength;
  　char buffer[256000]; // 容纳来自驱动器的数据的缓冲区
  　struct bpf_stat stat;
  　// 获得本机网卡名
  　AdapterLength = 4096;
  　printf("Packet.dll test application. Library version:%s\n", PacketGetVersion());
  　printf("Adapters installed:\n");
  　i = 0;
  　　下面这段代码是用来在不同版本下得到网络适配器名：
  　　Win9x 和WinNT中的网卡名称是分别用ASCII和UNICODE实现的，所以首先要得到本地操作系统的版本号：
  dwVersion = GetVersion();
  dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
  　　这里首先用到的Packet.dll函数是PacketGetAdapterNames（PTSTR pStr，PULONG BufferSize, 通常它是与驱动程序通信并被调用的第一个函数，它将返回的用户本地系统中安装的网络适配器的名字放在缓冲区pStr中；BufferSize是缓冲区的长度：
  if (!(dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4)) {
    　//是Windows NT
    　// 找不到设备列表
    　if (PacketGetAdapterNames(AdapterName, &AdapterLength) == FALSE) {
      　　printf("Unable to retrieve the list of the adapters!\n");
      　　return -1;
      　
    }
    　// 找到设备列表
    　temp = AdapterName;
    　temp1 = AdapterName;
    　while ((*temp != '\0')(*(temp - 1) != '\0'))
      　 {
      　　if (*temp == '\0')
        　　{
        　　　memcpy(AdapterList[i], temp1, (temp - temp1) * 2);
        　　　temp1 = temp + 1;
        　　　i++;
        　　
      }
      　　temp++;
      　
    }
    　// 显示适配器列表
    　AdapterNum = i;
    　for (i = 0; i < AdapterNum; i++) {
      　　wprintf(L"\n%d- %s\n", i + 1, AdapterList[i]);
    }
    　　printf("\n");
    　
  }
  　else //否则就是windows 9x，获取适配器名的方法同WinNT下
    　 {
    　　if (PacketGetAdapterNames(AdapterNamea, &AdapterLength) == FALSE) {
    　　　printf("Unable to retrieve the list of the adapters!\n");
      　　　return -1;
      　
    }
  　tempa = AdapterNamea;
  　temp1a = AdapterNamea;
  　while ((*tempa != '\0')(*(tempa - 1) != '\0'))
  　 {
  　　if (*tempa == '\0')
      　　{
      　　　memcpy(AdapterList[i], temp1a, tempa - temp1a);
      　　　temp1a = tempa + 1;
      　　　i++;
      　　
    }
    　　tempa++;
    　
  }
  　AdapterNum = i;
  　for (i = 0; i < AdapterNum; i++) {
  　　printf("\n%d- %s\n", i + 1, AdapterList[i]);
  }
  　　printf("\n");
  }
  　　下面这段代码就是让用户选择监听的网络适配器号：
  // 选择设备
  do {
    　printf("Select the number of the adapter to open : ");
    　scanf("%d", &Open);
    　if (Open > AdapterNum) {
      　　printf("\nThe number must be smaller than %d", AdapterNum);
    }
  }
  while (Open > AdapterNum);
  　　然后，将所选择的设备打开，这里可以设置为“混杂”模式打开，也可以是“直接”模式打开。代码如下：
  // 打开设备
  lpAdapter = PacketOpenAdapter(AdapterList[Open - 1]);
  // 当设备无法打开时，出示错误信息：
  if (!lpAdapter(lpAdapter->hFile == INVALID_HANDLE_VALUE)) {
    　dwErrorCode = GetLastError();
    　printf("Unable to open the adapter, Error Code : %lx\n", dwErrorCode);
    　return -1;
  }
  　　将网卡设置为“混杂”模式，代码如下：
  　　这里用到函数PacketSetHwFilter（LPADAPTER AdapterObject，ULONG Filter），它在到来的包上设置了一个硬件过滤器，如操作成功，返回TRUE。AdapterObject是过滤器所在的网卡设备指针；过滤器的常量Filter定义在头文件ntddndis.h 中，包括有：
  　　·NDIS - PACKET - TYPE - PROMISCUOUS：设置混杂模式，每个到来的包都会被网卡接受；
  　　·NDIS - PACKET - TYPE - DIRECTED：只有直接到主机网卡的包才会被接受；
  　　·NDIS - PACKET - TYPE - BROADCAST：只接受广播包；
  　　·NDIS - PACKET - TYPE - MULTICAST：只接受到主机所在的组的多播包；
  　　·NDIS - PACKET - TYPE - ALL - MULTICAS：接受每个多播的包。
  // set the network adapter in promiscuous mode
  // 假如混杂模式设置失败，提示错误：
  if (PacketSetHwFilter(lpAdapter, NDIS_PACKET_TYPE_PROMISCUOUS) == FALSE) {
    printf("Warning: unable to set promiscuous mode!\n");
  }
  　　然后在driver中置512K的缓冲：
  　　这里用到函数PacketSetBuff（LPADAPTER AdapterObject，int dim），它被用于设置AdapterObject指向的网卡的驱动程序的缓冲区，成功则返回TRUE。Dim是新的缓冲区的大小，当它被设定时，旧缓冲区中的数据将被丢弃，其中存储的包也会失去。
  　　需要注重的地方：驱动器缓冲区的大小设置是否恰当，将影响截包进程的性能，设置应能保证运行快且不会丢包。这里设置的是512000Byte。
  // set a 512K buffer in the driver
  // 当无法设置缓冲区时，提示错误：
  if (PacketSetBuff(lpAdapter, 512000) == FALSE) {
    　printf("Unable to set the kernel buffer!\n");
    　return -1;
  }
  　　PacketSetReadTimeout（LPADAPTER AdapterObject，int timeout）函数的功能是，设置与AdapterObject指定网卡绑定的读操作超时的值，timeout以毫秒为单位，0表示没有超时，当没有包到时，read就不返回。
  // set a 1 second read timeout
  // 设置1秒的读取操作超时
  if (PacketSetReadTimeout(lpAdapter, 1000) == FALSE) {
    　printf("Warning: unable to set the read tiemout!\n");
  }
  　　接下来，定位设备，代码如下：
  　　这里用到函数PacketAllocatePacket（Void）将在内存中分配一个PACKET结构并返回一个指向它的指针，但这个结构的Buffer字段还没有设定，所以应再调用PacketInitPacket函数来对其进行初始化。
  //allocate and initialize a packet structure that will be used to
  //receive the packets.
  // 当定位失败时，提示错误：
  if ((lpPacket = PacketAllocatePacket()) == NULL) {
    　printf("\nError: failed to allocate the LPPACKET structure.");
    　return (-1);
  }
  　　然后，就可以初始化设备，开始接受网络包了：
  　　用函数PacketInitPacket（LPPACKET lpPacket，PVOID Buffer，UINT Length）来初始化PACKET结构。lpPacket是要被初始化的指针；Buffer为指向用户分配的包含包的数据的缓冲区的指针；Length为缓冲区长度。
  　　需要注重的地方：PACKET结构关联的缓冲区存储由packet capture driver 截获的包，包的数量被缓冲区大小所限制，最大缓冲区的大小就是应用程序从驱动器中一次能读到的数据的多少。所以设置大的缓冲区可减少系统调用的次数，提高截获效率。这里设置的是256K。
  PacketInitPacket(lpPacket, (char*)buffer, 256000);
  　　接下来，是截包主循环：
  //main capture loop
  　　这里又用到函数PacketReceivePacket（LPADAPTER AdapterObject，LPPACKET lpPacket，BOOLEAN Sync），它将接受（截获）一个包的集合。参数包括一个指向用来指定截包的网卡的ADAPTER结构指针、一个指向用来容纳包的PACKET结构、一个指出是同步还是异步方式操作的标记。当操作同步时，函数锁定程序；当操作异步时，函数不锁定程序，必须调用PacketWaitPacket过程来检查是否正确完成。一般采用同步模式。
  // 直到有键盘键入：
  while (!kbhit()) {
    　// capture the packets 捕捉包
    　// 捕捉包失败时，提示错误：
    　if (PacketReceivePacket(lpAdapter, lpPacket, TRUE) == FALSE) {
      　　printf("Error: PacketReceivePacket failed");
      　　return (-1);
      　
    }
    　// 打印包中的数据，调用自定义函数PrintPackets（）
    　PrintPackets(lpPacket);
  }
  　　最后将得到的统计数据打印出来，代码如下：
  　　这里用到函数PacketGetStats（LPADAPTER AdapterObject，struct bpf_star* s）可以得到两个驱动程序的内部变量的值：从调用PacketOpenAdapter开始，已经被指定网卡接收的包数目；以及已经被网卡接收但被内核丢弃的包数目。这两个值被驱动程序拷贝到应用提供的bpf_stat结构中。
      //print the capture statistics
      // 得到统计值
      // 当无法从内核读取状态时，提示错误：
      if (PacketGetStats(lpAdapter, &stat) == FALSE) {
      　printf("Warning: unable to get stats from the kernel!\n");
    }
  // 打印“XX包被截取；XX包被丢弃”：
    else {
      printf("\n\n%d packets received.\n%d Packets lost", stat.bs_recv, stat.bs_drop);
    }
  　　这里用函数PacketFreePacket(LPPACKET lpPacket)来释放由lpPacket指向的结构：
  // 释放空间
  PacketFreePacket(lpPacket);
  用函数PacketCloseAdapter(LPADAPTER lpAdapter)来释放ADAPTER结构lpAdapter，并关闭网卡指针：
  // close the adapter and exit
  // 关闭设备退出
  PacketCloseAdapter(lpAdapter);
  return (0);
} // 主程序结束

