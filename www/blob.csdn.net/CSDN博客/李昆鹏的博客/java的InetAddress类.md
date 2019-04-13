
# java的InetAddress类 - 李昆鹏的博客 - CSDN博客


2018年04月20日 23:38:11[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：79


------------------------java的InetAddress类-------------------------
**InetAddress是用于管理IP的类，没有构造器**
**1.    单例模式**
**2.    根据静态的方法来返回该对象。**
|boolean
|equals|(|Object|obj)
|将此对象与指定对象比较。
|
|byte[]
|getAddress|()
|返回此 InetAddress 对象的原始 IP 地址。
|
|String
|getHostAddress|()
|返回 IP 地址字符串（以文本表现形式）。
|
|int
|hashCode|()
|返回此 IP 地址的哈希码。
|
|boolean
|isAnyLocalAddress|()
|检查 InetAddress 是否是通配符地址的实用例行程序。
|
|boolean
|isLinkLocalAddress|()
|检查 InetAddress 是否是链接本地地址的实用例行程序。
|
|boolean
|isLoopbackAddress|()
|检查 InetAddress 是否是回送地址的实用例行程序。
|
|boolean
|isMCGlobal|()
|检查多播地址是否具有全局范围的实用例行程序。
|
|boolean
|isMCLinkLocal|()
|检查多播地址是否具有链接范围的实用例行程序。
|
|boolean
|isMCNodeLocal|()
|检查多播地址是否具有节点范围的实用例行程序。
|
|boolean
|isMCOrgLocal|()
|检查多播地址是否具有组织范围的实用例程。
|
|boolean
|isMCSiteLocal|()
|检查多播地址是否具有站点范围的实用例行程序。
|
|boolean
|isMulticastAddress|()
|检查 InetAddress 是否是 IP 多播地址的实用例行程序。
|
|boolean
|isSiteLocalAddress|()
|检查 InetAddress 是否是站点本地地址的实用例行程序。
|

**示例代码：**

**public****class**IPAddressDemo {
**public****static****void**main(String[]args) {
**try**{
//获得本机IP地址
InetAddressia= InetAddress.*getLocalHost*();
System.**out**.println(ia);
//LAPTOP-FDF1MKB1/192.168.1.104
//获得本机的IP字符串
Stringip=ia.getHostAddress();
System.**out**.println(ip);
//192.168.1.104
//获得主机名
System.**out**.println(ia.getHostName());
//LAPTOP-FDF1MKB1
//通过主机名获得IP对象
InetAddressia1=ia.*getByName*("LAPTOP-FDF1MKB1");
System.**out**.println(ia1);
//LAPTOP-FDF1MKB1/192.168.1.104
}**catch**(UnknownHostExceptione) {
e.printStackTrace();
}
}
}


