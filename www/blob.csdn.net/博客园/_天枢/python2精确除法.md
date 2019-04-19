# python2精确除法 - _天枢 - 博客园
## [python2精确除法](https://www.cnblogs.com/yhleng/p/9223944.html)
2018-06-25 13:38 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9223944)
**python2和python3除法的最大区别:**
python2:
```
print 500/1000
```
python2结果:**取整数部分,小数并没有保留**
```
0
Process finished with exit code 0
```
python3:
```
print 500/1000
```
python3结果:**得到真实结果,小数保留**
```
0.5
Process finished with exit code 0
```
**那么,如果python2想保留小数部分,要怎么做呢?**
只需要增加一个导入包.就可以了.并不需要其它操作
```
from __future__ import division #用于/相除的时候,保留真实结果.小数
```
增加导入包后的,python2操作:
```
#coding:utf-8
from __future__ import division
print 500/1000
```
结果:
```
0.5
Process finished with exit code 0
```
**还有另一种方式.将除数或被除数两个其它至少一个转换成float型:**
```
print float(500)/1000
```
**结果:**
```
0.5
Process finished with exit code 0
```
G
M
T
||Detect languageAfrikaansAlbanianArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanLaoLatinLatvianLithuanianMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPersianPolishPortuguesePunjabiRomanianRussianSerbianSesothoSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshYiddishYorubaZulu||AfrikaansAlbanianArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanLaoLatinLatvianLithuanianMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPersianPolishPortuguesePunjabiRomanianRussianSerbianSesothoSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshYiddishYorubaZulu||||||
Text-to-speech function is limited to 200 characters
|||[Options](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?bbl) : [History](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?hist) : [Feedback](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?feed) : [Donate](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GD9D8CPW8HFA2)|Close|

