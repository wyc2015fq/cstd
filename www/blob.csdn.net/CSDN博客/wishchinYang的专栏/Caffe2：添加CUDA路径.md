# Caffe2：添加CUDA路径 - wishchinYang的专栏 - CSDN博客
2018年01月30日 15:12:00[wishchin](https://me.csdn.net/wishchin)阅读数：1689
在使用Eclipse时候，仍然出现 libcuda.so 找不到的情况.....
原因：cuda环境没有设置到系统环境中。
方案：
设置CUDA path：
在bashrc中添加路径，只对当前用户产生影响。
`sudo gedit ~/.bashrc`
在最后行添加内容：
```
export PATH=/usr/local/cuda/bin:$PATH  
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```
保存后, 执行下列命令, 使环境变量立即生效,可以暂时不进行重启
`source ~/.bashrc`
同时需要添加lib库路径： 在 /etc/ld.so.conf.d/加入文件 cuda.conf,
`sudo gedit /etc/ld.so.conf.d/cuda.conf`
内容如下
```
/usr/local/cuda/lib64 
/lib
/usr/lib
/usr/lib32
```
保存后，执行下列命令使之立刻生效
`sudo ldconfig -v`
结果，导入成功。
