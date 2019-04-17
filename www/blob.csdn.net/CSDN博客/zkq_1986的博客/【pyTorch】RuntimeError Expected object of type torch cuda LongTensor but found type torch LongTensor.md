# 【pyTorch】RuntimeError: Expected object of type torch.cuda.LongTensor but found type torch.LongTensor - zkq_1986的博客 - CSDN博客





2018年11月14日 16:39:57[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：783








错误信息：RuntimeError: Expected object of type torch.cuda.LongTensor but found type torch.LongTensor for argument #3 'index'

解决方案：

将如下语句

prediction = model(text)

改成

prediction = model(text.cuda())



