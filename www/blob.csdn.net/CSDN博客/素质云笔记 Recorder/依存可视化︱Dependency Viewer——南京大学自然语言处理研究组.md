
# 依存可视化︱Dependency Viewer——南京大学自然语言处理研究组 - 素质云笔记-Recorder... - CSDN博客

2017年07月08日 16:33:37[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2035


来源网页：[http://nlp.nju.edu.cn/tanggc/tools/DependencyViewer.html](http://nlp.nju.edu.cn/tanggc/tools/DependencyViewer.html)
视频演示网页：[http://nlp.nju.edu.cn/tanggc/tools/DependencyViewer_demo.html](http://nlp.nju.edu.cn/tanggc/tools/DependencyViewer_demo.html)
可视化效果效果如下：
![这里写图片描述](https://img-blog.csdn.net/20170708162912772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170708162922186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170708162922186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.
[
](https://img-blog.csdn.net/20170708162922186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1、数据格式如下：
[
](https://img-blog.csdn.net/20170708162922186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)`1``赵宁  赵宁  NR  NR  _``4``d``-genetive``_   _``2``的   的   DEG DEG _``1``aux``-depend``_   _``3``哀乐  哀乐  NN  NN  _``4``d``-attribute``_   _``4``中年  中年  NN  NN  _``9``scope   _   _``5``，   ，   PU  PU  _``9``PU  _   _``6``转折  转折  NN  NN  _``9``patient _   _``7``不   不   AD  AD  _``9``negation    _   _``8``可   可   VV  VV  _``9``modal   _   _``9``谓   谓   VV  VV  _``0``ROOT    _   _``10``不   不   AD  AD  _``11``negation    _   _``11``大   大   VA  VA  _``9``d``-attribute``_   _``12``。   。   PU  PU  _``9``PU  _   _`[
](https://img-blog.csdn.net/20170708162922186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)当然也可以存成txt，只要格式规范
.
## 2、search功能
![这里写图片描述](https://img-blog.csdn.net/20170708163105057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中可以通过输入关键词检索。
句子长度3~15，带标签错误数目0~3，不带标签错误数目1，含有“不”且词性为AD，同时包含“转折”依存于“谓”
.
## 3、statistic功能
![这里写图片描述](https://img-blog.csdn.net/20170708163208494?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以通过整个文档的一些属性。

