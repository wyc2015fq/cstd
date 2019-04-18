# String类 - YZXnuaa的博客 - CSDN博客
2018年06月12日 23:18:58[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：63

自有的常用属性，可用于正则化
![](https://img-blog.csdn.net/20180612113029649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1laWG51YWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
字符串大小写变换
capwords(s, sep=None):
    """capwords(s [,sep]) -> string
测试：
from string import capwordsprint(capwords('aFe'))
结果：Afe
Template模块：
源代码表示要先输入模板template
class Template(metaclass=_TemplateMetaclass):
    """A string class for supporting $-substitutions."""
delimiter = '$'
idpattern = r'[_a-z][_a-z0-9]*'
flags = _re.IGNORECASE
    def __init__(self, template):
        self.template = template案例1：substitude方法
from string import Template
s = Template('There  ${moneyType} is  ${money}')
print(s.substitute(moneyType='Dollar', money=12))
结果：There  Dollar is  12
案例2：安全替代，只改变其中一个，但是变成了string类型，没什么用
b = Template(s.safe_substitute(moneyType='Yuan'))
print(b.substitute(money=100))
结果：There  Yuan is  100
