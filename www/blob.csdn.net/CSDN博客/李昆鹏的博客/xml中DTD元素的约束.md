
# xml中DTD元素的约束 - 李昆鹏的博客 - CSDN博客


2018年07月02日 10:56:13[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：59


---------------------------------------xml中DTD元素的约束---------------------------------------
## DTD定义元素

### 1　定义元素的语法
使用ELEMENT声明元素：<!ELEMENT 元素名**内容类型**或**内容**>
例如：<!ELEMENTname (\#PCDATA)>
表示name元素的内容为文本数据
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (\#PCDATA)>
|]>
|<|students|>
|李昆鹏
|</|students|>
|

### 2　元素类型
元素类型可以是ANY或EMPTY
<!ELEMENTstu ANY>：表示stu元素的内容可以是任意元素，也可以是文本数据，也可以是文本数据+子元素，反正就是任意。
<!ELEMENTstu EMPTY>：表示stu不能有任何内容，即空元素。例如：<stu/>。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student+)>
|<!ELEMENT  student (name,age)>
|<!ELEMENT name  ANY>
|<!ELEMENT age  EMPTY>
|]>
|<|students|>
|<|student|>
|<|name|>|李昆鹏123|</|name|>
|<|age|></|age|>
|</|student|>
|</|students|>
|

### 3　元素内容
元素内容可以是文本数据，也可以是子元素
<!ELEMENTstu (\#PCDATA)>表示stu元素内容为文本，例如：<stu>hello</stu>
<!ELEMENT stu (name)>表示stu元素内容为name子元素，例如<stu><name></name><stu>，但要注意，如果<name>元素没有声明，那么就会出错。
代码示例：
|<?xml version="1.0" encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student+)>
|<!ELEMENT  student (name,age,gender)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ELEMENT  gender (\#PCDATA)>
|]>
|<|students|>
|<|student|>
|<|name|>|张三|</|name|>
|<|age|>|20|</|age|>
|<|gender|>|男|</|gender|>
|</|student|>
|</|students|>
|

### 4　子元素出现次数
可以使用“?”、“*”、“+”来指定子元素的出现次数
<!ELEMENTstu (name?)>表示stu元素可以有0~1个name子元素，即name子元素可有可无。
<!ELEMENTstu(name*)>表示stu元素可以有0~n个name子元素；
<!ELEMENTstu(name+)>表示stu元素可以有1~n个name子元素。

### 5　多个子元素
<!ELEMENTstu (name,age,sex)>表示stu必须有三个子元素，分别是name、age、sex，并且子元素出现的顺序也要与声明的顺序一致。
### 6　枚举子元素
<!ELEMENT stu (name | age | sex)表示stu只有一个子元素，可以是name、age、sex中的任意一个。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student+)>
|<!ELEMENT  student (name|age|gender)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ELEMENT  gender (\#PCDATA)>
|]>
|<|students|>
|<|student|>
|<|name|>|张三|</|name|>
|</|student|>
|</|students|>
|

### 7　复合声明1
<!ELEMENT stu(name | age | sex)?>表示stu元素可以有0~1个(name | age | sex)，而(name | age | sex)表示name、age、sex其中的一个。
<stu></stu>
<stu><name/></stu>
<stu><age/></stu>
<stu><sex/></stu>

### 8　复合声明2
<!ELEMENT stu (name | age | sex)*>表示stu元素可以有0~n个(name |age | sex)，而(name | age | sex)表示name、age、sex其中的一个。
<stu></stu>
<stu><name/><name/></stu>
<stu><name/><age/><age/></stu>
<stu><name/><age/><name/><sex/><sex/></stu>

### 9　复合声明3
<!ELEMENT stu(name | age | sex)+>表示stu元素可以有1~n个(name | age | sex)，而(name | age | sex)表示name、age、sex其中的一个。
<stu><age/></stu>
<stu><name/><name/></stu>
<stu><name/><age/><age/></stu>
<stu><name/><age/><name/><sex/><sex/></stu>

### 10　练习
|<!DOCTYPE 课件 [
|<!ELEMENT 章节 (简介, (标题, (段落 | 图片 | 表格 | 说明)+)+)>
|<!ELEMENT 简介 (\#PCDATA)>
|<!ELEMENT 标题 (\#PCDATA)>
|<!ELEMENT 段落 (\#PCDATA)>
|<!ELEMENT 图片 (\#PCDATA)>
|<!ELEMENT 表格 (\#PCDATA)>
|<!ELEMENT 说明 (\#PCDATA)>
|<!ELEMENT 课件 (章节+)>
|]>
|
根据上面DTD写xml文档！！！
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE|课件 [
|<!ELEMENT|章节 (|简介, (标题, (段落 | 图片 | 表格 | 说明)+)+)>
|<!ELEMENT|简介 (\#PCDATA)>
|<!ELEMENT|标题 (\#PCDATA)>
|<!ELEMENT|段落 (\#PCDATA)>
|<!ELEMENT|图片 (\#PCDATA)>
|<!ELEMENT|表格 (\#PCDATA)>
|<!ELEMENT|说明 (\#PCDATA)>
|<!ELEMENT|课件 (|章节+)>
|]>
|<|课件|>
|<|章节|>
|<|简介|>|水浒传是一部农民起义的故事|</|简介|>
|<|标题|>|水浒传第三回景阳冈|</|标题|>
|<|段落|>|武松来到景阳冈。。。|</|段落|>
|<|说明|>|作者：施耐庵|</|说明|>
|</|章节|>
|<|章节|>
|<|简介|>|水浒传是一部农民起义的故事|</|简介|>
|<|标题|>|宋三郎怒杀阎婆惜|</|标题|>
|<|段落|>|那日。。。|</|段落|>
|<|表格|></|表格|>
|</|章节|>
|</|课件|>
|


