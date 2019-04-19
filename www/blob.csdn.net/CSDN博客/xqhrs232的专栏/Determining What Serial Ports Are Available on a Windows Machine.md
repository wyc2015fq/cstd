# Determining What Serial Ports Are Available on a Windows Machine - xqhrs232的专栏 - CSDN博客
2017年07月06日 11:43:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：219
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://www.codeguru.com/cpp/w-p/system/hardwareinformation/article.php/c5721/Determining-What-Serial-Ports-Are-Available-on-a-Windows-Machine.htm](http://www.codeguru.com/cpp/w-p/system/hardwareinformation/article.php/c5721/Determining-What-Serial-Ports-Are-Available-on-a-Windows-Machine.htm)
# Determining What Serial Ports Are Available on a Windows Machine
Posted by [Zach
 Gorman](http://www.codeguru.com/member.php/Zach+Gorman/) on July 9th, 2002- 
0.25
0.50
0.75
1.00
1.25
1.50
1.75
2.00
2.25
2.50
2.75
3.00
3.25
3.50
3.75
4.00
4.25
4.50
4.75
5.00
- 
Vote!
- [Tweet](https://twitter.com/share)
- 
- in**Share**
- 
- 
Environment:
You might think that determining which serial ports are present on a Windows PC would be an easy task. It seems like a reasonable enough thing for the OS to support. Unfortunately, there was no support for it at all (short of reading the Registry yourself)
 before Win2k, and even then, the API is a bit cumbersome.
![](http://www.codeguru.com/images/pdfImage.jpg)
Mobile Application Management Platform Evaluation Checklist
[Download
 Now](http://o1.qnsr.com/cgi/r?WT.qs_dlk=WV2wFgrIZ78AADsjgG4AAAAN;;n=203;c=1395297;s=9479;x=7936;f=201308291424140;u=j;z=TIMESTAMP;k=http://assetform.codeguru.com/controller?asset=231456410&srvid=95910&vkey=10450410&io=228964&BLUID=2017070520355785465588:010450410:001&qset=CONTACTFORM_HQB&formHQB=y&domain=www.codeguru.com&cfurl=http%3A%2F%2Fwww.codeguru.com%2Fcpp%2Fw-p%2Fsystem%2Fhardwareinformation%2Farticle.php%2Fc5721%2FDetermining-What-Serial-Ports-Are-Available-on-a-Windows-Machine.htm)
The attached serial port enumeration code first determines which operating system it is running under, and then runs the appropriate routine to enumerate the serial ports. In Windows 9x (and Me), it uses the Registry. In W2k and later, it uses the setupapi
 that was included in that version of the WinSDK. It also has support for "brute force" enumeration of serial ports under NT4.
Unfortunately, I statically linked with setupapi.lib, so the provided executable won't actually run under Windows 95 and NT4. (I didn't really need to support those OSs for my application.) This could be finagled by replacing the SetupDi* function calls with
 dynamic binding via LoadLibrary() if needed.
To use the EnumSerial code, simply include EnumSerial.cpp and .h in your project, and link with setupapi.lib in the WIN32 SDK (this is under "additional dependencies" in the project link settings in Visual Studio).
All you have to do now is #include "EnumSerial.h" in your source code, allocate an empty CArray of SSerInfo structs, and make a call to EnumSerialPorts. It will populate your array with filled-out SSerInfo structs which contain the following information:
- CString strDevPath;// Device path for use with CreateFile()
- CString strPortName;// Simple name (for example, COM1)
- CString strFriendlyName;// Full name to be displayed to a user
-   BOOL bUsbDevice;// Provided through a USB connection?
- CString strPortDesc;// Friendly name without the COMx
example:
- CArray asi;
- EnumSerialPorts(asi,FALSE/*include all*/);
### Downloads
[Download demo project - 145 Kb](http://www.codeguru.com/code/legacy/system/SerialEnum.zip)
