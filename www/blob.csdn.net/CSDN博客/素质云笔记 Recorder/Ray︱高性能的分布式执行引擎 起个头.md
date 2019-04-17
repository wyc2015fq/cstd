# Ray︱高性能的分布式执行引擎 起个头~ - 素质云笔记/Recorder... - CSDN博客





2018年03月28日 17:32:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1441








Ray是一个神奇的平台，可以做分布式训练，由于最近还不需要分布式去做一些模型，所以先Mark一下，等到有需求的时候在，再学习，只能说Ray，异常强大！
- 文档：[http://ray.readthedocs.io/en/latest/index.html](http://ray.readthedocs.io/en/latest/index.html)
- github:[https://github.com/ray-project/ray](https://github.com/ray-project/ray)
- 作者博客：[https://rise.cs.berkeley.edu/blog/pandas-on-ray/](https://rise.cs.berkeley.edu/blog/pandas-on-ray/)

**还能做啥：**
- GPU训练、分布式训练，Using Ray with GPUs；
- 训练界面Web UI；
- 强化学习训练，有的模型：Proximal Policy Optimization (PPO)、The Asynchronous 

Advantage Actor-Critic (A3C)、Deep Q Networks (DQN)、Ape-X Distributed 

Prioritized Experience Replay等；- 与tensorflow联合分布式训练；
- Streaming MapReduce
- …

## 一、Ray安装

来源：[Installing Ray](http://ray.readthedocs.io/en/latest/installation.html)
`pip install ray`
好像也有一些依赖需要安装：

```
sudo apt-get update
sudo apt-get install -y cmake pkg-config build-essential autoconf curl libtool unzip flex bison python # we install python here because python2 is required to build the webui

# If you are not using Anaconda, you need the following.
sudo apt-get install python-dev  # For Python 2.
sudo apt-get install python3-dev  # For Python 3.

# If you are on Ubuntu 14.04, you need the following.
pip install cmake

pip install cython
```

## 二、Pandas on Ray简单介绍

在一篇机器之心发表的文章中[《资源 | Pandas on Ray：仅需改动一行代码，即可让Pandas加速四倍》](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650739820&idx=3&sn=cedea425dabca94b6a1258af847c31e2&chksm=871ad012b06d590474aa55c1e3667c665af3531b014b37c07d17c141fe419639e499d05cac3b&mpshare=1&scene=1&srcid=03242dHm5rOUJQL7KXsrzefA#rd)，感觉很给力，但是笔者自己尝试的时候，发现好像并木有描述的那么faster…可能笔者还找到正确的打开方式。。 

而且调用、使用方式跟文章里面不一样…不过！ 确实是一行代码就可以替换了。

```
import ray.dataframe as rd
import pandas as pd
import ray
```

直接导入ray之后，可以通过以下命令去设置调用CPU/GPU数量：
`ray.init(num_cpus=20, num_gpus=2)`
笔者用了自己的一个数据集对比：

```
%time dataframe_ray = ray.dataframe.dataframe.pd.read_csv('data.csv')
%time dataframe_rd = rd.dataframe.pd.read_csv('data.csv')
%time dataframe_pd = pd.read_csv('data.csv')

# ray.dataframe
>>> CPU times: user 4.68 s, sys: 400 ms, total: 5.08 s
>>> Wall time: 5.08 s
# pandas 
>>> CPU times: user 4.52 s, sys: 320 ms, total: 4.84 s
>>> Wall time: 4.83 s
# ray
>>> CPU times: user 5.31 s, sys: 468 ms, total: 5.78 s
>>> Wall time: 5.78 s
```

其中直接用ray，尼玛中间引用好多：`ray.dataframe.dataframe.pd.read_csv`

来看看查询效率：

```
%time result = dataframe_ray[dataframe_ray.keyword=='word1']
%time result = dataframe_rd[dataframe_rd.keyword=='word1']
%time result = dataframe_pd[dataframe_pd.keyword=='word1']

>>> CPU times: user 104 ms, sys: 12 ms, total: 116 ms
>>> Wall time: 113 ms
>>> CPU times: user 84 ms, sys: 28 ms, total: 112 ms
>>> Wall time: 111 ms
>>> CPU times: user 88 ms, sys: 4 ms, total: 92 ms
>>> Wall time: 92.1 ms
```




