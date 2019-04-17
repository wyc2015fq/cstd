# Anaconda 安装第三方包(以jieba工具为例) - miner_zhu的博客 - CSDN博客





2018年08月10日 18:03:44[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：2633












1.直接在cmd窗口运行
`pip install jieba`
2.使用conda自带的安装工具
`conda install jieba`
3.有一些模块是无法使用以上两种方式安装上，这时就需要首先寻找模块，再安装
`anaconda search -t conda jieba`
这时会出现该模块的很多版本的信息如下图，找到合适的版本

![](https://img-blog.csdn.net/20180804133438843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E4NTc1NTMzMTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

根据版本的信息，输入
`anaconda show conda-forge/jieba`
这时会出现该版本的信息如下图，再执行最后一行的信息即可安装

![](https://img-blog.csdn.net/20180804133726628?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E4NTc1NTMzMTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`conda install --channel https://conda.anaconda.org/conda-forge jieba`


参考地址：[https://blog.csdn.net/a857553315/article/details/81410142](https://blog.csdn.net/a857553315/article/details/81410142)



