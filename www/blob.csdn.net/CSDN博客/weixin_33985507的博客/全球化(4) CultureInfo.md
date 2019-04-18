# 全球化(4):CultureInfo - weixin_33985507的博客 - CSDN博客
2019年01月08日 03:03:06[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
CultrueInfo保存区域特定的信息，如关联的语言、子语言、国家/地区、日历和区域性约定。该类还提供对DateTimeFormatInfo、NumberformatInfo、CompareInfo和TextInfo的区域性特定实例的访问。这些对象包含区域性特定操作（如大小写、格式化日期时间和数字以及比较字符串）所需的信息。
      部分区域性名称和标识符：
|区域性名称|区域性标识符|语言-国家/地区|
|----|----|----|
|ar|0x0001|阿拉伯语|
|ar-EG|0x0C01|阿拉伯语 - 埃及|
|zh-CN|0x0804|中文 - 中国|
|zh-CHS|0x0004|中文（简体）|
|zh-TW|0x0404|中文 - 台湾|
|zh-CHT|0x7C04|中文（繁体）|
|en|0x0009|英语|
|en-GB|0x0809|英语 - 英国|
|en-US|0x0409|英语 - 美国|
|fr|0x000C|法语|
|fr-FR|0x040C|法语 - 法国|
|de|0x0007|德语|
|de-DE|0x0407|德语 - 德国|
|es|0x000A|西班牙语|
|es-BO|0x400A|西班牙语 - 玻利维亚|
|es-ES|0x0C0A|西班牙语 - 西班牙|
      可以通过名称和区域性标识符来创建CultureInfo：
|名称|说明|
|----|----|
|[CultureInfo (Int32)](http://msdn.microsoft.com/zh-cn/library/205h6kwc(v=VS.80).aspx)|根据区域性标识符指定的区域性初始化**CultureInfo**类的新实例。由 .NET Compact Framework 支持。|
|[CultureInfo (String)](http://msdn.microsoft.com/zh-cn/library/ky2chs3h(v=VS.80).aspx)|根据由名称指定的区域性初始化**CultureInfo**类的新实例。由 .NET Compact Framework 支持。|
|[CultureInfo (Int32, Boolean)](http://msdn.microsoft.com/zh-cn/library/26xkefd5(v=VS.80).aspx)|基于区域性标识符指定的区域性并基于布尔值（指定是否使用系统中用户选定的区域性设置）来初始化**CultureInfo**类的新实例。由 .NET Compact Framework 支持。|
|[CultureInfo (String, Boolean)](http://msdn.microsoft.com/zh-cn/library/7h898a16(v=VS.80).aspx)|基于名称指定的区域性并基于布尔值（指定是否使用系统中用户选定的区域性设置）来初始化**CultureInfo**类的新实例。由 .NET Compact Framework 支持。|
       例如：
1:  CultureInfo culture1 = new CultureInfo("zh-CN");2:  3:  CultureInfo culture2 = new CultureInfo(0x0804);4:  
