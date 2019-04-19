# iOS开发 CoreText中的高度计算问题 - 月若无涯的专栏 - CSDN博客
2013年11月08日 13:27:50[月若无涯](https://me.csdn.net/u010124617)阅读数：3240
iOS开发中，对于复杂的文本处理，通常使用CoreText，优点是灵活，加载速度也比UIWebView块。
这次是有一个技术群里人问CoreText中的文字高度计算方法，CTFramesetterSuggestFrameSizeWithConstraints得到的高度和他预想的不一样，比如设置字体的字号是17，间距是4，三行的高度居然是68。
那么我来为大家解析一下这个方法的高度是怎么计算得到的。
(本文章偏向于计算高度的原理，技术实现以后再介绍，对CoreText不太熟悉也没关系)
首先对于一段文字来说，整段文字的高度是有几方面决定的：行高、行数和行间距。
在CoreText中，行高与所用的字体大小有关，行间距是可以由我们开发者自行设置的，行数则根据对段落的宽度、折行方式的调整来动态计算的，一般情况下我们不关心有几行。
那么行高是怎么得到的呢？行高=Ascent+Descent+Leading。关于文字的属性具体的可以看这个图：![](http://livedocs.adobe.com/flex/3_cn/langref/images/text-metrics.jpg)
fontLeading
检查CoreText的API，可以发现CTFontGetAscent、CTFontGetDescent和CTFontGetLeading这三个方法。
行高= CTFontGetAscent(coreTextFont)+CTFontGetDescent(coreTextFont)+CTFontGetLeading(coreTextFont);
而通过我代码实测：17号字体的Ascender=16.184000,Descent=4.097000,Leading=0.000000，则行高=20.2（小数点后为误差）
CoreText的CTFramesetterSuggestFrameSizeWithConstraints方法返回的总高度为：行高*行数+行间距*(行数-1)，（68=20*3+4*(3-1)符合条件）。
为什么是这样，因为行间距在CoreText中，默认是相邻两行的间距，第一行上面和最后一行的下面没有相邻的行，所以总间距数是(行数-1)。
好了，通过这种方式，大家就可以了解整个CTFramesetterSuggestFrameSizeWithConstraints方法返回的高度是如何得到了。
对于其他平台的文字处理，应该也可供参考。

