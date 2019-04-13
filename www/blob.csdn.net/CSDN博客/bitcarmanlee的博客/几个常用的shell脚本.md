
# 几个常用的shell脚本 - bitcarmanlee的博客 - CSDN博客


2017年05月21日 22:41:45[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：347标签：[shell																](https://so.csdn.net/so/search/s.do?q=shell&t=blog)[脚本																](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)[awk																](https://so.csdn.net/so/search/s.do?q=awk&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=shell&t=blog)个人分类：[linux shell																](https://blog.csdn.net/bitcarmanlee/article/category/6148977)
[
																								](https://so.csdn.net/so/search/s.do?q=shell&t=blog)



## 1.选择文件大小大于某个值的文件
有一个文件夹里有若干文件，打印文件大小大于300B的文件名。
```python
#!/bin/bash
```
```python
for
```
```python
file
```
```python
in
```
```python
`ls
```
```python
-l
```
```python
| awk
```
```python
'$5>300 {print $9}'
```
```python
`
```
```python
do
```
```python
echo
```
```python
$file
```
```python
done
```
## 2.某个文件夹中找到.py结尾的文件取10个，并取所有文件的第一行
```python
#!/bin/bash
```
```python
for
```
```python
file
```
```python
in
```
```python
`find dir -type f -name
```
```python
"*.py"
```
```python
| head -n
```
```python
10
```
```python
`
```
```python
do
```
```python
sed -n
```
```python
'1p'
```
```python
$file
```
```python
>> result_file
```
```python
done
```
稍微接一下：dir表示要查找的路径，-type表示查找的是file，-name表示名称。sed -n ‘1p’表示取第一行。
## 3.打印fileB中有fileA中没有的行
方法1:用awk的方法
```python
awk
```
```python
'NR==FNR{a[
```
```python
$0
```
```python
]++} NR>FNR{if(!(
```
```python
$0
```
```python
in a)) print
```
```python
$0
```
```python
}'
```
```python
file1 file2
```
方法2:使用grep
```python
grep
```
```python
-vxFf file1 file2
```
grep选取-v表示不选择匹配的行，-x表示只打印整行匹配的行，-F表示匹配的模式按行分割，-f a表示匹配模式来自文件file1，最后表示目标文件file2

