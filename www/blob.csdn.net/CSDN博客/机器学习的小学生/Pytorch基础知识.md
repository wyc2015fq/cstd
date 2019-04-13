
# Pytorch基础知识 - 机器学习的小学生 - CSDN博客


2018年04月29日 09:00:08[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：100



### 1. 保存和加载整个模型
(1) 保存和加载整个模型
```python
torch.save(model_object,
```
```python
'model.pkl'
```
```python
)
model = torch.
```
```python
load
```
```python
(
```
```python
'model.pkl'
```
```python
)
```
(2) 仅保存和加载模型参数(推荐使用)
```python
torch.save(model_object.state_dict(),
```
```python
'params.pkl'
```
```python
)
model_object.load_state_dict(torch.
```
```python
load
```
```python
(
```
```python
'params.pkl'
```
```python
))
```

