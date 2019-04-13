
# 浏览器内核分析5 -- CSS解释器和样式布局 - 谢杨易的博客 - CSDN博客

2017年02月16日 19:34:42[谢杨易](https://me.csdn.net/u013510838)阅读数：606



# 1 主要类
CSSStyleSheet：描述CSS文件，不论是内嵌的，还是引用的外部文件
CSSRule：一条CSS规则，解析如下的CSS代码得到：
```python
p
```
```python
,
```
```python
div
```
```python
{
```
```python
​
```
```python
margin-top
```
```python
:
```
```python
3
```
```python
px
```
```python
;
```
```python
​
```
```python
color
```
```python
:
```
```python
#ffffff
```
```python
;
```
```python
}
```
包含选择器和属性表两部分
CSSSelector：选择器
属性集合：一个CSSPropertyID: CSSValue组成的map，描述了元素的属性。
![](http://pic002.cnblogs.com/images/2011/265173/2011110316290151.png)
# 2 解释过程
解释过程将CSS文件字节流变成渲染引擎的内部规则表示，得到样式规则集合。同样分为三个步骤
词法分析：字符串变为一个个词语，这个和HTML解析阶段的词法分析很像
语法分析：由CSSParser调用CSSGrammer，解析startSelector, endSelector,     startRuleBody, startProperty, parseValue, endRuleBody
生成样式规则StyleRule
CSS样式属性都有默认值，每个Webkit移植都可以自行定义它们。
# 3 样式匹配
CSS样式规则解析结果保存到DocumentRuleSets中。StyleResolver根据元素的id class 标签等信息，从样式规则集中来获取最精确的匹配，保存在RenderStyle中，最终在RenderObject树构建的布局和重绘阶段所使用。

