# BREW 应用的测试签名 - 我相信...... - CSDN博客





2010年10月20日 10:33:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1138标签：[测试																[brew																[手机																[互联网																[工具																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)




每个应用必须要有一个有效的数字签名(.sig文件)文件才能在目标手机上运行。数字签名文件有两种：

·         测试签名：这是针对注册开发商的请求提供的。一个测试签名使任何应用在单一设备上，而且只能在该设备上运行。在申请测试签名时必须提供目标手机的电子串行码(ESN)、IMEI或者MEID。

·         商用签名：这类签名只有在应用通过TRUE BREW测试时才会产生这类签名。与测试签名不同，商用签名使一个单一的应用可以运行在任何设备上。一个商用签名不但表明应用已经通过TRUE BREW测试程序，而且表明它是完好无损的程序，保证了应用的完整性和一致性。

应用的.sig文件必须同.mod文件存储在目标手机的同一目录下，而且必须同.mod文件拥有同样的文件名，比方说，如果有myapp.mod，必须对应myapp.sig文件。如果目录中没有一个合法的且属于上述两种签名之一的.sig文件，AEE层将不会启动该应用并在运行该应用时提示错误信息。

测试签名是由开发商外联网上一个基于互联网的名为“TestSig生成器”的在线工具生成的。TestSig生成器基于手机的ESN（或者IMEI或者MEID）创建一个测试签名文件。该签名文件从生成之日起90天内有效，到期后需要根据这些信息重新生成一个签名文件。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/07/18/5744095.aspx](http://blog.csdn.net/wireless_com/archive/2010/07/18/5744095.aspx)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=互联网&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)




