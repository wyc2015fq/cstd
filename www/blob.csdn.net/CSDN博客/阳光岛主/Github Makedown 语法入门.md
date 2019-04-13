
# Github Makedown 语法入门 - 阳光岛主 - CSDN博客

2017年08月09日 15:49:27[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1152


GitHub上创建GitHub项目，会包含了一个 README.md 文件，此文件编辑就会用到Markdown
本文将详细介绍Markdown文件（md文件）的语法。
本文示例开源项目，请见GitHub：
[https://github.com/mimvp/mimvp-markdown-syntax](https://github.com/mimvp/mimvp-markdown-syntax)
**1. 新建GitHub项目**
文本以米扑科技的项目库为例：[mimvp](https://github.com/mimvp)
新建项目链接：https://github.com/organizations/mimvp/repositories/new
![github-makedown-yu-fa-ru-men-01](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-01.png)
**2. 查看创建项目**
[mimvp-markdown-syntax](https://github.com/mimvp/mimvp-markdown-syntax)
![github-makedown-yu-fa-ru-men-02](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-02.png)
**3. 下载编辑 README.md**
**0） 克隆项目到本地**
https://github.com/mimvp/mimvp-markdown-syntax.git
编辑好README.md文件后，再提交上传到GitHub，这种方式效率低。
推荐一种更快捷的编辑README.md方式，直接在Github上编辑（推荐）
a）点击 README.md文件
b）点击下图红框，直接编辑
![github-makedown-yu-fa-ru-men-03](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-03.png)
**1）Markdown 标题**
markdown 支持两种标题的语法：Setext 和 atx 形式，两者也可以结合使用，效果更好！
**a）Setext 形式**
用底线的形式，利用 === （最高阶标题）和 ----- （第二阶标题）
**Markdown 语法：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|米扑科技
|========
|官网 [http:|//mimvp|.com](http:|//mimvp|.com)
|官网 <a target=|"_blank"|href=|"http://mimvp.com"|>http:|//mimvp|.com<|/a|>

|米扑代理
|--------
|官网 [http:|//proxy|.mimvp.com](http:|//proxy|.mimvp.com)

|米扑域名
|--------
|官网 <a target=|"_blank"|href=|"http://domain.mimvp.com"|>http:|//domain|.mimvp.com<|/a|>

|米扑财富
|--------
|官网 <a href=|"http://money.mimvp.com"|target=|"_blank"|>http:|//money|.mimvp.com<|/a|>
|

**Markdown****结果：**
![github-makedown-yu-fa-ru-men-04](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-04.png)
**b）atx 形式**
用\#号标识标题，最多六级标题（不支持七级、八级等标题，会直接显示出文字）
**Markdown 语法：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|标题示例
|========
|\# 一级标题
|\#\# 二级标题
|\#\#\# 三级标题
|\#\#\#\# 四级标题
|\#\#\#\#\# 五级标题
|\#\#\#\#\#\# 六级标题
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-06](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-06.png)
**2）Markdown 代码块**
代码块，以大于号（>）开头。若末尾不加换行符<br/>，则所有文字会连接在一起不会换行。
**Markdown 语法：**
|1
|2
|3
|4
|5
|6
|Markdown 代码块
|========
|> 米扑科技，小而美，简而信，工匠艺术的互联网服务。 <br/>
|> 米扑团队，来自百度、小米、创新工场、阿里，核心研发，实战经验丰富。 <br/>
|> 米扑产品，已上线服务包括米扑代理、米扑域名、米扑支付、米扑财富、米扑博客等。 <br/>
|> 米扑科技官网： [mimvp.com](http:|//mimvp|.com)
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-07](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-07.png)
**3）Markdown 引用块**
引用块，以特殊引号（`）开头。注意：不是单引号，而是特殊引号，在键盘左上角的“esc”左下方的那个按键
**Markdown 语法：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|Markdown 引用块
|========
|米扑科技简介
|```
|米扑科技，小而美，简而信，工匠艺术的互联网服务。
|米扑团队，来自百度、小米、创新工场、阿里，核心研发，实战经验丰富。
|米扑产品，已上线服务包括米扑代理、米扑域名、米扑支付、米扑财富、米扑博客等。
|米扑科技官网： mimvp.com
|```
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-08](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-08.png)
**4）Markdown 代码高亮**
代码高亮，以特殊引号（`）开头，并跟上代码语言，例如：python、java、javascript、ruby、perl、go、c、c++、bash等。
注意：不是单引号，而是特殊引号，在键盘左上角的“esc”左下方的那个按键
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|Markdown 代码高亮
|========
|```python
|\#!/usr/bin/env python
|\# -*- coding:utf-8 -*-
|if|__name__ ==|"__main__"|:
|echo|"hello mimvp"
|print(|"I love mimvp.com"|)
|```
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-09](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-09.png)
**5）Markdown 字体加粗**
* 或 _，表示斜体
** 或 __，表示粗体
~~ 表示删除线
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|Markdown 字体加粗
|========
|我爱 米扑科技  <br/>
|我爱 *米扑科技*  （斜体） <br/>
|我爱 _米扑科技_  （斜体） <br/>
|我爱 **米扑科技**  （粗体） <br/>
|我爱 __米扑科技__  （粗体） <br/>
|我爱 ***米扑*科技**  （斜体 + 粗体） <br/>
|我爱 ~~米扑科技~~  （删除线） <br/>
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-10](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-10.png)
**6）Markdown 无序列表**
*（星号） +（加号） - （减号）表示无序列表
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|Markdown 无序列表
|========
|*（星号）
|--------
|* 米扑科技
|* 米扑代理
|* 米扑域名
|* 米扑财富

|+（加号）
|--------
|+ 米扑科技
|+ 米扑代理
|+ 米扑域名
|+ 米扑财富

|-（减号）
|--------
|- 米扑科技
|- 米扑代理
|- 米扑域名
|- 米扑财富
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-11](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-11.png)
**7）Markdown 有序列表**
直接用1. xxx     2. xxx   3. xxx 编辑
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|Markdown 有序列表
|========
|1. 米扑科技
|2. 米扑代理
|3. 米扑域名
|4. 米扑财富
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-12](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-12.png)
**8）Markdown 插入链接**
Markdown 插入链接，有两种方式：
方式1） [mimvp.com](http://mimvp.com)
方式2）<a target="_blank" href="http://mimvp.com">mimvp.com</a>  （推荐）
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|米扑科技
|========
|官网 [http:|//mimvp|.com](http:|//mimvp|.com)
|官网 <a target=|"_blank"|href=|"http://mimvp.com"|>http:|//mimvp|.com<|/a|>

|米扑代理
|--------
|官网 [http:|//proxy|.mimvp.com](http:|//proxy|.mimvp.com)

|米扑域名
|--------
|官网 <a target=|"_blank"|href=|"http://domain.mimvp.com"|>http:|//domain|.mimvp.com<|/a|>

|米扑财富
|--------
|官网 <a href=|"http://money.mimvp.com"|target=|"_blank"|>http:|//money|.mimvp.com<|/a|>
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-04](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-04.png)
**9）Markdown 插入图片**
Markdown 插入图片有两种方式：
方式1） ![mimvp-logo.png](./mimvp-logo.png)
方式2） <img src="./mimvp-logo.png" title="mimvp-logo.png" />
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|Markdown 插入图片
|========
|\#\#\# 1. 米扑科技（默认）
|![mimvp-logo.png](.|/mimvp-logo|.png)

|\#\#\# 2. 米扑科技（img）
|<img src=|"./mimvp-logo.png"|title=|"mimvp-logo.png"|/>
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-13](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-13.png)
**10）Markdown 表格**
Makrdown 支持table表格，语法跟wiki类似。同时，也支持table标签
**方式1**
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|Markdown 表格
|========
||产品|网址|年份|
||----|-----|-----|
||米扑代理|[proxy.mimvp.com](http:|//proxy|.mimvp.com)|2014|
||米扑域名|[domain.mimvp.com](http:|//domain|.mimvp.com)|2015|
||米扑支付|[pay.mimvp.com](http:|//pay|.mimvp.com)|2016|
||米扑财富|[money.mimvp.com](http:|//money|.mimvp.com)|2017|
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-14](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-14.png)
**方式2**
**Markdown 语法：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|<table>
|<|tr|>
|<td colspan=|"3"|color=|"red"|>米扑产品<|/td|>
|<|/tr|>
|<|tr|>
|<td>米扑代理<|/td|>
|<td>http:|//proxy|.mimvp.com<|/td|>
|<td>2014<|/td|>
|<|/tr|>
|<|tr|>
|<td>米扑域名<|/td|>
|<td>http:|//domain|.mimvp.com<|/td|>
|<td>2015<|/td|>
|<|/tr|>
|<|tr|>
|<td>米扑支付<|/td|>
|<td>http:|//pay|.mimvp.com<|/td|>
|<td>2016<|/td|>
|<|/tr|>
|<|tr|>
|<td>米扑财富<|/td|>
|<td>http:|//money|.mimvp.com<|/td|>
|<td>2017<|/td|>
|<|/tr|>
|<|/table|>
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-15](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-15.png)
**11）Markdown 多选项**
多选项，结合了多种元素，如无序列表减号（-），方括号等
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|Markdown 多选项
|========
|米扑产品包含？
|- [x] 米扑代理
|- [x] 米扑域名
|- [x] 米扑支付
|- [x] 米扑财富
|- [ ] 米扑基金
|- [ ] 米扑期货
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-16](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-16.png)
**12）Markdown emoji表情**
**Markdown 语法:**
|1
|2
|3
|4
|Markdown emoji
|========
|@mimvp is :+1: This PR looks great - I love mimvp.com  :smiley:
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-17](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-17.png)
**13）Markdown diff 高亮对比**
diff 语法是代码高亮语法的一种，以 `+ `开头表示新增，`- `开头表示删除。
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|Markdown|diff|高亮对比
|----------
|```|diff
|+ 增加 emoji.md
|- 删除 emoji.txt
|```
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-18](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-18.png)
**14）Markdown 其它知识**
知识1：  [回到顶部](\#readme)
知识2： 版本符号 &copy;
**Markdown 语法:**
|1
|2
|3
|4
|5
|Markdown 其它知识
|----------
||`[回到顶部](|\#readme)`|[回到顶部](\#readme)|
||`版本符号 &copy;`|&copy;|
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-19](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-19.png)
**15）Markdown 版权格式**
一般开源项目，末尾会追加一些开原作者的信息，例如米扑科技的作者信息如下，供参考。
**Markdown 语法:**
|1
|2
|3
|4
|5
|6
|米扑科技
|----------
|<img src=|"./mimvp-logo.png"|title=|"mimvp-logo.png"|/>
|All Right &copy; [mimvp.com](http:|//mimvp|.com)   :blush:
|

**Markdown 结果：**
![github-makedown-yu-fa-ru-men-20](http://blog.mimvp.com/wp-content/uploads/2017/08/github-makedown-yu-fa-ru-men-20.png)
本文Markdown示例，请见米扑科技的 GitHub：
[https://github.com/mimvp/mimvp-markdown-syntax](https://github.com/mimvp/mimvp-markdown-syntax)



