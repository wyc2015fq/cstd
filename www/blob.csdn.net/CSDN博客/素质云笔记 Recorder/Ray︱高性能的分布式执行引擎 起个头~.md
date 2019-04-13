
# Ray︱高性能的分布式执行引擎 起个头~ - 素质云笔记-Recorder... - CSDN博客

2018年03月28日 17:32:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1408


Ray是一个神奇的平台，可以做分布式训练，由于最近还不需要分布式去做一些模型，所以先Mark一下，等到有需求的时候在，再学习，只能说Ray，异常强大！
文档：[http://ray.readthedocs.io/en/latest/index.html](http://ray.readthedocs.io/en/latest/index.html)
github:[https://github.com/ray-project/ray](https://github.com/ray-project/ray)
作者博客：[https://rise.cs.berkeley.edu/blog/pandas-on-ray/](https://rise.cs.berkeley.edu/blog/pandas-on-ray/)
**还能做啥：**
GPU训练、分布式训练，Using Ray with GPUs；
训练界面Web UI；
强化学习训练，有的模型：Proximal Policy Optimization (PPO)、The Asynchronous
Advantage Actor-Critic (A3C)、Deep Q Networks (DQN)、Ape-X Distributed
Prioritized Experience Replay等；
与tensorflow联合分布式训练；
Streaming MapReduce
…
一、Ray安装
来源：[Installing Ray](http://ray.readthedocs.io/en/latest/installation.html)
pipinstallray好像也有一些依赖需要安装：
sudo apt-getupdatesudo apt-getinstall -y cmake pkg-config build-essential autoconf curl libtool unzip flex bison python \# we install python here because python2isrequiredtobuild the webui
\#IfyouarenotusingAnaconda, you need the following.
sudo apt-getinstall python-dev  \#ForPython2.sudo apt-getinstall python3-dev  \#ForPython3.\#IfyouareonUbuntu14.04, you need the following.
pip install cmake
pip install cython二、Pandas on Ray简单介绍
在一篇机器之心发表的文章中[《资源 | Pandas on Ray：仅需改动一行代码，即可让Pandas加速四倍》](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650739820&idx=3&sn=cedea425dabca94b6a1258af847c31e2&chksm=871ad012b06d590474aa55c1e3667c665af3531b014b37c07d17c141fe419639e499d05cac3b&mpshare=1&scene=1&srcid=03242dHm5rOUJQL7KXsrzefA#rd)，感觉很给力，但是笔者自己尝试的时候，发现好像并木有描述的那么faster…可能笔者还找到正确的打开方式。。
而且调用、使用方式跟文章里面不一样…不过！ 确实是一行代码就可以替换了。
importray.dataframeasrdimportpandasaspdimportray直接导入ray之后，可以通过以下命令去设置调用CPU/GPU数量：
ray.init(num_cpus=20, num_gpus=2)笔者用了自己的一个数据集对比：
%timedataframe_ray = ray.dataframe.dataframe.pd.read_csv('data.csv')%timedataframe_rd = rd.dataframe.pd.read_csv('data.csv')%timedataframe_pd = pd.read_csv('data.csv')\# ray.dataframe>>> CPUtimes: user4.68s, sys:400ms, total:5.08s>>> Walltime:5.08s\# pandas>>> CPUtimes: user4.52s, sys:320ms, total:4.84s>>> Walltime:4.83s\# ray>>> CPUtimes: user5.31s, sys:468ms, total:5.78s>>> Walltime:5.78s其中直接用ray，尼玛中间引用好多：ray.dataframe.dataframe.pd.read_csv
来看看查询效率：
%timeresult= dataframe_ray[dataframe_ray.keyword=='word1']
%timeresult= dataframe_rd[dataframe_rd.keyword=='word1']
%timeresult= dataframe_pd[dataframe_pd.keyword=='word1']
>>> CPU times: user104ms, sys:12ms, total:116ms
>>> Walltime:113ms
>>> CPU times: user84ms, sys:28ms, total:112ms
>>> Walltime:111ms
>>> CPU times: user88ms, sys:4ms, total:92ms
>>> Walltime:92.1ms

