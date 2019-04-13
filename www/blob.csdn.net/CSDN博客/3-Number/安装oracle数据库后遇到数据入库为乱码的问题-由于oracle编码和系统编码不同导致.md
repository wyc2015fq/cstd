
# 安装oracle数据库后遇到数据入库为乱码的问题-由于oracle编码和系统编码不同导致 - 3-Number - CSDN博客


置顶2015年05月14日 14:35:58[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：821标签：[oracle																](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)[乱码																](https://so.csdn.net/so/search/s.do?q=乱码&t=blog)[NLS_LANG																](https://so.csdn.net/so/search/s.do?q=NLS_LANG&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=乱码&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)个人分类：[开源项目学习																](https://blog.csdn.net/pangyemeng/article/category/2560571)
[
																								](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)


在项目中安装oracle数据库后遇到数据入库为乱码的问题。
0x01 背景知识：下表是locale ID与字符集名称的对应关系：
|Language
|Locale ID
|NLS_LANG
|
|
|English (American)
|en_US.UTF-8
|AMERICAN_AMERICA.AL32UTF8
|
|English (American)
|en_US.ISO-8859-1
|AMERICAN_AMERICA.WE8ISO8859P1
|
|English (American)
|en_US.ISO-8859-15
|AMERICAN_AMERICA.WE8ISO8859P15
|
|English (Australian)
|en_AU.UTF-8
|ENGLISH_AUSTRALIA.AL32UTF8
|
|English (Australian)
|en_AU.ISO-8859-1
|ENGLISH_AUSTRALIA.WE8ISO8859P1
|
|English (Australian)
|en_AU.ISO-8859-15
|ENGLISH_AUSTRALIA.WE8ISO8859P15
|
|English (British)
|en_GB.UTF-8
|ENGLISH_UNITED KINGDOM.AL32UTF8
|
|English (British)
|en_GB.ISO-8859-1
|ENGLISH_UNITED KINGDOM.WE8ISO8859P1
|
|English (British)
|en_GB.ISO-8859-15
|ENGLISH_UNITED KINGDOM.WE8ISO8859P15
|
|English (Ireland)
|en_IE.UTF-8
|ENGLISH_IRELAND.AL32UTF8
|
|English (Ireland)
|en_IE.ISO-8859-1
|ENGLISH_IRELAND.WE8ISO8859P1
|
|English (Ireland)
|en_IE.ISO-8859-15
|ENGLISH_IRELAND.WE8ISO8859P15
|
|German
|de_DE.UTF-8
|GERMAN_GERMANY.AL32UTF8
|
|German
|de_DE.ISO-8859-1
|GERMAN_GERMANY.WE8ISO8859P1
|
|German
|de_DE.ISO-8859-15
|GERMAN_GERMANY.WE8ISO8859P15
|
|French
|fr_FR.UTF-8
|FRENCH_FRANCE.AL32UTF8
|
|French
|fr_FR.ISO-8859-1
|FRENCH_FRANCE.WE8ISO8859P1
|
|French
|fr_FR.ISO-8859-15
|FRENCH_FRANCE.WE8ISO8859P15
|
|Italian
|it_IT.UTF-8
|ITALIAN_ITALY.AL32UTF8
|
|Italian
|it_IT.ISO-8859-1
|ITALIAN_ITALY.WE8ISO8859P1
|
|Italian
|it_IT.ISO-8859-15
|ITALIAN_ITALY.WE8ISO8859P15
|
|Spanish
|es_ES.UTF-8
|SPANISH_SPAIN.AL32UTF8
|
|Spanish
|es_ES.ISO-8859-1
|SPANISH_SPAIN.WE8ISO8859P1
|
|Spanish
|es_ES.ISO-8859-15
|SPANISH_SPAIN.WE8ISO8859P15
|
|Spanish (Mexico)
|es_MX.UTF-8
|MEXICAN SPANISH_MEXICO.AL32UTF8
|
|Spanish (Mexico)
|es_MX.ISO-8859-1
|MEXICAN SPANISH_MEXICO.WE8ISO8859P1
|
|Spanish (Mexico)
|es_MX.ISO-8859-15
|MEXICAN SPANISH_MEXICO.WE8ISO8859P15
|
|Portuguese (Brazilian)
|pt_BR.UTF-8
|BRAZILIAN PORTUGUESE_BRAZIL.AL32UTF8
|
|Portuguese (Brazilian)
|pt_BR.ISO-8859-1
|BRAZILIAN PORTUGUESE_BRAZIL.WE8ISO8859P1
|
|Portuguese (Brazilian)
|pt_BR.ISO-8859-15
|BRAZILIAN PORTUGUESE_BRAZIL.WE8ISO8859P15
|
|Japanese
|ja_JP.EUC-JP
|JAPANESE_JAPAN.JA16EUC
|
|Japanese
|ja_JP.UTF-8
|JAPANESE_JAPAN.AL32UTF8
|
|Korean
|ko_KR.EUC-KR
|KOREAN_KOREA.KO16KSC5601
|
|Korean
|ko_KR.UTF-8
|KOREAN_KOREA.AL32UTF8
|
|Chinese (simplified)
|zh_CN.GB18030
|SIMPLIFIED CHINESE_CHINA.ZHS32GB18030
|
|Chinese (simplified)
|zh_CN.UTF-8
|SIMPLIFIED CHINESE_CHINA.AL32UTF8
|
|Chinese (traditional)
|zh_TW.BIG5
|TRADITIONAL CHINESE_TAIWAN.ZHT16BIG5
|
|Chinese (traditional)
|zh_TW.UTF-8
|TRADITIONAL CHINESE_TAIWAN
|

0x02 设置字符编码的步骤
1.找到操作系统使用的字符集，并按上表找到对应的字符集名称。
2.修改客户端软件的字符集NLS_LANG环境变量设置。
linux环境下：
查看os字符集：cat /etc/sysconfig/i18n
配置相应的环境变量：su oracle
cd ~
vim .bash_profile
添加：export NLS_LANG




