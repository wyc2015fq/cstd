# python cookbook 2.9 将Unicode文本标准化 - 别说话写代码的博客 - CSDN博客





2019年04月12日 09:59:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#你正在处理Unicode字符串，需要确保所有字符串在底层有相同的表示
#在Unicode中，某些字符能够用多个合法的编码表示。为了说明，考虑下面这个例子：
s1='Spicy Jalape\u00f1o'  #U+00f1
s2='Spicy Jalapen\u0303o'   #n后面加~，也就是U+0303字母组合
print(s1)
print(s2)
print(s1==s2)
print(len(s1),len(s2))
```

![](https://img-blog.csdnimg.cn/20190412095832589.png)

```python
#在需要比较字符串的程序中使用字符的多种表示会出现问题。为修正这个问题，可以使用unicodedata模块先将
#文本标准化
import unicodedata
t1=unicodedata.normalize('NFC',s1)  #normalize第一个参数是制定字符串标准化方式
t2=unicodedata.normalize('NFC',s2)  #NFC表示字符串应该是整体组成（可能的话使用单一编码）
print(t1==t2)
print(ascii(t1))

t3=unicodedata.normalize('NFD',s1)  #NFD表示字符应该分解为多个组合字符表示
t4=unicodedata.normalize('NFD',s2)
print(t1==t2)
print(ascii(t3))
```

![](https://img-blog.csdnimg.cn/201904120958538.png)

```python
#python同样支持扩展的标准化形式NFKC和NFKC，他们在处理某些字符时候增加了额外的兼容特性
s='\ufb01'
print(s)
print(unicodedata.normalize('NFD',s))
print(unicodedata.normalize('NFKD',s))
print(unicodedata.normalize('NFKC',s))
```

![](https://img-blog.csdnimg.cn/20190412095911361.png)

```
#标准化对任何需要以一致性方法处理unicode文本的程序都非常重要。当处理来自用户输入字符串而你很难控制编码时候尤其如此
#在清理和过滤文本的时候字符的标准化很重要。比如，加入想清除掉一些文本上面变音符时（可能为了搜索和匹配）
t1=unicodedata.normalize('NFD',s1)
print(''.join(c for c in t1 if not unicodedata.combining(c)))
```

![](https://img-blog.csdnimg.cn/20190412095927309.png)

conbining()函数可以测试一个字符是否为和音字符,这个模块还有其他函数用于查找字符类别，测试是否为数字字符等等



