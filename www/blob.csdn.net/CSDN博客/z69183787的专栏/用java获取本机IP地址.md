# 用java获取本机IP地址 - z69183787的专栏 - CSDN博客
2015年02月02日 16:06:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2786
在网上找了几个用java获取本机IP地址的代码，发现都少都有些不完美，自己整理了一下.突然之间很想把自己的IP地址给获取了，虽然用系统自带命令可以得到，但自己想写一个程序获取一下，到网上搜索了一下java获取本机IP地址的方法，结果居然发现没有一个是可以用的，气的我老人家吐血，
这些人闭着眼睛写程序，写完了就往网上发，也不测试一下，害的我以为自己RP问题，老是获取不到正确的IP地址，强烈谴责！！！
为了表示鄙视，现把网上找到的主要的两种方法的不足给指出一下
方法一（只能在Windows上使用，Linux平台就gei屁了）：
try
{ 
System.out.println("本机的IP = " + InetAddress.getLocalHost());
} catch (UnknownHostException e)
{ 
e.printStackTrace();
}
在Linux下的执行结果是：本机的IP = xxx/127.0.1.1 (其中xxx是你的计算机名，偶这里马赛克了)
方法二(宣称可以在Linux下执行)
Enumeration netInterfaces=NetworkInterface.getNetworkInterfaces();
InetAddress ip = null;
while(netInterfaces.hasMoreElements())
{
NetworkInterface ni=(NetworkInterface)netInterfaces.nextElement();
System.out.println(ni.getName());
ip=(InetAddress) ni.getInetAddresses().nextElement();
if( !ip.isSiteLocalAddress() 
&& !ip.isLoopbackAddress() 
&& ip.getHostAddress().indexOf(":")==-1)
{
System.out.println ("本机的ip=" + ip.getHostAddress());
break;
}
else
{
ip=null;
}
}
从红色部分的代码可以看到，该代码对于获取到的第一个NetworkInterface的IP地址的获取，没有循环的获取，只是对第一个IP地址进行了处理，这样就导致了如果第一个IP地址不是一个
Inet4Address的地址而是一个< span
id="ArticleContent1_ArticleContent1_lblContent">Inet6Address，这个判断 ip.getHostAddress().indexOf(":")==-1将永远是false,这个if条件进不去呀，多害人，强烈鄙视！
不过方法二思路是对了，就是有些小毛病，让偶修改了一下，最终版的可以在 Linux下正确执行的代码如下：
Enumeration allNetInterfaces = NetworkInterface.getNetworkInterfaces();
InetAddress ip = null;
while (allNetInterfaces.hasMoreElements())
{
NetworkInterface netInterface = (NetworkInterface) allNetInterfaces.nextElement();
System.out.println(netInterface.getName());
Enumeration addresses = netInterface.getInetAddresses();
while (addresses.hasMoreElements())
{
ip = (InetAddress) addresses.nextElement();
if (ip != null && ip instanceof Inet4Address)
{
System.out.println("本机的IP = " + ip.getHostAddress());
} 
}
}
