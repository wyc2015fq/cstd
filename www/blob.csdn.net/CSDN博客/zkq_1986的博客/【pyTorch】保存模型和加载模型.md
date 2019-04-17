# 【pyTorch】保存模型和加载模型 - zkq_1986的博客 - CSDN博客





2018年11月13日 15:13:41[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：68








```python
import torch
save_model_path='model.pkl'
#保存模型
torch.save(model, save_model_path)
#加载模型
model = torch.load(save_model_path)
```





