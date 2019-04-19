# vc获取操作系统的语言 - xqhrs232的专栏 - CSDN博客
2017年08月10日 11:20:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：230
原文地址::[http://blog.csdn.net/davintanger/article/details/7332637](http://blog.csdn.net/davintanger/article/details/7332637)
相关文章
1、[C，C++，VC判断当前操作系统是那种语言 GetSystemDefaultLangID（）](http://blog.csdn.net/kyfvc/article/details/41074729)----[http://blog.csdn.net/kyfvc/article/details/41074729](http://blog.csdn.net/kyfvc/article/details/41074729)
GetSystemDefaultLCID
GetUserDefaultUILanguage
GetSystemDefaultUILanguage()
对LCID lcid 的值，可以用 WORD PriLan = PRIMARYLANGID(lcid); WORD SubLan = SUBLANGID(lcid);两个方法取得主语言和子语言。如主语言是中文， 那么子语言可以分为中文简体和中文繁体。
LCID lcid = GetSystemDefaultLCID();
if(lcid == 0x409)
{   
        AfxMessageBox("美国英语");   
}
if(lcid == 0x404)
{   
        AfxMessageBox("中国繁体");//台湾   
}
if(lcid == 0x804)
{   
        AfxMessageBox("中国简体");   //指中华人民共和国   
}  
语言对照表：
|0x0c04|Chinese|Hong Kong SAR|950|ZHH|
|----|----|----|----|----|
|0x1404|Chinese|Macao SAR|950|ZHM|
|0x0804|Chinese|PRC|936|CHS|
|0x1004|Chinese|Singapore|936|ZHI|
|0x0404|Chinese|Taiwan|950|CHT|
|0x0827|Classic Lithuanian|Lithuania|1257|LTC|
|0x040f|Icelandic|Iceland|1252|ISL|
|----|----|----|----|----|
|0x0421|Indonesian|Indonesia (Bahasa)|1252|IND|
|0x0410|Italian|Italy|1252|ITA|
|0x0810|Italian|Switzerland|1252|ITS|
|0x0411|Japanese|Japan|932|JPN|
|0x044b|Kannada|India (Kannada script)|Unicode only|KAN|
|0x043f|Kazakh|Kazakstan|1251|KKZ|
|0x0457|Konkani|India|Unicode only|KNK|
|0x0412|Korean|Korea|949|KOR|
|0x0c09|English|Australia|1252|ENA|
|----|----|----|----|----|
|0x2809|English|Belize|1252|ENL|
|0x1009|English|Canada|1252|ENC|
|0x2409|English|Caribbean|1252|ENB|
|0x1809|English|Ireland|1252|ENI|
|0x2009|English|Jamaica|1252|ENJ|
|0x1409|English|New Zealand|1252|ENZ|
|0x3409|English|Philippines|1252|ENP|
|0x1c09|English|South Africa|1252|ENS|
|0x2c09|English|Trinidad|1252|ENT|
|0x0809|English|United Kingdom|1252|ENG|
|0x0409|English|United States|1252|USA|
|0x3009|English|Zimbabwe|1252|ENW|
|0x0425|Estonian|Estonia|1257|ETI|
|0x0438|Faeroese|Faeroe Islands|1252|FOS|
|0x0429|Farsi|Iran|1256|FAR|
|0x040b|Finnish|Finland|1252|FIN|
|0x080c|French|Belgium|1252|FRB|
|0x0c0c|French|Canada|1252|FRC|
|0x040c|French|France|1252|FRA|
|0x140c|French|Luxembourg|1252|FRL|
|0x180c|French|Monaco|1252|FRM|
|0x100c|French|Switzerland|1252|FRS|
