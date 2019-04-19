# Python入门学习之operator--操作符函数 - 数据之美的博客 - CSDN博客
2017年05月06日 20:52:58[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：242
|操作|语法|函数|
|----|----|----|
|相加|a + b|add(a, b)|
|字符串拼接|seq1 + seq2|concat(seq1, seq2)|
|包含[测试](http://lib.csdn.net/base/softwaretest)|obj in seq|contains(seq, obj)|
|普通除法|a / b|truediv(a, b)|
|取整除法|a // b|floordiv(a, b)|
|按位与|a & b|and_(a, b)|
|按位异或|a ^ b|xor(a, b)|
|按位取反|~ a|invert(a)|
|按位或|a | b|or_(a, b)|
|指数运算|a ** b|pow(a, b)|
|识别|a is b|is_(a, b)|
|识别|a is not b|is_not(a, b)|
|索引赋值|obj[k] = v|setitem(obj, k, v)|
|索引删除|del obj[k]|delitem(obj, k)|
|索引|obj[k]|getitem(obj, k)|
|左移|a << b|lshift(a, b)|
|取模|a % b|mod(a, b)|
|乘法|a * b|mul(a, b)|
|负数|-a|neg(a)|
|非运算|not a|not_(a)|
|正数|+ a|pos(a)|
|右移运算|a >> b|rshift(a, b)|
|切片赋值|seq[i:j] = values|setitem(seq, slice(i, j), values)|
|切片删除|del seq[i:j]|delitem(seq, slice(i, j))|
|切片|seq[i: j]|getitem(seq, slice(i, j))|
|字符串格式化|s % obj|mod(s, obj)|
|减法|a - b|sub(a, b)|
|真值测试|obj|truth(obj)|
|小于|a < b|lt(a, b)|
|小于等于|a <= b|le(a, b)|
|等于|a == b|eq(a, b)|
|不等于|a != b|ne(a, b)|
|大于等于|a >= b|ge(a, b)|
|大于|a > b|gt(a, b)|
举例：
#[Python](http://lib.csdn.net/base/python) 3.5
import operator
a = operator.add(1, 3)
print(a)
结果输出如下：
4
