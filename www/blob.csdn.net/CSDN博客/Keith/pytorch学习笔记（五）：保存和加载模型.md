# pytorch学习笔记（五）：保存和加载模型 - Keith - CSDN博客





2017年04月01日 23:21:08[ke1th](https://me.csdn.net/u012436149)阅读数：48132
个人分类：[pytorch](https://blog.csdn.net/u012436149/article/category/6689265)

所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









```python
# 保存和加载整个模型
torch.save(model_object, 'model.pkl')
model = torch.load('model.pkl')
```

```python
# 仅保存和加载模型参数(推荐使用)
torch.save(model_object.state_dict(), 'params.pkl')
model_object.load_state_dict(torch.load('params.pkl'))
```



