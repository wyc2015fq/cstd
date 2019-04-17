# mac 安装 neuraltalk2  无gpu - zj360202的专栏 - CSDN博客





2017年03月04日 16:13:42[zj360202](https://me.csdn.net/zj360202)阅读数：657








网上有一些安装的说明，不过都或多或少有些遗漏，特此在这里总结一些，也许有一些步骤也是多余的，不过应该不少

先上一个成果



```java
th eval.lua -model model_id1-501-1448236541.t7_cpu.t7 -image_folder ./images/ -gpuid -1  #gpuid<0表示cpu运行
```

![](https://img-blog.csdn.net/20170304155309487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





具体步骤

1.下载

`git clone --recursive https://github.com/karpathy/neuraltalk2 #代码`
2.安装torch



```
curl -s https://raw.githubusercontent.com/torch/ezinstall/master/install-deps | bash
git clone https://github.com/torch/distro.git ~/torch --recursive
cd ~/torch; ./install.sh
```
添加torch的环境变量，将以下内容写到~/.profile中去



```
source /Users/myusername/torch/install/bin/torch-activate  #myusername是你的用户名
```
保存完成后执行


`source ~/.profile`完成以上两步就算完成torch了，可以执行以下两个命令来试试



```
luarocks install image  #测试luarocks命令是否OK
th			#测试th命令是否OK
```


3.安装neuraltalk2的环境依赖



```
sudo ~/torch/install/bin/luarocks install nn
sudo ~/torch/install/bin/luarocks install nngraph
sudo ~/torch/install/bin/luarocks install image
```

```
brew install protobuf
sudo ~/torch/install/bin/luarocks install loadcaffe
```

```
brew tap homebrew/science
从HDF官网（https://support.hdfgroup.org/HDF5/）上下载hdf5-1.8.17.tar.gz
简要安装步骤如下：（详细步骤：hdf5-1.8.17/release_docs/INSTALL）
$ gunzip < hdf5-X.Y.Z.tar.gz | tar xf -   #解压缩
$ cd hdf5-X.Y.Z
$ ./configure --prefix=/usr/local/hdf5  #安装路径
$ make
$ make check                # run test suite.
$ make install
$ make check-install        # verify installation.
```
如果在上面安装过程中，有报ffi.lua不能使用hdf5-1.10.x的问题，可以去lua对应位置加上一个条件，支持1.10.x就ok了

4.模型下载
[http://cs.stanford.edu/people/karpathy/neuraltalk2/checkpoint_v1.zip](http://cs.stanford.edu/people/karpathy/neuraltalk2/checkpoint_v1.zip)    #带gpu的模型

[http://cs.stanford.edu/people/karpathy/neuraltalk2/checkpoint_v1_cpu.zip](http://cs.stanford.edu/people/karpathy/neuraltalk2/checkpoint_v1_cpu.zip) 
 非gpu模型
将checkpoint_v1.zip解压到neuraltalk2目录下

5.准备图片


```
cd neuraltalk2
mkdir images #将图片放到此目录下
```

6.生成图片描述

```
th eval.lua -model model_id1-501-1448236541.t7_cpu.t7 -image_folder ./images/ -gpuid -1 #没有gpu的读者运行此命令
cd vis
python -m SimpleHTTPServer #启动后访问[http://localhost:8000](http://localhost:8000)
```




访问结果在最开始的地方


感谢以下文章的支持

[https://ask.julyedu.com/question/7413](https://ask.julyedu.com/question/7413)

[http://blog.topspeedsnail.com/archives/594](http://blog.topspeedsnail.com/archives/594)
[http://www.th7.cn/system/lin/201611/189996.shtml](http://www.th7.cn/system/lin/201611/189996.shtml)









