# python之raw_input和put之区别 - nosmatch的专栏 - CSDN博客
2011年08月17日 15:40:15[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：2234
使用input和raw_input都可以读取控制台的输入，但是input和raw_input在处理数字时是有区别的
## 1.输入字符串nID = ''while1:    nID = raw_input("Input your id plz")iflen(nID)!= len("13222319810101****"):print'wring length of id,input again'else:breakprint'your id is %s'%(nID)
## 2.输入整数
## nAge = int(raw_input("input your age plz:\n"))if nAge >0and nAge <120:print'thanks!'else:print'bad age'print'your age is %d\n'% nAge
## 3.输入浮点型
## fWeight = 0.0fWeight = float(raw_input("input your weight\n"))print'your weight is %f'% fWeight
## 4.输入16进制数据
## nHex = int(raw_input('input hex value(like 0x20):\n'),16)print'nHex = %x,nOct = %d\n'%(nHex,nHex)
## 5.输入8进制数据
## nOct = int(raw_input('input oct value(like 020):\n'),8)print'nOct = %o,nDec = %d\n'%(nOct,nOct)
```python
raw_put和put的区别：
```
```python
```
## 纯数字输入
当输入为纯数字时 
- 
input返回的是数值类型，如int,float
- 
raw_inpout返回的是字符串类型，string类型
## 输入字符串为表达式
input会计算在字符串中的数字表达式，而raw_input不会。 
如输入 “57 + 3”: 
- 
input会得到整数60
- 
raw_input会得到字符串”57 + 3”
## python input的实现
看[python input的文档](http://docs.python.org/library/functions.html?highlight=input#input)，可以看到input其实是通过raw_input来实现的，原理很简单，就下面一行代码： 
definput(prompt):
    return(eval(raw_input(prompt)))
