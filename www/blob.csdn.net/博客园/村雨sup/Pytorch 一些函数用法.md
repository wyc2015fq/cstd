# Pytorch 一些函数用法 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





PyTorch中view的用法:https://blog.csdn.net/york1996/article/details/81949843

max用法

```
import torch

d=torch.Tensor([[1,3],[2,4]])
print(d)
```

tensor([[1., 3.],
        [2., 4.]])

```
import torch

d=torch.Tensor([[1,3],[2,4]])
print(torch.max(d,0))
print(torch.max(d,1))
```

(tensor([2., 4.]), tensor([1, 1]))
(tensor([3., 4.]), tensor([1, 1]))

torch.max()返回两个结果，第一个是最大值，第二个是对应的索引值；第二个参数 0 代表按列取最大值并返回对应的行索引值，1 代表按行取最大值并返回对应的列索引值。











