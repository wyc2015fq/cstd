# Convert IPv6 Address to IP numbers (C#) - weixin_33985507的博客 - CSDN博客
2016年10月27日 11:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
URL: [http://lite.ip2location.com/](http://lite.ip2location.com/)
Use the code below to convert the IP address of your web visitors and lookup for their geographical location, e.g. country, state, city, latitude/longitude, ZIPs, timezone and so on. Free database can be downloaded at http://lite.ip2location.com.
Expand | [Embed](http://snipplr.com/view/84723/#) | [Plain Text](http://snipplr.com/view.php?codeview&id=84723)
- 
/// <summary>
- 
/// Convert IPV6 Address to IP Number
- 
/// Free geolocation database can be downloaded at:
- 
/// http://lite.ip2location.com/
- 
/// </summary>
- 
- 
string strIP = "2404:6800:4001:805::1006";
- 
System.Net.IPAddress address;
- 
System.Numerics.BigInteger ipnum;
- 
- 
if (System.Net.IPAddress.TryParse(strIP, out address)) {
- 
byte[] addrBytes = address.GetAddressBytes();
- 
- 
if (System.BitConverter.IsLittleEndian) {
- 
System.Collections.Generic.List<byte> byteList = [new](http://www.google.com/search?q=new+msdn.microsoft.com)System.Collections.Generic.List<byte>(addrBytes);
- 
byteList.Reverse();
- 
addrBytes = byteList.ToArray();
- 
}
- 
- 
if (addrBytes.Length > 8) {
- 
//IPv6
- 
ipnum = System.BitConverter.ToUInt64(addrBytes, 8);
- 
ipnum <<= 64;
- 
ipnum += System.BitConverter.ToUInt64(addrBytes, 0);
- 
} else {
- 
//IPv4
- 
ipnum = System.BitConverter.ToUInt32(addrBytes, 0);
- 
}
- 
}
