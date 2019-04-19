# selenium处理元素定位到了点击无效问题 - _天枢 - 博客园
## [selenium处理元素定位到了点击无效问题](https://www.cnblogs.com/yhleng/p/10212534.html)
2019-01-03 09:52 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10212534)
**在WEB自动化测试过程中，经常会遇到这样的问题：**
元素定位到了，但是点击无效？有人可能会问了，怎么判断元素定位到了，这个问题很好判断
**1.给元素加高亮显示**
```
self.driver.execute_script(
            "arguments[0].setAttribute('style', arguments[1]);",
            element,
            "border: 2px solid red;" #边框border:2px; red红色
        )
```
arguments[0] 这个参数，可以理解为python中的字符串格式化。比如"{}{}".format('aaa','bbbb')
**2.执行点击过程不报错。**
不会报一些找不到元素之类的错误。
怎么判断元素定位到了，点击没成功(这里说一下，一般是做了点击操作，只是我们直观去判断没有点击成功)
那么，现在来说一下如何解决这个问题。
**解决问题：**
**1.分析一下，默认selenium的点击操作是点元素0，0点坐标，那么我们可以在元素x,y坐标上加5，让点击操作往里一点。**
**导入包ActionChains,**
```
from selenium.webdriver.common.action_chains import ActionChains
```
```
ele = self.find_element(*loc)
ActionChains(driver).move_to_element(ele).move_by_offset(5,5).click().perform()
```
通过如上代码，先定位元素，然后移动到元素，在元素基础上做偏移x,y 5,5然后点击。
需要注意的是最后的perform()。要加上这个。否则不生效。
**2.通过js去点击元素。**
可以通过dom去定位，或者jquery定位。然后点击操作。这里不具体说明了。可以找度娘。
**3.可以分析代码，如果有直接调用函数的地方，可以通过js直接调用。**
要说明的是，这个方法，一般不可行。因为js前端代码会压缩加密。所以上线后我们看到的是压缩的。
G
M
T
||Detect languageAfrikaansAlbanianAmharicArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CorsicanCroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchFrisianGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHawaiianHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanKurdishKyrgyzLaoLatinLatvianLithuanianLuxembourgishMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPashtoPersianPolishPortuguesePunjabiRomanianRussianSamoanScots GaelicSerbianSesothoShonaSindhiSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshXhosaYiddishYorubaZulu||AfrikaansAlbanianAmharicArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CorsicanCroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchFrisianGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHawaiianHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanKurdishKyrgyzLaoLatinLatvianLithuanianLuxembourgishMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPashtoPersianPolishPortuguesePunjabiRomanianRussianSamoanScots GaelicSerbianSesothoShonaSindhiSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshXhosaYiddishYorubaZulu||||||
Text-to-speech function is limited to 200 characters
|||[Options](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?bbl) : [History](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?hist) : [Feedback](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?feed) : [Donate](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GD9D8CPW8HFA2)|Close|

