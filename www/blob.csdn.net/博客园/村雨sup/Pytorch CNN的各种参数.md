# Pytorch CNN的各种参数 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Model(torch.nn.Module):

    def __init__(self):
        super(Model, self).__init__()
        self.conv1 = torch.nn.Sequential(torch.nn.Conv2d(1, 4, kernel_size=5, stride=1,padding=0),
                                         torch.nn.MaxPool2d(stride=4, kernel_size=4))

        self.dense = torch.nn.Sequential(torch.nn.Linear(6 * 6 * 4, 33),
                                         torch.nn.ReLU(),
                                         torch.nn.Linear(33, 10))

    def forward(self, x):
        x = self.conv1(x)
        x = x.view(-1, 6 * 6 * 4)  #把卷积池化得到的向量平铺开
        x = self.dense(x)
        return x
```

conv ：卷积层(2d就是二维平面的)

　　kernel_size 卷积核大小

　　stride 每次移动的步长

　　padding 四周填充的大小，注意是四周所以在算下一层的向量维度时要将padding 乘以 2

　　maxpool2d 二维平面的池化层

dense：全链接层

　　Linear 一层神经元 输入维数 输出维数

　　激活函数

　　输出层，10分类



forward 函数里的view(-1,n)就是把卷积池化得到的2维向量展开成1维的，便于传入全连接层













