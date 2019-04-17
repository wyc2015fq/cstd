# selenium使用Xpath+CSS+JavaScript+jQuery的定位方法 - 专治各种定位不准并发症 - zhusongziye的博客 - CSDN博客





2018年01月18日 16:42:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：110








跟你说，你总是靠那个firebug，chrome的F12啥的右击复制xpath绝对总有一天踩着地雷炸的你死活定位不到，这个时候就需要自己学会动手写xpath，人脑总比电脑聪明，开始把xpath语法给我学起来！




第1种方法：通过绝对路径做定位（相信大家不会使用这种方式）

By.xpath("html/body/div/form/input")


By.xpath("//input")




第2种方法：通过元素索引定位

By.xpath("//input[4]")




第3种方法：使用xpath属性定位

By.xpath("//input[@id='kw1']")

By.xpath("//input[@type='name' and @name='kw1']")




第4种方法：使用部分属性值匹配（最强大的方法）

By.xpath("//input[starts-with(@id,'nice')
By.xpath("//input[ends-with(@id,'很漂亮')


By.xpath("//input[contains(@id,'那么美')]")









