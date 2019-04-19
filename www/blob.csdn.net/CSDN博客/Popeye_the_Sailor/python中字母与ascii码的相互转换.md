# python中字母与ascii码的相互转换 - Popeye_the_Sailor - CSDN博客
2018年06月18日 21:28:54[_Sailor_](https://me.csdn.net/lz0499)阅读数：107
### python中字母与ascii码的相互转换
在做python编程时，碰到了需要将字母转换成ascii码的，原本以为用Int()就可以直接将字符串转换成整形了，可是int()带了一个默认参数，base=10，这里表示的是十进制，若出现字母，则会报错，认为超出该进制的表示范围。
通过查阅网络和python的帮助文档，明确了几个函数的使用，记录如下：
ord(c):参数是长度为1的字符串，简称字符。当参数为统一对象时（unicode object），返回能代表该字符的统一编码，当参数为8比特的字符串时，返回该字节的值。例如，ord('a')返回整形数值97，ord(u'\u2020')返回8224。
chr(i)：返回一个字符，字符的ascii码等于参数中的整形数值。例如chr(97)返回字符'a'，该方法是ord()的反方法。参数必须是0-255的整形数值，否则会抛出valueError错误。
在应用函数时，观察到有个python函数比较高效，map(function,iterable,...)将函数应用于每一项iterable，并且返回一个结果的列表。**map()是 Python 内置的高阶函数，它接收一个函数 f 和一个 list，并通过把函数 f 依次作用在 list 的每个元素上，得到一个新的 list 并返回。**
注意：map()函数不改变原有的 list，而是返回一个新的 list。
利用map()函数，可以把一个 list 转换为另一个 list，只需要传入转换函数。
由于list包含的元素可以是任何类型，因此，map() 不仅仅可以处理只包含数值的 list，事实上它可以处理包含任意类型的 list，只要传入的函数f可以处理这种数据类型。
转载于：[python中字母与ascii码的相互转换](https://www.cnblogs.com/jingmu/p/7305040.html)
