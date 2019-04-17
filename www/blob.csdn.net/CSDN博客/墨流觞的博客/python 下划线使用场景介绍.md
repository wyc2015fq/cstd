# python 下划线使用场景介绍 - 墨流觞的博客 - CSDN博客





2018年09月14日 14:31:37[墨氲](https://me.csdn.net/dss_dssssd)阅读数：40标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python 基础](https://blog.csdn.net/dss_dssssd/article/category/6947829)








- 
前导单下划线`_var`

pep 8, 约定，  
**使用场景**： 仅限内部使用， 比如只在类内部使用的函数，变量 

注意:通配符导入 ：from module import *是不会到导入带单前导下划线的对象的- 
单末尾下划线 `var_`

**使用场景**： 某个变量的最合适的名称已被关键字占用，而你还想用，可以附加一个下划线解决命名冲突 

比如： `class_` ,`id_`

- 
双前导下划线`__var`

python解释器会重写属性名称，避免子类中的命名冲突，重写为： `_classname__var`. 简单的说，就像c++中的私有变量，对子类和外部是完全透明的，不可引用 
**使用场景**：一般某个对象很重要，不想被子类或外部直接引用

- 
双前导和双末尾下划线  `__var__`

python保留作为特殊用途，`__init___`,  `__call__`等， **不建议在自己的代码中使用**

- 
单下划线   `_`

**使用场景**: 标识某个临时或无关紧要的变量， 常在unpacking表达式中使用

比如：age不重要

```python
name_age_id = ('醉陌', '20', '21315271927')
name, _, id_ = name_age_id
```


[更全面的介绍文章](https://blog.csdn.net/tcx1992/article/details/80105645)





