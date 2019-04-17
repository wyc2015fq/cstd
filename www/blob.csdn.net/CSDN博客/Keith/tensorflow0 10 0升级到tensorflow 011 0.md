# tensorflow0.10.0升级到tensorflow.011.0 - Keith - CSDN博客





2016年11月14日 23:42:32[ke1th](https://me.csdn.net/u012436149)阅读数：7655








# tensorflow0.10.0升级到tensorflow0.11.0

tensorflow0.11.0已经可以安装了.下面总结一下安装步骤:

(1). 卸载`tensorflow0.10.0`

```python
sudo pip uninstall tensorflow
```

(2). 卸载`cuda7.5`,`cuda8.0`不需要卸载

```python
cd /usr/local/cuda/bin
sudo ./uninstall_cuda_7.5.pl
#手动清除cuda-7.5文件夹
cd /usr/local/
sudo rm -rf cuda-7.5
#删除cuda7.5sample
cd ~
sudo rm -rf NVIDIA_CUDA-7.5_Samples
```

(3). 安装`cuda8.0`, 已有的不需要安装 

官网下载`cuda8.0`,[网速慢的话](http://www.cnblogs.com/kingstrong/p/5959664.html)

```python
chmod +x cuda_8.0.44_linux.run
sudo ./cuda_8.0.44_linux.run
#除了安装显卡驱动的地方选no,其他地方默认就好
```

(4). 安装`cudnn5.1`

官网下载,解压

```python
sudo cp cuda/include/cudnn.h /usr/local/cuda/include/    
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/    
sudo chmod a+r /usr/local/cuda/include/cudnn.h    
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```

(5). 修改`.bashrc`文件 

将以前的`7.5`改成`8.0`,执行`source .bashrc`

(6). 安装`tensorflow0.11.0`
```python
sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.11.0-cp27-none-linux_x86_64.whl
```

Over!




