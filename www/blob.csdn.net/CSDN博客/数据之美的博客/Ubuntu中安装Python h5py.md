# Ubuntu中安装Python h5py - 数据之美的博客 - CSDN博客
2017年02月27日 18:52:33[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1439
直接利用 sudo pip install h5py 首先出现没有python；安装完python后会提示一个g++错误，这是由于没有安装hdf5；安装完hdf5再安装h5py就能够成功安装。
安装h5py的命令如下：
sudo pip install python
sudo apt-get install libhdf5-dev
sudo pip install h5py
安装完成后可以用如下命令[测试](http://lib.csdn.net/base/softwaretest)：
[Python](http://lib.csdn.net/base/python)
import h5py
