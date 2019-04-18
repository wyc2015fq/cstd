# 纯客户端页面关键字搜索高亮jQuery插件 - z69183787的专栏 - CSDN博客
2015年03月02日 15:22:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1875

### 一、效果抢鲜展示
此插件是我今天下午写的，为了直观地看到是个什么东东，先展示效果，您可以狠狠地点击这里：[搜索页面文字jQuery插件Demo](http://www.zhangxinxu.com/study/201006/front-client-keyword-search.html)
下图为Demo页面的两个测试截图 – 测试页面HTML代码取自[豆瓣网帮助页面](http://www.douban.com/help/)：
![搜索插件搜索name结果 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-21_233259.png)![搜索豆瓣后的页面效果 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-21_233445.png)
**下载**
您可以狠狠地点击这里：[jquery.textSearch-1.0.js](http://www.zhangxinxu.com/study/js/jquery.textSearch-1.0.js)
文件较小，功能简单，恕不打包~~
### 二、功能简述
从上图也应该看出来了，就是对HTML页面上的文字内容进行高亮标记，这个可以用在内容搜索上。纯粹的客户端程序，与后台一点瓜葛都没有。
### 三、如何使用
使用方法是`textSearch`，具体为：$(选择器). textSearch(String,可选参数)。例如，
$("body").textSearch("世界杯");
表示的含义就是查询并红色高亮标注body标签下的所有的“世界杯”这个关键字，也就是页面下高亮标注所有的“世界杯”文字。又如：
$(".test").textSearch("空姐 凤姐 芙蓉姐",{markColor: "blue"});
则表示class中有test样式的所有标签下的“空姐”，“凤姐”，“芙蓉姐”文字用蓝色高亮标注。
此方法中，有一个参数是必须的，就是你要搜索的关键字字符串（默认状况下，使用空格隔开可表示多个关键字），还有一个可选参数，可自定义一些样式，关键字处理方法，回调函数等，具体参见本文下一部分。
### 四、一些可选参数
参见下表：
|参数|默认值|说明|
|----|----|----|
|divFlag|true|布尔型，true表示对字符串进行多关键字处理，false表示不处理，整个字符串看成1个关键字|
|divStr|" "|字符串，表示分割多个关键字的字符，默认为空格，如果divFlag为false，此参数将失效|
|markClass|""|代码高亮的class类，默认为没有样式，如果设置样式，将覆盖默认的红色高亮颜色值|
|markColor|"red"|字符串，指高亮文字的颜色值，默认红色。markClass不为空，则此参数失效。|
|nullReport|true|布尔型，表示当搜索结果为空时，是否弹出提示信息。默认弹出。|
|callback|return false;|回调函数，默认无效果。当存在搜索结果时执行。|
**其他一些说明**：
1. 高亮的文字的jQuery对象可以通过$(“span[rel='mark']“)获取。
2. 支持中英文和各类字符搜索，支持多关键字搜索。
![多关键字搜索的截图 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-22_200537.png)
### 五、结勒个语
抬头一看，文章短短的，妥妥的。js文件很小，加上注释不足100行，难点在于稍稍复杂的正则表达式和一些相应的字符串处理！最后，本人还是颗成长期的小菠菜，资历有限，出现问题难免。欢迎指出，不甚感谢。或者您有更好的搜索实现方案，也非常欢迎提出，交流。
转载请注明来自[张鑫旭-鑫空间-鑫生活](http://www.zhangxinxu.com/)[[http://www.zhangxinxu.com](http://www.zhangxinxu.com/)]
### 一、效果抢鲜展示
此插件是我今天下午写的，为了直观地看到是个什么东东，先展示效果，您可以狠狠地点击这里：[搜索页面文字jQuery插件Demo](http://www.zhangxinxu.com/study/201006/front-client-keyword-search.html)
下图为Demo页面的两个测试截图 – 测试页面HTML代码取自[豆瓣网帮助页面](http://www.douban.com/help/)：
![搜索插件搜索name结果 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-21_233259.png)![搜索豆瓣后的页面效果 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-21_233445.png)
**下载**
您可以狠狠地点击这里：[jquery.textSearch-1.0.js](http://www.zhangxinxu.com/study/js/jquery.textSearch-1.0.js)
文件较小，功能简单，恕不打包~~
### 二、功能简述
从上图也应该看出来了，就是对HTML页面上的文字内容进行高亮标记，这个可以用在内容搜索上。纯粹的客户端程序，与后台一点瓜葛都没有。
### 三、如何使用
使用方法是`textSearch`，具体为：$(选择器). textSearch(String,可选参数)。例如，
$("body").textSearch("世界杯");
表示的含义就是查询并红色高亮标注body标签下的所有的“世界杯”这个关键字，也就是页面下高亮标注所有的“世界杯”文字。又如：
$(".test").textSearch("空姐 凤姐 芙蓉姐",{markColor: "blue"});
则表示class中有test样式的所有标签下的“空姐”，“凤姐”，“芙蓉姐”文字用蓝色高亮标注。
此方法中，有一个参数是必须的，就是你要搜索的关键字字符串（默认状况下，使用空格隔开可表示多个关键字），还有一个可选参数，可自定义一些样式，关键字处理方法，回调函数等，具体参见本文下一部分。
### 四、一些可选参数
参见下表：
|参数|默认值|说明|
|----|----|----|
|divFlag|true|布尔型，true表示对字符串进行多关键字处理，false表示不处理，整个字符串看成1个关键字|
|divStr|" "|字符串，表示分割多个关键字的字符，默认为空格，如果divFlag为false，此参数将失效|
|markClass|""|代码高亮的class类，默认为没有样式，如果设置样式，将覆盖默认的红色高亮颜色值|
|markColor|"red"|字符串，指高亮文字的颜色值，默认红色。markClass不为空，则此参数失效。|
|nullReport|true|布尔型，表示当搜索结果为空时，是否弹出提示信息。默认弹出。|
|callback|return false;|回调函数，默认无效果。当存在搜索结果时执行。|
**其他一些说明**：
1. 高亮的文字的jQuery对象可以通过$(“span[rel='mark']“)获取。
2. 支持中英文和各类字符搜索，支持多关键字搜索。
![多关键字搜索的截图 张鑫旭-鑫空间-鑫生活](http://image.zhangxinxu.com/image/blog/201006/2010-06-22_200537.png)
### 五、结勒个语
抬头一看，文章短短的，妥妥的。js文件很小，加上注释不足100行，难点在于稍稍复杂的正则表达式和一些相应的字符串处理！最后，本人还是颗成长期的小菠菜，资历有限，出现问题难免。欢迎指出，不甚感谢。或者您有更好的搜索实现方案，也非常欢迎提出，交流。
转载请注明来自[张鑫旭-鑫空间-鑫生活](http://www.zhangxinxu.com/)[[http://www.zhangxinxu.com](http://www.zhangxinxu.com/)]
