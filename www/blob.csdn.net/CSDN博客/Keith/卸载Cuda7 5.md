# 卸载Cuda7.5 - Keith - CSDN博客





2016年11月14日 19:51:40[ke1th](https://me.csdn.net/u012436149)阅读数：17893








由于需要安装cuda8.0,,所以需要卸载cuda7.5. 
**如果是使用.run 文件安装的，使用下面方法卸载**

在`/usr/local/cuda/bin` 目录下,有`cuda` 自带的卸载工具`uninstall_cuda_7.5.pl`
```python
cd /usr/local/cuda/bin
sudo ./uninstall_cuda_7.5.pl
```

**如果是使用 .deb 安装的，使用下面方法卸载**

```python
sudo apt-get --purge remove <package_name>
```

over! 

然后去官网下载cuda8.0安装就可以了




