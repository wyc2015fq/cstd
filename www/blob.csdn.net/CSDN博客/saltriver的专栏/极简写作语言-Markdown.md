
# 极简写作语言-Markdown - saltriver的专栏 - CSDN博客


2016年10月30日 14:19:53[saltriver](https://me.csdn.net/saltriver)阅读数：1406


你写作时思如泉涌，却因排版问题浪费了太多时间，中断了思路。
你有没有因为版本问题，而出现打不开文件的尴尬。
你有没有想过在文章中无缝贴入源代码，公式等内容。
**那么你需要掌握这种工具—Markdown**
#### 1.
#### Markdown 是一种极简”标记语言”
Markdown的语法非常简单
常用的标记符号不超过10个
30分钟内即可掌握。
#### 2.
#### Markdown 用简洁的语法代替排版，目标是”易读易写”
不像一般我们用的字处理软件 Word 或wps，有大量的排版、字体设置。
Markdown用「标记」语法，来代替常见的排版，使我们专心于写作。
需要注意的是标记符号和文本之间有一个空格。
#### 3.
#### Markdown 获得了大量支持和广泛应用
支持 Markdown 语法的编辑器有很多，包括“网易云笔记”、“印象笔记”、”简书”、各种博客（csdn等）
Markdown 导出格式随心所欲，可以导出HTML 、PDF等格式的文件。
#### 4.
#### Markdown 极简语法说明
##### 1) 标题
\#：一级标题
\#\#：二级标题
\#\#\#：三级标题
\#\#\#\#：四级标题
\#\#\#\#\#：五级标题
\#\#\#\#\#\#：六级标题
```python
# 一级标题
```
```python
## 二级标题
```
```python
### 三级标题
```
```python
#### 四级标题
```
```python
##### 五级标题
```
```python
###### 六级标题
```
效果：
# 一级标题
## 二级标题
### 三级标题
#### 四级标题
##### 五级标题
###### 六级标题
##### 2) 段落/换行
直接使用空行隔开形成段落
行尾按下双空格即为换行
##### 3) 粗体/斜体/下划线
用以下引号内的符号放置在需要设置的文字前后
““：粗体**
““：斜体*
“++”：++下划线++
```python
**我是粗体**
```
```python
*我是斜体*
```
```python
+
```
```python
+
```
```python
我是下划线
```
```python
+
```
```python
+
```
效果：
**我是粗体**
*我是斜体*
++我是下划线++
##### 4) 无序列表
以下引号内的2种符号都可以，放置在需要设置的文字前面
“-“
“*”
```python
-
```
```python
我是无序列表
```
```python
1
```
```python
* 我是无序列表2
```
效果：
- 我是无序列表1
* 我是无序列表2
##### 5) 有序列表
“1.”
“2.”
```python
1.
```
```python
我是有序列表1
```
```python
2.
```
```python
我是有序列表2
```
效果：
1. 我是有序列表1
2. 我是有序列表2
##### 6) 引用
“> “，引用文字前加该符号
落霞与孤鹜齐飞，秋水共长天一色
7) 链接
[文字链接] (链接地址)
[百度] (http://www.baidu.com)
[百度](http://www.baidu.com)
![] (链接地址)，多一个感叹号
![百度] (https://ss0.bdstatic.com/5aV1bjqh_Q23odCf/static/superman/img/logo/bd_logo1_31bdc765.png)
![百度](https://ss0.bdstatic.com/5aV1bjqh_Q23odCf/static/superman/img/logo/bd_logo1_31bdc765.png)
8) 分割线
“***“,一行放三个星号即是分割线
---

9) 表格
header1| header 2---|---row1col1| row 1 col 2row2col1| row 2 col 2header 1header 2|
|---|---|
row 1 col 1row 1 col 2|
row 2 col 1row 2 col 2|
10) 公式
支持LaTeX公式
$E=mc^2$$H=\Sigmap \cdot log(p)$E=mc^2
H=\Sigma p \cdot log(p)
11) 字体/字号/颜色
使用内嵌HTML
<fontcolor=\#FF8C00size=3face="黑体">我是字体/字号/颜色</font>效果：
我是字体/字号/颜色

