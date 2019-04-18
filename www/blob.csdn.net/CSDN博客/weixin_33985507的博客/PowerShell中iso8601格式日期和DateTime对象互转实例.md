# PowerShell中iso8601格式日期和DateTime对象互转实例 - weixin_33985507的博客 - CSDN博客
2018年07月12日 00:27:36[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：72
参考链接：
[https://technet.microsoft.com/zh-cn/library/aa997624(v=exchg.150).aspx](https://technet.microsoft.com/zh-cn/library/aa997624(v=exchg.150).aspx)
[https://www.pstips.net/convert-iso8601-date-string-to-date-object.html](https://www.pstips.net/convert-iso8601-date-string-to-date-object.html)
[https://www.pstips.net/convert-datetime-to-iso8601-format.html](https://www.pstips.net/convert-datetime-to-iso8601-format.html)
[https://segmentfault.com/a/1190000004292140](https://segmentfault.com/a/1190000004292140)
[https://edu.aliyun.com/a/9853?spm=5176.11310711.0.0.YrSQfd](https://edu.aliyun.com/a/9853?spm=5176.11310711.0.0.YrSQfd)
Powershell将日期时间转换成iso8601格式
#$now=Get-Date
#$now.ToString('yyyy-MM-ddTHH:mm:ss.fffZ')
Powershell将iso8601格式的日期字符串转换成DateTime对象
#$dateTimeStr = '2018-07-10T13:49:38.532Z'
#$format = 'yyyy-MM-ddTHH:mm:ss.fffZ'
#$formatProvider = [Globalization.CultureInfo]::InvariantCulture
#[datetime]::ParseExact($dateTimeStr,$format,$formatProvider)
注：执行的时候请删除#
