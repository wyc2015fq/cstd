
# python 列表推导式 - 机器学习的小学生 - CSDN博客


2017年06月25日 10:14:47[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：463



```python
#coding=gbk
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
import
```
```python
scipy.io
```
```python
as
```
```python
sio
```
```python
# loadmat
```
```python
mysolvers = list()
mysolvers0 = list()
niter =
```
```python
10
```
```python
######### 解析过程，由外向内解析##############
```
```python
############################################## 例子1 ######################################
```
```python
solvers = [(
```
```python
'pretrained'
```
```python
,mysolvers),(
```
```python
'scratch'
```
```python
,mysolvers0)]
blobs = (
```
```python
'loss'
```
```python
,
```
```python
'accuracy'
```
```python
)
loss,accuracy = ({name:np.zeros(niter)
```
```python
for
```
```python
name,_
```
```python
in
```
```python
solvers }
```
```python
for
```
```python
_
```
```python
in
```
```python
blobs)
```
```python
############## 第一层姐解析################
```
```python
# blobs 长度为2 ，遍历的时候将{}内的两个分别赋给 loss1和loss2
```
```python
loss = {name:np.zeros(niter)
```
```python
for
```
```python
name,_
```
```python
in
```
```python
solvers}
accuracy = {name:np.zeros(niter)
```
```python
for
```
```python
name,_
```
```python
in
```
```python
solvers}
```
```python
############## 第二层姐解析################
```
```python
# 分别对 loss1和loss2解析
```
```python
loss1 = {
```
```python
'loss'
```
```python
: np.zeros(niter),
```
```python
'accuracy'
```
```python
: np.zeros(niter)}
loss2 = {
```
```python
'loss'
```
```python
: np.zeros(niter),
```
```python
'accuracy'
```
```python
: np.zeros(niter)}
```
```python
############################################## 例子2 ######################################
```
```python
it =
```
```python
1
```
```python
# 迭代次数
```
```python
loss_disp =
```
```python
' ; '
```
```python
.join(
```
```python
'%s: loss = %.2f ,acc = %.2f'
```
```python
% (n, loss[n][it], accuracy[n][it])
```
```python
for
```
```python
n, _
```
```python
in
```
```python
solvers)
```
```python
# loss_disp 与  loss_disp_eq 是相同的
```
```python
loss_disp_eq =
```
```python
' ; '
```
```python
.join((
```
```python
'%s: loss = %.2f ,acc = %.2f'
```
```python
% (n, loss[n][it], accuracy[n][it]))
```
```python
for
```
```python
n, _
```
```python
in
```
```python
solvers)
```
```python
############## 第一层姐解析################
```
```python
loss_disp1 =
```
```python
'%s: loss = %.2f ,acc = %.2f'
```
```python
% (
```
```python
'pretrained'
```
```python
, loss[
```
```python
'pretrained'
```
```python
][it], accuracy[
```
```python
'pretrained'
```
```python
][it])
loss_disp2 =
```
```python
'%s: loss = %.2f ,acc = %.2f'
```
```python
% (
```
```python
'scratch'
```
```python
, loss[
```
```python
'scratch'
```
```python
][it], accuracy[
```
```python
'scratch'
```
```python
][it])
```
```python
############## 第二层姐解析################
```
```python
loss_disp_parse = loss_disp1 +
```
```python
' ; '
```
```python
+ loss_disp2
```
```python
######## 下面三种输出形式是等价的 ###############
```
```python
print
```
```python
loss_disp
```
```python
print
```
```python
loss_disp_eq
```
```python
print
```
```python
loss_disp_parse
```

