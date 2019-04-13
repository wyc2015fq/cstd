
# python sys.path.append()和sys.path.insert() - 机器学习的小学生 - CSDN博客


2018年09月04日 21:04:18[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：84个人分类：[Python																](https://blog.csdn.net/xuluhui123/article/category/6679259)


转载自：[https://blog.csdn.net/dcrmg/article/details/79546962](https://blog.csdn.net/dcrmg/article/details/79546962)
python程序中使用 import XXX 时，python解析器会在当前目录、已安装和第三方模块中搜索 xxx，如果都搜索不到就会报错。
使用sys.path.append()方法可以临时添加搜索路径，方便更简洁的import其他包和模块。这种方法导入的路径会在python程序退出后失效。
## 1. 加入上层目录和绝对路径
```python
import
```
```python
sys
sys.path.append(
```
```python
'..'
```
```python
)
```
```python
#表示导入当前文件的上层目录到搜索路径中
```
```python
sys.path.append(
```
```python
'/home/model'
```
```python
)
```
```python
# 绝对路径
```
```python
from
```
```python
folderA.folderB.fileA
```
```python
import
```
```python
functionA
```
## 2. 加入当前目录
```python
import
```
```python
os,sys
sys.path.append(os.getcwd())
os.getcwd()用于获取当前工作目录
```
## 3. 定义搜索优先顺序
```python
import
```
```python
sys
sys.path.insert(
```
```python
1
```
```python
,
```
```python
"./model"
```
```python
)
sys.path.insert(
```
```python
1
```
```python
,
```
```python
"./crnn"
```
```python
)定义搜索路径的优先顺序，序号从
```
```python
0
```
```python
开始，表示最大优先级，sys.path.insert()加入的也是临时搜索路径，程序退出后失效。
```

