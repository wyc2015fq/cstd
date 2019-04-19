# Python基础(八) yaml在python中的使用 - _天枢 - 博客园
## [Python基础(八) yaml在python中的使用](https://www.cnblogs.com/yhleng/p/7778178.html)
2017-11-03 14:57 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7778178)
**yaml 通常用来存储数据，类似于json**
#### YAML 简介
YAML（Yet Another Markup Language），一种直观的能够被电脑识别的数据序列化格式，是一个可读性高并且容易被人类阅读，容易和脚本语言交互，用来表达资料序列的编程语言。它参考了其它多种语言，包括：XML、C语言、Python、Perl以及电子邮件格式RFC2822，是类似于标准通用标记语言的子集XML的数据描述语言，语法比XML简单很多。
 由于YAML使用空白字符和分行来分隔资料，使得它特别适合用 grep、Python、Perl、Ruby 操作。
#### 适用场景
- 脚本语言
由于实现简单，解析成本很低，YAML 特别适合在脚本语言中使用。列一下现有的语言实现：Ruby，Java，Perl，Python，PHP，OCaml，JavaScript，除了Java，其他都是脚本语言。
- 序列化
YAML 比较适合做序列化。因为它是宿主语言数据类型直转的。
- 配置文件
YAML 做配置文件也不错。写 YAML 要比写 XML 快得多(无需关注标签或引号)，并且比 INI 文档功能更强。
由于兼容性问题，不同语言间的数据流转建议不要用 YAML。
#### 语言比较
- XML
- 和 XML 相比，YAML 具有以下的优势- 可读性好
- 和脚本语言的交互性好
- 使用实现语言的数据类型
- 可以基于流来处理
- 表达能力强，扩展性好
- XML 解析效率比较低，不过支持自定义的数据类型
- YAML类似于XML的数据描述语言，语法比XML简单很多，YAML试图用一种比XML更敏捷的方式，来完成XML所完成的任务。
- JSON
- JSON 的语法其实是 YAML 的子集，大部分的 JSON 文件都可以被 YAML 的剖析器剖析。虽然大部分的数据分层形式也可以使用类似 JSON 的格式，不过 YAML 并不建议这样使用，除非这样编写能让文件可读性增加，更重要的是，YAML 的许多扩展在 JSON 是找不到的，如：进阶资料形态、关系锚点、字串不需要引号、映射资料形态会储存键值的顺序等。- INI
- INI文件由节、键、值组成。比较简单，原生的 SHELL 难以解析
#### YAML 语法
YAML 使用可打印的 Unicode 字符，可使用 UTF-8 和 UTF-16 编码。
- - 使用空格 Space 缩进表示分层，不同层次之间的缩进可以使用不同的空格数目，但是同层元素一定左对齐，即前面空格数目相同（不能使用 Tab，各个系统 Tab对应的 Space 数目可能不同，导致层次混乱）
- ‘#’表示注释，只能单行注释，从#开始处到行尾
- 破折号后面跟一个空格（a dash and space）表示列表
- 用冒号和空格表示键值对 key: value
- 简单数据（scalars，标量数据）可以不使用引号括起来，包括字符串数据。用单引号或者双引号括起来的被当作字符串数据，在单引号或双引号中使用C风格的转义字符
#### **yaml安装：**
**pip install pyyaml**
yaml格式，使用缩进对格式进行要求，不允许使用tab，只能使用空格，来确定子项，同级，等等。
标签和值之间要有一个空格，要不然会当成一个字符串，不会当成是标签项。
```
userInfo:
   name: yhleng
   password: q123456
phone: 13718651887
```
#### 加载yaml数据   
```
path = os.path.join(sys.path[0],'test.yaml')
    f = open(path)
    x = yaml.load(f)
　　　　
    print x['userInfo']['name']
```
#### 返回字典对象：
```
{'phone': 13718651887L, 'userInfo': {'password': 'q123456', 'name': 'yhleng'}}
```
#### 输出：
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171103145512873-592134155.png)
