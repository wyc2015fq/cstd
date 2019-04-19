# xpath  轴,节点之间的关系 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月04日 13:55:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：484
[http://www.w3school.com.cn/xpath/xpath_axes.asp](http://www.w3school.com.cn/xpath/xpath_axes.asp)
[http://www.freeformatter.com/xpath-tester.html](http://www.freeformatter.com/xpath-tester.html) 测试
轴可定义相对于当前节点的节点集。
|轴名称|结果|
|----|----|
|ancestor|选取当前节点的所有先辈（父、祖父等）。|
|ancestor-or-self|选取当前节点的所有先辈（父、祖父等）以及当前节点本身。|
|attribute|选取当前节点的所有属性。|
|child|选取当前节点的所有子元素。|
|descendant|选取当前节点的所有后代元素（子、孙等）。|
|descendant-or-self|选取当前节点的所有后代元素（子、孙等）以及当前节点本身。|
|following|选取文档中当前节点的结束标签之后的所有节点。|
|namespace|选取当前节点的所有命名空间节点。|
|parent|选取当前节点的父节点。|
|preceding|选取文档中当前节点的开始标签之前的所有节点。|
|preceding-sibling|选取当前节点之前的所有同级节点。|
|self|选取当前节点。|
位置路径可以是绝对的，也可以是相对的。
绝对路径起始于正斜杠( / )，而相对路径不会这样。

### 斜杠分割的step包括：
轴定义所选节点与当前节点之间的树关系节点测试（node-test）识别某个轴内部的节点零个或者更多谓语（predicate）更深入地提炼所选的节点集
### 步的语法：
轴名称::节点测试[谓语]
|例子|结果|
|----|----|
|child::book|选取所有属于当前节点的子元素的 book 节点。|
|attribute::lang|选取当前节点的 lang 属性。|
|child::*|选取当前节点的所有子元素。|
|attribute::*|选取当前节点的所有属性。|
|child::text()|选取当前节点的所有文本子节点。|
|child::node()|选取当前节点的所有子节点。|
|descendant::book|选取当前节点的所有 book 后代。|
|ancestor::book|选择当前节点的所有 book 先辈。|
|ancestor-or-self::book|选取当前节点的所有 book 先辈以及当前节点（如果此节点是 book 节点）|
|child::*/child::price|选取当前节点的所有 price 孙节点。|
