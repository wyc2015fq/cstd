# 向后传播算法（Back propagation algorithm） - weixin_33985507的博客 - CSDN博客
2017年09月27日 19:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
一样的思想，我们的目标是把代价函数最小化：
![7920211-71bce46ee2280f16.png](https://upload-images.jianshu.io/upload_images/7920211-71bce46ee2280f16.png)
向前传播：
![7920211-dfaeb0d419d04550.png](https://upload-images.jianshu.io/upload_images/7920211-dfaeb0d419d04550.png)
向后传播：
![7920211-d9b7dafd475a9eb5.png](https://upload-images.jianshu.io/upload_images/7920211-d9b7dafd475a9eb5.png)
δ(l)j：第l层第j个节点的误差。
向后传播算法实现步骤：
![7920211-cedee560c4fcca5d.png](https://upload-images.jianshu.io/upload_images/7920211-cedee560c4fcca5d.png)
