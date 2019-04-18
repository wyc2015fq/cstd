# webdriver通过class获取元素 - z69183787的专栏 - CSDN博客
2016年08月10日 11:34:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3647
通过webdriver 取得页面元素的时候，有时候由于某些元素只有样式类，没有ID和NAME。这个时候我们就需要通过特别的方式获取该元素了。
1：当元素只有一个样式，比如 class="style1" ，这个时候可以通过：
**find_element_by_class_name("style1")   **获取
2：当元素多个样式的时候，比如 class="style1 style2 style3" ，这个时候可以通过：
** driver.find_element_by_css_selector(".style1.style2.style3")   **获取
