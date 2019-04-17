# Sigar介绍与使用 - DoubleLi - 博客园






Sigar是Hyperic-hq产品的基础包,是Hyperic HQ主要的数据收集组件。它用来从许多平台收集系统和处理信息.

这些平台包括：Linux, Windows, Solaris, AIX, HP-UX, FreeBSD and Mac OSX.

Sigar有C，C#，Java和Perl API，java版的API为sigar.jar sigar.jar的底层是用C语言编写的，它通过本地方法来调用操作系统API来获取系统相关数据。Windows操作系统下Sigar.jar 依赖sigar-amd64-winnt.dll或sigar-x86-winnt.dll，linux 操作系统下则依赖libsigar-amd64-linux.so或libsigar-x86-linux.so



Hyperic-hq官方网站：[http://www.hyperic.com](http://sigar%E6%98%AFhyperic-hq%E4%BA%A7%E5%93%81%E7%9A%84%E5%9F%BA%E7%A1%80%E5%8C%85%2C%E6%98%AFhyperic%20hq%E4%B8%BB%E8%A6%81%E7%9A%84%E6%95%B0%E6%8D%AE%E6%94%B6%E9%9B%86%E7%BB%84%E4%BB%B6%E3%80%82%E5%AE%83%E7%94%A8%E6%9D%A5%E4%BB%8E%E8%AE%B8%E5%A4%9A%E5%B9%B3%E5%8F%B0%E6%94%B6%E9%9B%86%E7%B3%BB%E7%BB%9F%E5%92%8C%E5%A4%84%E7%90%86%E4%BF%A1%E6%81%AF%EF%BC%8C%E8%BF%99%E4%BA%9B%E5%B9%B3%E5%8F%B0%E5%8C%85%E6%8B%AC%EF%BC%9Alinux%2C%20windows%2C%20solaris%2C%20aix%2C%20hp-ux%2C%20freebsd%20and%20mac%20osx%E3%80%82sigar%E6%9C%89c%EF%BC%8Cc/#%EF%BC%8CJava%E5%92%8CPerl%20API%EF%BC%8Cjava%E7%89%88%E7%9A%84API%E4%B8%BAsigar.jar%20sigar.jar%E7%9A%84%E5%BA%95%E5%B1%82%E6%98%AF%E7%94%A8C%E8%AF%AD%E8%A8%80%E7%BC%96%E5%86%99%E7%9A%84%EF%BC%8C%E5%AE%83%E9%80%9A%E8%BF%87%E6%9C%AC%E5%9C%B0%E6%96%B9%E6%B3%95%E6%9D%A5%E8%B0%83%E7%94%A8%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9FAPI%E6%9D%A5%E8%8E%B7%E5%8F%96%E7%B3%BB%E7%BB%9F%E7%9B%B8%E5%85%B3%E6%95%B0%E6%8D%AE%E3%80%82Windows%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E4%B8%8BSigar.jar%20%E4%BE%9D%E8%B5%96sigar-amd64-winnt.dll%E6%88%96sigar-x86-winnt.dll%EF%BC%8Clinux%20%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E4%B8%8B%E5%88%99%E4%BE%9D%E8%B5%96libsigar-amd64-linux.so%E6%88%96libsigar-x86-linux.so%20%20Hyperic-hq%E5%AE%98%E6%96%B9%E7%BD%91%E7%AB%99%EF%BC%9Ahttp://www.hyperic.com%20%20Sigar.jar%E4%B8%8B%E8%BD%BD%E5%9C%B0%E5%9D%80%EF%BC%9Ahttp://sigar.hyperic.com)



Sigar.jar下载地址：[http://sigar.hyperic.com](http://sigar.hyperic.com/)







Sigar API 提供一个方便的接口来收集系统信息，如：

◆系统内存，页面交换，cpu，平均负载，运行时间，登录信息

◆每个进程占用的内存，cpu，帐号信息，状态，参数，环境，打开的文件

◆文件系统探测和度量

◆网络接口探测，配置信息和度量

◆网络路由和连接表









获取cpu信息代码  ![收藏代码](http://364434006.iteye.com/images/icon_star.png)

- // CPU数量（单位：个）  
- int cpuLength = sigar.getCpuInfoList().length;  
- print(cpuLength);  
- 
- // CPU的总量（单位：HZ）及CPU的相关信息  
- CpuInfo infos[] = sigar.getCpuInfoList();  
- for (int i = 0; i < infos.length; i++) {// 不管是单块CPU还是多CPU都适用  
-     CpuInfo info = infos[i];  
-     print("mhz=" + info.getMhz());// CPU的总量MHz  
-     print("vendor=" + info.getVendor());// 获得CPU的卖主，如：Intel  
-     print("model=" + info.getModel());// 获得CPU的类别，如：Celeron  
-     print("cache size=" + info.getCacheSize());// 缓冲存储器数量  
- }  
- 
- /** CPU的用户使用量、系统使用剩余量、总的剩余量、总的使用占用量等（单位：100%） **/  
- // 方式一，主要是针对一块CPU的情况  
- CpuPerc cpu;  
- try {  
-     cpu = sigar.getCpuPerc();  
-     printCpuPerc(cpu);  
- } catch (SigarException e) {  
-     e.printStackTrace();  
- }  
- 
- // 方式二，不管是单块CPU还是多CPU都适用  
- CpuPerc cpuList[] = null;  
- try {  
-     cpuList = sigar.getCpuPercList();  
- } catch (SigarException e) {  
-     e.printStackTrace();  
- }  
- for (int i = 0; i < cpuList.length; i++) {  
-     // printCpuPerc(cpuList[i]);  
- }  








获取内存信息代码  ![收藏代码](http://364434006.iteye.com/images/icon_star.png)

- // 物理内存信息  
- Mem mem = sigar.getMem();  
- // 内存总量  
- print("Total = " + mem.getTotal() / 1024L / 1024 + "M av");  
- // 当前内存使用量  
- print("Used = " + mem.getUsed() / 1024L / 1024 + "M used");  
- // 当前内存剩余量  
- print("Free = " + mem.getFree() / 1024L / 1024 + "M free");  
- 
- // 系统页面文件交换区信息  
- Swap swap = sigar.getSwap();  
- // 交换区总量  
- print("Total = " + swap.getTotal() / 1024L + "K av");  
- // 当前交换区使用量  
- print("Used = " + swap.getUsed() / 1024L + "K used");  
- // 当前交换区剩余量  
- print("Free = " + swap.getFree() / 1024L + "K free");  










获取操作系统信息代码  ![收藏代码](http://364434006.iteye.com/images/icon_star.png)

- <span style="white-space: normal; #ffffff;"> </span><span style="white-space: normal; #ffffff;">// 取到当前操作系统的名称</span>  
- String hostname = "";  
- try {  
-     hostname = InetAddress.getLocalHost().getHostName();  
- } catch (Exception exc) {  
-     try {  
-         hostname = sigar.getNetInfo().getHostName();  
-     } catch (SigarException e) {  
-         hostname = "localhost.unknown";  
-     } finally {  
-         sigar.close();  
-     }  
- }  
- print(hostname);  
- 
- // 取当前操作系统的信息  
- OperatingSystem OS = OperatingSystem.getInstance();  
- // 操作系统内核类型如： 386、486、586等x86  
- print("OS.getArch() = " + OS.getArch());  
- print("OS.getCpuEndian() = " + OS.getCpuEndian());//  
- print("OS.getDataModel() = " + OS.getDataModel());//  
- // 系统描述  
- print("OS.getDescription() = " + OS.getDescription());  
- print("OS.getMachine() = " + OS.getMachine());//  
- // 操作系统类型  
- print("OS.getName() = " + OS.getName());  
- print("OS.getPatchLevel() = " + OS.getPatchLevel());//  
- // 操作系统的卖主  
- print("OS.getVendor() = " + OS.getVendor());  
- // 卖主名称  
- System.out  
-         .println("OS.getVendorCodeName() = " + OS.getVendorCodeName());  
- // 操作系统名称  
- print("OS.getVendorName() = " + OS.getVendorName());  
- // 操作系统卖主类型  
- print("OS.getVendorVersion() = " + OS.getVendorVersion());  
- // 操作系统的版本号  
- print("OS.getVersion() = " + OS.getVersion());  
- 
- // 取当前系统进程表中的用户信息  
- Who who[] = sigar.getWhoList();  
- if (who != null && who.length > 0) {  
-     for (int i = 0; i < who.length; i++) {  
-         print("\n~~~~~~~~~" + String.valueOf(i) + "~~~~~~~~~");  
-         Who _who = who[i];  
-         print("getDevice() = " + _who.getDevice());  
-         print("getHost() = " + _who.getHost());  
-         print("getTime() = " + _who.getTime());  
-         // 当前系统进程表中的用户名  
-         print("getUser() = " + _who.getUser());  
-     }  
- }  






获取磁盘信息代码  ![收藏代码](http://364434006.iteye.com/images/icon_star.png)

- <span style="white-space: normal; #ffffff;">// 取硬盘已有的分区及其详细信息（通过sigar.getFileSystemList()来获得FileSystem列表对象，然后对其进行编历</span>  
- FileSystem fslist[] = sigar.getFileSystemList();  
- String dir = System.getProperty("user.home");// 当前用户文件夹路径  
- print(dir + "   " + fslist.length);  
- for (int i = 0; i < fslist.length; i++) {  
-     print("\n~~~~~~~~~~" + i + "~~~~~~~~~~");  
- FileSystem fs = fslist[i];  
- // 分区的盘符名称  
- print("fs.getDevName() = " + fs.getDevName());  
- // 分区的盘符名称  
- print("fs.getDirName() = " + fs.getDirName());  
- print("fs.getFlags() = " + fs.getFlags());//  
- // 文件系统类型，比如 FAT32、NTFS  
- print("fs.getSysTypeName() = " + fs.getSysTypeName());  
- // 文件系统类型名，比如本地硬盘、光驱、网络文件系统等  
- print("fs.getTypeName() = " + fs.getTypeName());  
- // 文件系统类型  
- print("fs.getType() = " + fs.getType());  
- FileSystemUsage usage = null;  
- try {  
-     usage = sigar.getFileSystemUsage(fs.getDirName());  
- } catch (SigarException e) {  
-     if (fs.getType() == 2)  
-         throw e;  
-     continue;  
- }  
- switch (fs.getType()) {  
- case 0: // TYPE_UNKNOWN ：未知  
-     break;  
- case 1: // TYPE_NONE  
-     break;  
- case 2: // TYPE_LOCAL_DISK : 本地硬盘  
-     // 文件系统总大小  
-     print(" Total = " + usage.getTotal() + "KB");  
-     // 文件系统剩余大小  
-     print(" Free = " + usage.getFree() + "KB");  
-     // 文件系统可用大小  
-     print(" Avail = " + usage.getAvail() + "KB");  
-     // 文件系统已经使用量  
-     print(" Used = " + usage.getUsed() + "KB");  
-     double usePercent = usage.getUsePercent() * 100D;  
-     // 文件系统资源的利用率  
-     print(" Usage = " + usePercent + "%");  
-     break;  
- case 3:// TYPE_NETWORK ：网络  
-     break;  
- case 4:// TYPE_RAM_DISK ：闪存  
-     break;  
- case 5:// TYPE_CDROM ：光驱  
-     break;  
- case 6:// TYPE_SWAP ：页面交换  
-     break;  
- }  
- print(" DiskReads = " + usage.getDiskReads());  
- print(" DiskWrites = " + usage.getDiskWrites());  
- }  






获取网络信息代码  ![收藏代码](http://364434006.iteye.com/images/icon_star.png)

- <span style="white-space: normal; #ffffff;"> // 当前机器的正式域名</span>  
- try {  
-     print(InetAddress.getLocalHost().getCanonicalHostName());  
- } catch (UnknownHostException e) {  
-     try {  
-         print(sigar.getFQDN());  
-     } catch (SigarException ex) {  
-     } finally {  
-         sigar.close();  
-     }  
- }  
- 
- // 取到当前机器的IP地址  
- String address = null;  
- try {  
-     address = InetAddress.getLocalHost().getHostAddress();  
-     // 没有出现异常而正常当取到的IP时，如果取到的不是网卡循回地址时就返回  
-     // 否则再通过Sigar工具包中的方法来获取  
-     print(address);  
-     if (!NetFlags.LOOPBACK_ADDRESS.equals(address)) {  
-     }  
- } catch (UnknownHostException e) {  
-     // hostname not in DNS or /etc/hosts  
- }  
- try {  
-     address = sigar.getNetInterfaceConfig().getAddress();  
- } catch (SigarException e) {  
-     address = NetFlags.LOOPBACK_ADDRESS;  
- } finally {  
- }  
- print(address);  
- 
- // 取到当前机器的MAC地址  
- String[] ifaces = sigar.getNetInterfaceList();  
- String hwaddr = null;  
- for (int i = 0; i < ifaces.length; i++) {  
-     NetInterfaceConfig cfg = sigar.getNetInterfaceConfig(ifaces[i]);  
-     if (NetFlags.LOOPBACK_ADDRESS.equals(cfg.getAddress())  
-             || (cfg.getFlags() & NetFlags.IFF_LOOPBACK) != 0  
-             || NetFlags.NULL_HWADDR.equals(cfg.getHwaddr())) {  
-         continue;  
-     }  
-     hwaddr = cfg.getHwaddr();  
-     print(hwaddr);  
-     // break;  
- }  
- print(hwaddr != null ? hwaddr : null);  
- 
- // 获取网络流量等信息  
- String ifNames[] = sigar.getNetInterfaceList();  
- for (int i = 0; i < ifNames.length; i++) {  
-     String name = ifNames[i];  
-     NetInterfaceConfig ifconfig = sigar.getNetInterfaceConfig(name);  
-     print("\nname = " + name);// 网络设备名  
-     print("Address = " + ifconfig.getAddress());// IP地址  
-     print("Netmask = " + ifconfig.getNetmask());// 子网掩码  
-     if ((ifconfig.getFlags() & 1L) <= 0L) {  
-         print("!IFF_UP...skipping getNetInterfaceStat");  
-         continue;  
-     }  
-     try {  
-         NetInterfaceStat ifstat = sigar.getNetInterfaceStat(name);  
-         print("RxPackets = " + ifstat.getRxPackets());// 接收的总包裹数  
-         print("TxPackets = " + ifstat.getTxPackets());// 发送的总包裹数  
-         print("RxBytes = " + ifstat.getRxBytes());// 接收到的总字节数  
-         print("TxBytes = " + ifstat.getTxBytes());// 发送的总字节数  
-         print("RxErrors = " + ifstat.getRxErrors());// 接收到的错误包数  
-         print("TxErrors = " + ifstat.getTxErrors());// 发送数据包时的错误数  
-         print("RxDropped = " + ifstat.getRxDropped());// 接收时丢弃的包数  
-         print("TxDropped = " + ifstat.getTxDropped());// 发送时丢弃的包数  
-     } catch (SigarNotImplementedException e) {  
-     } catch (SigarException e) {  
-         print(e.getMessage());  
-     }  
- }  
- 
- // 一些其他的信息  
- for (int i = 0; i < ifaces.length; i++) {  
-     NetInterfaceConfig cfg = sigar.getNetInterfaceConfig(ifaces[i]);  
-     if (NetFlags.LOOPBACK_ADDRESS.equals(cfg.getAddress())  
-             || (cfg.getFlags() & NetFlags.IFF_LOOPBACK) != 0  
-             || NetFlags.NULL_HWADDR.equals(cfg.getHwaddr())) {  
-         continue;  
-     }  
-     print("cfg.getAddress() = " + cfg.getAddress());// IP地址  
-     print("cfg.getBroadcast() = " + cfg.getBroadcast());// 网关广播地址  
-     print("cfg.getHwaddr() = " + cfg.getHwaddr());// 网卡MAC地址  
-     print("cfg.getNetmask() = " + cfg.getNetmask());// 子网掩码  
-     System.out  
-             .println("cfg.getDescription() = " + cfg.getDescription());// 网卡描述信息  
-     print("cfg.getType() = " + cfg.getType());//  
-     System.out  
-             .println("cfg.getDestination() = " + cfg.getDestination());  
-     print("cfg.getFlags() = " + cfg.getFlags());//  
-     print("cfg.getMetric() = " + cfg.getMetric());  
-     print("cfg.getMtu() = " + cfg.getMtu());  
-     print("cfg.getName() = " + cfg.getName());  
- }  










