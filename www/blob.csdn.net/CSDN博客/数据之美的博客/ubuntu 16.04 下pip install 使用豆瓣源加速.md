# ubuntu 16.04 下pip install 使用豆瓣源加速 - 数据之美的博客 - CSDN博客
2017年12月18日 10:48:10[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：176
安装caffe2 的环境时，pip install protobuf 一直显示time out，一开始以为是系统其他变量有问题，结果发现其实是因为使用国外的源太慢了，国内最推荐的就是豆瓣源，其地址是 http://pypi.douban.com/simple/。
具体使用命令为： sudo pip install -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com xxxx
你会发现下载速度特别快。
