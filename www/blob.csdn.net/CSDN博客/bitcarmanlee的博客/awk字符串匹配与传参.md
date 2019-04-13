
# awk字符串匹配与传参 - bitcarmanlee的博客 - CSDN博客


2017年11月22日 21:43:06[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1491



## 1.模式匹配
有名为00的文件，里面的具体内容如下：
```python
123
```
```python
abc,
```
```python
100196
```
```python
,
```
```python
an
```
```python
123
```
```python
abc,
```
```python
100196
```
```python
,
```
```python
an
```
```python
113
```
```python
abc,
```
```python
100196
```
```python
,
```
```python
an
```
```python
133
```
```python
abc,
```
```python
100196
```
```python
,
```
```python
an
```
想要对第二个字段做模式匹配，匹配里面是否有100196这个字符串。
最开始博主用了如下方式：
```python
#!/bin/bash
```
```python
app=
```
```python
$1
```
```python
awk -F
```
```python
"\t"
```
```python
'$2~/app/ {a[$1]++} END{print length(a)}'
```
```python
app=
```
```python
$app
```
```python
00
```
执行该脚本，传入参数：
```python
./test
```
```python
.sh
```
```python
100196
```
发现并不能得到想要的结果！最后经过很长时间的尝试，将上面的代码改为如下：
```python
#!/bin/bash
```
```python
app=
```
```python
$1
```
```python
awk -F
```
```python
"\t"
```
```python
'$2~app {a[$1]++} END{print length(a)}'
```
```python
app=
```
```python
$app
```
```python
00
```
即可得到预期的结果！在这部分代码中，～就是表示用来匹配后面的正则表达式，告诉awk后面开始是正则语法。
## 2.用match方法
当然，也可以使用match方法，达到上面同样的效果：
```python
awk
```
```python
-F
```
```python
"\t"
```
```python
'{if(match(
```
```python
$2
```
```python
, app)>0) a[
```
```python
$1
```
```python
]++} END{print length(a)}'
```
```python
app=
```
```python
$app
```
```python
00
```
## 3.两种传参方式的不同
上面的代码采用了传参的方式。在awk中，有两种传参方式，分别如下：
```python
➜  shell
```
```python
#!/bin/bash
```
```python
app=
```
```python
$1
```
```python
awk -F
```
```python
"\t"
```
```python
'BEGIN{print app} {print match($2, app)}'
```
```python
app=
```
```python
$app
```
```python
00
```
```python
awk -F
```
```python
"\t"
```
```python
-v app=
```
```python
$app
```
```python
'BEGIN{print app} {print match($2, app)}'
```
```python
00
```
两种方式的区别是什么呢？看看代码运行的结果：
```python
5
5
5
5
100196
5
5
5
5
```
两者的区别在于：用-v的方式传参，参数在BEGIN模块中就起了作用，而在另外一种传参方式中，参数在BEGIN模块中并没有起作用！

