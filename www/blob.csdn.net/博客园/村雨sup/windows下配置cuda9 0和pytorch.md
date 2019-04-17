# windows下配置cuda9.0和pytorch - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





今天看了看pytorch官网竟然支持windows了，赶紧搞一个。

下载cuda 9.0  https://developer.nvidia.com/cuda-downloads

下载anaconda3  https://www.anaconda.com/download/

（记得安装在默认位置，不然的话windows下会发生很多错误）

pytorch官网查conda安装pytorch的命令

conda install pytorch -c pytorch 
 pip3 install torchvision  （或者pip）

在conda中敲入python

>>import torch 

>>torch.cuda.is_available

True

表示安装成功了

感觉比ubuntu简单啊，我的虚拟机可以删了.....











