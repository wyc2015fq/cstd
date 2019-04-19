# python 单双下划线_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
Python 用下划线作为变量前缀和后缀指定特殊变量/方法。
主要存在四种情形
- object #
public
- 
__object__ # special, python system use, user should not define
like it
- __object
# private (name mangling during runtime)
- _object # obey python coding convention, consider
it as private
核心风格：避免用下划线作为变量名的开始。
因为下划线对解释器有特殊的意义，而且是内建标识符所使用的符号，我们建议程序员避免用下划线作为变量名的开始。一般来讲，变量名_object被看作是“私有
的”，在模块或类外不可以使用，不能用'from moduleimport
*'导入。当变量是私有的时候，用_object来表示变量是很好的习惯。因为变量名__object__对Python
来说有特殊含义，对于普通的变量应当避免这种命名风格。
python有关private的描述，python中不存在protected的概念，要么是public要么就是private，但是python中的private不像C++,
Java那样，它并不是真正意义上的private，通过name
mangling（名称改编(目的就是以防子类意外重写基类的方法或者属性)，即前面加上“单下划线”+类名,eg：_Class__object）机制就可以访问private了。
"单下划线" 开始的成员变量叫做保护变量，意思是只有类对象和子类对象自己能访问到这些变量；"双下划线"
开始的是私有成员，意思是只有类对象自己能访问，连子类对象也不能访问到这个数据。(如下列所示)
以单下划线开头（_foo）的代表不能直接访问的类属性，需通过类提供的接口进行访问，不能用“from xxx import
*”而导入；以双下划线开头的（__foo）代表类的私有成员；以双下划线开头和结尾的（__foo__）代表python里特殊方法专用的标识，如
__init__（）代表类的构造函数。
